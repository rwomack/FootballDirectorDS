//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : liffarchive.h
// Description : IFF Archive
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIFFARCHIVE_H_INCLUDED
#define LIFFARCHIVE_H_INCLUDED

#include "lobjectapi.h"
#include "../io/liff.h"

BEGIN_L_NAMESPACE

class LOBJECT_API lIFFNodeImpl;
class LOBJECT_API lIFFDataImpl;
class LOBJECT_API lIFFArchiveImpl;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFData
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Data value class, wraps lIFFDataImpl pointer with reference counting.
// Exposes data methods, no virtual destructor.

class LOBJECT_API lIFFData
{
  lIFFDataImpl * pointee;
  void init();

public:
  lIFFData( lIFFDataImpl* realPtr = 0 );
  lIFFData( const lIFFData& rhs );
  ~lIFFData();
  lIFFData& operator=(const lIFFData& rhs);

  void           Serialize( void * data, size_t size );
  lIFFNodeImpl * Parent();
  void           Close();

  bool           IsLoading();
  bool           IsSaving();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFNode
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Node value class, wraps lIFFNodeImpl pointer with reference counting.
// Exposes node methods, no virtual destructor.

class LOBJECT_API lIFFNode
{
  lIFFNodeImpl * pointee;
  void init();

public:
  lIFFNode( lIFFNodeImpl* realPtr = 0 );
  lIFFNode( const lIFFNode& rhs );
  ~lIFFNode();
  lIFFNode& operator=(const lIFFNode& rhs);

  lIFFData Data(lIFFId);
  lIFFNode Node(lIFFId);

  lIFFData Data(const char *);
  lIFFNode Node(const char *);

  lIFFNode Parent();
  lIFFId   Id();
  void     Close();

  bool     IsLoading();
  bool     IsSaving();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFArchive
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lIFFArchive
{
  lIFFArchiveImpl * m_ptr;

public:

  enum lIFFArchiveMode { save, load };

  lIFFArchive( lIOStreamSeekable * iostream, lIFFArchiveMode mode );
  ~lIFFArchive();

  lIFFNode Node(lIFFId);
  lIFFNode Node(const char *);

  bool     IsLoading();
  bool     IsSaving();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Basic archive operators
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Note: bool is converted to a signed char as it may
//       vary in size on different platforms.
inline lIFFData& operator<< ( lIFFData& ar, bool & c )      { uint8 x = c ? 1 : 0; ar.Serialize( &x, 1 ); c = (x == 0) ? false : true; return ar; }
inline lIFFData& operator<< ( lIFFData& ar, uint8 & c )     { ar.Serialize( &c, 1 ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, int8 & c )      { ar.Serialize( &c, 1 ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, uint16 & c )    { ar.Serialize( &c, sizeof(uint16) ); c = lEndian::Little16( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, int16 & c )     { ar.Serialize( &c, sizeof(int16) );  c = lEndian::Little16( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, uint32 & c )    { ar.Serialize( &c, sizeof(uint32) ); c = lEndian::Little32( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, int32 & c )     { ar.Serialize( &c, sizeof(int32) );  c = lEndian::Little32( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, int64 & c )     { ar.Serialize( &c, sizeof(int64) );  c = lEndian::Little64( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, uint64 & c )    { ar.Serialize( &c, sizeof(uint64) ); c = lEndian::Little64( c ); return ar; }
inline lIFFData& operator<< ( lIFFData& ar, float & c )     { ar.Serialize( &c, sizeof(float) );  uint32 ct = lEndian::Little32( *(luint32_t *)&c ); c = *(float *)&ct;  return ar; }
inline lIFFData& operator<< ( lIFFData& ar, double & c )    { ar.Serialize( &c, sizeof(double) ); uint64 ct = lEndian::Little64( *(luint64_t *)&c ); c = *(double *)&ct; return ar; }

END_L_NAMESPACE
#endif
