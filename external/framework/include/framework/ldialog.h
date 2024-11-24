//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ldialog.h
// Description : Dialog box
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LDIALOG_H_INCLUDED
#define LDIALOG_H_INCLUDED

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

class InputObject;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lDialog
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
enum
{
  L_DIALOG_PRIORITY_INFO = 0,
  L_DIALOG_PRIORITY_QUESTION,
  L_DIALOG_PRIORITY_WARNING,
  L_DIALOG_PRIORITY_ERROR,
  L_DIALOG_PRIORITY_CRITICAL_ERROR,
};

struct lDialog
{
  lString m_title;
  lString m_body;
  lArray<lString> m_optionList;
  int m_currentOption;
  int m_priority;
  int m_titleScriptId;
  int m_bodyScriptId;
  lArray<int> m_optionScriptIdList;

  lDialog()
  {
    m_title = lString("");
    m_body = lString("");
    m_optionList.Empty();
    m_currentOption = 0;
    m_priority = L_DIALOG_PRIORITY_INFO;
    m_titleScriptId = -1;
    m_bodyScriptId = -1;
    m_optionScriptIdList.Empty();
  }

  lDialog( lString title, lString body, int priority = L_DIALOG_PRIORITY_INFO)
  {
    m_title = title;
    m_body = body;
    m_optionList.Empty();
    m_currentOption = 0;
    m_priority = priority;
    m_titleScriptId = -1;
    m_bodyScriptId = -1;
    m_optionScriptIdList.Empty();
  }

  lDialog( lString title, lString body, const lArray<lString> options, int priority = L_DIALOG_PRIORITY_INFO)
  {
    m_title = title;
    m_body = body;
    m_optionList = options;
    m_currentOption = 0;
    m_priority = priority;
    m_titleScriptId = -1;
    m_bodyScriptId = -1;
    m_optionScriptIdList.Empty();
  }
  
  lDialog( int titleScriptId, int bodyScriptId, int priority = L_DIALOG_PRIORITY_INFO )
  {
    m_title = lString("");
    m_body = lString("");
    m_optionList.Empty();
    m_titleScriptId = titleScriptId;
    m_bodyScriptId = bodyScriptId;
    m_optionScriptIdList.Empty();
    m_currentOption = 0;
    m_priority = priority;
  }
  
  lDialog( int titleScriptId, int bodyScriptId, const lArray<int> options, int priority = L_DIALOG_PRIORITY_INFO )
  {
    m_title = lString("");
    m_body = lString("");
    m_optionList.Empty();
    m_titleScriptId = titleScriptId;
    m_bodyScriptId = bodyScriptId;
    m_optionScriptIdList = options;
    m_currentOption = 0;
    m_priority = priority;
  }
};



//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lDialogManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lDialogManager
{
  public:

  static void Init( InputObject* optionUp, InputObject* optionDown, InputObject* action );

  static void Open( lDialog* dialog );
  static void Close( lDialog* dialog );

  static bool IsOpen();
  static bool IsOpen( lDialog* dialog );
  static bool IsActive( lDialog* dialog );
  
  static lDialog* GetActiveDialog();

  static void Update();

  static void Render();

  //Signals
  static lSignal1<lDialog*> DialogOpenSignal;
  static lSignal1<lDialog*> DialogCloseSignal;
  static lSignal1<lDialog*> DialogOptionChangeSignal;
  static lSignal1<lDialog*> DialogOnScreenSignal;
};



END_L_NAMESPACE

#endif

