//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lStorageEventHandler.h
// Description : Common game event handling
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSTORAGEEVENTHANDLER_H_INCLUDED
#define LSTORAGEEVENTHANDLER_H_INCLUDED

#include <framework/lsigslot.h>
#include <framework/ldialog.h>

BEGIN_L_NAMESPACE

struct lDialog;
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lStorageEventHandler
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lStorageEventHandler : public lHasSlots<>
{
  public:

  lStorageEventHandler();
  ~lStorageEventHandler();
  
  private:
  
  void SaveLoadComplete();
  void DialogOpen(lDialog*);
  void DialogClose(lDialog*);
  
};



END_L_NAMESPACE

#endif

