//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdscreen.cpp
// Description : FDScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <framework/lsprite.h>
#include <fdscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDScreen::FDScreen( FDMainMenuUI *parent ) : m_parent(parent), m_bHidden(false), m_pModalOK(NULL), m_pModalVerifyFunc(NULL), m_bForcedUpdateRequested(false)
{
}

FDScreen::~FDScreen()
{
}

FDMainMenuUI * FDScreen::GetMainUI()
{
  return m_parent;
}

lUIScreenWidget * FDScreen::GetMainScreenWidget()
{
  return m_parent->GetMainScreenWidget();
}

lUIScreenWidget * FDScreen::GetSubScreenWidget()
{
  return m_parent->GetSubScreenWidget();
}

void FDScreen::Show()
{
  if( m_bHidden )
  {
    m_bHidden = false;
    DoShow();
  }
}

void FDScreen::Hide()
{
  if( !m_bHidden )
  {
    m_bHidden = true;
    DoHide();
  }
}

void FDScreen::SetModalOK(lUIButtonWidget *pWidget)
{
	m_pModalOK = pWidget;
}

lUIButtonWidget *FDScreen::GetModalOK()
{
	return m_pModalOK;
}

void FDScreen::SetModalVerifyOKToExitFunction(ModalVerifyOKToExitFunction pFunc)
{
	m_pModalVerifyFunc = pFunc;
}

bool FDScreen::RunModalVerifyOKToExitFunction()
{
	return (*m_pModalVerifyFunc)(this);
}

bool FDScreen::IsForcedUpdateRequested() const
{
	return m_bForcedUpdateRequested;
}

void FDScreen::SetForcedUpdateRequested(const bool bUpdate)
{
	m_bForcedUpdateRequested = bUpdate;
}
