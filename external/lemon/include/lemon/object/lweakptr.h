//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lweakptr.h
// Description : lWeakPtr class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LWEAKPTR_H_INCLUDED
#define LWEAKPTR_H_INCLUDED

#include "lobjectapi.h"
#include "./private/lrefhandle.h"

BEGIN_L_NAMESPACE

// Foward-declarations

template<typename T> class lSharedPtr;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lWeakPtr
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T> class lWeakPtr
{
public:

  lWeakPtr();
  ~lWeakPtr();

  template<typename TOTHER> lWeakPtr(lWeakPtr<TOTHER> const & rhs);
  template<typename TOTHER> explicit lWeakPtr(lSharedPtr<TOTHER> const & rhs);

  lWeakPtr(lWeakPtr const & rhs);
  lWeakPtr & operator=(lWeakPtr const & rhs);

  lSharedPtr<T> Lock() const;
  int           GetSharedCount() const;
  void          Reset();
  void          Swap(lWeakPtr<T> & other);

private:

  #if !defined(L_COMPILER_GCC) || L_COMPILER_GCC_VERSION > 29500
  template<typename TOTHER> friend class lWeakPtr;
  template<typename TOTHER> friend class lSharedPtr;
  #else
  friend class lSharedPtr<T>;
  #endif

  T                    * m_pointee;
  lPrivate::lRefHandle * m_pRefHandle;
};


// lWeakPtr implementation

template<typename T>
inline lWeakPtr<T>::lWeakPtr()
  : m_pointee(0),
    m_pRefHandle(0)
{
}

template<typename T>
inline lWeakPtr<T>::~lWeakPtr()
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->RemoveWeakReference();
  }
}

template<typename T>
template<typename TOTHER>
inline lWeakPtr<T>::lWeakPtr(lWeakPtr<TOTHER> const & rhs)
  : m_pointee(rhs.m_pointee),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->AddWeakReference();
  }

  // todo: should we lock here?
  //m_pointee = rhs.Lock().Get();
}

template<typename T>
template<typename TOTHER>
inline lWeakPtr<T>::lWeakPtr(lSharedPtr<TOTHER> const & rhs)
  : m_pointee(rhs.m_pointee),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->AddWeakReference();
  }
}

template<typename T>
inline lWeakPtr<T>::lWeakPtr(lWeakPtr<T> const & rhs)
  : m_pointee(rhs.m_pointee),
    m_pRefHandle(rhs.m_pRefHandle)
{
  if(m_pRefHandle != 0)
  {
    m_pRefHandle->AddWeakReference();
  }

  // todo: should we lock here?
  //m_pointee = rhs.Lock().Get();
}

template<typename T>
inline lWeakPtr<T> & lWeakPtr<T>::operator=(lWeakPtr<T> const & rhs)
{
  if( rhs.m_pRefHandle != m_pRefHandle )
  {
    // add a weak reference to the target object first.
    // we do this incase the the current object holds the last weak
    // reference to the target object, causing it to be deleted.

    if( rhs.m_pRefHandle != 0 ) rhs.m_pRefHandle->AddWeakReference();

    // then, remove the weak reference from the current object

    if( m_pRefHandle != 0 ) m_pRefHandle->RemoveWeakReference();

    // copy the ref handle

    m_pRefHandle = rhs.m_pRefHandle;
  }

  // todo: do we need to lock here?
  //m_pointee = rhs.Lock().Get();
  m_pointee = rhs.m_pointee;

  return *this;
}

template<typename T>
inline lSharedPtr<T> lWeakPtr<T>::Lock() const
{
  #ifdef L_MULTI_THREADED

  if(m_pRefHandle->GetSharedCount() == 0)
  {
    return lSharedPtr<T>();
  }

  // todo: if the copy-ctor resets instead of asserting, we're ok
  // and can skip this little snippet.
  if( m_pRefHandle == 0 || !m_pRefHandle->AddStrongReferenceLock() )
  {
    return lSharedPtr<T>();
  }
  else
  {
    lSharedPtr<T> p;
    p.m_pRefHandle = m_pRefHandle;
    p.m_pointee    = m_pointee;
  }

  #else

  if(m_pRefHandle->GetSharedCount() == 0)
  {
    return lSharedPtr<T>();
  }
  else
  {
    return lSharedPtr<T>(*this);
  }

  #endif
}

template<typename T>
inline int lWeakPtr<T>::GetSharedCount() const
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
inline void lWeakPtr<T>::Reset()
{
  lWeakPtr().Swap(*this);
}

template<typename T>
inline void lWeakPtr<T>::Swap(lWeakPtr<T> & other) // never throws
{
  lSwap(m_pointee, other.m_pointee);
  lPrivate::lRefHandle * pRefHandle = other.m_pRefHandle;
  other.m_pRefHandle = m_pRefHandle;
  m_pRefHandle = pRefHandle;
}


END_L_NAMESPACE
#endif
