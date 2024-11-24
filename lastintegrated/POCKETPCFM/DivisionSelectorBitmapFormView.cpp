/*

	GridDivisionSelectorBitmapForm.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"


BEGIN_MESSAGE_MAP(CDivisionSelectorBitmapFormView, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapFormView::CDivisionSelectorBitmapFormView
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelectorBitmapFormView::CDivisionSelectorBitmapFormView(UINT nIDTemplate)
	: CBitmapFormView(nIDTemplate)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapFormView::~CDivisionSelectorBitmapFormView
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelectorBitmapFormView::~CDivisionSelectorBitmapFormView(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapFormView::OnInitialUpdate
	Access:    	protected 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _StrID
	Parameter: 	bool _ColourBackGround
	Parameter: 	COLORREF _Colour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID)
{
	CBitmapFormView::OnInitialUpdate(_strHelpTopic, _BasicDisplay, _StrID);
	m_DivisionSelector.DoInitialise(this, _BasicDisplay, IDR_PNGLEFT, IDR_PNGRIGHT);
}


/*------------------------------------------------------------------------------
Method:   	CDivisionSelectorBitmapFormView::OnKeyDown
Access:    	public 
Parameter: 	UINT nChar
Parameter: 	UINT nRepCnt
Parameter: 	UINT nFlags
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapFormView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_DivisionSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelectorBitmapFormView::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapFormView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_DivisionSelector.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CDivisionSelectorBitmapFormView::OnLButtonDown
Access:    	public 
Parameter: 	UINT _nFlags
Parameter: 	CPoint _Point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapFormView::OnLButtonDown(UINT _nFlags, CPoint _Point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(_nFlags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CDivisionSelectorBitmapFormView::OnPaint
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelectorBitmapFormView::OnPaint()
{
	CBitmapFormView::GetBasicDisplay()->OnPaint(true);
	m_DivisionSelector.OnPaint();
}
