/*

	CupSelectorBitmapFormView.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"


BEGIN_MESSAGE_MAP(CCupSelectorBitmapFormView, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::CCupSelectorBitmapFormView
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupSelectorBitmapFormView::CCupSelectorBitmapFormView(UINT nIDTemplate)
	: CBitmapFormView(nIDTemplate)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::~CCupSelectorBitmapFormView
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupSelectorBitmapFormView::~CCupSelectorBitmapFormView(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::OnInitialUpdate
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
void CCupSelectorBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID)
{
	CBitmapFormView::OnInitialUpdate(_strHelpTopic, _BasicDisplay, _StrID);
	m_CupSelector.DoInitialise(this, _BasicDisplay, IDR_PNGLEFT, IDR_PNGRIGHT);
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::OnNextCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnNextCup()
{
	m_CupSelector.OnIncCurSel();
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::OnPreviousCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnPreviousCup()
{
	m_CupSelector.OnDecCurSel();
}


/*------------------------------------------------------------------------------
Method:   	CCupSelectorBitmapFormView::OnKeyDown
Access:    	public 
Parameter: 	UINT nChar
Parameter: 	UINT nRepCnt
Parameter: 	UINT nFlags
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_CupSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelectorBitmapFormView::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_CupSelector.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CCupSelectorBitmapFormView::OnLButtonDown
Access:    	public 
Parameter: 	UINT _nFlags
Parameter: 	CPoint _Point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnLButtonDown(UINT _nFlags, CPoint _Point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(_nFlags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CCupSelectorBitmapFormView::OnPaint
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCupSelectorBitmapFormView::OnPaint()
{
	CBitmapFormView::GetBasicDisplay()->OnPaint(true);
	m_CupSelector.OnPaint();
}
