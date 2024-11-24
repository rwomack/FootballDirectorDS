/*

	Screen48View.cpp

	Player Transfer List

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "FootballDirector.h"
#include "PopupPlayerMenu.h"
#include "Form.h"
#include "Screen48View.h"
#include "HumanManager.h"
#include "PlayerSearch.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::CScreen48View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen48View::CScreen48View()
    : CGridDivisionSelectorBitmapFormView(CScreen48View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::~CScreen48View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen48View::~CScreen48View()
{
}


BEGIN_MESSAGE_MAP(CScreen48View, CGridDivisionSelectorBitmapFormView)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnFilter)
	ON_COMMAND(ID_ACTION2, OnSortHighestSkill)
	ON_COMMAND(ID_ACTION3, OnSortLowestSkill)
	ON_COMMAND(ID_ACTION4, OnSortHighestPrice)
	ON_COMMAND(ID_ACTION5, OnSortLowestPrice)
	ON_COMMAND(ID_ACTION6, OnViewDetails)
	ON_COMMAND(ID_ACTION7, OnMakeOffer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen48View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN48MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT7);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnFilter
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnFilter()
{
	theApp.OnPlayerSearch();
	DoBuildPlayerList();
}


void CScreen48View::OnSortHighestSkill()
{
}
void CScreen48View::OnSortLowestSkill()
{
}
void CScreen48View::OnSortHighestPrice()
{
}
void CScreen48View::OnSortLowestPrice()
{
}
void CScreen48View::OnViewDetails()
{
}
void CScreen48View::OnMakeOffer()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnDraw
	Access:    	protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnPaint()
{
	CGridDivisionSelectorBitmapFormView::OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnInitialUpdate()
{
	CGridDivisionSelectorBitmapFormView::OnInitialUpdate(TEXT("Transfer List"), &theApp.GetBasicDisplay(), IDS_SCREEN48);
	m_Grid.SetRowHeight(20);
	HDC dc = ::GetDC(m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);
	m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNC);
	OnSelchangeDivisionlist();

	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			ReDraw();
			break;

		case VK_F23:
		case VK_RETURN:
			OnViewPlayerMenu();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnSelchangeDivisionlist()
{
	DoBuildPlayerList();
}



/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnViewPlayerMenu
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnViewPlayerMenu()
{
	WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
	CPlayer* pPlayer = (CPlayer*)m_Grid.GetCurrentRowPointer();
	CPopupPlayerMenu TheMenu;
	TheMenu.DoModal(pPlayer, CPoint(GetCentreX() - 30, GetCentreY() - 20));
	DoBuildPlayerList();
	m_ClubBalanceText.DoUpdate();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::DoBuildPlayerList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::DoBuildPlayerList()
{
	WorldData().GetCurrentUser()->GetPlayerSearch().DoEmpty();
	m_DivisionSelector.GetCurrentDivision().GetAllTransferListed(WorldData().GetCurrentUserClub(), WorldData().GetCurrentUser()->GetPlayerSearch());
	if (WorldData().GetCurrentUser()->GetPlayerSearch().GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == true)
	{
		WorldData().GetOutOfContractPlayerList().DoBuildSuitableList(WorldData().GetCurrentUser()->GetPlayerSearch());
	}

	// Clear List
	m_Grid.RemoveAllData();
	m_Grid.DoAddColumn(24, COL_BITMAP);
	m_Grid.DoAddColumn(86);
	m_Grid.DoAddColumn(26, COL_NUMERICR);
	m_Grid.DoAddColumn(80, COL_MONEY);

	CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize(); LoopCount++)
	{
		pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[LoopCount]);
		m_Grid.DoAddRow();
		m_Grid.SetRowPointerDetails(LoopCount, pPlayer, PLAYERCONTRACT);

		m_Grid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
		m_Grid.SetItemText(LoopCount, 1, pPlayer->GetName());
		m_Grid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);

		m_Grid.SetItemNum(LoopCount, 2, pPlayer->GetSkills().GetOverallSkill());
		m_Grid.SetItemTextColour(LoopCount, 2, PosnColour[pPlayer->GetSkills().GetPosition()]);

		m_Grid.SetItemText(LoopCount, 3, WorldData().GetCurrentUserClub()->GetForeignStr(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount]));
		m_Grid.SetItemTextColour(LoopCount, 3, PosnColour[pPlayer->GetSkills().GetPosition()]);
	}
	m_Grid.ResetScrollBar();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
	else
	{
		int iColumn = m_Grid.GetCurrentRow(_Point);
		if (iColumn != NOTFOUND)
		{
			if (iColumn < WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize())
			{
				WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
				CPlayer* pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[iColumn]);
				CPopupPlayerMenu TheMenu;
				TheMenu.DoModal(pPlayer, _Point);
				DoBuildPlayerList();
				m_ClubBalanceText.DoUpdate();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen48View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen48View::OnSize(UINT nType, int cx, int cy)
{
	CGridDivisionSelectorBitmapFormView::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + TABHEIGHT));

		CRect theRect(theWindow);
		int RowsToDisplay = (theRect.bottom - 2 - DIVISIONSELECTOR_TOPY - 16) / m_Grid.GetRowHeight();
		theRect = CRect(GetCentreX() - GRID_WIDTH / 2, DIVISIONSELECTOR_TOPY, GetCentreX() + GRID_WIDTH / 2 - m_Grid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);

		m_ClubBalanceText.MoveWindow(CRect(0, theWindow.bottom - 18, theWindow.right, theWindow.bottom - 2));
	}

	Sleep(1);
}