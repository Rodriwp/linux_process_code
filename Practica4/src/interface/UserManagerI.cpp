
#include <UserManagerI.h>

::Ice::Int
CallSystem::UserManagerI::darAlta(::Ice::Int dni,
                                  const Ice::Current& current)
{
    return 0;
}

::Ice::Int
CallSystem::UserManagerI::comprarMinutos(::Ice::Int dni,
                                         ::Ice::Int minutos,
                                         const Ice::Current& current)
{
    return 0;
}

::Ice::Int
CallSystem::UserManagerI::avisarConsumo(::Ice::Int dni,
                                        ::Ice::Int minutesAlertThreshold,
                                        const Ice::Current& current)
{
    return 0;
}

::Ice::Int
CallSystem::UserManagerI::connect(const ::std::string& myip,
                                  const ::std::string& port,
                                  const Ice::Current& current)
{
    return 0;
}

::Ice::Int
CallSystem::UserManagerI::disconnect(const ::std::string& myip,
                                     const Ice::Current& current)
{
    return 0;
}
