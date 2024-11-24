//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcorememory.h
// Description : Memeory system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCOREMEMORY_H_INCLUDED
#define LCOREMEMORY_H_INCLUDED

#include "lcore.h"

//TODO: Add new calls with arenaID

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Arena IDs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "private/arenaids.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Memory
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

// Memory management

//##
// Allocate function
// Remarks: Allocate memory from the free heap. You should not
// call these functions directly, instead use lAlloc macro.
// In:
//   file - file of caller
//   function - function name of caller
//   line - line in file of caller
//   arenaID - ID of memory arena to allocate in
//   size - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes.
//
// See Also: lAlloc
//
LPLATFORM_API void * lPlatform_DebugAlloc(const char * file, const char * function, unsigned int line, enum lArenaID arenaID, size_t size );

//##
// Free function
// Remarks: Free memory and release it back to free heap. You should not
// call these functions directly, instead use lFree.
// In:
//   file - file of caller
//   function - function name of caller
//   line - line in file of caller
//   ptr - pointer to previously allocated buffer
//
// See Also: lFree
//
LPLATFORM_API void   lPlatform_DebugFree(const char * file, const char * function, unsigned int line, void *ptr );

//##
// Reallocate function
// Remarks: Re-allocate memory which may involve freeing memory it back to free heap and
// allocating from it again if the currently reserved block size is not large enough
// to fullfil the request. You should not call these functions directly, instead use lFree.
// (TODO TO CONFIRM) The arenaID passed in will only affect any new allocations required (OR)
// The resulting allocation if required will be in the arenaID specified if not already
//
// In:
//   file - file of caller
//   function - function name of caller
//   line - line in file of caller
//   arenaID - ID of memory arena to allocate in
//   ptr - pointer to previously allocated buffer
//   size - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes.
//
// See Also: lRealloc
//
LPLATFORM_API void * lPlatform_DebugRealloc(const char * file, const char * function, unsigned int line, enum lArenaID arenaID, void *ptr, size_t size);

//##
//<COMBINE lPlatform_DebugAlloc>
LPLATFORM_API void * lPlatform_ReleaseAlloc( enum lArenaID arenaID, size_t size );

//##
//<COMBINE lPlatform_DebugFree>
LPLATFORM_API void   lPlatform_ReleaseFree( void * ptr );

//##
//<COMBINE lPlatform_DebugRealloc>
LPLATFORM_API void * lPlatform_ReleaseRealloc( enum lArenaID arenaID, void *ptr, size_t size );

//##
// Dump memory leaks to a file.
// <color red>Preliminary API</color>
LPLATFORM_API void lPlatform_MemoryLeakDump();

struct LPLATFORM_API lMemStats
{
  //int commitSize;    // current size of commited memory from OS
  //int commitPeak;    // peak size of commited memory from OS

  int actualSize;    // current size of actual allocated memory
  int actualPeak;    // peak size of actual allocated memory

  int numAllocs;     // curent number of allocated memory blocks
  int peakAllocs;    // peak number of allocates at a point in time

  int totalAllocs;   // total number of allocates
  int totalReallocs; // total number of reallocates
  int totalFrees;    // total number of frees

  //float        allocTime;     // time in mseconds spent allocating
  //float        reallocTime;   // time in mseconds spent reallocating
  //float        freeTime;      // time in mseconds spent freeing

  #ifdef __cplusplus
  lMemStats operator- (const lMemStats &b)
  {
    lMemStats result;

    //result.commitSize    = commitSize    -  b.commitSize;
    //result.commitPeak    = commitPeak    -  b.commitPeak;

    result.actualSize    = actualSize    -  b.actualSize;
    result.actualPeak    = actualPeak    -  b.actualPeak;

    result.numAllocs     = numAllocs     -  b.numAllocs;
    result.peakAllocs    = peakAllocs    -  b.peakAllocs;

    result.totalAllocs   = totalAllocs   -  b.totalAllocs;
    result.totalReallocs = totalReallocs -  b.totalReallocs;
    result.totalFrees    = totalFrees    -  b.totalFrees;

    //result.allocTime     = a.allocTime     -  b.allocTime;
    //result.reallocTime   = a.reallocTime   -  b.reallocTime;
    //result.freeTime      = a.freeTime      -  b.freeTime;

    return result;
  }
  #endif

};

LPLATFORM_API void lPlatform_GetMemStats( lMemStats * );

#ifdef __cplusplus
}
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Macros
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Memory manager

#ifdef L_DEBUG
//##lAlloc
// Allocate macro
// Remarks: This macro maps to lPlatform_DebugAlloc or lPlatform_ReleaseAlloc
// depending on the compilation mode.
// NOTE: Allocations are made in the L_ARENA_GLOBAL arena
// In:
//   _size - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes.
//
#define lAlloc( _size ) lPlatform_DebugAlloc( __FILE__, __FUNCTION__, __LINE__, L_ARENA_GLOBAL, _size )
#define lAllocDbg( _file, _func, _line, _size ) lPlatform_DebugAlloc( _file, _func, _line, L_ARENA_GLOBAL, _size )

//##lAllocArena
// Allocate macro
// Remarks: This macro maps to lPlatform_DebugAlloc or lPlatform_ReleaseAlloc
// depending on the compilation mode.
// In:
//   _arenaID - arena ID to allocate inside
//   _size    - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes allocaed from the memory arena specified by <b>arenaID</b>.
//
#define lAllocArena( _arenaID, _size ) lPlatform_DebugAlloc( __FILE__, __FUNCTION__, __LINE__, _arenaID, _size )
#define lAllocArenaDbg( _file, _func, _line, _arenaID, _size ) lPlatform_DebugAlloc( _file, _func, _line, _arenaID, _size )

/*//##lAllocAligned
// Allocate aligned macro
// Remarks: This macro maps to lPlatform_DebugAllocAligned or lPlatform_ReleaseAllocAligned
// depending on the compilation mode.
// NOTE: Allocations are made in the L_ARENA_GLOBAL arena
// In:
//   _size  - number of bytes to allocate
//   _align - number of bytes to align the pointer to
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes aligned to <b>align</b> bytes.
//
#define lAllocAligned( _size, _align ) lPlatform_DebugAllocAligned( __FILE__, __FUNCTION__, __LINE__, L_ARENA_GLOBAL, _size, _align )
#define lAllocAlignedDbg( _file, _func, _line, _size, _align ) lPlatform_DebugAllocAligned( _file, _func, _line, L_ARENA_GLOBAL, _size, _align )

//##lAllocArenaAligned
// Allocate aligned macro
// Remarks: This macro maps to lPlatform_DebugAllocAligned or lPlatform_ReleaseAllocAligned
// depending on the compilation mode.
// In:
//   _arenaID - arena ID to allocate inside
//   _size  - number of bytes to allocate
//   _align - number of bytes to align the pointer to
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes aligned to <b>align</b> bytes.
//
#define lAllocArenaAligned( _arenaID, _size, _align ) lPlatform_DebugAllocAligned( __FILE__, __FUNCTION__, __LINE__, _arenaID, _size, _align )
#define lAllocArenaAlignedDbg( _file, _func, _line, _arenaID, _size, _align ) lPlatform_DebugAllocAligned( _file, _func, _line, _arenaID, _size, _align )
*/

//##lFree
// Free function
// Remarks: This macro maps to lPlatform_DebugFree or lPlatform_ReleaseFree
// depending on the compilation mode.
// In:
//   _ptr - pointer to previously allocated buffer
//
#define lFree( _ptr )   lPlatform_DebugFree( __FILE__, __FUNCTION__, __LINE__, _ptr )
#define lFreeDbg( _file, _func, _line, _ptr ) lPlatform_DebugFree( _file, _func, _line, _ptr )

//NOTE: lRealloc will use existing arena of memory if set, lReallocArena will move allocation if not in corect arena

//##lRealloc
// Reallocate function
// Remarks: This macro maps to lPlatform_DebugRealloc or lPlatform_ReleaseRealloc
// depending on the compilation mode.
// (TODO TO CONFIRM) The arenaID passed in will only affect any new allocations required (OR)
// The resulting allocation if required will be in the arenaID specified if not already
// In:
//   _ptr - pointer to previously allocated buffer
//   _size - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes.
//
#define lRealloc( _ptr, _size ) lPlatform_DebugRealloc( __FILE__, __FUNCTION__, __LINE__, L_ARENA_GLOBAL, _ptr, _size )
#define lReallocDbg( _file, _func, _line, _ptr, _size ) lPlatform_DebugRealloc( _file, _func, _line, L_ARENA_GLOBAL, _ptr, _size )

//##lReallocArena
// Reallocate function
// Remarks: This macro maps to lPlatform_DebugRealloc or lPlatform_ReleaseRealloc
// depending on the compilation mode.
// In:
//   _arenaID - arena ID to allocate inside
//   _ptr - pointer to previously allocated buffer
//   _size - number of bytes to allocate
// Out:
//   Returns a pointer to a memory buffer of <b>size</b> bytes.
//
#define lReallocArena( _arenaID, _ptr, _size ) lPlatform_DebugRealloc( __FILE__, __FUNCTION__, __LINE__, _arenaID, _ptr, _size )
#define lReallocArenaDbg( _file, _func, _line, _arenaID, _ptr, _size ) lPlatform_DebugRealloc( _file, _func, _line, _arenaID, _ptr, _size )

#else
#  define lAlloc( _size ) lPlatform_ReleaseAlloc( L_ARENA_GLOBAL, _size )
#  define lAllocArena( _arenaid, _size ) lPlatform_ReleaseAlloc( _arenaid, _size )
//#  define lAllocAligned( _size ) lPlatform_ReleaseAllocAligned( L_ARENA_GLOBAL, _size )
//#  define lAllocArenaAligned( _arenaid, _size ) lPlatform_ReleaseAllocAligned( _arenaid, _size )
#  define lFree( _ptr )   lPlatform_ReleaseFree( _ptr )
#  define lRealloc( _ptr, _size ) lPlatform_ReleaseRealloc( L_ARENA_GLOBAL, _ptr, _size )
#  define lReallocArena( _arenaid, _ptr, _size ) lPlatform_ReleaseRealloc( _arenaid, _ptr, _size )

#  define lAllocDbg( _file, _func, _line, _size ) lPlatform_DebugAlloc( _file, _func, _line, L_ARENA_GLOBAL, _size )
#  define lAllocArenaDbg( _file, _func, _line, _arenaid, _size ) lPlatform_DebugAlloc( _file, _func, _line, _arenaid, _size )
//#  define lAllocAlignedDbg( _file, _func, _line, _size ) lPlatform_DebugAllocAligned( _arenaid, _size )
//#  define lAllocArenaAlignedDbg( _file, _func, _line, _arenaid, _size ) lPlatform_DebugAllocAligned( _arenaid, _size )
#  define lFreeDbg( _file, _func, _line, _ptr ) lPlatform_DebugFree( _file, _func, _line, _ptr )
#  define lReallocDbg( _file, _func, _line, _ptr, _size ) lPlatform_DebugRealloc( _file, _func, _line, L_ARENA_GLOBAL, _ptr, _size )
#  define lReallocArenaDbg( _file, _func, _line, _arenaid, _ptr, _size ) lPlatform_DebugRealloc( _file, _func, _line, _arenaid, _ptr, _size )
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Memory utility templates routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus

BEGIN_L_NAMESPACE

class LPLATFORM_API lMemStatsMark
{
public:
  lMemStats stats;

  lMemStatsMark()
  {
    lPlatform_GetMemStats( &stats );
  }

  lMemStats Diff()
  {
    lMemStats now;
    lPlatform_GetMemStats( &now );
    return now - stats;
  }
};

class lNewPlacement { public: enum E { Val }; };

END_L_NAMESPACE

void * operator new( size_t size, L_NAMESPACE_NAME::lNewPlacement::E tag, void * ptr );
inline void * operator new( size_t, L_NAMESPACE_NAME::lNewPlacement::E, void * ptr ) { return (void *)ptr; }
inline void operator delete( void *, L_NAMESPACE_NAME::lNewPlacement::E, void * ) { }

BEGIN_L_NAMESPACE

//##
// Placement new
// Remarks: Placement new that will work regardless of a redefined '<i>operator new</i>' or <i>delete</i>.
// Example:
// <code>
// lPlacementConstructor<Type>::Construct( memoryPointer );
// lPlacementConstructor<Type>::CopyConstruct( memoryPointer, sourceObject );
// </code>
//
template<class T> class lPlacementConstructor
{
public:

  //##
  //Placement new construct
  //In: ptr - memory pointer
  //Remarks: Perform a construction using placement new into the specific memory address.
  //The memory pointed to be <i>ptr</i> should be large enough for the object.
  //Example:
  //<code>
  // lPlacementConstructor<Type>::Construct( memoryPointer );
  //</code>
  inline static void Construct( void * ptr )
  {
    ::new( lNewPlacement::Val, (void *)ptr ) T;
  }

  //##
  //Placement new copy-construct
  //In: ptr - memory pointer
  //    src - source object
  //Remarks: Perform a copy construction using placement new into the specified memory address.
  //The memory pointed to be <i>ptr</i> should be large enough for the object.
  //<code>
  // lPlacementConstructor<Type>::CopyConstruct( memoryPointer, sourceObject );
  //</code>
  inline static void CopyConstruct( void * ptr, const T & src )
  {
    ::new( lNewPlacement::Val, (void *)ptr ) T(src);
  }

  template<class TOTHER> inline static void CopyConstructOther( void * ptr, TOTHER & src )
  {
    ::new( lNewPlacement::Val, (void *)ptr ) T(src);
  }
};

// Template allocations

//##
// Template type allocation
// Remarks: Allocates a buffer of <b>count</b> * sizeof(T) bytes and return a pointer-to-T.
// <p><color red>No construction is performed for the objects</color>
// In: file - file of caller
//     function - function name of caller
//     line - line in file of caller
//     count - number of objects to allocate
// Out: Returns a pointer-to-T buffer of <b>count</b> objects.
template <class T> T * allocate(const char *file, const char *func, const unsigned int line, enum lArenaID arenaID, const unsigned int count );

template <class T> inline T * allocate(const char *file, const char *func, const unsigned int line, enum lArenaID arenaID, const unsigned int count)
{
  #ifdef L_DEBUG
  T * ptr = static_cast<T *>(lPlatform_DebugAlloc( file,func,line,arenaID,sizeof(T) * count ));
  #else
  L_UNUSED_PARAM( file );
  L_UNUSED_PARAM( func );
  L_UNUSED_PARAM( line );
  T * ptr = static_cast<T *>(lPlatform_ReleaseAlloc( arenaID,sizeof(T) * count ));
  #endif

  lAssert( ptr ); //if (!ptr) -- out of memory
	return	ptr;
}

//##
// Template type deallocation
// Remarks: Deallocate a memory allocated with allocate
// <p><color red>No destruction is performed for the objects</color>
// In: file - file of caller
//     function - function name of caller
//     line - line in file of caller
//     ptr - pointer to the previously allocated memory
template <class T> void deallocate(const char *file, const char *func, const unsigned int line, T* ptr);

template <class T> inline void deallocate(const char *file, const char *func, const unsigned int line, T* ptr)
{
  #ifdef L_DEBUG
  lPlatform_DebugFree( file,func,line, static_cast<void *>(ptr) );
  #else
  L_UNUSED_PARAM( file );
  L_UNUSED_PARAM( func );
  L_UNUSED_PARAM( line );
  lPlatform_ReleaseFree( ptr );
  #endif
}

//##
// Template type reallocation
// Remarks: Reallocates a buffer previously allocated to a size of <b>count</b> * sizeof(T) bytes and return a pointer-to-T.
// <p><color red>No construction or destruction is performed for the objects</color>
// In: file - file of caller
//     function - function name of caller
//     line - line in file of caller
//     ptr - pointer to the previously allocated memory
//     count - number of objects to allocate
// Out: Returns a pointer-to-T buffer of <b>count</b> objects.
template <class T> T * reallocate(const char *file, const char *func, const unsigned int line, enum lArenaID arenaID, T * existingPtr, const unsigned int count);

template <class T> inline T * reallocate(const char *file, const char *func, const unsigned int line, enum lArenaID arenaID, T * existingPtr, const unsigned int count)
{
  #ifdef L_DEBUG
  T * ptr = static_cast<T *>(lPlatform_DebugRealloc( file,func,line, arenaID, existingPtr, sizeof(T) * count ));
  #else
  L_UNUSED_PARAM( file );
  L_UNUSED_PARAM( func );
  L_UNUSED_PARAM( line );
  T * ptr = static_cast<T *>(lPlatform_ReleaseRealloc( arenaID, existingPtr, sizeof(T) * count ));
  #endif

  lAssert( ptr != NULL || count == 0 ); //if (!ptr && count > 0) -- out of memory

  return ptr;
}

//##
// Move object elements of an array from one buffer to another, allowing for overlap.
// Remarks: Moves object elements of an array from one buffer to another.
// The appropriate copy constructor will be called, as this routine will performs C++
// assignments between array elements. The buffers can overlap and the move will be
// performed correctly.
// In: dst - destination array
//     src - source array
//     count - number of object to move
template <class T> void moveElements(T* dst, T* src, const unsigned int count);

template <class T> inline void moveElements(T* dst, T* src, const unsigned int count)
{
	if (!count) return;

	if (dst < src)
	{
    // forwards

		for (unsigned int i = 0; i < count; ++i, ++dst, ++src)
		{
			*dst = *src;
		}
	}
	else
	{
    // backwards

		T*	s = src + count - 1;
		T*	d = dst + count - 1;
		for (unsigned int i = 0; i < count; ++i, --d, --s)
		{
			*d = *s;
		}
	}
}

END_L_NAMESPACE
#endif /*__cplusplus*/


#endif




