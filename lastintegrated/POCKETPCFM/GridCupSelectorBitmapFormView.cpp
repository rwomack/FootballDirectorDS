/*

	GridDivisionSelectorBitmapForm.cpp

	Football Director

	(c) Rw Software 2007 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"


BEGIN_MESSAGE_MAP(CGridCupSelectorBitmapFormView, CCupSelectorBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::CGridCupSelectorBitmapFormView
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGridCupSelectorBitmapFormView::CGridCupSelectorBitmapFormView(UINT nIDTemplate)
	: CCupSelectorBitmapFormView(nIDTemplate)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::~CGridCupSelectorBitmapFormView
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridCupSelectorBitmapFormView::~CGridCupSelectorBitmapFormView(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::OnInitialUpdate
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
void CGridCupSelectorBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID)
{
	CCupSelectorBitmapFormView::OnInitialUpdate(_strHelpTopic, _BasicDisplay, _StrID);
	m_Grid.OnInitialUpdate(this, _BasicDisplay);
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_Grid.SetHiliteBackgroundColour(TRANSCOLOUR);

}


/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridCupSelectorBitmapFormView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;
	}
	CCupSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridCupSelectorBitmapFormView::OnPaint()
{
	CBitmapFormView::GetBasicDisplay()->OnPaint(true);
	CCupSelectorBitmapFormView::OnPaint();
	m_Grid.OnPaint();
}


/*------------------------------------------------------------------------------
	Method:   	CGridCupSelectorBitmapFormView::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridCupSelectorBitmapFormView::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}
