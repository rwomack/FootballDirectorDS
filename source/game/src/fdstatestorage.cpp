//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstatestorage.cpp
// Description : FDStateStorage implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <lemon/platform/lcore.h>
#include <fdmemory.h>
#include <fdstatestorage.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDStateStorage
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDStateStorage::FDStateStorage()
{
  m_dataPos = 0;
  m_dataSize = 0;
  m_bLoading = false;
}

FDStateStorage::~FDStateStorage()
{
}

void      FDStateStorage::Serialize( void *data, unsigned int size )
{
  if( IsLoading() ) Read( data, size );
  else              Write( data, size );
}

void      FDStateStorage::Reset()
{
  m_dataPos = 0;
  m_dataSize = 0;
}

int       FDStateStorage::Pos()
{
  return m_dataPos;
}

int       FDStateStorage::Length()
{
  return m_dataSize;
}

bool      FDStateStorage::IsLoading(void)
{
  return m_bLoading;
}

bool      FDStateStorage::IsSaving(void)
{
  return !m_bLoading;
}


// initialise a load of this state data
void      FDStateStorage::InitLoad()
{
  m_dataPos = 0;
  m_bLoading = true;
}

// initialise a save over this state data
void      FDStateStorage::InitSave()
{
  m_dataPos = 0;
  m_dataSize = 0;
  m_bLoading = false;
}

unsigned int FDStateStorage::Read(void *data,unsigned int size)
{
  lAssertAlways( IsLoading() );

  lLogFmt("Read %x, %d\n", data, size);

  if( size == 0 ) return 0;
  unsigned int remain = m_dataSize - m_dataPos;
  if( remain < size ) size = remain;

  memcpy( data, &m_data[m_dataPos], size );

  m_dataPos += size;
  return size;
}


unsigned int FDStateStorage::Write(const void *data,unsigned int size)
{
  lAssertAlways( IsSaving() );

  lLogFmt("Write %x, %d\n", data, size);
  if( size == 0 ) return 0;
  unsigned int remain = MAX_STATE_DATASIZE - m_dataPos;

  if( remain < size )
  {
    // write as much as we can.
    size = remain;
  }

  memcpy( &m_data[m_dataPos], data, size );
  m_dataPos += size;

  if( m_dataPos > m_dataSize )
  {
    m_dataSize = m_dataPos;
  }

  return size;
}


