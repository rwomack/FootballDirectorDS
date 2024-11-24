
/*

	Football Director

	Copyright 1994-2009 Sports Director Ltd. All rights reserved. 

	---------------------------------------------------------------------------------------
*/

#ifndef __LEAGUECLUB_H__
#define __LEAGUECLUB_H__


class CLeagueClub
{
private:

public:
	CLeagueClub();
	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoAddTransferExpenditure(const uint _Cost);
	void					DoAddTransferIncome(const int _Cost);
	void					DoBidsThisWeekAdd(const byte _HowMuch);
	void					DoCashBalanceAdd(const uint _HowMuch);
	void					DoCashBalanceSub(const uint _HowMuch);
	void					DoFinancialLoanAccepted(const uint HowMuch);
	void					DoRepayAllLoan();
	void					DoRepayLoan(const uint HowMuch);
	void					DoUserRepayFinancialLoan(const uint dValue);
	CAccountsItemList&		GetAccounts();
	byte					GetBidsThisWeek();
	int						GetCashBalance();
	CClubHistory&			GetClubHistory();
	CCurrentForm&			GetCurrentForm();
	CEmployeeList&          GetEmployeeList();
	CFinancialLoan&			GetFinancialLoan();
	CLeagueTable&			GetLeagueTable();
	CManagerContractOffer&	GetManagerContract();
	CClubMatchDetails&		GetMidWeekMatch();
	CClubSeasonStats&		GetSeasonStats();
	CSquadTraining&   		GetSquadTrainingDetails();
	CClubMatchDetails&		GetWeekEndMatch();
	CYouthList&             GetYouthSquad();

	void					SetBidsThisWeek(const byte _Value);
	void					SetCashBalance(const uint _Value);
	void					SetTicketPrice(const uint _Value);
	void					SetYearFounded(const ushort _Value);

#ifdef TICKETPRICECOMPENSATOR
	uint					GetTicketPrice();
#endif
#ifdef REFUSEDDEALS
	CRefusedDealList&		GetRefusedDeals();
#endif
#ifdef USECLUBYEARFOUNDED
	ushort					GetYearFounded();
#endif
#ifdef ACTIVEFRIENDLIES
	CClubFriendlies&		GetFriendlies();
#endif

	// Variables
private:
	int						m_CashBalance;
#ifdef TICKETPRICECOMPENSATOR
	uint					m_TicketPrice;
#endif
	byte					m_BidsThisWeek;
	CEmployeeList           m_EmployeeList;
	CSquadTraining   		m_SquadTrainingDetails;	// 6 Bytes
	CYouthList              m_YouthSquad;			// 16 Bytes
	CFinancialLoan			m_FinancialLoan;		// 12 bytes
	CAccountsItemList		m_Accounts;				// 4 Bytes
	CManagerContractOffer	m_ManagerContract;		// 5 bytes
	CClubMatchDetails		m_WeekEndMatch;			// 8 Bytes
	CClubMatchDetails		m_MidWeekMatch;			// 8 Bytes
	CClubSeasonStats		m_SeasonStats;			// 11 bytes
	CClubHistory			m_ClubHistory;
	CLeagueTable			m_LeagueTable;
	CCurrentForm			m_CurrentForm;

#ifdef REFUSEDDEALS
	CRefusedDealList		m_RefusedDeals;			// 16 Bytes
#endif
#ifdef USECLUBYEARFOUNDED
	ushort					m_YearFounded;
#endif
#ifdef ACTIVEFRIENDLIES
	CClubFriendlies			m_Friendlies;
#endif
#ifdef TROPHYHISTORY
	CTrophyHistoryList		m_TrophyHistory;
#endif
#ifdef PLAYERTRANSFERHISTORY
	CTransferHistoryList    m_PlayersBought;
	CTransferHistoryList    m_PlayersSold;
#endif
#ifdef	MATCHTRACKING
	CNumericArray<uint>		m_GameAttendance;
	CNumericArray<uint>		m_GateReceipts;
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CLeagueClubList : public CExtArray
{
public:
	// Constructor
	CLeagueClubList() : m_pList(NULL) {};
	~CLeagueClubList();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	CLeagueClub&			operator [](ushort x);
	void					DoInitialise(const ushort _HowMany);

	// Variables
private:
	CLeagueClub*			m_pList;
};
#endif
