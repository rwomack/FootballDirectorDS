//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : rcptr.h
// Description : RcPtr<> template
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifndef __RCPTR_H
#define __RCPTR_H

#include "lresource.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// RcPtr - smart pointers-to-T; T must implement RC members
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// RC members needed:
// AddReference()
// RemoveRefernece()  - should also dispose of object when reference count is zero
// GetSharedCount()   - get current reference count

template<typename T>
class RcPtr
{
public:
  RcPtr( T* realPtr = 0 );
  RcPtr( const RcPtr& rhs );
  ~RcPtr();

  RcPtr& operator=(const RcPtr& rhs);

  T* operator->() const;
  T& operator*() const;
  operator T *() const;
  T *Get() const;

  friend lResourceFixer & operator << (lResourceFixer &fixer, RcPtr<T> &object )
  {
    fixer.FixResource( object.pointee );
    return fixer;
  }

private:
  T * pointee;
  void init();
};

// RcPtr implementation

template<typename T> inline RcPtr<T>::RcPtr(T* realPtr) : pointee(realPtr)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  init();
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
}

template<typename T> inline RcPtr<T>::RcPtr(const RcPtr &rhs) : pointee(rhs.pointee)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  init();
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
}
template<typename T> inline RcPtr<T>::~RcPtr()
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  if( pointee )
  {
    if( pointee->GetSharedCount() == 1 )
    {
      pointee->ClearSharedCount();
      if( pointee->GetResourceFlags()&lResource::RESOURCE_ALLOCATED )
      {
        delete pointee;
        pointee = NULL;
      }
    }
    else
    {
      pointee->RemoveReference();
    }
  }
}

template<typename T> inline RcPtr<T>& RcPtr<T>::operator=(const RcPtr &rhs)
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  if( pointee != rhs.pointee )
  {
    if( pointee )
    {
      if( pointee->GetSharedCount() == 1 )
      {
        pointee->ClearSharedCount();
        if( pointee->GetResourceFlags()&lResource::RESOURCE_ALLOCATED )
        {
          delete pointee;
          pointee = NULL;
        }
      }
      else
      {
        pointee->RemoveReference();
      }
    }

    pointee = rhs.pointee;
    init();
  }
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );

  return *this;
}

template<typename T> inline T* RcPtr<T>::operator->() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return pointee;
}

template<typename T> inline T& RcPtr<T>::operator*() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return *pointee;
}

template<typename T> inline RcPtr<T>::operator T*() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return pointee;
}

template<typename T> inline T* RcPtr<T>::Get() const
{
  if( pointee ) lAssert( pointee->GetSharedCount() >= 0 );
  return pointee;
}


template<typename T>
inline void RcPtr<T>::init()
{
  if( pointee == 0 ) return;
  lAssert( pointee->GetSharedCount() >= 0 );
  pointee->AddReference();
  lAssert( pointee->GetSharedCount() >= 0 );
}

END_L_NAMESPACE
#endif
