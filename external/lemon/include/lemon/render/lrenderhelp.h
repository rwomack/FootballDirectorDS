//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrenderhelp.h
// Description : Rendering Helpers
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrender.h"
#include "../math/lmray.h"

#ifndef LRENDERUTILS_H_INCLUDED
#define LRENDERUTILS_H_INCLUDED
BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrColour + helpers
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//NOTE:
//  These functions should specify the type of packed colour being referred to
//  we usually treat lrColour as (R,G,B,A)

//##
// lrColour is a lmVector4 which is configured as such (x,y,z,w)=(r,g,b,a)
typedef lmVector4 lrColour;

//##
// This function converts an uint32 value colour (ARGB8888) to a lrColour
inline lrColour lrColourFromPackedColour( const unsigned int packedColour )
{
  return lrColour( ((packedColour>>16)&0xff) / 255.0f,
                     ((packedColour>>8)&0xff) / 255.0f,
                          (packedColour&0xff) / 255.0f,
                    ((packedColour>>24)&0xff) / 255.0f );
}

//##
// This function converts a lrColour into a uint32 value colour (ARGB8888)
inline unsigned int lrPackedColourFromColour( const lrColour &col )
{
  unsigned int outcol = ((unsigned int)(((int)(col(3)*255)<<24) | ((int)(col(0)*255)<<16) | ((int)(col(1)*255)<<8) | (int)(col(2)*255) ));
  return outcol;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderHelp class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lRenderHelp
{
public:
  static void MatrixPush( const lmMatrix4 &m );
  static void MatrixPrecatPush(const lmMatrix4 &m);
  static void MatrixDirectPush(const lmMatrix4 &m);
  static void MatrixPop();
  static void MatrixTop(lmMatrix4 &m);
private:
  static lArray<lmMatrix4,TRenderArrayTrait> s_stack;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Compatability classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lrPackedNormal
{
  lrPackedNormal() { }
  lrPackedNormal( const lmVector3 &v ) { SetVector(v); }
  lrPackedNormal( float x, float y, float z ) { SetVector(lmVector3(x,y,z)); }

  void Init( float x, float y, float z ) { SetVector(lmVector3(x,y,z)); }

  #ifdef L_PLATFORM_XBOX

  // XBox packed normal support

  enum
  {
    DESC_TYPE = lrVertexDesc::TYPE_NORMPACKED3,
    DESC_SIZE = 4,
  };

  uint32 value;

  inline void SetVector( const lmVector3 &v )
  {
    value = ( ( ( (uint32)(v(2) * 511.0f) )  & 0x3ff ) << 22L ) |
            ( ( ( (uint32)(v(1) * 1023.0f) ) & 0x7ff ) << 11L ) |
            ( ( ( (uint32)(v(0) * 1023.0f) ) & 0x7ff ) <<  0L );
  }

  inline lmVector3 GetVector() const
  {
    lmVector3 v;
    v(0) = float((value>>0)&0x7ff) / 1023.0f;
    v(1) = float((value>>11)&0x7ff) / 1023.0f;
    v(2) = float((value>>22)&0x3ff) / 511.0f;
    return v;
  }

  friend lStream& operator<< ( lStream& ar, lrPackedNormal &n )
  {
    if( ar.IsLoading() )
    {
      lmVector3 v;
      ar << v;
      n.SetVector(v);
    }
    else
    {
      lmVector3 v = n.GetVector();
      ar << v;
    }
    return ar;
  }

  #else

  // Win32

  enum
  {
    DESC_TYPE = lrVertexDesc::TYPE_FLOAT3,
    DESC_SIZE = 12,
  };

  lmVector3 value;

  inline void SetVector( const lmVector3 &v )
  {
    value = v;
  }

  inline lmVector3 GetVector() const
  {
    return value;
  }

  friend lStream& operator<< ( lStream& ar, lrPackedNormal &n )
  {
    return ar << n.value;
    return ar;
  }

  #endif
};

struct LRENDER_API lrVertex
{
  lrVertex() {}
  lrVertex( const lmVector3Packed &p ) : p(p), n(lmVector3Packed(0,0,0)), colour(0), u(0), v(0) {}
  lrVertex( const lmVector3Packed &p, const lmVector3Packed &n, unsigned int  colour = 0, float u = 0, float v = 0 ) : p(p), n(n), colour(colour), u(u), v(v) {}
  lrVertex( float px, float py, float pz, float nx=0, float ny=0, float nz=0, unsigned int  colour=0, float u=0, float v=0 ) : p(px,py,pz), n(nx,ny,nz), colour(colour), u(u), v(v) {}

  lmVector3Packed    p;
  lmVector3Packed    n;
  unsigned int  colour;
  float         u,v;

  // 36 byte vertex format

  friend lStream& operator<< ( lStream& ar, lrVertex &v )
  {
    ar << v.p << v.n << v.colour << v.u << v.v;
    return ar;
  }

  void Transform(const lmMatrix4 &m)
  {
    p = lmVector3(p)*m;
    n = n.SubMul3(m);
  }

  static lrVertexDesc & GetDesc();
};

struct LRENDER_API lrVertexUV2
{
  lrVertexUV2() {}
  lrVertexUV2( lmVector3Packed &p ) : p(p), n(lmVector3(0,0,0)), colour(0), u(0), v(0), u2(0), v2(0) {}
  lrVertexUV2( lmVector3Packed &p, lmVector3Packed &n, unsigned int  colour = 0, float u = 0, float v = 0, float u2 = 0, float v2 = 0 ) :
                  p(p), n(n), colour(colour), u(u), v(v), u2(u2), v2(v2) {}
  lrVertexUV2( float px, float py, float pz, float nx=0, float ny=0, float nz=0, unsigned int  colour=0, float u=0, float v=0, float u2 = 0, float v2 = 0 ) :
                  p(px,py,pz), n(nx,ny,nz), colour(colour), u(u), v(v), u2(u2), v2(v2) {}

  lmVector3Packed    p;
  lmVector3Packed    n;
  unsigned int  colour;
  float         u,v;
  float         u2,v2;
  // 44 byte vertex format

  friend lStream& operator<< ( lStream& ar, lrVertexUV2 &v )
  {
    ar << v.p << v.n << v.colour << v.u << v.v << v.u2 << v.v2;
    return ar;
  }

  void Transform(const lmMatrix4 &m)
  {
    p = lmVector3(p)*m;
    n = n.SubMul3(m);
  }

  static lrVertexDesc & GetDesc();
};

struct lrTri
{
  lrTri() {}
  lrTri( unsigned short i1, unsigned short i2, unsigned short i3 ) : i1(i1), i2(i2), i3(i3) {}
  unsigned short i1;
  unsigned short i2;
  unsigned short i3;

  inline void Flip()
  {
    lSwap( i1, i2 );
  }

  inline void OffsetIndices(unsigned short offset)
  {
    i1 = (unsigned short)(i1 + offset);
    i2 = (unsigned short)(i2 + offset);
    i3 = (unsigned short)(i3 + offset);
  }

  friend lStream& operator<< ( lStream& ar, lrTri &v ) { ar << v.i1 << v.i2 << v.i3; return ar; }
};

template<typename VT> struct lrTriset
{
  lrVertexStream<VT,sizeof(VT)>  vertList;
  lrIndexStream<lrTri,2>       triList;

  void Append( lrTriset<VT> &triset )
  {
    vertList.Append( triset.vertList );
    triList.Append( triset.triList );
  }

  void Transform( const lmMatrix4 &m )
  {
    for( int i=0;i< vertList.Size(); i++ ) vertList(i).Transform(m);
    vertList.Dirty();
  }

  float TriArea( int i1,int i2,int i3 ) const
  {
    lmVector3 a = (vertList(i2).p-vertList(i1).p);
    lmVector3 b = (vertList(i3).p-vertList(i1).p);
    lmVector3 n = a.Cross(b);
    return lmSqrt( n.Dot(n) ) / 2.0;
  }

  float TriArea( const lrTri &tri ) const
  {
    return TriArea(tri.i1,tri.i2,tri.i3);
  }

  void FlipTris()
  {
    for (int i = 0; i < triList.Size(); i++)
    {
      triList(i).Flip();
    }
  }

  void Merge(const lrTriset<VT> &triset, bool bFlipTris)
  {
    lAssert( vertList.Size() + triset.vertList.Size()  <  65536 );

    unsigned short offset = vertList.Size();

    int firstTri = triList.Size();

    vertList.Append( triset.vertList );
    triList.Append( triset.triList );

    for (int i = firstTri; i < triList.Size(); i++)
    {
      triList(i).OffsetIndices( offset );
      if ( bFlipTris )
      {
        triList(i).Flip();
      }
    }
  }
};

template <typename VT>
inline lStream& operator<< ( lStream& ar, lrTriset<VT> &v ) { ar << v.vertList << v.triList; return ar; }

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Large index triset
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lrBigTri
{
  lrBigTri() {}
  lrBigTri( unsigned int i1, unsigned int i2, unsigned int i3 ) : i1(i1), i2(i2), i3(i3) {}
  unsigned int i1;
  unsigned int i2;
  unsigned int i3;

  inline void Flip()
  {
    lSwap( i1, i2 );
  }

  inline void OffsetIndices(unsigned int offset)
  {
    i1 += offset;
    i2 += offset;
    i3 += offset;
  }

  friend lStream& operator<< ( lStream& ar, lrBigTri &v ) { ar << v.i1 << v.i2 << v.i3; return ar; }
};

template<typename VT> struct lrBigTriset
{
  lArray<VT>        vertList;
  lArray<lrBigTri> triList;

  void Transform( const lmMatrix4 &m )
  {
    for( int i=0;i< vertList.Size(); i++ ) vertList(i).Transform(m);
    vertList.Dirty();
  }

  float TriArea( int i1,int i2,int i3 ) const
  {
    lmVector3 a = (vertList(i2).p-vertList(i1).p);
    lmVector3 b = (vertList(i3).p-vertList(i1).p);
    lmVector3 n = a.Cross(b);
    return lmSqrt( n.Dot(n) ) / 2.0;
  }

  float TriArea( const lrTri &tri ) const
  {
    return TriArea(tri.i1,tri.i2,tri.i3);
  }

  void FlipTris()
  {
    for (int i = 0; i < triList.Size(); i++)
    {
      triList(i).Flip();
    }
  }

  void Merge(const lrBigTriset<VT> &triset, bool bFlipTris)
  {
    unsigned int offset = vertList.Size();

    int firstTri = triList.Size();

    vertList.Append( triset.vertList );
    triList.Append( triset.triList );

    for (int i = firstTri; i < triList.Size(); i++)
    {
      triList(i).OffsetIndices( offset );
      if ( bFlipTris )
      {
        triList(i).Flip();
      }
    }
  }

  void AddSmall( lrTriset<VT> &triset )
  {
    unsigned int firstVertex = vertList.Size();
    unsigned int firstTri    = triList.Size();

    vertList.Add( triset.vertList.Size() );
    triList.Add( triset.triList.Size() );

    for( int j=0;j<triset.vertList.Size();j++ )
    {
      vertList(firstVertex+j) = triset.vertList(j);
    }

    for( int j=0;j<triset.triList.Size();j++ )
    {
      triList(firstTri+j).i1 = triset.triList(j).i1;
      triList(firstTri+j).i2 = triset.triList(j).i2;
      triList(firstTri+j).i3 = triset.triList(j).i3;
    }

    for (int j = firstTri; j < triList.Size(); j++)
    {
      triList(j).OffsetIndices( firstVertex );
    }
  }

  void CombineSmall( lArray< lrTriset<VT> > &trisetList )
  {
    for( int i=0;i<trisetList.Size(); i++ )
    {
      lrTriset<VT> &triset = trisetList(i);
      AddSmall( triset );
    }
  }

  void SplitSmall( lArray< lrTriset<VT> > &trisetList )
  {
    unsigned int numTrisLeft = triList.Size();

    while( numTrisLeft > 0 )
    {
      trisetList.Add();
      lrTriset<VT> &triset = trisetList.Last();
      lArray<int>   vertMap;

      // try and split into triset of about 30000

      while( numTrisLeft > 0 && triset.vertList.Size() < 30000 && triset.triList.Size() < 30000 )
      {
        // add this triangles vertices

        int indices[3];
        indices[0] = triList(triList.Size()-numTrisLeft).i1;
        indices[1] = triList(triList.Size()-numTrisLeft).i2;
        indices[2] = triList(triList.Size()-numTrisLeft).i3;

        int newIndices[3];

        for( int j=0;j<3;j++ )
        {
          unsigned int origVertexIndex = indices[j];
          unsigned int newVertexIndex  = -1;

          // see if we've already added this vertex

          int k;
          for( k=0;k<vertMap.Size();k++ )
          {
            if( vertMap(k) == origVertexIndex )
            {
              newVertexIndex = k;
              break;
            }
          }

          if( k>=vertMap.Size() )
          {
            // add this vertex

            newVertexIndex = triset.vertList.Add(1);
            vertMap.Push( origVertexIndex );
            triset.vertList(newVertexIndex) = vertList(origVertexIndex);

            lAssert( vertMap.Size() == triset.vertList.Size() );
          }

          lAssert( newVertexIndex != -1 );

          newIndices[j] = newVertexIndex;
        }

        triset.triList.Add();
        triset.triList.Last().i1 = newIndices[0];
        triset.triList.Last().i2 = newIndices[1];
        triset.triList.Last().i3 = newIndices[2];

        numTrisLeft--;
      }
    }




  }

};

template <typename VT>
inline lStream& operator<< ( lStream& ar, lrBigTriset<VT> &v ) { ar << v.vertList << v.triList; return ar; }




//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrCamera - Camera helper
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lrCamera
{
  lmMatrix4 localToWorld;
  lmMatrix4 projectionMatrix;

  lrCamera()
  {
    Default();
  }

  void Default()
  {
    SetProjection( 90, 1.3f, 1, 3000 );
    localToWorld.Identity();
  }

  void Select()
  {
    lRender::TransformSet( LR_TRANSFORM_PROJECTION, projectionMatrix );
    lRender::TransformSet( LR_TRANSFORM_VIEW, localToWorld.OrthoInverse() );
  }

  void SetProjection( float fovDegrees, float aspect, float front, float back )
  {
    float _2fovRad = lmDeg2Rad(fovDegrees)/2.0f;
    float w = lmSin(_2fovRad) / lmCos(_2fovRad);
    float h = w / aspect;

    // w = cos(fov/2) / sin(fov/2);
    // h = w / aspect;
    //
    // 1/w     0       0               0
    // 0       1/h     0               0
    // 0       0       bk/(bk-ft)      1
    // 0       0       (ft*bk)/(ft-bk) 0

    float Q = back/(back - front);

    projectionMatrix.Zero();
    projectionMatrix(0,0) = (1.0f/w);
    projectionMatrix(1,1) = (1.0f/h);
    projectionMatrix(2,2) = Q;
    projectionMatrix(2,3) = 1;
    projectionMatrix(3,2) = -Q*front; // == -(bk/(bk-ft)) * ft == (ft*bk)/(ft-bk)

    // which is derived from:
    //
    //{
    //  float _2fovRad = lmDeg2Rad(fovDegrees)/2.0f;
    //  float D        = front; // focal length
    //  float F        = back;  // back plane
    //  float h        = D * (lmSin(_2fovRad) / lmCos(_2fovRad));
    //
    //  projectionMatrix.Zero();
    //  projectionMatrix(0,0) = D / h;
    //  projectionMatrix(1,1) = (D / h) * aspect;
    //  projectionMatrix(2,2) = (h*F) / (D*(F-D));
    //  projectionMatrix(2,3) = (-h*F) / (F-D);
    //  projectionMatrix(3,2) = h/D;
    //}

    // Notes:
    // Swapping near and far values inverts the z-buffer range.
    // This can also be done by concatenating with:
    //   1   0   0   0
    //   0   1   0   0
    //   0   0  -1   0
    //   0   0   1   1
    //
    // To scale the z-buffer range of the projection matrix,
    // add zmin to Q at element (2,2) and multiply by the range
    // in element (3,2).
    //   m(2,2) = Q+zmin;
    //   m(3,2) = (-Q*front)*(zmax-zmin);
    //
    // Or concatenate with:
    //   1   0   0         0
    //   0   1   0         0
    //   0   0   zmax-zmin 0
    //   0   0   zmin      1
  }

  void SetOrthoProjection( float w, float h, float front, float back )
  {
    // 2/w  0    0           0
    // 0    2/h  0           0
    // 0    0    1/(bk-ft)   0
    // 0    0    ft/(ft-bk)  1

    projectionMatrix.Zero();
    projectionMatrix(0,0) = 2/w;
    projectionMatrix(1,1) = 2/h;
    projectionMatrix(2,2) = 1/(back-front);
    projectionMatrix(3,3) = 1;
    projectionMatrix(3,2) = front/(front-back);
  }

  lmRay3 EyeRay() const
  {
    return lmRay3( localToWorld.GetTranslate(), localToWorld.GetRow<2>(), 1.0f );
  }

  lmRay3 PixelRay(float x, float y, float xRes, float yRes) const
  {
    float nx = 2.0f * x / xRes  -  1.0f;
    float ny = -( 2.0f * y / yRes  -  1.0f );

    lmVector4 screenSpace( nx, ny, 0.0, 1.0f );
    lmVector4 viewSpace = screenSpace * projectionMatrix.Inverse();
    lmVector4 ws = viewSpace * localToWorld;
    float oneOverW = 1.0f / ws(3);
    lmVector3 worldSpace( ws(0) * oneOverW, ws(1) * oneOverW, ws(2) * oneOverW );

    lmVector3 camPosition = localToWorld.GetTranslate();

    return lmRay3( camPosition, worldSpace - camPosition, 1.0f );
  }

  float GetAspect()
  {
    return projectionMatrix(0,0) * projectionMatrix(1,1);
  }

  // Find the Z where the given screen plane fits within the frustum.

  float FitScreenPlaneZ( float screenWidth, float screenHeight )
  {
    float ooTanFov2W = projectionMatrix(0,0);             // 1/tan(fov/2)
    float zPlaneW    = (screenWidth * 0.5f) * ooTanFov2W; // (screenWidth/2) / tan(fov/2)

    float ooTanFov2H = projectionMatrix(1,1);              // 1/tan(fov/2)
    float zPlaneH    = (screenHeight * 0.5f) * ooTanFov2H; // (screenHeight/2) / tan(fov/2)

    if( zPlaneW > zPlaneH ) return zPlaneW;
    return zPlaneH;
  }


};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// TIMJ-HACK lrCameraOrbit - lrCameraController with mouse orbit functionality
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lrCameraOrbit : public lrCamera
{
  public:
  lrCameraOrbit() :
    _rotation(0,0,0), _curRotation(_rotation),
    _position(0,0,0), _curPosition(_position),
    _delta(0,0,0),
    _offsetDist(5)
  { _player.Identity(); }
  ~lrCameraOrbit() { }

  lmMatrix4 &Matrix() { return _player; }

  void Update( float deltaTime )
  {
    _position.Init( _player(3,0), _player(3,1), _player(3,2));

    deltaTime *= 10.0f;
    if( deltaTime > 1 ) deltaTime = 1;
    if( deltaTime < 0 ) deltaTime = 0;

    lmMatrix4 mat;
    mat.Identity();
    mat.Rotate( _rotation );
    _position += _delta;
    _delta.Init( 0,0,0 );

    lmVector3 offset( mat(2,0), mat(2,1), mat(2,2) );
    offset *= -_offsetDist;

    lmVector3 deltaRot = _rotation - _curRotation;
    _curRotation += deltaRot * deltaTime;

    lmVector3 deltaPos = (_position+offset) - _curPosition;
    _curPosition += deltaPos * deltaTime;

    mat.Identity();
    mat.Rotate( _curRotation );
    mat.Translate( _curPosition + offset );

    this->localToWorld = mat;
  }

  //private:
    lmMatrix4 _player;
    lmVector3 _rotation, _curRotation;
    lmVector3 _position, _curPosition;
    lmVector3 _delta;
    float      _offsetDist;
};




//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Colour lerp
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline unsigned int lrLerpPackedColour(float t, unsigned int c1, unsigned int c2)
{
  unsigned int result;
  unsigned char *col1 = (unsigned char*)&c1;
  unsigned char *col2 = (unsigned char*)&c2;
  unsigned char *res = (unsigned char*)&result;

  res[0] = (unsigned char)lmLerp( t, (float)col1[0], (float)col2[0] );
  res[1] = (unsigned char)lmLerp( t, (float)col1[1], (float)col2[1] );
  res[2] = (unsigned char)lmLerp( t, (float)col1[2], (float)col2[2] );
  res[3] = (unsigned char)lmLerp( t, (float)col1[3], (float)col2[3] );

  return result;
}

END_L_NAMESPACE

#endif
