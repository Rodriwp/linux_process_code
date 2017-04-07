#include "UserManager.h"
#include "AlertSystemI.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;

const int SHUTDOWN = 4;
int status = 0;
Ice::CommunicatorPtr ic;

//ICE client
void * client_func(void *){
  try {

      Ice::ObjectPrx base = ic->stringToProxy("UserManager:default -h localhost -p 10000");
      UserManagerPrx remoteService = UserManagerPrx::checkedCast(base);
      if (!remoteService)
          throw "Invalid proxy";
      // your client code here!
      int option = 0;
      int dni = 0;
      int minutos = 0;
      do{
          cout << "\nSeleciona una opción: "<< endl;
          cout << "\t[1] Dar de alta" << endl;
          cout << "\t[2] Comprar minutos" << endl;
          cout << "\t[3] Avisar de consumo" << endl;
          cout << "\t[4] Apagar" << endl;
          cin >> option;
          switch(option){
            case 1:
              std::cout << "DNI: ";
              std::cin >> dni;
              if(remoteService->darAlta(dni)==1){
                  cout << "El DNI: "<<dni<<" ya esta en el sistema"<<endl;
                }
            break;
            case 2:
                cout << "DNI: ";
                cin >> dni;
                cout << "Minutos: ";
                cin >> minutos;
                if(remoteService->comprarMinutos(dni,minutos)==1){
                    cout << "El DNI: "<<dni<<" no esta en el sistema"<<endl;
                  }
            break;
            case 3:
                cout << "DNI: ";
                cin >> dni;
                cout << "Aviso de Limite de Minutos : ";
                cin >> minutos;
                if(remoteService->avisarConsumo(dni,minutos)==1){
                    std::cout << "El DNI: "<<dni<<"  no esta en el sistema"<<endl;
                  }
            break;
            default:
            break;
          }
      }while(option != SHUTDOWN);
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
//ICE server for alarmsystem
void * server_func(void *){
  try {
      Ice::ObjectAdapterPtr adapter =
      ic->createObjectAdapterWithEndpoints("asii_adapter","default -p 10001");
      Ice::ObjectPtr object = new AlertSystemI;
      adapter->add(object, ic->stringToIdentity("AlertSystem"));
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
void
CallSystem::AlertSystemI::consumAlert(::Ice::Int dni,
                                      ::Ice::Int minutesAlertThreshold,
                                      const Ice::Current& current)
{
  cout << dni <<endl;
}
int main(int argc, char* argv[])
{
  int ret;
  ic = Ice::initialize(argc, argv);

  pthread_t client_pth;
  pthread_t server_pth;

  ret = pthread_create(&server_pth, NULL, &server_func, NULL);
  if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
  }
  ret = pthread_create(&client_pth, NULL, &client_func, NULL);
  if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
  }

  pthread_join(client_pth, NULL);
  pthread_join(server_pth, NULL);
  //TODO: informar al servidor de la terminación
  exit(status);
}
