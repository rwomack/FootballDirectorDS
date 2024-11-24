//����������������������������������������������������������������������������
// File        : limagepacker.h
// Description : Image packing routines
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIMAGEPACKER_H_INCLUDED
#define LIMAGEPACKER_H_INCLUDED

#include "./limageapi.h"
#include "./limage.h"     // for lIMAGE_TYPE
#include "../containers/larray.h"

BEGIN_L_NAMESPACE

class lImage;

class  lImagePackerParams;
class  lImagePackerResult;
class  lImagePackerSource;

struct lImagePackerPageInfo;
struct lImagePackerImageInfo;
struct lImagePackerCellInfo;

//����������������������������������������������������������������������������
// lImagePacker
//����������������������������������������������������������������������������

class LIMAGE_API lImagePacker
{
  struct Image
  {
    int sourceIndex;
    int sourceX;
    int sourceY;
    int sourceWidth;
    int sourceHeight;
    int sourceFlags;
  };

  lArray<lImagePackerSource *> m_sourceList;
  lArray<Image>                m_imageList;

public:
  lImagePacker();
  ~lImagePacker();

  int AddSource( lImagePackerSource * );

  enum
  {
    IMAGE_FLAGS_DONTSHAREPAGE = 0x001,
  };


  int AddImageGrid( int flags, int sourceIndex, int gridCellWidth, int gridCellHeight, int &outNumImages );
  int AddSubImageGrid( int flags, int sourceIndex, int x, int y, int width, int height, int gridCellWidth, int gridCellHeight, int &outNumImages );

  int AddImage( int flags, int sourceIndex );
  int AddSubImage( int flags, int sourceIndex, int x, int y, int width, int height );

  bool Pack( lImagePackerResult &, const lImagePackerParams & );

  bool                 GetImageInfo( int imageIndex, int &sourceIndex, int &x, int &y, int &width, int &height );
  lImagePackerSource * GetSource( int index );
};

//����������������������������������������������������������������������������
// lImagePackerSource
//����������������������������������������������������������������������������

class LIMAGE_API lImagePackerSource
{
public:
  virtual ~lImagePackerSource();

  virtual int         GetWidth() const = 0;
  virtual int         GetHeight() const = 0;
  virtual lIMAGE_TYPE GetType() const = 0;

  virtual lImage    * Prepare() = 0;
  virtual void        Dispose( lImage * ) = 0;
};

//����������������������������������������������������������������������������
// lImagePackerParams
//����������������������������������������������������������������������������

class LIMAGE_API lImagePackerParams
{
public:
  lImagePackerParams()
    : flags(0),
      maxPageWidth(0),
      maxPageHeight(0),
      maxCellWidth(0),
      maxCellHeight(0),
      border(0)
  {
  }

  lImagePackerParams( int _flags, int w, int h )
    : flags(_flags),
      maxPageWidth(w),
      maxPageHeight(h),
      maxCellWidth(maxPageWidth),
      maxCellHeight(maxPageHeight),
      border(0)
  {
  }

  enum
  {
    REMOVE_CELL_EMPTY_SPACE = 0x001,
  };

  int flags;
  int maxPageWidth;
  int maxPageHeight;
  int maxCellWidth;
  int maxCellHeight;
  int border;
};

//����������������������������������������������������������������������������
// lImagePackerResult
//����������������������������������������������������������������������������

class LIMAGE_API lImagePackerResult
{
  friend class lImagePacker;

  lArray<lImagePackerImageInfo> m_imageList;
  lArray<lImagePackerCellInfo>  m_cellList;
  lArray<lImagePackerPageInfo>  m_pageList;

public:

  lImagePackerResult();
  ~lImagePackerResult();

  void Clear();

  int  GetNumPages() const;
  int  GetNumImages() const;
  int  GetNumCells() const;

  bool GetPage( int index, lImagePackerPageInfo & ) const;
  bool GetImage( int index, lImagePackerImageInfo & ) const;
  bool GetCell( int index, lImagePackerCellInfo & ) const;
};


//����������������������������������������������������������������������������
// lImagePackerImageInfo
//����������������������������������������������������������������������������

struct LIMAGE_API lImagePackerImageInfo
{
  int numCells;
  int cellIndex;
  int width;
  int height;
};

//����������������������������������������������������������������������������
// lImagePackerCellInfo
//����������������������������������������������������������������������������

struct LIMAGE_API lImagePackerCellInfo
{
  int offsetX;
  int offsetY;
  int pageIndex;
  int pageX;
  int pageY;
  int width;
  int height;
};

//����������������������������������������������������������������������������
// lImagePackerPageInfo
//����������������������������������������������������������������������������

struct LIMAGE_API lImagePackerPageInfo
{
  int      pixelsTotal;
  int      pixelsUsed;
  lImage * image;
};

//����������������������������������������������������������������������������
// lImagePackerMemorySource
//����������������������������������������������������������������������������

class LIMAGE_API lImagePackerMemorySource : public lImagePackerSource
{
  lImage * m_image;

public:
  lImagePackerMemorySource( lImage * );
  virtual ~lImagePackerMemorySource();

  // lImagePackerSource interface

  virtual int         GetWidth() const;
  virtual int         GetHeight() const;
  virtual lIMAGE_TYPE GetType() const;

  virtual lImage    * Prepare();
  virtual void        Dispose( lImage * );
};

//����������������������������������������������������������������������������
// lImagePackerFileSource
//����������������������������������������������������������������������������

class LIMAGE_API lImagePackerFileSource : public lImagePackerSource
{
  lImage * m_image;

public:
  lImagePackerFileSource( const char * fileName );
  virtual ~lImagePackerFileSource();

  bool                IsValid() const;

  // lImagePackerSource interface

  virtual int         GetWidth() const;
  virtual int         GetHeight() const;
  virtual lIMAGE_TYPE GetType() const;

  virtual lImage    * Prepare();
  virtual void        Dispose( lImage * );
};

END_L_NAMESPACE
#endif
