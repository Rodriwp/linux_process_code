#include "UserManagerI.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;



int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
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

    return status;
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

    cout << dni << endl;
    return dni;
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
int searchClient(int dni){
  vector<client_t>::iterator it;
  for(it = datos_cl.begin();it < datos_cl.end();it++,i++){
    
  }
}
