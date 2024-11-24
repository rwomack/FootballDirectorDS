//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lzipfilereader.h
// Description : Zip file reader class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 When using the GetFile() methods, we currently decompress the zipped data
 to a lBufferIOStream and return it.
 We should really do this on the fly using something like lZipIOStream.

 We should also factor out the time-data code and wild-card code,
 to reduce the number of includes and extra code we need here.

*/

#ifndef LZIPFILE_H_INCLUDED
#define LZIPFILE_H_INCLUDED

#include "./liostream.h"
#include "../vendor/zlib/zlib.h"
#include "../containers/larray.h"
#include "../platform/lcorefile.h"

#include <ctype.h>                         // for toupper()
#include <time.h>                          // for tzset and time_t
extern "C" {
extern int __cdecl _isindst(struct tm *);  // perhaps not a great idea to use this.
}

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

typedef unsigned int lZIPFILE_FIND_HANDLE;

struct lZIP_FILE_FIND_DATA
{
  time_t              creationTime;
  unsigned int        size;
  unsigned int        compressedSize;
  unsigned int        zipIndex;
  char                name[ L_FILE_MAX_PATH ];
  char                comment[64];
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lBufferIOStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lBufferIOStream : public lIOStreamSeekable
{
public:
  lBufferIOStream(L_IO_ACCESS access) : m_access(access), m_index(0) { }
  ~lBufferIOStream() {}

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


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lZipFileStream - Zip file stream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Just *reads* from a zipped memory buffer

class lZipFileStreamReader : public lIOStream
{
  void *         m_compressedData;
  unsigned int   m_compressedSize;
  unsigned int   m_uncompressedSize;
  unsigned int   m_uncompressedLeft;
  z_stream       m_zStream;
  int            m_zStatus;
  int            m_windowBits;

public:

  lZipFileStreamReader( void *data, int cSize, int ucSize, int wbits = -MAX_WBITS )
    : m_compressedData(data),
      m_compressedSize(cSize),
      m_uncompressedSize(ucSize),
      m_windowBits(wbits) // windowBits < 0 indicates no zlib header inside the data.
  {
    Init();
  }

  virtual ~lZipFileStreamReader()
  {
    Deinit();
  }

  // lZipFileStreamReader interface

  void       * GetCompressedData() const   { return m_compressedData; }
  unsigned int GetCompressedSize() const   { return m_compressedSize; }
  unsigned int GetUncompressedSize() const { return m_uncompressedSize; }
  int          GetStatus() const           { return m_zStatus; }

  // lIOStream interface

  virtual bool Reading(void)
  {
    return true;
  }

  virtual bool Writing(void)
  {
    return false;
  }

  virtual unsigned int Read(void *data,unsigned int size)
  {
    lAssertAlways( Reading() );

    if( size == 0 ) return 0;
    if( m_zStatus != Z_OK ) return 0;

    lAssert( data != NULL );

    m_zStream.next_out  = (Bytef*)data;
    m_zStream.avail_out = size;
    m_zStatus = inflate(&m_zStream, 0);

    unsigned int readBytes = size - m_zStream.avail_out;

    m_uncompressedLeft -= readBytes;

    // if m_zStatus == Z_OK : ok, progress made
    // if m_zStatus == Z_STREAM_END : eof of stream
    // otherwise, there was a problem

    return readBytes;
  }

  virtual unsigned int Write(void *data,unsigned int size)
  {
    // not supported, will always fire assert
    lAssertAlways( Writing() );
    return 0;
  }

  virtual void Flush(void)
  {
    // flush output and force buffered data to be written
    // nothing to do here.
  }

  virtual void Reset(void)
  {
    // reposition the stream to beginning
    Deinit();
    Init();
  }

  virtual unsigned int AvailableRead(void)
  {
    // bytes available to read without blocking
    return m_uncompressedLeft;
  }

private:

  // private implementation

  void Deinit()
  {
    inflateEnd(&m_zStream);
  }

  void Init()
  {
    memset( &m_zStream, 0, sizeof(m_zStream) );
    m_zStream.next_in   = (Bytef*)m_compressedData;
    m_zStream.avail_in  = (uInt)m_compressedSize;
    m_zStream.next_out  = (Bytef*)0;
    m_zStream.avail_out = 0;
    m_zStream.zalloc    = (alloc_func)NULL;
    m_zStream.zfree     = (free_func)NULL;
    m_zStatus           = inflateInit2(&m_zStream, m_windowBits);

    m_uncompressedLeft = m_uncompressedSize;
  }
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lZipFile - Zip file class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lZipFile
{
public:

  lZipFile() : m_file(NULL), m_files(NULL), m_dirData(NULL) {}
  ~lZipFile()
  {
    Close();
  }

  bool Open( lIOStreamSeekable * pSeekableStream )
  {
    Close();

    // read the header

    m_file = pSeekableStream;
    if( m_file == NULL ) return false;

    if( ! ReadHeader() )
    {
      Close();
      return false;
    }

    return true;
  }

  void Close()
  {
    if( m_file )    { m_file = NULL; }
    if( m_files   ) { delete[] m_files; m_files = NULL; }
    if( m_dirData ) { deallocate( __FILE__, __FUNCTION__,__LINE__ , m_dirData ); m_dirData = NULL; }
  }

  bool IsOpen() const
  {
    return m_file!=NULL;
  }

  lIOStreamSeekable * GetFile( const char * fileName ) const
  {
    lZIP_FILE_FIND_DATA find;
    lZIPFILE_FIND_HANDLE hFind = FindFirst( fileName, &find );
    lIOStreamSeekable * ret = NULL;
    if( hFind )
    {
      ret = GetFile( find.zipIndex );
    }
    FindClose( hFind );
    return ret;
  }

  bool GetFileToStream( lIOStream * stream, const char * fileName ) const
  {
    lZIP_FILE_FIND_DATA find;
    lZIPFILE_FIND_HANDLE hFind = FindFirst( fileName, &find );
    bool ret = false;
    if( hFind )
    {
      ret = GetFileToStream( stream, find.zipIndex );
    }
    FindClose( hFind );
    return ret;

  }

  bool GetFileToStream( lIOStream * iostream, unsigned int fileIndex ) const
  {
    if( !IsOpen() ) return NULL;
    if( iostream == NULL || !iostream->Writing() ) return false;
    if( fileIndex >= m_header.nDirEntries ) return false;

    // read the file header

    ZipLocalHeader hdr;
    m_file->Seek( m_files[fileIndex]->hdrOffset, L_IO_SEEK_SET );
    unsigned int bytesRead = m_file->Read( &hdr, sizeof(hdr) );
    if( bytesRead != sizeof(hdr) || hdr.sig != LOCAL_SIGNATURE ) return false;

    // allocate the read/write buffer

    unsigned int bufferSize = 1024*512;
    char * buffer = allocate<char>(__FILE__,__FUNCTION__, __LINE__,L_ARENA_IO,bufferSize);

    // read the file data

    m_file->Seek( hdr.fnameLen+hdr.xtraLen, L_IO_SEEK_CUR );

    switch( hdr.compression )
    {
      case LOCAL_COMP_STORE:
      {
        // read in uncompressed

        lAssert( hdr.cSize == hdr.ucSize );

        unsigned int totalSize = hdr.ucSize;
        while( totalSize )
        {
          unsigned int bytesRead = m_file->Read( buffer, bufferSize );

          if( bytesRead == 0 )
          {
            deallocate( __FILE__, __FUNCTION__,__LINE__, buffer );
            return false;
          }

          iostream->Write( buffer, bytesRead );
          totalSize -= bytesRead;
        }
      }
      break;

      case LOCAL_COMP_DEFLAT:
      {
        // read compresse data
        // TODO: should spool uncompressed data as well...

        char * pCompressed = allocate<char>( __FILE__, __FUNCTION__, __LINE__, L_ARENA_IO,hdr.cSize+1 );
        bytesRead = m_file->Read( pCompressed, hdr.cSize );

        pCompressed[hdr.cSize] = 0; // dummy byte for wbits<0, see below

        if( bytesRead != hdr.cSize )
        {
          deallocate( __FILE__, __FUNCTION__,__LINE__, pCompressed );
          deallocate( __FILE__, __FUNCTION__,__LINE__, buffer );
          return false;
        }

        // setup zstream

        z_stream stream = { 0 };
        stream.next_in   = (Bytef*)pCompressed;
        stream.avail_in  = (uInt)hdr.cSize+1;
        stream.next_out  = (Bytef*)buffer;
        stream.avail_out = bufferSize;
        stream.zalloc    = (alloc_func)NULL;
        stream.zfree     = (free_func)NULL;

        // Perform inflation; wbits < 0 indicates no zlib header inside the data.
        // windowBits is passed < 0 to tell that there is no zlib header.
        // Note that in this case inflate *requires* an extra "dummy" byte
        // after the compressed stream in order to complete decompression and
        // return Z_STREAM_END.

        int err = inflateInit2(&stream, -MAX_WBITS);

        if( err == Z_OK )
        {
          while( err == Z_OK )
          {
            err = inflate(&stream, Z_SYNC_FLUSH);

            if( stream.avail_out == 0 )
            {
              iostream->Write( buffer, bufferSize );
              stream.next_out  = (Bytef*)buffer;
              stream.avail_out = bufferSize;
            }
          }

          inflateEnd(&stream);
          if( err==Z_STREAM_END)
          {
            iostream->Write( buffer, bufferSize - stream.avail_out );
            err = Z_OK;
          }
          inflateEnd(&stream);
        }

        deallocate( __FILE__, __FUNCTION__,__LINE__, pCompressed );

        if( err!=Z_OK )
        {
          deallocate( __FILE__, __FUNCTION__,__LINE__, buffer );
          return false;
        }
      }
      break;

      default:
      {
        deallocate( __FILE__, __FUNCTION__,__LINE__, buffer );
        return false;
      }
    }

    deallocate( __FILE__, __FUNCTION__,__LINE__, buffer );

    return true;
  }

  /////

  void * GetCompressedData( const char * fileName, unsigned int &compressedSize, unsigned int &uncompressedSize ) const
  {
    compressedSize = 0;
    uncompressedSize = 0;

    lZIP_FILE_FIND_DATA find;
    lZIPFILE_FIND_HANDLE hFind = FindFirst( fileName, &find );
    void * ret = NULL;
    if( hFind )
    {
      ret = GetCompressedData( find.zipIndex, compressedSize, uncompressedSize );
    }
    FindClose( hFind );
    return ret;
  }

  void * GetCompressedData( unsigned int fileIndex, unsigned int &compressedSize, unsigned int &uncompressedSize ) const
  {
    compressedSize = 0;
    uncompressedSize = 0;

    if( !IsOpen() ) return NULL;
    if( fileIndex >= m_header.nDirEntries ) return NULL;

    // read the file header

    ZipLocalHeader hdr;
    m_file->Seek( m_files[fileIndex]->hdrOffset, L_IO_SEEK_SET );
    unsigned int bytesRead = m_file->Read( &hdr, sizeof(hdr) );
    if( bytesRead != sizeof(hdr) || hdr.sig != LOCAL_SIGNATURE ) return NULL;

    // read the file data

    m_file->Seek( hdr.fnameLen+hdr.xtraLen, L_IO_SEEK_CUR );

    switch( hdr.compression )
    {
      case LOCAL_COMP_DEFLAT:
      {
        // read compresse ddata

        char * pCompressed = allocate<char>( __FILE__, __FUNCTION__, __LINE__, L_ARENA_IO,hdr.cSize );
        bytesRead = m_file->Read( pCompressed, hdr.cSize );

        if( bytesRead != hdr.cSize )
        {
          deallocate( __FILE__, __FUNCTION__,__LINE__, pCompressed );
          return NULL;
        }

        compressedSize = hdr.cSize;
        uncompressedSize = hdr.ucSize;
        return pCompressed;
      }
      break;
    };

    return NULL;
  }

  /////


  lIOStreamSeekable * GetFile(unsigned int fileIndex) const
  {
    if( !IsOpen() ) return NULL;
    if( fileIndex >= m_header.nDirEntries ) return NULL;

    // read the file header

    ZipLocalHeader hdr;
    m_file->Seek( m_files[fileIndex]->hdrOffset, L_IO_SEEK_SET );
    unsigned int bytesRead = m_file->Read( &hdr, sizeof(hdr) );
    if( bytesRead != sizeof(hdr) || hdr.sig != LOCAL_SIGNATURE ) return NULL;

    // allocate the io buffer
    // note: we could use a lZipIOStream instead, but that class needs work.

    lBufferIOStream * ioStream = new lBufferIOStream( L_IO_ACCESS_READ );
    ioStream->GetDataArray().Add( hdr.ucSize );
    char * pData = (char *)ioStream->GetDataArray().Data();

    // read the file data

    m_file->Seek( hdr.fnameLen+hdr.xtraLen, L_IO_SEEK_CUR );

    switch( hdr.compression )
    {
      case LOCAL_COMP_STORE:
      {
        // read in uncompressed

        lAssert( hdr.cSize == hdr.ucSize );

        bytesRead = m_file->Read( pData, hdr.ucSize );

        if( bytesRead != hdr.ucSize )
        {
          delete ioStream;
          return NULL;
        }
      }
      break;

      case LOCAL_COMP_DEFLAT:
      {
        // read compresse ddata

        char * pCompressed = allocate<char>( __FILE__, __FUNCTION__, __LINE__, L_ARENA_IO,hdr.cSize );
        bytesRead = m_file->Read( pCompressed, hdr.cSize );

        if( bytesRead != hdr.cSize )
        {
          deallocate( __FILE__, __FUNCTION__,__LINE__, pCompressed );
          delete ioStream;
          return NULL;
        }

        // setup zstream

        z_stream stream;
        memset( &stream, 0, sizeof(stream) );
        stream.next_in   = (Bytef*)pCompressed;
        stream.avail_in  = (uInt)hdr.cSize;
        stream.next_out  = (Bytef*)pData;
        stream.avail_out = hdr.ucSize;
        stream.zalloc    = (alloc_func)NULL;
        stream.zfree     = (free_func)NULL;

        // Perform inflation; wbits < 0 indicates no zlib header inside the data.

        int err = inflateInit2(&stream, -MAX_WBITS);

        if( err==Z_OK )
        {
          err = inflate(&stream, Z_FINISH);
          inflateEnd(&stream);
          if( err==Z_STREAM_END) err = Z_OK;
          if( err==Z_BUF_ERROR && stream.avail_out == 0 )
          {
            // this is ok when Z_FINISH is used on inflate().
            // it just means zlib doesn't think it has reached the end of the input stream,
            // when infact it has from our point of view (we have all our output)
            err = Z_OK;
          }
          inflateEnd(&stream);
        }

        deallocate( __FILE__, __FUNCTION__,__LINE__, pCompressed );

        if( err!=Z_OK )
        {
          delete ioStream;
          return NULL;
        }
      }
      break;

      default:
      {
        delete ioStream;
        return NULL;
      }
    }

    return ioStream;
  }

  // Find file interface

  lZIPFILE_FIND_HANDLE FindFirst( const char * fileName, lZIP_FILE_FIND_DATA * pFind ) const
  {
    if( !IsOpen() ) return NULL;
    if( fileName == NULL ) return NULL;
    if( pFind == NULL ) return NULL;
    if( strlen( fileName ) >= L_FILE_MAX_PATH ) return NULL; // must be less than L_MAX_PATH

    memset( pFind, 0, sizeof(lZIP_FILE_FIND_DATA) );

    FileFindHandle * ffHnd = new FileFindHandle;
    strcpy( ffHnd->path, fileName );
    ffHnd->pos = 0;

    if( ! FindNext((lZIPFILE_FIND_HANDLE)ffHnd, pFind) )
    {
      delete ffHnd;
      return NULL;
    }
    else
    {
      return (lZIPFILE_FIND_HANDLE)ffHnd;
    }
  }

  bool FindNext( lZIPFILE_FIND_HANDLE hnd, lZIP_FILE_FIND_DATA * pFind ) const
  {
    if( !IsOpen() ) return false;
    if( hnd == NULL ) return false;
    if( pFind == NULL ) return false;

    FileFindHandle * ffHnd = (FileFindHandle *)hnd;

    while( true )
    {
      if( ffHnd->pos >= m_header.nDirEntries ) return false;

      ZipDirFileHeader *fh = m_files[ffHnd->pos];

      // get the file name

      char szFile[L_FILE_MAX_PATH];

      int len = fh->fnameLen;
      if( len >= L_FILE_MAX_PATH-1 ) len = L_FILE_MAX_PATH-2;
      memcpy( szFile, fh->GetName(), len );
      szFile[len] = NULL;

      lAssert( len < L_FILE_MAX_PATH );

      if( xstrcmp( ffHnd->path, szFile ) )
      {
        pFind->creationTime   = DosTimeDataToTimeT( fh->modDate, fh->modTime );
        pFind->size           = fh->ucSize;
        pFind->compressedSize = fh->cSize;
        pFind->zipIndex       = ffHnd->pos;

        strcpy( pFind->name, szFile );
        strcpy( pFind->comment, "" );

        ffHnd->pos++;

        return true;
      }

      ffHnd->pos++;
    }
    return false;
  }

  bool FindClose( lZIPFILE_FIND_HANDLE hnd ) const
  {
    if( hnd == NULL ) return false;
    FileFindHandle * ffHnd = (FileFindHandle *)hnd;
    delete ffHnd;
    return true;
  }

private:

  bool ReadHeader()
  {
    lAssert( m_file != NULL );

    if( m_files   ) { delete[] m_files; m_files = NULL; }
    if( m_dirData ) { deallocate( __FILE__, __FUNCTION__,__LINE__, m_dirData ); m_dirData = NULL; }

    // read the zip directory header, located at the end of the file

    m_file->Seek( -(int)sizeof(m_header), L_IO_SEEK_END );
    unsigned int bytesRead = m_file->Read( &m_header, sizeof(m_header) );

    if( bytesRead != sizeof(m_header) || m_header.sig != DIR_SIGNATURE )
    {
      return false;
    };

    // read the directory info

    m_dirData = allocate<char>( __FILE__, __FUNCTION__, __LINE__, L_ARENA_IO,m_header.dirSize );
    m_file->Seek( -(int)(sizeof(m_header)+m_header.dirSize), L_IO_SEEK_END );
    bytesRead = m_file->Read( m_dirData, m_header.dirSize );

    if( bytesRead != m_header.dirSize )
    {
      deallocate( __FILE__, __FUNCTION__,__LINE__, m_dirData );
      return false;
    }

    // process directory data

    m_files = new ZipDirFileHeader *[m_header.nDirEntries];
    char * pData = m_dirData;

    for( int i=0; i<m_header.nDirEntries; i++ )
    {
      ZipDirFileHeader *fh = (ZipDirFileHeader *)pData;
      m_files[i] = fh;

      if( fh->sig != FILE_SIGNATURE )
      {
        deallocate( __FILE__, __FUNCTION__,__LINE__, m_dirData );
        delete[] m_files;
        return false;
      }

      // Get next header

      pData += sizeof(ZipDirFileHeader) + fh->fnameLen + fh->xtraLen + fh->cmntLen;
    }

    return true;
  }

 #ifdef L_HAS_PRAGMA_PACK
  #pragma pack(push,2)
 #endif

  enum
  {
    DIR_SIGNATURE = 0x06054b50,
  };

  struct ZipDirHeader L_GCC_PACKED(2)
  {
    unsigned int   sig;
    unsigned short nDisk;
    unsigned short nStartDisk;
    unsigned short nDirEntries;
    unsigned short totalDirEntries;
    unsigned int   dirSize;
    unsigned int   dirOffset;
    unsigned short cmntLen;
  };

  enum
  {
     FILE_SIGNATURE    = 0x02014b50,
     FILE_COMP_STORE   = 0,
     FILE_COMP_DEFLAT  = 8,
  };

  struct ZipDirFileHeader L_GCC_PACKED(2)
  {
     unsigned int   sig;
     unsigned short verMade;
     unsigned short verNeeded;
     unsigned short flag;
     unsigned short compression;      // COMP_xxxx
     unsigned short modTime;
     unsigned short modDate;
     unsigned int   crc32;
     unsigned int   cSize;            // Compressed size
     unsigned int   ucSize;           // Uncompressed size
     unsigned short fnameLen;         // Filename string follows header.
     unsigned short xtraLen;          // Extra field follows filename.
     unsigned short cmntLen;          // Comment field follows extra field.
     unsigned short diskStart;
     unsigned short intAttr;
     unsigned int   extAttr;
     unsigned int   hdrOffset;

     const char * GetName() const { return (const char *)(this + 1); }
     const char * GetExtra() const { return GetName() + fnameLen; }
     const char * GetComment() const { return GetExtra() + xtraLen; }
  };

  enum
  {
     LOCAL_SIGNATURE   = 0x04034b50,
     LOCAL_COMP_STORE  = 0,
     LOCAL_COMP_DEFLAT = 8,
  };

  struct ZipLocalHeader L_GCC_PACKED(2)
  {
     unsigned int   sig;
     unsigned short version;
     unsigned short flag;
     unsigned short compression;      // COMP_xxxx
     unsigned short modTime;
     unsigned short modDate;
     unsigned int   crc32;
     unsigned int   cSize;
     unsigned int   ucSize;
     unsigned short fnameLen;         // Filename string follows header.
     unsigned short xtraLen;          // Extra field follows filename.
  };

 #ifdef L_HAS_PRAGMA_PACK
  #pragma pack(pop)
 #endif

  struct FileFindHandle
  {
    char path[L_FILE_MAX_PATH];
    int  pos;
  };

  lIOStreamSeekable  * m_file;
  ZipDirHeader         m_header;
  ZipDirFileHeader  ** m_files;
  char               * m_dirData;

  // Support routines

  time_t DosTimeDataToTimeT( unsigned short dosDate, unsigned short dosTime ) const
  {
#ifdef L_PLATFORM_SYMBIAN
    //NOTE: HACK: Need to implement conversion on Symbian, tzset not available
    return time(NULL);
#else
    // The following calculations are based on code from the VC6 CRT
    //

    const unsigned int _BASE_YEAR        = 70L;  // 1970 is the base year
    const unsigned int _LEAP_YEAR_ADJUST = 17L;  // Leap years 1900 - 1970
    const unsigned int _MAX_YEAR         = 138L; // 2038 is the max year
    const int _days[] =
          { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

    int yr      =  1980 + (dosDate >> 9);    /* 0 based */
    int mo      =  (dosDate >> 5 ) & 0xf;    /* 1 based */
    int dy      =  (dosDate ) & 0xf;         /* 1 based */
    int hr      =  (dosTime >> 11 );
    int mn      =  (dosTime >> 5 ) & 0x3f;
    int sc      =  (dosTime & 0xf) >> 1;
    int dstflag = -1;

    int    tmpdays;
    long   tmptim;
    struct tm tb;

    /*
     * Do a quick range check on the year and convert it to a delta
     * off of 1900.
     */
    if ( ((long)(yr -= 1900) < _BASE_YEAR) || ((long)yr > _MAX_YEAR) )
            return (time_t)(-1);

    /*
     * Compute the number of elapsed days in the current year. Note the
     * test for a leap year would fail in the year 2100, if this was in
     * range (which it isn't).
     */
    tmpdays = dy + _days[mo - 1];
    if ( !(yr & 3) && (mo > 2) )
            tmpdays++;

    /*
     * Compute the number of elapsed seconds since the Epoch. Note the
     * computation of elapsed leap years would break down after 2100
     * if such values were in range (fortunately, they aren't).
     */
    tmptim = /* 365 days for each year */
              (((long)yr - _BASE_YEAR) * 365L

              /* one day for each elapsed leap year */
              + (long)((yr - 1) >> 2) - _LEAP_YEAR_ADJUST

              /* number of elapsed days in yr */
              + tmpdays)

              /* convert to hours and add in hr */
              * 24L + hr;

    tmptim = /* convert to minutes and add in mn */
              (tmptim * 60L + mn)

              /* convert to seconds and add in sec */
              * 60L + sc;
    /*
     * Account for time zone.
     */
    _tzset();
    tmptim += _timezone;

    /*
     * Fill in enough fields of tb for _isindst(), then call it to
     * determine DST.
     */
    tb.tm_yday = tmpdays;
    tb.tm_year = yr;
    tb.tm_mon  = mo - 1;
    tb.tm_hour = hr;
    if ( (dstflag == 1) || ((dstflag == -1) && _daylight &&
                            _isindst(&tb)) )
            tmptim += _dstbias;

    return(tmptim);
#endif
  }

  // all your base are belong to us
  bool patimat(char *pat, char *str) const
  {
    switch (*pat)
    {
      case '\0': return !*str;
      case '*' : return patimat(pat+1, str) || *str && patimat(pat, str+1);
      case '?' : return *str && patimat(pat+1, str+1);
      case '\\': return (*str == '\\' || *str == '/') && patimat(pat+1, str+1);
      case '/' : return (*str == '\\' || *str == '/') && patimat(pat+1, str+1);
      default  : return (toupper(*pat) == toupper(*str)) && patimat(pat+1, str+1);
    }
  }

  bool xstrcmp (char *pat, char *str) const
  {
    if (str == NULL || pat == NULL)
     return false;

    return(patimat(pat, str));
  }
};


END_L_NAMESPACE
#endif
