#ifndef __UserManagerI_h__
#define __UserManagerI_h__

#include "UserManager.h"

namespace CallSystem
{

class UserManagerI : virtual public UserManager
{
public:

    virtual ::Ice::Int darAlta(::Ice::Int,
                               const Ice::Current&);

    virtual ::Ice::Int comprarMinutos(::Ice::Int,
                                      const Ice::Current&);

    virtual ::Ice::Int avisarConsumo(::Ice::Int,
                                     ::Ice::Int,
                                     const Ice::Current&);
};

}

#endif
