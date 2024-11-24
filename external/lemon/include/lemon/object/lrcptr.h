//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrcptr.h
// Description : lrcptr class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRCPTR_H_INCLUDED
#define LRCPTR_H_INCLUDED

#include "lobjectapi.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRcPtr - smart pointers-to-T; T must implement RC members
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// RC members needed:
// AddReference()
// RemoveReference()  - should also dispose of object when reference count is zero
// GetSharedCount()   - get current reference count

template<typename T>
class lRcPtr
{
public:
  lRcPtr( T* realPtr = 0 );
  lRcPtr( const lRcPtr& rhs );
  ~lRcPtr();

  lRcPtr& operator=(const lRcPtr& rhs);

  T* operator->() const;
  T& operator*() const;
  operator T *() const;

protected:

  T * pointee;
  void init();
};

// lRcPtr implementation

template<typename T> inline lRcPtr<T>::lRcPtr(T* realPtr) : pointee(realPtr)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  init();
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
}

template<typename T> inline lRcPtr<T>::lRcPtr(const lRcPtr &rhs) : pointee(rhs.pointee)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  init();
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
}
template<typename T> inline lRcPtr<T>::~lRcPtr()
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  if( pointee )
  {
    pointee->RemoveReference();
    pointee = NULL;
  }
}

template<typename T> inline lRcPtr<T>& lRcPtr<T>::operator=(const lRcPtr &rhs)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  if( pointee != rhs.pointee )
  {
    if( pointee )
    {
      pointee->RemoveReference();
      pointee = NULL;
    }

    pointee = rhs.pointee;
    init();
  }
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );

  return *this;
}

template<typename T> inline T* lRcPtr<T>::operator->() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return pointee;
}

template<typename T> inline T& lRcPtr<T>::operator*() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return *pointee;
}

template<typename T> inline lRcPtr<T>::operator T*() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return pointee;
}

template<typename T>
inline void lRcPtr<T>::init()
{
  if( pointee == 0 ) return;
  lAssert( pointee->GetSharedCount() >= 0 );
  pointee->AddReference();
  lAssert( pointee->GetSharedCount() >= 0 );
}


END_L_NAMESPACE
#endif
