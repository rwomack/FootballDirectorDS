
/*

	TitledTextSelectorRegion.cpp: implementation for the TitledTextSelectorRegion class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"



/*------------------------------------------------------------------------------
	Method:   	CTitledTextSelectorRegion::CTitledTextSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTitledTextSelector::CTitledTextSelector(void)
	: m_TitleTextRegion(NULL)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTitledTextSelectorRegion::~CTitledTextSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTitledTextSelector::~CTitledTextSelector(void)
{
	delete m_TitleTextRegion;
}


/*------------------------------------------------------------------------------
	Method:   	CTitledTextSelectorRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	int _TitleStrID
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownGfxID
	Parameter: 	int _ButtonUpGfxID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTitledTextSelector::DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	m_Ypos = _YPos;
	m_TitleTextRegion = new CTextRegion;
	m_TitleTextRegion->DoInitialise(_BasicDisplay, _TitleStrID, COL_ALIGNR);
	_BasicDisplay->DoAddTextRegion(m_TitleTextRegion);
	CTextSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownGfxID, _ButtonUpGfxID);
}


/*------------------------------------------------------------------------------
	Method:   	CTitledTextSelector::SetStyle
	Access:    	public 
	Parameter: 	const uint _Style
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTitledTextSelector::SetStyle(const uint _Style)
{
	CTextSelector::GetTextRegion()->SetStyle(_Style);
}


/*------------------------------------------------------------------------------
	Method:   	CTitledTextSelectorRegion::MoveWindow
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTitledTextSelector::MoveWindow()
{
	CRect theWindow;
	CTextSelector::GetTextRegion()->GetParentWnd()->GetClientRect(&theWindow);
	
	int XPos = theWindow.right / 2 - eUPDOWNWIDTH - 2;
	EXT_ASSERT_POINTER(m_TitleTextRegion, CTextRegion);
	m_TitleTextRegion->MoveWindow(CRect(2, m_Ypos, XPos - 6, m_Ypos + 16));
	CTextSelector::MoveWindow(CRect(XPos + eUPDOWNWIDTH, m_Ypos, XPos + eUPDOWNWIDTH + 100, m_Ypos + 16));
}
