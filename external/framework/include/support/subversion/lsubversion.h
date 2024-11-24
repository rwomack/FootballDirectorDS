//����������������������������������������������������������������������������
// File        : lsubversion.h
// Description : Subversion interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSUBVERSION_H_INCLUDED
#define LSUBVERSION_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/platform/lcore.h>
#include <lemon/containers/lstring.h>
#include <lemon/containers/larray.h>
#include <lemon/xml/lxml.h>

#include <support/process/lprocess.h>

BEGIN_L_NAMESPACE

class lSubversionInfo;
class lSubversionLog;

//����������������������������������������������������������������������������
// lSubversion
//����������������������������������������������������������������������������

class lSubversion
{
public:
  lSubversion();
  ~lSubversion();

  bool GetInfo( const char * path, lSubversionInfo &info );
  bool GetLog( const char * path, lSubversionLog &log, int firstRevision, int lastRevision, int limit );
};

//����������������������������������������������������������������������������
// lSubversionInfo
//����������������������������������������������������������������������������

class lSubversionInfo
{
  friend class lSubversion;

  lString m_url;
  lString m_repos;
  int     m_headRevision;
  int     m_lastChangedRevision;
  lString m_lastChangedAuthor;

public:
  lSubversionInfo();
  ~lSubversionInfo();

  const char * GetURL();
  const char * GetRepos();
  int          GetHeadRevision();
  int          GetLastChangedRevision();
  const char * GetLastChangedAuthor();
};

//����������������������������������������������������������������������������
// lSubversionLogEntry
//����������������������������������������������������������������������������

struct lSubversionLogEntry
{
  int     revision;
  lString author;
  lString message;
};

//����������������������������������������������������������������������������
// lSubversionLog
//����������������������������������������������������������������������������

class lSubversionLog
{
  lArray<lSubversionLogEntry> m_logList;
  friend class lSubversion;

public:
  lSubversionLog();
  virtual ~lSubversionLog();

  int                  GetNumEntries();
  lSubversionLogEntry & GetEntry(int index);
};


END_L_NAMESPACE

#endif

