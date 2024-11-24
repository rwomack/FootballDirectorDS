/*

	GridDivisionSelectorBitmapForm.cpp

	Football Director

	(c)  2007

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"


BEGIN_MESSAGE_MAP(CGridDivisionSelectorBitmapFormView, CDivisionSelectorBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::CGridDivisionSelectorBitmapFormView
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Returns:   	
	Qualifier: 	: CDivisionSelectorBitmapFormView(nIDTemplate)
	Purpose:
------------------------------------------------------------------------------*/
CGridDivisionSelectorBitmapFormView::CGridDivisionSelectorBitmapFormView(UINT nIDTemplate)
	: CDivisionSelectorBitmapFormView(nIDTemplate)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::~CGridDivisionSelectorBitmapFormView
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridDivisionSelectorBitmapFormView::~CGridDivisionSelectorBitmapFormView(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::OnInitialUpdate
	Access:    	virtual protected 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _StrID
	Parameter: 	bool _ColourBackGround
	Parameter: 	COLORREF _Colour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID)
{
	CDivisionSelectorBitmapFormView::OnInitialUpdate(_strHelpTopic, _BasicDisplay, _StrID);
	m_Grid.OnInitialUpdate(this, _BasicDisplay);
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapFormView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;
	}
	CDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapFormView::OnPaint()
{
	CDivisionSelectorBitmapFormView::OnPaint();
	m_Grid.OnPaint();
}


/*------------------------------------------------------------------------------
	Method:   	CGridDivisionSelectorBitmapFormView::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridDivisionSelectorBitmapFormView::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}
