/*

	GridBitmapForm.cpp

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"


BEGIN_MESSAGE_MAP(CGridBitmapDialog, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::CGridBitmapDialog
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParentWnd
	Returns:   	
	Qualifier: 	: CBitmapDialog(nIDTemplate, pParentWnd)
	Purpose:
------------------------------------------------------------------------------*/
CGridBitmapDialog::CGridBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
	: CBitmapDialog(nIDTemplate, pParentWnd)
{

}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::CGridBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridBitmapDialog::CGridBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::~CGridBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridBitmapDialog::~CGridBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CGridBitmapDialog::OnInitDialog(const CString _strHelpTopic, int _StrID)
{
	m_Grid.OnInitialUpdate(this, GetBasicDisplay());
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	return CBitmapDialog::OnInitDialog(_strHelpTopic, _StrID);
}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridBitmapDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;

	}
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridBitmapDialog::OnPaint()
{
	CBitmapDialog::OnPaint(true, false);
	m_Grid.OnPaint();
	CBitmapDialog::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CGridBitmapDialog::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridBitmapDialog::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}
