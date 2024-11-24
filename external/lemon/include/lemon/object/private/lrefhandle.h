//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrefhandle.h
// Description : Shared reference handle implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LREFHANDLE_H_INCLUDED
#define LREFHANDLE_H_INCLUDED

#include "../../platform/lcore.h"

BEGIN_L_NAMESPACE

// private implementation namespace
namespace lPrivate {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPtrReference - Reference type specialisation for void pointer types
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T> struct lPtrReference
{
  typedef T & ref;
};

// all reference-to-T for void use the 'void' type.

template<> struct lPtrReference<void>
{
  typedef void ref;
};

template<> struct lPtrReference<const void>
{
  typedef void ref;
};

template<> struct lPtrReference<volatile void>
{
  typedef void ref;
};

template<> struct lPtrReference<const volatile void>
{
  typedef void ref;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRefHandle - Reference to an object
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// The object being managed is destroyed (with DestroyObject) when all
// strong references are exhausted.
//
// This lRefHandle object is destroyed (with Destroy) when all weak references
// are exhausted.
//
// A single weak reference which is initialised on construction to represent
// all the strong references to the managed object and it removed when all
// strong references are exhausted.

class lRefHandle
{
  // don't allow this object to be copied

  lRefHandle( lRefHandle const & );               // intentionally left unimplemented
  lRefHandle & operator= ( lRefHandle const & );  // intentionally left unimplemented

  int m_strongRefCount;                           // references to the object
  int m_weakRefCount;                             // references to the lRefHandle (weak refs to the object)

public:

  lRefHandle();
  virtual ~lRefHandle();

  // lRefHandle interface

  virtual void DestroyObject() = 0;
  virtual void Destroy() = 0;

  // Weak references (to the lRefHandle)

  void AddWeakReference();
  void RemoveWeakReference();

  // Strong references (to the managed object)

  void AddStrongReference();
  void RemoveStrongReference();
  bool AddStrongReferenceLock();

  int  GetSharedCount() const;
};


// lRefHandle implementation

inline lRefHandle::lRefHandle() : m_strongRefCount(1), m_weakRefCount(1)
{
}

inline lRefHandle::~lRefHandle()
{
}

inline void lRefHandle::AddWeakReference()
{
  // there must already be at least one weak reference
  lAssert( m_weakRefCount != 0 );

  m_weakRefCount++;
}

inline void lRefHandle::RemoveWeakReference()
{
  // there must already be at least one weak reference
  lAssert( m_weakRefCount != 0 );

  m_weakRefCount--;

  if( m_weakRefCount == 0 )
  {
    // all weak references exhausted,
    // therefore all strong reference should be exhausted as well

    lAssert( m_strongRefCount == 0 );

    // delete ourself

    Destroy();
    return;
  }
}

// Strong references (to the managed object)

inline void lRefHandle::AddStrongReference()
{
  // there must already be at least one strong reference
  lAssert( m_strongRefCount != 0 );

  // if there's a strong reference, there should be a weak reference
  lAssert( m_weakRefCount != 0 );

  m_strongRefCount++;
}

inline void lRefHandle::RemoveStrongReference()
{
  // there must already be at least one strong reference
  lAssert( m_strongRefCount != 0 );

  // if there's a strong reference, there should be a weak reference
  lAssert( m_weakRefCount != 0 );

  m_strongRefCount--;

  if( m_strongRefCount == 0 )
  {
    // all strong references exhausted

    // destroy the object

    DestroyObject();

    // Remove the initial weak reference we created on initialisation
    // This may cause this lRefHandle to be destroyed is there are
    // no other weak references.

    RemoveWeakReference();
    return;
  }
}

inline bool lRefHandle::AddStrongReferenceLock()
{
  // there should be a weak reference
  lAssert( m_weakRefCount != 0 );

  // if there are no strong references, we cannot add one
  if( m_strongRefCount == 0 )
  {
    return false;
  }

  m_strongRefCount++;

  return true;
}

inline int lRefHandle::GetSharedCount() const
{
  return m_strongRefCount;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRefHandleT - Reference handle for T
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T>
class lRefHandleT : public lRefHandle
{
  T * m_pointee;

  // don't allow this object to be copied

  lRefHandleT( lRefHandleT const & );               // intentionally left unimplemented
  lRefHandleT & operator= ( lRefHandleT const & );  // intentionally left unimplemented

public:

  explicit lRefHandleT( T * pointee );

  // lRefHandle interface

  virtual void DestroyObject();
  virtual void Destroy();
};

// lRefHandleT implementation

template<typename T> inline lRefHandleT<T>::lRefHandleT( T * pointee ) : m_pointee( pointee )
{
}

template<typename T> inline void lRefHandleT<T>::DestroyObject()
{
  delete m_pointee;
}

template<typename T> inline void lRefHandleT<T>::Destroy()
{
  delete this;
}


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRefHandleDeleteT - Reference handle for T (with custom delete)
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename TPTR , typename TDELETE>
class lRefHandleDeleteT : public lRefHandle
{
  TPTR    m_pointee;
  TDELETE m_delete;

  // don't allow this object to be copied

  lRefHandleDeleteT( lRefHandleDeleteT const & );               // intentionally left unimplemented
  lRefHandleDeleteT & operator= ( lRefHandleDeleteT const & );  // intentionally left unimplemented

public:

  explicit lRefHandleDeleteT( TPTR pointee, TDELETE _delete );

  // lRefHandle interface

  virtual void DestroyObject();
  virtual void Destroy();
};

// lRefHandleDeleteT implementation

template<typename TPTR , typename TDELETE>
inline lRefHandleDeleteT<TPTR,TDELETE>::lRefHandleDeleteT( TPTR pointee, TDELETE _delete )
    : m_pointee( pointee ), m_delete( _delete )
{
}

template<typename TPTR , typename TDELETE>
inline void lRefHandleDeleteT<TPTR,TDELETE>::DestroyObject()
{
  m_delete( m_pointee );
}

template<typename TPTR , typename TDELETE>
inline void lRefHandleDeleteT<TPTR,TDELETE>::Destroy()
{
  delete this;
}

} // lPrivate
END_L_NAMESPACE
#endif
