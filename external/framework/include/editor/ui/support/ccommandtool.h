//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ccommandtool.h
// Description : CCommandTool interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CCOMMANDTOOL_H_INCLUDED
#define EDITOR_UI_SUPPORT_CCOMMANDTOOL_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>
#include <lemon/containers/larray.h>

#include "../ui.h"

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CCommandTool - Command tool
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CCommandTool : public lEditor::UITool, public CWindowImpl<CCommandTool>
{
public:

  // UITool interface

  CCommandTool();
  ~CCommandTool();

  virtual HWND GetHWND();

  // CCommandTool

  void Print( const char * );
  Lemon::lSignal1<const char *> SignalCommand;

  // CWindowImpl

  DECLARE_WND_CLASS_EX(NULL,0,COLOR_BTNFACE)

private:

  class LogWindow
  {
  public:
    CEdit m_edit;
    int   m_windowCurPos;

    LogWindow();
    void Reset();
    void Print(const char * str);
  };

  class InputEdit : public CWindowImpl<InputEdit,CEdit>
  {
  public:
    BEGIN_MSG_MAP(InputEdit)
      MESSAGE_HANDLER(WM_KEYDOWN,       PassMessage )
      MESSAGE_HANDLER(WM_CHAR,          PassMessage )
    END_MSG_MAP()

    LRESULT PassCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT PassMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  };

  BOOL PreTranslateMessage(MSG* pMsg);;

  enum { ID_INPUT = 1, ID_RESET = 2 };

  BEGIN_MSG_MAP(CUserScriptWindow)
    MESSAGE_HANDLER(WM_PAINT,  OnPaint)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_CLOSE,  OnClose)
    MESSAGE_HANDLER(WM_SIZE,   OnSize)

    COMMAND_ID_HANDLER( ID_INPUT, OnInput )
    COMMAND_ID_HANDLER( ID_RESET, OnReset )
    MESSAGE_HANDLER(WM_KEYDOWN,   OnKeyDown )
  END_MSG_MAP()

  LRESULT OnInput(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  Lemon::lArray<Lemon::lString> m_inputHistory;
  int                           m_currentHistory;
  int                           m_currentComplete;
  LogWindow                     m_output;
  InputEdit                     m_input;
  CButton                       m_reset;
};





}
#endif
