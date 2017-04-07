#include "UserManagerI.h"
#include "AlertSystem.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;

static const int MAX_NUM_CLIENT = 255;
typedef struct client{
     unsigned int dni;
     unsigned int minutos;
     unsigned int lim;
}client_t;
int consumo_status = 0;
int monitora_status = 0;
pthread_t consumo_pth;
pthread_t monitora_pth;
pthread_mutex_t _mutex;
static vector<client_t> datos_cl;

const int SHUTDOWN = 4;
int status = 0;
int ret;
Ice::CommunicatorPtr ic;

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

void * client_func(void *){
  try {
      AlertSystemPrx remoteService;
      do{
          Ice::ObjectPrx base = ic->stringToProxy("AlertSystem:default -h localhost -p 10001");
          remoteService = AlertSystemPrx::checkedCast(base);
      } while(!remoteService);
      // your client code here!
      while(1){
        printUsers();
          //remoteService->consumAlert(20,1);
          sleep(3);
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
void * monitora_func(void *){
  while(datos_cl.size()> 1){
    pthread_mutex_lock( &_mutex );
    for(int i = 0;i < datos_cl.size();i++){
      if(datos_cl.at(i).minutos == 0){
        datos_cl.erase(datos_cl.begin()+i);
        i--;
      }
    }
    pthread_mutex_unlock( &_mutex );
  }
  monitora_status == 0;
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
      datos_cl.push_back(tmp);
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
    pthread_mutex_unlock( &_mutex );
    return 0;
  }
  pthread_mutex_unlock( &_mutex );
  return 1;
}

::Ice::Int
CallSystem::UserManagerI::avisarConsumo(::Ice::Int dni,
                                        ::Ice::Int minutesAlertThreshold,
                                        const Ice::Current& current)
{
  int pos;
  pthread_mutex_lock( &_mutex );
  if((pos = searchClient(dni))!= -1){
    datos_cl.at(pos).lim = minutesAlertThreshold;
    pthread_mutex_unlock( &_mutex );
    return 0;
  }
  pthread_mutex_unlock( &_mutex );
  return 1;
}
int main(int argc, char* argv[])
{
  pthread_mutex_init(&_mutex, NULL);
  ic = Ice::initialize(argc, argv);
  try {
      Ice::ObjectAdapterPtr adapter =
      ic->createObjectAdapterWithEndpoints("asii_adapter","default -p 10000");
      Ice::ObjectPtr object = new UserManagerI;
      adapter->add(object, ic->stringToIdentity("UserManager"));
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
          if(datos_cl.size()>1&&monitora_status == 0){
            ret = pthread_create(&monitora_pth, NULL, &monitora_func, NULL);
            if(ret != 0) {
            printf("Error: pthread_create() failed\n");
            exit(EXIT_FAILURE);
            }
            monitora_status = 1;
          }
      }while(option != SHUTDOWN);
      //Cierre controlado
      pthread_mutex_destroy(&_mutex);
      if(monitora_status == 1){
        pthread_cancel(monitora_pth);
      }
      if(consumo_status == 1){
        pthread_cancel(consumo_pth);
      }
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

  exit(status);
}
