//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : limage.h
// Description : Image class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIMAGE_H_INCLUDED
#define LIMAGE_H_INCLUDED

#include "limageapi.h"

BEGIN_L_NAMESPACE

struct lIFFContext;
class lIOStreamSeekable;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIMAGE_TYPE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lIMAGE_TYPE
{
  L_IMAGE_UNKNOWN         =  0x00,   // unknown type
  L_IMAGE_PAL_1           =  0x01,   // 1 bit palette (8 bytes ARGB)
  L_IMAGE_PAL_4           =  0x02,   // 4 bit palette (64 bytes ARGB)
  L_IMAGE_PAL_8           =  0x03,   // 8 bit palette (1024 bytes ARGB)
  L_IMAGE_RGB_565         =  0x04,
  L_IMAGE_BGR_565         =  0x05,
  L_IMAGE_RGBX_4444       =  0x06,
  L_IMAGE_BGRX_4444       =  0x07,
  L_IMAGE_RGBA_4444       =  0x08,
  L_IMAGE_BGRA_4444       =  0x09,
  L_IMAGE_XRGB_4444       =  0x0a,
  L_IMAGE_XBGR_4444       =  0x0b,
  L_IMAGE_ARGB_4444       =  0x0c,
  L_IMAGE_ABGR_4444       =  0x0d,
  L_IMAGE_RGBX_5551       =  0x0e,
  L_IMAGE_BGRX_5551       =  0x0f,
  L_IMAGE_RGBA_5551       =  0x10,
  L_IMAGE_BGRA_5551       =  0x11,
  L_IMAGE_XRGB_1555       =  0x12,
  L_IMAGE_XBGR_1555       =  0x13,
  L_IMAGE_ARGB_1555       =  0x14,
  L_IMAGE_ABGR_1555       =  0x15,
  L_IMAGE_RGB_888         =  0x16,
  L_IMAGE_BGR_888         =  0x17,
  L_IMAGE_RGBX_8888       =  0x18,
  L_IMAGE_BGRX_8888       =  0x19,
  L_IMAGE_RGBA_8888       =  0x1a,
  L_IMAGE_BGRA_8888       =  0x1b,
  L_IMAGE_XRGB_8888       =  0x1c,
  L_IMAGE_XBGR_8888       =  0x1d,
  L_IMAGE_ARGB_8888       =  0x1e,
  L_IMAGE_ABGR_8888       =  0x1f,
  L_IMAGE_A_8             =  0x20,
  L_IMAGE_Z_16            =  0x21,
  L_IMAGE_Z_32            =  0x22,
  L_IMAGE_COMP_DXT1       =  0x23,
  L_IMAGE_COMP_DXT2       =  0x24,
  L_IMAGE_COMP_DXT3       =  0x25,
  L_IMAGE_COMP_DXT4       =  0x26,
  L_IMAGE_COMP_DXT5       =  0x27,
  L_IMAGE_UV_88           =  0x28,
  L_IMAGE_VU_88           =  0x29,
  L_IMAGE_Z_24_S_8        =  0x2a,
  L_IMAGE_RGBE_8888       =  0x2b,
  L_IMAGE_MAX             =  0x2b
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCHANNEL_TYPE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lCHANNEL_TYPE
{
  L_CHANNEL_UNKNOWN       = 0x00,   // unknown channel type
  L_CHANNEL_X             = 0x01,   // unused channel
  L_CHANNEL_PAL           = 0x02,
  L_CHANNEL_R             = 0x03,
  L_CHANNEL_G             = 0x04,
  L_CHANNEL_B             = 0x05,
  L_CHANNEL_A             = 0x06,
  L_CHANNEL_U             = 0x07,
  L_CHANNEL_V             = 0x08,
  L_CHANNEL_W             = 0x09,
  L_CHANNEL_Z             = 0x0A,
  L_CHANNEL_STENCIL       = 0x0B,
  L_CHANNEL_LUMINANCE     = 0x0C,
  L_CHANNEL_EXPONENT      = 0x0D, // exponent for other of channels
  L_CHANNEL_MAX           = 0x0D
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCHANNEL_DATA
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lCHANNEL_DATA
{
  L_CHANNEL_DATA_UNKNOWN  = 0x0,    // unknown data type
  L_CHANNEL_DATA_UINT     = 0x01,
  L_CHANNEL_DATA_SINT     = 0x02,
  L_CHANNEL_DATA_FLOAT    = 0x03,
  L_CHANNEL_DATA_MAX      = 0x03
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIMAGE_FLAG
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lIMAGE_FLAG
{
  L_IMAGE_FLAG_COMPRESSED = 0x01,
  L_IMAGE_FLAG_SWIZZLED   = 0x02,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIMAGE_FLIP_FLAG
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lIMAGE_FLIP_FLAG
{
  L_IMAGE_FLIP_NONE                = 0x00,
  L_IMAGE_FLIP_VERTICAL            = 0x01,  // flip top-to-bottom
  L_IMAGE_FLIP_HORIZONTAL          = 0x02,  // flip left-to-right
  L_IMAGE_FLIP_VERTICAL_HORIZONTAL = 0x03,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lImageChannelMap
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LIMAGE_API lImageChannelMap
{
  lCHANNEL_TYPE channelMap[L_CHANNEL_MAX+1];     // map for destination to source channels
  float         channelDefault[L_CHANNEL_MAX+1]; // default values channels

  lImageChannelMap();
  ~lImageChannelMap();
  void SetIdentity();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lFILTER_TYPE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lFILTER_TYPE
{
  L_FILTER_POINT        = 0x0,
  L_FILTER_BOX          = 0x01,
  L_FILTER_TRIANGLE     = 0x02,
  L_FILTER_CUBIC        = 0x03,
  L_FILTER_QUADRATIC    = 0x04,
  L_FILTER_CATROM       = 0x05,
  L_FILTER_GAUSSIAN     = 0x06,
  L_FILTER_SINC         = 0x07,
  L_FILTER_BELL         = 0x08,
  L_FILTER_BESSEL       = 0x09,
  L_FILTER_LANCZOS      = 0x0A,
  L_FILTER_MITCHELL     = 0x0B,
  L_FILTER_MAX          = 0x0B,
  L_FILTER_DEFAULT      = L_FILTER_TRIANGLE
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lWINDOW_TYPE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lWINDOW_TYPE
{
  L_WINDOW_NONE         = 0x0,
  L_WINDOW_HANNING      = 0x01,
  L_WINDOW_HAMMING      = 0x02,
  L_WINDOW_SINC         = 0x03,
  L_WINDOW_BLACKMAN     = 0x04,
  L_WINDOW_KAISER       = 0x05,
  L_WINDOW_MAX          = 0x05,
  L_WINDOW_DEFAULT      = L_WINDOW_NONE
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lDITHER_TYPE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lDITHER_TYPE
{
  L_DITHER_NONE            = 0x0,
  L_DITHER_FLOYD_STEINBERG = 0x01,
  L_DITHER_MAX             = 0x01,
  L_DITHER_DEFAULT         = L_DITHER_FLOYD_STEINBERG
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lImageLevelInfo
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lImageLevelInfo
{
  void       * data;
  unsigned int dataSize;
  void       * table;
  unsigned int tableSize;
  lIMAGE_TYPE  type;
  unsigned int width;
  unsigned int height;
  unsigned int pitch;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lImageIFFInfo
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lImageIFFInfo
{
  unsigned int version;
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  unsigned int numMipMaps;
  unsigned int type;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Import/export function types
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lImage;
typedef lImage * (*lImageImportFunction)( lIOStreamSeekable *, const char * );
typedef bool     (*lImageExportFunction)( const lImage *, lIOStreamSeekable *, const char * );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lImage
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LIMAGE_API lImage
{
  // image format, 8-bits (an lIMAGE_TYPE)
  // mipmaps, 4 bits (number of mip maps)
  // usize,   4 bits (log2 of size)
  // vsize,   4 bits (log2 of size)
  // psize,   4 bits (log2 of size)
  // 8 bits spare for future use: to store cubemap flag, number of dimensions etc.
  unsigned int  m_format[4];
  void        * m_data;
public:

  lImage();
  lImage( const lImage & );
  lImage( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type );

  ~lImage();

  lImage &operator=(const lImage &);

  void           SetHeader( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type, void * data );
  void           Reset();

  // image information

  lIMAGE_TYPE    Type() const;
  unsigned int   NumLevels() const;
  unsigned int   Width() const;
  unsigned int   Height() const;
  unsigned int   Depth() const;

  // type information

  const char   * TypeName() const;
  unsigned int   BitsPerPixel() const;
  unsigned int   ChannelBits(lCHANNEL_TYPE) const;
  unsigned int   ChannelShift(lCHANNEL_TYPE) const;
  lCHANNEL_DATA  ChannelData(lCHANNEL_TYPE) const;
  unsigned int   Flags() const;

  // data access interface

  void         * XX_GetData() const;
  unsigned int   XX_GetDataSize() const;

  void         * GetTable() const;
  unsigned int   GetTableSize() const;

  bool           GetLevel( unsigned int level, lImageLevelInfo & ) const;
  void         * GetLevelData( unsigned int level ) const;
  unsigned int   GetLevelDataSize( unsigned int level ) const;

  // image manipulation operations

  lImage       * Copy() const;
  lImage       * Convert( unsigned int width, unsigned int height, unsigned int numMipMaps, lIMAGE_TYPE type, lFILTER_TYPE filterType, lWINDOW_TYPE windowType, lDITHER_TYPE ditherType, const lImageChannelMap * channelMap ) const;
  lImage       * Convert( lIMAGE_TYPE type ) const;
  lImage       * Convert( lIMAGE_TYPE type, const lImageChannelMap * channelMap ) const;
  bool           FilterLevels( unsigned int firstLevel, lFILTER_TYPE filterType, lWINDOW_TYPE windowType, lDITHER_TYPE ditherType ) const;
  void           Flip( lIMAGE_FLIP_FLAG );
  void           Clear();

  // import/export

  static bool RegisterImportFunction( lImageImportFunction importFunc, const char * description, const char * extensionList );
  static bool RemoveImportFunction( lImageImportFunction importFunc );

  static bool RegisterExportFunction( lImageExportFunction exportFunc, const char * description, const char * extensionList );
  static bool RemoveExportFunction( lImageExportFunction exportFunc );

  static lImage * Import( const char * filename );
  static lImage * Import( lIOStreamSeekable *, const char * filename );
  bool            Export( const char * filename ) const;
  bool            Export( lIOStreamSeekable * stream, const char * fileName ) const;

  // load / save to IFF

  static lImage * LoadIFF( lIFFContext *ctxt );
  void            SaveIFF( lIFFContext *ctxt ) const;

  // IFF peek

  static bool     PeekIFF( lIFFContext *ctxt, lImageIFFInfo &info );
  static bool     PeekIFF( lIOStreamSeekable *, lImageIFFInfo &info );
  static bool     PeekIFF( const char * fileName, lImageIFFInfo &info );

  // static pixel information/manipulation interface

  // type information

  static const char   * TypeName(lIMAGE_TYPE);
  static lIMAGE_TYPE    TypeId(const char *str);

  static const char   * ChannelTypeName(lCHANNEL_TYPE);
  static lCHANNEL_TYPE  ChannelTypeId(const char *str);

  static const char   * ChannelDataName(lCHANNEL_DATA);
  static lCHANNEL_DATA  ChannelDataId(const char *str);

  static unsigned int   BitsPerPixel(lIMAGE_TYPE);
  static unsigned int   ChannelBits(lIMAGE_TYPE, lCHANNEL_TYPE);
  static unsigned int   ChannelShift(lIMAGE_TYPE, lCHANNEL_TYPE);
  static lCHANNEL_DATA  ChannelData(lIMAGE_TYPE, lCHANNEL_TYPE);
  static unsigned int   Flags(lIMAGE_TYPE);

  static const char *   FilterTypeName(lFILTER_TYPE);
  static lFILTER_TYPE   FilterTypeId(const char *str);

  static const char *   WindowTypeName(lWINDOW_TYPE);
  static lWINDOW_TYPE   WindowTypeId(const char *str);

  static const char *   DitherTypeName(lDITHER_TYPE);
  static lDITHER_TYPE   DitherTypeId(const char *str);

  // pixel filter/convert

  static void FilterConvert( void * dstData, unsigned int dstWidth, unsigned int dstHeight, unsigned int dstPitch,
                             void * dstTable, unsigned int dstTableSize, lIMAGE_TYPE dstType,
                             void * srcData, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcPitch,
                             void * srcTable, unsigned int srcTableSize, lIMAGE_TYPE srcType,
                             lFILTER_TYPE filterType, lWINDOW_TYPE windowType, lDITHER_TYPE ditherType, bool bExistingPalette,
                             const lImageChannelMap * channelMap );

  // temp

  bool Compressed() { return( (Flags()&L_IMAGE_FLAG_COMPRESSED) == L_IMAGE_FLAG_COMPRESSED ); }
};


END_L_NAMESPACE
#endif
