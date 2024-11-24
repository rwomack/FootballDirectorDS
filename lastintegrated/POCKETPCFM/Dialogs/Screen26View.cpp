// Screen26View.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen26View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CScreen26View

IMPLEMENT_DYNCREATE(CScreen26View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::CScreen26View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen26View::CScreen26View()
	: CBitmapFormView(CScreen26View::IDD)
	, m_NumOnScreen(11)
	, m_eWhichPeriod(WEEKLY_FINANCE)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::~CScreen26View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen26View::~CScreen26View()
{
}


BEGIN_MESSAGE_MAP(CScreen26View, CBitmapFormView)
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnViewCurrentWeek)
	ON_COMMAND(ID_ACTION2, OnViewYearToDate)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreen26View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnActionClicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN26MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnViewCurrentWeek
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnViewCurrentWeek()
{
	m_eWhichPeriod = WEEKLY_FINANCE;
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnViewYearToDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnViewYearToDate()
{
	m_eWhichPeriod = ANNUAL_FINANCE;
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Accounts"), &theApp.GetBasicDisplay(), IDS_SCREEN26);
	m_Grid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_NOHIGHLIGHT, GAPY - 2);
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_Grid.SetTextFont(GetBasicDisplay()->GetSmallFont());
	m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNR, true);

// 	m_Header.DoInitialise(GetBasicDisplay(), COL_ALIGNR);
// 	int top = 44;
// 	int left = GetBasicDisplay()->GetCentreX() - 2;
// 	if (m_Grid.IsScrollBarVisible() == false)
// 	{
// 		left += m_Grid.GetScrollBarWidth() / 2; 
// 	}
// 	CExtString strPrint;
// 	strPrint.LoadString(IDS_INCOME);
// 	GetBasicDisplay()->SetFontSmall();
// 	GetBasicDisplay()->PrintString(left, top, strPrint);
// 
// 	left += 60;
// 	strPrint.LoadString(IDS_EXPENDITURE);
// 	GetBasicDisplay()->PrintString(left, top, strPrint);

	m_eWhichPeriod = WEEKLY_FINANCE;
	DoBuildGrid();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::DoBuildGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::DoBuildGrid()
{
	m_Grid.RemoveAllData(true);
	m_Grid.DoAddColumn(112);				// Description
	m_Grid.DoAddColumn(62, COL_NUMERICR);	// Income
	m_Grid.DoAddColumn(62, COL_NUMERICR);	// Expenditure

	CClub* pClub = WorldData().GetCurrentUserClub();

	CAccountsGridItem theList[50];		// Ensure when adding more to accounts that this is sufficient
	int NumFound = 0;
	int Income = 0, Expenditure = 0;


	if (m_eWhichPeriod == WEEKLY_FINANCE)
	{
		if (pClub->GetMidWeekMatchGateReceipts() > 0)
		{
			ProfitOut(static_cast<int>(pClub->GetMidWeekMatchGateReceipts()), IDS_MIDGATERECIEPTS)
		}
		if (pClub->GetWeekEndMatchGateReceipts() > 0)
		{
			ProfitOut(static_cast<int>(pClub->GetWeekEndMatchGateReceipts()), IDS_GATERECIEPTS)
		}
	}
	else if (pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod) > 0)
	{
		ProfitOut(static_cast<int>(pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod)), IDS_GATERECIEPTS)
	}
// 	DEBUG_OUTPUT(("Annual Gate Receipts %d, Ticket Price %d, Average Attendance %d"), (pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod), pClub->GetTicketPrice(), pClub->DoCalculateAverageAttendance()));

	double dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALWAGE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(dAmount, IDS_MERCHANDISE)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::SPONSORSHIPINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_SPONSORSHIP)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TELEVISIONINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_TELEVISION)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::SECURITY).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_SECURITYCOSTS)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALWAGE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_GENERALWAGES)	// General wages
	}

	if (pClub->GetManagerID() != NOMANAGER)
	{
		DebitOut(static_cast<int>(pClub->GetAccounts()->GetAt(CAccountsItemList::MANAGERSWAGES).GetAmount(m_eWhichPeriod)), IDS_MANAGERSWAGES)   // Manager's wage
	}

	DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::PLAYERWAGES).GetAmount(m_eWhichPeriod), IDS_PLAYERWAGES)
	DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::EMPLOYEEWAGES).GetAmount(m_eWhichPeriod), IDS_EMPLOYEEWAGES)
	DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::YOUTHWAGES).GetAmount(m_eWhichPeriod), IDS_YOUTHWAGES)
	DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::RUNNINGCOSTS).GetAmount(m_eWhichPeriod), IDS_RUNNINGCOSTS)       // Running costs

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::LOANRECEIVED).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_LOANRECEIVED)     // Loan repayments
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::LOANPAID).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_LOANREPAY)     // Loan repayments
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TRANSFERINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_TRANSFERINCOME)    //Transfer income
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TRANSFEREXPENDITURE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_TRANSFEREXPEND) //Transfer expenditure
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALEXPENDITURE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_GENERALEXPEND)
	}
	
	m_Grid.DoAddRows(NumFound + 2);
	m_Grid.SetColumnTextColour(2, RED);
	CAccountsGridItem* pItem = &theList[0];
	for (int LoopCount = 0; LoopCount < NumFound; LoopCount++)
	{
		m_Grid.SetItemText(LoopCount, 0, pItem->m_Description);
		
		if (pItem->m_IncomeStr.IsEmpty() == false)
		{
			m_Grid.SetItemText(LoopCount, 1, pItem->m_IncomeStr);
		}
		if (pItem->m_ExpenditureStr.IsEmpty() == false)
		{
			m_Grid.SetItemText(LoopCount, 2, pItem->m_ExpenditureStr);
		}
		pItem++;
	}

	CExtString strPrint;
	// Totals
	m_Grid.SetItemStrId(NumFound, 0, IDS_TOTALS);

	strPrint.FormatLargeNumber(Income);
	m_Grid.SetItemText(NumFound, 1, strPrint);

	strPrint.FormatLargeNumber(static_cast<int>(Expenditure));
	m_Grid.SetItemText(NumFound, 2, strPrint);

	// Week's Profit or loss
	strPrint.FormatLargeNumber(abs(Income - Expenditure));
	if (Income - Expenditure > 0)
	{
		m_Grid.SetItemStrId(NumFound + 1, 0, IDS_PROFIT);
		m_Grid.SetItemText(NumFound + 1, 1, strPrint);
	}
	else
	{
		m_Grid.SetItemStrId(NumFound + 1, 0, IDS_LOSS);
		m_Grid.SetItemText(NumFound + 1, 2, strPrint);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnPaint()
{
	CBitmapFormView::OnInitRedrawArea();

	m_Grid.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen26View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen26View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theWindow;
		GetClientRect(&theWindow);
		int RowsToDisplay = (theWindow.bottom - 56 - 20) / m_Grid.GetRowHeight();
		CRect theRect(theWindow.right / 2 - 118, 56, theWindow.right / 2 + 118, 56 + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);

		// Club Balance
		theRect.top = theRect.bottom + 2;
		theRect.left = GetCentreX() - 118;
		theRect.right = theRect.left + 148;
		theRect.bottom = theRect.top + 18;
		m_ClubBalanceText.MoveWindow(&theRect);
	}
	Sleep(1);
}