
#include "AlertSystem.h"
#include <Ice/Ice.h>
#include <arpa/inet.h>

#include <UserManager.h>


/*UserManagerI.h*/
namespace CallSystem
{

class UserManagerI : virtual public UserManager
{
public:

    virtual ::Ice::Int darAlta(::Ice::Int,
                               const Ice::Current&);

    virtual ::Ice::Int comprarMinutos(::Ice::Int,
                                      ::Ice::Int,
                                      const Ice::Current&);

    virtual ::Ice::Int avisarConsumo(::Ice::Int,
                                     ::Ice::Int,
                                     const ::std::string&,
                                     const Ice::Current&);

    virtual ::Ice::Int connect(const ::std::string&,
                               const ::std::string&,
                               const Ice::Current&);

    virtual ::Ice::Int disconnect(const ::std::string&,
                                  const Ice::Current&);
};
/*End of UserManagerI.h*/
}
using namespace std;
using namespace CallSystem;

typedef struct machine_client{
     unsigned long id_machine;
     string ip;
     string port;
     pthread_t pth;
}machine_client_t;
typedef struct client{
     unsigned int dni;
     unsigned int minutos;
     unsigned int lim;
     bool aviso_flag;
     unsigned long id_machine;
}client_t;


bool cerrar_servidor = 0;
bool consumo_status = 0;
bool monitora_status = 0;
int  var_cond_cierre = 0;
pthread_t consumo_pth;
pthread_t monitora_pth;
pthread_mutex_t _mutex;
pthread_mutex_t _mutex_machines;
pthread_mutex_t _mutex_avisos;
pthread_cond_t _var_cond;
pthread_cond_t _var_cond_cierre;
static vector<client_t> datos_cl;
static vector<machine_client_t> maquinas_avisos_cl;
static vector<unsigned int> avisos_cl;

const int SHUTDOWN = 4;
int status = 0;
int ret;
Ice::CommunicatorPtr ic;

//Auxiliar functions
int searchClient(unsigned int dni){
  for(int i = 0;i < datos_cl.size();i++){
      if(datos_cl.at(i).dni == dni){
        return i;
      }
  }
  return -1;
}
void printUsers(){
  cout << "Lista de clientes:" <<endl;
  pthread_mutex_lock( &_mutex );
  for(int i = 0;i < datos_cl.size();i++){
    cout << "DNI: "<<datos_cl.at(i).dni << " | " << datos_cl.at(i).minutos << endl;
  }
  pthread_mutex_unlock( &_mutex );
}

//Hebra consumo
void * consumo_func(void *){
  while(1){
    pthread_mutex_lock( &_mutex );
    for(int i = 0;i < datos_cl.size();i++){
      if(datos_cl.at(i).minutos <= 2){
        datos_cl.at(i).minutos = 0;
      }else{
        datos_cl.at(i).minutos -= 2;
      }
    }
    pthread_mutex_unlock( &_mutex );
    sleep(4);
  }
  pthread_exit(NULL);
}
//Hebra monitora
void * monitora_func(void *){
  int unused;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&unused);
  while(1){
        pthread_mutex_lock( &_mutex );
        while(monitora_status==0){
            pthread_cond_wait(&_var_cond, &_mutex);
        }
        for(int i = 0;i < datos_cl.size();){
            if(datos_cl.at(i).minutos == 0){
                datos_cl.erase(datos_cl.begin()+i);
                if(datos_cl.size()<2){
                    monitora_status = 0;
                }
            }else if(datos_cl.at(i).minutos <= datos_cl.at(i).lim && datos_cl.at(i).aviso_flag == 1){
                datos_cl.at(i).aviso_flag = 0;
                pthread_mutex_lock(&_mutex_avisos);
                avisos_cl.push_back(datos_cl.at(i).dni);
                pthread_mutex_unlock( &_mutex_avisos);
                i++;
            }else{
                i++;
            }
          }
          pthread_mutex_unlock( &_mutex );
        }
}
//Hebra avisadora
void * avisadora_func(void * indata){
    int unused;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&unused);
    machine_client_t datos_hebra = *((machine_client_t*)indata);
    try {
        AlertSystemPrx remoteService;
        do{
            Ice::ObjectPrx base = ic->stringToProxy("AlertSystem"+datos_hebra.ip+":default -h "+datos_hebra.ip+" -p "+datos_hebra.port);
            remoteService = AlertSystemPrx::checkedCast(base);
        } while(!remoteService);
        remoteService->consumAlert(0,0);
        while(1){
            pthread_mutex_lock( &_mutex_avisos);
            if(avisos_cl.size() != 0){
                pthread_mutex_lock( &_mutex );
                int pos = searchClient(avisos_cl.back());
                if(pos == -1){
                    //Client not longer exits
                    avisos_cl.pop_back();
                    pthread_mutex_unlock( &_mutex);
                    break;
                }
                if(datos_hebra.id_machine==datos_cl.at(pos).id_machine){
                    avisos_cl.pop_back();
                    remoteService->consumAlert(datos_cl.at(pos).dni,datos_cl.at(pos).lim);

                }
                pthread_mutex_unlock( &_mutex );
            }
            pthread_mutex_unlock( &_mutex_avisos);

            if(cerrar_servidor){
                    pthread_mutex_lock( &_mutex_machines);
                    remoteService->shutDown();
                    var_cond_cierre = var_cond_cierre-1;
                    pthread_mutex_unlock( &_mutex_machines);
                    pthread_cond_signal(&_var_cond_cierre);
                    pthread_exit(NULL);
                  }
        }
    } catch (const Ice::ConnectionRefusedException& ex) {
      pthread_mutex_unlock( &_mutex );
      pthread_mutex_unlock( &_mutex_avisos);
      pthread_mutex_lock(&_mutex_machines);
      for(int i = 0;i < maquinas_avisos_cl.size();i++){
          if(maquinas_avisos_cl.at(i).id_machine == datos_hebra.id_machine){
            maquinas_avisos_cl.erase(maquinas_avisos_cl.begin()+i);
            var_cond_cierre = var_cond_cierre-1;
            if(cerrar_servidor)
            pthread_mutex_unlock( &_mutex_machines );
            if(cerrar_servidor)
                pthread_cond_signal(&_var_cond_cierre);
            pthread_exit(NULL);
          }
      }
    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    pthread_exit(NULL);
}
/*
 * Aqui implementamos las funciones declaradas como virtual
 * en la interfaz UserManagerI. Todas ellas se ejecutan distintas hebras,
 * cada vez que se llaman.
 */

::Ice::Int
CallSystem::UserManagerI::darAlta(::Ice::Int dni,
                                  const Ice::Current& current)
{
    pthread_mutex_lock( &_mutex );
    if(searchClient(dni)== -1 && dni != 0){
      client tmp;
      tmp.dni = dni;
      tmp.minutos = 10;//10 minutos de regalo para todos los nuevos clientes
      tmp.lim = 0;
      tmp.aviso_flag = 0;
      tmp.id_machine=-1;
      datos_cl.push_back(tmp);
      if(datos_cl.size()>1&& monitora_status == 0){
         monitora_status = 1;
         pthread_mutex_unlock( &_mutex );
         pthread_cond_signal(&_var_cond);
         return 0;
      }
      pthread_mutex_unlock( &_mutex );
      return 0;
    }
    pthread_mutex_unlock( &_mutex );
    return 1;
}

::Ice::Int
CallSystem::UserManagerI::comprarMinutos(::Ice::Int dni,::Ice::Int minutos,
                                         const Ice::Current& current)
{
  int pos;
  pthread_mutex_lock( &_mutex );
  if((pos = searchClient(dni))!= -1){
    datos_cl.at(pos).minutos += minutos;
    datos_cl.at(pos).aviso_flag = 1;
    pthread_mutex_unlock( &_mutex );
    return 0;
  }
  pthread_mutex_unlock( &_mutex );
  return 1;
}

::Ice::Int
CallSystem::UserManagerI::avisarConsumo(::Ice::Int dni,
                                        ::Ice::Int minutesAlertThreshold,
                                        const ::std::string& myip,
                                        const Ice::Current& current)
{
  int pos;
  unsigned long id = 0;
  if(myip.compare("localhost")){
      inet_pton(AF_INET,myip.c_str(),&id);
  }
  pthread_mutex_lock( &_mutex );
  if((pos = searchClient(dni))!= -1){
    datos_cl.at(pos).lim = minutesAlertThreshold;
    datos_cl.at(pos).aviso_flag = 1;
    datos_cl.at(pos).id_machine = id;
    pthread_mutex_unlock( &_mutex );
    return 0;
  }
  pthread_mutex_unlock( &_mutex );
  return 1;
}

::Ice::Int
CallSystem::UserManagerI::connect(const ::std::string& myip,
                                  const ::std::string& port,
                                  const Ice::Current& current)
{
    unsigned long id = 0;
    if(myip.compare("localhost")){
        inet_pton(AF_INET,myip.c_str(),&id);
    }
    pthread_mutex_lock( &_mutex_machines );
    for(int i = 0;i < maquinas_avisos_cl.size();i++){
        if(maquinas_avisos_cl.at(i).id_machine == id){
          pthread_mutex_unlock( &_mutex_machines );
          return 1;
        }
    }
    cout << "New Client Incoming with Ip: "<<id<<endl;
    machine_client_t tmp;
    tmp.id_machine = id;
    tmp.ip = myip;
    tmp.port = port;
    maquinas_avisos_cl.push_back(tmp);
    int ret = pthread_create(&(maquinas_avisos_cl.back().pth), NULL, &avisadora_func,(void*)&maquinas_avisos_cl.back());
    if(ret != 0) {
        maquinas_avisos_cl.erase(maquinas_avisos_cl.end());
        pthread_mutex_unlock( &_mutex_machines );
        printf("Error: pthread_create() failed\n");
        return 1;
    }
    pthread_mutex_unlock( &_mutex_machines );
    return 0;
}

::Ice::Int
CallSystem::UserManagerI::disconnect(const ::std::string& myip,
                                     const Ice::Current& current)
{
    unsigned long id = 0;
    if(myip.compare("localhost")){
        inet_pton(AF_INET,myip.c_str(),&id);
    }
    pthread_mutex_lock(&_mutex_machines);
    for(int i = 0;i < maquinas_avisos_cl.size();i++){
        if(maquinas_avisos_cl.at(i).id_machine == id){
          pthread_cancel(maquinas_avisos_cl.at(i).pth);
          maquinas_avisos_cl.erase(maquinas_avisos_cl.begin()+i);
          pthread_mutex_unlock( &_mutex_machines );
          return 0;
        }
    }
    pthread_mutex_unlock( &_mutex_machines );
    return 0;//The machien is not in the system, no problem
}

int main(int argc, char* argv[])
{
  pthread_mutex_init(&_mutex, NULL);
  pthread_mutex_init(&_mutex_machines, NULL);
  pthread_mutex_init(&_mutex_avisos, NULL);
  pthread_cond_init(&_var_cond, NULL);
  pthread_cond_init(&_var_cond_cierre, NULL);
  ic = Ice::initialize(argc, argv);
  try {
      Ice::ObjectAdapterPtr adapter =
      ic->createObjectAdapterWithEndpoints("asii_adapter","default -p 55555");
      Ice::ObjectPtr object = new UserManagerI;
      adapter->add(object, ic->stringToIdentity("UserManager"));
      int ret = pthread_create(&monitora_pth, NULL, &monitora_func, NULL);
      if(ret != 0) {
                    printf("Error: pthread_create() failed\n");
                    exit(EXIT_FAILURE);
      }
      adapter->activate();
      int option = 0;
      int dni = 0;
      int minutos = 0;
      do{
          cout << "\nSeleciona una opción: "<< endl;
          cout << "\t[1] Imprimir datos de clientes" << endl;
          cout << "\t[2] Arrancar simulacion de consumo" << endl;
          cout << "\t[3] Parar simulacion de consumo" << endl;
          cout << "\t[4] Apagar" << endl;
          cin >> option;
          switch(option){
            case 1:
              printUsers();
            break;
            case 2:
              if(consumo_status == 0){
                ret = pthread_create(&consumo_pth, NULL, &consumo_func, NULL);
                if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
                }
                cout << "Hebra simuladora de consumo arrancada"<< endl;
                consumo_status = 1;
              }
            break;
            case 3:
              if(consumo_status == 1){
                pthread_cancel(consumo_pth);
                cout << "Simulacion de consumo parado"<< endl;
                consumo_status = 0;
              }
            break;
            default:
            break;
          }
      }while(option != SHUTDOWN);
      //Cerrar clientes
      pthread_cancel(monitora_pth);
      pthread_join(monitora_pth, NULL);
      pthread_mutex_lock( &_mutex_machines );
      if(maquinas_avisos_cl.size()!=0){
        cout << "Avisando a los "<<maquinas_avisos_cl.size()<<" clientes de la terminacion!"<< endl;
      }
      else{
        cout << "No hay clientes conectados. Forever alone :( "<< endl;
      }
      var_cond_cierre = maquinas_avisos_cl.size();
      cerrar_servidor = 1;
      while(var_cond_cierre != 0){
          pthread_cond_wait(&_var_cond_cierre, &_mutex_machines);
        }
      if(consumo_status == 1){
        pthread_cancel(consumo_pth);
        pthread_join(consumo_pth, NULL);
      }
      //Cerrar arrays
      maquinas_avisos_cl.clear();
      avisos_cl.clear();
      datos_cl.clear();
      //liberar mutex
      pthread_mutex_destroy(&_mutex);
      pthread_mutex_destroy(&_mutex_machines);
      pthread_mutex_destroy(&_mutex_avisos);
      pthread_cond_destroy(&_var_cond);
      pthread_cond_destroy(&_var_cond_cierre);
  } catch (const Ice::Exception& e) {
      cerr << e << endl;
      status = 1;
  } catch (const char* msg) {
      cerr << msg << endl;
      status = 1;
  }
  if (ic) {
      try {
          ic->destroy();
      } catch (const Ice::Exception& e) {
          cerr << e << endl;
          status = 1;
      }
  }
  cout << "Servidor terminado!"<<endl;
  exit(status);
}
