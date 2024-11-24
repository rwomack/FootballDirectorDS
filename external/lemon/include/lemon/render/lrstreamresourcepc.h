//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrstreamresourcepc.h
// Description : Win32 PC stream resource
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRSTREAMRESOURCEPC_H_INCLUDED
#define LRSTREAMRESOURCEPC_H_INCLUDED

#include "./lrstreamresourcecommon.h"

BEGIN_L_NAMESPACE

class lrCacheIdPC;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrMemCachePC - tracks lrCacheId's by their memory usage
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrMemCachePC
{
public:
  lrMemCachePC();
  ~lrMemCachePC();

  void Init( unsigned int startAddress, unsigned int poolSize, unsigned int numPools, const char * name = "MemCache" );
  bool IsLocked( lrCacheIdPC * );
  bool IsCached( lrCacheIdPC * );
  bool Cache( lrCacheIdPC * );
  void Free( lrCacheIdPC * );
  void Dump();
  void Flush();
  void Update();
  void SanityCheck();

private:
  lrCacheIdPC * head;
  unsigned int   time;
  unsigned int   numPools;
  unsigned int   poolSize;
  unsigned int   totalSize;
  unsigned int   bandwidthIn;

  char           name[32];
  enum           { LockCost = 0x10000000 };

  unsigned int PoolAlignAddress( int poolIndex, unsigned int absAddr, unsigned int align );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrCacheIdPC - describe a piece of data that the renderer is tracking
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrCacheIdPC
{
public:
  lrCacheIdPC() :
    size(0),
    align(0),
    loaded(false),
    address(0),
    pool(0),
    cache(NULL),
    cost(0),
    time(0),
    next(NULL),
    prev(NULL) {}

  ~lrCacheIdPC()  { Flush(); }
  void Flush()     { if( cache ) cache->Free(this); else loaded = false; }

  unsigned int     size;       // size in bytes
  unsigned int     align;      // memory alignment

  bool             loaded;     // is item loaded?
  unsigned int     address;    // item address
  unsigned int     pool;       // pool index
  lrMemCachePC * cache;      // cache item resides on

  unsigned int     cost;       // current cost of item
  unsigned int     time;       // last access time

  lrCacheIdPC  * next;       // previous item
  lrCacheIdPC  * prev;       // next item
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrStreamPC - basic memory object for passing linear data to the renderer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef L_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4348 ) //  warning C4348: 'lrStreamPC' : redefinition of default parameter : parameter 2
#endif

template<class T, int S = sizeof(T)> class lrStreamPC : public lArray<T>
{
public:

  lrStreamPC() {}
  ~lrStreamPC() {}

  lrStreamPC( const lrStreamPC& a )
  {
    lArray<T>::operator=(a);
    Dirty();
  }

  lrStreamPC& operator=( const lrStreamPC& a )
	{
    lArray<T>::operator=(a);
    Dirty();
    return *this;
  }

  inline lrCacheIdPC       & CacheId()       { return m_cacheId; }
  inline const lrCacheIdPC & CacheId() const { return m_cacheId; }
  inline const unsigned int    Stride() const  { return S; }
  inline void                  Dirty() { m_cacheId.Flush(); m_cacheId.size = sizeof(T) * Size(); }


private:
  lrCacheIdPC m_cacheId;
};

#ifdef L_COMPILER_MSVC
#pragma warning( pop )
#endif

template<class T, int S>
inline lStream& operator<< ( lStream &ar, lrStreamPC<T,S> &c )
{
  lArray<T> &a = c;
  ar << a;
  if(ar.IsLoading()) c.Dirty();
  return ar;
}

template<class T, int S>
inline lResourceFixer & operator << (lResourceFixer &fixer, lrStreamPC<T,S> &array )
{
  // call array fixer operator

  operator<< ( fixer, static_cast< lArray<T>& >(array) );

  // reset the cache id
  // (remember that we're working on a copy of this object if we're saving)

  array.CacheId().size    = sizeof(T) * array.Size();
  array.CacheId().align   = 0;
  array.CacheId().loaded  = false;
  array.CacheId().address = 0;
  array.CacheId().pool    = 0;
  array.CacheId().cache   = NULL;
  array.CacheId().cost    = 0;
  array.CacheId().time    = 0;
  array.CacheId().next    = NULL;
  array.CacheId().prev    = NULL;

  return fixer;
}

// Index and vertex streams are the same on the PC platform

#define lrIndexStreamPC   lrStreamPC
#define lrVertexStreamPC  lrStreamPC

END_L_NAMESPACE

#endif
