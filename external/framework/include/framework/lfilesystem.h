//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lfilesystem.h
// Description : lFileSystem
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.
//
//  TODO:
//
//
//  Note:
//


#ifndef _lfilesystem_
#define _lfilesystem_

#include <lemon/platform/lcore.h>
#include <lemon/io/liostream.h>
#include <lemon/io/lfileiostream.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>

BEGIN_L_NAMESPACE

enum SystemPriority {
  LFS_PACK,
  LFS_FILE,
};

class FileWrapper: public lFileIOStream
{
public:
  FileWrapper( const char * fileName, L_IO_ACCESS mode )
    : lFileIOStream( fileName, mode )
  {
  }

  ~FileWrapper()
  {
    Close();
  }
};

class lFileSystem
{
public:
  //open file by name from pack or filename if not wanting to use pak
  static lIOStreamSeekable * OpenFile( const char * fileName );


  //init - search for and load any paks
  // - priority , pack, file unless specified
  static void Init( char * gameDirectory );
  static void DeInit();

  static void LoadPack( const char * fileName );

  static void SetPriority(SystemPriority priority);

  /*template <class T>
  static T* GetFile( char * fileName );*/

private:
  static lIOStreamSeekable * OpenFileFromPack( const char * fileName );
};

END_L_NAMESPACE

#endif
