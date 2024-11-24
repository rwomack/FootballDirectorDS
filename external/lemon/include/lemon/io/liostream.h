//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : liostream.h
// Description : IO Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIOSTREAM_H_INCLUDED
#define LIOSTREAM_H_INCLUDED

#include "./lioapi.h"
#include "../platform/lcorefile.h" // for L_IO_SEEK

BEGIN_L_NAMESPACE

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// IO stream interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lIOStream
{
public:
  virtual             ~lIOStream() {}

  virtual bool         Reading(void) = 0;                     //## Returns true if this stream is currently read enabled
  virtual bool         Writing(void) = 0;                     //## Returns true if this stream is currently write enabled

  //##
  // Read 'size' bytes from the stream to the memory location specified by 'ptr'
  // In:
  //   ptr  - memory location to read to
  //   size - number of bytes to read
  // Out:
  //   Returns the number of bytes read
  virtual unsigned int Read(void * ptr,unsigned int size)   = 0;

  //##
  // Writes 'size' bytes from the memory location specified by 'ptr' to the stream
  // In:
  //   ptr  - memory location to write from
  //   size - number of bytes to write
  // Out:
  //   Returns the number of bytes write
  virtual unsigned int Write(const void *ptr,unsigned int size)  = 0;
};

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// A Seekable IO stream interface derived from lIOStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lIOStreamSeekable : public lIOStream
{
public:
  //##
  //  Seek to the location specified within the stream
  // In:
  //   offset - offset
  //   whence - where to seek from (L_IO_SEEK_xxx)
  // Out:
  //   Returns the resulting file position from the start of the file
  virtual unsigned int Seek(int offset,L_IO_SEEK whence) = 0;

  //##
  //  Return the file position from the start of the file
  virtual unsigned int Tell(void) = 0;

  //##
  //  Return the file length
  virtual unsigned int Length(void) = 0;
};

END_L_NAMESPACE
#endif
