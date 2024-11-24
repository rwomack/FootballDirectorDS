//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcodec.h
// Description : Codec class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCODEC_H_INCLUDED
#define LCODEC_H_INCLUDED

#include "./liostream.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCodec error codes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define lCODECERR_OK          0   //## no error
#define lCODECERR_END         1   //## codec stream ended
#define lCODECERR_CODEC_ERROR 2   //## error in the codec
#define lCODECERR_WRITE_ERROR 3   //## error writing to output stream
#define lCODECERR_READ_ERROR  4   //## error reading from input stream

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Generic Codec interface derived from the lIOStream interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lCodec : public lIOStream
{
public:

  // lCodec interface

  virtual lIOStreamSeekable  * GetStream() = 0;      //## Returns the attached i/o stream being read from or written to
  virtual int                  Reset() = 0;          //## Reset the codec
  virtual int                  FinishStream() = 0;   //## Finish the codec stream (no further read/write possible after this, until Reset() is called)
  virtual int                  FlushStream() = 0;    //## Flush pending data when writing
  virtual int                  GetErrorCode() = 0;   //## Returns the error state of the stream
  virtual const char *         GetErrorString() = 0; //## Returns error string, possibly containing extended error information.

  // Note: When a stream error occurs, the codec is closed and all functions will return the same error
  //       code and no reading or writing is possible until Reset() is called.
  //       Reading() and Writing() methods from lIOStream will both return false in this situation.
  // Note: When Reset() is called, the compression is reset, but the position in the attached i/o stream
  //       will not change. The user must call handle this manually.

  // lCodec implementation

  static const char *          GetErrorString(int);  //## Get predefined error string for an error code
};

END_L_NAMESPACE
#endif
