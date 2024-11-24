/*

	GridDivisionSelectorBitmapDialog.cpp

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"


BEGIN_MESSAGE_MAP(CGridDivisionSelectorBitmapDialog, CDivisionSelectorBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::CGridDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CGridDivisionSelectorBitmapDialog::CGridDivisionSelectorBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
	: CDivisionSelectorBitmapDialog(nIDTemplate, pParentWnd)
{

}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::CGridDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridDivisionSelectorBitmapDialog::CGridDivisionSelectorBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::~CGridDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridDivisionSelectorBitmapDialog::~CGridDivisionSelectorBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::OnInitDialog
	Access:    	virtual protected 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	int _StrID
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CGridDivisionSelectorBitmapDialog::OnInitDialog(const CString _strHelpTopic, int _StrID)
{
	m_Grid.OnInitialUpdate(this, GetBasicDisplay());
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	return CDivisionSelectorBitmapDialog::OnInitDialog(_strHelpTopic, _StrID);
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;
	}
	CDivisionSelectorBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapDialog::OnPaint()
{
	m_DivisionSelector.OnPaint();
	m_Grid.OnPaint();
	CBitmapDialog::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapDialog::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapDialog::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}
