//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lblockbuffer.h
// Description : lBlockBuffer class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LBLOCKBUFFER_H_INCLUDED
#define LBLOCKBUFFER_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lBlockBufferT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// Block allocated linked list buffer.
//
// Development notes (TODO) :
//   Two naming convention potins are raised,
//   * Should the class name by changed to represent the untyped byte stream
//     nature of this container. If so, other similar containers should follow
//     suit.
//   * The 'T' postfix on the base template implementation, and a typedef
//     for the default. This has been used previously to good effect but needs
//     to be applied consisently if kept.
//   Portability:
//   * Our assumption about aligning the block data to a 16 byte boundary
//     only holds if the pointer type size is 4 bytes. A template assert has
//     been added to force this issue to the foreground when porting to 64bit.
//     We should likely have a better mechnaism for specify _both_ the optimal
//     boundary and header size. This isn't the only place where such a
//     mechnaism would be useful.
//     Note that the data[xx] member of block is intentional, as it's very
//     easy for the user to understand at a glace what it means, and it's
//     simple to access. This is supposed to be a _very_ simple class.
//
template< unsigned int BLOCKSIZE >
class lBlockBufferT
{
public:

  struct Block
  {
    static const uint32 DATA_SIZE = BLOCKSIZE-16; // block size (minus header)

    Block  * nextBlock;
    uint32   usedBlockBytes;
    uint32   pad0;
    uint32   pad1;
    uint8    data[DATA_SIZE]; // 16-byte aligned
  };

  lBlockBufferT();
  ~lBlockBufferT();

  void    Init();         // Only needs to be called if object is not constructed
  void    Deinit();       // Only needs to be called if object is not destroyed
  void    FreeAllData();  // Free all resources used

  Block * GetHead();
  Block * GetTail();

  bool    AddData( const void * buf, uint32 len );

private:

  Block * AllocateNewBlock();
  Block * m_blockHead;
  Block * m_blockTail;
};

// Default instance of lBlockBufferT with 4k data blocks
//
typedef lBlockBufferT<4096> lBlockBuffer;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lBlockBufferT implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<unsigned int BLOCKSIZE>
lBlockBufferT<BLOCKSIZE>::lBlockBufferT()
{
  Init();
}

template<unsigned int BLOCKSIZE>
lBlockBufferT<BLOCKSIZE>::~lBlockBufferT()
{
  Deinit();
}

template<unsigned int BLOCKSIZE>
void lBlockBufferT<BLOCKSIZE>::Init()
{
  // Our assumption about aligning the block data to a 16 byte boundary
  // only holds if the pointer type size is 4 bytes.
  //
  lTAssert( sizeof(Block *) == 4 );

  m_blockHead = NULL;
  m_blockTail = NULL;
}

template<unsigned int BLOCKSIZE>
void lBlockBufferT<BLOCKSIZE>::Deinit()
{
  FreeAllData();
}

template<unsigned int BLOCKSIZE>
void lBlockBufferT<BLOCKSIZE>::FreeAllData()
{
  Block * block = m_blockHead;

  while( block != NULL )
  {
    Block * nextBlock = block->nextBlock;

    if( block )
    {
      deallocate(__FILE__,__FUNCTION__,__LINE__,block);
    }

    block = nextBlock;
  }

  m_blockHead = NULL;
  m_blockTail = NULL;
}

template<unsigned int BLOCKSIZE>
typename lBlockBufferT<BLOCKSIZE>::Block * lBlockBufferT<BLOCKSIZE>::GetHead()
{
  return m_blockHead;
}

template<unsigned int BLOCKSIZE>
typename lBlockBufferT<BLOCKSIZE>::Block * lBlockBufferT<BLOCKSIZE>::GetTail()
{
  return m_blockTail;
}

template<unsigned int BLOCKSIZE>
bool lBlockBufferT<BLOCKSIZE>::AddData( const void * buf, uint32 len )
{
  if( m_blockTail == NULL )
  {
    m_blockHead = m_blockTail = AllocateNewBlock();

    if( m_blockHead == NULL )
    {
      // out of memory
      return false;
    }
  }

  Block   * block   = m_blockTail;
  const uint8 * srcData = static_cast<const unsigned char*>(buf);

  while( len > 0 )
  {
    lAssert( block->usedBlockBytes <= Block::DATA_SIZE );

    uint32 availableBlockBytes = Block::DATA_SIZE - block->usedBlockBytes;

    if( availableBlockBytes == 0 )
    {
      block->nextBlock = AllocateNewBlock();

      if( block->nextBlock == NULL )
      {
        // out of memory
        return false;
      }

      block = block->nextBlock;

      m_blockTail = block;

      lAssert( block->usedBlockBytes == 0 );
      availableBlockBytes = Block::DATA_SIZE;
    }

    uint32 bytesToCopy;

    if( availableBlockBytes < len )
    {
      bytesToCopy = availableBlockBytes;
    }
    else
    {
      bytesToCopy = len;
    }

    uint8 * destData = &block->data[block->usedBlockBytes];

    memcpy( destData, srcData, bytesToCopy );

    block->usedBlockBytes      += bytesToCopy;
    srcData                    += bytesToCopy;
    len                        -= bytesToCopy;
  }

  return true;
}

template<unsigned int BLOCKSIZE>
typename lBlockBufferT<BLOCKSIZE>::Block * lBlockBufferT<BLOCKSIZE>::AllocateNewBlock()
{
  Block * block = allocate<Block>(__FILE__,__FUNCTION__, __LINE__,L_ARENA_PLATFORM,1);

  if( block != NULL)
  {
    block->nextBlock      = NULL;
    block->usedBlockBytes = 0;
  }

  return block;
}



END_L_NAMESPACE
#endif
