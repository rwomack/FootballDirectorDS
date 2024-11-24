//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsigslot.h
// Description : Signals and slots
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSIGSLOT_H_INCLUDED
#define LSIGSLOT_H_INCLUDED

#include <lemon/lapi.h>
#include "./private/lsigslot_private.h"

BEGIN_L_NAMESPACE

template<class MT_POLICY> class lSignalBase;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lHasSlots
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lHasSlots : public MT_POLICY
{
public:
  lHasSlots()
  {
  }

  lHasSlots(const lHasSlots &h)
    : MT_POLICY(h)
  {
  }

  ~lHasSlots()
  {
    DisconnectAllSlots();
  }

  void DisconnectAllSlots()
  {
    lPrivate::SigSlot::Connection<MT_POLICY> *c = m_connections.m_head;

    while ( c != static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0) )
    {
      lPrivate::SigSlot::Connection<MT_POLICY> *next = c->GetSlotsNext();

      c->DisableMethod();
      c->GetSignal()->SlotDisconnect( c );

      c = next;
    }

    m_connections.m_head = static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0);
    m_connections.m_tail = static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0);
  }


  lHasSlots & operator=(const lHasSlots &h)
  {
    return *this;
  }

private:
  void AddIncomingConnection(lPrivate::SigSlot::Connection<MT_POLICY> *c)
  {
    c->SlotsPush( m_connections );
  }

  void RemoveIncomingConnection(lPrivate::SigSlot::Connection<MT_POLICY> *c)
  {
    c->SlotsRemove( m_connections );
  }

  lPrivate::SigSlot::ConnectionList<MT_POLICY> m_connections;

  friend class lSignalBase<MT_POLICY>;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignalBase
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignalBase : public lPrivate::SigSlot::SignalInterface<MT_POLICY>
{
public:
  lSignalBase()
  {
  }

  lSignalBase(const lSignalBase &s)
  {
  }

  ~lSignalBase()
  {
    DisconnectAll();
  }


  lSignalBase & operator=(const lSignalBase &s)
  {
    return *this;
  }

protected:
  bool ConnectToSlot(lHasSlots<MT_POLICY> *target, lPrivate::SigSlot::SlotsMethodBase *method)
  {
    lPrivate::SigSlot::Connection<MT_POLICY> *c = m_connections.m_head;

    while ( c != static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0) )
    {
      if ( c->GetMethod()->IsEqualTo( method ) )
      {
        // already have a connection to this method
        return false;
      }

      c = c->GetSignalNext();
    }

    lPrivate::SigSlot::Connection<MT_POLICY> *connection = new lPrivate::SigSlot::Connection<MT_POLICY>( this, method, target );
    connection->SignalPush( m_connections );

    if( target )
    {
      target->AddIncomingConnection( connection );
    }

    return true;
  }

  void DisconnectFromSlot(lHasSlots<MT_POLICY> *target, lPrivate::SigSlot::SlotsMethodBase *method)
  {
    lPrivate::SigSlot::Connection<MT_POLICY> *c = m_connections.m_head;

    while ( c != static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0) )
    {
      lPrivate::SigSlot::Connection<MT_POLICY> *next = c->GetSignalNext();

      if ( c->GetMethod()->IsEqualTo( method ) )
      {
        // found connection to this method

        c->SignalRemove( m_connections );

        if( target )
        {
          target->RemoveIncomingConnection( c );
        }

        delete c;

        return;
      }

      c = next;
    }
  }

  virtual void SlotDisconnect(lPrivate::SigSlot::Connection<MT_POLICY> *connection)
  {
    connection->SignalRemove( m_connections );
    delete connection;
  }

  lPrivate::SigSlot::SignalEmissionList * CreateEmissionList()
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = new lPrivate::SigSlot::SignalEmissionList();

    lPrivate::SigSlot::Connection<MT_POLICY> *c = m_connections.m_head;

    while ( c != static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0) )
    {
      emissions->Push( c->GetMethod() );

      c = c->GetSignalNext();
    }

    return emissions;
  }

  void DestroyEmissionList(lPrivate::SigSlot::SignalEmissionList *emissions)
  {
    delete emissions;
  }

  void DisconnectAll()
  {

    lPrivate::SigSlot::Connection<MT_POLICY> *c = m_connections.m_head;

    while ( c != static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0) )
    {
      lPrivate::SigSlot::Connection<MT_POLICY> *next = c->GetSignalNext();
      lHasSlots<MT_POLICY> *target = c->GetTarget();

      c->SignalRemove( m_connections );

      if( target )
      {
        target->RemoveIncomingConnection( c );
      }

      delete c;

      c = next;
    }
  }

private:
  lPrivate::SigSlot::ConnectionList<MT_POLICY> m_connections;

  friend class lHasSlots<MT_POLICY>;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignal0
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignal0 : public lSignalBase<MT_POLICY>
{
public:
  lSignal0()
  {
  }

  lSignal0(const lSignal0 &s)
  {
  }

  lSignal0 & operator=(const lSignal0 &s)
  {
    return *this;
  }

  template <typename TDEST> void Connect(TDEST *dest, void (TDEST::*methodPtr)())
  {
    lPrivate::SigSlot::SlotsMethod0<TDEST> *method = new lPrivate::SigSlot::SlotsMethod0<TDEST>( dest, methodPtr );
    method->Ref();
    ConnectToSlot( dest, method );
    method->Unref();
  }

  template <typename TDEST> void Disconnect(TDEST *dest, void (TDEST::*methodPtr)())
  {
    lPrivate::SigSlot::SlotsMethod0<TDEST> *method = new lPrivate::SigSlot::SlotsMethod0<TDEST>( dest, methodPtr );
    DisconnectFromSlot( dest, method );
    delete method;
  }

  void Connect( void (*methodPtr)() )
  {
    lPrivate::SigSlot::SlotsStaticMethod0 *method = new lPrivate::SigSlot::SlotsStaticMethod0( methodPtr );
    method->Ref();
    lSignalBase<MT_POLICY>::ConnectToSlot( static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0), method );
    method->Unref();
  }

  void Disconnect( void (*methodPtr)() )
  {
    lPrivate::SigSlot::SlotsStaticMethod0 *method = new lPrivate::SigSlot::SlotsStaticMethod0( methodPtr );
    lSignalBase<MT_POLICY>::DisconnectFromSlot( static_cast<lPrivate::SigSlot::Connection<MT_POLICY> *>(0), method );
    delete method;
  }

  void operator()()
  {
    Emit();
  }

  void Emit()
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = lSignalBase<MT_POLICY>::CreateEmissionList();
    lPrivate::SigSlot::lSignalEmission *e = emissions->GetHead();

    while ( e != static_cast<lPrivate::SigSlot::lSignalEmission *>(0) )
    {
      lPrivate::SigSlot::SlotsMethod0Base *method = static_cast<lPrivate::SigSlot::SlotsMethod0Base*>( e->m_method );
      method->Invoke();
      e = e->m_next;
    }

    lSignalBase<MT_POLICY>::DestroyEmissionList( emissions );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignal1
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignal1 : public lSignalBase<MT_POLICY>
{
public:
  lSignal1()
  {
  }

  lSignal1(const lSignal1 &s)
  {
  }

  lSignal1 & operator=(const lSignal1 &s)
  {
    return *this;
  }

  template <typename TDEST> void Connect(TDEST *dest, void (TDEST::*methodPtr)(T0))
  {
    lPrivate::SigSlot::SlotsMethod1<TDEST,T0> *method = new lPrivate::SigSlot::SlotsMethod1<TDEST,T0>( dest, methodPtr );
    method->Ref();
    ConnectToSlot( dest, method );
    method->Unref();
  }

  template <typename TDEST> void Disconnect(TDEST *dest, void (TDEST::*methodPtr)(T0))
  {
    lPrivate::SigSlot::SlotsMethod1<TDEST,T0> *method = new lPrivate::SigSlot::SlotsMethod1<TDEST,T0>( dest, methodPtr );
    DisconnectFromSlot( dest, method );
    delete method;
  }

  void Connect( void (*methodPtr)(T0) )
  {
    lPrivate::SigSlot::SlotsStaticMethod1<T0> *method = new lPrivate::SigSlot::SlotsStaticMethod1<T0>( methodPtr );
    method->Ref();
    ConnectToSlot( static_cast<lHasSlots<MT_POLICY> *>(0), method );
    method->Unref();
  }

  void Disconnect( void (*methodPtr)(T0) )
  {
    lPrivate::SigSlot::SlotsStaticMethod1<T0> *method = new lPrivate::SigSlot::SlotsStaticMethod1<T0>( methodPtr );
    DisconnectFromSlot( static_cast<lHasSlots<MT_POLICY> *>(0), method );
    delete method;
  }

  void operator()(T0 a0)
  {
    Emit(a0);
  }

  void Emit(T0 a0)
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = lSignalBase<MT_POLICY>::CreateEmissionList();
    lPrivate::SigSlot::lSignalEmission *e = emissions->GetHead();

    while ( e != static_cast<lPrivate::SigSlot::lSignalEmission *>(0) )
    {
      lPrivate::SigSlot::SlotsMethod1Base<T0> *method = static_cast<lPrivate::SigSlot::SlotsMethod1Base<T0>*>( e->m_method );
      method->Invoke( a0 );
      e = e->m_next;
    }

    lSignalBase<MT_POLICY>::DestroyEmissionList( emissions );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignal2
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignal2 : public lSignalBase<MT_POLICY>
{
public:
  lSignal2()
  {
  }

  lSignal2(const lSignal2 &s)
  {
  }

  lSignal2 & operator=(const lSignal2 &s)
  {
    return *this;
  }

  template <typename TDEST> void Connect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1))
  {
    lPrivate::SigSlot::SlotsMethod2<TDEST,T0,T1> *method = new lPrivate::SigSlot::SlotsMethod2<TDEST,T0,T1>( dest, methodPtr );
    method->Ref();
    ConnectToSlot( dest, method );
    method->Unref();
  }

  template <typename TDEST> void Disconnect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1))
  {
    lPrivate::SigSlot::SlotsMethod2<TDEST,T0,T1> *method = new lPrivate::SigSlot::SlotsMethod2<TDEST,T0,T1>( dest, methodPtr );
    DisconnectFromSlot( dest, method );
    delete method;
  }

  void Connect( void (*methodPtr)(T0, T1) )
  {
    lPrivate::SigSlot::SlotsStaticMethod2<T0,T1> *method = new lPrivate::SigSlot::SlotsStaticMethod2<T0,T1>( methodPtr );
    method->Ref();
    ConnectToSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    method->Unref();
  }

  void Disconnect( void (*methodPtr)(T0, T1) )
  {
    lPrivate::SigSlot::SlotsStaticMethod2<T0,T1> *method = new lPrivate::SigSlot::SlotsStaticMethod2<T0,T1>( methodPtr );
    DisconnectFromSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    delete method;
  }

  void operator()(T0 a0, T1 a1)
  {
    Emit(a0,a1);
  }

  void Emit(T0 a0, T1 a1)
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = lSignalBase<MT_POLICY>::CreateEmissionList();
    lPrivate::SigSlot::lSignalEmission *e = emissions->GetHead();

    while ( e != static_cast<lPrivate::SigSlot::lSignalEmission *>(0) )
    {
      lPrivate::SigSlot::SlotsMethod2Base<T0,T1> *method = static_cast<lPrivate::SigSlot::SlotsMethod2Base<T0,T1>*>( e->m_method );
      method->Invoke( a0, a1 );
      e = e->m_next;
    }

    lSignalBase<MT_POLICY>::DestroyEmissionList( emissions );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignal3
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2, class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignal3 : public lSignalBase<MT_POLICY>
{
public:
  lSignal3()
  {
  }

  lSignal3(const lSignal3 &s)
  {
  }

  lSignal3 & operator=(const lSignal3 &s)
  {
    return *this;
  }

  template <typename TDEST> void Connect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1,T2))
  {
    lPrivate::SigSlot::SlotsMethod3<TDEST,T0,T1,T2> *method = new lPrivate::SigSlot::SlotsMethod3<TDEST,T0,T1,T2>( dest, methodPtr );
    method->Ref();
    ConnectToSlot( dest, method );
    method->Unref();
  }

  template <typename TDEST> void Disconnect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1,T2))
  {
    lPrivate::SigSlot::SlotsMethod3<TDEST,T0,T1,T2> *method = new lPrivate::SigSlot::SlotsMethod3<TDEST,T0,T1,T2>( dest, methodPtr );
    DisconnectFromSlot( dest, method );
    delete method;
  }

  void Connect( void (*methodPtr)(T0, T1, T2) )
  {
    lPrivate::SigSlot::SlotsStaticMethod3<T0,T1,T2> *method = new lPrivate::SigSlot::SlotsStaticMethod3<T0,T1,T2>( methodPtr );
    method->Ref();
    ConnectToSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    method->Unref();
  }

  void Disconnect( void (*methodPtr)(T0, T1, T2) )
  {
    lPrivate::SigSlot::SlotsStaticMethod3<T0,T1,T2> *method = new lPrivate::SigSlot::SlotsStaticMethod3<T0,T1,T2>( methodPtr );
    DisconnectFromSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    delete method;
  }

  void operator()(T0 a0, T1 a1, T2 a2)
  {
    Emit(a0,a1,a2);
  }

  void Emit(T0 a0, T1 a1, T2 a2)
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = lSignalBase<MT_POLICY>::CreateEmissionList();
    lPrivate::SigSlot::lSignalEmission *e = emissions->GetHead();

    while ( e != static_cast<lPrivate::SigSlot::lSignalEmission *>(0) )
    {
      lPrivate::SigSlot::SlotsMethod3Base<T0,T1,T2> *method = static_cast<lPrivate::SigSlot::SlotsMethod3Base<T0,T1,T2>*>( e->m_method );
      method->Invoke( a0, a1, a2 );
      e = e->m_next;
    }

    lSignalBase<MT_POLICY>::DestroyEmissionList( emissions );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSignal4
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <typename T0, typename T1, typename T2, typename T3, class MT_POLICY = L_SIGSLOT_DEFAULT_MT_POLICY>
class lSignal4 : public lSignalBase<MT_POLICY>
{
public:
  lSignal4()
  {
  }

  lSignal4(const lSignal4 &s)
  {
  }

  lSignal4 & operator=(const lSignal4 &s)
  {
    return *this;
  }

  template <typename TDEST> void Connect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1,T2,T3))
  {
    lPrivate::SigSlot::SlotsMethod4<TDEST,T0,T1,T2,T3> *method = new lPrivate::SigSlot::SlotsMethod4<TDEST,T0,T1,T2,T3>( dest, methodPtr );
    method->Ref();
    ConnectToSlot( dest, method );
    method->Unref();
  }

  template <typename TDEST> void Disconnect(TDEST *dest, void (TDEST::*methodPtr)(T0,T1,T2,T3))
  {
    lPrivate::SigSlot::SlotsMethod4<TDEST,T0,T1,T2,T3> *method = new lPrivate::SigSlot::SlotsMethod4<TDEST,T0,T1,T2,T3>( dest, methodPtr );
    DisconnectFromSlot( dest, method );
    delete method;
  }

  void Connect( void (*methodPtr)(T0, T1, T2, T3) )
  {
    lPrivate::SigSlot::SlotsStaticMethod4<T0,T1,T2,T3> *method = new lPrivate::SigSlot::SlotsStaticMethod4<T0,T1,T2,T3>( methodPtr );
    method->Ref();
    ConnectToSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    method->Unref();
  }

  void Disconnect( void (*methodPtr)(T0, T1, T2, T3) )
  {
    lPrivate::SigSlot::SlotsStaticMethod4<T0,T1,T2,T3> *method = new lPrivate::SigSlot::SlotsStaticMethod4<T0,T1,T2,T3>( methodPtr );
    DisconnectFromSlot( static_cast<lHasSlots<MT_POLICY> *>(0),method );
    delete method;
  }

  void operator()(T0 a0, T1 a1, T2 a2, T3 a3)
  {
    Emit(a0,a1,a2,a3);
  }

  void Emit(T0 a0, T1 a1, T2 a2, T3 a3)
  {
    lPrivate::SigSlot::SignalEmissionList *emissions = lSignalBase<MT_POLICY>::CreateEmissionList();
    lPrivate::SigSlot::lSignalEmission *e = emissions->GetHead();

    while ( e != static_cast<lPrivate::SigSlot::lSignalEmission *>(0) )
    {
      lPrivate::SigSlot::SlotsMethod4Base<T0,T1,T2,T3> *method = static_cast<lPrivate::SigSlot::SlotsMethod4Base<T0,T1,T2,T3>*>( e->m_method );
      method->Invoke( a0, a1, a2, a3 );
      e = e->m_next;
    }

    lSignalBase<MT_POLICY>::DestroyEmissionList( emissions );
  }
};

END_L_NAMESPACE

#endif

