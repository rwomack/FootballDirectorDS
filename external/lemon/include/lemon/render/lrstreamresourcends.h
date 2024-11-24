//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrstreamresourcends.h
// Description : NDS stream resource
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRSTREAMRESOURCENDS_H_INCLUDED
#define LRSTREAMRESOURCENDS_H_INCLUDED

#include "./lrstreamresourcecommon.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrMemCacheNDS - tracks lrCacheId's by their memory usage
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lrCacheIdNDS;

class LRENDER_API lrMemCacheNDS
{
public:
  lrMemCacheNDS();
  ~lrMemCacheNDS();

  void Init( unsigned int startAddress, unsigned int poolSize, unsigned int numPools, const char * name = "MemCache" );
  bool IsLocked( lrCacheIdNDS * );
  bool IsCached( lrCacheIdNDS * );
  bool Cache( lrCacheIdNDS * );
  void Free( lrCacheIdNDS * );
  void Dump();
  void Flush();
  void Update();
  void SanityCheck();

  void IncrementDrawCall();

private:
  lrCacheIdNDS * head;
  unsigned int   time;
  unsigned int   numPools;
  unsigned int   poolSize;
  unsigned int   totalSize;
  unsigned int   bandwidthIn;
  unsigned int   drawcall;

  char           name[32];
  enum           { LockCost = 0x10000000 };

  unsigned int PoolAlignAddress( int poolIndex, unsigned int absAddr, unsigned int align );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrCacheIdNDS - describe a piece of data that the renderer is tracking
//
// NOTE: This must remain a POD type with no virtual functions to ensure that
//       it functions correctly when used in textureresourcepsp
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrCacheIdNDS
{
public:
  lrCacheIdNDS() :
    size(0),
    align(0),
    loaded(false),
    address(0),
    pool(0),
    cache(NULL),
    cost(0),
    time(0),
    next(NULL),
    prev(NULL)
    {}

#ifdef L_PLATFORM_NDS
  // NOTE: To avoid linker issues if this is included in tools on Win32 we #ifdef these functions out
  ~lrCacheIdNDS()  { Flush(); }
  void Flush()     { if( cache ) cache->Free(this); else loaded = false; }
#endif

  unsigned int     size;       // size in bytes
  unsigned int     align;      // memory alignment

  bool             loaded;     // is item loaded?
  unsigned int     address;    // item address
  unsigned int     pool;       // pool index
  lrMemCacheNDS  * cache;      // cache item resides on

  unsigned int     cost;       // current cost of item
  unsigned int     time;       // last access time

  unsigned int     drawcall;   // associated drawcall

  lrCacheIdNDS   * next;       // previous item
  lrCacheIdNDS   * prev;       // next item

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrStreamNDS - basic memory object for passing linear data to the renderer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef L_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4348 ) //  warning C4348: 'lIO::lrStreamPSP' : redefinition of default parameter : parameter 2
#endif

template<class T, int S = sizeof(T)> class lrStreamNDS : public lArray<T>
{
public:

  lrStreamNDS() {}
  ~lrStreamNDS() {}

  lrStreamNDS( const lrStreamNDS& a )
  {
    lArray<T>::operator=(a);
    Dirty();
  }

  lrStreamNDS& operator=( const lrStreamNDS& a )
	{
    lArray<T>::operator=(a);
    Dirty();
    return *this;
  }

  inline lrCacheIdNDS       & CacheId()       { return m_cacheId; }
  inline const lrCacheIdNDS & CacheId() const { return m_cacheId; }
  inline const unsigned int    Stride() const  { return S; }
  inline void                  Dirty()         { }

private:
  lrCacheIdNDS m_cacheId;
};

#ifdef L_COMPILER_MSVC
#pragma warning( pop )
#endif

template<class T, int S>
inline lStream& operator<< ( lStream &ar, lrStreamNDS<T,S> &c )
{
  lArray<T> &a = c;
  ar << a;
  return ar;
}


// Index and vertex streams are the same on the PSP platform

#define lrIndexStreamNDS   lrStreamNDS
#define lrVertexStreamNDS  lrStreamNDS

END_L_NAMESPACE

#endif
