#include "UserManager.h"
#include "AlertSystemI.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;

const int SHUTDOWN = 4;
const int GETLINE_LEN = 256;
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
      char str_input[GETLINE_LEN];
      do{
          std::cout << "Please, enter your number: ";
          std::cin.getline (str_input,GETLINE_LEN);
          option = atoi(str_input);
          remoteService->darAlta(option);
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
  ic = Ice::initialize(argc, argv);

  pthread_t client_pth;
  pthread_t server_pth;

  pthread_create(&server_pth, NULL, &server_func, NULL);
  pthread_create(&client_pth, NULL, &client_func, NULL);

  pthread_join(client_pth, NULL);
  pthread_join(server_pth, NULL);

  exit(status);
}
