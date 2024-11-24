//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lstream.h
// Description : Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSTREAM_H_INCLUDED
#define LSTREAM_H_INCLUDED

#include "./lioapi.h"
#include "../object/lobjectapi.h"
#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

class LOBJECT_API lObject;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lStream - Stream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lStream
{
public:
  virtual ~lStream() {};
  virtual void      Serialize( void *, unsigned int size ) = 0;
  virtual lObject * SerializeObject( lObject * ) = 0;
  virtual bool      IsLoading(void) = 0;
  virtual bool      IsSaving(void) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Basic achive operators for streams
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Note: bool is converted to a signed char as it may
//       vary in size on different platforms.

//TODO: Endian shouldn't be swapped after on save
inline lStream& operator<< ( lStream& stream, bool & c )           { uint8 x = c ? 1 : 0; stream.Serialize( &x, 1 ); c = (x == 0) ? false : true; return stream; }
inline lStream& operator<< ( lStream& stream, uint8 & c )          { stream.Serialize( &c, 1 ); return stream; }
//inline lStream& operator<< ( lStream& stream, int8 & c )           { stream.Serialize( &c, 1 ); return stream; }
inline lStream& operator<< ( lStream& stream, char & c )           { stream.Serialize( &c, 1 ); return stream; }
inline lStream& operator<< ( lStream& stream, uint16 &c )          { stream.Serialize( &c, 2 ); c = lEndian::Little16( c ); return stream; }
inline lStream& operator<< ( lStream& stream, int16 & c )          { stream.Serialize( &c, 2 ); c = lEndian::Little16( c ); return stream; }
inline lStream& operator<< ( lStream& stream, uint32 &c )          { stream.Serialize( &c, sizeof(uint32) ); c = lEndian::Little32( c ); return stream; }
inline lStream& operator<< ( lStream& stream, int32 & c )          { stream.Serialize( &c, sizeof(int32) ); c = lEndian::Little32( c ); return stream; }
inline lStream& operator<< ( lStream& stream, uint64 & c )         { stream.Serialize( &c, sizeof(uint64) ); c = lEndian::Little64( c ); return stream; }
inline lStream& operator<< ( lStream& stream, int64 & c )          { stream.Serialize( &c, sizeof(int64) );  c = lEndian::Little64( c ); return stream; }
inline lStream& operator<< ( lStream& stream, float & c )          { stream.Serialize( &c, sizeof(float) );  uint32 ct = lEndian::Little32( *(luint32_t *)&c ); c = *(float *)&ct;  return stream; }
inline lStream& operator<< ( lStream& stream, double & c )         { stream.Serialize( &c, sizeof(double) ); uint64 ct = lEndian::Little64( *(luint64_t *)&c ); c = *(double *)&ct; return stream; }

// Macro to declare a stream operator for an enumeration type.
//
#define L_STREAM_ENUM( e ) \
  static inline L_NAMESPACE_NAME::lStream& operator << ( L_NAMESPACE_NAME::lStream &ar, enum e &c ) { int t=(int)c; ar<<t; t = lEndian::Little32( t ); c=(e)t; return ar;}

// For use inside classes
//
#define L_STREAM_ENUM_FRIEND( e ) \
  friend inline L_NAMESPACE_NAME::lStream& operator << ( L_NAMESPACE_NAME::lStream &ar, enum e &c ) { int t=(int)c; ar<<t; t = lEndian::Little32( t );c=(e)t; return ar;}



END_L_NAMESPACE
#endif
