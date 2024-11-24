// Screen11View.cpp : implementation file
//


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "MessageDisplay.h"
#include "FootballDirector.h"
#include "Screen11View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen11View

IMPLEMENT_DYNCREATE(CScreen11View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::CScreen11View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen11View::CScreen11View()
	: CBitmapFormView(CScreen11View::IDD) {}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::~CScreen11View
	Access:    	public 
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen11View::~CScreen11View()
{
};


BEGIN_MESSAGE_MAP(CScreen11View, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen11View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Club News"), &theApp.GetBasicDisplay(), IDS_SCREEN11);
    // Setup message display
    m_MessageDisplay.SetDrawInfo(this, 									// parent
								 0x1000,								// scrollbar ID
								 CRect(2, 44, 240, 242),				// rectangle
								 BLACK,									// text colour
								 TRUE);									// visible border
	m_MessageDisplay.SetDrawLastFirst(true);

// 	TCITEM tcItem;
// 	tcItem.mask = TCIF_TEXT;
// 	CString pstrString;
//     CTabCtrl* pViewMode = (CTabCtrl*)GetDlgItem(IDC_TAB11);
// 	EXT_ASSERT(pViewMode);
// 
// 	pstrString.LoadString(IDS_TAB1101);
// 	tcItem.pszText = pstrString.GetBuffer(256);
// 	pViewMode->InsertItem(0, &tcItem);
// 
// 	pstrString.LoadString(IDS_TAB1102);
// 	tcItem.pszText = pstrString.GetBuffer(256);
// 	pViewMode->InsertItem(1, &tcItem);

	LRESULT result;
	NMHDR nNMHDR;
	OnSelchangeTab(&nNMHDR, &result);
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnDraw(CDC* pDC)
{
	OnInitRedrawArea();
	m_MessageDisplay.OnPaint(CBitmapFormView::GetMemDC());
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnSelchangeTab
	Access:    	protected 
	Parameter: 	NMHDR * pNMHDR
	Parameter: 	LRESULT * pResult
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
 	OnClearDisplayLists();
 	CString Date = TEXT("");	/*WorldData().LongDateStr();*/
//     CTabCtrl* pViewMode = (CTabCtrl*)GetDlgItem(IDC_TAB11);
// 	EXT_ASSERT_POINTER(pViewMode, CTabCtrl);
// 	if (pViewMode->GetCurSel() == 1)
// 	{
 		WorldData().GetCurrentUserClub()->GetActiveCountry().GetNewsList().DoBuildDisplayList(m_strlistNews, false/*true*/, Date, true);
// 	}
// 	else
// 	{
 		WorldData().GetCurrentUser()->GetNewsList().DoBuildDisplayList(m_strlistNews, false, Date, true);
// 	}
 	m_MessageDisplay.AddStringList(m_strlistNews, BLUE);
	*pResult = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnClearDisplayLists
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnClearDisplayLists()
{
	m_strlistNews.RemoveAll();
	m_MessageDisplay.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen11View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen11View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);

		CRect theRect;
		GetClientRect(&theRect);

		m_MessageDisplay.MoveWindow(CRect(2, 44, theRect.right, theRect.bottom - 24));

// 		theRect.top = theRect.bottom - TABHEIGHT - 2;
// 		theRect.bottom -= 2;
// 		theRect.left = theRect.right - 120;
// 		theRect.right -= 20;
// 		GetDlgItem(IDC_TAB11)->MoveWindow(&theRect);
	}
	Sleep(1);
}