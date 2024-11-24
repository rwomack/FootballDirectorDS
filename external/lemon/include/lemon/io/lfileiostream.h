//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lfileiostream.h
// Description : File IO Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LFILEIOSTREAM_H_INCLUDED
#define LFILEIOSTREAM_H_INCLUDED

#include "./liostream.h"

BEGIN_L_NAMESPACE

//##
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File IO stream derived from lIOStreamSeekable
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIO_API lFileIOStream : public lIOStreamSeekable
{
  lFILE_FIND_HANDLE m_hnd;

public:

  //##
  // Constructs an invalid lFileIOStream ready for opening a file (with Open) or attaching a handle with Attach
  lFileIOStream();

  //##
  // Opens 'fileName' with the specified modes
  // In:
  //   fileName - the filename to open
  //   accessMode - Access mode to use when opening (L_IO_ACCESS)
  //   openMode   - Optional open mode to use when opening (L_IO_OPEN)
  // Note:
  //   If the file open fails the handle inside the lFileIOStream class will be invalid this can be queried with IsValid()
  lFileIOStream( const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode = L_IO_OPEN_DEFAULT );

  //##
  // Constructs an lFileIOStream from the file handle passed in
  lFileIOStream( lFILE_FIND_HANDLE hnd );

  virtual ~lFileIOStream();

  lFileIOStream& operator=(lFILE_FIND_HANDLE hnd);
  operator lFILE_FIND_HANDLE();

  void                 Attach( lFILE_FIND_HANDLE hnd );
  lFILE_FIND_HANDLE    Detach();

  //##
  // Opens 'fileName' with the specified modes
  // In:
  //   fileName - the filename to open
  //   accessMode - Access mode to use when opening (L_IO_ACCESS)
  //   openMode   - Optional open mode to use when opening (L_IO_OPEN)
  // Out:
  //   Returns true if successful, false if not
  // Note:
  //   If the file open fails the handle inside the lFileIOStream class will be invalid this can be queried with IsValid()
  //   If an existing file was opened by this instance it must first be closed ( see Close() )
  bool                 Open( const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode = L_IO_OPEN_DEFAULT );

  //##
  // Close the open file handle inside this class
  void                 Close();

  //##
  // Return the access mode (L_IO_ACCESS) which is being used by the file opened by this class, the class must be valid ( see IsValid() )
  L_IO_ACCESS          GetAccessMode();

  //##
  // Return the open mode (L_IO_OPEN) which is being used by the file opened by this class, the class must be valid ( see IsValid() )
  L_IO_OPEN            GetOpenMode();

  //##
  // Returns the file handle for the opened file (lFILE_FIND_HANDLE)
  lFILE_FIND_HANDLE    GetFileHandle();

  //##
  // Returns true if the file handle in this class is valid, false if not
  bool                 IsValid();

  // lIOStream

  virtual bool         Reading(void);
  virtual bool         Writing(void);
  virtual unsigned int Read(void *,unsigned int);
  virtual unsigned int Write(const void *,unsigned int);

  // lIOStreamSeekable

  virtual unsigned int Seek(int,L_IO_SEEK);
  virtual unsigned int Tell(void);
  virtual unsigned int Length(void);
};

// lFileIOStream implementation

inline lFileIOStream::lFileIOStream()
{
  m_hnd = lINVALID_FILE_HANDLE;
}

inline lFileIOStream::lFileIOStream( const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode )
{
  m_hnd = lPlatform_FileOpen( fileName, accessMode, openMode );
}

inline lFileIOStream::lFileIOStream( lFILE_FIND_HANDLE hnd )
{
  m_hnd = hnd;
}

inline lFileIOStream::~lFileIOStream()
{
  if( m_hnd != lINVALID_FILE_HANDLE )
  {
    lLogFmt("lFileIOStream::~lFileIOStream(): File handle still active, use Detach() or Close()\n");
  }
}

inline lFileIOStream& lFileIOStream::operator=(lFILE_FIND_HANDLE hnd)
{
  lAssert( m_hnd == lINVALID_FILE_HANDLE );
  m_hnd = hnd;
  return *this;
}

inline lFileIOStream::operator lFILE_FIND_HANDLE()
{
  return m_hnd;
}

inline void lFileIOStream::Attach( lFILE_FIND_HANDLE hnd )
{
  lAssert( m_hnd == lINVALID_FILE_HANDLE );
  m_hnd = hnd;
}

inline lFILE_FIND_HANDLE lFileIOStream::Detach()
{
  lFILE_FIND_HANDLE hnd = m_hnd;
  m_hnd = lINVALID_FILE_HANDLE;

  return hnd;
}

inline lFILE_FIND_HANDLE lFileIOStream::GetFileHandle()
{
  return m_hnd;
}

inline bool lFileIOStream::IsValid()
{
  if( m_hnd == lINVALID_FILE_HANDLE ) return false;
  return true;
}

inline bool lFileIOStream::Open( const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode )
{
  lAssert( m_hnd == lINVALID_FILE_HANDLE );
  m_hnd = lPlatform_FileOpen( fileName, accessMode, openMode );
  return IsValid();
}

inline void lFileIOStream::Close()
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  lPlatform_FileClose(m_hnd);
  m_hnd = lINVALID_FILE_HANDLE;
}

inline L_IO_ACCESS lFileIOStream::GetAccessMode()
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileGetAccessMode(m_hnd);
}

inline L_IO_OPEN lFileIOStream::GetOpenMode()
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileGetOpenMode(m_hnd);
}

inline bool lFileIOStream::Reading(void)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  if( GetAccessMode() & L_IO_ACCESS_READ ) return true;
  return false;
}

inline bool lFileIOStream::Writing(void)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  if( GetAccessMode() & L_IO_ACCESS_WRITE ) return true;
  return false;
}

inline unsigned int lFileIOStream::Read(void *data,unsigned int size)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileRead(m_hnd,data,size);
}

inline unsigned int lFileIOStream::Write(const void *data,unsigned int size)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileWrite(m_hnd,data,size);
}

inline unsigned int lFileIOStream::Seek(int offset,L_IO_SEEK whence)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileSeek(m_hnd,offset,whence);
}

inline unsigned int lFileIOStream::Tell(void)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileTell(m_hnd);
}

inline unsigned int lFileIOStream::Length(void)
{
  lAssert( m_hnd != lINVALID_FILE_HANDLE );
  return lPlatform_FileLength(m_hnd);
}


END_L_NAMESPACE
#endif
