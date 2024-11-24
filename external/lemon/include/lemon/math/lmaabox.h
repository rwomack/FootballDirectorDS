//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmAABox.h
// Description : Axis-aligned bounding box
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMAABOX_H_INCLUDED
#define LMAABOX_H_INCLUDED

#include "../containers/larray.h"
#include "lmath.h"
#include "lmside.h"
#include "lmmatrix.h"
#include "lmplane.h"
#include "lmcapsule.h"

BEGIN_L_NAMESPACE

template <unsigned int N, typename T>
class lmSphere;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//##
//Axis-aligned bounding box template class
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
template<unsigned int N, typename T>
class lmAABox
{
public:
  lmMatrix<1,N,T> m_lower, m_upper;

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Constructors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Default constructor to empty box
  inline lmAABox()
	{
		Empty();
	}

  //##
  // Construct lmAABox from lower and upper vector
  inline lmAABox( const lmMatrix<1,N,T> &lower, const lmMatrix<1,N,T> &upper ) : m_lower(lower), m_upper(upper)
	{
	}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Static Construction methods
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Construct lmAABox from centre + extents
  // In:
  //   centre - centre of box
  //   extents - extents of box from one side to the other
  static lmAABox<N,T> FromCentreExtents( const lmMatrix<1,N,T> &centre, const lmMatrix<1,N,T> &extents  )
  {
    lmAABox<N,T> box;

    box.m_lower = centre - extents;
    box.m_upper = centre + extents;

    return box;
	}

  //##
  // Construct lmAABox from array of points
  // In:
  //   points - Array of points to construct lmAABox from
  static lmAABox<N,T> AroundPoints( const lArray< lmMatrix<1,N,T> > &points )
	{
    lmAABox<N,T> box;

		if(points.Size())
		{
      box.ExtendArray( points.Data(), points.ElementSize(), points.Size() );
    }

    return box;
	}

  //##
  // Construct lmAABox from array of points
  // In:
  //   points - Array of points to construct lmAABox from
  static lmAABox<N,T> AroundPoints( const void *data, int stride, int numPoints )
	{
    lmAABox<N,T> box;

    if(data != NULL && numPoints > 0)
		{
      box.ExtendArray( data, stride, numPoints );
    }

    return box;
	}

  //##
  // Construct lmAABox around sphere
  // In:
  //   sphere - sphere to create around
  static lmAABox<N,T> AroundSphere( const lmSphere<N,T> &sphere )
	{
    lmAABox<N,T> box;
    box.Extend( sphere );
    return box;
	}

  //##
  // Construct lmAABox around capsule
  // In:
  //   capsule - capsule to create around
  static lmAABox<N,T> AroundSphere( const lmCapsule &capsule )
	{
    lmAABox<N,T> box;
    box.Extend( capsule );
    return box;
	}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Serialization
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Serialize to a lStream object
  friend inline lStream &operator<<( lStream &ar, lmAABox &b )
  {
    ar << b.m_lower << b.m_upper;
    return ar;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Utility Logging
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Print a aabox to the log
  #ifdef LCORELOG_H_INCLUDED
  inline void Log(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_lower(i) );
    lLogFmt(" : ");
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_upper(i) );
    lLogFmt("\n");
  }
  #endif

  //##
  // Print a aabox to stdout
  inline void Logf(const char * str = NULL) const
  {
    if( str!=NULL ) lLogFmt("%s: ", str );
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_lower(i) );
    lLogFmt(" : ");
    for(int i=0;i<N;i++) lLogFmt("%6.3f ", (double)m_upper(i) );
    lLogFmt("\n");
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Empty / Extending
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Make the box empty
	inline void Empty()
	{
		for (unsigned int i = 0; i < N; i++)
		{
      m_lower(i) = LM_FLOAT_MAX;
      m_upper(i) = -LM_FLOAT_MAX;
		}
	}

  //##
  // Check if the box is empty
	inline bool IsEmpty() const
	{
		//if lower > upper, then the box is emtpy
		return m_lower(0) > m_upper(0);
  }

  //##
  // Extend the box to contain the point p
  // In:
  //   p - point to extend box around
  inline void Extend( const lmMatrix<1,N,T> & p )
	{
    if ( IsEmpty() )
		{
			m_lower = p;
			m_upper = p;
		}
		else
		{
      if(N == 3)
      {
        m_lower(0) = lmMin( m_lower(0), p(0) );
        m_lower(1) = lmMin( m_lower(1), p(1) );
        m_lower(2) = lmMin( m_lower(2), p(2) );
        m_upper(0) = lmMax( m_upper(0), p(0) );
        m_upper(1) = lmMax( m_upper(1), p(1) );
        m_upper(2) = lmMax( m_upper(2), p(2) );
      } else
      {
        for (unsigned int i = 0; i < N; i++)
        {
          m_lower(i) = lmMin( m_lower(i), p(i) );
          m_upper(i) = lmMax( m_upper(i), p(i) );
        }
      }
		}
	}

  //##
  // Extend the box to contain this other box
  // In:
  //   box - box to extend around
  inline void Extend( const lmAABox<N,T> &box )
	{
		for (unsigned int i = 0; i < N; i++)
		{
      m_lower(i) = lmMin( m_lower(i), box.m_lower(i) );
      m_upper(i) = lmMax( m_upper(i), box.m_upper(i) );
		}
	}

  //##
  // Extend the box to contain sphere
  // In:
  //   sphere - sphere to extend around
  inline void Extend( const lmSphere<N,T> &sphere )
	{
    lmMatrix<1,N,T> origin = sphere.GetOrigin();
    T               radius = sphere.GetRadius();

		for (unsigned int i = 0; i < N; i++)
		{
      m_lower(i) = lmMin( m_lower(i), origin(i) - radius );
      m_upper(i) = lmMax( m_upper(i), origin(i) + radius );
		}
	}

  //##
  // Extend the box to contain capsule (lmCapsule is 3d at the moment)
  // In:
  //   capsule - capsule to extend around
  inline void Extend( const lmCapsule &capsule )
	{
    T radius = capsule.GetRadius();

    for (unsigned int i = 0; i < 3; i++)
		{
      m_lower(i) = lmMin( m_lower(i), capsule.m_a(i) - radius );
      m_upper(i) = lmMax( m_upper(i), capsule.m_a(i) + radius );

      m_lower(i) = lmMin( m_lower(i), capsule.m_b(i) - radius );
      m_upper(i) = lmMax( m_upper(i), capsule.m_b(i) + radius );
		}
	}

  //##
  // Extend the box to contain array of points
  // In:
  //   points - Array of points to extend box around
  inline void ExtendArray( const void *data, int stride, int numPoints )
	{
    char *pointData = (char *) data;

    if(N == 3)
    {
      for (unsigned int i = 0; i < numPoints; i++)
      {
        T *point = (T *)pointData;

        m_lower(0) = lmMin( m_lower(0), point[0] );
        m_lower(1) = lmMin( m_lower(1), point[1] );
        m_lower(2) = lmMin( m_lower(2), point[2] );
        m_upper(0) = lmMax( m_upper(0), point[0] );
        m_upper(1) = lmMax( m_upper(1), point[1] );
        m_upper(2) = lmMax( m_upper(2), point[2] );

        pointData += stride;
      }
    } else
    {
      for (unsigned int i = 0; i < numPoints; i++)
      {
        T *point = (T *)pointData;

        for (unsigned int j = 0; j < N; j++)
        {
          m_lower(j) = lmMin( m_lower(j), point[j] );
          m_upper(j) = lmMax( m_upper(j), point[j] );
        }

        pointData += stride;
      }
    }
  }

  //##
  // Inflates dimensions if less than minimumDimensions
  // In:
  //   minimumDimensions - smallest dimensions to set box to
  inline void SetMinimumDimensions( const lmMatrix<1,N,T> &minimumDimensions )
  {
    for (unsigned int i = 0; i < N; i++)
    {
      T val = (m_upper(i) - m_lower(i)) - minimumDimensions(i);
      if(val < T(0.0))
      {
        val *= T(-0.5);
        m_lower(i) -= val;
        m_upper(i) += val;
      }
    }
  }

  //##
  // Inflates dimensions if less than minimumDimension
  // In:
  //   minimumDimensions - smallest dimensions to set box to
  inline void SetMinimumDimension( float minimumDimension )
  {
    for (unsigned int i = 0; i < N; i++)
    {
      T val = (m_upper(i) - m_lower(i)) - minimumDimension;

      if(val < T(0.0))
      {
        val *= T(-0.5);
        m_lower(i) -= val;
        m_upper(i) += val;
      }
    }
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Creation
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return a box containing the union of box1 + box2
  inline static lmAABox<N,T> MakeUnion( const lmAABox<N,T> &box1, const lmAABox<N,T> &box2 )
  {
    lmAABox<N,T> result( lmMatrix<1,N,T>::Minimum( box1.GetLower(), box2.GetLower() ),
                         lmMatrix<1,N,T>::Maximum( box1.GetUpper(), box2.GetUpper() ) );

    return result;
  }

  //##
  // Return a box containing the intersection of box1 + box2
  inline static lmAABox<N,T> MakeIntersection( const lmAABox<N,T> &box1, const lmAABox<N,T> &box2 )
  {
    lmAABox<N,T> result( lmMatrix<1,N,T>::Maximum( box1.GetLower(), box2.GetLower() ),
                         lmMatrix<1,N,T>::Minimum( box1.GetUpper(), box2.GetUpper() ) );

     return result;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Accessors
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Get Lower / Upper vectors of box
  inline const lmMatrix<1,N,T> & GetLower() const { return m_lower; }
  inline const lmMatrix<1,N,T> & GetUpper() const { return m_upper; }

  inline lmMatrix<1,N,T>       & GetLower()       { return m_lower; }
  inline lmMatrix<1,N,T>       & GetUpper()       { return m_upper; }

  //##
  // Set Lower / Upper vectors
  inline void SetLowerUpper( lmMatrix<1,N,T> &lower, lmMatrix<1,N,T> &upper )
  {
    m_lower = lower;
    m_upper = upper;
  }

  //##
  // Set size of box
  inline void SetSize(lmMatrix<1,N,T> &v)
	{
    m_lower = -v*0.5;
    m_upper = v*0.5;
  }

  //##
  // Get size of box
  inline lmMatrix<1,N,T> GetSize() const
	{
		return m_upper - m_lower;
	}

  //##
  // Get half size of box
  inline lmMatrix<1,N,T> GetHalfSize() const
	{
    return GetSize() * 0.5f;
	}

  //##
	// get the mid-point
  inline lmMatrix<1,N,T> GetCentre() const
	{
    return ( m_lower + m_upper )  *  T(0.5);
	}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Comparisons
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Compare two boxes
  // In:
  //   other - box to compare with
  inline bool EqualTo( const lmAABox<N,T> &other ) const
  {
    for( unsigned int i=0; i<N; i++)
    {
      if( m_upper(i) != other.m_upper(i) ) return false;
      if( m_lower(i) != other.m_lower(i) ) return false;
    }
    return true;
  }

  //##
  // Compare two boxes within tolerance epsilon
  // In:
  //   other - box to compare with
  //   tol - tolerance to use
  inline bool AlmostEqualTo( const lmAABox<N,T> &other, const T &tol ) const
  {
    for( unsigned int i=0; i<N; i++)
    {
      if( !lmAlmostEqualTo(m_upper(i), other.m_upper(i), tol ) ) return false;
      if( !lmAlmostEqualTo(m_lower(i), other.m_lower(i), tol ) ) return false;
    }
    return true;
  }

  //##
  //comparison operators
  inline bool operator==( const lmAABox<N,T> &other ) const
  {
    return EqualTo( other );
  }

  inline bool operator!=( const lmAABox<N,T> &other ) const
  {
    return !EqualTo( other );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Intersections and Containment
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Clamp a point to be within the box
  // In:
  //   v - point to clamp within box
  inline void ClampToBox( lmMatrix<1,N,T> &v ) const
  {
	  for (unsigned int i = 0; i < N; i++)
	  {
      v(i) = lmMax( m_lower(i), v(i) );
      v(i) = lmMin( m_upper(i), v(i) );
	  }
  }

  //##
  // Clamp the box to be below this point
  // In:
  //   v - point
  inline void ClampToBelow( const lmMatrix<1,N,T> &v )
	{
	  for (unsigned int i = 0; i < N; i++)
	  {
      m_lower(i) = lmMin( m_lower(i), v(i) );
      m_upper(i) = lmMin( m_upper(i), v(i) );
	  }
	}

  //##
  // Clamp this box to be above this point
  // In:
  //   v - point
  inline void ClampToAbove( const lmMatrix<1,N,T> &v )
	{
	  for (unsigned int i = 0; i < N; i++)
	  {
      m_lower(i) = lmMax( m_lower(i), v(i) );
      m_upper(i) = lmMax( m_upper(i), v(i) );
	  }
	}

  //##
  // Return true if the box contains this point
  inline bool Contains( const lmMatrix<1,N,T> &v ) const
	{
	  for (unsigned int i = 0; i < N; i++)
	  {
	    if ( v(i) < m_lower(i)  ||  v(i) > m_upper(i) )
	    {
	      return false;
	    }
	  }

	  return true;
	}

  //##
  // Return true if the box contains this sphere, false if only intersects or not in at all
  // TODO: test me
  inline int ContainsSphere( const lmMatrix<1,N,T> &p, T radius ) const
	{
    lmMatrix<1,N,T> offsetLower = p - m_lower;
    lmMatrix<1,N,T> offsetUpper = p - m_upper;

	  for (unsigned int i = 0; i < N; i++)
	  {
      if( offsetLower(i) < radius || offsetUpper(i) > -radius ) return false;  // sphere not fully inside box
	  }

    return true;
	}

  //##
  // Return true if the box contains this sphere, false if only intersects or not in at all
  // In:
  //   s - sphere to check
  inline int ContainsSphere( const lmSphere<N,T> &s ) const
  {
    return ContainsSphere( s.GetCentre(), s.GetRadius() );
  }

  //##
  // Return true if the box contains this capsule, false if only intersects or not in at all
  // TODO: test me
  inline int ContainsCapsule( lmCapsule &capsule ) const
	{
    if( !ContainsSphere( capsule.m_a, capsule.m_radius ) ) return false;
    if( !ContainsSphere( capsule.m_b, capsule.m_radius ) ) return false;
    return true;
	}

  //##
  // Return true if the box contains this other box
  inline bool ContainsBox(const lmAABox<N,T> &box) const
  {
    for (unsigned int i = 0; i < N; i++)
    {
      if ( box.m_lower(i) < m_lower(i)  ||  box.m_upper(i) > m_upper(i) )
      {
        return false;
      }
    }

    return true;
  }

  //##
  // Return true if this box intersects this other box
  inline bool IntersectsBox(const lmAABox<N,T> &box) const
	{
	  for (unsigned int i = 0; i < N; i++)
	  {
	    if ( box.m_upper(i) < m_lower(i)  ||  box.m_lower(i) > m_upper(i) )
	    {
	      return false;
	    }
	  }

	  return true;
	}

  //##
  // Return true if this sphere intersects this box
  // TODO: this can return false positives - should use closest point and test with that
  inline bool IntersectsSphere( const lmMatrix<1,N,T> &p, T radius ) const
	{
    lmMatrix<1,N,T> offsetLower = p - m_lower;
    lmMatrix<1,N,T> offsetUpper = p - m_upper;

	  for (unsigned int i = 0; i < N; i++)
	  {
      if( offsetLower(i) < -radius || offsetUpper(i)  > radius ) return false;  // sphere not inside box at all
	  }

    return true;
	}

  //##
  // Return true if this sphere intersects this box
  inline bool IntersectsSphere( const lmSphere<N,T> &s ) const
  {
    return IntersectsSphere( s.GetCentre(), s.GetRadius() );
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Translate box by v
  inline void Translate(const lmMatrix<1,N,T> &v)
	{
	  m_lower += v;
	  m_upper += v;
	}

  //##
  // Scale this box from the centre by s
  inline void ScaleFromCentre( T s )
  {
    lmMatrix<1,N,T> centre = ( m_lower + m_upper ) * T(0.5);
    m_lower = centre + ((m_lower - centre) * s);
    m_upper = centre + ((m_upper - centre) * s);
  }

  //##
  // Transform this box by m
  template<unsigned int P,unsigned int Q>
  inline void Transform(const lmMatrix<P,Q,T> &m)
  {
    lmAABox<N,T> result;

    unsigned int numberOfCorners = GetNumberOfCorners();
    for (unsigned int i = 0; i < numberOfCorners; i++)
    {
      result.Extend( GetCorner( i ) * m );
    }

    (*this) = result;
  }

  //##
  // Transform this box by m and translate by p
  inline void Transform(const lmMatrix<1,N,T> &p, const lmMatrix<N,N,T> &m)
  {
    lmAABox<N,T> result;

    unsigned int numberOfCorners = GetNumberOfCorners();
    for (unsigned int i = 0; i < numberOfCorners; i++)
    {
      result.Extend( GetCorner( i ) * m );
    }
    result.Translate( p );
    (*this) = result;
  }


  ////##
  //// Transform this box by m and translate by p
  //inline void Transform2( const lmMatrix<1,N,T> &p, const lmMatrix<N,N,T> &m )
  //{
  //  lmMatrix<1,N,T> lower, upper;
  //  lower = upper = p;
  //
  //  for(int i = 0; i < N; i++)
  //  {
  //    // add in extreme values obtained by computing product of min + maxes w/ matrix
  //    for(int j = 0; j < N; j++)
  //    {
  //      float _a = m(i,j) * m_lower(i);
  //      float _b = m(i,j) * m_upper(i);
  //
  //      if(_a < _b)
  //      {
  //        lower(i) += _a;
  //        upper(i) += _b;
  //      } else
  //      {
  //        lower(i) += _b;
  //        upper(i) += _a;
  //      }
  //    }
  //  }
  //
  //  m_lower = lower;
  //  m_upper = upper;
  //}
  //
  ////##
  //// Transform this box by m
  //template<unsigned int P,unsigned int Q>
  //inline void Transform2( const lmMatrix<P,Q,T> &m )
  //{
  //  lmMatrix<1,N,T> lower, upper;
  //  lower = upper = m.GetTranslate();
  //
  //  for(int i = 0; i < P-1; i++)
  //  {
  //    // add in extreme values obtained by computing product of min + maxes w/ matrix
  //    for(int j = 0; j < Q-1; j++)
  //    {
  //      float _a = m(i,j) * m_lower(i);
  //      float _b = m(i,j) * m_upper(i);
  //
  //      if(_a < _b)
  //      {
  //        lower(i) += _a;
  //        upper(i) += _b;
  //      } else
  //      {
  //        lower(i) += _b;
  //        upper(i) += _a;
  //      }
  //    }
  //  }
  //
  //  m_lower = lower;
  //  m_upper = upper;
  //}

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Transformation operators
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  template<unsigned int P,unsigned int Q> inline const lmAABox operator*  (const lmMatrix<P,Q,T>& m) const { lmAABox<N,T> r = (*this); r.Transform(m); return r;  }
  template<unsigned int P,unsigned int Q> inline void          operator*= (const lmMatrix<P,Q,T>& m)       { Transform(m); }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Misc information extraction
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Return the volume of the box
  // TODO:   return ((m_upper - m_lower) * 2.0f).SqrLength();
  inline T GetVolume() const
  {
    float volume = 1.0f;
    for (int i = 0; i < N; i++)
    {
      volume *= ( m_upper(i) - m_lower(i) );
    }

    return volume;
  }

  //##
  // Get the radius of the sphere, centred on the centre of the box,
  // that would **fit inside the largest dimension** of the box
  inline T GetInnerRadius() const
	{
    lmMatrix<1,N,T> d = m_upper - m_lower;

    T result = d(0);

    for (unsigned int i = 1; i < N; i++)
    {
      result = lmMax( result, d(i) );
    }

    return result*0.5f;
  }

  //##
  // Get the radius of the sphere, centred on the centre of the box,
  // that would **fit inside the smallest dimension** of the box
  inline T GetSmallestInnerRadius() const
	{
    lmMatrix<1,N,T> d = m_upper - m_lower;

    T result = d(0);

    for (unsigned int i = 1; i < N; i++)
    {
      result = lmMin( result, d(i) );
    }

    return result*0.5f;
  }


  //##
  // Get the radius of the sphere, centred on the centre of the box,
  // that would enclose the box
	inline T GetRadius() const
	{
    return GetHalfSize().Length();
	}

  //##
  // get the number of corners
	inline unsigned int GetNumberOfCorners() const
	{
		return 1 << N;
	}

  //##
  // Get the corner specified by index
  inline lmMatrix<1,N,T> GetCorner(unsigned int index) const
	{
    lmMatrix<1,N,T> point;

		unsigned int mask = 1;
		for (unsigned int i = 0; i < N; i++)
		{
      point(i) = ( ( index & mask )  != 0 )  ?  m_upper(i)  :  m_lower(i);
      mask = mask << 1;
		}

		return point;
	}

  //##
  // Get the number of faces
  inline unsigned int GetNumberOfFaces() const
  {
    return N << 1;
  }

  //##
  // Get the normal of the face specified by the index; NOTE; normals point INWARDS
  inline lmMatrix<1,N,T> GetFaceNormal(unsigned int faceIndex) const
  {
    lmMatrix<1,N,T> normal( 0.0f, 0.0f, 0.0f );

    unsigned int axis = faceIndex >> 1;
    normal(axis) = ( faceIndex & 0x1 )  !=  0   ?   T(-1.0)  :  T(1.0);

    return normal;
  }

  //##
  // Get the plane of the face specified by the index
  inline lmPlane<N,T> GetFacePlane(unsigned int faceIndex) const
  {
    lmMatrix<1,N,T> normal = GetFaceNormal( faceIndex );

    unsigned int axis = faceIndex >> 1;
    float normalDotPoint = ( faceIndex & 0x1 )  !=  0   ?   -m_upper(axis)  :  m_lower(axis);

    return lmPlane<N,T>( normal, -normalDotPoint );
  }


  //##
  // Get face plane to point distance
  inline T GetFacePlaneToPointDist(unsigned int faceIndex, const lmMatrix<1,N,T> &point) const
  {
    unsigned int axis = faceIndex >> 1;
    T facePoint = ( faceIndex & 0x1 )  !=  0   ?   m_upper(axis)  :  m_lower(axis);

    return lmFabs( point(axis) - facePoint );
  }

  //##
  // Get the leading point along a vector
  inline lmMatrix<1,N,T> GetLeadingPoint(const lmMatrix<1,N,T> &vector) const
	{
    lmMatrix<1,N,T> point;

		for (unsigned int i = 0; i < N; i++)
		{
      point(i) = vector(i) > T(0.0)  ?  m_upper(i)  :  m_lower(i);
		}

		return point;
	}

  //##
  // Get the trailing point along a vector
  inline lmMatrix<1,N,T> GetTrailingPoint(const lmMatrix<1,N,T> &vector) const
	{
    lmMatrix<1,N,T> point;

		for (unsigned int i = 0; i < N; i++)
		{
      point(i) = vector(i) > T(0.0)  ?  m_lower(i)  :  m_upper(i);
		}

		return point;
	}

  //##
  // Get side of plane that box is on
  inline lmSide GetSide(const lmPlane<N,T> &plane)
	{
    lmMatrix<1,N,T> leading  = GetLeadingPoint( plane.GetNormal() );
    lmMatrix<1,N,T> trailing = GetTrailingPoint( plane.GetNormal() );

    lmSide leadingSide = plane.Side( leading );
    lmSide trailingSide = plane.Side( trailing );

		if ( leadingSide == trailingSide )
		{
			return leadingSide;
		}
		else
		{
      return LMSIDE_BOTH;
		}
	}

  //##
  // Get the major axis of box
  inline int GetMajorAxis() const
  {
    lmMatrix<1,N,T> size = GetSize();

    T maxDimension = size(0);
    int majorAxis = 0;

    for (int i = 1; i < N; i++)
    {
      if ( size(i) > maxDimension )
      {
        maxDimension = size(i);
        majorAxis = i;
      }
    }

    return majorAxis;
  }

  //##
  // Get the minor axis of box
  inline int GetMinorAxis() const
  {
    lmMatrix<1,N,T> size = GetSize();

    T minDimension = size(0);
    int minorAxis = 0;

    for (int i = 1; i < N; i++)
    {
      if ( size(i) < minDimension )
      {
        minDimension = size(i);
        minorAxis = i;
      }
    }

    return minorAxis;
  }

  //##
  // Get closest distance from box to point
  inline lmMatrix<1,N,T> ClosestPoint(const lmMatrix<1,N,T> &point) const
  {
    lmMatrix<1,N,T> closestPoint;

    for (unsigned int i = 0; i < N; i++)
    {
      float v = point(i);

      if ( v < m_lower(i) )
      {
        v = m_lower(i);
      }
      else if ( point(i) > m_upper(i) )
      {
        v = m_upper(i);
      }

      closestPoint(i) = v;
    }

    return closestPoint;
  }

  //##
  // Get closest square distance and closest point from box to point
  inline T SqrDistanceToClosestPoint(const lmMatrix<1,N,T> &point, lmMatrix<1,N,T> &closestPoint) const
  {
    T sqrDistance = T( 0.0 );

    for (unsigned int i = 0; i < N; i++)
    {
      float v = point(i);

      if ( v < m_lower(i) )
      {
        sqrDistance += (m_lower(i) - v) * (m_lower(i) - v);
        v = m_lower(i);
      }
      else if ( v > m_upper(i) )
      {
        sqrDistance += (v - m_upper(i)) * (v - m_upper(i));
        v = m_upper(i);
      }

      closestPoint(i) = v;
    }

    return sqrDistance;
  }

  //##
  // Get closest distance and closest point from box to point
  inline T DistanceToClosestPoint(const lmMatrix<1,N,T> &point, lmMatrix<1,N,T> &closestPoint) const
  {
    return lmSqrt( SqrDistanceToClosestPoint( point, closestPoint ) );
  }

  //##
  //compute the intersection of the faces of @this with an outgoing ray
  void ComputeOutgoingRayIntersection(const lmVector3 &rayOrigin, const lmVector3 &rayVector,
                                      T &t, unsigned int &faceIndex) const
  {
    lAssert( Contains( rayOrigin ) );

    faceIndex = 0;
    bool bFound = false;

    for (unsigned int i = 0; i < N; i++)
    {
      T r = rayVector(i);

      if ( r != T(0.0) )
      {
        T thisT;
        unsigned int faceSign;

        if ( r < T(0.0) )
        {
          thisT = ( m_lower(i) - rayOrigin(i) )  /  r;
          faceSign = 0;
        }
        else if ( r > T(0.0) )
        {
          thisT = ( m_upper(i) - rayOrigin(i) )  /  r;
          faceSign = 1;
        }

        if ( !bFound  ||  thisT < t )
        {
          t = thisT;
          faceIndex = i * 2  +  faceSign;
          bFound = true;
        }
      }
    }
  }

  //##
  // Is box adjacent to this other box along the specified axis (using the epsilon)
  inline bool IsAdjacent( int axis, lmAABox<N,T> &otherBox, float epsilon = LM_EPSILON )
  {
    if( lmFabsf( otherBox.GetLower()( axis ) - GetUpper()( axis ) ) < epsilon ||
        lmFabsf( otherBox.GetUpper()( axis ) - GetLower()( axis ) ) < epsilon )
    {
      int nextaxis[3] = { 1, 2, 0 };

      int axis2 = nextaxis[ axis ];
      int axis3 = nextaxis[ axis2 ];

      if((GetUpper()( axis2 ) < otherBox.GetLower()( axis2 )) || (GetLower()( axis2 ) > otherBox.GetUpper()( axis2 ))) return false;
      if((GetUpper()( axis3 ) < otherBox.GetLower()( axis3 )) || (GetLower()( axis3 ) > otherBox.GetUpper()( axis3 ))) return false;
      return true;
    }

    return false;
  }

	//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  // Misc operations
  //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

  //##
  // Split the box into two boxes along specified axis
  inline void Split( int splitAxis, lmAABox<N,T> &outBox1, lmAABox<N,T> &outBox2 )
  {
    // copy this to two boxes
    outBox1 = *this;
    outBox2 = *this;

    // do split

    float splitPoint = m_lower(splitAxis) + ((m_upper(splitAxis) - m_lower(splitAxis)) * 0.5f);

    outBox1.GetUpper()(splitAxis) = splitPoint;
    outBox2.GetLower()(splitAxis) = splitPoint;
  }
};

//##
// Serialize to a lStream object
template<unsigned int N, typename T>
inline lStream &operator<<( lStream &ar, lmAABox<N,T> &b )
{
  ar << b.m_lower << b.m_upper;
  return ar;
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Typedefs
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef lmAABox<2,float> lmAABox2;
typedef lmAABox<3,float> lmAABox3;

typedef lmAABox<2,double> lmAABox2l;
typedef lmAABox<3,double> lmAABox3l;

END_L_NAMESPACE
#endif

