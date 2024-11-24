//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsort.h
// Description : Quick sort template class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 You can use lSort with a global function, or a function object.
 The compare function should return an integer indicating the following
 conditions:

  0  -  elements are equal
  -1 -  first element is smaller than seconds element
  1  -  first element is larger than second element

 Example:

  struct CompareFunctionObject
  {
    int operator()(const int &a, const int &b )
    {
      if ( a < b ) return -1;
      if ( a > b ) return 1;
      return 0;
    }
  };

  int CompareFunction( const int &a, const int &b )
  {
    if ( a < b ) return -1;
    if ( a > b ) return 1;
    return 0;
  }

 Using a global function:

   lSort<int>( array, size, CompareFunction );

 Using a function object:

   CompareFunctionObject f;
   lSort<int>( array, size, f );

*/


#ifndef LSORT_H_INCLUDED
#define LSORT_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSort
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T,class T_COMPARE> void lSort( T* data, uint32 dataCount, T_COMPARE &Compare )
{
  const int constStackSize = (8*sizeof(void*) - 2);
  const int constCutOff = 8;

  if( dataCount<2 ) return;

  struct Stack
  {
    T* lo;
    T* hi;
  };

  Stack sortStack[constStackSize];
  Stack current;
  Stack inner;

  sortStack[0].lo = data;
  sortStack[0].hi = data+dataCount-1;

  for( Stack* stackTop=sortStack; stackTop>=sortStack; --stackTop )
	{
    current = *stackTop;

    recurse:

    int size = current.hi - current.lo + 1;
    if( size <= constCutOff )
    {
      // insertion sort for sorting short arrays (<= constCutOff in size)

      while( current.hi > current.lo )
      {
        T *hi = current.lo;
        for( T * p = current.lo+1; p<=current.hi; p++ )
        {
          if( Compare(*p, *hi) > 0 )
          {
            hi = p;
          }
        }

        lSwap( *hi, *current.hi );
        current.hi--;
      }
    }
    else
		{
      T * mid = &current.lo[size/2]; // find middle element

      // Sort the first, middle, last elements into order

      if( Compare(*current.lo, *mid) > 0)          lSwap(*current.lo, *mid);
      if( Compare(*current.lo, *current.hi)  > 0)  lSwap(*current.lo, *current.hi);
      if( Compare(*mid, *current.hi) > 0)          lSwap(*mid, *current.hi);

      inner.lo = current.lo;
      inner.hi = current.hi;

      // Parition the list into three parts, <= mid, == mid and > mid

			for( ; ; )
			{
        do { inner.lo++; } while( ( inner.lo <= current.hi )  && Compare(*inner.lo, *mid) <= 0);
        do { inner.hi--; } while( ( inner.hi > mid         )  && Compare(*inner.hi, *mid) > 0);

        if( inner.hi < inner.lo ) break;

        lSwap( *inner.lo, *inner.hi );

        // if the mid element moved in the swap, follow it

        if( mid == inner.hi ) mid = inner.lo;
			}

      // Find adjacent elements equal to the partition element.

      inner.hi++;
      do { inner.hi--; } while( ( inner.hi > current.lo ) && Compare(*inner.hi, *mid) == 0);

      // sort sub array, [current.lo , inner.hi] and [inner.lo, current.hi]
      // do the smallest list first

      if( inner.hi-current.lo >= current.hi-inner.lo )
			{
        if( current.lo < inner.hi )
				{
          stackTop->lo = current.lo;
          stackTop->hi = inner.hi;
          stackTop++;
				}
        if( inner.lo < current.hi )
				{
          current.lo = inner.lo;
          goto recurse;
				}
			}
			else
			{
        if( inner.lo < current.hi )
				{
          stackTop->lo = inner.lo;
          stackTop->hi = current.hi;
          stackTop++;
				}
        if( current.lo<inner.hi )
				{
          current.hi = inner.hi;
          goto recurse;
				}
			}
		}
	}
}

END_L_NAMESPACE
#endif
