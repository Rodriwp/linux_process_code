
#include <StringServiceI.h>

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
