//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : larrayiostream.h
// Description : Array IO Stream class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LARRAYIOSTREAM_H_INCLUDED
#define LARRAYIOSTREAM_H_INCLUDED

#include <string.h>                     // for memcpy
#include "./lioapi.h"
#include "./liostream.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArrayIOStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lArrayIOStream : public lIOStreamSeekable
{
public:
  lArrayIOStream(L_IO_ACCESS access) : m_access(access), m_index(0) { }
  ~lArrayIOStream() {}

  virtual unsigned int Read(void *data,unsigned int size)
  {
    lAssertAlways( Reading() );
    if( !Reading() ) return 0;
    if( size == 0 ) return 0;
    unsigned int remain = m_data.Size() - m_index;
    if( remain < size ) size = remain;
    if( size == 0 ) return 0;
    memcpy( data, &m_data(m_index), size );
    m_index += size;
    return size;
  }

  virtual unsigned int Write(const void *data,unsigned int size)
  {
    lAssertAlways( Writing() );
    if( !Writing() ) return 0;
    if( size == 0 ) return 0;
    unsigned int remain = m_data.Size() - m_index;
    if( remain < size ) m_data.Add( size-remain );
    memcpy( &m_data(m_index), data, size );
    m_index += size;
    return size;
  }

  virtual unsigned int Seek(int offset,L_IO_SEEK whence)
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
      m_index = m_data.Size() + offset;
    }
    if( m_index < 0 ) m_index = 0;
    if( m_index > m_data.Size() ) m_index = m_data.Size();
    return m_index;
  }

  virtual unsigned int Tell(void)
  {
    return m_index;
  }

  virtual unsigned int Length(void)
  {
    return m_data.Size();
  }

  virtual bool         Reading(void)
  {
    return ((m_access & L_IO_ACCESS_READ) == L_IO_ACCESS_READ);
  }

  virtual bool         Writing(void)
  {
    return ((m_access & L_IO_ACCESS_WRITE) == L_IO_ACCESS_WRITE);
  }

  lArray<char> & GetDataArray() { return m_data; }

private:
  L_IO_ACCESS   m_access;
  int           m_index;
  lArray<char>  m_data;
};

END_L_NAMESPACE

#endif
