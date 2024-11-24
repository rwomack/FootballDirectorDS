//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lzipiostream.h
// Description : Zip IO Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LZIPIOSTREAM_H_INCLUDED
#define LZIPIOSTREAM_H_INCLUDED

#include "./lcodec.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lZipIOStream - Zip IO stream interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lZipIOStream : public lCodec
{
  void               * m_pStream;      // zLib stream
  unsigned char      * m_buffer;       // io buffer
  unsigned int         m_bufferSize;   // io buffer size
  unsigned int         m_bufferPos;    // position in io buffer
  lIOStreamSeekable  * m_ioStream;     // io stream
  bool                 m_bRunning;     // is the zLib stream running?
  int                  m_bError;       // has an error occured?
  bool                 m_bCompressing; // are we compressing?

  bool Init( lIOStreamSeekable * stream, bool bCompress );

public:
  lZipIOStream( lIOStreamSeekable * stream, L_IO_ACCESS access, unsigned int bufferSize = 1024*1024*2 /*2mb*/ );
  ~lZipIOStream();

  // lIOStream interface

  unsigned int Write( const void * data, unsigned int size );
  unsigned int Read( void * data, unsigned int size );

  bool         Reading(void);
  bool         Writing(void);

  // lCodec interface

  lIOStreamSeekable  * GetStream();
  int                  Reset();
  int                  FinishStream();
  int                  FlushStream();
  int                  GetErrorCode();
  const char *         GetErrorString();
};

END_L_NAMESPACE
#endif
