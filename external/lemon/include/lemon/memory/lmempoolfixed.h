//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmempoolfixed.h
// Description : Fixed memory pool class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LMEMPOOLFIXED_H_INCLUDED
#define LMEMPOOLFIXED_H_INCLUDED

#include "../platform/lcore.h"
#include "../math/lmath.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemPoolFixed class - Memory object pool fixed in position + size
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// This pooling system takes a pointer and size and then manages that ram
// allowing allocations and frees to be applied. It is also possible to reset
// the state of the memory pool. No overhead is incurred directly as the allocation
// information is stored within an array in the class
//
class lMemPoolFixed
{
public:
  lMemPoolFixed()
  {
    Init( NULL, 0 );
  }

  ~lMemPoolFixed()
  {
    Deinit();
  }

  void Init( void *ptr, int32 size, int32 defaultAlignment = 1 )
  {
    m_ptr = (uint8 *)ptr;
    m_size = size;
    m_defaultAlignment = defaultAlignment;

    // set up chunk list
    m_chunkList.Empty();
    m_chunkList.Add(1);
    m_chunkList.Last().size = -size;
  }

  void Deinit()
  {
    FreeAll();
  }

  // Allocate space

  void *Alloc( int32 size, int32 alignment = -1 )
  {
    lAssert( size > 0 );
    lAssert( alignment != -1 || m_defaultAlignment != -1 );
    lAssert( m_ptr != NULL );
    lAssert( m_size > 0 );
    lAssert( m_chunkList.Size() > 0 );

    // use default alignment or not
    int alignTo = alignment == -1 ? m_defaultAlignment : alignment;

    //  Find a free chunk with the correct alignment
    int32 curChunk = 0;
    int32 pos = 0;

    while( pos < m_size )
    {
      //  Do we have a possible candidate
      if( m_chunkList(curChunk).size < 0 && lmAbs(m_chunkList(curChunk).size) >= size )
      {
        // work out aligned address
        uint32 curPtr = (uint32)m_ptr + pos;
        uint32 alignedPtr = lAlign( curPtr, alignTo );

        // does aligned pointer still fit inside this chunk?
        if( ((alignedPtr - curPtr) + size) > m_chunkList(curChunk).size )
        {
          // no
          continue;
        }

        break;
      }

      //  Advance to next chunk
      pos += lmAbs<int32>( m_chunkList(curChunk).size );
      curChunk++;
    }

    if( pos == m_size )
    {
      // ERROR: out of memory
      return NULL;
    }

    // is position valid?
    lAssert( pos >= 0 && pos < m_size );

    // is chunk valid
    lAssert( m_chunkList(curChunk).size < 0 );

    // mark chunks as allocated
    if( size == (-m_chunkList(curChunk).size) )  // exact fit?
    {
      m_chunkList(curChunk).size = -m_chunkList(curChunk).size;
    } else
    {
      // not an exact fit, add chunk
      m_chunkList.Insert( curChunk+1, 1 );
      m_chunkList(curChunk+1).size = m_chunkList(curChunk).size + size;
      m_chunkList(curChunk).size = size;
    }

    //  Return memory allocation address
    return m_ptr + pos;
  }

  // Free space

  void Free( void *ptr )
  {
    lAssert( ptr != NULL );
    lAssert( ptr >= m_ptr && ptr < m_ptr+m_size );

    uint32 ptrpos = ((uint8*)ptr) - m_ptr;
    int32 curChunk = 0;
    int32 pos = 0;

    while( pos < m_size && pos != ptrpos )
    {
      pos += lmAbs( m_chunkList( curChunk++ ).size );
    }

    lAssert( m_chunkList(curChunk).size >= 0 && pos < m_size );

    m_chunkList( curChunk ).size = -m_chunkList( curChunk ).size;

    // try to merge chunk
    if( curChunk < m_chunkList.Size()-1 && m_chunkList( curChunk+1 ).size < 0 )
    {
      m_chunkList(curChunk).size += m_chunkList( curChunk+1 ).size;
      m_chunkList.Remove( curChunk+1 );
    }

    if( curChunk > 0 && m_chunkList( curChunk-1 ).size < 0 )
    {
      m_chunkList(curChunk-1).size += m_chunkList( curChunk ).size;
      m_chunkList.Remove( curChunk );
    }
  }

  // free all memory

  void FreeAll(void)
  {
    Init( m_ptr, m_size, m_defaultAlignment );
  }

  // log state of memory
  void Log(const char * str = NULL) const
  {
  #ifdef LCORELOG_H_INCLUDED
    if( str!=NULL ) lLogFmt("%s:\n", str );
    lLogFmt("lMemPoolFixed: Base=%x, Size=%d bytes (%d byte alignment default)\n",  m_ptr, m_size, m_defaultAlignment);
    lLogFmt("  %d chunks:-\n", m_chunkList.Size());
    uint32 pos = 0;
    lLogFmt("  Abs     :Rel     : Status\n");
    for(int i = 0; i < m_chunkList.Size(); i++)
    {
      if( m_chunkList(i).size < 0 )
        lLogFmt("  %08x:%08x: FREE : %d\n", (uint32)m_ptr+pos, pos, lmAbs( m_chunkList(i).size ));
      else
        lLogFmt("  %08x:%08x: ALLOC: %d\n",  (uint32)m_ptr+pos, pos, m_chunkList(i).size);

      pos += lmAbs( m_chunkList(i).size );
    }
  #endif
  }

private:

  struct MemChunk
  {
    int32      size;
    // if positive then the size is the amount of data allocated
    // if negative then it stores the size of space available
  };

  lArray<MemChunk> m_chunkList;
  uint8          * m_ptr;
  uint32           m_size;
  int32            m_defaultAlignment;
};


END_L_NAMESPACE
#endif
