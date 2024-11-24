//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lspritebank.h
// Description : lSpriteBank
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LSPRITEBANK_H
#define __LSPRITEBANK_H

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/render/lrtextureresource.h>  // for lrTextureResrouce

BEGIN_L_NAMESPACE

// Pre-declared classes

struct lIFFContext;
class lIOStreamSeekable;
class lFileIOStream;
class lImage;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSpriteBank
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_IFF_LSPB L_IFFID('L','S','P','B')

#define L_SPRITE_BANK_VERSION (2)

enum
{
  L_SPRITE_BANK_PLATFORM_NONE = 0,
  L_SPRITE_BANK_PLATFORM_PC   = 0,
  L_SPRITE_BANK_PLATFORM_DS   = 1,
  L_SPRITE_BANK_PLATFORM_WII  = 2,
  L_SPRITE_BANK_PLATFORM_PS2  = 3,
  L_SPRITE_BANK_PLATFORM_PSP  = 4
};

struct lSpriteBankHeader
{
  luint32_t version;
  luint32_t platform;
  luint32_t flags;
  luint32_t nameTableSize;
  luint32_t numSprites;
  luint32_t numFrames;
  luint32_t numCells;
  luint32_t numHotSpots;
  luint32_t numPages;
  luint32_t pageDataSize;
  luint32_t collisionDataSize;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    version        = lEndian::Little32( version       );
    platform       = lEndian::Little32( platform      );
    flags          = lEndian::Little32( flags         );
    nameTableSize  = lEndian::Little32( nameTableSize );
    numSprites     = lEndian::Little32( numSprites    );
    numFrames      = lEndian::Little32( numFrames     );
    numCells       = lEndian::Little32( numCells      );
    numHotSpots    = lEndian::Little32( numHotSpots   );
    numPages       = lEndian::Little32( numPages      );
    pageDataSize   = lEndian::Little32( pageDataSize  );
    collisionDataSize = lEndian::Little32( collisionDataSize  );
#endif
  }
};

struct lSpriteBankCellV1
{
  luint32_t pageIndex;
  luint32_t offsetX;
  luint32_t offsetY;
  luint32_t x,y,w,h;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    pageIndex = lEndian::Little32( pageIndex );
    offsetX   = lEndian::Little32( offsetX   );
    offsetY   = lEndian::Little32( offsetY   );
    x         = lEndian::Little32( x         );
    y         = lEndian::Little32( y         );
    w         = lEndian::Little32( w         );
    h         = lEndian::Little32( h         );
#endif
  }
};

struct lSpriteBankCell
{
  luint16_t pageIndex;
  luint16_t offsetX;
  luint16_t offsetY;
  luint16_t x,y,w,h;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    pageIndex = lEndian::Little16( pageIndex );
    offsetX   = lEndian::Little16( offsetX   );
    offsetY   = lEndian::Little16( offsetY   );
    x         = lEndian::Little16( x         );
    y         = lEndian::Little16( y         );
    w         = lEndian::Little16( w         );
    h         = lEndian::Little16( h         );
#endif
  }
};

struct lSpriteBankFrameV1
{
  luint32_t numCells;
  luint32_t cellIndex;
  luint32_t width;
  luint32_t height;
  luint32_t offsetx;
  luint32_t offsety;
  luint32_t collisionIndex;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    numCells  = lEndian::Little32( numCells  );
    cellIndex = lEndian::Little32( cellIndex );
    width     = lEndian::Little32( width     );
    height    = lEndian::Little32( height    );
    offsetx   = lEndian::Little32( offsetx   );
    offsety   = lEndian::Little32( offsety   );
    collisionIndex       = lEndian::Little32( collisionIndex  );
#endif
  }

};

struct lSpriteBankFrame
{
  luint16_t numCells;
  luint32_t cellIndex;
  luint16_t width;
  luint16_t height;
  luint16_t offsetx;
  luint16_t offsety;
  luint16_t totalw;
  luint16_t totalh;
  luint32_t collisionIndex;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    numCells  = lEndian::Little16( numCells  );
    cellIndex = lEndian::Little32( cellIndex );
    width     = lEndian::Little16( width     );
    height    = lEndian::Little16( height    );
    offsetx   = lEndian::Little16( offsetx   );
    offsety   = lEndian::Little16( offsety   );
    totalw    = lEndian::Little16( totalw    );
    totalh    = lEndian::Little16( totalh    );
    collisionIndex       = lEndian::Little32( collisionIndex  );
#endif
  }

};


struct lSpriteBankSprite
{
  luint32_t nameIndex;
  luint16_t numFrames;
  luint16_t width;
  luint16_t height;
  luint16_t frameIndex;
  luint32_t hotSpotIndex;
  luint32_t numHotSpots;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    nameIndex     = lEndian::Little32( nameIndex    );
    numFrames     = lEndian::Little16( numFrames    );
    width         = lEndian::Little16( width        );
    height        = lEndian::Little16( height       );
    frameIndex    = lEndian::Little16( frameIndex   );
    hotSpotIndex  = lEndian::Little32( hotSpotIndex );
    numHotSpots   = lEndian::Little32( numHotSpots  );
#endif
  }
};

struct lSpriteBankHotSpot
{
  luint32_t nameIndex;
  luint16_t x;
  luint16_t y;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    nameIndex = lEndian::Little32( nameIndex );
    x         = lEndian::Little16( x );
    y         = lEndian::Little16( y );
#endif
  }
};

struct lSpriteBankPage
{
  luint16_t type;
  luint16_t width;
  luint16_t height;
  luint16_t flags;
  luint32_t size;
  luint32_t dataIndex;

  void EndianConvert()
  {
#ifdef L_PLATFORM_WII
    type       = lEndian::Little16( type );
    width      = lEndian::Little16( width );
    height     = lEndian::Little16( height );
    flags      = lEndian::Little16( flags );
    size       = lEndian::Little32( size );
    dataIndex  = lEndian::Little32( dataIndex );
#endif
  }
};

class lSpriteBankArrayTrait
{
public:
  lSpriteBankArrayTrait() {}
  ~lSpriteBankArrayTrait() { }

  template<typename T> T * Reallocate(T * existingPtr, unsigned int newSize, unsigned int oldSize )
  {
    return (T*) Allocator( (void *)existingPtr, newSize*sizeof(T), oldSize*sizeof(T) );
  }

  static unsigned int CalcReserveSize( unsigned int size )
  {
    return size;
  }

private:
  void * Allocator(void * existingPtr, unsigned int newSize, unsigned int oldSize );
};

// TODO: sort name indices so we can perform a binary search for sprite names.
//

class lSpriteBank
{
public:

  lString                                           m_name;
  lSpriteBankHeader                                 m_hdr;

  lArray<char, lSpriteBankArrayTrait>               m_nameTable;
  lArray<lSpriteBankSprite, lSpriteBankArrayTrait>  m_spriteList;
  lArray<lSpriteBankFrame, lSpriteBankArrayTrait>   m_frameList;
  lArray<lSpriteBankCell, lSpriteBankArrayTrait>    m_cellList;
  lArray<lSpriteBankPage, lSpriteBankArrayTrait>    m_pageList;
  lArray<lSpriteBankHotSpot, lSpriteBankArrayTrait> m_hotSpotList;
  lArray<char, lSpriteBankArrayTrait>               m_pageData;
  lArray<luint8_t, lSpriteBankArrayTrait>           m_collisionData;

  mutable lArray<luint32_t>                         m_pageCacheIdList;

  int                        m_pageDataIndex;
  bool                       m_bStream;

  void LoadIFF( lIFFContext * ctx );

  lSpriteBank();
  lSpriteBank( const char * fileName, bool bStream = true );
  lSpriteBank( lIOStreamSeekable * );
  ~lSpriteBank();

  const char * GetName() const;

  const lSpriteBankSprite  * FindSprite( const char * name ) const;
  const lSpriteBankFrame   * GetFrame( int index ) const;
  const lSpriteBankCell    * GetCell( int index ) const;
  const lSpriteBankPage    * GetPage( int index ) const;
  const void               * GetPageData( int index ) const;
  const luint32_t            GetPageCacheId( int index ) const;
  const void                 SetPageCacheId( int index, luint32_t ) const;
  const lSpriteBankHotSpot * GetHotSpot( int index ) const;
  const luint8_t           * GetFrameCollisionData( int index ) const;
  const luint32_t            GetNumPages() const;
  int                        GetTotalSize() const;
  lString                    GetSizeString() const;

  #ifdef L_PLATFORM_WINDOWS
  void ConvertToPC();
  #endif

  #if defined(L_PLATFORM_NDS)
  mutable FSFile                    m_DSFile;
  #endif
};

END_L_NAMESPACE
#endif

