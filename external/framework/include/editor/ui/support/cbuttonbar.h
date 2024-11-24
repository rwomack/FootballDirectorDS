//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : cbuttonbar.h
// Description : CButtonBar interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CBUTTONBAR_H_INCLUDED
#define EDITOR_UI_SUPPORT_CBUTTONBAR_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>
#include <lemon/containers/larray.h>
#include "../ui.h"

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CButtonBar
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CButtonBar : public lEditor::UIButtonBar, public CWindowImpl<CButtonBar>
{
public:

  CButtonBar(bool bSmall = false, bool bClipWidth = true, bool bTransparent = false );
  ~CButtonBar();

  int  GetButtonSize();
  int  GetBarHeight();
  int  GetBarClippedWidth();
  bool GetSmall();
  void SetSmall(bool);
  bool GetTransparent();
  void SetTransparent(bool);
  bool GetClipWidth();
  void SetClipWidth(bool);

  lEditor::UIButtonGroup * CreateButtonGroup( bool insert = false );
  void                     DestroyButtonGroup( lEditor::UIButtonGroup * );

  // CWindowImpl

  DECLARE_WND_CLASS(NULL)

private:

  // CButtonBar implementation

  class Group;                                  // private group interface
  class Button;                                 // private button interface

  Lemon::lArray<Button *> m_buttonList;
  Lemon::lArray<Group *>  m_buttonGroupList;
  bool                    m_bSmall;
  int                     m_buttonSize;         // full size of buttons
  bool                    m_bClipWidth;         // clip the bar to the actual width need for the buttons
  bool                    m_bTransparent;       // use a transparent background
  int                     m_barClippedWidth;    // actual clipped width of the bar

  lEditor::UIButton * CreateButton( Group * , const char * name, const char * help, const char * image, int order );
  void                DestroyButton( lEditor::UIButton * );
  void                FormatButtons();

  enum { ID_BUTTON_FIRST = 100, ID_BUTTON_LAST = 200 };

  BEGIN_MSG_MAP(CButtonBar)
    MESSAGE_HANDLER(WM_CREATE,     OnCreate)
    MESSAGE_HANDLER(WM_SIZE,       OnSize)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    COMMAND_RANGE_HANDLER( ID_BUTTON_FIRST, ID_BUTTON_LAST, OnButton );
  END_MSG_MAP()

  LRESULT OnButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

}
#endif
