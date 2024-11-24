//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmemoryiostream.h
// Description : Static Memory IO Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMEMORYIOSTREAM_H_INCLUDED
#define LMEMORYIOSTREAM_H_INCLUDED

#include <string.h>                     // for memcpy
#include "./lioapi.h"
#include "./liostream.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemoryIOStream - Static memory io stream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lMemoryIOStream : public lIOStreamSeekable
{
public:

  lMemoryIOStream()
    : m_data( NULL ),
      m_dataSize( 0 ),
      m_access( L_IO_ACCESS_READWRITE ),
      m_index(0)
  {
  }

  lMemoryIOStream( void * data, unsigned int size, L_IO_ACCESS access )
  {
    Init( data, size, access );
  }

  void Init( void * data, unsigned int size, L_IO_ACCESS access )
  {
    m_data     = (char *)data;
    m_dataSize = size;
    m_access   = access;
    m_index    = 0;
  }

  unsigned int DataSize()     { return m_dataSize; }
  void       * Data()         { return m_data; }

  // lIOStreamSeekable interface

  virtual unsigned int Read(void *data,unsigned int size);
  virtual unsigned int Write(const void *data,unsigned int size);
  virtual unsigned int Seek(int offset,L_IO_SEEK whence);
  virtual unsigned int Tell(void);
  virtual unsigned int Length(void);
  virtual bool         Reading(void);
  virtual bool         Writing(void);
  virtual void         Flush(void);
  virtual void         Reset(void);
  virtual unsigned int AvailableRead(void);

private:

  char        * m_data;
  unsigned int  m_dataSize;

  L_IO_ACCESS   m_access;
  int           m_index;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lMemoryIOStream - Static memory io stream implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline unsigned int lMemoryIOStream::Read(void *data,unsigned int size)
{
  lAssertAlways( Reading() );

  if( size == 0 ) return 0;
  unsigned int remain = m_dataSize - m_index;
  if( remain < size ) size = remain;

  memcpy( data, m_data+m_index, size );

  m_index += size;
  return size;
}

inline unsigned int lMemoryIOStream::Write(const void *data,unsigned int size)
{
  lAssertAlways( Writing() );
  if( size == 0 ) return 0;
  unsigned int remain = m_dataSize - m_index;

  if( remain < size )
  {
    // write as much as we can.
    size = remain;
  }

  memcpy( m_data+m_index, data, size );
  m_index += size;
  return size;
}

inline unsigned int lMemoryIOStream::Seek(int offset,L_IO_SEEK whence)
{
  if( whence == L_IO_SEEK_SET )
  {
    m_index = offset;
  }
  else if( whence == L_IO_SEEK_CUR )
  {
    m_index += offset;
  }
  else if( whence == L_IO_SEEK_END )
  {
    m_index = m_dataSize - offset;
  }

  if( m_index < 0 ) m_index = 0;
  if( m_index > (int)m_dataSize ) m_index = (int)m_dataSize;
  return m_index;
}

inline unsigned int lMemoryIOStream::Tell(void)
{
  return m_index;
}

inline unsigned int lMemoryIOStream::Length(void)
{
  return m_dataSize;
}

inline bool lMemoryIOStream::Reading(void)
{
  return (m_access & L_IO_ACCESS_READ) ? true : false;
}

inline bool lMemoryIOStream::Writing(void)
{
  return (m_access & L_IO_ACCESS_WRITE) ? true : false;
}

inline void lMemoryIOStream::Flush(void)
{
}

inline void lMemoryIOStream::Reset(void)
{
  m_index = 0;
}

inline unsigned int lMemoryIOStream::AvailableRead(void)
{
  return m_dataSize - m_index;
}



END_L_NAMESPACE

#endif
