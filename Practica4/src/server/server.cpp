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
static vector<client_t> datos_cl(MAX_NUM_CLIENT);

int status = 0;
Ice::CommunicatorPtr ic;

vector<client_t>::iterator searchClient(int dni){
  vector<client_t>::iterator it;
  for(it = datos_cl.begin();it < datos_cl.end();it++){
      if(datos_cl[it].dni == dni)
        return it;
  }
  return NULL;
}
void printUsers(){
  vector<client_t>::iterator it;
  for(it = datos_cl.begin();it < datos_cl.end();it++){
    cout << "DNI: "<<datos_cl[it].dni << " | " << datos_cl[it].minutos << endl;
  }
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
          remoteService->consumAlert(20,1);
          sleep(1);
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
void * server_func(void *)
{
    try {
        Ice::ObjectAdapterPtr adapter =
        ic->createObjectAdapterWithEndpoints("asii_adapter","default -p 10000");
        Ice::ObjectPtr object = new UserManagerI;
        adapter->add(object, ic->stringToIdentity("UserManager"));
        adapter->activate();
        ic->waitForShutdown();
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
    vector<client_t>::iterator it = searchClient(dni);
    if(it = NULL){
      client tmp;
      tmp.dni = dni;
      tmp.minutos = 10;//10 minutos de regalo para todos los nuevos clientes
      tmp.lim = 0;
      it = myvector.end();
      datos_cl.insert(it,datos_cl.end,&tmp);
      return 0;
    }
    return 1;
}

::Ice::Int
CallSystem::UserManagerI::comprarMinutos(::Ice::Int dni,::Ice::Int minutos,
                                         const Ice::Current& current)
{
    return 2;
}

::Ice::Int
CallSystem::UserManagerI::avisarConsumo(::Ice::Int dni,
                                        ::Ice::Int minutesAlertThreshold,
                                        const Ice::Current& current)
{
    return 3;
}
int main(int argc, char* argv[])
{
  ic = Ice::initialize(argc, argv);

  pthread_t client_pth;
  pthread_t server_pth;

  pthread_create(&server_pth, NULL, &server_func, NULL);
  sleep(5);
  pthread_create(&client_pth, NULL, &client_func, NULL);

  pthread_join(client_pth, NULL);
  pthread_join(server_pth, NULL);

  exit(status);
}
