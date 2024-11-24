//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : logg.h
// Description : OGG lSoundStream
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./laudioapi.h"
#include "../platform/lcorefile.h"

#ifndef LOGG_H_INCLUDED
#define LOGG_H_INCLUDED

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lOGGStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define OGG_BUFFER_SIZE 4096

class lOGGStream : public lSoundStream
{
public:
  lOGGStream( const char *filename, bool b8bit = false );
  virtual ~lOGGStream();

  // read / rewind / eof
  virtual uint32 Read( void * ptr, uint32 size );
  virtual void   Rewind( bool bFreshPlay );
  virtual bool   IsEOF();

  // stream information
  virtual lSAMPLE_FORMAT Format() const;
  virtual unsigned int   Frequency() const;
  virtual unsigned int   GetFlags()  const;

  void SetLoop( bool loop ) { m_bLoop = loop; }
  bool GetLoop()            { return m_bLoop; }
  void SetLoopStartEnd( float start, float end, bool bSeekToLoopStart = false );

private:
  lFILE_HANDLE     m_file;
  void *           m_oggFile;
  uint8           *m_fileData;
  uint32           m_filePos;
  uint32           m_fileSize;
  int              current_section;
  uint8            m_buffer[ OGG_BUFFER_SIZE ];
  int              m_bufferPos;
  int              m_bufferSize;
  bool             m_bEOF;
  bool             m_bLoop;
  lSAMPLE_FORMAT   m_fmt;
  unsigned int     m_freq;
  int              m_oggwordsize;
  float            m_loopStart;
  float            m_loopEnd;

  // IO functions

  static size_t oggreadFunc( void *ptr, size_t size, size_t nmemb, void *datasource );
  static int oggseekFunc( void *datasource, int64 offset, int whence );
  static int oggcloseFunc( void *datasource );
  static long oggtellFunc( void *datasource );
};


END_L_NAMESPACE
#endif
