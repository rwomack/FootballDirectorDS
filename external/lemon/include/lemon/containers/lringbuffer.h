//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lringbuffer.h
// Description : RingBuffer class
// Notes       : TODO: IN PROGRESS
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRINGBUFFER_H_INCLUDED
#define LRINGBUFFER_H_INCLUDED

#include "../platform/lcore.h"

BEGIN_L_NAMESPACE

//class lArchive;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRingBuffer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lRingBuffer
{
public:
  lRingBuffer() : m_bufferSize(0), m_writeIndex(0), m_readIndex(0), m_bigMask(0), m_smallMask(0), m_buffer(NULL)  {}
  ~lRingBuffer() {}

  // Initalise FIFO
  // numBytes must be power of 2, returns false if not
  bool Init( unsigned int numBytes, void *dataPtr )
	{
    if( ((numBytes-1) & numBytes) != 0)
			return false; //Not Power of two.

    m_bufferSize = numBytes;
    m_buffer = (char *)dataPtr;

    Flush();

    m_bigMask = (numBytes*2)-1;
    m_smallMask = (numBytes)-1;

		return true;
	}


  // Return number of bytes available for reading.
  unsigned int GetReadAvailable()
	{
    return ( (m_writeIndex - m_readIndex) & m_bigMask );
	}

  // Return number of bytes available for writing.
  unsigned int GetWriteAvailable()
	{
    return ( m_bufferSize - GetReadAvailable() );
	}

  // Returns bytes written.
  unsigned int Write( void *data, unsigned int numBytes )
	{
    unsigned int size1, size2, numWritten;
		void *data1, *data2;

    numWritten = GetWriteRegions( numBytes, &data1, &size1, &data2, &size2 );

    if( size2 > 0 )
		{
			memcpy( data1, data, size1 );
			data = ((char *)data) + size1;
			memcpy( data2, data, size2 );
		}
		else
		{
			memcpy( data1, data, size1 );
		}

    AdvanceWriteIndex( numWritten );
		return numWritten;
	}

  // Returns bytes read
  unsigned int Read( void *data, unsigned int numBytes )
	{
    unsigned int size1, size2, numRead;
		void *data1, *data2;

    numRead = GetReadRegions( numBytes, &data1, &size1, &data2, &size2 );
		if( size2 > 0 )
		{
			memcpy( data, data1, size1 );
			data = ((char *)data) + size1;
			memcpy( data, data2, size2 );
		}
		else
		{
			memcpy( data, data1, size1 );
		}

    AdvanceReadIndex( numRead );
		return numRead;
	}

  // Clear buffer. Should only be called when buffer is NOT being read.
  void Flush( )
	{
    m_writeIndex = m_readIndex = 0;
	}

 protected:

  // Get address of region(s) to which we can write data.
  //  If the region is contiguous, size2 will be zero.
  //  If non-contiguous, size2 will be the size of second region.
  //  Returns room available to be written or numBytes, whichever is smaller.
  unsigned int GetWriteRegions( unsigned int numBytes, void **dataPtr1, unsigned int *sizePtr1, void **dataPtr2, unsigned int *sizePtr2 )
	{
    unsigned int   index;
    unsigned int   available = GetWriteAvailable( );

		if( numBytes > available )
			numBytes = available;

		//Check to see if write is not contiguous.
    index = m_writeIndex & m_smallMask;
    if( (index + numBytes) > m_bufferSize )
		{
			//Write data in two blocks that wrap the buffer.
      unsigned int   firstHalf = m_bufferSize - index;
      *dataPtr1 = &m_buffer[index];
			*sizePtr1 = firstHalf;
      *dataPtr2 = &m_buffer[0];
			*sizePtr2 = numBytes - firstHalf;
		}
		else
		{
      *dataPtr1 = &m_buffer[index];
			*sizePtr1 = numBytes;
			*dataPtr2 = 0;
			*sizePtr2 = 0;
		}

		return numBytes;
	}

  // Get address of region(s) from which we can read data.
  //  If the region is contiguous, size2 will be zero.
  //  If non-contiguous, size2 will be the size of second region.
  //  Returns room available to be written or numBytes, whichever is smaller.
  unsigned int GetReadRegions( unsigned int numBytes, void **dataPtr1, unsigned int *sizePtr1, void **dataPtr2, unsigned int *sizePtr2 )
	{
    unsigned int   index;
    unsigned int   available = GetReadAvailable( );
		if( numBytes > available ) numBytes = available;

    // Check to see if read is not contiguous.
    index = m_readIndex & m_smallMask;
    if( (index + numBytes) > m_bufferSize )
		{
      // Write data in two blocks that wrap the buffer.
      unsigned int firstHalf = m_bufferSize - index;
      *dataPtr1 = &m_buffer[index];
			*sizePtr1 = firstHalf;
      *dataPtr2 = &m_buffer[0];
			*sizePtr2 = numBytes - firstHalf;
		}
		else
		{
      *dataPtr1 = &m_buffer[index];
			*sizePtr1 = numBytes;
			*dataPtr2 = 0;
			*sizePtr2 = 0;
		}

		return numBytes;
	}


	//---------------------------------------------------------------//
  unsigned int AdvanceWriteIndex( unsigned int numBytes )
	{
    return m_writeIndex = (m_writeIndex + numBytes) & m_bigMask;
	}

	//---------------------------------------------------------------//
  unsigned int AdvanceReadIndex( unsigned int numBytes )
	{
    return m_readIndex = (m_readIndex + numBytes) & m_bigMask;
	}

  // These are declared volatile because they are written by a
  // different thread than the reader.

  //! Index of next writable byte. Set by RingBuffer_AdvanceWriteIndex.
  volatile unsigned int m_writeIndex;

  //! Index of next readable byte. Set by RingBuffer_AdvanceReadIndex.
  volatile unsigned int m_readIndex;

public:
  //Number of bytes in FIFO. Power of 2. Set by RingBuffer_Init
  unsigned int    m_bufferSize;

  //! Used for wrapping indices with extra bit to distinguish full/empty.
  unsigned int    m_bigMask;
  // Used for fitting indices to buffer.
  unsigned int    m_smallMask;

  char           *m_buffer;
};


END_L_NAMESPACE
#endif
