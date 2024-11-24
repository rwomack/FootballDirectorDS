//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// File        : fdloadsave.cpp
// Description : Load / Save code
// Notes       :
//
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdeditmodedata.h>
#include <lemon/math/lmrand.h>
#include <lemon/io/lmemoryiostream.h>
#include <lemon/io/lzipiostream.h>
#include <lemon/hash/lcrc32.h>
#include <fdcommandlineoptions.h>

using namespace L_NAMESPACE_NAME;

#ifdef L_PLATFORM_NDS
#define CALLBACKINTERVAL 5000
#else
#define CALLBACKINTERVAL 50000
#endif

extern void ForceLoadSaveHeap( bool b );

#ifdef L_PLATFORM_WINDOWS
#define WINDOWS_COMPRESSED
#endif

// define this to have the savefile named specifically for the team / seed in use by autoplay, note normal games still use 'game.save'
#define  AUTOPLAY_SPECIFIC_SAVEFILE_NAME

//#define DISABLE_SAVE
//#define DONT_WRITE_TO_CARD

static bool SaveEditModeData(CDataFile *saveFile);
static bool LoadEditModeData(CDataFile *saveFile);


static FDLoadSaveCallback * s_callback = NULL;
static  int                 s_callbackTotal;
static  int                 s_callbackLast;

// Used to stop sleep mode during access
bool s_bPerformingCARDAccess = false;

class CardAccessHelper
{
 public:
  CardAccessHelper()
  {
    lLogFmt("Setting card access flag\n");
    s_bPerformingCARDAccess= true;
  }

  ~CardAccessHelper()
  {
    lLogFmt("Releasing card access flag\n");
    s_bPerformingCARDAccess= false;
  }
};

class ForceLoadSaveHeapHelper
{
 public:
  ForceLoadSaveHeapHelper()
  {
    lLogBreak();
    lLogFmt(">>Setting loadsave heap flag\n");
    lLogBreak();
    ForceLoadSaveHeap( true );
  }

  ~ForceLoadSaveHeapHelper()
  {
    lLogBreak();
    lLogFmt("<<Releasing loadsave heap flag\n");
    lLogBreak();
    ForceLoadSaveHeap( false );
  }
};

#define ROMSIZE     (1 << 18)

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// NDS save stuff
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#ifdef L_PLATFORM_NDS

#include <nitro/card.h>

//#define FORCE_CORRUPT
#define STORAGELOG  lLogFmt


static int s_cardLockId = OS_LOCK_ID_ERROR;

enum
{
  READ_OK = 0,
  READ_CORRUPT,
  READ_FAIL,
};

#include "fdndserror.h"

#define MAGICID    0xFEED7234
#define NUM_MAGIC  10

struct FDStorageHeader
{
  uint32 magicID[NUM_MAGIC];            // store multiple magicID's in a row
  uint32 crc;
  uint32 size;
};

#define OPTIONS_OFFSET  (0)
#define EDITMODE_OFFSET (OPTIONS_OFFSET  + sizeof(FDSaveData) + sizeof(FDStorageHeader))
#define SAVEGAME_OFFSET (EDITMODE_OFFSET + sizeof(FDEditModeData) + sizeof(FDStorageHeader))

static void WriteMagic( FDStorageHeader *hdr )
{
  for(int i = 0; i < NUM_MAGIC; i++)
  {
    hdr->magicID[i] = MAGICID;
  }
}

static uint32 s_comparisonmagicID[NUM_MAGIC];            // store multiple magicID's in a row
static bool   s_comparisonmagicIDInit = false;

static bool VerifyMagic( FDStorageHeader *hdr )
{
  if( !s_comparisonmagicIDInit )
  {
    s_comparisonmagicIDInit = true;
    for(int i = 0; i < NUM_MAGIC; i++)
    {
      s_comparisonmagicID[i] = MAGICID;
    }
  }

  uint8 *src   = (uint8*)hdr->magicID;
  uint8 *valid = (uint8*)s_comparisonmagicID;

  int validBytes = 0;
  for(int i = 0; i < NUM_MAGIC*4; i++)
  {
    /*
    for(int j = 0; j < 8; j++)
    {
      int left  = src[i]   & (1<<j);
      int right = valid[i] & (1<<j);

      if( left == right )
      {
        validBits++;
      }
    }*/
    if( src[i] == valid[i] )
    {
      validBytes++;
    }
  }

  //int totalBits = NUM_MAGIC*4*8;
  //int passBits  = NUM_MAGIC*4*6;
  int totalBytes = NUM_MAGIC*4;
  int passBytes  = NUM_MAGIC;

  //lLogFmt("%d bits of %d valid (Pass = %d)\n", validBits, totalBits, passBits);
  lLogFmt("%d bytes of %d valid (Pass = %d)\n", validBytes, totalBytes, passBytes);
  if( validBytes > passBytes )
  {
    lLogFmt("  Valid\n");
    return true;
  }
  return false;
}


static bool s_bInit = false;

static void StorageInit()
{
  if( !s_bInit )
  {
    s_bInit = true;

    lLogFmt("StorageInit\n");
    lLogFmt("  OPTIONS_OFFSET: %d\n", OPTIONS_OFFSET);
    lLogFmt("  EDITMODE_OFFSET: %d\n", EDITMODE_OFFSET);
    lLogFmt("  SAVEGAME_OFFSET: %d\n", SAVEGAME_OFFSET);

    lLogFmt("SaveData: %d\n", sizeof(FDSaveData));
    lLogFmt("EditModeData: %d\n", sizeof(FDEditModeData));

    CARD_Init();

    int lockId = OS_GetLockID();
    if (lockId == OS_LOCK_ID_ERROR)
    {
      lAssert("StorageInit - GetLockID() failed");
    }
    s_cardLockId = lockId;

    CARD_LockBackup(s_cardLockId);
    bool success = CARD_IdentifyBackup(CARD_BACKUP_TYPE_FLASH_2MBITS);
    CARD_UnlockBackup(s_cardLockId);

    if (!success)
    {
      //NOTE: Should fail instead?
      //lAssert("lStorageManager::Init - CARD_identifyBackup failed");
      lLogFmt("StorageInit - CARD_identifyBackup failed");
      ErrorHandle( ERROR_ACCESS_FAIL );
      return;
    }
  }
}

static bool StorageConfirmBackupMemory()
{
 // if( s_storageState != STORAGE_STATE_IDLE ) return false;
  //NOTE: Commented out to save it messing up load process

  STORAGELOG("ConfirmBackupMemory(): %d\n", s_cardLockId);
  CARD_LockBackup(s_cardLockId);
  uint8 byte;
  bool success = CARD_ReadFlash( 0, &byte, 1 );
  CARD_UnlockBackup(s_cardLockId);
  STORAGELOG("ConfirmBackupMemory(): %s\n", success ? "SUCCESS" : "FAIL");
  return success;
}

////

extern bool DoMainLoop();

static bool WaitOnAsync()
{
  float bla = 0.0f;
  while( !CARD_TryWaitBackupAsync() )
  {
    // process ongoing
    OS_Sleep(2);
    //DoMainLoop();
    //s_callback->SaveProgress( bla );
    //bla += 0.005f;
  }

  // return true if succesful, false if not
  return (CARD_GetResultCode() == CARD_RESULT_SUCCESS);
}

static bool WriteData( int romOffset, const char * data, int dataSize )
{
  STORAGELOG("WriteData(): %d - %x - %d\n", s_cardLockId, data, dataSize);

  if( (romOffset + dataSize + sizeof(FDStorageHeader) ) > ROMSIZE )
  {
    lLogFmt("Not enough space for storage header + data! off%d + dat%d + hdr%d > %d\n", romOffset, dataSize, sizeof(FDStorageHeader), ROMSIZE);
    return false;
  }

  // create crc + magic ID
  FDStorageHeader hdr;
  WriteMagic( &hdr );
  hdr.crc     = lCRC32( 0, data, dataSize);
  hdr.size    = dataSize;

  lLogFmt("Created header for %d bytes - CRC %08x\n", hdr.size, hdr.crc);

  // write
  bool success = true;

  // write header
  CARD_LockBackup(s_cardLockId);
  CARD_WriteAndVerifyFlashAsync( romOffset, &hdr, sizeof(FDStorageHeader), NULL, NULL );
  success = WaitOnAsync();
  CARD_UnlockBackup(s_cardLockId);

  if( !success )
  {
    lLogFmt(" Fail on header write!\n");

    // NOTE: This is bad as we now have corrupt data stored
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  // write data
  CARD_LockBackup(s_cardLockId);
  CARD_WriteAndVerifyFlashAsync( romOffset + sizeof(FDStorageHeader), data, dataSize, NULL, NULL );
  success = WaitOnAsync();
  CARD_UnlockBackup(s_cardLockId);

  STORAGELOG("WriteData(): %s\n", success ? "SUCCESS" : "FAIL");
  return success;
}

static bool BlankData( int romOffset )
{
  STORAGELOG("BlankData(): %d - %x - %d\n", s_cardLockId);

  if( (romOffset + sizeof(FDStorageHeader) ) > ROMSIZE )
  {
    lLogFmt("Not enough space for storage header + data! off%d + hdr%d > %d\n", romOffset, sizeof(FDStorageHeader), ROMSIZE);
    return false;
  }

  // create crc + magic ID
  FDStorageHeader hdr;
  memset( &hdr, 0, sizeof(FDStorageHeader) );

  // write
  bool success = true;

  // write a blank header
  CARD_LockBackup(s_cardLockId);
  CARD_WriteAndVerifyFlashAsync( romOffset, &hdr, sizeof(FDStorageHeader), NULL, NULL );
  success = WaitOnAsync();
  CARD_UnlockBackup(s_cardLockId);

  if( !success )
  {
    lLogFmt(" Fail on header write!\n");

    // NOTE: This is bad as we now have corrupt data stored
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  STORAGELOG("BlankData(): %s\n", success ? "SUCCESS" : "FAIL");
  return success;
}


static bool DoesSaveExist( int romOffset )
{
  STORAGELOG("DoesSaveExist(): %d\n", s_cardLockId);

  // read checksum + magic ID
  FDStorageHeader header;

  CARD_LockBackup(s_cardLockId);
  bool successhdr = CARD_ReadFlash( romOffset, &header, sizeof(FDStorageHeader) );
  CARD_UnlockBackup(s_cardLockId);

  if( !successhdr )
  {
    lLogFmt("Error checking for save\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( VerifyMagic( &header ) )
  {
    lLogFmt("Read valid header: CRC: %x - size:%d\n", header.crc, header.size);
  } else
  {
    lLogFmt("Read invalid header: CRC: %x - size:%d\n", header.crc, header.size);
    return false;
  }

  if( header.size == 0 )
  {
    lLogFmt("Size = 0 - not valid\n");
    return false;
  }

  return true;
}


static int ReadData( int romOffset, char ** data, int *datasize )
{
  STORAGELOG("Readdata(): %d\n", s_cardLockId);

  if( data == NULL || datasize == NULL )
  {
    lLogFmt("Require char ** pointer\n");
    return READ_FAIL;
  }

  // read checksum + magic ID
  FDStorageHeader header;

  CARD_LockBackup(s_cardLockId);
  CARD_ReadFlashAsync( romOffset, &header, sizeof(FDStorageHeader), NULL, NULL );
  bool successhdr = WaitOnAsync();
  CARD_UnlockBackup(s_cardLockId);

  if( !successhdr )
  {
    // error reading
    ErrorHandle( ERROR_ACCESS_FAIL );
    lLogFmt("Error reading header\n");
    return READ_FAIL;
  }

  if( VerifyMagic( &header ) )
  {
    lLogFmt("Read valid header: CRC: %x - size:%d\n", header.crc, header.size);
  } else
  {
    lLogFmt("Read invalid header: CRC: %x - size:%d\n", header.crc, header.size);
    return READ_FAIL;
  }

  // check if size is valid
  if( header.size > ROMSIZE )
  {
    lLogFmt("Error - can't save more than ROM size - must be corrupt\n");
    return READ_CORRUPT;
  }

  // alloc space
  *data = (char *)FD_LOADSAVEMUSIC_NEW char[ header.size ];
  *datasize = header.size;

  // read data
  CARD_LockBackup(s_cardLockId);
  CARD_ReadFlashAsync( romOffset + sizeof(FDStorageHeader), *data, header.size, NULL, NULL );
  bool success = WaitOnAsync();
  CARD_UnlockBackup(s_cardLockId);

  if( !success )
  {
    // error reading
    lLogFmt("Error reading: %d/%d\n", successhdr, success);
    delete *data;
    *data = NULL;
    ErrorHandle( ERROR_ACCESS_FAIL );
    return READ_FAIL;
  }

  // check checksum!
  uint32 testcrc = lCRC32( 0, *data, header.size );

  #ifdef FORCE_CORRUPT
  testcrc = 0xFEFEFEFE;                 // would be unlucky to be the same!
  #endif

  bool checksumOK = (testcrc == header.crc) ? true : false;

  // was checksum ok
  if( checksumOK )
  {
    lLogFmt("Checksum ok - read ok\n");
    return READ_OK;
  } else
  {
    lLogFmt("Checksum fail on read\n");
    delete *data;
    *data = NULL;
    return READ_CORRUPT;
  }

  return READ_OK;
}


//TODO
// On reading check CRC - if fail then we have to reinit database

class CDataFileFlashROMWrite : public lIOStreamSeekable
{
  #define BUFFER_SIZE 2048

  uint8           m_writeBuffer[ BUFFER_SIZE ];
  int             m_writeBufferPos;

  int             m_writeStartPos;
  int             m_writePos;

  int             m_crc;

  bool            m_bValid;

public:
  CDataFileFlashROMWrite( int writePos ) : m_writeStartPos( writePos ), m_writeBufferPos( 0 ), m_bValid( true ), m_crc(0)
  {
    m_writePos = m_writeStartPos + sizeof(FDStorageHeader);
    lLogFmt("CDataFileFlashROMWrite - %d (header) - %d (data)\n", m_writeStartPos, m_writePos);

    lLogFmt("Locking card\n");
  }

  virtual ~CDataFileFlashROMWrite()
  {
    Flush();
    lLogFmt("Unlocking card\n");
  }

  virtual bool         IsValid()
  {
    return m_bValid;
  }

  virtual bool         Reading(void)
  {
    return false;
  }

  virtual bool         Writing(void)
  {
    return true;
  }

  virtual unsigned int Read(void *data, unsigned int size)
  {
    return 0;
  }

  virtual unsigned int Write(const void *data, unsigned int size)
  {
    lLogFmt("Write %x - %d bytes\n", data, size);

    // copy into buffer and write a piece at a time, CRC as we go
    uint8 *src = (uint8*)data;
    int remain = size;
    while( remain )
    {
      int tocopy = lmMin( remain, BUFFER_SIZE - m_writeBufferPos );

      memcpy( &m_writeBuffer[ m_writeBufferPos ], src, tocopy );
      src += tocopy;
      m_writeBufferPos += tocopy;
      remain -= tocopy;

      if( m_writeBufferPos == BUFFER_SIZE )
      {
        // write buffer out
        FlushBuffer();
      }
    }

    return size;
  }

  void FlushBuffer()
  {
    lLogFmt("Flushing buffer %d\n", m_writeBufferPos);

    if( m_writeBufferPos > 0 )
    {
      // do callback
      if( s_callback != NULL )
      {
        float pct = (float)s_callbackLast / (float)s_callbackTotal;
        pct = lmClamp( pct, 0.0f, 1.0f );
        s_callback->SaveProgress( pct );
      }

      // add to crc
      m_crc = lCRC32( m_crc, m_writeBuffer, m_writeBufferPos );

      bool success = true;

      if (FDCommandLineOptions::IsAutoPlay() == true)
      {
        //lLogFmt("  ** NOT ACTUALLY WRITING - AUTOPLAY\n");
      } else
      {
        // write to card
#ifndef DONT_WRITE_TO_CARD
        CARD_LockBackup(s_cardLockId);
        CARD_WriteAndVerifyFlashAsync( m_writePos, m_writeBuffer, m_writeBufferPos, NULL, NULL );
        success = WaitOnAsync();
        CARD_UnlockBackup(s_cardLockId);
#endif
      }
      m_writePos += m_writeBufferPos;
      m_writeBufferPos = 0;

      if( !success )
      {
        // log error
        ErrorHandle( ERROR_ACCESS_FAIL );
      }
    }
  }

  virtual unsigned int Seek(int offset,L_IO_SEEK whence)
  {
    lLogFmt("ERROR: Seek called\n");
    return 0;
  }

  //##
  //  Return the file position from the start of the file
  virtual unsigned int Tell(void)
  {
    lLogFmt("ERROR: Tell called\n");
    return 0;
  }

  //##
  //  Return the file length
  virtual unsigned int Length(void)
  {
    lLogFmt("ERROR: Length called\n");
    return 0;
  }


  void Flush()
  {
    lLogFmt("Flush - startpos %d\n", m_writeStartPos);

    // write remaining data
    FlushBuffer();

    // calc data size
    int dataSize = m_writePos - m_writeStartPos - sizeof(FDStorageHeader);
    lLogFmt("Data: %d bytes - %08x crc\n", dataSize, m_crc);

    // write crc + magic ID
    FDStorageHeader hdr;
    WriteMagic( &hdr );
    hdr.crc     = m_crc;                // data crc
    hdr.size    = dataSize;  // data size

    // write header data
    bool success = true;
    if (FDCommandLineOptions::IsAutoPlay() == true)
    {
      //lLogFmt("  ** NOT ACTUALLY WRITING - AUTOPLAY\n");
    } else
    {
#ifndef DONT_WRITE_TO_CARD
      CARD_LockBackup(s_cardLockId);
      CARD_WriteAndVerifyFlashAsync( m_writeStartPos, &hdr, sizeof(FDStorageHeader), NULL, NULL );
      success = WaitOnAsync();
      CARD_UnlockBackup(s_cardLockId);
#endif
    }

    if( !success )
    {
      //log error
      ErrorHandle( ERROR_ACCESS_FAIL );
    }

    STORAGELOG("WriteData(): %s\n", success ? "SUCCESS" : "FAIL");
  }
};

#endif

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Windows save stuff
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

#ifdef L_PLATFORM_WINDOWS

class CDataFileLocalFile : public CDataFile
{
  lFILE_HANDLE m_hnd;
  bool         m_bSave;
  int          m_written;

public:
  CDataFileLocalFile( const char *file, bool bSave )
  {
    lLogFmt("CDataFileLocalFile: %s\n", file);

    s_callback = NULL;
    s_callbackLast = 0;
    s_callbackTotal = 1;

    m_written = 0;
    m_bSave = bSave;
    if( bSave )
    {
      m_hnd = lPlatform_FileOpen( file, L_IO_ACCESS_WRITE );
    } else
    {
      if( lPlatform_FileExists( file ) )
      {
        m_hnd = lPlatform_FileOpen( file, L_IO_ACCESS_READ, L_IO_OPEN_EXISTING );
      } else
      {
        m_hnd = lINVALID_FILE_HANDLE;
      }
    }

    if( !IsValid() )
    {
      lLogFmt("   FAILED!\n");
    }
  }

  virtual ~CDataFileLocalFile()
  {
#ifdef WINDOWS_COMPRESSED
    if( m_bSave )
    {
      lLogFmt("Wrote %d compressed\n", m_written);
      if( m_written >= 225000 )
      {
        lLogFmt("Wrote %d - too much\n", m_written);
        lAssertAlways( !"Wrote too much data" );
      }
    }
#endif

    lPlatform_FileClose(m_hnd);
  }

  virtual bool         IsValid()
  {
    return ( m_hnd != lINVALID_FILE_HANDLE );
  }

  virtual bool         Reading(void)
  {
    return !m_bSave;
  }

  virtual bool         Writing(void)
  {
    return m_bSave;
  }

  virtual unsigned int Read(void *data, unsigned int size)
  {
    if( m_bSave ) return 0;
    return lPlatform_FileRead( m_hnd, data, size );
  }

  void SetCallback( FDLoadSaveCallback *callback, int total )
  {
    s_callback = callback;
    s_callbackTotal = total;
  }

  virtual unsigned int Write(const void *data, unsigned int size)
  {
    if( !m_bSave ) return 0;
    m_written += size;

    if( s_callback != NULL )
    {
      if( (m_written - s_callbackLast) > CALLBACKINTERVAL )
      {
        float pct = (float)m_written / (float)s_callbackTotal;
        pct = lmClamp( pct, 0.0f, 1.0f );
        s_callback->SaveProgress( pct );
        s_callbackLast = m_written;
      }
    }

    return lPlatform_FileWrite( m_hnd, data, size );
  }

  virtual void         Debug( const char *str )
  {
    if( IsValid() )
    {
      static int s_lastPos = 0;
      int curPos = lPlatform_FileTell( m_hnd );

      int diff = curPos - s_lastPos;

      s_lastPos = curPos;

      lLogFmt("%s: pos %d (%s) - %d diff\n", m_bSave ? "WRITING" : "READING", curPos, str, diff );
    } else
    {
      lLogFmt("%s: Invalid m_hnd", m_bSave ? "WRITING" : "READING");
    }
  }
};

#endif

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Compressed save
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

class CDataFileCompressed : public CDataFile
{
  bool                     m_bSave;
  lZipIOStream           * m_zipStream;
  lIOStreamSeekable      * m_stream;

  int                      m_totalSize;
  bool                     m_bValid;

public:
  CDataFileCompressed( bool bSave, lIOStreamSeekable *stream ) : m_zipStream( NULL ),
                                                                 m_bSave( bSave ),
                                                                 m_stream( stream ),
                                                                 m_totalSize( 0 )
  {
    lLogFmt("CDataFileFlashROM\n");

    s_callback = NULL;
    s_callbackLast = 0;
    s_callbackTotal = 1;

    // On save
    if( bSave )
    {
      // Create compressor
      m_zipStream = FD_GAME_NEW lZipIOStream( stream, L_IO_ACCESS_WRITE, 2048 );
    } else
    {
      m_zipStream = FD_GAME_NEW lZipIOStream( stream, L_IO_ACCESS_READ, 2048 );
    }

    m_bValid = true;
  }

  virtual ~CDataFileCompressed()
  {
    delete m_zipStream;
  }

  void SetCallback( FDLoadSaveCallback *callback, int total )
  {
    s_callback = callback;
    s_callbackTotal = total;
  }


  virtual bool         IsValid()
  {
    return m_bValid;
  }

  virtual bool         Reading(void)
  {
    return !m_bSave;
  }

  virtual bool         Writing(void)
  {
    return m_bSave;
  }

  virtual unsigned int Read(void *data, unsigned int size)
  {
    if( m_bSave ) return 0;
    m_totalSize += size;
    return m_zipStream->Read( data, size );
  }

  virtual unsigned int Write(const void *data, unsigned int size)
  {
    if( !m_bSave ) return 0;
    m_totalSize += size;

    if( s_callback != NULL )
    {
      if( (m_totalSize - s_callbackLast) > CALLBACKINTERVAL )
      {
        float pct = (float)m_totalSize / (float)s_callbackTotal;
        pct = lmClamp( pct, 0.0f, 1.0f );
        s_callback->SaveProgress( pct );
        s_callbackLast = m_totalSize;
      }
    }

    return m_zipStream->Write( data, size );
  }

  virtual void         Debug( const char *str )
  {
#ifdef L_PLATFORM_WINDOWS
    if( IsValid() )
    {
      static int s_lastPos = 0;
      int curPos = m_totalSize;

      int diff = curPos - s_lastPos;

      s_lastPos = curPos;

      lLogFmt("%s: pos %d (%s) - %d diff\n", m_bSave ? "WRITING" : "READING", curPos, str, diff );
    } else
    {
      lLogFmt("%s: Invalid m_hnd", m_bSave ? "WRITING" : "READING");
    }
#endif
  }
};




//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Options load/save/exist check
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
bool FDMainMenuUI::OptionsLoad()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif


  lLogBreak();

  lLogFmt("Performing load!\n");
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

#ifdef L_PLATFORM_WINDOWS

  CDataFileLocalFile *optionsFile = FD_GAME_NEW CDataFileLocalFile( "options.save", false );

  if( !optionsFile->IsValid() )
  {
    lLogFmt("Couldn't open %s to read\n", "options.save");
    return false;
  }

#else                                   // #ifdef L_PLATFORM_WINDOWS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( OPTIONS_OFFSET ) )
  {
    lLogFmt("  Data doesn't exist!\n");
    return false;
  }

  // Load compressed data
  lMemoryIOStream memStream;
  char *ptr = NULL;
  int   ptrsize = 0;
  int ret = ReadData( OPTIONS_OFFSET, &ptr, &ptrsize );

  if( ret == READ_OK )
  {
    // create array stream and read with zipiostream
    lLogFmt("Read data %d!\n", ptrsize);
    memStream.Init( ptr, ptrsize, L_IO_ACCESS_READ );
  } else
  if( ret == READ_CORRUPT )
  {
    lLogFmt("  Corrupt options data\n");
    ErrorHandle( ERROR_ACCESS_CORRUPT );

    if( ptr != NULL )
    {
      lFree( ptr );
    }

    // now overwrite *all* the data
    OptionsDefault();
    OptionsWipe();
    EditModeDefault();
    EditModeWipe();
    GamedataWipe();
    return false;
  } else
  {
    lLogFmt("Error reading data\n");
    if( ptr != NULL )
    {
      lFree( ptr );
    }

    return false;
  }

  lMemoryIOStream *optionsFile = &memStream;
#endif                                  // #ifdef L_PLATFORM_WINDOWS

  // load engine data
  if (optionsFile->Read(&m_saveData, sizeof(m_saveData)) != sizeof(m_saveData))
  {
    lLogFmt("loading options failed!\n");
#ifdef L_PLATFORM_NDS
    if( ptr != NULL )
    {
      lFree( ptr );
    }
#else
    delete optionsFile;
#endif
    return false;
  }

  lLogBreak();

#ifdef L_PLATFORM_NDS
  if( ptr != NULL )
  {
    lFree( ptr );
  }
#endif

  lLogFmt("Loaded\n");
  return true;
}

bool FDMainMenuUI::OptionsSave()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  CardAccessHelper helper;
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

  lLogBreak();
  lLogFmt("Performing save!\n");

#ifdef L_PLATFORM_WINDOWS
  CDataFileLocalFile *optionsFile = FD_GAME_NEW CDataFileLocalFile( "options.save", true );

  if( !optionsFile->IsValid() )
  {
    lLogFmt("Couldn't open %s to write\n", "options.save");
    delete optionsFile;
    return false;
  }

  // load game data
  optionsFile->Write( &m_saveData, sizeof(m_saveData) );

  delete optionsFile;

  lLogFmt("Saved options\n");
  return true;
#else
  StorageInit();

  static FDSaveData s_lastSavedData;

  if( !memcmp(&s_lastSavedData, &m_saveData, sizeof(FDSaveData) ) )
  {
    lLogBreak();
    lLogFmt("  Options data identical - not saving\n");
    lLogBreak();
    return true;
  }

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  CDataFileFlashROMWrite *optionsFile = FD_GAME_NEW CDataFileFlashROMWrite( OPTIONS_OFFSET );
  lAssert( optionsFile != NULL );

  if( !optionsFile->IsValid() )
  {
    delete optionsFile;
    lLogFmt("Error creating FlashRomWrite\n");
    return false;
  }

  // write options
  optionsFile->Write( &m_saveData, sizeof(m_saveData) );

  lLogFmt("Flushing write stream\n");
  optionsFile->Flush();

  delete optionsFile;

  memcpy( &s_lastSavedData, &m_saveData, sizeof(FDSaveData) );
#endif

  lLogFmt("Saved\n");
  return true;
}

bool FDMainMenuUI::OptionsExist()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

#ifdef L_PLATFORM_WINDOWS
  CDataFileLocalFile *saveFile = FD_GAME_NEW CDataFileLocalFile( "options.save", false );

  if( !saveFile->IsValid() )
  {
    lLogFmt("Couldn't open options\n");
    delete saveFile;
    return false;
  }

  delete saveFile;
  return true;
#else
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( OPTIONS_OFFSET ) )
  {
    return false;
  }
  return true;
#endif
}

void FDMainMenuUI::OptionsApply()
{
	// Applies the current options to the game.
  lLogBreak();
  lLogFmt("OptionsApply - %.2f %.2f\n", m_saveData.m_userPrefs.m_sfxVolume, m_saveData.m_userPrefs.m_crowdVolume);
	FDSound::SetSoundEffectVolume((int)(m_saveData.m_userPrefs.m_sfxVolume * 5.0f));
	FDSound::SetCrowdVolume((int)(m_saveData.m_userPrefs.m_crowdVolume * 5.0f));
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// EditMode load/save/exist check
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
bool FDMainMenuUI::EditModeLoad()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif


  lLogBreak();

  lLogFmt("Performing load!\n");
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

#ifdef L_PLATFORM_WINDOWS

  CDataFileLocalFile *editmodeFile = FD_GAME_NEW CDataFileLocalFile( "editmode.save", false );

  if( !editmodeFile->IsValid() )
  {
    lLogFmt("Couldn't open %s to read\n", "editmode.save");
    return false;
  }

#else                                   // #ifdef L_PLATFORM_WINDOWS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( EDITMODE_OFFSET ) )
  {
    lLogFmt("  Data doesn't exist!\n");
    return false;
  }

  // Load compressed data
  lMemoryIOStream memStream;
  char *ptr = NULL;
  int   ptrsize = 0;
  int ret = ReadData( EDITMODE_OFFSET, &ptr, &ptrsize );

  if( ret == READ_OK )
  {
    // create array stream and read with zipiostream
    lLogFmt("Read data %d!\n", ptrsize);
    memStream.Init( ptr, ptrsize, L_IO_ACCESS_READ );
  } else
  if( ret == READ_CORRUPT )
  {
    lLogFmt("  Corrupt editmode data\n");
    ErrorHandle( ERROR_ACCESS_CORRUPT );

    if( ptr != NULL )
    {
      lFree( ptr );
    }

    // now overwrite *all* the data
    OptionsDefault();
    OptionsWipe();
    EditModeDefault();
    EditModeWipe();
    GamedataWipe();
    return false;
  } else
  {
    lLogFmt("Error reading data\n");
    if( ptr != NULL )
    {
      lFree( ptr );
    }

    return false;
  }

  lMemoryIOStream *editmodeFile = &memStream;
#endif                                  // #ifdef L_PLATFORM_WINDOWS

  // load engine data
  if (editmodeFile->Read(&m_EditModeData, sizeof(m_EditModeData)) != sizeof(m_EditModeData))
  {
    lLogFmt("loading editmode failed!\n");
#ifdef L_PLATFORM_NDS
    if( ptr != NULL )
    {
      lFree( ptr );
    }
#else
    delete editmodeFile;
#endif
    return false;
  }

  lLogBreak();

#ifdef L_PLATFORM_NDS
  if( ptr != NULL )
  {
    lFree( ptr );
  }
#endif

  lLogFmt("Loaded\n");
  return true;
}

bool FDMainMenuUI::EditModeSave( FDLoadSaveCallback * saveCallback )
{
 #ifdef DISABLE_SAVE
  SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
  return false;
 #endif

  CardAccessHelper helper;
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

  lLogBreak();
  lLogFmt("Performing save!\n");

  if( saveCallback != NULL )
    saveCallback->SaveProgress( 0.0f );

#ifdef L_PLATFORM_WINDOWS
  CDataFileLocalFile *editmodeFile = FD_GAME_NEW CDataFileLocalFile( "editmode.save", true );

  if( !editmodeFile->IsValid() )
  {
    lLogFmt("Couldn't open %s to write\n", "editmode.save");
    delete editmodeFile;
    SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
    return false;
  }

  // load game data
  editmodeFile->Write( &m_EditModeData, sizeof(m_EditModeData) );

  delete editmodeFile;

  lLogFmt("Saved editmode\n");
  SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
  return true;
#else
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
    return false;
  }

  s_callback = saveCallback;
  s_callbackLast = 0;
  s_callbackTotal = 8000;

  CDataFileFlashROMWrite *editmodeFile = FD_GAME_NEW CDataFileFlashROMWrite( EDITMODE_OFFSET );
  lAssert( editmodeFile != NULL );

  if( !editmodeFile->IsValid() )
  {
    delete editmodeFile;
    lLogFmt("Error creating FlashRomWrite\n");
    SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
    return false;
  }

  // write editmode
  editmodeFile->Write( &m_EditModeData, sizeof(m_EditModeData) );

  lLogFmt("Flushing write stream\n");
  editmodeFile->Flush();

  delete editmodeFile;
#endif

  if( saveCallback != NULL )
    saveCallback->SaveProgress( 1.0f );

  lLogFmt("Saved\n");
  SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
  return true;
}

bool FDMainMenuUI::EditModeExist()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

#ifdef L_PLATFORM_WINDOWS
  CDataFileLocalFile *saveFile = FD_GAME_NEW CDataFileLocalFile( "editmode.save", false );

  if( !saveFile->IsValid() )
  {
    lLogFmt("Couldn't open editmode\n");
    delete saveFile;
    return false;
  }

  delete saveFile;
  return true;
#else
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( EDITMODE_OFFSET ) )
  {
    return false;
  }
  return true;
#endif
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// Game data load/save/exist check
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
bool FDMainMenuUI::GamedataLoad( FDLoadSaveCallback *loadCallback )
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  lLogBreak();

  lLogFmt("Performing load!\n");
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

#ifdef L_PLATFORM_WINDOWS

  char filename[256];

  if (FDCommandLineOptions::IsAutoPlay() == true)
  {
#ifdef AUTOPLAY_SPECIFIC_SAVEFILE_NAME
    // Write club / seed specific information
    if( FDCommandLineOptions::IsUsingRandomSeedFromCommandLine() )
    {
      sprintf(filename, "game-team%d-seed%x.save", FDCommandLineOptions::GetTeamToPlay(),FDCommandLineOptions::GetRandomSeedFromCommandLine());
    } else
    {
      sprintf(filename, "game-team%d.save", FDCommandLineOptions::GetTeamToPlay());
    }
#else
    // Write default filename
    strcpy( filename, "game.save" );
#endif
  } else
  {
    strcpy( filename, "game.save" );
  }

#ifndef WINDOWS_COMPRESSED
  CDataFileLocalFile *saveFile = FD_GAME_NEW CDataFileLocalFile( filename, false );
#else
  lFileIOStream *saveFileRaw = FD_GAME_NEW lFileIOStream( filename, L_IO_ACCESS_READ );
  CDataFileCompressed *saveFile = FD_GAME_NEW CDataFileCompressed( false, saveFileRaw );
#endif

#else                                   // #ifdef L_PLATFORM_WINDOWS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( SAVEGAME_OFFSET ) )
  {
    lLogFmt("  Data doesn't exist!\n");
    return false;
  }

  // Load compressed data
  lMemoryIOStream memStream;
  char *ptr = NULL;
  int   ptrsize = 0;
  int ret = ReadData( SAVEGAME_OFFSET, &ptr, &ptrsize );

  if( ret == READ_OK )
  {
    // create array stream and read with zipiostream
    lLogFmt("Read data %d!\n", ptrsize);
    memStream.Init( ptr, ptrsize, L_IO_ACCESS_READ );
  } else
  if( ret == READ_CORRUPT )
  {
    lLogFmt("  Corrupt gamedata data\n");
    ErrorHandle( ERROR_ACCESS_CORRUPT );

    if( ptr != NULL )
    {
      lFree( ptr );
    }

    // now overwrite *all* the data
    OptionsDefault();
    OptionsWipe();
    EditModeDefault();
    EditModeWipe();
    GamedataWipe();
    return false;
  } else
  {
    lLogFmt("Error reading data\n");
    if( ptr != NULL )
    {
      lFree( ptr );
    }

    return false;
  }

  CDataFileCompressed *saveFile = FD_GAME_NEW CDataFileCompressed( false, &memStream );
#endif                                  // #ifdef L_PLATFORM_WINDOWS

  if( !saveFile->IsValid() )
  {
    lLogFmt("Couldn't open %s to read\n", "game.save");
    delete saveFile;
#ifdef WINDOWS_COMPRESSED
    saveFileRaw->Close();
    delete saveFileRaw;
#endif
#ifdef L_PLATFORM_NDS
    if( ptr != NULL )
    {
      lFree( ptr );
    }
#endif

    return false;
  }

  // load engine data
  if( !WorldData().GetGameFromDisk( saveFile ) || !LoadEditModeData(saveFile) )
  {
    lLogFmt("GetGameFromDisk failed!\n");
    delete saveFile;
#ifdef WINDOWS_COMPRESSED
    saveFileRaw->Close();
    delete saveFileRaw;
#endif
#ifdef L_PLATFORM_NDS
    if( ptr != NULL )
    {
      lFree( ptr );
    }
#endif
    return false;
  }

  lLogBreak();

  delete saveFile;
#ifdef WINDOWS_COMPRESSED
	saveFileRaw->Close();
  delete saveFileRaw;
#endif

#ifdef L_PLATFORM_NDS
  if( ptr != NULL )
  {
    lFree( ptr );
  }
#endif

  lLogFmt("Loaded\n");
  return true;
}

bool FDMainMenuUI::GamedataWipe()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  lLogBreak();

  lLogFmt("Performing wipe!\n");

#ifdef L_PLATFORM_NDS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  BlankData( SAVEGAME_OFFSET );
#endif

  return true;
}

bool FDMainMenuUI::OptionsWipe()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  lLogBreak();

  lLogFmt("Performing wipe!\n");

#ifdef L_PLATFORM_NDS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  BlankData( OPTIONS_OFFSET );
#endif

  return true;
}

bool FDMainMenuUI::EditModeWipe()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  lLogBreak();

  lLogFmt("Performing wipe!\n");

#ifdef L_PLATFORM_NDS
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  BlankData( EDITMODE_OFFSET );
#endif

  return true;
}


#ifdef L_PLATFORM_NDS
#define SAVELOG OS_Printf
#else
#define SAVELOG lLogFmt
#endif


bool FDMainMenuUI::GamedataSave( FDLoadSaveCallback *saveCallback )
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

  m_bReadyToTick = false;

  CardAccessHelper helper;

  lLogBreak();

  SAVELOG("Performing save!\n");
  ForceLoadSaveHeapHelper loadsaveHeapHelper;

  if( saveCallback != NULL )
    saveCallback->SaveProgress( 0.0f );

#ifdef L_PLATFORM_WINDOWS

  char filename[256];

  if (FDCommandLineOptions::IsAutoPlay() == true)
  {
#ifdef AUTOPLAY_SPECIFIC_SAVEFILE_NAME
    // Write club / seed specific information
    if( FDCommandLineOptions::IsUsingRandomSeedFromCommandLine() )
    {
      sprintf(filename, "game-team%d-seed%x.save", FDCommandLineOptions::GetTeamToPlay(),FDCommandLineOptions::GetRandomSeedFromCommandLine());
    } else
    {
      sprintf(filename, "game-team%d.save", FDCommandLineOptions::GetTeamToPlay());
    }
#else
    // Write default filename
    strcpy( filename, "game.save" );
#endif
  } else
  {
    strcpy( filename, "game.save" );
  }

#ifndef WINDOWS_COMPRESSED
  CDataFileLocalFile *saveFile = FD_GAME_NEW CDataFileLocalFile( filename, true );
#else
  lFileIOStream *saveFileRaw = FD_GAME_NEW lFileIOStream( filename, L_IO_ACCESS_WRITE );
  CDataFileCompressed *saveFile = FD_GAME_NEW CDataFileCompressed( true, saveFileRaw );
#endif

#else

  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    SAVELOG("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    m_bReadyToTick = true;
    return false;
  }

  SAVELOG("Creating compressor\n");
  CDataFileFlashROMWrite *writeStream = FD_GAME_NEW CDataFileFlashROMWrite( SAVEGAME_OFFSET );
  lAssert( writeStream != NULL );

  if( !writeStream->IsValid() )
  {
    delete writeStream;
    SAVELOG("Error creating FlashRomWrite\n");
    m_bReadyToTick = true;
    return false;
  }

  CDataFileCompressed *saveFile = FD_GAME_NEW CDataFileCompressed( true, writeStream );
#endif

  saveFile->SetCallback( saveCallback, 600000 );

  if( !saveFile->IsValid() )
  {
    SAVELOG("Couldn't open %s to write\n", "test.save");
    delete saveFile;
#ifdef WINDOWS_COMPRESSED
    saveFileRaw->Close();
    delete saveFileRaw;
#endif
#ifdef L_PLATFORM_NDS
    delete writeStream;
#endif
    m_bReadyToTick = true;
    return false;
  }

  // save engine data
  if( !WorldData().SaveGameFile( saveFile ) || !SaveEditModeData(saveFile) )
  {
    SAVELOG("SaveGameFile failed!\n");
    delete saveFile;
#ifdef WINDOWS_COMPRESSED
    saveFileRaw->Close();
    delete saveFileRaw;
#endif
#ifdef L_PLATFORM_NDS
    delete writeStream;
#endif
    m_bReadyToTick = true;
    return false;
  }

#ifdef L_PLATFORM_NDS
  SAVELOG("Flushing write stream\n");
  writeStream->Flush();
#endif

  lLogBreak();
  delete saveFile;

#ifdef WINDOWS_COMPRESSED
  int maxSize = ROMSIZE - (sizeof(FDSaveData) + (48*2));
  lLogFmt("Compressed file = %d / %d\n", saveFileRaw->Tell(), maxSize);
  lAssertAlways( saveFileRaw->Tell() < maxSize );
  saveFileRaw->Close();
  delete saveFileRaw;
#endif

#ifdef L_PLATFORM_NDS
  delete writeStream;
#endif

  if( saveCallback != NULL )
    saveCallback->SaveProgress( 1.0f );

  SAVELOG("Saved\n");
  m_bReadyToTick = true;
  return true;
}

bool FDMainMenuUI::GamedataExist()
{
 #ifdef DISABLE_SAVE
  return false;
 #endif

#ifdef L_PLATFORM_WINDOWS
  CDataFileLocalFile *saveFile = FD_GAME_NEW CDataFileLocalFile( "game.save", false );

  if( !saveFile->IsValid() )
  {
    lLogFmt("Couldn't open save\n");
    delete saveFile;
    return false;
  }

  delete saveFile;
  return true;
#else
  StorageInit();

  // Check status of backup memory
  if( !StorageConfirmBackupMemory() )
  {
    lLogFmt("  Couldn't confirm backup memory\n");
    ErrorHandle( ERROR_ACCESS_FAIL );
    return false;
  }

  if( !DoesSaveExist( SAVEGAME_OFFSET ) )
  {
    lLogFmt("  Data doesn't exist!\n");
    return false;
  }
  return true;
#endif
}

static bool SaveEditModeData(CDataFile *saveFile)
{
	return EditModeData().SaveGameFile(saveFile);
}

static bool LoadEditModeData(CDataFile *saveFile)
{
	return EditModeData().LoadGameFile(saveFile);
}
