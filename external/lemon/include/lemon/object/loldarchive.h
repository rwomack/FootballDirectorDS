//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : loldarchive.h
// Description : lOldArchive class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LOLDARCHIVE_H_INCLUDED
#define LOLDARCHIVE_H_INCLUDED

#include "lobjectapi.h"
#include "../io/lstream.h"
#include "../containers/larray.h"

BEGIN_L_NAMESPACE

// object array trait
typedef TArrayTrait<3,8,16,L_ARENA_OBJECT> TObjectArrayTrait;  // 3/8 + 16

class LOBJECT_API lIOStream;
class LOBJECT_API lObject;
class LOBJECT_API lClass;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lOldArchive
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_OLDARCHIVE_VERSION 0

class LOBJECT_API lOldArchive : public lStream
{
public:

  enum lArchiveMode
  {
    save,
    load
  };

  lOldArchive( lIOStream *iostream, lArchiveMode mode );
  ~lOldArchive();

  // lStream interface

  virtual void      Serialize( void *data, unsigned int size );
  virtual lObject * SerializeObject( lObject * );
  virtual bool      IsLoading(void);
  virtual bool      IsSaving(void);

  // lOldArchive interface

  void              SerializeObjectInto( lObject * );
  unsigned int      Version(void);

  int               ClassVersion( lClass * );
  void              SetAllowReferences( bool b );

protected:
  lIOStream         *m_stream;
  lArchiveMode       m_mode;

  // lOldArchive implementation

  void              SerializeHeader(void);
  void              ReadHeader(void);
  void              WriteHeader(void);

  void              WriteClass( lClass * );
  lClass        *   ReadClass( void );

  void              WriteObject( lObject * );
  lObject *         ReadObject( lClass *, lObject * = NULL );

  unsigned int      GetVersion(void);
  void              SetVersion(unsigned int);

  bool               m_allowRefs;
  unsigned int       m_archiveVersion;
  unsigned int       m_classVersion;
  lArray<lObject*,TObjectArrayTrait>   m_objectList;

  struct ClassVersionInfo
  {
    lClass        * objectClass;
    unsigned int    version;
  };

  lArray<ClassVersionInfo,TObjectArrayTrait> m_versionStack;
};

END_L_NAMESPACE
#endif
