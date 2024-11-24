
 /*

    LeagueClub.cpp: implementation of the CLeagueClub class.


	Football Director

	Copyright 1994-2009 Sports Director Ltd. All rights reserved. 

	---------------------------------------------------------------------------------------
 */

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

	const int         		TRANSFERFEETAXRATE = 33;


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::CLeagueClub
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CLeagueClub::CLeagueClub()
	: m_CashBalance(0)
#ifdef TICKETPRICECOMPENSATOR
	, m_TicketPrice(0) 
#endif
	, m_BidsThisWeek(0)
{
	m_ManagerContract.DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoAddTransferExpenditure
	Access:    	public 
	Parameter: 	const uint _Cost
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoAddTransferExpenditure(const uint _Cost)
{
	DoCashBalanceSub(_Cost);
	GetAccounts().GetAt(CAccountsItemList::TRANSFEREXPENDITURE).Plus(_Cost);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoAddTransferIncome
	Access:    	public 
	Parameter: 	const int _Cost
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoAddTransferIncome(const int _Cost)
{
	int TaxPaid = (_Cost / 100) * TRANSFERFEETAXRATE;
	DoCashBalanceAdd(_Cost - TaxPaid);
	GetAccounts().GetAt(CAccountsItemList::TRANSFERINCOME).Plus(_Cost - TaxPaid);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoBidsThisWeekAdd
	Access:    	public 
	Parameter: 	const byte _HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoBidsThisWeekAdd(const byte _HowMuch)
{
	m_BidsThisWeek += _HowMuch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoCashBalanceAdd
	Access:    	public 
	Parameter: 	const uint _HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoCashBalanceAdd(const uint _HowMuch)
{
	EXT_ASSERT(_HowMuch > 0);
	m_CashBalance += _HowMuch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoCashBalanceSub
	Access:    	public 
	Parameter: 	const uint _HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoCashBalanceSub(const uint _HowMuch)
{
	EXT_ASSERT(_HowMuch > 0);
	m_CashBalance -= _HowMuch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoFinancialLoanAccepted
	Access:    	public 
	Parameter: 	const uint HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoFinancialLoanAccepted(const uint HowMuch)
{
	DoCashBalanceAdd(HowMuch);
	GetAccounts().GetAt(CAccountsItemList::LOANRECEIVED).Plus(HowMuch);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRepayAllLoan
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoRepayAllLoan()
{
	DoCashBalanceSub(GetFinancialLoan().GetLoanOwed());
	GetAccounts().GetAt(CAccountsItemList::LOANPAID).Plus(GetFinancialLoan().GetLoanOwed());
	GetFinancialLoan().DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoRepayLoan
	Access:    	public 
	Parameter: 	uint HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoRepayLoan(const uint HowMuch)
{
	if (HowMuch > 0)
	{
		uint dAmountRepaid = GetFinancialLoan().DoRepayLoan(HowMuch);
		DoCashBalanceSub(dAmountRepaid);
		GetAccounts().GetAt(CAccountsItemList::LOANPAID).Plus(dAmountRepaid);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::DoUserRepayFinancialLoan
	Access:    	public 
	Parameter: 	const uint dValue
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::DoUserRepayFinancialLoan(const uint dValue)
{
	DoCashBalanceSub(GetFinancialLoan().DoRepayLoan(dValue));
	GetAccounts().GetAt(CAccountsItemList::LOANPAID).Plus(dValue);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetAccounts
	Access:    	public 
	Returns:   	CAccountsItemList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItemList& CLeagueClub::GetAccounts()
{
	return m_Accounts;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetBidsThisWeek
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueClub::GetBidsThisWeek()
{
	return m_BidsThisWeek;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetCashBalance
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CLeagueClub::GetCashBalance()
{
	return m_CashBalance;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetClubHistory
	Access:    	public 
	Returns:   	CClubHistory&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubHistory& CLeagueClub::GetClubHistory()
{
	return m_ClubHistory;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetCurrentForm
	Access:    	public 
	Returns:   	CCurrentForm&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCurrentForm& CLeagueClub::GetCurrentForm()
{
	return m_CurrentForm;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetEmployeeList
	Access:    	public 
	Returns:   	CEmployeeList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeList& CLeagueClub::GetEmployeeList()
{
	return m_EmployeeList;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetFinancialLoan
	Access:    	public 
	Returns:   	CFinancialLoan&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFinancialLoan& CLeagueClub::GetFinancialLoan()
{
	return m_FinancialLoan;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetLeagueTable
	Access:    	public 
	Returns:   	CLeagueTable&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTable& CLeagueClub::GetLeagueTable()
{
	return m_LeagueTable;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetManagerContract
	Access:    	public 
	Returns:   	CManagerContractOffer&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerContractOffer& CLeagueClub::GetManagerContract()
{
	return m_ManagerContract;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetMidWeekMatch
	Access:    	public 
	Returns:   	CClubMatchDetails&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubMatchDetails& CLeagueClub::GetMidWeekMatch()
{
	return m_MidWeekMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetSeasonStats
	Access:    	public 
	Returns:   	CClubSeasonStats&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubSeasonStats& CLeagueClub::GetSeasonStats()
{
	return m_SeasonStats;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetSquadTrainingDetails
	Access:    	public 
	Returns:   	CSquadTraining&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSquadTraining& CLeagueClub::GetSquadTrainingDetails()
{
	return m_SquadTrainingDetails;
}


#ifdef TICKETPRICECOMPENSATOR
/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetTicketPrice
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CLeagueClub::GetTicketPrice()
{
	return m_TicketPrice;
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetWeekEndMatch
	Access:    	public 
	Returns:   	CClubMatchDetails&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubMatchDetails& CLeagueClub::GetWeekEndMatch()
{
	return m_WeekEndMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetYouthSquad
	Access:    	public 
	Returns:   	CYouthList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CYouthList& CLeagueClub::GetYouthSquad()
{
	return m_YouthSquad;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::SetBidsThisWeek
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::SetBidsThisWeek(const byte _Value)
{
	m_BidsThisWeek = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::SetCashBalance
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::SetCashBalance(const uint _Value)
{
	m_CashBalance = _Value;
}


#ifdef TICKETPRICECOMPENSATOR
/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::SetTicketPrice
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::SetTicketPrice(const uint _Value)
{
	m_TicketPrice = _Value;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::SetYearFounded
	Access:    	public 
	Parameter: 	const ushort _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::SetYearFounded(const ushort _Value)
{
	m_YearFounded = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->Write(&m_CashBalance, sizeof(m_CashBalance));
#ifdef TICKETPRICECOMPENSATOR
	_pDataFile->Write(&m_TicketPrice, sizeof(m_TicketPrice));
#endif
	m_EmployeeList.Write(_pDataFile);
	m_SquadTrainingDetails.Write(_pDataFile);
	m_YouthSquad.Write(_pDataFile);
	_pDataFile->Write(&m_FinancialLoan, sizeof( m_FinancialLoan));
	m_Accounts.Write(_pDataFile);
	_pDataFile->Write(&m_WeekEndMatch, sizeof(m_WeekEndMatch));
	_pDataFile->Write(&m_MidWeekMatch, sizeof(m_MidWeekMatch));
	_pDataFile->Write(&m_SeasonStats, sizeof(CClubSeasonStats));
	m_ClubHistory.Write(_pDataFile);
	m_LeagueTable.Write(_pDataFile);
	m_CurrentForm.Write(_pDataFile);
#ifdef REFUSEDDEALS
	m_RefusedDeals.Write(_pDataFile);
#endif
#ifdef ACTIVEFRIENDLIES
	_pDataFile->Write(&m_Friendlies, sizeof(CClubFriendlies));
#endif
#ifdef PLAYERTRANSFERHISTORY
	m_PlayersBought.Write(_pDataFile);
	m_PlayersSold.Write(_pDataFile);
#endif
#ifdef TROPHYHISTORY
	m_TrophyHistory.Write(_pDataFile);
#endif
#ifdef	MATCHTRACKING
	m_GameAttendance.Write(_pDataFile);
	m_GateReceipts.Write(_pDataFile);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClub::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->Read(&m_CashBalance, sizeof(m_CashBalance));
#ifdef TICKETPRICECOMPENSATOR
	_pDataFile->Read(&m_TicketPrice, sizeof(m_TicketPrice));
#endif
	m_EmployeeList.Read(_pDataFile);
	m_SquadTrainingDetails.Read(_pDataFile);
	m_YouthSquad.Read(_pDataFile);
	_pDataFile->Read(&m_FinancialLoan, sizeof(m_FinancialLoan));
	m_Accounts.Read(_pDataFile);
	_pDataFile->Read(&m_WeekEndMatch, sizeof(m_WeekEndMatch));
	_pDataFile->Read(&m_MidWeekMatch, sizeof(m_MidWeekMatch));
	_pDataFile->Read(&m_SeasonStats, sizeof(CClubSeasonStats));
	m_ClubHistory.Read(_pDataFile);
	m_LeagueTable.Read(_pDataFile);
	m_CurrentForm.Read(_pDataFile);
#ifdef REFUSEDDEALS
	m_RefusedDeals.Read(_pDataFile);
#endif
#ifdef ACTIVEFRIENDLIES
	_pDataFile->Read(&m_Friendlies, sizeof(CClubFriendlies));
#endif
#ifdef PLAYERTRANSFERHISTORY
	m_RefusedDeals.Read(_pDataFile);
#endif
#ifdef PLAYERTRANSFERHISTORY
	m_PlayersBought.Read(_pDataFile);
	m_PlayersSold.Read(_pDataFile);
#endif
#ifdef TROPHYHISTORY
	m_TrophyHistory.Read(_pDataFile);
#endif
#ifdef	MATCHTRACKING
	m_GameAttendance.Read(_pDataFile);
	m_GateReceipts.Read(_pDataFile);
#endif
	m_BidsThisWeek = 0;
}


#ifdef REFUSEDDEALS
/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetRefusedDeals
	Access:    	public 
	Returns:   	CRefusedDealList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRefusedDealList& CLeagueClub::GetRefusedDeals()
{
	return m_RefusedDeals;
}
#endif
#ifdef USECLUBYEARFOUNDED
/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetYearFounded
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeagueClub::GetYearFounded()
{
	return m_YearFounded;
}
#endif
#ifdef ACTIVEFRIENDLIES
/*------------------------------------------------------------------------------
	Method:   	CLeagueClub::GetFriendlies
	Access:    	public 
	Returns:   	CClubFriendlies&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubFriendlies& CLeagueClub::GetFriendlies()
{
	return m_Friendlies;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CLeagueClubList::~CLeagueClubList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueClubList::~CLeagueClubList()
{
	delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClubList::operator[]
	Access:    	public 
	Parameter: 	ushort x
	Returns:   	CLeagueClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueClub& CLeagueClubList::operator[](ushort x)
{
	EXT_ASSERT(x < GetSize());
	return static_cast<CLeagueClub&> (m_pList[x]);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClubList::DoInitialise
	Access:    	public 
	Parameter: 	const ushort _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClubList::DoInitialise(const ushort _HowMany)
{
	CExtArray::SetSize(_HowMany);
	EXT_ASSERT(CExtArray::GetSize() > 0);
	m_pList = new CLeagueClub[CExtArray::GetSize()];
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClubList::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClubList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_pList[ListPos].Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueClubList::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueClubList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_pList[ListPos].Read(_pDataFile);
	}
}

