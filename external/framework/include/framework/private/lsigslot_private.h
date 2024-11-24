//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsigslot_private.h
// Description : Private detail implementation for signals and slots
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSIGSLOT_PRIVATE_H_INCLUDED
#define LSIGSLOT_PRIVATE_H_INCLUDED

// TODO: implement MT policies in sig/slot classes using ScopeLock.
//       public interface will remain the same, as parameters are already
//       implemented there, but MT_POLICY will need to filter down into the
//       private classes.
//       can we provide MT lock-free operation too?
//       do Lock/Unlock in MT policy need to be virtual?

#include <lemon/lapi.h>

BEGIN_L_NAMESPACE

template<class MT_POLICY> class lHasSlots;

#define L_SIGSLOT_DEFAULT_MT_POLICY lPrivate::SigSlot::SingleThreaded

namespace lPrivate {
namespace SigSlot {

template<class MT_POLICY> class Connection;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// ScopeLock
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY>
class ScopeLock
{
public:
  MT_POLICY * m_object;

  ScopeLock(MT_POLICY * object)
    : m_object(object)
  {
    m_object->Lock();
  }

  ~ScopeLock()
  {
    m_object->Unlock();
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SingleThreaded
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class SingleThreaded
{
public:
  SingleThreaded()
  {
  }

  ~SingleThreaded()
  {
  }

  void Lock()
  {
  }

  void Unlock()
  {
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethodBase
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class SlotsMethodBase
{
public:
  SlotsMethodBase()
  {
    m_refCount = 0;
    m_bEnabled = true;
  }

  virtual ~SlotsMethodBase()
  {
  }

  void Ref()
  {
    m_refCount++;
  }

  void Unref()
  {
    m_refCount--;

    if ( m_refCount == 0 )
    {
      delete this;
    }
  }

  void Disable()
  {
    m_bEnabled = false;
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const = 0;

  // IsEqualTo uses double dispatch to compare between class methods
  // and static method.

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const = 0;
  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const = 0;

protected:

  bool IsEnabled() const
  {
    return m_bEnabled;
  }

private:
  int  m_refCount;
  bool m_bEnabled;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod0Base
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class SlotsMethod0Base : public SlotsMethodBase
{
public:
  virtual ~SlotsMethod0Base()
  {
  }

  virtual void Invoke() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod1Base
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0> class SlotsMethod1Base : public SlotsMethodBase
{
public:
  virtual ~SlotsMethod1Base()
  {
  }

  virtual void Invoke(T0 a0) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod2Base
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1> class SlotsMethod2Base : public SlotsMethodBase
{
public:
  virtual ~SlotsMethod2Base()
  {
  }

  virtual void Invoke(T0 a0, T1 a1) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod3Base
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2> class SlotsMethod3Base : public SlotsMethodBase
{
public:
  virtual ~SlotsMethod3Base()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod4Base
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2, typename T3> class SlotsMethod4Base : public SlotsMethodBase
{
public:
  virtual ~SlotsMethod4Base()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2, T3 a3) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod0
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename TDEST> class SlotsMethod0 : public SlotsMethod0Base
{
private:
  typedef SlotsMethod0<TDEST> TSELF;

public:
  typedef void (TDEST::*TMETHOD)();

  SlotsMethod0(TDEST *target, TMETHOD p) : m_target( target ), m_method( p )
  {
  }

  virtual ~SlotsMethod0()
  {
  }

  virtual void Invoke()
  {
    if ( IsEnabled() )
    {
      (m_target->*m_method)();
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToClassMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_target == m->m_target && m_method == m->m_method;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

private:
  TDEST   * m_target;
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod1
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename TDEST, typename T0> class SlotsMethod1 : public SlotsMethod1Base<T0>
{
  typedef SlotsMethod1<TDEST,T0> TSELF;

public:
  typedef void (TDEST::*TMETHOD)(T0);

  SlotsMethod1(TDEST *target, TMETHOD p) : m_target( target ), m_method( p )
  {
  }

  virtual ~SlotsMethod1()
  {
  }

  virtual void Invoke(T0 a0)
  {
    if ( SlotsMethod1Base<T0>::IsEnabled() )
    {
      (m_target->*m_method)( a0 );
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToClassMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_target == m->m_target && m_method == m->m_method;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

private:
  TDEST   * m_target;
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod2
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename TDEST, typename T0, typename T1> class SlotsMethod2 : public SlotsMethod2Base<T0,T1>
{
  typedef SlotsMethod2<TDEST,T0,T1> TSELF;

public:
  typedef void (TDEST::*TMETHOD)(T0, T1);

  SlotsMethod2(TDEST *target, TMETHOD p) : m_target( target ), m_method( p )
  {
  }

  virtual ~SlotsMethod2()
  {
  }

  virtual void Invoke(T0 a0, T1 a1)
  {
    if ( SlotsMethod2Base<T0,T1>::IsEnabled() )
    {
      (m_target->*m_method)( a0, a1 );
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToClassMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_target == m->m_target && m_method == m->m_method;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

private:
  TDEST   * m_target;
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod3
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename TDEST, typename T0, typename T1, typename T2> class SlotsMethod3 : public SlotsMethod3Base<T0,T1,T2>
{
  typedef SlotsMethod3<TDEST,T0,T1,T2> TSELF;

public:
  typedef void (TDEST::*TMETHOD)(T0, T1, T2);

  SlotsMethod3(TDEST *target, TMETHOD p) : m_target( target ), m_method( p )
  {
  }

  virtual ~SlotsMethod3()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2)
  {
    if ( SlotsMethod3Base<T0,T1,T2>::IsEnabled() )
    {
      (m_target->*m_method)( a0, a1, a2 );
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToClassMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );

    return m_target == m->m_target && m_method == m->m_method;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

private:
  TDEST   * m_target;
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsMethod4
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename TDEST, typename T0, typename T1, typename T2, typename T3> class SlotsMethod4 : public SlotsMethod4Base<T0,T1,T2,T3>
{
  typedef SlotsMethod4<TDEST,T0,T1,T2,T3> TSELF;

public:
  typedef void (TDEST::*TMETHOD)(T0, T1, T2, T3);

  SlotsMethod4(TDEST *target, TMETHOD p) : m_target( target ), m_method( p )
  {
  }

  virtual ~SlotsMethod4()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2, T3 a3)
  {
    if ( SlotsMethod4Base<T0,T1,T2,T3>::IsEnabled() )
    {
      (m_target->*m_method)( a0, a1, a2, a3 );
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToClassMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_target == m->m_target && m_method == m->m_method;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

private:
  TDEST   * m_target;
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsStaticMethod0
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class SlotsStaticMethod0 : public SlotsMethod0Base
{
  typedef SlotsStaticMethod0 TSELF;

public:
  typedef void (*TMETHOD)();

  SlotsStaticMethod0(TMETHOD p) : m_method( p )
  {
  }

  virtual ~SlotsStaticMethod0()
  {
  }

  virtual void Invoke()
  {
    if ( IsEnabled() )
    {
      (*m_method)();
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToStaticMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_method == m->m_method;
  }

private:
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsStaticMethod1
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0> class SlotsStaticMethod1 : public SlotsMethod1Base<T0>
{
  typedef SlotsStaticMethod1<T0> TSELF;

public:
  typedef void (*TMETHOD)(T0);

  SlotsStaticMethod1(TMETHOD p) : m_method( p )
  {
  }

  virtual ~SlotsStaticMethod1()
  {
  }

  virtual void Invoke(T0 a0)
  {
    if ( SlotsMethod1Base<T0>::IsEnabled() )
    {
      (*m_method)(a0);
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToStaticMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_method == m->m_method;
  }

private:
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsStaticMethod2
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1> class SlotsStaticMethod2 : public SlotsMethod2Base<T0,T1>
{
  typedef SlotsStaticMethod2<T0,T1> TSELF;

public:
  typedef void (*TMETHOD)(T0, T1);

  SlotsStaticMethod2(TMETHOD p) : m_method( p )
  {
  }

  virtual ~SlotsStaticMethod2()
  {
  }

  virtual void Invoke(T0 a0, T1 a1)
  {
    if ( SlotsMethod2Base<T0,T1>::IsEnabled() )
    {
      (*m_method)(a0,a1);
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToStaticMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_method == m->m_method;
  }

private:
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsStaticMethod3
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2> class SlotsStaticMethod3 : public SlotsMethod3Base<T0,T1,T2>
{
  typedef SlotsStaticMethod3<T0,T1,T2> TSELF;

public:
  typedef void (*TMETHOD)(T0,T1,T2);

  SlotsStaticMethod3(TMETHOD p) : m_method( p )
  {
  }

  virtual ~SlotsStaticMethod3()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2)
  {
    if ( SlotsMethod3Base<T0,T1,T2>::IsEnabled() )
    {
      (*m_method)(a0,a1,a2);
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToStaticMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_method == m->m_method;
  }

private:
  TMETHOD   m_method;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SlotsStaticMethod4
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2, typename T3> class SlotsStaticMethod4 : public SlotsMethod4Base<T0,T1,T2,T3>
{
  typedef SlotsStaticMethod4<T0,T1,T2,T3> TSELF;

public:
  typedef void (*TMETHOD)(T0, T1, T2, T3);

  SlotsStaticMethod4(TMETHOD p) : m_method( p )
  {
  }

  virtual ~SlotsStaticMethod4()
  {
  }

  virtual void Invoke(T0 a0, T1 a1, T2 a2, T3 a3)
  {
    if ( SlotsMethod4Base<T0,T1,T2,T3>::IsEnabled() )
    {
      (*m_method)(a0,a1,a2,a3);
    }
  }

  virtual bool IsEqualTo(const SlotsMethodBase *method) const
  {
    return method->DispatchEqualToStaticMethod( this );
  }

  virtual bool DispatchEqualToClassMethod(const SlotsMethodBase *method) const
  {
    L_UNUSED_PARAM( method );
    return false;
  }

  virtual bool DispatchEqualToStaticMethod(const SlotsMethodBase *method) const
  {
    const TSELF *m = static_cast<const TSELF*>( method );
    return m_method == m->m_method;
  }

private:
  TMETHOD   m_method;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SignalInterface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class SignalInterface
{
public:
  virtual void SlotDisconnect(Connection<MT_POLICY> *c) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// ConnectionList
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class ConnectionList
{
public:
  ConnectionList()
  {
    m_head = m_tail = static_cast<Connection<MT_POLICY> *>(0);
  }

  Connection<MT_POLICY> * m_head;
  Connection<MT_POLICY> * m_tail;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Connection
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class Connection
{
public:
  Connection(SignalInterface<MT_POLICY> *signal, SlotsMethodBase *method, lHasSlots<MT_POLICY> *target)
  {
    m_target = target;
    m_signal = signal;
    m_method = method;
    m_method->Ref();
    m_signalPrev = m_signalNext = static_cast<Connection *>(0);
    m_slotsPrev = m_slotsNext = static_cast<Connection *>(0);
  }

  ~Connection()
  {
    m_method->Unref();
  }

  SignalInterface<MT_POLICY> * GetSignal()
  {
    return m_signal;
  }

  SlotsMethodBase * GetMethod()
  {
    return m_method;
  }

  lHasSlots<MT_POLICY> * GetTarget()
  {
    return m_target;
  }

  void DisableMethod()
  {
    m_method->Disable();
  }

  void SignalPush(ConnectionList<MT_POLICY> &cl)
  {
    if ( cl.m_head == static_cast<Connection *>(0) )
    {
      cl.m_head = this;
    }

    if ( cl.m_tail != static_cast<Connection *>(0) )
    {
      cl.m_tail->m_signalNext = this;
    }

    m_signalPrev = cl.m_tail;

    cl.m_tail = this;
  }

  void SignalRemove(ConnectionList<MT_POLICY> &cl)
  {
    if ( cl.m_head == this )
    {
      cl.m_head = m_signalNext;
    }

    if ( cl.m_tail == this )
    {
      cl.m_tail = m_signalPrev;
    }

    if ( m_signalPrev != static_cast<Connection *>(0) )
    {
      m_signalPrev->m_signalNext = m_signalNext;
    }

    if ( m_signalNext != static_cast<Connection *>(0) )
    {
      m_signalNext->m_signalPrev = m_signalPrev;
    }

    m_signalPrev = m_signalNext = static_cast<Connection *>(0);
  }

  Connection * GetSignalNext()
  {
    return m_signalNext;
  }

  void SlotsPush(ConnectionList<MT_POLICY> &cl)
  {
    if ( cl.m_head == static_cast<Connection *>(0) )
    {
      cl.m_head = this;
    }

    if ( cl.m_tail != static_cast<Connection *>(0) )
    {
      cl.m_tail->m_slotsNext = this;
    }

    m_slotsPrev = cl.m_tail;

    cl.m_tail = this;
  }

  void SlotsRemove(ConnectionList<MT_POLICY> &cl)
  {
    if ( cl.m_head == this )
    {
      cl.m_head = m_slotsNext;
    }

    if ( cl.m_tail == this )
    {
      cl.m_tail = m_slotsPrev;
    }

    if ( m_slotsPrev != static_cast<Connection *>(0) )
    {
      m_slotsPrev->m_slotsNext = m_slotsNext;
    }

    if ( m_slotsNext != static_cast<Connection *>(0) )
    {
      m_slotsNext->m_slotsPrev = m_slotsPrev;
    }

    m_slotsPrev = m_slotsNext = static_cast<Connection *>(0);
  }

  Connection * GetSlotsNext()
  {
    return m_slotsNext;
  }

private:

  lHasSlots<MT_POLICY>       * m_target;
  SignalInterface<MT_POLICY> * m_signal;
  SlotsMethodBase * m_method;

  Connection * m_signalPrev;
  Connection * m_signalNext;
  Connection * m_slotsPrev;
  Connection * m_slotsNext;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignalEmission
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lSignalEmission
{
public:
  lSignalEmission(SlotsMethodBase *method)
  {
    m_method = method;
    m_method->Ref();
    m_next = static_cast<lSignalEmission *>(0);
  }

  ~lSignalEmission()
  {
    m_method->Unref();
  }

  SlotsMethodBase *m_method;
  lSignalEmission *m_next;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// SignalEmissionList
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class SignalEmissionList
{
public:
  SignalEmissionList()
  {
    m_head = m_tail = static_cast<lSignalEmission *>(0);;
  }

  ~SignalEmissionList()
  {
    lSignalEmission *e = m_head;

    while ( e != static_cast<lSignalEmission *>(0) )
    {
      lSignalEmission *next = e->m_next;

      delete e;

      e = next;
    }
  }

  void Push(SlotsMethodBase *method)
  {
    lSignalEmission *e = new lSignalEmission( method );

    if ( m_head == static_cast<lSignalEmission *>(0) )
    {
      m_head = e;
    }

    if ( m_tail != static_cast<lSignalEmission *>(0) )
    {
      m_tail->m_next = e;
    }

    m_tail = e;
  }

  lSignalEmission * GetHead()
  {
    return m_head;
  }


private:
  lSignalEmission * m_tail;
  lSignalEmission * m_head;
};

} // SigSlot
} // lPrivate

END_L_NAMESPACE

#endif

