// Screen09.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen09View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen09View dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::CScreen09View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen09View::CScreen09View ()
    : CBitmapFormView(CScreen09View::IDD)
{
}


BEGIN_MESSAGE_MAP(CScreen09View, CBitmapFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
    ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen09View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::GetCurrentClub
	Access:    	protected 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CScreen09View::GetCurrentClub() const						
{ 
	return m_iCurrentClub; 
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::SetCurrentClub
	Access:    	protected 
	Parameter: 	const ushort x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::SetCurrentClub(const ushort x)					
{ 
	m_iCurrentClub = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnDraw
	Access:    	protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();
	m_Grid.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Information"), &theApp.GetBasicDisplay(), IDS_SCREEN09);
	m_Grid.OnInitialUpdate(this, GetBasicDisplay(), LC_NOHIGHLIGHT);
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_iCurrentClub = WorldData().CurrentUserClubID();
	DoBuildGrid();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::ReDraw
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::DoBuildGrid()
{
	CClub* pClub = &WorldData().GetClub(GetCurrentClub());
	CClubHistory& rClubHistory = pClub->GetClubHistory();

	m_Grid.RemoveAllData(true);
	m_Grid.DoAddColumn(-1, COL_ALIGNC);

	m_Grid.DoAddRows(36);
	int Count = 0;
	m_Grid.SetItemStrId(Count++, 0, IDS_YEARFOUNDED);
	m_Grid.SetItemText(Count++, 0, pClub->GetYearFoundedStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_MANAGER);
	m_Grid.SetItemText(Count++, 0, pClub->GetManagerName());
	m_Grid.SetItemStrId(Count++, 0, IDS_STADIUM);
	m_Grid.SetItemText(Count++, 0, pClub->GetStadium()->GetName());
	m_Grid.SetItemStrId(Count++, 0, IDS_CAPACITY);
	m_Grid.SetItemText(Count++, 0, pClub->GetStadium()->GetCapacityStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_AVATTEND);
	m_Grid.SetItemText(Count++, 0, pClub->GetSeasonStats().GetAverageAttendanceStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDSALE);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordSaleInfoStr(pClub->GetCountry()));
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDBUY);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordBuyInfoStr(pClub->GetCountry()));
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDAPPEARANCES);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordAppearanceInfoStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDCLUBGOALS);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordGoalsInfoStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDSEASONGOALS);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordSeasonGoalsInfoStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDVICTORY);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetRecordVictory()->DetailsStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDDEFEAT);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetRecordDefeat()->DetailsStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDATTENDANCE);
	m_Grid.SetItemText(Count++, 0, rClubHistory.RecordAttendanceInfoStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_LOWESTATTENDANCE);
	m_Grid.SetItemText(Count++, 0, rClubHistory.LowestAttendanceInfoStr());
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDPOINTS);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetMostPoints()->DetailsStr(pClub->GetCountry()));
	m_Grid.SetItemStrId(Count++, 0, IDS_LEASTPOINTS);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetLeastPoints()->DetailsStr(pClub->GetCountry()));
	m_Grid.SetItemStrId(Count++, 0, IDS_RECORDPOSITION);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetHighestPosition()->DetailsStr(pClub->GetCountry()));
	m_Grid.SetItemStrId(Count++, 0, IDS_LOWESTPOSITION);
	m_Grid.SetItemText(Count++, 0, rClubHistory.GetLowestPosition()->DetailsStr(pClub->GetCountry()));
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen09View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen09View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theWindow;
		GetClientRect(&theWindow);
		int RowsToDisplay = (theWindow.bottom - TOPY - 2) / m_Grid.GetRowHeight();
		m_Grid.MoveWindow(CRect (2, TOPY, theWindow.right - 2 - m_Grid.GetScrollBarWidth(), TOPY + RowsToDisplay * m_Grid.GetRowHeight()));

		Sleep(1);
	}

}