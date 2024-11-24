/*

	Screen04.cpp

	Manager Club selection

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "FootballDirector.h"

#include "Screen04.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CScreen04::CScreen04
	Access:    	public 
	Parameter: 	const ushort nLastClubID
	Parameter: 	CWnd* pParent 
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen04::CScreen04(const ushort nLastClubID, CWnd* pParent /*=null*/)
	: m_LastClubID(nLastClubID)
	, CDivisionSelectorBitmapDialog(CScreen04::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen04, CDivisionSelectorBitmapDialog)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION1, OnViewClub)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen04 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDivisionSelectorBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			ReDraw();
			break;

		case VK_F23:
		case VK_RETURN:
			OnViewClub();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnViewClub
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnViewClub()
{
	theApp.OnViewSquadDetails((CClub*)CBitmapDialog::GetGrid().GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnLButtonDown
	Access:    	public 
	Parameter: 	HWND _hWnd
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnLButtonDown(UINT _Flags, CPoint _Point)
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
	Method:   	CScreen04::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen04::OnInitDialog()
{
	CDivisionSelectorBitmapDialog::OnInitDialog(IDS_HELPSCREEN04, IDS_SCREEN04);
	CBitmapDialog::DoDisableOKButton();
	CBitmapDialog::LoadMenu(IDR_SCREEN04MENU);
	CBitmapDialog::GetGrid().SetRowHeight(20);
	CBitmapDialog::GetGrid().SetTextFont(GetBasicDisplay()->GetNormalFont());
	HDC dc = ::GetDC(m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);
	SetIsInitialised(true);
	OnSelchangeDivisionlist();
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea(false);
	m_DivisionSelector.OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnSelchangeDivisionlist()
{
	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.RemoveAllData();
	theGrid.DoAddColumn(24, COL_BITMAP);
	theGrid.DoAddColumn(156, COL_ALIGNL);
	CDivision& theDivision = WorldData().GetDivisionList().GetAt(m_DivisionSelector.GetCurSel()); 
	theGrid.DoAddRows(theDivision.GetNumberOfClubsInDivision());
	for (int LoopCount = 0; LoopCount < theDivision.GetNumberOfClubsInDivision(); LoopCount++)
	{
		theGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, theDivision.GetClub(LoopCount).GetHomeKitID());
		theGrid.SetItemText(LoopCount, 1, theDivision.GetClub(LoopCount).GetName());
		theGrid.SetRowPointer(LoopCount, &theDivision.GetClub(LoopCount));
	}
	theGrid.DoSortLow(1);
	theGrid.SetSelectedRow(0);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	ushort iColumn = CBitmapDialog::GetGrid().GetCurrentRow(_Point);
	if (iColumn != wNOTFOUND)
	{
		ReDraw();
	}
	else 
	{
		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
		if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
		{
			OnSelchangeDivisionlist();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnOK()
{
	CClub* pClub = (CClub*)CBitmapDialog::GetGrid().GetCurrentRowPointer();
	WorldData().OnInitialiseUser(pClub->DoFindID());
	EndDialog(IDOK);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen04::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);

		// Club Grid
		int CentreX = theRect.right / 2 - CBitmapDialog::GetGrid().GetScrollBarWidth() / 2;
		int CurrY = TOPY + 20;
		int RowsToDisplay = (theRect.bottom - CurrY - 2) / CBitmapDialog::GetGrid().GetRowHeight();
		CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, CurrY, GetCentreX() + GRID_WIDTH / 2 - CBitmapDialog::GetGrid().GetScrollBarWidth(), CurrY + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
		
		// Division Listbox
		theRect.top = TOPY;
		theRect.bottom = theRect.top + 18;
		m_DivisionSelector.MoveWindow(theRect);
	}	
	Sleep(1);
}
