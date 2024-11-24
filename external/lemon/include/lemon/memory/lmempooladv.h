//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmempooladv.h
// Description : Memory pool class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LMEMPOOLADV_H_INCLUDED
#define LMEMPOOLADV_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemPoolAdv class - Memory object pool
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// T             - type of the pool items
// ChunkElements - number of elements in each chunk
//
// The pooling system will allocate chunks of items of size ChunkSize then
// give pointers to each item in a linear fashion using the Next() member.
// New chunks will be allocated on demand. Calling Reset() will repool all
// chunks and items. Calling Free() will free all memory for the chunks.
//
template<typename T, unsigned int ChunkElements> class lMemPoolAdv
{
public:
  lMemPoolAdv()
  {
    head      = static_cast<MemChunk*>(0);
    tail      = static_cast<MemChunk*>(0);
    availHead = static_cast<MemChunk*>(0);
  }

  ~lMemPoolAdv()
  {
    FreeAll();
  }

  // allocate the next 'count' items (for just one item)
  // hopefully, this is *slightly* faster than the general one.

  T * Next()
  {
    MemChunk * chunk = head;
    T        * freeptr = NULL;

    while( chunk )
    {
      if(chunk->freePtr)
      {
        freeptr        = chunk->freePtr;
        chunk->freePtr = (*(T**)(freeptr));
        break;
      }

      chunk = chunk->next;
    }

    if(freeptr)
    {
      lPlacementConstructor<T>::Construct( (void *)freeptr );
      return freeptr;
    }

    if( !tail || tail->chunkAvail < 1 ) NextChunk();
    lAssert( tail );
    lAssert( tail->chunkAvail >= 0 );
    T * ptr = &tail->chunkPtr[tail->chunkSize - tail->chunkAvail];
    lPlacementConstructor<T>::Construct( (void *)ptr );
    tail->chunkAvail --;
    return ptr;
  }

  // Free item
  void Free( T *item )
  {
    MemChunk * chunk = head;
    int itemPtr = (int)((char *)item);

    item->~T();

    while( chunk )
    {
      int chunkPtr = (int)((char *)chunk->chunkPtr);
      int offset = ((int)itemPtr) - ((int)chunkPtr);
      if((unsigned int)offset < (chunk->chunkSize*sizeof(T)))
      {
        break;
      }

      chunk = chunk->next;
    }

    if(chunk != NULL)
    {
      T *freeptr           = (T*)item;
      (*(T**)(freeptr))    = chunk->freePtr;
      chunk->freePtr = freeptr;
    }
  }

  // reset allocations

  void Reset(void)
  {
    if( tail )
    {
      // just move the used list into the avail list
      tail->next = availHead;
      availHead = head;
      head = tail = static_cast<MemChunk*>(0);
    }
    else
    {
      lAssert( !head );
    }
  }

  // free all memory

  void FreeAll(void)
  {
    Reset();
    while( availHead )
    {
      MemChunk * chunk = availHead;
      availHead = availHead->next;
      for( int i=0;i<ChunkElements;i++ ) chunk->chunkPtr[i].~T();
      deallocate<char>(__FILE__,__FUNCTION__,__LINE__,(char *)chunk);
    }
    lAssert( !head );
    lAssert( !tail );
    lAssert( !availHead );
  }

private:

  // implementation

  // get the next chunk from the available list

  void NextChunk()
  {
    // if we don't have any spare chunks, allocate a new one

    if( !availHead  ) AllocChunk();
    lAssert( availHead );

    // pull the next chunk from the available pool

    MemChunk * chunk = availHead;
    availHead = availHead->next;

    // reset the chunk watermark

    chunk->chunkAvail = chunk->chunkSize;
    chunk->freePtr    = NULL;

    if( !head )
    {
      lAssert( !tail );
      head = tail = chunk;
      chunk->next = static_cast<MemChunk*>(0);
    }
    else
    {
      lAssert( tail );
      tail->next = chunk;
      tail = chunk;
    }
  }

  // allocate a new chunk to the available list

  void AllocChunk()
  {
//    static int count=0;
//    lLog->Fmt( "Alloc chunk: %d : %d\n", ChunkElements, count++ );

    char         * data        = allocate<char>(__FILE__,__FUNCTION__,__LINE__, L_ARENA_P ATFORM,(ChunkElements*sizeof(T)) + sizeof(MemChunk) );
    MemChunk     * chunk       = (MemChunk *)data;

    chunk->chunkPtr = (T*)(data+sizeof(MemChunk));

    // this is done in Next now
/*    for(int i=0;i<ChunkElements;i++)
    {
      lPlacementConstructor<T>::Construct( (void *)&chunk->chunkPtr[i] );
    }*/

    chunk->freePtr    = NULL;
    chunk->chunkSize  = ChunkElements;
    chunk->chunkAvail = ChunkElements;
    chunk->next       = static_cast<MemChunk*>(0);

    if( !availHead )
    {
      availHead = chunk;
    }
    else
    {
      chunk->next = availHead;
      availHead = chunk;
    }
  }

  struct MemChunk
  {
    int        chunkSize;
    int        chunkAvail;
    T        * chunkPtr;
    T        * freePtr;
    MemChunk * next;
  };

  MemChunk * head;
  MemChunk * tail;
  MemChunk * availHead;
};


END_L_NAMESPACE
#endif
