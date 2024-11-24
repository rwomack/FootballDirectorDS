// Screen92.cpp : Team Of The Week : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen92.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen92 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen92::CScreen92
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen92Dlg::CScreen92Dlg(CWnd* pParent /*=null*/)
    : CDivisionSelectorBitmapDialog(CScreen92Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen92Dlg, CDivisionSelectorBitmapDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION3, OnViewPlayerDetails)
	ON_COMMAND(ID_ACTION4, OnViewSquadDetails)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen92 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen92Dlg::OnViewPlayerDetails
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnViewPlayerDetails()
{
	WorldData().OnViewPlayerDetails((CPlayer*)CBitmapDialog::GetGrid().GetCurrentRowPointer(1));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92Dlg::OnViewSquadDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnViewSquadDetails()
{
	WorldData().OnViewSquadDetails((CClub*)CBitmapDialog::GetGrid().GetCurrentRowPointer(2));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_DivisionSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			OnSelchangeDivisionlist();
			break;

		case VK_SELECT:
			OnOK();
			break;
	}
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDivisionSelectorBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_DivisionSelector.OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen92Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN92, IDS_SCREEN92);
	m_DivisionSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	CBitmapDialog::GetGrid().SetRowHeight(20);
	HDC dc = ::GetDC(m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);

	m_DivisionSelector.SetCurSel(WorldData().GetCurrentUserClub()->GetDivisionID());
	OnSelchangeDivisionlist();
	CBitmapDialog::LoadMenu(IDR_SCREEN92MENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnSelchangeDivisionlist()
{
	// Clear List
	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.RemoveAllData();
	CDivision& theDivision = m_DivisionSelector.GetCurrentDivision();

	if (theDivision.GetTeamOfTheWeek().IsAvailable() == true)
	{
		theGrid.DoAddColumn(16, COL_NUMERICR);
		theGrid.DoAddColumn(98);	// Player name
		theGrid.DoAddColumn(24, COL_BITMAP);
		theGrid.DoAddColumn(98);	// Club name
		theGrid.DoAddRows(PLAYERSINTEAM);
		CPlayerOfTheWeek* pPlayerOfTheWeek;
		for (int LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
		{
			pPlayerOfTheWeek = theDivision.GetTeamOfTheWeek().GetAt(LoopCount);	
			theGrid.SetItemNum(LoopCount, 0, LoopCount + 1);
			theGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayerOfTheWeek->GetPlayer().GetSkills().GetPosition()]);
			theGrid.SetItemText(LoopCount, 1, pPlayerOfTheWeek->GetPlayer().GetName(), &pPlayerOfTheWeek->GetPlayer());
			theGrid.SetItemBitmap(LoopCount, 2, &m_pngSmallKits, WorldData().GetClub(pPlayerOfTheWeek->GetClubID()).GetHomeKitID());
			theGrid.SetItemText(LoopCount, 3, WorldData().GetClub(pPlayerOfTheWeek->GetClubID()).GetName(), &WorldData().GetClub(pPlayerOfTheWeek->GetClubID()));
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	CDivisionSelectorBitmapDialog::OnLButtonDown(_Flags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
	else	
	{
		int iColumn = CBitmapDialog::GetGrid().GetCurrentRow(point);
		if (iColumn != NOTFOUND)
		{
			CPlayer* pPlayer = &m_DivisionSelector.GetCurrentDivision().GetTeamOfTheWeek().GetAt(iColumn)->GetPlayer();
			if (CBitmapDialog::GetGrid().HitTestCol(point) == 1)
			{
				// View Player stuff
				CPopupPlayerMenu TheMenu;
				TheMenu.DoModal(pPlayer, point);
			}
			else
			{
				// View Club Info
				CPopupClubMenu TheMenu;
				TheMenu.DoModal(&pPlayer->GetClub(), point);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen92::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen92Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);

		CRect theRect;
		GetClientRect(&theRect);
		int GridHeight = (theRect.bottom - eUPDOWNHEIGHT - 2 - TOPY);
		int RowsToDisplay = min(PLAYERSINTEAM, GridHeight / CBitmapDialog::GetGrid().GetRowHeight());
		int TopY = TOPY + eUPDOWNHEIGHT + 2;
		CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, TopY, GetCentreX() + GRID_WIDTH / 2 - CBitmapDialog::GetGrid().GetScrollBarWidth(), TopY + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
		m_DivisionSelector.MoveWindow(CRect(0, TOPY, 0, TOPY + eUPDOWNHEIGHT));
	}
	Sleep(1);
}