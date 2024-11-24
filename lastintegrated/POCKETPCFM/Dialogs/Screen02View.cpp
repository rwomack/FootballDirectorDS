
/*

	Screen02View.cpp

	Cup Schedules

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"

#include "CupDisplayEvent.h"
#include "Competition.h"

#include "Screen02View.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen02View dialog

IMPLEMENT_DYNCREATE(CScreen02View, CFormView)



/*------------------------------------------------------------------------------
	Method:   	CScreen02View::CScreen02View
	Access:    	public 
	Returns:   		
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen02View::CScreen02View()
    : CGridCupSelectorBitmapFormView(CScreen02View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::~CScreen02View
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen02View::~CScreen02View()
{
}


BEGIN_MESSAGE_MAP(CScreen02View, CGridCupSelectorBitmapFormView)
	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnNextCup)
	ON_COMMAND(ID_ACTION2, OnPreviousCup)

	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN02MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnNextCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnNextCup()
{
	CGridCupSelectorBitmapFormView::OnNextCup();
	OnSelchangeCupList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnPreviousCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnPreviousCup()
{
	CGridCupSelectorBitmapFormView::OnPreviousCup();
	OnSelchangeCupList();
}


 /*------------------------------------------------------------------------------
 	Method:   	CScreen02View::OnPaint
 	Access:    	protected 
 	Parameter: 	CDC* _pDC
 	Returns:   	void
 	Qualifier: 	
 	Purpose:
 ------------------------------------------------------------------------------*/
 void CScreen02View::OnPaint()
{
	CGridCupSelectorBitmapFormView::OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnInitialUpdate()
{
	CGridCupSelectorBitmapFormView::OnInitialUpdate(TEXT("Cup Schedules"), &theApp.GetBasicDisplay(), IDS_SCREEN02);
	m_Grid.DoAddStyle(LC_SHOWCOLUMNS);
	OnSelchangeCupList();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridCupSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeCupList();
			ReDraw();
			break;

		case VK_SELECT:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	if (m_CupSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeCupList();
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnSelchangeCupList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnSelchangeCupList()
{
	m_Grid.RemoveAllData(false);
	m_Grid.DoAddColumn(40, COL_TEXT | COL_ALIGNR);		
	m_Grid.DoAddColumn(60);							
	DoAdjustColumnWidths();

	CCupDisplayEventList	CupEventList;
	m_CupSelector.GetCurrentCup().DoBuildEventsStrList(CupEventList);
	m_Grid.DoAddRows(CupEventList.GetSize());
	m_Grid.DoInitialiseScrollBar(true);

	CCupRound* pCupRound;

	for (int LoopCount = 0; LoopCount < CupEventList.GetSize(); LoopCount++)
	{
		pCupRound = CupEventList[LoopCount].GetCupRound();
		switch (CupEventList[LoopCount].GetType())
		{
			case DRAWDATE:
				m_Grid.SetRowTextColour(LoopCount, RED);
				m_Grid.SetItemText(LoopCount, 0, pCupRound->LongDrawDateStr());
				break;
			case MATCHDATE:
				m_Grid.SetRowTextColour(LoopCount, BLACK);
				m_Grid.SetItemText(LoopCount, 0, pCupRound->LongMatchDateStr());
				break;
			case REPLAYDATE:
				m_Grid.SetRowTextColour(LoopCount, BLUE);
				m_Grid.SetItemText(LoopCount, 0, pCupRound->LongReplayDateStr());
				break;
			default:
				EXT_ASSERT(false);
				break;
		}
		m_Grid.SetItemText(LoopCount, 1, CupEventList[LoopCount].TitleStr());
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);
		m_CupSelector.MoveWindow(CRect(GetCentreX() - COMPETITIONNAMELENGTH, TOPY, GetCentreX() + COMPETITIONNAMELENGTH, TOPY + TABHEIGHT));

		int GridWidth = min(theRect.right - 10, 260);
		int RowsToDisplay = (theRect.bottom - 2 - DIVISIONSELECTOR_TOPY) / m_Grid.GetRowHeight();
		theRect = CRect(GetCentreX() - GridWidth / 2, DIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth / 2 - m_Grid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);
		DoAdjustColumnWidths();
	}
	Sleep(1);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen02View::DoAdjustColumnWidths
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen02View::DoAdjustColumnWidths()
{
	if (m_Grid.CountColumns() > 0)
	{
		m_Grid.SetColumnWidthPercentage(0, 40);		
		m_Grid.SetColumnWidthPercentage(1, 60);							
	}
}