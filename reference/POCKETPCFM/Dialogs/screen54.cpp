
/*

	screen54.cpp : Arrange friendly matches implementation file
	
	PocketPC

	Football Director

	(c) Rw Software 1994 - 2008

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen54.h"
#include "HumanManager.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen54 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen54::CScreen54
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen54::CScreen54(CWnd* pParent /*=null*/)
	: m_nCurrentSelection(NOCLUB)
	, CDivisionSelectorBitmapDialog(CScreen54::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::~CScreen54
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen54::~CScreen54()
{
}


BEGIN_MESSAGE_MAP(CScreen54, CDivisionSelectorBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_COMMAND(ID_ACTION1, OnNextDate)
	ON_COMMAND(ID_ACTION2, OnPreviousDate)
	ON_COMMAND(ID_ACTION3, OnNextDivision)
	ON_COMMAND(ID_ACTION4, OnPreviousDivision)
	ON_COMMAND(ID_ACTION1, OnViewClub)
	ON_COMMAND(IDP_VENUEHOME, OnHome)
	ON_COMMAND(IDP_VENUEAWAY, OnAway)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen54 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen54::OnInitDialog()
{
	CDivisionSelectorBitmapDialog::OnInitDialog(IDS_HELPSCREEN54, IDS_SCREEN54);
	m_DateList.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	for (int LoopCount = 0; LoopCount < MAXFRIENDLIES; LoopCount++)
	{
		m_DateList.DoAddItem(WorldData().GetCurrentUserClub()->FriendlyDateStr(LoopCount));
	}
	CBitmapDialog::GetGrid().SetRowHeight(20);
	HDC dc = ::GetDC(m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);
	OnSelchangeDivisionlist();
	CBitmapDialog::LoadMenu(IDR_SCREEN54MENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnNextDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnNextDate()
{
	m_DateList.OnIncCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnPreviousDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnPreviousDate()
{
	m_DateList.OnIncCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnNextDivision
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnNextDivision()
{
	m_DivisionSelector.OnIncCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnPreviousDivision
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnPreviousDivision()
{
	m_DivisionSelector.OnDecCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnViewClub
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnViewClub()
{
	theApp.OnViewSquadDetails((CClub*)CBitmapDialog::GetGrid().GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	SHRGINFO shrgi = {0};
	shrgi.cbSize = sizeof(SHRGINFO);
	shrgi.hwndClient = m_hWnd;
	shrgi.ptDown.x = _Point.x;
	shrgi.ptDown.y = _Point.y;
	shrgi.dwFlags = SHRG_RETURNCMD;
	if (CBitmapDialog::GetGrid().GetCurrentRow(_Point) != NOTFOUND && GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi))
	{
		OnViewClub();
	}
	CDivisionSelectorBitmapDialog::OnLButtonDown(_Flags, _Point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int nCurrSel = CBitmapDialog::GetGrid().GetCurrentRow(_Point);
	if (nCurrSel != NOTFOUND)
	{
		ushort wClubID = m_DivisionSelector.GetCurrentDivision().DoFindClubNumberInTablePosition(nCurrSel);
		CClub* pClub = &WorldData().GetClub(wClubID);
		if (pClub != WorldData().GetCurrentUserClub())
		{
			ReDraw();
#ifdef FMOD
			theApp.GetSoundManager().DoPlaySoundFX(SOUND_BEEP);
#endif
			m_nCurrentSelection = wClubID;
			CMenu menu;
			VERIFY(menu.LoadMenu(IDR_VENUE));
			CMenu* pPopup = menu.GetSubMenu(0);
			EXT_ASSERT(pPopup != null);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y, this);
		}
	}
	else 
	{
		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
		if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true || 
			m_DateList.OnLButtonUp(ButtonPressed) == true)
		{
			OnSelchangeDivisionlist();
			ReDraw();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDivisionSelectorBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			break;

		case VK_RETURN:
		case VK_F23:
			OnOK();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDivisionSelectorBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnSelchangeDivisionlist()
{
	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.RemoveAllData();
	theGrid.DoAddColumn(24, COL_BITMAP);
	theGrid.DoAddColumn(156, COL_ALIGNL);
	CDivision& theDivision = m_DivisionSelector.GetCurrentDivision();
	theGrid.DoAddRows(theDivision.GetNumberOfClubsInDivision());
	CClub* pClub;
	for (int LoopCount = 0; LoopCount < theDivision.GetNumberOfClubsInDivision(); LoopCount++)
	{
		pClub = &theDivision.DoFindClubInTablePosition(LoopCount);
		theGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pClub->GetHomeKitID());
		theGrid.SetItemText(LoopCount, 1, pClub->GetName());
		theGrid.SetRowPointer(LoopCount, &theDivision.GetClub(LoopCount));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	int iCurSel = m_DateList.GetCurSel();
	CClubFriendlies& theFriendlies = WorldData().GetCurrentUserClub()->GetFriendlies();
	ushort theClubID = theFriendlies.GetOpponents(iCurSel);
	CString strPrint;
	if (theClubID != NOCLUB)
	{
		CString strVenue;
		if (theFriendlies.GetVenue(iCurSel) == HOME)
		{
			strVenue.LoadString(IDS_HOME);
		}
		else
		{
			strVenue.LoadString(IDS_AWAY);
		}
		strPrint.Format(_T("%s %s %s"), WorldData().GetClub(theClubID).GetName(), 
			strVenue, theFriendlies.GetStadiumNameStr(iCurSel, WorldData().GetCurrentUserClub()));
	}
	else
	{
		strPrint.LoadString(IDS_NOMATCHARRANGED);
	}
	GetBasicDisplay()->PrintCentredString(TOPY54, strPrint, BLACK, SMALLFONT);
	m_DateList.OnPaint();
	m_DivisionSelector.OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnHome
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnHome()
{
	WorldData().GetCurrentUserClub()->GetFriendlies().CreateFriendlyMatch(m_DateList.GetCurSel(), m_nCurrentSelection, HOME);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnAway
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnAway()
{
	WorldData().GetCurrentUserClub()->GetFriendlies().CreateFriendlyMatch(m_DateList.GetCurSel(), m_nCurrentSelection, AWAY);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen54::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen54::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);

		int CurrY = TOPY54 + 16;
		theRect.top = CurrY;
		CurrY += eUPDOWNHEIGHT;
		theRect.bottom = CurrY; 
		theRect.left = GetCentreX() - COMPETITIONNAMELENGTH;
		theRect.right = GetCentreX() + COMPETITIONNAMELENGTH;
		m_DateList.MoveWindow(theRect);
		CurrY += 2;
		
		theRect.top = CurrY;
		CurrY += eUPDOWNHEIGHT;
		theRect.bottom = CurrY; 
		m_DivisionSelector.MoveWindow(theRect);
		CurrY += 2;

		GetClientRect(&theRect);
		int RowsToDisplay = (theRect.bottom - CurrY - 2) / CBitmapDialog::GetGrid().GetRowHeight();
		CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, CurrY, GetCentreX() + GRID_WIDTH / 2 - CBitmapDialog::GetGrid().GetScrollBarWidth(), CurrY + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));

		Sleep(1);
	}
}