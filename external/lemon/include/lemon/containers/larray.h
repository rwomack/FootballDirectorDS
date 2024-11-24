//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : larray.h
// Description : lArray classes
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LARRAY_H_INCLUDED
#define LARRAY_H_INCLUDED

#include "../platform/lcore.h"
#include "../io/lstream.h"
//#include "lresource.h"

#ifdef L_PLATFORM_PS2
#include <malloc.h>
#endif

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Notes
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//
// TODO! faster Sort using comparison operators
// TODO! See notes about Empty() and Clear()
// TODO! and using Empty() to reserve space, is this right and propper?
//----------------------------------------------------------------------------
// TODO!! Test copy-ctor and operator= for lmMatrix!!!!! Template versions don't seem to work here!!!
// TODO!! Look for TODO's in this file!
// TODO!! Do we really want to copy/insert from other array traits?
//
// Insertion operations have three cases:
// Depending on the operation new elements are either constructed, reconstructed or copy-constructed.
//
// Case0: index == m_size
//   Construct(for new elements) or copy-construct(for copies) new elements at the end of the array
//
// Case1: index + num < m_size
//   Insert elements that fit within the existing range
//
//   Example:
//   Insert 3 elements into an array of size 6 at index 1
//
//   |0 1 2 3 4 5|
//      ^
//      7 8 9
//
//   |0 [7 8 9] [1 2]| [3 4 5]
//
//   1: |0 1 2 x x x| 3 4 5   - copy-construct last 'num' elements [3 4 5] into to the new space
//   2: |0 x x x 1 2| 3 4 5   - assign (operator=) middle elements [1 2] to the end of the existing space
//   3: |0 7 8 9 1 2| 3 4 5   - re-construct(for new elements) or assign(for copies) new elements [7 8 9]
//
// Case2:
//   The new elements extend past the existing range
//   so all existing elements moved, are moved into new space
//
//   Example:
//   Insert 5 elements into an array of size 6 at index 4
//
//   |0 1 2 3 4 5|
//            ^
//            7 8 9 10 11
//
//   |0 1 2 3 [7 8]| [9 10 11] [4 5]
//
//
//   1: |0 1 2 3 x x| x x  x  4 5  - copy-construct last elements [4 5] to the end of the new range
//   2: |0 1 2 3 7 8| x x  x  4 5  - re-construct(for new elements) or assign(for copies) existing elements [7 8] at the end of existing space
//   3: |0 1 2 3 7 8| 9 10 11 4 5  - construct(for new elements) or copy-construct(for copies) new elements [9 10] in new space
//

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// TArrayTrait - Array traits template
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//
// ArrayTrait class must implement the following two methods:
//
// template<typename T>
//   static T * Reallocate(T * existingPtr, unsigned int newSize);
// - Reallocate a pointer using normal realloc rules
// - Such that realloc(ptr,0) is a free(), and realloc(NULL,count) is a malloc.
//
// static unsigned int CalcReserveSize( unsigned int size );
// - Return the amount of space to reserve for an array of 'size'.
// - Value returned must be >= size.
//
// static lResourceFixer::DataType GetFixerDataType();
// - Returns the fixer data type for the array data.
// - can be lResourceFixer::INVALID to disallow fixing.
// - can be lResourceFixer::ANY if it doesn't matter
// - to support fixing, the 'T' type of the lArray<T> must have defined operator<< for a lFixer.
//   (todo: check that gcc won't have a benny over this,
//          if it does, we might and have to define a lResourceArray<> with appropriate ctors and copy-ctors,
//          that has the array operator<< for lFixer and derives from lArray)

template< unsigned int TMULT, unsigned int TDIV, unsigned int TADD, enum lArenaID TARENA = L_ARENA_GLOBAL >
class TArrayTrait
{
public:
  template<typename T> T * Reallocate(T * existingPtr, unsigned int newSize, unsigned int )
  {
    if( newSize == 0 && existingPtr == NULL ) return NULL; /* realloc will return a valid pointer if existingPtr is NULL */
    return reallocate<T>(__FILE__,__FUNCTION__,__LINE__,TARENA,existingPtr,newSize);
  }

  static unsigned int CalcReserveSize( unsigned int size )
  {
    return size; //size + (TMULT*size/TDIV) + TADD;
  }

//  static lResourceFixer::DataType GetFixerDataType() { return lResourceFixer::ANY; }
};

#ifdef L_PLATFORM_PS2
template< unsigned int TMULT, unsigned int TDIV, unsigned int TADD, enum lArenaID TARENA = L_ARENA_GLOBAL >
class TArrayTraitAligned64
{
public:
  template<typename T> T * Reallocate(T * existingPtr, unsigned int newSize, unsigned int )
  {
    if( newSize == 0 && existingPtr == NULL ) return NULL; /* realloc will return a valid pointer if existingPtr is NULL */

    if( existingPtr == NULL )
    {
      //TODO: reallocatealigned?
      return (T*)memalign(64, sizeof(T) * newSize);
    }
    else
    {
      return (T*)realloc( existingPtr, sizeof(T) * newSize );
    }
  }

  static unsigned int CalcReserveSize( unsigned int size )
  {
    return size + (TMULT*size/TDIV) + TADD;
  }
};

typedef TArrayTraitAligned64<3,8,16> TDefaultArrayTraitAligned64;  // 3/8 + 16
#endif

// Typedefs for default array trait types

typedef TArrayTrait<0,1,0>  TFixedArrayTrait;    // 1 (fixed)
typedef TArrayTrait<3,8,16> TDefaultArrayTrait;  // 3/8 + 16
//typedef TArrayTrait<1,3,16> TDefaultArrayTrait;  // 1/3 + 16

// Notes: This little chunk of a fix for GCC generating
// loops on basic-types for constructors and deconstructors.

namespace lPrivate
{
  namespace lArrayDetail
  {

  template<typename T> class ArrayTypeTraitComplex
  {
  public:
    static void Move( T * dst, T * src, int count )
    {
      moveElements<T>(dst, src, count);
    }

    static void Assign( T * array, const T * otherArray, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        array[i] = otherArray[i];
      }
    }

    static void AssignReplicate( T * array, const T & element, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        array[i] = element;
      }
    }

    static void Construct( T * array, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        lPlacementConstructor<T>::Construct( (void *)(array+i) );
      }
    }

    static void ReConstruct( T * array, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        (array+i)->~T();
        lPlacementConstructor<T>::Construct( (void *)(array+i) );
      }
    }

    static void CopyConstruct( T * array, const T * otherArray, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        lPlacementConstructor<T>::CopyConstruct( (void *)(array+i), *(otherArray+i) );
      }
    }

    static void CopyConstructReplicate( T * array, const T & element, int start, int end )
    {
      for(int i=start;i<end;i++)
      {
        lPlacementConstructor<T>::CopyConstruct( (void *)(array+i), element );
      }
    }

    static void Destruct( T * array, int start, int end )
    {
      //NOTE: If object type has a constructor / destructor the compiler may
      //      complain about unused parameter so we're forcing a reference here
      L_UNUSED_PARAM( array );

      for( int i=start;i<end;i++ )
      {
        (array+i)->~T();
      }
    }
  };

  template<typename T> class ArrayTypeTraitSimple
  {
  public:
    static void Move( T * dst, T * src, int count )
    {
      moveElements<T>(dst, src, count);
    }

    static void Assign( T * array, const T * otherArray, int start, int end )
    {
      if( end > start )
      {
        memcpy( array+start, otherArray+start, (end-start) * sizeof(T) );
      }
    }

    static void AssignReplicate( T * array, const T & element, int start, int end )
    {
      for( int i=start; i<end; i++ )
      {
        array[i] = element;
      }
    }

    static void Construct( T * array, int start, int count )
    {
      L_UNUSED_PARAM( array );
      L_UNUSED_PARAM( start );
      L_UNUSED_PARAM( count );
    }

    static void ReConstruct( T * array, int start, int count )
    {
      L_UNUSED_PARAM( array );
      L_UNUSED_PARAM( start );
      L_UNUSED_PARAM( count );
    }

    static void CopyConstruct( T * array, const T * otherArray, int start, int end )
    {
      if( end > start )
      {
        memcpy( array+start, otherArray+start, (end-start) * sizeof(T) );
      }
    }

    static void CopyConstructReplicate( T * array, const T & element, int start, int end )
    {
      for( int i=start; i<end; i++ )
      {
        array[i] = element;
      }
    }

    static void Destruct( T * array, int start, int count )
    {
      L_UNUSED_PARAM( array );
      L_UNUSED_PARAM( start );
      L_UNUSED_PARAM( count );
    }
  };

  // TODO: for char types, we can optimise the Assign functions
  //       for all basic types, on a forward move or non-overlapping move we
  //       can optimise the move function.

  template<typename T> class ArrayTypeTrait : public ArrayTypeTraitComplex<T> { };

  template<> class ArrayTypeTrait<unsigned char>   : public ArrayTypeTraitSimple<unsigned char>  {};
  template<> class ArrayTypeTrait<char>            : public ArrayTypeTraitSimple<char>           {};
  template<> class ArrayTypeTrait<bool>            : public ArrayTypeTraitSimple<bool>           {};
  template<> class ArrayTypeTrait<unsigned short>  : public ArrayTypeTraitSimple<unsigned short> {};
  template<> class ArrayTypeTrait<short>           : public ArrayTypeTraitSimple<short>          {};
  template<> class ArrayTypeTrait<unsigned long>   : public ArrayTypeTraitSimple<unsigned long>  {};
  template<> class ArrayTypeTrait<long>            : public ArrayTypeTraitSimple<long>           {};
  template<> class ArrayTypeTrait<unsigned int>    : public ArrayTypeTraitSimple<unsigned int>   {};
  template<> class ArrayTypeTrait<int>             : public ArrayTypeTraitSimple<int>            {};
  template<> class ArrayTypeTrait<float>           : public ArrayTypeTraitSimple<float>          {};
  template<> class ArrayTypeTrait<double>          : public ArrayTypeTraitSimple<double>         {};

} }

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lArray - Dynamic array template
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//
// Use the TTRAIT template parameter to override the allocation type and
// allocation strategy.

template<typename T, typename TTRAIT = TDefaultArrayTrait>
class lArray : protected TTRAIT
{
  T   * m_array;
  int   m_size;
  int   m_reservedSize;
  bool  m_bFixed;

protected:

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Allocation and reservation methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // Check the reserved space, adding more space if needed
  //

  void CheckReserveGrow( int newReservedSize )
	{
    // reseve cannot be less than current size
    lAssert( newReservedSize >= m_size );

    // add extra space if needed
    if( newReservedSize > m_reservedSize )
    {
      if( m_bFixed )
      {
        // cannot grow a a fixed memory array
        lAssertAlways( !( "lArray::CheckReserveGrow: Cannot grow a fixed memory array" ) );
        return;
      }

      int oldreservedSize = m_reservedSize;

      m_reservedSize = TTRAIT::CalcReserveSize( newReservedSize );
      lAssert( m_reservedSize >= newReservedSize );
      m_array        = TTRAIT::Reallocate(m_array, m_reservedSize, oldreservedSize);
    }
  }

  // Check the reserved space, remove space if needed
  //

  void CheckReserveReduce()
  {
    // only check for reduction if we're not a fixed memory array
    if( !m_bFixed )
    {
      int normalReserve = TTRAIT::CalcReserveSize( m_size );
      lAssert( normalReserve >= m_size );

      // reduce the reserve if have more than twice the extra spare
      // we would normally need

      if( m_reservedSize-m_size > (normalReserve-m_size)<<1 )
      {
        int oldreservedSize = m_reservedSize;

        m_reservedSize = normalReserve;
        m_array        = TTRAIT::Reallocate(m_array,m_reservedSize,oldreservedSize);
      }
    }
  }

  // Set the reserved size, removing or adding space as needed
  //

  void SetReserve( int newReservedSize )
  {
    if( m_bFixed )
    {
      // cannot set reserved size on a fixed memory array
      lAssertAlways( !( "lArray::SetReserve: Cannot set reserve of a fixed memory array" ) );
      return;
    }

    // reseve cannot be less than current size
    lAssert( newReservedSize >= m_size );

    // resize reserved space
    int oldreservedSize = m_reservedSize;
    m_reservedSize = newReservedSize;
    m_array        = TTRAIT::Reallocate(m_array,m_reservedSize,oldreservedSize);
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Construction
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

public:

  lArray()           : m_array(static_cast<T*>(0)), m_size(0), m_reservedSize(0), m_bFixed(false) { }
  lArray( int size ) : m_array(static_cast<T*>(0)), m_size(0), m_reservedSize(0), m_bFixed(false) { Add(size); }

  ~lArray()
  {
    Empty();
    lAssert( m_size == 0 );
    if( !m_bFixed )
    {
      lAssert( m_array == NULL );
    }
  }

  // Compactly copy-construct the array from another array of the same trait
  // (no unused reserved space)

  lArray( const lArray<T,TTRAIT>& other ) : m_array(static_cast<T*>(0)), m_size(0), m_reservedSize(0), m_bFixed(false)
  {
    SetReserve( other.m_size );

    // copy-construct the elements

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, other.m_array, 0, other.m_size );

    // set the new size

    m_size = other.m_size;
  }

  // Compactly assign the array from another array of the same trait
  // (unused reserved space)

  lArray<T,TTRAIT>& operator=( const lArray<T,TTRAIT>& other )
  {
    Copy( other );
    return *this;
  }

  // Set the fixed data
  // Note: when the array is fixed using lFixer the unused
  //       reserve will be eliminated. so that reserveSize = size.

  void SetFixedData( T * data, int size, int reservedSize )
  {
    lAssert( reservedSize >= size );

    Clear();
    SetReserve(0);

    lAssert( m_size == 0 );
    lAssert( m_reservedSize == 0);
    lAssert( m_array == static_cast<T*>(0) );

    m_array        = data;
    m_size         = size;
    m_reservedSize = reservedSize;
    m_bFixed       = true;
  }

  // Compactly assign the array from another array of any other trait
  // (there will be no unused reserved space)

  template<typename OTRAIT>
  void Copy( const lArray<T,OTRAIT>& other )
  {
    // nasty reinterpret cast here,
    // but we know what we're doing
    // TODO: can we avoid this with specialisation?

    if( reinterpret_cast< const lArray<T,TTRAIT> * >( &other ) != this )
    {
      if( m_size < other.m_size )
      {
        // if this array is smaller than the new array

        SetReserve( other.m_size );

        // assign first 'm_size' elements

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Assign( m_array, other.m_array, 0, m_size );

        // copy-construct the rest

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, other.m_array, m_size, other.m_size );

        // set the new size

        m_size = other.m_size;
      }
      else
      {
        // if this array is larger than or equal to the new array

        lAssert( m_size >= other.m_size );

        // assign 'other.m_size' elements

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Assign( m_array, other.m_array, 0, other.m_size );

        // destruct the rest

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Destruct( m_array, other.m_size, m_size );

        // set the new size

        m_size = other.m_size;

        // set the reserve

        SetReserve( m_size );
      }
    }
  }

  bool operator==( const lArray<T,TTRAIT>& a ) const
  {
    if( Size() != a.Size() ) return false;

    for( int i=0;i<Size();i++)
    {
      if( !(m_array[i] == a(i)) ) return false;
    }

    return true;
  }

  bool operator!=( const lArray<T,TTRAIT>& a ) const
  {
    return !(this == a);
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Size/Clear methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // Clear the array (doesn't affect reserve)
  // TODO: Should we combine Empty and Clear() ?
  // TODO: The semantics seem odd
  // TODO: Should be be using Empty() to reserve space in the array?

  void Clear()
  {
    // destruct elements

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::Destruct( m_array, 0, m_size );

    // set new size

    m_size = 0;
  }

  // Clear the array without destruction (doesn't affect reserve)
  // !!!Use with caution!!!

  void ClearNoDestroy()
  {
    m_size = 0;
  }

  // Compact the array, (free any unused reserved space)

  void Compact()
  {
    SetReserve( m_size );
  }

  // Empty the array, setting the new reserved space size

  void Empty( int newReservedSize=0 )
	{
    Clear();
    if( !m_bFixed )
    {
      // TODO: this seems silly! see notes and decide.
      SetReserve( newReservedSize );
    }
  }

  // Set the array size compactly, so there is no extra reseved space
  // construct or destruct elements as required.
  // Existing elements still within the range are left constructed.

  void SetSize( int size )
  {
    if( m_size == size ) return;

    if( size == 0 )
    {
      // clear to 0 size
      Clear();
      SetReserve( 0 );
    }
    else if( size > m_size )
    {
      // add extra elements

      SetReserve( size );
      Add( size-m_size );
    }
    else
    {
      // remove extra elements

      Remove( m_size-(m_size-size), m_size-size );
      SetReserve( size );
    }
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Addition/Insertion/Removal methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  // Add:
  //  Add 'n' elements onto the end of the array

  int Add( int num = 1 )
  {
    int index = m_size;

    // make sure we have enough space for 'num' new elements

    CheckReserveGrow(m_size+num);
    m_size += num;

    // construct the new elements

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::Construct( m_array, index, index+num );

    return index;
  }

  // Add:
  //  Add 'n' copies of an element onto the end of the array
  //  Note: 'copies' should not have a default value, as lArray<int>::Add() would then be ambiguous.
  //  Note: checks that the reference to the element is not currently within the array,
  //        as the address can change if the reserve is grown.

  int Add(const T &element, int copies)
  {
    int index = m_size;

    // if the address of the element to add is within the array,
    // then we throw an assertion, as the address of the element may change
    // if the reserve is grown.

    lAssert( (&element < m_array) || (&element >= m_array+m_size) );

    // make sure we have enough space for 'copies' new elements

    CheckReserveGrow(m_size+copies);
    m_size += copies;

    // copy-construct the new elements

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstructReplicate( m_array, element, index, index+copies );

    return index;
  }

  // Insert:
  //  Insert 'n' new elements into the array

  void Insert( int index, int num=1 )
  {
    lAssert( num>0 );
    lAssert( index>=0 );
    lAssert( index<=m_size );

    // make sure we have enough space for 'num' new elements

    CheckReserveGrow( m_size+num );

    if( index == m_size )
    {
      // Case0: Construct new elements at the end of the array

      lPrivate::lArrayDetail::ArrayTypeTrait<T>::Construct( m_array, m_size, m_size+num );
    }
    else
    {
      if( index + num < m_size )
      {
        // Case1: Insert elements that fit within the existing range

        // 1: Copy-construct last 'num' elements into to the new space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, m_array-num, m_size, m_size+num );

        // 2: Move middle elements to the end of the existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Move( m_array+(index+num), m_array+index, m_size-(index+num) );

        // 3: re-construct new elements

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::ReConstruct( m_array, index, index+num );
      }
      else
      {
        // Case2: The new elements extend past the existing range

        // 1: Copy-construct last elements to the end of the new range

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array+num, m_array, index, m_size );

        // 2: Re-construct existing elements at the end of existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::ReConstruct( m_array, index, m_size );

        // 3: Construct new elements in new space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Construct( m_array, m_size, index+num );
      }
    }

    // extend existing range into new space

    m_size += num;
  }

  // Insert 'n' copies of an elements into the array

  void Insert( const T& element, int index, int num=1 )
  {
    lAssert( num>0 );
    lAssert( index>=0 );
    lAssert( index<=m_size );

    // make sure we have enough space for 'num' new elements

    CheckReserveGrow( m_size+num );

    if( index == m_size )
    {
      // Case0: Copy-construct new elements at the end of the array

      lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstructReplicate( m_array, element, m_size, m_size+num );

    }
    else
    {
      if( index + num < m_size )
      {
        // Case1: Insert elements that fit within the existing range

        // 1: Copy-construct last 'num' elements into to the new space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, m_array-num, m_size, m_size+num );

        // 2: Move middle elements to the end of the existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Move( m_array+(index+num), m_array+index, m_size-(index+num) );

        // 3: Assign new elements

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::AssignReplicate( m_array, element, index, index+num );

      }
      else
      {
        // Case2: The new elements extend past the existing range

        // 1: Copy-construct last elements to the end of the new range

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array+num, m_array, index, m_size );

        // 2: Assign existing elements at the end of existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::AssignReplicate( m_array, element, index, m_size );

        // 3: Copy-construct new elements in new space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstructReplicate( m_array, element, m_size, index+num );

      }
    }

    // extend existing range into new space

    m_size += num;
  }

  // Insert another array into the array

  template<class OTRAIT>
  void Insert( const lArray<T,OTRAIT>& other, int index )
  {
    Insert( other, index, 0, other.Size() );
  }

  // Inset another array into the array

  template<class OTRAIT>
  void Insert( const lArray<T,OTRAIT>& other, int index, int offset, int num )
  {
    if( num == 0 ) return;

    lAssert( offset >= 0 );
    lAssert( offset <= other.m_size );
    lAssert( num+offset <= other.m_size );

    const T * otherBase = other.m_array+offset;

    lAssert( num>0 );
    lAssert( index>=0 );
    lAssert( index<=m_size );

    // make sure we have enough space for 'num' new elements

    CheckReserveGrow( m_size+num );

    if( index == m_size )
    {
      // Case0: Copy-construct new elements at the end of the array

      lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, otherBase-m_size, m_size, m_size+num );

    }
    else
    {
      if( index + num < m_size )
      {
        // Case1: Insert elements that fit within the existing range

        // 1: Copy-construct last 'num' elements into to the new space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, m_array-num, m_size, m_size+num );

        // 2: Move middle elements to the end of the existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Move( m_array+(index+num), m_array+index, m_size-(index+num) );

        // 3: Assign new elements

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Assign( m_array, otherBase-index, index, index+num );

      }
      else
      {
        // Case2: The new elements extend past the existing range

        // 1: Copy-construct last elements to the end of the new range

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array+num, m_array, index, m_size );

        // 2: Assign existing elements at the end of existing space

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::Assign( m_array, otherBase-index, index, m_size );

        // 3: Copy-construct new elements in new space

        int otherIndex = m_size-index-m_size;

        lPrivate::lArrayDetail::ArrayTypeTrait<T>::CopyConstruct( m_array, otherBase+otherIndex, m_size, index+num );
      }
    }

    // extend existing range into new space

    m_size += num;
  }

  // Remove:
  //  Remove 'n' elements from the array

  void Remove( int index, int num=1 )
  {
    if( num == 0 ) return;

    lAssert( num>0 );
    lAssert( index>=0 );
    lAssert( index + num <= m_size );

    // move the elements down

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::Move( m_array+index, m_array+(index+num), (m_size-index-num) );

    // destruct remaining elements

    lPrivate::lArrayDetail::ArrayTypeTrait<T>::Destruct( m_array, m_size-num, m_size );

    // set new size

    m_size -= num;

    // reduce the reserve if needed

    CheckReserveReduce();
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Property accessor methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  const bool IsFixed() const         { return m_bFixed; }
  const int  Size() const            { return m_size; }
  const int  ReservedSize() const    { return m_reservedSize; }
  const int  ElementSize() const     { return sizeof(T); }
  const int  NumBytes() const        { return sizeof(T) * m_size; }
  const T *  Data() const            { return m_array; }
  T       *  Data()                  { return m_array; }
  bool       IsEmpty() const         { return m_size == 0; }

  // HACK-TimJ
  #ifdef L_PLATFORM_XBOX
  void XboxMaskContignous()
  {
    DWORD ptr = (DWORD)m_array;
    ptr |= 0x80000000;
    m_array = (T*)ptr;

  }
  #endif

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Data accessor methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  T& At(int index)
  {
    lAssert( index>=0 );
    lAssert( index<m_size );
    return m_array[index];
  }

  const T& At( int index ) const
	{
    lAssert( index>=0 );
    lAssert( index<m_size );
    return m_array[index];
  }

  T& operator()( int index )
	{
    lAssert( index>=0 );
    lAssert( index<m_size );
    return m_array[index];
	}

  const T& operator()( int index ) const
	{
    lAssert( index>=0 );
    lAssert( index<m_size );
    return m_array[index];
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Find methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  bool Find( const T& element, int *index ) const
  {
    for( int i=0;i<m_size;i++)
    {
      if( m_array[i] == element )
      {
        if( index ) (*index) = i;
        return true;
      }
    }

    return false;
  }

  int Find(const T& element) const
  {
    for (int i = 0; i < m_size; i++)
    {
      if ( m_array[i] == element )
      {
        return i;
      }
    }

    return -1;
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  void     Push( const T& element )             { Add( element, 1 ); }
  T        Pop()                                { lAssert( m_size>0 ); T r = m_array[m_size-1]; Remove( m_size-1 );  return r; }
  T&       Last()                               { lAssert( m_size>0 ); return m_array[m_size-1]; }
  const T& Last() const                         { lAssert( m_size>0 ); return m_array[m_size-1]; }
  T&       First()                              { lAssert( m_size>0 ); return m_array[0]; }
  const T& First() const                        { lAssert( m_size>0 ); return m_array[0]; }
  T*       Begin()                              { return m_array; }
  const T* Begin() const                        { return m_array; }
  T*       End()                                { return m_array + m_size; }
  const T* End() const                          { return m_array + m_size; }

  template<class OTRAIT>
  void Append( const lArray<T,OTRAIT>& other )
  {
    Insert( other, m_size, 0, other.m_size );
  }

  template<class OTRAIT>
  void Append(const lArray<T,OTRAIT>& other, int offset, int size)
  {
    Insert( other, m_size, offset, size );
  }

  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Unique element insertion and removal
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  int AddElement( const T& element )
  {
    (*this)(Add())=element; return Size()-1;
  }

  int AddUniqueElement( const T& element )
  {
    int index;
    if(Find(element,&index)) return index;
    return Add(element,1);
  }

  int RemoveElement( const T& element )
  {
    int removed=0;

    for( int i=0;i<m_size;i++)
    {
      if ( (*this)(i) == element )
      {
        Remove( i );
	      i--;
	      removed++;
	    }
	  }

	  return removed;
	}

	bool RemoveSingleElement(const T &element)
	{
    int index;
    if( Find( element, &index ) )
    {
	    Remove( index );
	    return true;
	  }
	  else
	  {
	    return false;
	  }
	}

  // slow
  template<class OTRAIT>
  void AppendUnique( const lArray<T,OTRAIT>& other )
  {
    for(int i = 0; i < other.Size(); i++)
    {
      AddUniqueElement( other(i) );
    }
  }

  // slow sort
  void Sort( bool bAscending = true )
  {
    if( bAscending )
    {
      for (int i1 = 0 ; i1 < m_size-1 ; i1++)
      {
        int changes = 0;
        for (int i2 = i1+1 ; i2 < m_size ; i2++)
        {
          if ( m_array[i1] > m_array[i2] )
          {
            lSwap( m_array[i1], m_array[i2] );
            changes++;
          }
        }
        if(!changes) return;
      }
    } else
    {
      for (int i1 = 0 ; i1 < m_size-1 ; i1++)
      {
        int changes = 0;
        for (int i2 = i1+1 ; i2 < m_size ; i2++)
        {
          if ( m_array[i1] < m_array[i2] )
          {
            lSwap( m_array[i1], m_array[i2] );
            changes++;
          }
        }
        if(!changes) return;
      }
    }
  }

//  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//  // Fixing
//  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//
//  lResourceFixer & ResourceFix( lResourceFixer &fixer )
//  {
//    m_bFixed       = true;
//    m_reservedSize = m_size;
//    fixer.FixPtr( m_array, m_size, TTRAIT::GetFixerDataType() );
//    return fixer;
//  }
//
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  inline lStream & SimpleSerialize( lStream &ar )
  {
    if ( ar.IsLoading() )
    {
      int newSize;
      ar << newSize;
      Empty( newSize );
      m_size = newSize;
    }
    else if ( ar.IsSaving() )
    {
      ar << m_size;
    }

    ar.Serialize( m_array, sizeof(T) * m_size );
    return ar;
  }

  inline lStream & ConstructSerialize(lStream &ar)
  {
    if( ar.IsLoading() )
    {
      int newSize;
      ar << newSize;
      SetSize(newSize);

      for(int i=0;i<newSize;i++)
      {
        ar << At(i);
      }
    }

    if( ar.IsSaving() )
    {
      ar << m_size;
      for(int i=0;i<m_size;i++)
      {
        ar << At(i);
      }
    }

    return ar;
  }
};

// Built-in data types can be serialized simply (without construction)
//

//NOTE: Disabled simple serialize on Wii so that endian is handled by the lStream operator<<
#ifndef L_PLATFORM_WII
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<uint8,TTRAIT> & c )  { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<int8,TTRAIT> & c )   { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<bool,TTRAIT> & c )   { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<uint16,TTRAIT> & c ) { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<int16,TTRAIT> & c )  { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<uint64,TTRAIT> & c ) { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<int64,TTRAIT> & c )  { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<uint32,TTRAIT> & c ) { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<int32,TTRAIT> & c )  { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<float,TTRAIT> & c )  { return c.SimpleSerialize( ar ); }
template< class TTRAIT > inline lStream& operator<< ( lStream& ar, lArray<double,TTRAIT> & c ) { return c.SimpleSerialize( ar ); }
#endif

template< class T, class TTRAIT > lStream& operator<< ( lStream &ar, lArray<T,TTRAIT> &a )    { return a.ConstructSerialize( ar ); }
//
//template< class T, class TTRAIT > lResourceFixer & operator << (lResourceFixer &fixer, lArray<T,TTRAIT> &a ) { return a.ResourceFix( fixer ); }

END_L_NAMESPACE
#endif
