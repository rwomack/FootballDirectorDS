//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcsv.h
// Description : CSV parser
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#ifndef LCSV_H_INCLUDED
#define LCSV_H_INCLUDED

#include <lemon/platform/lcore.h>
#include <lemon/platform/lcorefile.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>
#include <lemon/io/lfileiostream.h>

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCSV
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lCSV
{
public:

  lCSV();
  ~lCSV();

  int                   Load( lIOStreamSeekable* stream );
  const char*           GetError( int errorCode );
  int                   GetNumCols();
  int                   GetNumRows();
  const char*           GetCell( int row, int col );
  void                  SetCell( int row, int col, const char * text );

  void                  RemoveRow( int row );
  void                  RemoveColumn( int col );

  void                  SetDelimiter(char c);
  void                  SetIgnoreQuotes( bool b );

private:

  lArray<lString>       m_cellList;
  int                   m_numCols;
  int                   m_numRows;
  char                  m_delimiter;
  bool                  m_bIgnoreQuotes;
};

END_L_NAMESPACE

#endif
