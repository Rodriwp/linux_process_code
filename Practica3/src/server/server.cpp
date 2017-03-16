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
        Ice::ObjectAdapterPtr adapter =
        ic->createObjectAdapterWithEndpoints("asii_adapter","default -p 10000");
        Ice::ObjectPtr object = new StringServiceI;
        adapter->add(object, ic->stringToIdentity("StringService"));
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

::Ice::Int
UC3M::StringServiceI::stringSize(const ::std::string& s,
                                 const Ice::Current& current)
{
    return s.length();
}

::std::string
UC3M::StringServiceI::toUpperCase(const ::std::string& s,
                                  const Ice::Current& current)
{
    ::std::string ns(s);
    ::std::transform(ns.begin(), ns.end(), ns.begin(), ::toupper);
    return ns;
}
