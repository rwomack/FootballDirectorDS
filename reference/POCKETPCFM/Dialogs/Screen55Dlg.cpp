
/*

	Class - CScreen55

	Group League Tables Dialog Box class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen55Dlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen55 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::CScreen55Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen55Dlg::CScreen55Dlg(CCup& _Cup, CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen55Dlg::IDD, pParent)
{
	_Cup.DoBuildGroupTables(m_GroupTables);
	m_NumberOfClubsInGroup = _Cup.GetNumberOfClubsInGroup();
	m_NumberOfGroups = _Cup.GetNumberOfGroups();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::~CScreen55Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen55Dlg::~CScreen55Dlg()
{
	m_GroupTables.RemoveList();
}


BEGIN_MESSAGE_MAP(CScreen55Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION1, OnNextGroup)
	ON_COMMAND(ID_ACTION2, OnPreviousGroup)
	ON_COMMAND(ID_ACTION3, OnViewSquad)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen55 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnNextGroup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnNextGroup()
{
	m_GroupSelector.OnIncCurSel();
	OnSelchangeGroupList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnPreviousGroup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnPreviousGroup()
{
	m_GroupSelector.OnDecCurSel();
	OnSelchangeGroupList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnViewSquad
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnViewSquad()
{
	WorldData().OnViewSquadDetails((CClub*)CBitmapDialog::GetGrid().GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen55Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN55, IDS_SCREEN55);
	CBitmapDialog::GetGrid().SetHiliteBackgroundColour(TRANSCOLOUR);

	m_GroupSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	CExtString str;
	for (int LoopCount = 0; LoopCount < m_NumberOfGroups; LoopCount++)
	{
		str.Format(IDS_GROUP, LoopCount + 1);
		m_GroupSelector.DoAddItem(str);
	}
	m_GroupSelector.SetCurSel(0);
	OnSelchangeGroupList();
	CBitmapDialog::LoadMenu(IDR_SCREEN55MENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeGroupList();
			break;

		case VK_RETURN:
		case VK_SELECT:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	CBitmapDialog::OnLButtonDown(_Flags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
	if (m_GroupSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeGroupList();
	}
	if (CBitmapDialog::GetGrid().GetCurrentRow(_Point) != NOTFOUND)
	{
		OnViewSquad();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnSelchangeGroupList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnSelchangeGroupList()
{
	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.RemoveAllData(true);
	theGrid.DoAddColumn(m_ClubNameLength);					// Club Name
	theGrid.DoAddColumn(18, COL_NUMERICR);	// Played
	theGrid.DoAddColumn(18, COL_NUMERICR);	// Won
	theGrid.DoAddColumn(18, COL_NUMERICR);	// Drawn
	theGrid.DoAddColumn(18, COL_NUMERICR);	// Lost
	theGrid.DoAddColumn(26, COL_NUMERICR);	// For
	theGrid.DoAddColumn(26, COL_NUMERICR);	// Against
	theGrid.DoAddColumn(26, COL_NUMERICR);	// Points
	theGrid.DoAddRows(m_NumberOfClubsInGroup);
	int First = m_GroupSelector.GetCurSel() * m_NumberOfClubsInGroup;
	CClub* pClub;
	for (int LoopCount = 0; LoopCount < m_NumberOfClubsInGroup; LoopCount++)
	{
		pClub = &WorldData().GetClub(m_GroupTables.GetAt(LoopCount + First)->GetClubID());
		DEBUG_OUTPUT(("%d %s : w %d : d %d : l %d"), (m_GroupTables.GetAt(LoopCount + First)->GetPoints(), pClub->GetName(), m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesWon(), m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesDrawn(), m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesLost()));
		theGrid.SetItemText(LoopCount, 0, pClub->GetName(), pClub);
		theGrid.SetItemNum(LoopCount, 1, m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesPlayed());
		theGrid.SetItemNum(LoopCount, 2, m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesWon());
		theGrid.SetItemNum(LoopCount, 3, m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesDrawn());
		theGrid.SetItemNum(LoopCount, 4, m_GroupTables.GetAt(LoopCount + First)->GetNumberGamesLost());
		theGrid.SetItemNum(LoopCount, 5, m_GroupTables.GetAt(LoopCount + First)->GetNumberGoalsScored());
		theGrid.SetItemNum(LoopCount, 6, m_GroupTables.GetAt(LoopCount + First)->GetNumberGoalsConceded());
		theGrid.SetItemNum(LoopCount, 7, m_GroupTables.GetAt(LoopCount + First)->GetPoints());
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen55Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen55Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);

		CRect theWindow;
		GetClientRect(&theWindow);
		int GridWidth = 118;
		m_ClubNameLength = 74;
		if (theWindow.right > 300)
		{
			m_ClubNameLength = 114;	// Club Name
			GridWidth += 20;
		}
		CBitmapDialog::GetGrid().SetColumnWidth(0, m_ClubNameLength);	// Club Name
		int RowsToDisplay = min(m_NumberOfClubsInGroup, (theWindow.bottom - TOPY - 2 - TABHEIGHT) / CBitmapDialog::GetGrid().GetRowHeight());
		CRect theRect(GetCentreX() - GridWidth, DATEDIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth - CBitmapDialog::GetGrid().GetScrollBarWidth(), DATEDIVISIONSELECTOR_TOPY + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight());	
		CBitmapDialog::GetGrid().MoveWindow(theRect);

		m_GroupSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + TABHEIGHT));
	}
	Sleep(1);
}