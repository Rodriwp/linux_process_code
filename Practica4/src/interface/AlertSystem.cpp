// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.1
//
// <auto-generated>
//
// Generated from file `AlertSystem.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <AlertSystem.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 305
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace
{

const ::std::string __CallSystem__AlertSystem__consumAlert_name = "consumAlert";

}
::IceProxy::Ice::Object* ::IceProxy::CallSystem::upCast(::IceProxy::CallSystem::AlertSystem* p) { return p; }

void
::IceProxy::CallSystem::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::CallSystem::AlertSystem>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::CallSystem::AlertSystem;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::CallSystem::AlertSystem::consumAlert(::Ice::Int dni, ::Ice::Int minutesAlertThreshold, const ::Ice::Context* __ctx)
{
    ::IceInternal::InvocationObserver __observer(this, __CallSystem__AlertSystem__consumAlert_name, __ctx);
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::CallSystem::AlertSystem* __del = dynamic_cast< ::IceDelegate::CallSystem::AlertSystem*>(__delBase.get());
            __del->consumAlert(dni, minutesAlertThreshold, __ctx, __observer);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, __observer);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, true, __cnt, __observer);
        }
    }
}

::Ice::AsyncResultPtr
IceProxy::CallSystem::AlertSystem::begin_consumAlert(::Ice::Int dni, ::Ice::Int minutesAlertThreshold, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __CallSystem__AlertSystem__consumAlert_name, __del, __cookie);
    try
    {
        __result->__prepare(__CallSystem__AlertSystem__consumAlert_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->__startWriteParams(::Ice::DefaultFormat);
        __os->write(dni);
        __os->write(minutesAlertThreshold);
        __result->__endWriteParams();
        __result->__send(true);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __result->__exceptionAsync(__ex);
    }
    return __result;
}

void
IceProxy::CallSystem::AlertSystem::end_consumAlert(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __CallSystem__AlertSystem__consumAlert_name);
}

const ::std::string&
IceProxy::CallSystem::AlertSystem::ice_staticId()
{
    return ::CallSystem::AlertSystem::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::CallSystem::AlertSystem::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::CallSystem::AlertSystem);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::CallSystem::AlertSystem::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::CallSystem::AlertSystem);
}

::IceProxy::Ice::Object*
IceProxy::CallSystem::AlertSystem::__newInstance() const
{
    return new AlertSystem;
}

void
IceDelegateM::CallSystem::AlertSystem::consumAlert(::Ice::Int dni, ::Ice::Int minutesAlertThreshold, const ::Ice::Context* __context, ::IceInternal::InvocationObserver& __observer)
{
    ::IceInternal::Outgoing __og(__handler.get(), __CallSystem__AlertSystem__consumAlert_name, ::Ice::Normal, __context, __observer);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(dni);
        __os->write(minutesAlertThreshold);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(__og.hasResponse())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.readEmptyParams();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateD::CallSystem::AlertSystem::consumAlert(::Ice::Int dni, ::Ice::Int minutesAlertThreshold, const ::Ice::Context* __context, ::IceInternal::InvocationObserver&)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int __p_dni, ::Ice::Int __p_minutesAlertThreshold, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_dni(__p_dni),
            _m_minutesAlertThreshold(__p_minutesAlertThreshold)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::CallSystem::AlertSystem* servant = dynamic_cast< ::CallSystem::AlertSystem*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->consumAlert(_m_dni, _m_minutesAlertThreshold, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int _m_dni;
        ::Ice::Int _m_minutesAlertThreshold;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __CallSystem__AlertSystem__consumAlert_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(dni, minutesAlertThreshold, __current);
        try
        {
            __direct.getServant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::Object* CallSystem::upCast(::CallSystem::AlertSystem* p) { return p; }

namespace
{
const ::std::string __CallSystem__AlertSystem_ids[2] =
{
    "::CallSystem::AlertSystem",
    "::Ice::Object"
};

}

bool
CallSystem::AlertSystem::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__CallSystem__AlertSystem_ids, __CallSystem__AlertSystem_ids + 2, _s);
}

::std::vector< ::std::string>
CallSystem::AlertSystem::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__CallSystem__AlertSystem_ids[0], &__CallSystem__AlertSystem_ids[2]);
}

const ::std::string&
CallSystem::AlertSystem::ice_id(const ::Ice::Current&) const
{
    return __CallSystem__AlertSystem_ids[0];
}

const ::std::string&
CallSystem::AlertSystem::ice_staticId()
{
    return __CallSystem__AlertSystem_ids[0];
}

::Ice::DispatchStatus
CallSystem::AlertSystem::___consumAlert(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::Ice::Int dni;
    ::Ice::Int minutesAlertThreshold;
    __is->read(dni);
    __is->read(minutesAlertThreshold);
    __inS.endReadParams();
    consumAlert(dni, minutesAlertThreshold, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __CallSystem__AlertSystem_all[] =
{
    "consumAlert",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

}

::Ice::DispatchStatus
CallSystem::AlertSystem::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__CallSystem__AlertSystem_all, __CallSystem__AlertSystem_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __CallSystem__AlertSystem_all)
    {
        case 0:
        {
            return ___consumAlert(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
CallSystem::AlertSystem::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
CallSystem::AlertSystem::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
CallSystem::__patch(AlertSystemPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::CallSystem::AlertSystemPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::CallSystem::AlertSystem::ice_staticId(), v);
    }
}