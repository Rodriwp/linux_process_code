#ifndef __AlertSystemI_h__
#define __AlertSystemI_h__

#include <AlertSystem.h>

namespace CallSystem
{

class AlertSystemI : virtual public AlertSystem
{
public:

    virtual void consumAlert(::Ice::Int,
                             ::Ice::Int,
                             const Ice::Current&);
    virtual void shutDown(const Ice::Current&);
};

}

#endif
