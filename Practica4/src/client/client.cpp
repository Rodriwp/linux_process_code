#include "UserManager.h"
#include <Ice/Ice.h>

using namespace std;
using namespace CallSystem;

const int SHUTDOWN = 4;
const int GETLINE_LEN = 256;

int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {

        ic = Ice::initialize(argc, argv);
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
    return status;
}
