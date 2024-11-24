//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lstorage.h
// Description : Storage include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

// TODO: need error returning system
// detect changes - unplugged etc
// asynchronous?
//
// System to handle TRCs etc?
// i.e. what to do with formatted memory cards, text to show
// max number of files in a subdirectory, file name specifics

// Storage device - memory card, hard drive
// Storage location - location which contains files / directories

// Provide a way to find mapping between an input device and a location

// Location id should be
// kkpppp-nnnnnxxxxxxxx (k = key code (BI - SCEI, BA - SCEA, BE - SCEE), p/n = product code (SLPS-nnnnn, SLUS-nnnnn, SLES-nnnnn),
//                       x = arbitary eight char string)
// i.e. BESLES-12345xxxxxxxx

#ifndef LSTORAGE_H_INCLUDED
#define LSTORAGE_H_INCLUDED

#include "../platform/lcorefile.h"
#include "../containers/lstring.h"
#include "../containers/larray.h"

#include "lstorageapi.h"

BEGIN_L_NAMESPACE

#ifdef L_PLATFORM_PS2

  #include <libvu0.h>
  #include <sifrpc.h>
  #include <sifdev.h>
  #include <libpad.h>
  #include <libmc.h>
  #include <libmtap.h>

#endif

//##
// Storage data to be provided for a location
typedef int   _iconVu0IVECTOR[4];
typedef float	_iconVu0FVECTOR[4];

struct LSTORAGE_API lStorageLocationData
{
 #ifdef L_PLATFORM_PS2
  lStorageLocationData() : viewIconFile(NULL), copyIconFile(NULL), delIconFile(NULL)
  {
    // initialise all the values to defaults
    for(int i = 0; i < 4; i++)
    {
      // Set light source directions and colours
      lightDir1[i] = 0.0f;    lightDir2[i] = 0.0f;    lightDir3[i] = 0.0f;
      lightColour1[i] = 0.0f; lightColour2[i] = 0.0f; lightColour3[i] = 0.0f;
      ambient[i] = 1.0f;
    }

    // initialise background
    backgroundTransparency = 0x80;
    for(int i = 0; i < 4; i++)
    {
      backgroundUL[i] = 0x80;
      backgroundUR[i] = 0x80;
      backgroundLL[i] = 0x80;
      backgroundLR[i] = 0x80;
    }

    viewIconFileSize = 0;
    copyIconFileSize = 0;
    delIconFileSize = 0;
    flags = 0;
  }

  void *viewIconFile;
  int   viewIconFileSize;
  void *copyIconFile;
  int   copyIconFileSize;
  void *delIconFile;
  int   delIconFileSize;

  // background transparency
  uint32        backgroundTransparency;

  // background colour (Upper/Lower, Left/Right)
  _iconVu0IVECTOR backgroundUL;
  _iconVu0IVECTOR backgroundUR;
  _iconVu0IVECTOR backgroundLL;
  _iconVu0IVECTOR backgroundLR;

  // light source direction
  _iconVu0FVECTOR lightDir1;
  _iconVu0FVECTOR lightDir2;
  _iconVu0FVECTOR lightDir3;

  // light source colour
  _iconVu0FVECTOR lightColour1;
  _iconVu0FVECTOR lightColour2;
  _iconVu0FVECTOR lightColour3;

  // ambient colour
  _iconVu0FVECTOR ambient;
 #else

 lStorageLocationData()
 {
   flags = 0;
 }

 #endif

  enum
  {
    LSTORAGE_LOCATION_FLAG_USESINGLEICON = 0x000001,  // use the icon specified by listIconFile for all the icon data

  };

  int flags;
};

//##
// Status information on a storage device
//
struct LSTORAGE_API lStorageDeviceStatus
{
  bool  m_bActive;

  bool  m_bError;
  bool  m_bFormatted;
  int   m_availableSpaceBytes;

 #ifdef L_PLATFORM_PS2
  enum
  {
    DEVICE_TYPE_PS1,
    DEVICE_TYPE_PS2,
    DEVICE_TYPE_POCKETSTATION,
    DEVICE_TYPE_UNKNOWN,
  };

  int   m_type;
  int   m_port;
  int   m_slot;
 #else


 #endif
};

//##
//  Storage system access class
struct LSTORAGE_API lStorage
{
  bool      Init();
  void      Deinit();

  //##
  // Update the status of all the cards currently inserted
  // Indexes will stay valid until update status is called
  void      UpdateStatus();

  //##
  // Returns true if there has been a change and update status needs to be called
  bool      StatusChange();

  //##
  // Get information on storage devices
  enum DeviceType
  {
    MemoryCard,
    HardDrive,
    MemoryStick,
    Unknown,
  };

  int               GetNumDevices();
  lString           GetDeviceName( int i );
  enum DeviceType   GetDeviceType( int i );

  static const char *      GetDeviceTypeName( enum DeviceType type )
  {
    switch( type )
    {
      case MemoryCard:  return "memory card (PS2)";
      case HardDrive:   return "hard drive";
      case MemoryStick: return "memory stick";
      default:          return "unknown";
    }

#ifdef L_COMPILER_MSVC
 #pragma warning(push)
 #pragma warning(disable:4702)
#endif
    return "";
#ifdef L_COMPILER_MSVC
 #pragma warning(pop)
#endif
  }

  //##
  // Set the current location which we're working with
  int       GetCurrentDeviceIdx();
  void      SetCurrentDeviceIdx( int idx );

  //##
  // Format / Unformat the device
  bool      Format( bool bFormat );     // false makes it unformatted

  //##
  // Return status of this device
  lStorageDeviceStatus GetDeviceStatus();

  bool CurrentDeviceStillValid();

  int       GetSaveLocationSize( lStorageLocationData *locationData, lArray<int> &fileSizeList );

  //##
  // Check if we have enough free space for our save location and the files in it
  bool      CheckSaveLocation( const char *locationID, const char *locationName, lStorageLocationData *locationData, lArray<int> &fileSizeList );

  //##
  // Check if a save location already exists and is valid
  bool      DoesExistSaveLocation( const char *locationID );

  //##
  // Prepare save location for this storage device
  // Must open a save location to be able to perform any operations below, operations will now relative to this location
  // NULL for saveData should only be used when we arent creating the data, i.e. if we'll never have to create
  // the fresh directory structure (at least on ps2)
  bool      OpenSaveLocation( const char *locationID, const char *locationName, lStorageLocationData *locationData, bool bCreateIfNoExist, bool bOpenForWrite );

   //##
  // Close a save location, this performs any final operations to make the save valid if it has just been created
  bool      CloseSaveLocation();

   //##
  // Delete a save location, this must be specified outside of any open save location
  bool      DeleteSaveLocation( const char *locationID );

  //##
  // Load a file
  bool      LoadFile( const char *fileName, void *data, int numBytes, int *readBytes = NULL, bool bChecksum = true );

  //##
  // Save a file
  bool      SaveFile( const char *fileName, void *data, int numBytes, bool bChecksum = true );

  //##
  // Get a file size
  // Returns -1 if unsuccessful, else returns size in bytes of file
  int       GetFileSize( const char *fileName );

  //##
  // Delete a file
  bool      RemoveFile( const char *fileName );

 private:

  enum
  {
    OPEN_ACCESS_READ    = 0x0001,
    OPEN_ACCESS_WRITE   = 0x0002,
    OPEN_ACCESS_CREATE  = 0x0200,
  };

  bool    Open( const char *fileName, int access );
  bool    Close();

  int     Read( void *data, int size );
  int     Write( void *data, int size );

  bool    Delete( const char *fileName );

  // tracking current device / location
  int         m_currentIdx;
  bool        m_bLocationOpen;
  char        m_locationID[256];
  bool        m_bStatusChange;

  // platform specific stuff

#ifdef L_PLATFORM_PS2
  #define MAX_STORAGE 16
  lStorageDeviceStatus m_storageInfo[MAX_STORAGE];
  int                  m_numStorage;

  int                  m_curFileHandle;

  int                  m_numFilesInDir;
  sceMcTblGetDir       m_dirList[ 32 ] __attribute__((aligned((64))));  // limit is really 18 files per directory
#else
  lFILE_HANDLE        m_curFileHandle;
#endif

  bool            MakeDirectory( const char *dirName );

  // ps2 specific functions
#ifdef L_PLATFORM_PS2
  int  WaitOnAsync( int *cmd, int *result );

  enum
  {
    LSTORAGEPS2_GETDIR_ERRCODE_OK = 1,
    LSTORAGEPS2_GETDIR_ERRCODE_UNFORMATTED,
    LSTORAGEPS2_GETDIR_ERRCODE_PATHDOESNTEXIST,
    LSTORAGEPS2_GETDIR_ERRCODE_NOMEMCARD,
  };

  bool            GetDirectoryList( const char *pathName, int &errCode );
  int             GetDirectoryListCount();
  char          * GetDirectoryListName( int idx );
  int             GetDirectoryListSize( int idx );

  bool            ChangeDirectory( const char *dirName );
#endif
};

END_L_NAMESPACE

#endif
