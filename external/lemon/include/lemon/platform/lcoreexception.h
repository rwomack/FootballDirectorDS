//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// File        : lcoreexception.h
// Description : Base exception class
// Notes       :
//
//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCOREEXCEPTION_H_INCLUDED
#define LCOREEXCEPTION_H_INCLUDED

#include "lcore.h"

#ifdef __cplusplus
BEGIN_L_NAMESPACE

//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// lException
//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ

class lException
{
public:
  virtual ~lException() {}
  virtual const char * GetErrorString() = 0;
};

//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// lFatalException
//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ

class lFatalException
{
public:
  lFatalException() {}
  virtual ~lFatalException() {}
  virtual const char * GetErrorString() = 0;

  static void Throw( char *, ... ) {}
};

END_L_NAMESPACE
#endif /*__cplusplus*/

#endif

