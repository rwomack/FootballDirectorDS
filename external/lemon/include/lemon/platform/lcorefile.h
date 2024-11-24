//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcorefile.h
// Description : File system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

//##LibRef_Platform_File
//<TITLE File I/O routines>
//
//Win32:
//  On the Win32 platform the file system will use memory mapped files
//  when the L_IO_ACCESS mode specified to lPlatform_FileOpen is L_IO_ACCESS_READ.
//  Memory mapped files offer a speed advantage over normal buffered reads.
//  They will not be used for write access, or read/write access files.
//
//Xbox:
//  If a path is provided to the file system on the the Xbox with no drive
//  the file system will automatically insert '<i>d:\\</i>' infront of the path.
//  All '/' are also translated into '\\'.
//
//PS2:
//  If a path is provided to the file system on the the PlayStation2 with no
//  device the file system will automatically insert '<i>host0:</i>'
//  infront of the path.
//  All '\\' are also translated into '/'.
//
//  The PlayStation2 does not fully support all file system functions.
//  See <Link LibRef_Platform_File_PS2_Notsupported, here> for a full list
//  of unsupported functions.

//##LibRef_Platform_File_PS2_Notsupported
//<GROUP LibRef_Platform_File>
//<TITLE Unsupported PS2 Functions>
// The follow functions are not supported on the PlayStation2
// * lPlatform_FileFindFirst
// * lPlatform_FileFindNext
// * lPlatform_FileFindClose
// * lPlatform_DirectoryCreate
// * lPlatform_DirectoryRemove

//##LibRef_Platform_File_PS2_Specific
//<GROUP LibRef_Platform_File>
//<TITLE PS2 Specific Functions>
// The follow functions are PlayStation2 specific
// * lPlatform_PS2_FileSetDefaultDevice
// * lPlatform_PS2_FileGetDefaultDevice

//##LibRef_Platform_File_PSP_Specific
//<GROUP LibRef_Platform_File>
//<TITLE PSP Specific Functions>
// The follow functions are PlayStation Portable specific
// * lPlatform_PSP_FileSetDefaultDevice
// * lPlatform_PSP_FileGetDefaultDevice

#ifndef LCOREFILE_H_INCLUDED
#define LCOREFILE_H_INCLUDED

#include "lcore.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File System
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

//##
// Seek position enumerations.
// Remarks: Seek position enumerations for use with lPlatform_FileSeek.
// These values match the stdc values for SEEK_SET, SEEK_CUR and SEEK_END.
//
enum L_IO_SEEK
{
  L_IO_SEEK_SET =  0,    //## Seek from the start of the stream
  L_IO_SEEK_CUR =  1,    //## Seek from the current position in the stream
  L_IO_SEEK_END =  2     //## Seek from the end of the stream
};

//##
// Stream access mode
// Remarks: Access mode enumations for use with lPlatform_FileOpen.
// Note that L_IO_ACCESS_READWRITE is simply a 'binary or' of
// L_IO_ACCESS_READ and L_IO_ACCESS_WRITE.
//
enum L_IO_ACCESS
{
  L_IO_ACCESS_READ      = 0x01,  //## Request read access
  L_IO_ACCESS_WRITE     = 0x02,  //## Request write access
  L_IO_ACCESS_READWRITE = 0x03   //## Request read and write access
};


//##
// Stream open mode
// Remarks: Open mode enumation for use with lPlatform_FileOpen.
// <p>If L_IO_OPEN_DEFAULT is used the open mode will be set based
// on the access mode according to the following table:
//<TABLE>
//  Access Mode              Open Mode                 Notes
//  -----------------------  ------------------------  ------------------------------------------------------
//  L_IO_ACCESS_READ         L_IO_OPEN_EXISTING        Only open for reading if file exists
//  L_IO_ACCESS_WRITE        L_IO_OPEN_CREATE_ALWAYS   Always overwrite when opening for writing
//  L_IO_ACCESS_READWRITE    L_IO_OPEN_ALWAYS          Open existing or create when opening for read/write
//</TABLE>
//
enum L_IO_OPEN
{
  L_IO_OPEN_DEFAULT        = 0x00,   //## Default mode (see remarks)
  L_IO_OPEN_CREATE_NEW     = 0x01,   //## Create a new file, if the file already exists then fail.
  L_IO_OPEN_CREATE_ALWAYS  = 0x02,   //## Always create a new file, existing file will be overwritten if it already exists.
  L_IO_OPEN_EXISTING       = 0x03,   //## Opens an existing file, it it doesn't exist then fail.
  L_IO_OPEN_ALWAYS         = 0x04,   //## Always open the file, if it doesn't exist then create it.
};

enum
{
  L_FILE_MAX_PATH = 255  //## Maximum path length
};


//##
// Describes the attribute of a file
//
enum L_FILE_ATTRIB
{
  L_FILE_ATTRIB_NONE       = 0x00,         //## No attribute
  L_FILE_ATTRIB_COMPRESSED = 0x01,         //## File is compressed
  L_FILE_ATTRIB_DIRECTORY  = 0x02          //## File is a directory
};

//##
// Describes the status of a file
// Remarks: File status set by lPlatform_FileStatus
//
typedef struct lFILE_STATUS_tag
{
  enum L_FILE_ATTRIB  attrib;                   //## Attribute of file
  time_t              creationTime;             //## Time when file was created
  time_t              lastAccessTime;           //## Time when file was last accessed
  time_t              lastModificationTime;			//## Time when the file as last modified
  unsigned int        size;                     //## Size in bytes of the file

} lFILE_STATUS;

//##
// Describes the results of a file find
// Remarks: File find data set by lPlatform_FileFindFirst and lPlatform_FileFindNext.
//
typedef struct lFILE_FIND_DATA_tag
{
  enum L_FILE_ATTRIB  attrib;                       //## File attribute
  time_t              creationTime;                 //## Time when file was created
  time_t              lastAccessTime;               //## Time when file was last accessed
  time_t              lastModificationTime;         //## Time when the file as last modified
  unsigned int        size;                         //## Size in bytes of the file
  char                name[ L_FILE_MAX_PATH ];      //## Name of the file

} lFILE_FIND_DATA;


//##
// File find handle
// Remarks: An opaque data handle returned by lPlatform_FileFindFirst and
// used by the file find functions.
//
// See Also: lPlatform_FileFindFirst
//
typedef void * lFILE_FIND_HANDLE;

//##
// Invalid file find handle
// Remarks: This value represents an invalid file find handle
//
// See Also: lFILE_FIND_HANDLE, lPlatform_FileFindFirst
//
#define lINVALID_FILE_FIND_HANDLE (0)

//##
// File handle
// Remarks: An opaque file handle returned by lPlatform_FileOpen and used by
// the file manipulation functions.
//
// See Also: lPlatform_FileOpen
//
typedef void * lFILE_HANDLE;

//##
// Invalid file handle
// Remarks: This value represents an invalid file handle
//
// See Also: lFILE_HANDLE,lPlatform_FileOpen
//
#define lINVALID_FILE_HANDLE (0)

//##
// Check if a file exists in the file system
// In:
//   fileName - name of file
// Out:
//   Returns true if file exists, otherwise flase
//
LPLATFORM_API bool lPlatform_FileExists(const char *fileName);

//##
// Return a valid file path for the specified filename on this platform
// In:
//   buf      - buffer which will have the valid file path put in it, if NULL is passed in the function will return the size of buffer needed
//   size     - size of the buffer
//   fileName - name of file
//
// Out:
//   Returns number of bytes written if successful or the number of bytes needed to write the full path if unsuccessful or buf was NULL
//
// Note:
//   Passing in a buf of size L_FILE_MAX_PATH is recommended
LPLATFORM_API luint32_t lPlatform_FileGetValidPath(char *buf, luint32_t size, const char *fileName);

//##
// Get status of a file
// In:
//   fileName - file name
//   fs       - lFILE_STATUS structure
// Out:
//   Returns true if the call succeeded, otherwise false
//
// PS2: <color red>Not supported</color>
LPLATFORM_API bool lPlatform_FileStatus(const char *fileName, lFILE_STATUS *fs);

#ifdef __cplusplus
//##lPlatform_FileOpen
// Open or create a file
// In:
//   fileName   - name of file to open or create
//   accessMode - file access mode
//   openMode   - file open/create mode
// Out:
//   Returns a lFILE_HANDLE for the opened file, or lINVALID_FILE_HANDLE
//   if the open failed.
//
LPLATFORM_API lFILE_HANDLE lPlatform_FileOpen(const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode = L_IO_OPEN_DEFAULT);
#else
LPLATFORM_API lFILE_HANDLE lPlatform_FileOpen(const char *fileName, enum L_IO_ACCESS accessMode, enum L_IO_OPEN openMode);
#endif

//##lPlatform_FileGetAccessMode
// Get the access mode of a file
// In:
//   hnd - file handle
// Out:
//   Returns the L_IO_ACCESS mode the file was opened with.
//   Or zero if hnd is lINVALID_FILE_HANDLE.
//
LPLATFORM_API L_IO_ACCESS lPlatform_FileGetAccessMode(lFILE_HANDLE hnd);

//##lPlatform_FileGetOpenMode
// Get the open mode of a file
// In:
//   hnd - file handle
// Out:
//   Returns the L_IO_OPEN mode the file was opened with.
//   Or zero if hnd is lINVALID_FILE_HANDLE.
//
LPLATFORM_API L_IO_OPEN lPlatform_FileGetOpenMode(lFILE_HANDLE hnd);

//##
// Close a file
// In:
//   hnd - file handle
//
// Out:
//   NONE
//
LPLATFORM_API void lPlatform_FileClose(lFILE_HANDLE hnd);

//##
// Read bytes from a file
// In:
//   hnd - file handle
//   buffer - destination buffer for bytes
//   size - numbers of bytes to read
//
// Out:
//   Returns the number of bytes actually read
//
LPLATFORM_API unsigned int lPlatform_FileRead(lFILE_HANDLE hnd,void *buffer,unsigned int size);

//##
// Write bytes to a file
// In:
//   hnd - file handle
//   buffer - source buffer
//   size - number of bytes to write
//
// Out:
//   Returns the numbqer of bytes actually written
//
LPLATFORM_API unsigned int lPlatform_FileWrite(lFILE_HANDLE hnd,const void *buffer,unsigned int size);

//##
// Seek to a position in a file
// In:
//   hnd - file handle
//   offset - offset
//   whence - where to seek from (L_IO_SEEK_xxx)
//
// Out:
//   Returns the resulting file position from the start of the file
//
LPLATFORM_API unsigned int lPlatform_FileSeek(lFILE_HANDLE hnd,int offset,enum L_IO_SEEK whence);

//##
// Get the current seek position of a file
// In:
//   hnd - file handle
//
// Out:
//   Returns the current file position from the start of the file
//
LPLATFORM_API unsigned int lPlatform_FileTell(lFILE_HANDLE hnd);

//##
// Get the length of a file
// In:
//   hnd - file handle
//
// Out:
//   Returns the length of a file
//
LPLATFORM_API unsigned int lPlatform_FileLength(lFILE_HANDLE hnd);

//##
// Find first occurance of a file based on name
// In:
//   path - path to search for
//   ff   - lFILE_FIND_DATA structure
//
// Out:
//   Returns a lFILE_FIND_HANDLE for subsequent calls to
//   lPlatform_FileFindNext or lPlatform_FileFindClose, or
//   lINVALID_FILE_FIND_HANDLE if the find failed.
//
// PS2: <color red>Not supported</color>
LPLATFORM_API lFILE_FIND_HANDLE lPlatform_FileFindFirst( const char *path, lFILE_FIND_DATA *ff );

//##
// Find next occurance of a file search
//
// In:
//   ffhnd - handle returned from FileFindFirst
//   ff    - lFILE_FIND_DATA structure
//
// Out:
//   Returns true if another file was found, otherwise false. The handle
//   must always be closed even if this function fails.
//
// PS2: <color red>Not supported</color>
LPLATFORM_API bool lPlatform_FileFindNext( lFILE_FIND_HANDLE ffhnd, lFILE_FIND_DATA * ff );

//##
// Close a file find
//
// In:
//   ffhnd -  handle returned from FileFindFirst
//
// Out:
//   NONE
//
// PS2: <color red>Not supported</color>
LPLATFORM_API void lPlatform_FileFindClose( lFILE_FIND_HANDLE ffhnd );

//##
// Create a directory
// In:
//   directoryName - name of directory
//
// Out:
//   Returns true if the directory was created
//
// PS2: <color red>Not supported</color>
LPLATFORM_API bool lPlatform_DirectoryCreate( const char * directoryName );

//##
// Remove a directory
// In:
//   directoryName - name of directory
//
// Out:
//   Returns true if the directory was removed
//
// PS2: <color red>Not supported</color>
LPLATFORM_API bool lPlatform_DirectoryRemove( const char * directoryName );

//##
// Check if a directory exists
// In:
//   directoryName - name of directory
//
// Out:
//   Returns true if the directory exists + is a directory, false otherwise
//
// PS2: <color red>Not supported</color>
LPLATFORM_API bool lPlatform_DirectoryExists( const char * directoryName );

// Playstation2 specific

#ifdef L_PLATFORM_PS2

//##
// Set the default file device name
// In:
//  deviceName - name of the device.
//
// Remarks:
// Sets the default device name. The device name is prefixed to
// file names that do not specify a device. (cdrom0:filename for example).
// <p>If the file name already has a device name the default device
// name is not prefixed.
// <p>A ':' is automatically added to <b>deviceName</b> if one isn't
// present at the end of the string.
// <p>The string is clipped to 8 characters not including the terminating
// NULL and ':'.
//
// See Also: lPlatform_PS2_FileGetDefaultDevice
//
// PS2: <color red>PS2 Specific</color>

LPLATFORM_API void lPlatform_PS2_FileSetDefaultDevice(const char * deviceName);

//##
// Get the default file device name
// Remarks:
// Gets the default device name. The device name is prefixed to
// file names that do not specify a device. (cdrom0:filename for example).
// See lPlatform_PS2_FileSetDefaultDevice for more details.
//
// Out:
//  Returns a pointer to a null terminated buffer containing the default
//  device name. <b>This buffer should not be modified</b>.
//
// See Also: lPlatform_PS2_FileSetDefaultDevice
//
// PS2: <color red>PS2 Specific</color>

LPLATFORM_API const char * lPlatform_PS2_FileGetDefaultDevice();

// Temp PS2 interfaces

LPLATFORM_API void lPlatformPS2_Temp_LoadIRX( const char *, void *params = NULL, int paramSize = 0 );
LPLATFORM_API bool lPlatformPS2_Temp_GetBootHost();
LPLATFORM_API bool lPlatformPS2_Temp_GetIOPReset();
LPLATFORM_API void lPlatformPS2_Temp_SetBootHost(bool);
LPLATFORM_API void lPlatformPS2_Temp_SetIOPReset(bool);

#endif

// Playstation Portable specific

#ifdef L_PLATFORM_PSP

//##
// Set the default file device name
// In:
//  deviceName - name of the device.
//
// Remarks:
// Sets the default device name. The device name is prefixed to
// file names that do not specify a device. (cdrom0:filename for example).
// <p>If the file name already has a device name the default device
// name is not prefixed.
// <p>A ':' is automatically added to <b>deviceName</b> if one isn't
// present at the end of the string.
// <p>The string is clipped to 8 characters not including the terminating
// NULL and ':'.
//
// See Also: lPlatform_PSP_FileGetDefaultDevice
//
// PSP: <color red>PSP Specific</color>

LPLATFORM_API void lPlatform_PSP_FileSetDefaultDevice(const char * deviceName);

//##
// Get the default file device name
// Remarks:
// Gets the default device name. The device name is prefixed to
// file names that do not specify a device. (cdrom0:filename for example).
// See lPlatform_PSP_FileSetDefaultDevice for more details.
//
// Out:
//  Returns a pointer to a null terminated buffer containing the default
//  device name. <b>This buffer should not be modified</b>.
//
// See Also: lPlatform_PSP_FileSetDefaultDevice
//
// PSP: <color red>PSP Specific</color>

LPLATFORM_API const char * lPlatform_PSP_FileGetDefaultDevice();

#endif


#ifdef __cplusplus
}
#endif

#endif

