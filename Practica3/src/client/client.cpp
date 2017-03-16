#include "StringService.h"
#include <Ice/Ice.h>

using namespace std;
using namespace UC3M;

int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {

        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("StringService:default -h localhost -p 10000");
        StringServicePrx remoteService = StringServicePrx::checkedCast(base);
        if (!remoteService)
            throw "Invalid proxy";
        // your client code here!
        cout << "size of \"\" : "
        << remoteService->stringSize("") << endl;
        cout << "size of \"a\" : "
        << remoteService->stringSize("a") << endl;
        cout << "size of \"abcdef\" : "
        << remoteService->stringSize("abcdef") << endl;
        cout << "toUpperCase of \"abcdef\" : "
        << remoteService->toUpperCase("abcdef") << endl;
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
