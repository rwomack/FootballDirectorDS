//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmfixed.h
// Description : Fixed point math class include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMFIXED_H_INCLUDED
#define LMFIXED_H_INCLUDED

#include "lmath.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Fixed point math routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// templated fixed point class, some predefined types
// also predefine values like pi etc in each of the formats as const statics

template <typename Tbase, typename Tlarger, unsigned int NintegerBits, bool Signed>
class lmFixed
{
  protected:
    Tbase      m_value;

	public:
    typedef lmFixed<Tbase,   Tlarger, NintegerBits,     Signed>  myType;
    typedef lmFixed<Tlarger, Tlarger, NintegerBits * 2, Signed>  lgType;

    static const unsigned int m_numBits      = 8 * sizeof(Tbase);
    static const unsigned int m_integerPart  = NintegerBits - ((Signed && (NintegerBits == 0)) ? 1 : 0);
    static const unsigned int m_fractionPart = m_numBits - m_integerPart;

    // min + max integers representable by this type
    static const uint64 m_maxInteger = (1 << (m_integerPart - Signed ? 1 : 0)) - 1;

    //TODO: WAS: uint64 which shouldnt be used here?
    static const int64 m_minInteger = Signed ? (-(int64)m_maxInteger - 1) : 0;
    //WAS//static const uint64 m_minInteger = Signed ? (-m_maxInteger - 1) : 0;


    // Values representing one (1.0) + a half (0.5)
    // TODO : Check if these are ok stored as Tbase (i think they should be)
    static const Tbase m_one        = (1 << m_fractionPart);
    static const Tbase m_half       = (1 << (m_fractionPart - 1));

    static const Tbase m_pi         = Tbase( LM_PI * (float)m_one );
    static const Tbase m_2pi        = Tbase( LM_2PI * (float)m_one );
    static const Tbase m_pi_2       = Tbase( LM_PI_2 * (float)m_one );
    static const Tbase m_e          = Tbase( LM_E * (float)m_one );
    static const Tbase m_sqrt2      = Tbase( LM_SQRT2 * (float)m_one );
    static const Tbase m_sqrt3      = Tbase( LM_SQRT3 * (float)m_one );

    //##
    // constructor / deconstructuor
    inline lmFixed()
    {}

    inline ~lmFixed()
    {}

    //##
		// cast from a different fixed-point type
    template <typename b, typename l, unsigned int i, bool s> lmFixed( const lmFixed<b,l,i,s>& in )
		{
      // is 'in's fractional part larger than ours
      if (lmFixed<b,l,i,s>::m_fractionPart > m_fractionPart)
			{
        m_value = (in.m_value + 1
                   << (lmFixed<b,l,i,s>::m_fractionPart - m_fractionPart - 1))
                   >> (lmFixed<b,l,i,s>::m_fractionPart - m_fractionPart);
			}
			else
			{
        m_value = in.m_value << (m_fractionPart - lmFixed<b,l,i,s>::m_fractionPart);
			}
		}

    // get raw fixed point value
    inline Tbase& Raw()           { return m_value; }
    inline Tbase  Raw() const     { return m_value; }

    // casts

    //##
    // integer to fixed point
    inline myType& operator= (const Tbase& in)
		{
      m_value = in << m_fractionPart;
			return *this;
		}

    //##
    // float to fixed point
    inline myType& operator= (const float& in)
		{
      m_value = Tbase(in * (float)m_one);
			return *this;
		}

    //##
    // double to fixed point
    inline myType& operator= (const double& in)
		{
      m_value = Tbase(in * (double)m_one);
			return *this;
		}

    //##
    // fixed point to double
    inline operator double() const
    {
      double tmp = double( m_value ) / double( m_one );
      return tmp;
    }

    //##
    // fixed point to float
    inline operator float() const
    {
      float tmp = float( m_value ) / float( m_one );
      return tmp;
    }

    //##
    // fixed point to unsigned int
    inline operator unsigned int() const
    {
      return m_value >> m_fractionPart;
    }

    //##
    // fixed point to int
    inline operator int() const
    {
      return m_value >> m_fractionPart;
    }

    //##
		// addition
    inline myType& operator+= (const myType& in)
		{
      m_value += in.m_value;
			return *this;
		}
    inline myType operator+ (const myType& in) const
		{
      myType tmp = *this;
      return tmp += in;
		}

    // TODO : could put additions for floats + doubles + ints here

    //##
		// subtraction
    inline myType& operator-= (const myType& in)
		{
      m_value -= in.m_value;
			return *this;
		}
    inline myType operator- (const myType& in) const
		{
      myType tmp = *this;
      return tmp -= in;
		}

    //##
		// multiplication
    inline myType& operator*= (const myType& in)
		{
      // We use the larger type here so overflows arent a problem

      Tlarger tmp = Tlarger( m_value ) * Tlarger( in.m_value ) + m_half;
      tmp >>= m_fractionPart;
      m_value = Tbase( tmp );

			return *this;
		}

    inline myType operator* (const myType& in)
		{
      myType tmp = *this;
      return tmp *= in;
		}

    // TODO : we could use larger fixed point type to return - why would we ?
//    /// non-assignment multiplication
//    inline lgType operator* (const myType& rhs) const
//    {
//      lgType res;
//      res.m_value = Tlarger(m_value) * Tlarger(rhs.m_value) + m_half;
//      res.m_value >>= (2 * m_fractionPart - lgType::m_fractionPart);
//      return res;
//    }

    //##
		// division
    inline myType& operator/= (const myType& in)
		{
      // We use the larger integer for the divide

      Tlarger tmp = Tlarger( m_value ) << m_fractionPart;
      tmp /= in.m_value;
      m_value = Tbase(tmp);

			return *this;
		}

    inline myType operator/ (const myType& in) const
		{
      myType tmp = *this;
      return tmp /= in;
		}

    // comparisons
    inline bool operator== (const myType& in) const
		{
      return m_value == in.m_value;
		}

    inline bool operator!= (const myType& in) const
		{
      return m_value != in.m_value;
		}

    inline bool operator>= (const myType& in) const
		{
      return m_value >= in.m_value;
		}

    inline bool operator<= (const myType& in) const
		{
      return m_value <= in.m_value;
		}

    inline bool operator> (const myType& in) const
		{
      return m_value > in.m_value;
		}

    inline bool operator< (const myType& in) const
		{
      return m_value < in.m_value;
		}

    // various functions

    //##
    // one over
    inline myType& OneOver()
		{
      // We use the larger integer for the divide

      Tlarger tmp = Tlarger( m_one ) << m_fractionPart;
      tmp /= Tlarger( m_value );
      m_value = Tbase( tmp );

			return *this;
		}

    //##
    // Negation
    inline myType operator- () const
		{
      myType tmp;
      tmp.m_value = -m_value;
			return tmp;
		}

    //##
    // Return the smallest positive nonzero value representable
    static inline myType SmallestValue()
		{
      myType tmp;
      tmp.m_value = 1;
			return tmp;
		}

    //##
    // shift functions
    inline myType& operator >>= (unsigned int shift)
		{
      m_value >>= shift;
			return *this;
		}
    inline myType& operator <<= (unsigned int shift)
		{
      m_value <<= shift;
			return *this;
		}
    inline myType operator >> (unsigned int shift) const
		{
      myType tmp;
      tmp.m_value >>= shift;
			return tmp;
		}
    inline myType operator << (unsigned int shift) const
		{
      myType tmp;
      tmp.m_value <<= shift;
			return tmp;
		}

    //##
    // Floor the fixed point value (returns the next lowest integer)
    inline myType Floor() const
		{
      myType tmp;
      tmp.m_value = m_value & (~(m_one - 1));
			return tmp;
		}

    //##
    // Ceil the fixed point value (returns the next greater or equal integer)
    inline myType Ceil() const
		{
      myType tmp;
      tmp.m_value = m_value;
      tmp.m_value += m_one - 1;
      tmp.m_value &= (~(m_one - 1));
			return tmp;
		}

    //##
    // Rounds to the nearest integer
    inline myType Round() const
		{
      myType tmp;
      tmp.m_value = (m_value + m_half) & (~(m_one - 1));
			return tmp;
		}
};


//##
// Returns the square root of the argument in the same fixed-point format.
// Algorithm very closely based on the one presented in
// 'Graphics Gems V', Alan W. Paeth. Pages 22-24: 'Fixed-Point Square Root'
// by Ken Turkowski, Apple Computer, Inc.; Cupertino, California;
// turk@apple.com.
template <typename b, typename l, unsigned int i, bool s>
  lmFixed<b,l,i,s> lmSqrt( const lmFixed<b,l,i,s>& x )
{
  // (total bits / 2) - 1 + (fracbits + 1) / 2
  const b iter = (sizeof(b) * 4 - 1) + (lmFixed<b,l,i,s>::m_fractionPart + 1) / 2;

  // using uint64 in case there are problems with signedness or size.
  // for compilers that support it, faster partial specialisations are provided
  uint64 root = 0, hi = 0, count = iter, test, lo = x.Raw();

  // even numbers of fractional bits are no problem, odd numbers need some tweaking.
  if (0 == (lmFixed<b,l,i,s>::m_fractionPart & 1))
  {
    hi = (lo >> (lmFixed<b,l,i,s>::m_numBits - 2)) & b(~0);
    lo <<= 2;
    lo &= (uint64(1) << lmFixed<b,l,i,s>::m_numBits) - 1;
  }
  else
  {
    hi = (lo >> (lmFixed<b,l,i,s>::m_numBits - 1)) & b(~0);
    lo <<= 1;
    lo &= (uint64(1) << lmFixed<b,l,i,s>::m_numBits) - 1;
  }

  do
  {
    // push in two more bits from the bottom
    // push up root by a bit.
    root <<= 1;

    test = (root << 1) + 1;

    if (hi >= test)
    {
      hi -= test;
      root |= 1;
    }

    hi = ((hi << 2) | (lo >> (lmFixed<b,l,i,s>::bits - 2))) & b(~0);
    lo <<= 2;
    lo &= (uint64(1) << lmFixed<b,l,i,s>::m_numBits) - 1;
  }
  while (count-- != 0);

  lmFixed <b,l,i,s> tmp;
  tmp.Raw() = root;
  return tmp;
}

END_L_NAMESPACE

#endif



