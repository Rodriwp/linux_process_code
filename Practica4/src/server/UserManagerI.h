#ifndef __UserManagerI_h__
#define __UserManagerI_h__

#include "UserManager.h"

namespace CallSystem
{

class UserManagerI : virtual public UserManager
{
private:
  const int MAX_NUM_CLIENT = 255;
  typedef struct client{
     unsigned int dni;
     unsigned int minutos;
     unsigned int lim;
  }client_t;
  static vector<client_t> datos_cl(MAX_NUM_CLIENT);


public:

    virtual ::Ice::Int darAlta(::Ice::Int,
                               const Ice::Current&);

    virtual ::Ice::Int comprarMinutos(::Ice::Int,
                                      const Ice::Current&);

    virtual ::Ice::Int avisarConsumo(::Ice::Int,
                                     ::Ice::Int,
                                     const Ice::Current&);
    virtual printUsers();
};

}

#endif
