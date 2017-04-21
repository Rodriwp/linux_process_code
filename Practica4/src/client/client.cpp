#include "UserManager.h"
#include "AlertSystemI.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;
class Main{
private:
    static const int SHUTDOWN;
    static int status;

    //Server parameters
    static const string LOCALHOST;
    static string IP_ADDR_SERVER;
    static const string PORT_SERVER;
    //Server on client parameters
    static const string PORT_CLIENT;
    static string myip;

    virtual void * client_func(void *);
    virtual void * server_func(void *);
};

int status = 0;
Ice::CommunicatorPtr ic;
const int SHUTDOWN = 4;
const string LOCALHOST = "localhost";
//Server parameters
string IP_ADDR_SERVER = LOCALHOST;
const string PORT_SERVER = "10000";
//Server on client parameters
const string PORT_CLIENT = "10001";
string myip = LOCALHOST;
pthread_t client_pth;
pthread_t server_pth;

//ICE client
void * client_func(void *){
  try {
      Ice::ObjectPrx base = ic->stringToProxy("UserManager:default -h "+IP_ADDR_SERVER+" -p "+PORT_SERVER);
      UserManagerPrx remoteService = UserManagerPrx::checkedCast(base);
      if (!remoteService)
          throw "Invalid proxy";
      //Client code.
      while(remoteService->connect(myip,PORT_CLIENT)!=0);
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
                if(remoteService->avisarConsumo(dni,minutos,myip)==1){
                    std::cout << "El DNI: "<<dni<<"  no esta en el sistema"<<endl;
                  }
            break;
            default:
            break;
          }
      }while(option != SHUTDOWN);
      while(remoteService->disconnect(myip)!=0);
  } catch (const Ice::ConnectionRefusedException& ex) {
        cout << "CallSystem no esta disponible en estos momentos, porfavor intentelo mas tarde." << endl;
        cout << "Si el problema persiste contacte con el servicio de atencion al cliente"<<endl;
        status = 1;
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
      ic->createObjectAdapterWithEndpoints("asii_adapter","default -p "+PORT_CLIENT);
      Ice::ObjectPtr object = new AlertSystemI;
      adapter->add(object, ic->stringToIdentity("AlertSystem"+myip));
      adapter->activate();
      ic->waitForShutdown();
  } catch (const Ice::Exception& e) {
      cerr << e << endl;
      status = 1;
  } catch (const char* msg) {
      cerr << msg << endl;
      status = 1;
  }
  if (ic){
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
  if(dni == 0){
    cout << "Estas correctamente subscrito al sistema de avisos"<<endl;
  }else{
    cout << "Aviso al cliente " << dni << ": tiempo de consumo es menor a "
         <<minutesAlertThreshold<<endl;
  }
}
void
CallSystem::AlertSystemI::shutDown(const Ice::Current& current)
{
    cout << "El servidor va a apagarse en ..."<<endl;
    pthread_cancel(client_pth);
    pthread_cancel(server_pth);
}
//MAIN
int main(int argc, char* argv[])
{
  cout << "Introduzca su ip:"<<endl;
  cin >> myip;
  cout << "Introduzca la ip del servidor: "<<endl;
  cin >> IP_ADDR_SERVER;
  int ret;
  ic = Ice::initialize(argc, argv);
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
  cout << "Cliente terminado!"<<endl;
  exit(status);
}
