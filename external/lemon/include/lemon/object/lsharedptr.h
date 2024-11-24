//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsharedptr.h
// Description : lSharedPtr class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSHAREDPTR_H_INCLUDED
#define LSHAREDPTR_H_INCLUDED

#include "lobjectapi.h"
#include "./private/lrefhandle.h"

/*
 Notes:

 Need to do a lock-free version of lRefHandle
 lRefHandleT needs a custom allocator option, for speed.

 L_NO_NEW_EXCEPTION        - define to not catch 'new' throws and check for NULL instead.
 L_OUT_OF_MEMORY_EXCEPTION - called when 'new' returns NULL.
 L_MULTI_THREADED          - defined for multithreaded
*/

BEGIN_L_NAMESPACE

// Foward-declarations

template<typename T> class lWeakPtr;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSharedPtr - Shared pointer-to-T
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T> class lSharedPtr
{
public:

  lSharedPtr();
  ~lSharedPtr();

  template<typename TPTR> explicit lSharedPtr( TPTR * pointee );
  template<typename TPTR, typename TDELETE> explicit lSharedPtr(TPTR pointee, TDELETE _delete);

  lSharedPtr & operator=(lSharedPtr const & rhs);

  template<typename TOTHER> explicit lSharedPtr(lWeakPtr<TOTHER> const & rhs);
  template<typename TOTHER> lSharedPtr(lSharedPtr<TOTHER> const & rhs);

  lSharedPtr(lSharedPtr const & rhs);

  void Reset();
  template<typename TOTHER> void Reset(TOTHER * p);
  template<typename TPTR, typename TDELETE> void Reset(TPTR * pointee, TDELETE _delete);

  typename lPrivate::lPtrReference<T>::ref operator*() const;

  T *      operator->() const;
  T *      Get() const;
  operator bool() const;
  bool     operator!() const;
  int      GetSharedCount() const;
  void     Swap(lSharedPtr<T> & rhs);

private:

  #if !defined(L_COMPILER_GCC) || L_COMPILER_GCC_VERSION > 29500
  template<typename TOTHER> friend class lWeakPtr;
  template<typename TOTHER> friend class lSharedPtr;
  #else
  friend class lWeakPtr<T>;
  #endif

  T                     * m_pointee;
  lPrivate::lRefHandle  * m_pRefHandle;
};


// lSharedPtr implementation

template<typename T>
inline lSharedPtr<T>::lSharedPtr()
  : m_pointee(0),
    m_pRefHandle(0)
{
}

template<typename T>
inline lSharedPtr<T>::~lSharedPtr()
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->RemoveStrongReference();
  }
}

// TPTR must be a complete type
template<typename T>
template<typename TPTR>
inline lSharedPtr<T>::lSharedPtr( TPTR * pointee )
  : m_pointee( pointee ),
    m_pRefHandle(0)
{
  #ifdef L_NO_NEW_EXCEPTION

  m_pRefHandle = new lPrivate::lRefHandleT<TPTR>( pointee );

  if( m_pRefHandle == 0 )
  {
    delete pointee;
    L_OUT_OF_MEMORY_EXCEPTION();
  }

  #else

#ifdef L_NO_EXCEPTIONS
  m_pRefHandle = new lPrivate::lRefHandleT<TPTR>( pointee );
#else
  try
  {
    m_pRefHandle = new lPrivate::lRefHandleT<TPTR>( pointee );
  }
  catch( ... )
  {
    delete pointee;
    throw;
  }
#endif

  #endif
}

// TDELETE's copy constructor must not throw
template<typename T>
template<typename TPTR, typename TDELETE>
inline lSharedPtr<T>::lSharedPtr(TPTR pointee, TDELETE _delete)
  : m_pointee( pointee ),
    m_pRefHandle(0)
{
  #ifdef L_NO_NEW_EXCEPTION

  m_pRefHandle = new lPrivate::lRefHandleDeleteT<TPTR,TDELETE>( pointee, _delete );

  if( m_pRefHandle == 0 )
  {
    _delete(pointee);
    L_OUT_OF_MEMORY_EXCEPTION();
  }

  #else

#ifdef L_NO_EXCEPTIONS
  m_pRefHandle = new lPrivate::lRefHandleDeleteT<TPTR,TDELETE>( pointee, _delete );
#else
  try
  {
    m_pRefHandle = new lPrivate::lRefHandleDeleteT<TPTR,TDELETE>( pointee, _delete );
  }
  catch( ... )
  {
    _delete(pointee);
    throw;
  }
#endif

  #endif
}

template<typename T>
inline lSharedPtr<T> & lSharedPtr<T>::operator=(lSharedPtr<T> const & rhs)
{
  if( rhs.m_pRefHandle != m_pRefHandle )
  {
    // add a strong reference to the target object first.
    // we do this incase the the current object holds the last strong
    // reference to the target object, causing it to be deleted.

    if( rhs.m_pRefHandle != 0 ) rhs.m_pRefHandle->AddStrongReference();

    // then, remove the strong reference from the current object

    if( m_pRefHandle != 0 ) m_pRefHandle->RemoveStrongReference();

    // copy the ref handle

    m_pRefHandle = rhs.m_pRefHandle;
  }

  m_pointee = rhs.m_pointee;
  return *this;
}

template<typename T>
template<typename TOTHER>
inline lSharedPtr<T>::lSharedPtr(lWeakPtr<TOTHER> const & rhs)
  : m_pointee(0),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if( m_pRefHandle == 0 || !m_pRefHandle->AddStrongReferenceLock() )
  {
    // failed to lock it.. bad.
    lAssert(0);
  }

  m_pointee = rhs.m_pointee;
}

template<typename T>
template<typename TOTHER>
inline lSharedPtr<T>::lSharedPtr(lSharedPtr<TOTHER> const & rhs)
  : m_pointee(rhs.m_pointee),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->AddStrongReference();
  }
}

template<typename T>
inline lSharedPtr<T>::lSharedPtr(lSharedPtr<T> const & rhs)
  : m_pointee(rhs.m_pointee),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->AddStrongReference();
  }
}


template<typename T>
void lSharedPtr<T>::Reset()
{
  // copy-and-swap idiom
  lSharedPtr().Swap(*this);
}

// TOTHER must be complete
template<typename T>
template<typename TOTHER>
inline void lSharedPtr<T>::Reset(TOTHER * p)
{
  // todo: do we want to allow reset on same pointer?
  //  -- should work with copy-and-swap
  //if( p != NULL )
  //{
  //  // don't allow a reset on same pointer
  //  lAssert(p != m_pointee);
  //}

  // copy-and-swap idiom
  lSharedPtr(p).Swap(*this);
}

template<typename T>
template<typename TPTR, typename TDELETE>
inline void lSharedPtr<T>::Reset(TPTR * pointee, TDELETE _delete)
{
  // copy-and-swap idiom
  lSharedPtr(pointee, _delete).Swap(*this);
}

template<typename T>
inline typename lPrivate::lPtrReference<T>::ref lSharedPtr<T>::operator*() const
{
  lAssert(m_pointee != 0);
  return *m_pointee;
}

template<typename T>
inline T * lSharedPtr<T>::operator->() const // never throws
{
  lAssert(m_pointee != 0);
  return m_pointee;
}

template<typename T>
inline T * lSharedPtr<T>::Get() const // never throws
{
  return m_pointee;
}

template<typename T>
inline lSharedPtr<T>::operator bool() const
{
  return m_pointee == 0 ? false : true;
}

template<typename T>
inline bool lSharedPtr<T>::operator! () const
{
  return m_pointee == 0;
}

template<typename T>
inline int lSharedPtr<T>::GetSharedCount() const
{
  if( m_pRefHandle != 0 )
  {
    return m_pRefHandle->GetSharedCount();
  }
  else
  {
    return 0;
  }
}

template<typename T>
inline void lSharedPtr<T>::Swap(lSharedPtr<T> & rhs)
{
  lSwap(m_pointee, rhs.m_pointee);
  lPrivate::lRefHandle * pRefHandle = rhs.m_pRefHandle;
  rhs.m_pRefHandle = m_pRefHandle;
  m_pRefHandle = pRefHandle;
}

// lSharedPtr equality

template<typename T, typename TOTHER>
inline bool operator==(lSharedPtr<T> const & a, lSharedPtr<TOTHER> const & b)
{
  return a.Get() == b.Get();
}

template<typename T, typename TOTHER>
inline bool operator!=(lSharedPtr<T> const & a, lSharedPtr<TOTHER> const & b)
{
  return a.Get() != b.Get();
}


END_L_NAMESPACE
#endif
