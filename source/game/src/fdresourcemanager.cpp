//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdresourcemanager.cpp
// Description : FDResourceManager implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdresourcemanager.h>
#include <lemon/object/lweakptr.h>
#include <framework/lspritebank.h>
#include <framework/lfont.h>
#include <framework/lsprite.h>
#include <lemon/io/lfileiostream.h>

using namespace L_NAMESPACE_NAME;

static lSharedPtr<lFont> s_font;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDResourceManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDResourceManager::Init()
{
  lLogSys("FDResourceManager","Init");

  // get font (non-streamed)
  s_font = GetFont("font12", "font12", false);
}

void FDResourceManager::Deinit()
{
  lLogSys("FDResourceManager","Deinit");

}

lSharedPtr<lFont> FDResourceManager::GetDefaultFont()
{
  return s_font;
}

static lArray< lSharedPtr<lFont> > s_fontList;
static lArray<unsigned int> s_fontHashes;

unsigned int font_name_hash(const char *fontId)
{
	const char *iter = fontId;
	const unsigned int b    = 378551;
	unsigned int a    = 63689;
	unsigned int hash = 0;

	while (*iter)
	{
		hash = hash * a + (*iter);
		a    = a * b;

		iter++;
	};

	return hash;
}

lSharedPtr<lFont> FDResourceManager::GetFont( const char * resourceId, const char * fontId, bool bStream )
{
  const unsigned int uiHash = font_name_hash(fontId);
  const int iFontIndex = s_fontHashes.Find(uiHash);

  if (iFontIndex >= 0)
	  return s_fontList(iFontIndex);

  lSharedPtr<lSpriteBank> fontBank = GetSpriteBank(resourceId, bStream);
  lSharedPtr<lFont>       font     = lSharedPtr<lFont>( FD_GAME_NEW lFont( fontBank, fontId ) );

  s_fontHashes.Add(uiHash, 1);
  s_fontList.Add(font, 1);

  return font;
}


static lArray< lWeakPtr<lSpriteBank> > s_spriteBankList;

static lString GetFilePrefix()
{
  lString filename;

  #if defined(L_PLATFORM_WINDOWS)
  if( lSpriteManager::IsEmulateDS() )
  {
    filename = lString("nds/");
  }
  else
  {
    filename = lString("pc/");
  }
  #endif

  return filename;
}



lSharedPtr<lSpriteBank> FDResourceManager::GetSpriteBank( const char * resourceIdIn, bool bStream )
{
  if( resourceIdIn == NULL || resourceIdIn[0] == '\0' )
  {
    return lSharedPtr<lSpriteBank>();
  }

  // HACK: DS doesn't do case in-sensitive strcmpi correctly.

  lString resourceId = resourceIdIn;
  resourceId.ToLower();

  // Find an existing bank.

  for( int i=0;i<s_spriteBankList.Size();i++ )
  {
    if( s_spriteBankList(i).GetSharedCount() == 0 )
    {
      lLogSys("FDResourceManager","Purging old bank at index %d\n", i);
      s_spriteBankList.Remove(i);
      i--;
      continue;
    }

    lSharedPtr<lSpriteBank> bank = s_spriteBankList(i).Lock();

    if( strcmpi( bank->m_name.CStr(), resourceId.CStr() ) == 0 )
    {
      //lLogSys("FDResourceManager","GetSpriteBank: Reusing '%s' (%d refs)\n", resourceId.CStr(), bank.GetSharedCount() );
      return bank;
    }
  }


  lString filename = GetFilePrefix();

  filename += lString(resourceId);
  filename += lString(".LSB");

  lSpriteBank * spriteBank = FD_GAME_NEW lSpriteBank( filename, bStream );

  lAssert( spriteBank != NULL );

  if( spriteBank )
  {
    // HACK: overwrite name. should have a naming convention or something.
    spriteBank->m_name = resourceId;

    //NOTE: This log causing the DS to crash when loading playerinfo.lsb, sprite - generic_player
    lLogSys("FDResourceManager","GetSpriteBank: Loaded '%s'\n", resourceId.CStr() );
    //lLogFmt("%s\n", spriteBank->GetSizeString().CStr());

    lSharedPtr<lSpriteBank> bank( spriteBank );
    s_spriteBankList.Push( lWeakPtr<lSpriteBank>( bank ) );

    return bank;
  }
  else
  {
    lLogSys("FDResourceManager","GetSpriteBank: Failed '%s'\n", resourceId.CStr() );
  }

  return lSharedPtr<lSpriteBank>();
}

lSharedPtr<lSprite> FDResourceManager::GetSprite( const char * bankId, const char * spriteId )
{
  lSharedPtr<lSpriteBank> bank = GetSpriteBank( bankId );
  return lSharedPtr<lSprite>( lSprite::Create( bank, spriteId ) );
}

lSharedPtr<lSprite> FDResourceManager::GetSpriteBG( const char * spriteId )
{
  lSharedPtr<lSpriteBank> bank = GetSpriteBank( spriteId );
  return lSharedPtr<lSprite>( lSprite::Create( bank, spriteId ) );
}


// Replace with pack file stream on appropriate platforms
class ResourceGameFile : public lFileIOStream
{
public:
  virtual ~ResourceGameFile()
  {
    if( IsValid() )
    {
      Close();
    }
  }
};


lIOStreamSeekable* FDResourceManager::GetFile( const char* fileId )
{
  lString filename = GetFilePrefix();

  filename += lString(fileId);

  ResourceGameFile * file = FD_GAME_NEW ResourceGameFile;
  file->Open( filename, L_IO_ACCESS_READ );

  if( !file->IsValid() )
  {
    lLogFmt("FDResourceManager::GetFile, File not found %s\n",filename.CStr() );
    delete file;
    return NULL;
  }

  return file;
}


