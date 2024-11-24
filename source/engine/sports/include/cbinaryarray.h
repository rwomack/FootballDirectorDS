//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : Cbinaryarray.h
// Description :
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>

#ifndef __CBINARYARRAY_H
#define __CBINARYARRAY_H

template<unsigned int TnumBits = 1>
class CBinaryArrayElem;

template<unsigned int TnumBits = 1>
class CBinaryArray
{
public:
    CBinaryArray()           : m_size(0), m_array() { }
    CBinaryArray( int size ) : m_size(0), m_array() { SetSize(size); }

    CBinaryArray( const CBinaryArray<TnumBits>& other ) : m_size(0)
    {
      m_array = other.m_array;
    }

    // Set the fixed data
    // Note: when the array is fixed using ctFixer the unused
    //       reserve will be eliminated. so that reserveSize = size.

    void SetFixedData( int numElements, char * data, int size, int reservedSize )
    {
      lAssert( reservedSize >= size );

      m_size = numElements;
      m_array.SetFixedData( data, size, reservedSize );
    }

    unsigned int Size() const
    {
      return m_size;
    }

    void Clear()
    {
      m_array.Empty();
      m_size = 0;
    }

    void SetSize( unsigned int size )
    {
      lAssert( TnumBits > 0 && TnumBits <= 32 );

      unsigned int totalBits  = (size * TnumBits);
      unsigned int totalBytes = totalBits / 8;

      if ((totalBits & 0x07) != 0)
      {
        totalBytes++;
      }

      m_array.SetSize( totalBytes );
      m_size = size;
    }

    unsigned int Get( unsigned int idx ) const
    {
      unsigned int arrayIdx = (idx * TnumBits) / 8;
      unsigned int arrayMask = 0x80 >> ((idx * TnumBits) & 0x07);
      unsigned int arrayByte = m_array(arrayIdx);

      unsigned int result = 0, resultMask = (1 << (TnumBits - 1));

      while(1)
      {
        if( (arrayByte & arrayMask) != 0 )
        {
          result |= resultMask;
        }

        arrayMask >>= 1;
        resultMask >>= 1;

        if( resultMask == 0 )
        {
          break;
        }

        if( arrayMask == 0 )
        {
          arrayByte = m_array( ++arrayIdx );
          arrayMask = 0x80;
        }
      }

      return result;
    }

    void Set( unsigned idx, unsigned int val )
    {
      lAssert(val < (1 << TnumBits));
      unsigned int valMask  = (1 << (TnumBits - 1));
      unsigned int arrayIdx = (idx * TnumBits) / 8;

      char *arrayByte = &m_array.Data()[arrayIdx];
      unsigned int   arrayMask = 0x80 >> ((idx * TnumBits) & 0x07);

      while( valMask != 0 )
      {
        if ((val & valMask) == 0)
        {
          *arrayByte &= ~arrayMask;
        }
        else
        {
          *arrayByte |= arrayMask;
        }

        arrayMask >>= 1;
        if (arrayMask == 0)
        {
          arrayByte++;
          arrayMask = 0x80;
        }

        valMask >>= 1;
      }
    }

    CBinaryArrayElem<TnumBits> operator()(unsigned int idx) const;
    CBinaryArrayElem<TnumBits> operator()(int idx) const;

private:
    unsigned int    m_size;             // size in elements of TnumBits
    lArray<char>   m_array;
};

///////////////////////////////////////////////////////////////////////////////

template<unsigned int TnumBits>
class CBinaryArrayElem
{
  public:
    CBinaryArrayElem(CBinaryArray<TnumBits> * BinArray, unsigned int idx)
    {
      init( BinArray, idx );
    }

    CBinaryArrayElem& operator=(unsigned int val)
    {
       _BinArray->Set(_idx, val);
       return *this;
    }

    CBinaryArrayElem& operator=(bool val)
    {
      _BinArray->Set(_idx, val ? 1 : 0);
      return *this;
    }

    unsigned int Get() const
    {
      return _BinArray->Get(_idx);
    }

    bool GetBool() const
    {
      return (_BinArray->Get(_idx) != 0);
    }

    operator unsigned int() const
    {
      return _BinArray->Get(_idx);
    }

    operator bool() const
    {
      return (_BinArray->Get(_idx) != 0);
    }

    // Comparison Operators

    bool operator<(unsigned int rhs) const        { return (Get() < rhs); }
    bool operator<=(unsigned int rhs) const       { return (Get() <= rhs); }
    bool operator>(unsigned int rhs) const        { return (Get() > rhs); }
    bool operator>=(unsigned int rhs) const       { return (Get() >= rhs); }
    bool operator==(unsigned int rhs) const       { return (Get() == rhs); }
    bool operator!=(unsigned int rhs) const       { return (Get() != rhs); }

    bool operator<(int rhs) const        { return (Get() < (unsigned int)rhs); }
    bool operator<=(int rhs) const        { return (Get() <= (unsigned int)rhs); }
    bool operator>(int rhs) const        { return (Get() > (unsigned int)rhs); }
    bool operator>=(int rhs) const        { return (Get() >= (unsigned int)rhs); }
    bool operator==(int rhs) const        { return (Get() == (unsigned int)rhs); }
    bool operator!=(int rhs) const        { return (Get() != (unsigned int)rhs); }

    bool operator<(const CBinaryArrayElem& rhs) const        { return (Get() < rhs.Get()); }
    bool operator<=(const CBinaryArrayElem& rhs) const        { return (Get() <= rhs.Get()); }
    bool operator>(const CBinaryArrayElem& rhs) const        { return (Get() > rhs.Get()); }
    bool operator>=(const CBinaryArrayElem& rhs) const        { return (Get() >= rhs.Get()); }
    bool operator==(const CBinaryArrayElem& rhs) const        { return (Get() == rhs.Get()); }
    bool operator!=(const CBinaryArrayElem& rhs) const        { return (Get() != rhs.Get()); }

  private:
    void init( CBinaryArray<TnumBits>* BinArray = NULL, unsigned int idx = 0 )
    {
      _BinArray = BinArray;
      _idx = idx;
    }

    CBinaryArray<TnumBits>* _BinArray;
    unsigned int          _idx;
};

template<unsigned int TnumBits>
CBinaryArrayElem<TnumBits> CBinaryArray<TnumBits>::operator()(unsigned int idx) const
{
  return CBinaryArrayElem<TnumBits>((CBinaryArray<TnumBits> *)this, idx);
}

template<unsigned int TnumBits>
CBinaryArrayElem<TnumBits> CBinaryArray<TnumBits>::operator()(int idx) const
{
  return CBinaryArrayElem<TnumBits>((CBinaryArray<TnumBits> *)this, (unsigned int)idx);
}

#endif
