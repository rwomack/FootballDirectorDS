//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lzipfilewriter.h
// Description : Zip file writer class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LZIPFILEWRITER_H_INCLUDED
#define LZIPFILEWRITER_H_INCLUDED

#include "./liostream.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lZipFileWriter error codes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  lZIPWRITE_ERR_OK                 = 0,  // no error
  lZIPWRITE_ERR_CODEC_ERROR        = 1,  // error in the codec
  lZIPWRITE_ERR_WRITE_ERROR        = 2,  // error writing to output stream, or stream is not writable
  lZIPWRITE_ERR_READ_ERROR         = 3,  // error reading from input stream, or stream is not readable
  lZIPWRITE_ERR_OUT_OF_MEMORY      = 4,  // out of memory
  lZIPWRITE_ERR_INVALID_PARAMETER  = 5,  // invalid parameter
  lZIPWRITE_ERR_ARCHIVE_OPEN       = 6,  // archive open, must be closed before opening another
  lZIPWRITE_ERR_ARCHIVE_NOT_OPEN   = 7,  // operation can only be performed when an archive has been opened
  lZIPWRITE_ERR_FILE_BEGUN         = 8,  // file begun, must be ended before beginning another
  lZIPWRITE_ERR_FILE_NOT_BEGUN     = 10, // operation can only be performed when writing a file
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lZipFileWriter
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lZipFileWriter : public lIOStream
{
  void              * m_pState;       // zLib stream and state
  int                 m_errorCode;    // error state
  lIOStreamSeekable * m_ioStream;     // io stream
  bool                m_bWritingFile; // true if we're writing a file
  uint32              m_numEntries;   // number of file entries

public:

  enum Method
  {
    METHOD_STORE   = 0,
    METHOD_DEFLATE = 1
  };

  enum
  {
    COMPRESSION_NONE    = 0,
    COMPRESSION_MAX     = 9,
    COMPRESSION_DEFAULT = -1
  };

  lZipFileWriter();
  virtual ~lZipFileWriter();

  bool          Open( lIOStreamSeekable * pSeekableStream );
  bool          Close( const char * comment = NULL );
  bool          IsOpen();
  bool          IsWritingFile();

  int           GetErrorCode();
  const char *  GetErrorString();

  bool BeginFile( const char * fileName,
                  const time_t fileTime,
                  Method       method               = lZipFileWriter::METHOD_DEFLATE,
                  int          compressionLevel     = lZipFileWriter::COMPRESSION_DEFAULT,
                  const char * comment              = NULL,
                  const void * extraFieldLocal      = NULL,
                  uint16       extraFieldLocalSize  = 0,
                  const void * extraFieldGlobal     = NULL,
                  uint16       extraFieldGlobalSize = 0 );

  bool EndFile();

  bool WriteFileFromStream( lIOStreamSeekable * inputStream,
                            const char * fileName,
                            const time_t fileTime,
                            Method       method               = lZipFileWriter::METHOD_DEFLATE,
                            int          compressionLevel     = lZipFileWriter::COMPRESSION_DEFAULT,
                            const char * comment              = NULL,
                            const void * extraFieldLocal      = NULL,
                            uint16       extraFieldLocalSize  = 0,
                            const void * extraFieldGlobal     = NULL,
                            uint16       extraFieldGlobalSize = 0 );

  // lIOStream interface
  // - Only valid between BeginFile and EndFile, when IsWritingFile returns true.

  // lIOStream interface

  unsigned int Write( const void * data, unsigned int size );
  unsigned int Read( void * data, unsigned int size );

  bool         Reading(void);
  bool         Writing(void);
};


END_L_NAMESPACE
#endif
