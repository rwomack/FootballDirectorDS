//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : liffarchive.h
// Description : IFF Archive
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIFFARCHIVEIMPL_H_INCLUDED
#define LIFFARCHIVEIMPL_H_INCLUDED

#include "lobjectapi.h"
#include "../io/liff.h"
#include "./liffarchive.h"

BEGIN_L_NAMESPACE

class LOBJECT_API lIFFNodeImpl;
class LOBJECT_API lIFFDataImpl;
class LOBJECT_API lIFFArchiveImpl;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFArchiveImpl
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lIFFArchiveImpl
{
  lIFFArchive::lIFFArchiveMode m_mode;
  lIFFContext     m_streamContext;
  friend class lIFFNodeImpl;

public:

  lIFFArchiveImpl( lIOStreamSeekable * iostream, lIFFArchive::lIFFArchiveMode mode );
  lIFFNodeImpl * Node(lIFFId id);

  bool     IsLoading();
  bool     IsSaving();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFDataImpl
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lIFFDataImpl
{
  lIFFNodeImpl * m_node;
  lIFFId         m_id;
  int            m_refCount;
  friend class   lIFFNodeImpl;

public:

  lIFFDataImpl();
  ~lIFFDataImpl();

  int            GetSharedCount();
  void           AddReference();
  void           RemoveReference();

  void           Serialize( void * data, size_t size );
  lIFFNodeImpl * Parent();

  bool           IsLoading();
  bool           IsSaving();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lIFFNodeImpl
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lIFFNodeImpl
{
  lIFFContext       m_context;
  lIFFContext     * m_parent;
  lIFFNodeImpl    * m_nodeParent;
  lIFFArchiveImpl * m_ar;
  int               m_refCount;

  friend class lIFFArchiveImpl;
  friend class lIFFDataImpl;

public:

  lIFFNodeImpl();
  ~lIFFNodeImpl();

  int      GetSharedCount();
  void     AddReference();
  void     RemoveReference();

  lIFFDataImpl * Data(lIFFId id);
  lIFFNodeImpl * Node(lIFFId id);
  lIFFNodeImpl * Parent();
  lIFFId         Id();

  bool           IsLoading();
  bool           IsSaving();
};

END_L_NAMESPACE
#endif
