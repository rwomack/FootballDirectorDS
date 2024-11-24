//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : liff.h
// Description : IFF routines
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIFF_H_INCLUDED
#define LIFF_H_INCLUDED

#include "../platform/lcoreexception.h"
#include "./lioapi.h"

BEGIN_L_NAMESPACE

class LIO_API lIOStreamSeekable;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Macros
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_IFFID(_a,_b,_c,_d)  ((lIFFId)( (lIFFId)(_a)<<24L | (lIFFId)(_b)<<16L | (lIFFId)(_c)<<8 | (lIFFId)(_d) ))

// Standard IFF89a IDs. Chunks with these IDs contain a
// Sub-ID followed by zero or more chunks.
// The IDs FOR1..FOR9, LIS1..LIS9, & CAT1..CAT9 are reserved for future standardization.
//
// Note: L_IFF_FILLER is used to signify that there are no particular contents for
//       for LISTs and CATs.

#define L_IFF_FORM   L_IFFID('F','O','R','M')
#define L_IFF_PROP   L_IFFID('P','R','O','P')
#define L_IFF_LIST   L_IFFID('L','I','S','T')
#define L_IFF_CAT    L_IFFID('C','A','T',' ')
#define L_IFF_FILLER L_IFFID(' ',' ',' ',' ')

// IFF Id type

typedef uint32 lIFFId;

// IFF chunk header

struct lIFFChunk
{
  uint32 chunkId;
  uint32 size;
};

// IFF group context

struct lIFFContext
{
  lIOStreamSeekable * stream;       // stream reading/writing to
  lIFFContext       * parent;       // parent group
  lIFFId              groupSubId;   // this groups sub id
  unsigned int        position;     // position of group in the stream
  unsigned int        bound;        // position bound for chunk in the stream
  lIFFChunk           chunkHdr;     // current chunk we're reading/writing
  unsigned int        bytesSoFar;   // bytes read/written for current chunk
  lIFFContext       * childGroup;   // child group being read/written
};

struct lIFF_POS
{
  lIFFContext  * ctx;
  lIFFChunk      chunkHdr;
  unsigned int   position;
  unsigned int   bytesSoFar;
};

// TODO: log extended information in functions
//       and provide a lookup for error code strings.
//       Try and group error codes together.

#define lIFFERR_OK                        0  // no error
#define lIFFERR_INVALID_CONTEXT           1  // context passed to function is invalid (NULL, corrupt, or incorrect context)
#define lIFFERR_INVALID_CHUNKID           2  // chunk id is invalid (chunk ID's cannot be zero and must be ASCII)
#define lIFFERR_IN_CHUNK                  3  // already inside a chunk (cannot enter another one) / chunk still open in the group
#define lIFFERR_NOT_IN_CHUNK              4  // not inside a chunk (cannot write or close it)
#define lIFFERR_INVALID_TOPLEVEL_CHUNK    5  // invalid top level chunk, must be FORM, LIST or CAT
#define lIFFERR_INVALID_TOPLEVEL_COUNT    6  // can only have a single top level chunk
#define lIFFERR_BOUND_OVERFLOW            7  // operation will overflow the group size
#define lIFFERR_CHUNK_OVERFLOW            8  // operation will overflow the chunk size
#define lIFFERR_WRITE_ERROR               9  // error writing data
#define lIFFERR_INVALID_SIZE              10 // invalid size (0xffffffff is used as a special tag)
#define lIFFERR_IN_GROUP                  11 // writing a group, cannot close the chunk or write to the context
#define lIFFERR_SIZE_MISMATCH             12 // size specified for chunk or group and the final size (on end/close) do not match
#define lIFFERR_CHUNKID_MISMATCH          13 // chunk id does not match the currently open id
#define lIFFERR_GROUP_MISMATCH            14 // the group that is being specified is not currently open in the parent
#define lIFFERR_BAD_FORMAT                15 // bad format IFF
#define lIFFERR_DONE                      255

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Basic Reader/Writer functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// L_IFF_AUTOSIZE can be passed as the size parameter to chunk and group
// methods to automatically fill the size in.

#define L_IFF_AUTOSIZE ((unsigned int)-1)

const char * lIFFGetErrorString( int iffErrorCode );

struct LIO_API lIFFWrite
{
  static int OpenStream( lIFFContext * ctx, lIOStreamSeekable * );
  static int BeginGroup( lIFFContext * ctx, lIFFContext * childCtx, lIFFId chunkId, unsigned int size, lIFFId subId );
  static int EndGroup( lIFFContext * ctx, lIFFContext * childCtx );
  static int OpenChunk( lIFFContext * ctx, lIFFId chunkId, unsigned int size );
  static int WriteChunkBytes( lIFFContext * ctx, const void * data, unsigned int size );
  static int CloseChunk( lIFFContext * ctx, lIFFId chunkId );
  static int WriteWholeChunk( lIFFContext * ctx, lIFFId chunkId, const void * data, unsigned int size );
};

struct LIO_API lIFFRead
{
  static int          OpenStream( lIFFContext * ctx, lIOStreamSeekable * );
  static int          NextChunk( lIFFContext * ctx );
  static int          OpenGroup( lIFFContext * ctx, lIFFContext * childCtx );
  static int          CloseGroup( lIFFContext * ctx, lIFFContext * childCtx );
  static int          ReadChunkBytes( lIFFContext * ctx, void * data, unsigned int size );

  static lIFF_POS     GroupTell( lIFFContext * ctx );
  static int          GroupSeek( lIFFContext * ctx, lIFF_POS );
  static int          GroupRewind( lIFFContext * ctx );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFException
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lIFFException : public lException
{
  int m_iffError;

public:
  lIFFException( int iffError );
  ~lIFFException();

  int          GetIFFError();
  const char * GetErrorString();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFBaseContext
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Base wrapper around an lIFFContext pointer

class LIO_API lIFFBaseContext
{
protected:
  lIFFContext * context;
public:

  lIOStreamSeekable * IOStream()            { return context->stream; }
  lIFFContext       * Parent()              { return context->parent; }
  lIFFId              GroupSubId()          { return context->groupSubId; }
  unsigned int        StreamPosition()      { return context->position; }
  unsigned int        StreamBound()         { return context->bound; }
  lIFFId              ChunkId()             { return context->chunkHdr.chunkId; }
  unsigned int        ChunkSize()           { return context->chunkHdr.size; }
  unsigned int        ChunkBytesSoFar()     { return context->bytesSoFar; }
  unsigned int        ChunkBytesRemaining() { return context->chunkHdr.size - context->bytesSoFar; }
  lIFFContext       * Child()               { return context->childGroup; }

  lIFFBaseContext( lIFFContext * ctx ) : context(ctx)
  {
  }

  operator lIFFContext * () { return context; }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFReadContext
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Read wrapper around an lIFFContext pointer

class LIO_API lIFFReadContext : public lIFFBaseContext
{
public:
  lIFFReadContext( lIFFContext * ctx ) : lIFFBaseContext(ctx)
  {
  }

  bool NextChunk()
  {
    int err = lIFFRead::NextChunk( context );
    if( err != lIFFERR_OK && err != lIFFERR_DONE )
#ifndef L_NO_EXCEPTIONS
      throw lIFFException(err);
#else
      return false;
#endif
    if( err == lIFFERR_DONE ) return false;
    return true;
  }

  void OpenGroup( lIFFContext * childCtx )
  {
    int err = lIFFRead::OpenGroup( context, childCtx );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void CloseGroup( lIFFContext * childCtx )
  {
    int err = lIFFRead::CloseGroup( context, childCtx );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void ReadChunkBytes( void * data, unsigned int size )
  {
    int err = lIFFRead::ReadChunkBytes( context, data, size );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  lIFF_POS GroupTell()
  {
    return lIFFRead::GroupTell(context);
  }

  void GroupSeek( lIFF_POS pos )
  {
    int err = lIFFRead::GroupSeek(context,pos);
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void GroupRewind()
  {
    int err = lIFFRead::GroupRewind(context);
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFWriteContext
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Write wrapper around an lIFFContext pointer

class LIO_API lIFFWriteContext : public lIFFBaseContext
{
public:
  lIFFWriteContext( lIFFContext * ctx ) : lIFFBaseContext(ctx)
  {
  }

  void BeginGroup( lIFFContext * childCtx, lIFFId chunkId, unsigned int size, lIFFId subId )
  {
    int err = lIFFWrite::BeginGroup( context, childCtx, chunkId, size, subId );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void EndGroup( lIFFContext * childCtx )
  {
    int err = lIFFWrite::EndGroup( context, childCtx );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void OpenChunk( lIFFId chunkId, unsigned int size )
  {
    int err = lIFFWrite::OpenChunk( context, chunkId, size );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void WriteChunkBytes( const void * data, unsigned int size )
  {
    int err = lIFFWrite::WriteChunkBytes( context, data, size );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void CloseChunk( lIFFId chunkId )
  {
    int err = lIFFWrite::CloseChunk( context, chunkId );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }

  void WriteWholeChunk( lIFFId chunkId, const void * data, unsigned int size )
  {
    int err = lIFFWrite::WriteWholeChunk( context, chunkId, data, size );
#ifndef L_NO_EXCEPTIONS
    if( err != lIFFERR_OK ) throw lIFFException(err);
#endif
  }
};


END_L_NAMESPACE
#endif
