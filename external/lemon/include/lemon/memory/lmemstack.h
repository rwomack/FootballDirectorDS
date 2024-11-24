//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmemstack.h
// Description : Memory stack class
// Notes       :
//
//  Allocating zero bytes from the mem stack will return you a NULL pointer.
//
//  lMemStack memStack( 0xffff ); // 64k blocks
//
//  MyFunc()
//  {
//    lMemStackMark mark( memStack );
//
//    memStack.Allocate( 100 );
//
//    mark.Free();
//  };
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LMEMSTACK_H_INCLUDED
#define LMEMSTACK_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

class lMemStackMark;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemStack - Memory stack
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lMemStack
{
public:
  lMemStack( unsigned int minimumBlockSize, bool bZeroed = false );
  ~lMemStack();

  void * Alloc( unsigned int size, unsigned int alignment = 1 );
  void   Reduce();
  void   FreeAll();

  struct Block
  {
    Block * next;
    unsigned int size;
  };

private:
  char               * m_top;
  char               * m_end;
  Block              * m_topBlock;
  Block              * m_availBlocks;
  unsigned int         m_minimumBlockSize;
  bool                 m_bZeroed;

  void CreateNewTopBlock( unsigned int size );
  void FreeBlocksAbove( Block * newTop );

  friend class lMemStackMark;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemStackMark - Memory stack mark
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lMemStackMark
{
  lMemStack        * m_stack;
  lMemStack::Block * m_block;
  char             * m_top;

public:

  lMemStackMark( lMemStack &stack )
  {
    m_stack = &stack;
    m_block = m_stack->m_topBlock;
    m_top   = m_stack->m_top;
  }

  ~lMemStackMark()
  {
    // the user must call Free() to deallocate the memory
  }

  void Free()
  {
    if( m_block != m_stack->m_topBlock )
    {
      m_stack->FreeBlocksAbove( m_block );
    }

    m_stack->m_top = m_top;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemStackScopeMark - Memory stack mark but on deconstructor does free (for use in scopes)
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lMemStackScopeMark : public lMemStackMark
{
public:
  lMemStackScopeMark( lMemStack &stack ) : lMemStackMark( stack )
  {
  }

  ~lMemStackScopeMark()
  {
    Free();
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemStack implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline lMemStack::lMemStack( unsigned int minimumBlockSize, bool bZeroed )
  : m_top(NULL),
    m_end(NULL),
    m_topBlock(NULL),
    m_availBlocks(NULL),
    m_minimumBlockSize(minimumBlockSize),
    m_bZeroed(bZeroed)
{
}

inline lMemStack::~lMemStack()
{
  FreeAll();
}

inline void lMemStack::FreeAll()
{
  FreeBlocksAbove(NULL);
  Reduce();
}

// lMemStack::Alloc
// Allocate bytes from the stack with the specified alignment
// Will return a NULL pointer if size is zero.
//
inline void * lMemStack::Alloc( unsigned int size, unsigned int alignment )
{
  if( size == 0 ) return NULL;

  // make sure alignment is suitable for lBitAlign

  lAssert( alignment > 0 );
  lAssert( (alignment&(alignment-1)) == 0 );

  // sanity check

  lAssert( m_top <= m_end );

  // get alignment top pointer

  char * ptr = lBitAlign( m_top, alignment );
  m_top = ptr + size;

  if( m_top > m_end )
  {
    // we overflowed the end of the chunk
    CreateNewTopBlock( size + alignment );

    // try again

    ptr   = lBitAlign( m_top, alignment );
    m_top = ptr + size;
  }

  return ptr;
}

// lMemStack::Reduce
// Free any unused memory
//
inline void lMemStack::Reduce()
{
  while( m_availBlocks )
  {
    Block * next = m_availBlocks->next;
    deallocate( __FILE__,__FUNCTION__,__LINE__, m_availBlocks );
    m_availBlocks = next;
  }
}

inline void lMemStack::CreateNewTopBlock( unsigned int size )
{
  Block * block = NULL;

  // try and find an existing block with the size needed.
  // note: this could end up being rather slow if we have lots of
  //       oddly sized blocks.

  Block ** search = &m_availBlocks;
  while( (*search) )
  {
    if( (*search)->size >= size )
    {
      // unlink from the available list

      block   = (*search);
      *search = (*search)->next;
      break;
    }

    search = &(*search)->next;
  }

  // if not, allocate a new block

  if( block == NULL )
  {
    unsigned int blockSize = lMax( m_minimumBlockSize, size );

    block = (Block *)allocate<char>(__FILE__,__FUNCTION__,__LINE__, L_ARENA_PLATFORM,blockSize + sizeof(Block) );
    block->size = blockSize;
  }

  // link this block into the stack and set it as top

  block->next = m_topBlock;
  m_topBlock  = block;
  m_top       = ((char *)block) + sizeof(Block);
  m_end       = m_top + block->size;

  // zero the block

  if( m_bZeroed )
  {
    memset( m_top, 0, block->size );
  }
}

inline void lMemStack::FreeBlocksAbove( Block * newTop )
{
  // passing NULL as newTop will free all blocks

  while( m_topBlock != newTop )
  {
    // move this block to the available list
    // if needed these blocks will be zeroed when CreateNewTopBlock is called

    Block * block = m_topBlock;
    m_topBlock    = m_topBlock->next;
    block->next   = m_availBlocks;
    m_availBlocks = block;
  }

  if( m_topBlock )
  {
    m_top = ((char *)m_topBlock) + sizeof(Block);
    m_end = m_top + m_topBlock->size;

    // zero the partial amount of this block unused

    if( m_bZeroed )
    {
      memset( m_top, 0, m_end - m_top );
    }
  }
  else
  {
    m_top = NULL;
    m_end = NULL;
  }
}


END_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Placement new and delete
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemStack placement new and delete
// Use lMemStackConstructor<T>::Construct and lMemStackConstructor<T>::CopyConstruct
// to avoid having to include ldbgnewoff.h and ldbgnewon.h to access placement 'new'

//#include <ldbgnewoff.h>

inline void * operator new( size_t size, L_NAMESPACE_NAME::lMemStack &memStack )
{
  return memStack.Alloc(size);
}

inline void operator delete( void *, L_NAMESPACE_NAME::lMemStack &memStack )
{
  L_UNUSED_PARAM( memStack );
  lAssertAlways( !"Cannot delete allocation constructed on a lMemStack" );
}

inline void * operator new( size_t size, L_NAMESPACE_NAME::lMemStack &memStack, unsigned int alignment )
{
  return memStack.Alloc(size,alignment);
}

inline void operator delete( void *, L_NAMESPACE_NAME::lMemStack &memStack, unsigned int alignment )
{
  L_UNUSED_PARAM( alignment );
  L_UNUSED_PARAM( memStack );
  lAssertAlways( !"Cannot delete allocation constructed on a lMemStack" );
}

BEGIN_L_NAMESPACE

template<class T> class lMemStackConstructor
{
public:

  static void Construct( lMemStack &memStack )
  {
    new( memStack ) T;
  }

  static void Construct( lMemStack &memStack, unsigned int alignment )
  {
    new( memStack, alignment ) T;
  }

  static void CopyConstruct( lMemStack &memStack, const T & src )
  {
    new( memStack ) T(src);
  }

  static void CopyConstruct( lMemStack &memStack, unsigned int alignment, const T & src )
  {
    new( memStack, alignment ) T(src);
  }
};

END_L_NAMESPACE

//#include <ldbgnewon.h>


#endif
