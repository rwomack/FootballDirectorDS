/*

	DivisionSelectorBitmapDialog.cpp

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"


BEGIN_MESSAGE_MAP(CDivisionSelectorBitmapDialog, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	//ON_WM_PAINT()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::CDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParentWnd
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelectorBitmapDialog::CDivisionSelectorBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
	: CBitmapDialog(nIDTemplate, pParentWnd)
{

}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::CDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelectorBitmapDialog::CDivisionSelectorBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::~CDivisionSelectorBitmapDialog
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelectorBitmapDialog::~CDivisionSelectorBitmapDialog(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::OnInitDialog
	Access:    	public 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	int _StrID
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CDivisionSelectorBitmapDialog::OnInitDialog(const int _HelpTopic, int _StrID)
{
	m_DivisionSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	return CBitmapDialog::OnInitDialog(_HelpTopic, _StrID);
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_DivisionSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_DivisionSelector.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapDialog::OnLButtonDown(UINT _nFlags, CPoint _Point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(_nFlags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapDialog::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
// void CDivisionSelectorBitmapDialog::OnPaint()
// {
// 	CBitmapDialog::OnPaint(true, true);
// }
