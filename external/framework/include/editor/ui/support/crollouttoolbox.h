//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : crollouttoolbox.h
// Description : CRolloutToolBox interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CROLLOUTTOOLBOX_H_INCLUDED
#define EDITOR_UI_SUPPORT_CROLLOUTTOOLBOX_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>
#include <lemon/containers/larray.h>

#include <editor/ui/support/crollout.h>
#include "../ui.h"

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CRolloutToolBox
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CRolloutToolBox : public lEditor::UIToolBox, public lEditor::UITool, public CWindowImpl<CRolloutToolBox>
{
public:

  // UITool interface

  CRolloutToolBox(int rolloutWidth, const char * caption);
  ~CRolloutToolBox();

  virtual HWND GetHWND();

  // UIToolBox interface

  virtual void AddTool( UITool * rollout, const char * caption );
  virtual void RemoveTool( UITool * rollout );

  // CWindowImpl interface

  DECLARE_WND_CLASS(NULL)

private:

  typedef CRolloutCtrl<CWindow> CUIRolloutCtrl;

  CRolloutContainerClient         m_rolloutContainer;
  Lemon::lArray<CUIRolloutCtrl *> m_rolloutListCreate;

  BOOL PreTranslateMessage(MSG* pMsg);

  BEGIN_MSG_MAP(CRolloutToolBox)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_CLOSE,  OnClose)
    MESSAGE_HANDLER(WM_SIZE,   OnSize)
	END_MSG_MAP()

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

}
#endif
