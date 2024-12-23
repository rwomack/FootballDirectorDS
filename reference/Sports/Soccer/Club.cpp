
 /*

        Class:- club

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#include "UI.h"
#ifndef TESTBED
#include "MessageDisplay.h"
#endif
#include "IndexedIntArray.h"
#include "MatchEvent.h"
#include "CurrentForm.h"
#include "MatchEvent.h"
#include "CharityShield.h"
#include "PlayerSearch.h"
#include "LeadingGoalscorer.h"
#include "LeadingDisciplinaryPoints.h"
#include "BasicEmployee.h"
#include "MatchInfo.h"
#include "FootballDirector.h"


	const int				ACCEPTMANAGERJOB = 3;
	const int				BID1STCHANCE = 10;
	const int				BIDATTEMPTS = 2;				// Number of attempts a non managed club has to buy a player in one turn
	const int				BUYFROMABROADCHANCE = 14;		// %age chance of buying a player from another country
	const int				CHANCEBIGBID = 6;				// Double bid for player chance
    const int				CHANCEBUYTWOPLAYERS = 20;		// Has just bought a player, if = 1 then may bid for another player
	const int				CHANCEBOARDSELLPLAYER = 4;		// If the club is ushort of money the board may sell players
	const int				CHANCENOTEXTENDPLAYERLOANPERIOD = 2;
	const int				CHANCENOTRENEWMANAGERCONTRACT = 2;
	const int				CHANCEREFUSECOUNTER = 5;		// Terminate transfer offer
	const int				CHANCERETIREPLAYERSTHROUGHINJURY = 100;
	const int				CHANCESELLTEAMPLAYER = 3;
	const int				CHANCETRYHIREMANAGER = 4;
	const int				CHANCETRYBID = 50;				// If = 1 then non managed club will try to buy a player
	const int				CHANCEWARNDISCIPLINARY = 6;
	const int				CHANCEWARNSMALLSQUAD = 18;		// Generates warning if squad low
	const int				CHANCEWARNPLAYERTIRED = 8;		// Generates warning if player fitness low
	const int				MANAGERSACKCHANCE	= 400;
	const int				MANAGERSACKIFPOORLEAGUEPOSITION = 9;
	const int				MINGAMESBEFORESACKING = 10;
	const int				NEARENDOFSEASON = 10;
	const int				OFFERCOACHMANAGERJOB = 3;
	const int				RETIREPLAYERSTHROUGHINJURYATTEMPTS = 3;
	const byte				TOTALTRAININGHOURS = 40;
	const int				WARNENERGYLOW = 40;
	#define					TRANSFERFEETAXRATE .67


/*------------------------------------------------------------------------------
	Method:   	CClub::CClub
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub::CClub()
	: m_pAccounts(null)
	, m_pCurrentForm(null)
	, m_StadiumID(0)
	, m_CurrentTacticsID(0)
	, m_ManagerID(NOMANAGER)
	, m_RegionID(0)
	, m_PopularityID(0)
	, m_TeamMorale(0)
	, m_TicketPrice(0.0)
	, m_CashBalance(0)
	, m_DivisionID(NODIVISION)
	, m_BidsThisWeek(0)
	, m_HomeKitID(0)
	, m_Temperament(0)
{
	GetTeamTactic().SetDefaults();
	m_ManagerContract.DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::~CClub
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub::~CClub()
{
    if (IsRanking0Club())
	{
		TurnRanking0Off();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoInitialiseFromResource
	Access:    	public
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	CExtString strName;
	strName.ReadFromResource(_Resource);
	m_NameID.DoFindID(strName);
#ifdef USELONGCLUBNAME
    m_LongClubName.Read(File);
#endif
    m_StadiumID = _Resource.ReadUInt16();
    m_PopularityID =  WorldData().GetPopularityList().DoCalculateID(_Resource.ReadUInt32());
    m_RegionID = _Resource.ReadByte();
    m_DivisionID = _Resource.ReadByte();
	if (IsRanking0Club())
	{
		TurnRanking0On();
#ifdef ACTIVEFRIENDLIES
		GetFriendlies().OnNewSeason();
#endif
	}
	if(IsNonLeagueClub() == false)
	{
		DoCalculateManagerContractOffer();
	    m_FinancialLoan.DoResetVariables();
		DoClearTable();
		m_TableData.DoResetVariables();
	}

    m_ManagerID = _Resource.ReadUInt16();
	if (m_ManagerID != NOMANAGER)
	{
		m_ManagerContract.SetContractOffer(1 + RandomNumber.IntLessThan(4));
		GetManager().DoNewClub(*this);
        m_ManagerContract.DoResetVariables();
    }
	m_HomeKitID = _Resource.ReadByte();

	strName.ReadFromResource(_Resource);
	m_NicknameID.DoFindID(strName);
#ifdef USECLUBYEARFOUNDED
    m_YearFounded = _Resource.ReadUInt16();
#endif

	DoSelectRandomTactics();
    m_TeamMorale = 90 - m_PopularityID;				// Good clubs start off with higher popularity

	// Financial matters
    m_CashBalance = GetPopularity().GetStartingClubBalance(GetCountry().GetFinancialRatioPercentage());
	DoInitialiseTicketPricesAndBonuses();

// 	DEBUG_OUTPUT(("%s"), (GetNickname()));
// 	DEBUG_OUTPUT(("%s"), (GetName()));
// 	DEBUG_OUTPUT(("Kit ID %d"), (m_HomeKitID));
// 	DEBUG_OUTPUT(("Year Founded %d"), (m_YearFounded));
//	DEBUG_OUTPUT(("%s  %s, Kit ID %d : Year Founded %d"), (GetName(), GetNickname(), m_HomeKitID, m_YearFounded));
//	DEBUG_OUTPUT(("%s, %d, %s, %d"), (GetName(), m_StadiumID, GetStadium()->GetName(), GetStadium()->GetCapacity()));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoBuildLeagueTableRow
	Access:    	public
	Parameter: 	const ushort _Row
	Parameter: 	CGrid & _Grid
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CClub::DoBuildLeagueTableRow(const ushort _Row, CGrid& _Grid)
{
	_Grid.SetRowPointerDetails(_Row, this);
	_Grid.SetItemText(_Row, 0, GetName());
	GetTableData().DoBuildLeagueTableRow(_Row, _Grid);
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CClub::GetClubHistory
	Access:    	public
	Returns:   	CClubHistory&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClubHistory& CClub::GetClubHistory()
{
	return m_ClubHistory;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetHomeKitID
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetHomeKitID()
{
	return WorldData().GetKitSystem()[m_HomeKitID].GetID();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetHomeKit
	Access:    	public
	Returns:   	CKit&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CKit& CClub::GetHomeKit()
{
	return WorldData().GetKitSystem()[m_HomeKitID];
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetID
	Access:    	public
	Parameter: 	ushort _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetID(ushort _Value)
{
	m_ID = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCalculateManagerContractOffer
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoCalculateManagerContractOffer()
{
	m_ManagerContract.DoCalculateOffer(GetCountry().GetCurrency().RoundedPounds(GetPopularity().GetManagerAnnualWageOffer(GetCountry().GetFinancialRatioPercentage())),
			2 + RandomNumber.IntLessThan(4));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetName
	Access:    	public
	Parameter: 	void
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CClub::GetName()
{
	return m_NameID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetNickname
	Access:    	public
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CClub::GetNickname()
{
	return m_NicknameID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetUpperCaseName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetUpperCaseName()
{
	CString str;
	str = GetName();
	str.MakeUpper();
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetNameID
	Access:    	public
	Parameter: 	void
	Returns:   	CStringID&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CStringID& CClub::GetNameID()
{
	return m_NameID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetLongClubName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetLongClubName()
{
	return GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetMorale
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetMorale() const
{
	return m_TeamMorale;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetFinancialLoan
	Access:    	public
	Parameter: 	void
	Returns:   	CFinancialLoan*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFinancialLoan* CClub::GetFinancialLoan()
{
	return &m_FinancialLoan;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetWeekEndMatchGateReceipts
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetMidWeekMatchGateReceipts()
{
	return m_MidWeekMatch.GetGateReceipts();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::pFinancialLoan
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetWeekEndMatchGateReceipts()
{
	return m_WeekEndMatch.GetGateReceipts();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSelectTactics
	Access:    	public
	Parameter: 	CClub* _pOpponentClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSelectTactics(CClub* _OpponentClub)
{
	EXT_ASSERT(_OpponentClub != null && IsUserControlled() == false);
	//afxDump << GetName() << " Suspended Players : " << m_squadFirstTeamPlayers.CountSuspendedPlayers() << "\n";
	ushort nSuspendedPlayers = m_squadFirstTeamPlayers.DoCountSuspendedPlayers();
	if (nSuspendedPlayers > 2)
	{
		// Too many players suspended, so cool it
		GetTeamTactic().SetAggression(2);
	}
	else if (WorldData().GetMatchHandler().IsCupMatch() == true && WorldData().GetMatchHandler().GetCupRound()->GetNumInRound() <= 4)
	{
		// Semi or Final of cup match
		GetTeamTactic().SetAggression(5 + RandomNumber.IntLessThan(3));
	}
	else if (_OpponentClub->GetPopularityID() < m_PopularityID + 3)
	{
		GetTeamTactic().SetAggression(3 + RandomNumber.IntLessThan(2));
	}
	else if (_OpponentClub->GetPopularityID() > m_PopularityID + 3 && nSuspendedPlayers == 0)
	{
		GetTeamTactic().SetAggression(6 + RandomNumber.IntLessThan(2));
	}
	else
	{
		GetTeamTactic().SetAggression(4);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTeamTactic
	Access:    	public
	Parameter: 	void
	Returns:   	CTeamTactic&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CClub::GetTeamTactic()
{
	if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetCurrentTeamTactic();
	}
	return m_TeamTactic;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRepayAllLoan
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Repay all of loan
------------------------------------------------------------------------------*/
void CClub::DoRepayAllLoan()
{
	EXT_ASSERT(m_FinancialLoan.GetLoanOwed() > 0);
	if (UserMessageBox(IDS_CONFFULLREPAY, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		m_CashBalance -= m_FinancialLoan.GetLoanOwed();
		if (IsRanking0Club())
		{
			m_pAccounts->GetAt(CAccountsItemList::LOANPAID).Plus(m_FinancialLoan.GetLoanOwed());
		}
		m_FinancialLoan.DoResetVariables();
	}
 }

/*------------------------------------------------------------------------------
	Method:   	CClub::GetForeignStr
	Access:    	public
	Parameter: 	int _Amount
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetForeignStr(int _Amount)
{
	return GetCurrency().GetForeignStr(_Amount);

}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCashStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetCashStr()
{
	return GetForeignStr(m_CashBalance);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsRanking0Club
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsRanking0Club()
{
	if (IsNonLeagueClub() == true)
	{
		return false;
	}
	if (GetCountryRanking() == 0)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoInitialiseFromResource
	Access:    	public
	Parameter: 	const CString & _szName
	Parameter: 	CBinaryReader & _Resource
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:	Used for non league clubs in cup competitions
------------------------------------------------------------------------------*/
void CClub::DoInitialiseFromResource(const CString& _szName, CBinaryReader& _Resource, CCountry& _Country)
{
	m_NameID.DoFindID(_szName);
    m_StadiumID = _Resource.ReadUInt16();
	DoSelectRandomTactics();
	m_squadFirstTeamPlayers.DoGenerateNonLeagueSquad(24, _Country);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TotalStaff
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::TotalStaff()
{
	return m_StaffList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TotalYouths
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::TotalYouths()
{
	return m_YouthSquad.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTicketPrice
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetTicketPrice()
{
	return m_TicketPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCashBalance
	Access:    	public const
	Parameter: 	void
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CClub::GetCashBalance() const
{
	return m_CashBalance;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetDivisionID
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetDivisionID() const
{
	return m_DivisionID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetRegionID
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetRegionID() const
{
	return m_RegionID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetStadiumID
	Access:    	public const
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetStadiumID() const
{
	return m_StadiumID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AddYouthToSquad
	Access:    	public
	Parameter: 	CYouth* AvailablepYouth
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAddYouthToSquad(CYouth* _AvailableYouth)
{
	EXT_ASSERT(_AvailableYouth != null);
	m_YouthSquad.Add(_AvailableYouth);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TurnRanking0Off
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TurnRanking0Off()
{
	delete m_pAccounts;
	delete m_pCurrentForm;
#ifdef	MATCHTRACKING
	TurnMatchTrackingOff();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TurnRanking0On
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TurnRanking0On()
{
	TurnAccountsOn();
	m_pCurrentForm = new CCurrentForm();
#ifdef	MATCHTRACKING
	InitialiseMatchTracking();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::MakeCountryRanking0
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::MakeCountryRanking0 ()
{
	EXT_ASSERT(GetCountryRanking() == 0);
	TurnAccountsOn();
	m_pCurrentForm = new CCurrentForm();
}



/*------------------------------------------------------------------------------
	Method:   	CClub::GetCurrentForm
	Access:    	public
	Parameter: 	void
	Returns:   	CCurrentForm*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCurrentForm* CClub::GetCurrentForm()
{
	EXT_ASSERT(GetCountryRanking() == 0);
	return m_pCurrentForm;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetDivision
	Access:    	public
	Parameter: 	void
	Returns:   	CDivision&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CClub::GetDivision()
{
	EXT_ASSERT(IsNonLeagueClub() != true);
	return WorldData().GetDivisionList().GetAt(m_DivisionID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetDivisionStrengthID
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetDivisionStrengthID()
{
	return GetDivision().GetDivisionStrengthID();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCountryID
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetCountryID()
{
	return GetDivision().GetCountryID();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCountryRanking
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetCountryRanking()
{
	return GetCountry().GetRanking();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPopularity
	Access:    	public
	Parameter: 	void
	Returns:   	CPopularity&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPopularity& CClub::GetPopularity()
{
	return WorldData().GetPopularityList()[m_PopularityID];
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetStadium
	Access:    	public
	Parameter: 	void
	Returns:   	CStadium*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CStadium* CClub::GetStadium()
{
	return WorldData().GetStadium(m_StadiumID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoPreMatchReport
	Access:    	public
	Parameter: 	CMessageDisplay* pMessageDisplay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CClub::DoPreMatchReport(CMessageDisplay* _pMessageDisplay)
{
	CString szStr;
	CPlayer* pLeadPlayer1 = null;
	CPlayer* pLeadPlayer2 = null;
	CPlayer* pLeadAssistPlayer1 = null;
	CPlayer* pLeadAssistPlayer2 = null;
	CPlayer* pLeadFoulPlayer1 = null;
	CPlayer* pLeadFoulPlayer2 = null;

	CExtString str;

#ifdef TEST_MATCH_REPORT
	pLeadPlayer1 = &m_squadFirstTeamPlayers.GetPlayer(1);
	pLeadPlayer2 = &m_squadFirstTeamPlayers.GetPlayer(2);
	szStr.Format(IDS_SCOREDBETWEENTHEM, pLeadPlayer1->GetName(), pLeadPlayer2->GetName(), 9999);
	// Report if either is injured or suspended
	pLeadPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
	pLeadPlayer2->PreMatchAvailabilityReport(_pMessageDisplay);
	_pMessageDisplay->AddString(szStr);

	szStr.Format(IDS_HASSCORED, pLeadPlayer1->GetName(), 10);
	_pMessageDisplay->AddString(szStr);

	pLeadFoulPlayer1 = &m_squadFirstTeamPlayers.GetPlayer(3);
	pLeadFoulPlayer2 = &m_squadFirstTeamPlayers.GetPlayer(4);
	szStr.Format(IDS_DISCPOINTSBETWEENTHEM, pLeadFoulPlayer1->GetName(), pLeadFoulPlayer2->GetName(), 999);
	pLeadFoulPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
	pLeadFoulPlayer2->PreMatchAvailabilityReport(_pMessageDisplay);

	pLeadFoulPlayer1 = &m_squadFirstTeamPlayers.GetPlayer(5);
	szStr.Format(IDS_PREDISCPOINTS, pLeadFoulPlayer1->GetName(), 66666);
	pLeadFoulPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
	_pMessageDisplay->AddString(szStr);
#else
	int iGoals;
	int iCount = GetDivision().DoCountGoodScorersForClub(*this);
	if (iCount > 0)
	{
		// if more than 1
		if (iCount > 1)
		{
			pLeadPlayer1 = m_squadFirstTeamPlayers.GetLeadingScorer();
			pLeadPlayer2 = m_squadFirstTeamPlayers.GetLeadingScorer(pLeadPlayer1);
			EXT_ASSERT(pLeadPlayer1 != pLeadPlayer2);
			iGoals = pLeadPlayer1->GetNumberGoalsThisSeason() + pLeadPlayer2->GetNumberGoalsThisSeason();
			szStr.Format(IDS_SCOREDBETWEENTHEM, pLeadPlayer1->GetName(), pLeadPlayer2->GetName(), iGoals);
			// Report if either is injured or suspended
			pLeadPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
			pLeadPlayer2->PreMatchAvailabilityReport(_pMessageDisplay);
		}
		else
		{
			pLeadPlayer1 = m_squadFirstTeamPlayers.GetLeadingScorer(null, true);
			if (pLeadPlayer1 != null && pLeadPlayer1->GetNumberGoalsThisSeason() > 1)
			{
				iGoals = pLeadPlayer1->GetNumberGoalsThisSeason();
				EXT_ASSERT(iGoals > 1);
				szStr.Format(IDS_HASSCORED, pLeadPlayer1->GetName(), iGoals);
				pLeadPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
			}
		}
		_pMessageDisplay->AddString(szStr);
	}

#ifdef ASSISTS
	iCount = GetDivision().CountGoodAssistersForClub(this);
	if (iCount > 0)
	{
		// if more than 1
		if (iCount > 1)
		{
			pLeadAssistPlayer1 = m_squadFirstTeamPlayers.GetLeadingAssister(null, true);
			CString str1 = pLeadAssistPlayer1->GetName();
			pLeadAssistPlayer2 = m_squadFirstTeamPlayers.GetLeadingAssister(pLeadAssistPlayer1, true);
			CString str2 = pLeadAssistPlayer2->GetName();
			iGoals = pLeadAssistPlayer1->GetSeasonAssist() + pLeadAssistPlayer2->GetSeasonAssist();
			EXT_ASSERT(iGoals > 1);
			szStr.Format(IDS_ASSISTEDBETWEENTHEM, str1, str2, iGoals);
			if (pLeadAssistPlayer1 != pLeadPlayer1 && pLeadAssistPlayer1 != pLeadPlayer2)
			{
				pLeadAssistPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
			}
			if (pLeadAssistPlayer2 != pLeadPlayer1 && pLeadAssistPlayer2 != pLeadPlayer2)
			{
				pLeadAssistPlayer2->PreMatchAvailabilityReport(_pMessageDisplay);
			}
		}
		else
		{
			pLeadAssistPlayer1 = m_squadFirstTeamPlayers.GetLeadingAssister(null, true);
			if (pLeadAssistPlayer1 != null)
			{
				iGoals = pLeadAssistPlayer1->GetSeasonAssist();
				EXT_ASSERT(iGoals > 1);
				szStr.Format(IDS_ASSISTED, pLeadAssistPlayer1->GetName(), iGoals);
				if (pLeadAssistPlayer1 != pLeadPlayer1 && pLeadAssistPlayer1 != pLeadPlayer2)
				{
					pLeadAssistPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
				}
			}
		}
		_pMessageDisplay->AddString(szStr);
	}
#endif

	iCount = GetDivision().CountGoodFoulersForClub(this);
	if (iCount > 0)
	{
		// if more than 1
		if (iCount > 1)
		{
			pLeadFoulPlayer1 = m_squadFirstTeamPlayers.GetLeadingFouler();
			CString str1 = pLeadFoulPlayer1->GetName();
			pLeadFoulPlayer2 = m_squadFirstTeamPlayers.GetLeadingFouler(pLeadFoulPlayer1);
			CString str2 = pLeadFoulPlayer2->GetName();
			EXT_ASSERT(pLeadFoulPlayer1 != pLeadFoulPlayer2);
			iGoals = pLeadFoulPlayer1->GetDiscipline().GetDisciplinaryPoints() + pLeadFoulPlayer2->GetDiscipline().GetDisciplinaryPoints();
			szStr.Format(IDS_DISCPOINTSBETWEENTHEM, str1, str2, iGoals);
			if (pLeadFoulPlayer1 != pLeadPlayer1 && pLeadFoulPlayer1 != pLeadPlayer2 &&
				pLeadFoulPlayer1 != pLeadAssistPlayer1 && pLeadFoulPlayer1 != pLeadAssistPlayer2)
			{
				pLeadFoulPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
			}
			if (pLeadFoulPlayer2 != pLeadPlayer1 && pLeadFoulPlayer2 != pLeadPlayer2 &&
				pLeadFoulPlayer2 != pLeadAssistPlayer1 && pLeadFoulPlayer2 != pLeadAssistPlayer2)
			{
				pLeadFoulPlayer2->PreMatchAvailabilityReport(_pMessageDisplay);
			}
		}
		else
		{
			pLeadFoulPlayer1 = m_squadFirstTeamPlayers.GetLeadingFouler();
			iGoals = pLeadFoulPlayer1->GetDiscipline().GetDisciplinaryPoints();
			szStr.Format(IDS_PREDISCPOINTS, pLeadFoulPlayer1->GetName(), iGoals);
			if (pLeadFoulPlayer1 != pLeadPlayer1 && pLeadFoulPlayer1 != pLeadPlayer2 &&
				pLeadFoulPlayer1 != pLeadAssistPlayer1 && pLeadFoulPlayer1 != pLeadAssistPlayer2)
			{
				pLeadFoulPlayer1->PreMatchAvailabilityReport(_pMessageDisplay);
			}
		}
		_pMessageDisplay->AddString(szStr);
	}
	// Report if any players that have played in 50% of matches are injured or suspended
#endif
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CClub::TurnAccountsOn
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TurnAccountsOn()
{
	m_pAccounts = new CAccountsItemList();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTableData
	Access:    	public
	Parameter: 	void
	Returns:   	CLeagueTable*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTable& CClub::GetTableData()
{
	return m_TableData;
}


/*------------------------------------------------------------------------------
Method:   	CClub::GetCurrency
Access:    	public
Returns:   	CCurrency&
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
CCurrency& CClub::GetCurrency()
{
	return GetCountry().GetCurrency();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCurrentTacticsID
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetCurrentTacticsID()
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetTacticsInUse();
	}
	else
	{
		return m_CurrentTacticsID;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetCurrentTacticsID
	Access:    	public
	Parameter: 	const ushort _TacticsID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetCurrentTacticsID(const ushort _TacticsID)
{
    if (IsUserControlled() == true)
	{
		GetUser().GetTacticsList().SetTacticsInUse(_TacticsID);
	}
	m_CurrentTacticsID = _TacticsID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetDivision
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetDivisionID(const byte _Value)
{
	m_DivisionID = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetMorale
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetMorale(const byte _Value)
{
	m_TeamMorale = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::FormationStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::FormationStr()
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetAt(GetCurrentTacticsID()).TitleStr();
	}
	else
	{
		return GetNonUser().GetTacticsList().GetAt(GetCurrentTacticsID()).TitleStr();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TacticTitleAndFamiliarityStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::TacticTitleAndFamiliarityStr()
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetAt(GetCurrentTacticsID()).TitleAndFamiliarityStr();
	}
	else
	{
		return GetNonUser().GetTacticsList().GetAt(GetCurrentTacticsID()).TitleStr();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTactics
	Access:    	public
	Parameter: 	void
	Returns:   	CTactic&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTactic& CClub::GetTactics()
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetAt(GetUser().GetTacticsList().GetTacticsInUse());
	}
	else
	{
		return GetNonUser().GetTacticsList().GetAt(GetCurrentTacticsID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTactics
	Access:    	public
	Parameter: 	const byte x
	Returns:   	CTactic*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTactic& CClub::GetTactics(const byte _TacticsID)
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetAt(_TacticsID);
	}
	else
	{
		return GetNonUser().GetTacticsList().GetAt(_TacticsID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTacticsPlayerPositionX
	Access:    	public
	Parameter: 	int x
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int	CClub::GetTacticsPlayerPositionX(int x)
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetPlayerPositionX (x);
	}
	else
	{
		return GetNonUser().GetTacticsList().GetPlayerPositionX (x, GetCurrentTacticsID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTacticsPlayerPositionY
	Access:    	public
	Parameter: 	int x
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int	CClub::GetTacticsPlayerPositionY(int x)
{
    if (IsUserControlled() == true)
	{
		return GetUser().GetTacticsList().GetPlayerPositionY (x);
	}
	else
	{
		return GetNonUser().GetTacticsList().GetPlayerPositionY (x, GetCurrentTacticsID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WonCharityShield
	Access:    	public
	Parameter: 	CString title
	Parameter: 	int Id
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::WonCharityShield(const CString &title, int Id, bool IsCup)
{
#ifdef TROPHYHISTORY
	TrophyWinners(title, Id, IsCup);
#endif
    if (IsUserControlled() == true)
	{
		GetUser().WonCharityShield(title);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetLeaguePositionAfterMatch
	Access:    	public
	Parameter: 	void
	Returns:   	CSeasonLeagueTableData*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSeasonLeagueTableData* CClub::GetLeaguePositionAfterMatch()
{
	return  m_TableData.GetLeaguePositionAfterMatch();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetManagerContract
	Access:    	public
	Parameter: 	void
	Returns:   	CManagerContractOffer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CManagerContractOffer& CClub::GetManagerContract()
{
	return m_ManagerContract;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetYouth
	Access:    	public
	Parameter: 	ushort _ListPos
	Returns:   	CYouth*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CYouth* CClub::GetYouth(ushort _ListPos)
{
	return m_YouthSquad.GetAt(_ListPos);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetFirstTeamSquadTrainingDetails
	Access:    	public
	Parameter: 	void
	Returns:   	CSoccerSquadTraining&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSquadTraining& CClub::GetFirstTeamSquadTrainingDetails()
{
	return m_SquadTrainingDetails;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetYouthListNo
	Access:    	public
	Parameter: 	ePlayerPosition Position
	Parameter: 	int No
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetYouthListNo(ePlayerPosition Position, int No)
{
	CYouth* pYouth;
	int Count = 0;
	bool Pos = false;

    for (int LoopCount = 0 ; LoopCount < m_YouthSquad.GetSize(); LoopCount ++)
	{
        pYouth = m_YouthSquad.GetAt(LoopCount);
        if (pYouth != null)
		{
			if (Position ==	-1)
			{
				Pos = true;
			}
			else if (pYouth->GetSkills().GetPosition() == Position)
			{
				Pos = true;
			}
			else
			{
				Pos = false;
			}

			if (Pos)
			{
				if (Count == No)
				{
					return LoopCount;
				}

				Count++;
			}
        }
    }

	return NOPLAYER;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::HumanManagerWelcomeStr
	Access:    	public
	Returns:   	CExtString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CExtString CClub::DoHumanManagerWelcomeStr()
{
	EXT_ASSERT(IsUserControlled() == true);
	CExtString str;
	// Send welcome message to manager from board
    str.Format(IDS_BOARDWELCOME, GetName());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoHumanManagerWelcome
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoHumanManagerWelcome()
{
	EXT_ASSERT(IsUserControlled() == true);
	AddToClubNews(DoHumanManagerWelcomeStr());
	CExtString str;
    str.Format(IDS_NEWSMANAGERAPPOINTED, GetManagerName(), GetName());
    GetActiveCountry().AddToNewsLists(str);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::CheckHomeRecords
	Access:    	public
	Parameter: 	CFixture& aFixture
	Parameter: 	const ushort strTitleID
	Parameter: 	const ushort strVenueID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::CheckHomeRecords(CFixture& _Fixture, const ushort _strTitleID, const ushort _strVenueID)
{
	GetClubHistory().CheckIfRecordAttend(_Fixture.GetAttendance(),
										 _Fixture.GetClub(AWAY).GetNameID(), WorldData().GetCurrentDate());
    GetClubHistory().CheckIfRecordReceipt(_Fixture.GetAttendance() * _Fixture.GetTicketPrice(),
										  _Fixture.GetClub(AWAY).GetNameID(), WorldData().GetCurrentDate());

	if (_Fixture.GetNumberGoals(HOME) > _Fixture.GetNumberGoals(AWAY))
	{
		GetClubHistory().GetRecordVictory()->CheckIfRecordVictory(_Fixture.GetNumberGoals(HOME), _Fixture.GetNumberGoals(AWAY),
															   _Fixture.GetClub(AWAY).GetNameID(), _strVenueID, _strTitleID, WorldData().GetCurrentDate());
	}
	if (_Fixture.GetNumberGoals(HOME) < _Fixture.GetNumberGoals(AWAY))
	{
		GetClubHistory().GetRecordDefeat()->CheckIfRecordDefeat(_Fixture.GetNumberGoals(HOME), _Fixture.GetNumberGoals(AWAY),
															 _Fixture.GetClub(AWAY).GetNameID(), _strVenueID, _strTitleID, WorldData().GetCurrentDate());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::CheckAwayRecords
	Access:    	public
	Parameter: 	CFixture& aFixture
	Parameter: 	const ushort strTitleID
	Parameter: 	const ushort strVenueID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::CheckAwayRecords(CFixture& aFixture, const ushort strTitleID, const ushort strVenueID)
{
	if (aFixture.GetNumberGoals(HOME) > aFixture.GetNumberGoals(AWAY))
	{
		GetClubHistory().GetRecordDefeat()->CheckIfRecordDefeat(aFixture.GetNumberGoals(AWAY), aFixture.GetNumberGoals(HOME),
															 aFixture.GetClub(HOME).GetNameID(), strVenueID, strTitleID, WorldData().GetCurrentDate());
	}
	if (aFixture.GetNumberGoals(HOME) < aFixture.GetNumberGoals(AWAY))
	{
		GetClubHistory().GetRecordVictory()->CheckIfRecordVictory(aFixture.GetNumberGoals(AWAY), aFixture.GetNumberGoals(HOME),
															   aFixture.GetClub(HOME).GetNameID(), strVenueID, strTitleID, WorldData().GetCurrentDate());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::NumberForeignDigits
	Access:    	public
	Parameter: 	int Value
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CClub::NumberForeignDigits(int Value)
{
	CString str;
    str.Format(TEXT("%d"), GetCountry().GetCurrency().ConvToForeign(Value));
	return str.GetLength();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoInitialiseTicketPricesAndBonuses
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoInitialiseTicketPricesAndBonuses()
{
	m_TicketPrice = GetPopularity().GetTicketPrice();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPopularityID
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetPopularityID() const
{
	return m_PopularityID;
}


 // -----------------------------------------------------------------------
void CClub::MakeUnPopular()
{
	m_PopularityID = LOWPOPULARITY;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WonPlayoff
	Access:    	public
	Parameter: 	CPlayOff* pCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::WonPlayoff(CPlayOff* _Cup)
{
    DoRaiseMorale(20);
#ifdef TROPHYHISTORY
	TrophyWinners(_Cup->GetName(), _Cup->GetTrophyID(), true);
	if (IsUserControlled() == true)
	{
        GetUser().WonPlayoff (_Cup, GetActiveCountry().GetSeasonStr());
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::RunnersUpPlayoff
	Access:    	public
	Parameter: 	CPlayOff* pCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::RunnersUpPlayoff(CPlayOff* _Cup)
{
    DoRaiseMorale(10);
#ifdef TROPHYHISTORY
	TrophyRunnersUp(_Cup->GetName(), _Cup->GetTrophyID(), true);
	if (IsUserControlled() == true)
	{
        GetUser().RunnersUpPlayoff (_Cup, GetActiveCountry().GetSeasonStr());
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::QualifiedTopPlayOff
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoQualifiedTopPlayOff()
{
    DoRaiseMorale(20);
	if (IsUserControlled() == true)
	{
        GetUser().DoQualifiedTopPlayOff ();
	}
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().DoQualifiedTopPlayOff ();
    }
}


/*------------------------------------------------------------------------------
	Method:   	CClub::QualifiedBottomPlayOff
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoQualifiedBottomPlayOff()
{
    DoLowerMorale(20);
	if (IsUserControlled() == true)
	{
        GetUser().DoQualifiedBottomPlayOff ();
	}
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().DoQualifiedBottomPlayOff ();
    }
}


/*------------------------------------------------------------------------------
	Method:   	CClub::BeenPromotedViaPlayOff
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::BeenPromotedViaPlayOff()
{
    if (IsUserControlled() == true)
	{
        GetUser().BeenPromotedViaPlayOff();
	}
	BeenPromoted(true);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AvoidedRelegation
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::AvoidedRelegation()
{
    if (IsUserControlled() == true)
	{
        GetUser().AvoidedRelegation();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::BeenPromoted
	Access:    	public
	Parameter: 	bool AsChampions
	Returns:   	void
	Qualifier:
	Purpose:	If {AsChampions} is true then user manager will not be congratulated
------------------------------------------------------------------------------*/
void CClub::BeenPromoted(bool _AsChampions)
{
    if (IsUserControlled() == true && _AsChampions == false)
	{
        GetUser().BeenPromoted();
	}
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().BeenPromoted();
    }
	m_squadFirstTeamPlayers.BeenPromoted();

	// Adjust average attendances
    SetPopularityID(m_PopularityID - GetPopularity().GetPromotionIncrease());
	DoInitialiseTicketPricesAndBonuses();

	// Adjust ground if not user controlled
    if (IsUserControlled() == false && GetStadium()->GetCapacity() < GetPopularity().GetMaximumAttendance())
	{
#ifdef	ACTIVESTADIUMPLANS
        RaiseCapacity(GetPopularity().GetMaximumAttendance() + GetPopularity().GetMaximumAttendance() / 10);
#else
		GetStadium()->SetCapacity(GetPopularity().GetMaximumAttendance());
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetManagerID
	Access:    	public const
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetManagerID() const
{
	return m_ManagerID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetPopularityID
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetPopularityID(const byte _NewID)
{
	EXT_ASSERT(_NewID > -1);
	EXT_ASSERT(abs(m_PopularityID - _NewID) < 5); // Catch silly fluctuations
	m_PopularityID = _NewID;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::BeenRelegated
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::BeenRelegated()
{
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().BeenRelegated();
		if (IsUserControlled() == true)
		{
			GetUser().BeenRelegated();
		}
    }
	// Adjust average attendances
    SetPopularityID(m_PopularityID + GetPopularity().GetRelegationDecrease());
 	DoInitialiseTicketPricesAndBonuses();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::RunnersUp
	Access:    	public
	Parameter: 	CDivision& _Div
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::RunnersUp(CDivision& _Div)
{
#ifdef TROPHYHISTORY
	TrophyRunnersUp(_Div.GetName(), _Div.GetTrophyID(), false);
#endif
	DoRaiseMorale(20);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WonTitle
	Access:    	public
	Parameter: 	bool IsTopDivision
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::WonTitle(bool _IsTopDivision)
{
#ifdef TROPHYHISTORY
	TrophyWinners(GetDivision().GetName(), GetDivision().GetTrophyID(), false);
#endif
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().WonTitle(_IsTopDivision, GetActiveCountry().GetSeasonStr());
    }
    if (IsUserControlled() == true)
	{
		CString Str;
		Str.Format(IDS_MANAGERWONTITLE, GetActiveCountry().GetSeasonStr(), GetDivision().GetName(), GetName());
        GetUser().Champions(GetDivision(), GetActiveCountry().GetSeasonStr());
	}
    DoRaiseMorale(40);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoFinishedBottomOfDivision
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoFinishedBottomOfDivision()
{
	if (m_ManagerID != NOMANAGER)
	{
		GetManager().DoFinishedBottomOfDivision();
	}
	if (IsUserControlled() == true)
	{
		GetUser().DoFinishedBottomOfDivision();
	}
	DoLowerMorale(10);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::RunnersUp
	Access:    	public
	Parameter: 	CCup* _Cup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::RunnersUp(CCup* _Cup)
{
	EXT_ASSERT(_Cup != null);
#ifdef TROPHYHISTORY
	TrophyRunnersUp(_Cup->GetName(), _Cup->GetTrophyID(), true);
    if (IsUserControlled() == true)
	{
        GetUser().RunnersUp(_Cup, GetActiveCountry().GetSeasonStr());
	}
#endif
     DoRaiseMorale(20);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WonCup
	Access:    	public
	Parameter: 	CCup* pCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::WonCup(CCup* _Cup)
{
	EXT_ASSERT(_Cup != null);
#ifdef TROPHYHISTORY
	TrophyWinners(_Cup->GetName(), _Cup->GetTrophyID(), true);
#endif
    if (IsUserControlled() == true)
	{
        GetUser().Champions(_Cup, GetActiveCountry().GetSeasonStr());
	}
	if (m_ManagerID != NOMANAGER)
	{
		GetManager().WonCup(_Cup, GetActiveCountry().GetSeasonStr());
	}
    DoRaiseMorale(40);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::MakeUserControlled
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoMakeUserControlled()
{
	m_squadFirstTeamPlayers.DoTakeAllPlayersOffTransferList();
	m_YouthSquad.DoCreate(GetCountry(), &GetPopularity(), GetDivisionStrengthID());
	m_squadFirstTeamPlayers.DoSelectTeam(5, *this);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCountry
	Access:    	public
	Parameter: 	void
	Returns:   	CCountry&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CClub::GetCountry()
{
	return WorldData().GetCountry(GetDivision().GetCountryID());
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetActiveCountry
	Access:    	public
	Returns:   	CActiveCountry&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CActiveCountry& CClub::GetActiveCountry()
{
	return GetCountry().GetActiveCountry();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetThisWeeksLeaguePosition
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Set's the league position of the club after match[Num]
------------------------------------------------------------------------------*/
void CClub::SetThisWeeksLeaguePosition()
{
    if (GetCountryRanking() != 0 || m_TableData.GetNumberGamesPlayed() < 1)
	{
		return;
	}
	m_TableData.GetLeaguePositionAfterMatch()->SetLeaguePosition(m_TableData.GetNumberGamesPlayed() - 1, m_TableData.GetLeaguePosition());
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetTicketPriceStr
	Access:    	public
	Parameter: 	bool CurrencyStr
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetTicketPriceStr(bool CurrencyStr)
{
	CString sRet;
    if (CurrencyStr == true)
	{
		return(GetForeignStr(m_TicketPrice));
	}
	else
	{
		sRet.Format(TEXT("%d"), GetCountry().GetCurrency().ConvToForeign(m_TicketPrice));
		return sRet;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::GetLoanAmountLength
	Access:    	public
	Parameter: 	void
	Returns:   	const uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const uint CClub::GetLoanAmountLength()
{
    return GetCountry().GetCurrency().GetStrLength(static_cast<uint> (99999999));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetWageAmountLength
	Access:    	public
	Parameter: 	void
	Returns:   	const uint
	Qualifier:
	Purpose:	Returns maximum number of digits in �99,999 in the currency of the club
------------------------------------------------------------------------------*/
const uint CClub::GetWageAmountLength()
{
	return GetCountry().GetCurrency().GetStrLength(static_cast<uint> (99999));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryAcceptBidFromNonUser
	Access:    	public
	Parameter: 	CClubTransferOffer& rTheOffer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoTryAcceptBidFromNonUser(CClubTransferOffer& _TheOffer)
{
	if (WorldData().IsFastTest() == true)
	{
		return false;
	}

	if (m_BidsThisWeek > 3)
	{
		return false;
	}

    if (IsUserControlled() == true)
	{
		if (_TheOffer.GetPlayer().GetTransferStatus().IsBidAllowed(_TheOffer) == false)
		{
			return false;
		}
		// Reduce the frequency of bids
		if (m_BidsThisWeek > 0 && RandomNumber.IntLessThan(10) < 6)
		{
			return false;
		}
		m_BidsThisWeek++;

		theApp.OnReceiveOfferForPlayer(&_TheOffer);
		WorldData().RestoreProgressDuringDailyTasks();
		return _TheOffer.IsDealCompleted();
	}

	// Computer Managed Club decide whether to accept bid
 	if (m_squadFirstTeamPlayers.CanAffordToLosePlayer(&_TheOffer.GetPlayer()) == false)
	{
		if (_TheOffer.GetBidClub().IsUserControlled() == true)
		{
			CString str;
			str.Format(IDS_BIDREFUSED, _TheOffer.GetBidClub().GetName(), _TheOffer.GetPlayer().GetName(), _TheOffer.GetPlayer().GetName());
			UserMessageBox(str);
		}
		return false;
	}

	// Check if equal or greater then asking price
	if (_TheOffer.GetAmountOffered() >= _TheOffer.GetPlayer().GetTransferStatus().GetAskingPrice())
	{
        return true;
	}

	// Is less than asking, check if is a fair price
	return _TheOffer.IsFairSellingPrice(GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CClub::ManagerWeeksAtClub
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CClub::ManagerWeeksAtClub()
{
	EXT_ASSERT(m_ManagerID != NOMANAGER);
	return GetManager().GetWeeksHere();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TryAcceptTransferBid
	Access:    	public
	Parameter: 	CClubTransferOffer& rTheBid
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::TryAcceptTransferBid(CClubTransferOffer& rTheBid)
{
    if (IsUserControlled() == true)
	{
		if (rTheBid.GetAmountOffered() < max(rTheBid.GetPlayer().DoCalculateMinimumTransferFee(GetCountry()), (rTheBid.GetPlayer().GetTransferStatus().GetAskingPrice() / 100) * (75 + RandomNumber.IntLessThan(20))))
		{
			rTheBid.m_szReasonString.Format(IDS_BIDTOOLOW, GetName(), rTheBid.GetPlayer().GetName());
	        return false;
  		}
		else
		{
			theApp.OnReceiveOfferForPlayer(&rTheBid);
			return rTheBid.IsDealCompleted();
		}
    }

    rTheBid.m_szReasonString.Format(IDS_BIDREFUSED, GetName(), rTheBid.GetPlayer().GetName(), rTheBid.GetPlayer().GetName());
	ushort nPlayerID = m_squadFirstTeamPlayers.DoFindPlayerID(&rTheBid.GetPlayer());
	if (m_RefusedDeals.DoCheckIfPlayerRefusedTransfer(nPlayerID) == true || rTheBid.GetPlayer().WillingToTransfer() == false)
	{
		return false;
	}

	if (rTheBid.ShouldConsiderBidForNonListed(CHANCESELLUNLISTEDPLAYER) == false)
	{
		m_RefusedDeals.DoAddTransfer(nPlayerID);			// Won't be able to negotiate with him for a while
		return false;
	}

	// Check if moving down to a lower division
	if (rTheBid.GetPlayer().IsWillingToPlayInDivision(rTheBid.GetBidClub().GetDivisionStrengthID()) == false)
	{
		m_RefusedDeals.DoAddTransfer(nPlayerID);			// Won't be able to negotiate with him for a while
		rTheBid.m_szReasonString.Format(IDS_BIDREFUSEDWONTGOLOWERDIVISION, GetName(), rTheBid.GetPlayer().GetName());
		return false;
	}

    // Computer Managed Club decide whether to accept bid
    if (CanSparePlayer(&rTheBid.GetPlayer()) == false)
	{
		rTheBid.m_szReasonString.Format(IDS_CANTSPAREPLAYER, GetName(), rTheBid.GetPlayer().GetName());
        return false;
    }

	if (rTheBid.IsSellOnPercentAcceptable() == false)
	{
		return false;
	}

    rTheBid.m_szReasonString.Format(IDS_BIDACCEPT, GetName(), rTheBid.GetPlayer().GetName(), GetForeignStr(rTheBid.GetAmountOffered()));
    if (rTheBid.GetPlayer().IsTransferListed())
	{
		return rTheBid.IsOfferForListedPlayerAcceptable(GetCountry(), GetName());
    }
	return rTheBid.IsFairOffer(GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CClub::CanSparePlayer
	Access:    	public
	Parameter: 	CPlayer* a_pPlayer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::CanSparePlayer(CPlayer* _pPlayer)
{
	EXT_ASSERT(_pPlayer != null);
	bool CanSpare = true;
	if (m_squadFirstTeamPlayers.DoCountAvailablePlayers() < MINSQUADSIZE ||
		m_squadFirstTeamPlayers.DoCountAvailablePlayersPosn(_pPlayer->GetSkills().GetPosition()) < m_squadFirstTeamPlayers.MinimumAvailablePlayersInPosition(_pPlayer->GetSkills().GetPosition()))
	{
		CanSpare = false;
	}
	return CanSpare;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoFinancialLoanAccepted
	Access:    	public
	Parameter: 	const uint HowMuch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoFinancialLoanAccepted(const uint HowMuch)
{
	m_CashBalance += HowMuch;
	if (IsRanking0Club())
	{
		m_pAccounts->GetAt(CAccountsItemList::LOANRECEIVED).Plus(HowMuch);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetAccounts
	Access:    	public
	Parameter: 	void
	Returns:   	CAccountsItemList*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItemList* CClub::GetAccounts()
{
	EXT_ASSERT(m_pAccounts != null);
	return m_pAccounts;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoUserApplyFinancialLoan
	Access:    	public
	Parameter: 	const uint HowMuch
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoUserApplyFinancialLoan(const uint HowMuch)
{
	bool bRet = false;
	if (m_FinancialLoan.DoApplyLoan(HowMuch, GetCountry(), &GetPopularity()) == true)
	{
		DoFinancialLoanAccepted(HowMuch);
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoUserRepayFinancialLoan
	Access:    	public
	Parameter: 	const uint dValue
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoUserRepayFinancialLoan(const uint dValue)
{
	m_CashBalance -= m_FinancialLoan.DoRepayLoan(dValue);
	if (IsRanking0Club())
	{
 		m_pAccounts->GetAt(CAccountsItemList::LOANPAID).Plus(dValue);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::LoanPaidThisWeekStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::LoanPaidThisWeekStr()
{
    if (IsRanking0Club())
	{
		return GetForeignStr(m_pAccounts->GetAt(CAccountsItemList::LOANPAID).GetWeekly());
	}
	return TEXT("");
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRepayLoan
	Access:    	public
	Parameter: 	uint HowMuch
	Returns:   	void
	Qualifier:
	Purpose:	Repay part of loan
------------------------------------------------------------------------------*/
void CClub::DoRepayLoan(uint HowMuch)
{
	EXT_ASSERT(m_FinancialLoan.GetLoanOwed() > 0);
	CString str;
	HowMuch = GetCountry().GetCurrency().ConvToPounds(HowMuch);
	str.Format(IDS_CONFPARTREPAY, GetForeignStr(HowMuch));
	if (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		if (HowMuch > 0)
		{
			uint dAmountRepaid = m_FinancialLoan.DoRepayLoan(HowMuch);
			m_CashBalance -= dAmountRepaid;
			if (IsRanking0Club())
			{
				m_pAccounts->GetAt(CAccountsItemList::LOANPAID).Plus(dAmountRepaid);
			}
		}
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::TryAcceptLoanTransferBid
	Access:    	public
	Parameter: 	CPlayer* pPlayer
	Parameter: 	CClub* pBiddingClub
	Parameter: 	int HowLong
	Parameter: 	CString* str
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::TryAcceptLoanTransferBid(CPlayer* pPlayer, CClub* pBiddingClub, int HowLong, CString* str)
{
	EXT_ASSERT(pBiddingClub->AreLoanTransfersAllowed() == true);
    if (IsUserControlled() == true)
	{
		// Ask manager if he wishes to loan player
		CString Str;
        Str.Format(IDS_RECEIVELOANBID, GetManagerName(),
										pBiddingClub->GetName(),
										pPlayer->GetName(),	HowLong);
		if (UserMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
		{
			str->Format(IDS_PLAYERLOANAGREED, GetName(), pPlayer->GetName());
			return true;
		}
		else
		{
            str->Format(IDS_REFUSETOLOAN, GetName(), pPlayer->GetName());
			return false;
		}
	}

	// Computer Managed Club decide whether to accept bid
	if (m_squadFirstTeamPlayers.CanAffordToLosePlayer(pPlayer, true) == false)
	{
		str->Format(IDS_REFUSETOLOANNOCOVER, GetName(), pPlayer->GetName());
		return false;
	}
	if (DoCountLeagueGamesPlayedThisSeason() > 6)
	{
		ushort SinceAppearRefusalChance[] =
		{
			0, 0, 0, 0, 10, 20, 40
		};
		if (RandomNumber.IntLessThan(100) > SinceAppearRefusalChance[min((byte)6, pPlayer->NumberMatchesSinceLastAppeared())])
		{
			// Refuse to loan
			str->Format(IDS_REFUSETOLOANINTEAMPLANS, GetName(), pPlayer->GetName());
			return false;
		}
	}
	return pPlayer->TryAcceptLoanTransferBid(pBiddingClub, str);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCountLeagueGamesPlayedThisSeason
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:	Returns the number of league games played this season
------------------------------------------------------------------------------*/
int CClub::DoCountLeagueGamesPlayedThisSeason()
{
	return m_TableData.GetHomeGames()->GetNumberGamesPlayed() + m_TableData.GetAwayGames()->GetNumberGamesPlayed();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::NeedEmployee
	Access:    	public
	Parameter: 	CBasicEmployee::eEmployeeType WhatType
	Parameter: 	bool bWarnings
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::NeedEmployee(CEmployeeBase::eEmployeeType WhatType , bool bWarnings)
{
	CString str;
	switch(WhatType)
	{
		default :
			EXT_ASSERT(false);
        case CEmployeeBase::COACH:
			if (GetCoachStaffNumber() == wNOTFOUND)
			{
				return true;
			}
			else
			{
				if (bWarnings == true)
				{
					UserMessageBox(IDS_DONTNEEDCOACH, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1);
				}
				return false;
			}
        case CEmployeeBase::PHYSIO:
			if (GetPhysioStaffNumber() == wNOTFOUND)
			{
				return true;
			}
			else
			{
				if (bWarnings == true)
				{
					UserMessageBox(IDS_DONTNEEDPHYSIO, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1);
				}
				return false;
			}
#ifdef USESCOUTS
        case CEmployeeBase::SCOUT:
			if (NumScouts() < 3)
			{
				return true;
			}
			else
			{
				if (bWarnings == true)
				{
					UserMessageBox(IDS_DONTNEEDSCOUT, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1);
				}
				return false;
			}
#endif
	}
    return true;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::AllowToHirePlayer
	Access:    	public
	Parameter: 	CYouth* pYouth
	Parameter: 	bool ShowMessage
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CClub::AllowToHirePlayer(CYouth* pYouth, bool ShowMessage)
{
	uint MaxPlayerPosition[] =
	{
		MAXIMUMGOALKEEPERS,
		MAXIMUMDEFENDERS,
		MAXIMUMMIDFIELDERS,
		MAXIMUMATTACKERS
	};
	if (m_squadFirstTeamPlayers.TotalSquadSize() >= PLAYERSINSQUAD)
	{
		if (ShowMessage == true)
		{
			UserMessageBox(IDS_SQUADFULL);
			return NOTFOUND;
		}
		else
		{
			return IDS_SQUADFULL;
		}
	}

	if (m_squadFirstTeamPlayers.DoCountAllPlayersPosn(pYouth->GetSkills().GetPosition()) >= MaxPlayerPosition[pYouth->GetSkills().GetPosition()])
	{
		if (ShowMessage == true)
		{
			UserMessageBox(IDS_POSITIONFULL);
			return NOTFOUND;
		}
		else
		{
			return IDS_POSITIONFULL;
		}
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AddToClubNews
	Access:    	public
	Parameter: 	CExtString str
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::AddToClubNews(const CExtString& str)
{
    if (IsUserControlled() == true)
	{
        GetUser().AddToNews(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AddToClubNews
	Access:    	public
	Parameter: 	const int aStrID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::AddToClubNews(const int aStrID)
{
    if (IsUserControlled() == true)
	{
		CExtString str;
		str.LoadString(aStrID);
        GetUser().AddToNews(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::ReceivedSellOnClauseMoney
	Access:    	public
	Parameter: 	const uint _Cost
	Parameter: 	byte SellOnClausePercentage
	Parameter: 	CString PlayerName
	Parameter: 	CString ClubName
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoReceivedSellOnClauseMoney(const uint _Cost, byte _SellOnClausePercentage, const CString &_PlayerName, const CString &_ClubName)
{
	CExtString str;
	uint dSellonCost = _Cost / 100;
	dSellonCost *= _SellOnClausePercentage;
	dSellonCost = _Cost - dSellonCost;
	DoAddTransferIncome(dSellonCost);
    str.Format(IDS_RECEIVEDSELLONMONEY,
				_PlayerName,
				_ClubName,
				GetForeignStr(_Cost),
				GetForeignStr(dSellonCost));
	// Inform the manager if human
	AddToClubNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSellPlayer
	Access:    	public
	Parameter: 	CPlayer& _Player
	Parameter: 	const uint _Cost
	Parameter: 	CClub& _BiddingClub
	Parameter: 	bool WasRecordSigning
	Parameter: 	bool IsBosman
	Returns:   	void
	Qualifier:
	Purpose:	The specified player has been sold
------------------------------------------------------------------------------*/
void CClub::DoSellPlayer(CPlayer& _Player, const uint _Cost, CClub& _BiddingClub, bool _WasRecordSigning, bool _IsBosman)
{
	EXT_ASSERT(_Player.IsOnLoan() == false);
	static uint FreeMsgs[] =
	{
		IDS_FREETRANSFEREDPLAYER,
		IDS_FREETRANSFEREDPLAYER1,
		IDS_FREETRANSFEREDPLAYER2,
		IDS_FREETRANSFEREDPLAYER3,
		IDS_FREETRANSFEREDPLAYER4,
		IDS_FREETRANSFEREDPLAYER5,
		IDS_FREETRANSFEREDPLAYER6,
	};
	const uint NumFreeMsgs = 7;
	static uint SoldMsgs[] =
	{
		IDS_TRANSFEREDPLAYER,
		IDS_TRANSFEREDPLAYER,
		IDS_TRANSFEREDPLAYER,
		IDS_TRANSFEREDPLAYER,
	};
	const uint NumSoldMsgs = 4;
	CExtString str;
	int SoldMsgNum;
	CContractClause* ContractClausePtr = WorldData().GetContractClauseList().DoFindPlayerClubClause(_Player.DoFindID(), this);
	if (ContractClausePtr != null)
	{
		if (_Cost > 0)
		{
			uint dSellOnCost = (_Cost / 100) * (100 - ContractClausePtr->GetSellOnClausePercentage());
			ContractClausePtr->GetClubTo().DoReceivedSellOnClauseMoney(dSellOnCost,
							   ContractClausePtr->GetSellOnClausePercentage(), _Player.GetName(), GetName());
			str.Format(IDS_DEDUCTEDSELLONMONEY, ContractClausePtr->GetClubTo().GetName(), GetForeignStr(_Cost - dSellOnCost),
												_Player.GetName(), ContractClausePtr->GetSellOnClausePercentage());
			// Inform the manager if human
			AddToClubNews(str);
			DoAddTransferExpenditure(_Cost - dSellOnCost);
		}
		WorldData().GetContractClauseList().RemoveOne(_Player.DoFindID(), this);
	}

	DoAddTransferIncome(_Cost);
    GetClubHistory().CheckIfRecordSale(_Player, _BiddingClub.GetNameID(), _Cost, WorldData().GetCurrentDate());
    if (IsUserControlled() == true && WorldData().IsFastTest() == false && _IsBosman == false)
	{
		m_squadFirstTeamPlayers.DoFansReactionToSale(_Player, _Cost);
	}

	// Record selling club name, fee and date
#ifdef PLAYERTRANSFERHISTORY
    pPlayer->AddTransferHistory(GetName(), _Cost, WorldData().GetCurrentDate().ShortNumDateStr());
#endif
    DoRemovePlayerFromSquad(_Player.DoFindID());

	if (_Cost > 0)
	{
		if (_WasRecordSigning == false)
		{
			SoldMsgNum = RandomNumber.IntLessThan(NumSoldMsgs - 1);
			str.Format(SoldMsgs[SoldMsgNum], _Player.GetName(), _BiddingClub.GetName(), GetForeignStr(_Cost),
					   GetName());
		}
		else
		{
			str.Format(IDS_TRANSFEREDPLAYERRECORD, _BiddingClub.GetName(), _Player.GetName(),
													GetForeignStr(_Cost), GetName());
		}
	}
	else
	{
		if (_IsBosman ==false)
		{
			SoldMsgNum = RandomNumber.IntLessThan(NumFreeMsgs - 1);
			str.Format(FreeMsgs[SoldMsgNum], _Player.GetName(), _BiddingClub.GetName(), GetName());
		}
		else
		{
			str.Format(IDS_LEFTONBOSMAN, _Player.GetName(), _BiddingClub.GetName(), GetName());
		}
	}
	// Inform the manager if human
	AddToClubNews(str);
#ifdef PLAYERTRANSFERHISTORY
    m_PlayersSold.Add(new CTransferHistory(_Player, _BiddingClub.GetNameID(), _Cost, WorldData().GetCurrentDate()));
#endif
	// Add to country notices
    GetActiveCountry().AddToNewsLists(str);          // Add to news

	// If player is going to a club in a different country then add to that country notices
	if (&GetCountry() != &_BiddingClub.GetCountry())
	{
		// Only need to reconstruct the message if a fee was involved as the only difference will be the different currency
		if (_Cost > 0)
		{
			if (_WasRecordSigning == false)
			{
				str.Format(SoldMsgs[SoldMsgNum], _Player.GetName(), _BiddingClub.GetName(),
							_BiddingClub.GetForeignStr(_Cost), GetName());
			}
			else
			{
				str.Format(IDS_TRANSFEREDPLAYERRECORD, _BiddingClub.GetName(), _Player.GetName(),
														_BiddingClub.GetForeignStr(_Cost), GetName());
			}
		}
        _BiddingClub.GetActiveCountry().AddToNewsLists(str);
	}
	WorldData().GetUserList().CheckIfPlayerInTransferNegotiations(_Player.DoFindID(), &_BiddingClub);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAddTransferIncome
	Access:    	public
	Parameter: 	const uint _Cost
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAddTransferIncome(const uint _Cost)
{
    if (_Cost > 0 && IsRanking0Club())
	{
		m_CashBalance += _Cost * TRANSFERFEETAXRATE;
        m_pAccounts->GetAt(CAccountsItemList::TRANSFERINCOME).Plus(_Cost * TRANSFERFEETAXRATE);
		if (m_ManagerID != NOMANAGER)
		{
            GetManager().DoAddTransferIncome(_Cost);
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoPaySignOnFee
	Access:    	public
	Parameter: 	const uint _SignOnFee
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoPaySignOnFee(const uint _SignOnFee)
{
    m_CashBalance -= _SignOnFee;
    if (IsRanking0Club())
	{
		m_pAccounts->GetAt(CAccountsItemList::TRANSFEREXPENDITURE).Plus(_SignOnFee);
	}
}


/*------------------------------------------------------------------------------
Method:   	CClub::DoBuyPlayer
Access:    	public
Parameter: 	CPlayer& _Player
Parameter: 	const uint Cost
Parameter: 	CContract& _PlayerContract
Parameter: 	const CStringID& ClubNameID
Returns:   	bool
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoBuyPlayer(CPlayer& _Player, const uint _Cost, CContract& _PlayerContract, const CStringID& _ClubNameID)
{
	DoAddTransferExpenditure(_Cost, &_PlayerContract);
	bool bRet = GetClubHistory().CheckIfRecordSign(_Player, _ClubNameID, _Cost, WorldData().GetCurrentDate());
	_Player.DoJoinedNewClub(_PlayerContract);
	DoAddPlayerToSquad(_Player.DoFindID());
#ifdef PLAYERTRANSFERHISTORY
	m_PlayersBought.Add(new CTransferHistory(_Player.GetName(), _ClubNameID, _Cost, WorldData().GetCurrentDate(), a_nPlayerID));
#endif
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAddTransferExpenditure
	Access:    	public
	Parameter: 	const uint Cost
	Parameter: 	CContract* pPlayerContract
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAddTransferExpenditure(const uint _Cost, CContract* _PlayerContract /*= null*/)
{
    m_CashBalance -= _Cost;
    if (IsRanking0Club())
	{
		if (_Cost > 0)
		{
			m_pAccounts->GetAt(CAccountsItemList::TRANSFEREXPENDITURE).Plus(_Cost);
			if (m_ManagerID != NOMANAGER)
			{
				GetManager().DoAddTransferExpenditure(_Cost);
			}
		}
	}
	// Receiving sellon clause money, no player costs
	if (_PlayerContract != null)
	{
		DoPaySignOnFee(_PlayerContract->GetSignOnFee());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoBuyTransferPlayer
	Access:    	public
	Parameter: 	CPlayer& _Player
	Parameter: 	const uint Cost
	Parameter: 	CContract& _PlayerContract
	Parameter: 	CClub* pFromClub
	Parameter: 	byte SellOnPercentage
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoBuyTransferPlayer(CPlayer& _Player, const uint _Cost, CContract& _PlayerContract, CClub* _FromClub, byte _SellOnPercentage)
{
    if (_FromClub != null)
	{
		// Has come from another club
		DoBuyPlayer(_Player, _Cost, _PlayerContract,_FromClub->GetNameID());
		if (_SellOnPercentage > 0 && _Cost > 0)
		{
			WorldData().GetContractClauseList().DoAddOne(_Player.DoFindID(), _SellOnPercentage, _FromClub->DoFindID(), DoFindID());
		}
	}
	else
	{
		_Player.DoJoinedNewClub(_PlayerContract);
		DoAddPlayerToSquad(_Player.DoFindID());
#ifdef PLAYERTRANSFERHISTORY
		CStringID ClubNameID;
		m_PlayersBought.Add(new CTransferHistory(_Player, ClubNameID, 0, WorldData().GetCurrentDate()));
#endif
		DoPaySignOnFee(_PlayerContract.GetSignOnFee());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoPlayerGoingOutOnLoan
	Access:    	public
	Parameter: 	const ushort a_nPlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoPlayerGoingOutOnLoan(const ushort _PlayerID)
{
	m_squadFirstTeamPlayers.DoIncrementPlayersOutOnLoan();
	DoRemovePlayerFromSquad(_PlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::PlayerBackFromLoan
	Access:    	public
	Parameter: 	const ushort a_nPlayerID
	Returns:   	void
	Qualifier:
	Purpose:	A player is returning to this club from a loan period
------------------------------------------------------------------------------*/
void CClub::DoPlayerBackFromLoan(const ushort _PlayerID)
{
	m_squadFirstTeamPlayers.DoDecrementPlayersOutOnLoan();
	if (m_squadFirstTeamPlayers.TotalSquadSize() < PLAYERSINSQUAD - 1)
	{
		DoAddPlayerToSquad(_PlayerID);
	}
	else
	{
		EXT_ASSERT(false); // loan out club has full squad, need to sort this out
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GotPlayerOnLoan
	Access:    	public
	Parameter: 	const ushort player
	Parameter: 	const int iWeeks
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::GotPlayerOnLoan(const ushort player, const int iWeeks)
{
	m_squadFirstTeamPlayers.GotPlayerOnLoan();
	DoAddPlayerToSquad(player);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAddPlayerToSquad
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAddPlayerToSquad(const ushort _PlayerID)
{
    EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize());
	m_squadFirstTeamPlayers.DoAddPlayerToSquad(_PlayerID);
	if (IsUserControlled() == true)
	{
	   GetUser().GetTacticsList().DoSquadChanged();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRemovePlayerFromSquad
	Access:    	public
	Parameter: 	const ushort a_nPlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoRemovePlayerFromSquad(const ushort _PlayerID)
{
    EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize());
	m_squadFirstTeamPlayers.DoRemoveFromSquad(m_squadFirstTeamPlayers.DoFindSquadNumber(_PlayerID));
	if (IsUserControlled() == true)
	{
		GetUser().GetTacticsList().DoSquadChanged();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GotEnoughMoney
	Access:    	public
	Parameter: 	const uint Amount
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::GotEnoughMoney(const uint Amount)
{
    if (m_CashBalance >= static_cast<int>(Amount))
	{
        return true;
	}
	else
	{
		return false;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::GotEnoughMoneyMsg
	Access:    	public
	Parameter: 	const uint Amount
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::GotEnoughMoneyMsg(const uint Amount)
{
    if (GotEnoughMoney(Amount) == false && IsUserControlled() == true)
	{
		UserMessageBox(IDS_NOTENOUGHMONEY);
		return false;
	}
	else
	{
        return true;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::GotEnoughForeignMoneyMsg
	Access:    	public
	Parameter: 	const uint Amount
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::GotEnoughForeignMoneyMsg(const uint Amount)
{
	return GotEnoughMoneyMsg(GetCountry().GetCurrency().ConvToPounds(Amount));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsLeagueMatchToday
	Access:    	public
	Parameter: 	const CCalendar& CurrentDate
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsLeagueMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match)
{
	bool bRet = false;

	// Default to league match for number of substitutes
	_Match.SetSubstitutes(GetDivision().GetSubsSelect(), GetDivision().GetSubsUse());

	if (GetDivision().IsCompetitiveMatch(_CurrentDate.GetMatchDay()) == true)
	{
		ushort HomeClubID, AwayClubID;
		ushort ClubToFind = DoFindID();
		CSoccerResultList cSoccerResultList;
		GetDivision().DoBuildLeagueFixtureList(_CurrentDate.GetMatchDay(), cSoccerResultList);
		for (int LoopCount = 0; LoopCount < cSoccerResultList.GetSize(); LoopCount ++)
		{
			HomeClubID = cSoccerResultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID();
			AwayClubID = cSoccerResultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID();
			// If this club is involved in the fixture
			if (HomeClubID == ClubToFind || AwayClubID == ClubToFind)
			{
				// Match to play
				_Match.DoInitialiseLeagueMatch(HomeClubID, AwayClubID, GetDivision(), null);

				ClubToFind = GetDivision().GetLeagueMatchResult(_CurrentDate.GetMatchDay(), HomeClubID, AwayClubID);
				if (ClubToFind != NOCLUB)
				{
					_Match.SetFixture(&GetDivision().GetFixture(ClubToFind));
				}

				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::NumberSubsSelect
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::NumberSubsSelect()
{
	CMatchInfo Match;
	if (IsMatchToday(WorldData().GetCurrentDate(), Match) == true)
	{
		return Match.GetSubsSelect();
	}
	else
	{
		return GetDivision().GetSubsSelect();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsMatchToday
	Access:    	public
	Parameter: 	const CCalendar& CurrentDate
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match)
{
	bool bRet = false;
    if (IsLeagueMatchToday(_CurrentDate, _Match) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
		bRet = true;
	}

	ushort nClubID = DoFindID();
    if (WorldData().GetCupList().IsClubMatchToday(_CurrentDate, nClubID, _Match) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
        bRet = true;
	}

    if (WorldData().GetCharityShieldList().IsClubMatchToday(_CurrentDate, nClubID, _Match) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
        bRet = true;
	}

    if (WorldData().GetPlayOffList().IsClubMatchToday(_CurrentDate, nClubID, _Match) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
        bRet = true;
	}

#ifdef ACTIVEFRIENDLIES
    if (GetFriendlies().IsFriendlyMatchToday(_CurrentDate.GetMatchDay(), _Match, this) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
		bRet = true;
	}
#endif

#ifdef ACTIVEFEDERATIONCUPS
    if (WorldData().GetFederationCupList().IsClubMatchToday(_CurrentDate, nClubID, _Match) == true)
	{
		EXT_ASSERT(bRet == false);	// No other matches on this date
        bRet = true;
	}
#endif
    return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetLeagueFixtureDetails
	Access:    	public
	Parameter: 	CMatchInfo* Match
	Parameter: 	const int TheWeek
	Parameter: 	const eClubResultsType eResultToShow
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::GetLeagueFixtureDetails(CMatchInfo& _MatchInfo, const int _TheWeek, const eClubResultsType _eResultToShow)
{
	EXT_ASSERT(_TheWeek < TOTALMATCHES);
	bool bRet = false;

    if (GetDivision().IsCompetitiveMatch(_TheWeek) == true)
	{
		ushort ClubtoFindID = DoFindID();
        ushort MatchNumber = GetDivision().GetMatchType(_TheWeek) * (GetDivision().NumberMatchClubs() / 2);
		CSoccerResult SoccerResult;
		GetDivision().SeekClubFixtureForMatchNumber(ClubtoFindID, _TheWeek, SoccerResult);

		ushort HomeClubID = SoccerResult.GetHomeClubDetails().GetClubID();
		ushort AwayClubID = SoccerResult.GetAwayClubDetails().GetClubID();

		bool bUseIt = false;
		if (_eResultToShow == MATCHES_ALL)
		{
			bUseIt = true;
		}
		else
		{
			if (GetDivision().GetFixture(MatchNumber).GetAttendance() > 0)
			{
				CFixture& TheFixture = GetDivision().GetFixture(MatchNumber);
				switch(_eResultToShow)
				{
					case MATCHES_PLAYED:
						bUseIt = true;
						break;
					case MATCHES_WON:
						if (TheFixture.GetWinningClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_DRAWN:
						if (TheFixture.GetNumberGoals(HOME) == TheFixture.GetNumberGoals(AWAY))
						{
							bUseIt = true;
						}
						break;
					case MATCHES_LOST:
						if (TheFixture.GetLosingClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_SCORED:
						if ((HomeClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) > 0) ||
							(AwayClubID == ClubtoFindID && TheFixture.GetNumberGoals(AWAY) > 0))
						{
							bUseIt = true;
						}
						break;
					case MATCHES_CONCEDED:
						if ((HomeClubID == ClubtoFindID && TheFixture.GetNumberGoals(AWAY) > 0) ||
							(AwayClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) > 0))
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_PLAYED:
						if (HomeClubID == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_WON:
						if (HomeClubID == ClubtoFindID && TheFixture.GetWinningClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_DRAWN:
						if (HomeClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) == TheFixture.GetNumberGoals(AWAY))
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_LOST:
						if (HomeClubID == ClubtoFindID && TheFixture.GetLosingClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_SCORED:
						if (HomeClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) > 0)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_HOME_CONCEDED:
						if (HomeClubID == ClubtoFindID && TheFixture.GetNumberGoals(AWAY) > 0)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_PLAYED:
						if (AwayClubID == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_WON:
						if (AwayClubID == ClubtoFindID && TheFixture.GetWinningClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_DRAWN:
						if (AwayClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) == TheFixture.GetNumberGoals(AWAY))
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_LOST:
						if (AwayClubID == ClubtoFindID && TheFixture.GetLosingClubID() == ClubtoFindID)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_SCORED:
						if (AwayClubID == ClubtoFindID && TheFixture.GetNumberGoals(AWAY) > 0)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_AWAY_CONCEDED:
						if (AwayClubID == ClubtoFindID && TheFixture.GetNumberGoals(HOME) > 0)
						{
							bUseIt = true;
						}
						break;
					case MATCHES_FORM_PLAYED:
					case MATCHES_FORM_WON:
					case MATCHES_FORM_DRAWN:
					case MATCHES_FORM_LOST:
					case MATCHES_FORM_SCORED:
					case MATCHES_FORM_CONCEDED:
						break;
				};
			}
		}
		if (bUseIt == true)
		{
			_MatchInfo.DoInitialiseLeagueMatch(HomeClubID, AwayClubID, GetDivision(), &GetDivision().GetFixture(MatchNumber));
			if (GetDivision().GetFixture(MatchNumber).GetAttendance() == 0)
			{
				_MatchInfo.SetFixture(null);
			}
			bRet = true;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetNextMatchDetails
	Access:    	public
	Parameter: 	CMatchInfo& Match
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::GetNextMatchDetails(CMatchInfo& Match)
{
    CCalendar TheDate(WorldData().GetCurrentDate());
	while (GetDivision().GetMatchType(TheDate.GetMatchDay()) != DIVENDSEASON && \
		   GetDivision().GetMatchType(TheDate.GetMatchDay()) != DIVCLOSESEASON )
	{
		if (IsMatchToday(TheDate, Match) == true)
		{
			Match.SetTheDate(TheDate);
			break;
		}
		TheDate.NextMatchDay();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AskSackYouth
	Access:    	public
	Parameter: 	CYouth* pYouth
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::AskSackYouth(CYouth* _Youth)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	CString str;
	str.Format(IDS_CONFYOUTHSACK, _Youth->GetName());
	if (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		SackYouth(_Youth);
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SackYouth
	Access:    	public
	Parameter: 	CYouth* pYouth
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SackYouth(CYouth* _Youth)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	m_YouthSquad.SackYouth(_Youth);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SackPlayer
	Access:    	public
	Parameter: 	CPlayer* pWho
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SackPlayer(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	m_squadFirstTeamPlayers.DoRemoveFromSquad(m_squadFirstTeamPlayers.DoFindSquadNumber(_Player));
    if (_Player->GetAge() > 35 || (_Player->GetAge() > 31 && RandomNumber.IntLessThan(10) < 3))
	{
        _Player->DoRetired(DoFindID());
    }
    else
	{
		// Sign him to a new club immediately
		CClub* pClub = _Player->DoFindSuitableNewClub(this);
		if (pClub != null)
		{
			CContract PlayerContract;
			_Player->DoGenerateRandomContract(PlayerContract, pClub->GetCountry());
			pClub->DoBuyTransferPlayer(*_Player, 0, PlayerContract, null, 0);
			DEBUG_OUTPUT(("Sacked Player Signing %s for %s"), (pClub->GetName(), _Player->GetName()));
		}
		else
		{
			_Player->DoRetired(DoFindID());
		}
    }

  if (IsRanking0Club())
	{
		m_pAccounts->GetAt(CAccountsItemList::GENERALEXPENDITURE).Plus(_Player->ContractValue());
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRemoveFromSquad
	Access:    	public
	Parameter: 	const ushort a_PlayerToRemoveSquadNumber
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoRemoveFromSquad(const ushort _PlayerToRemoveSquadNumber)
{
	m_squadFirstTeamPlayers.DoRemoveFromSquad(_PlayerToRemoveSquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::EmployStaff
	Access:    	public
	Parameter: 	CBasicEmployee::eEmployeeType Type
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoEmployStaff(CEmployeeBase::eEmployeeType Type)
{
    EXT_ASSERT(m_StaffList.GetSize() != EMPLOYEESINSQUAD);
    m_StaffList.Add(new CEmployee(Type, GetCountry(), GetPopularity()));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::EmployStaff
	Access:    	public
	Parameter: 	CEmployee* _Employee
	Returns:   	int - staff index
	Qualifier:
	Purpose:	Add the employee to the backroom staff
------------------------------------------------------------------------------*/
int CClub::DoEmployStaff(CEmployee* _Employee)
{
    EXT_ASSERT(m_StaffList.GetSize() != EMPLOYEESINSQUAD);
	// Remove from the available employees list
	GetActiveCountry().SackEmployee(_Employee);
    int Who = m_StaffList.Add(_Employee);
	return Who;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoStaffMemberLeavingClub
	Access:    	public
	Parameter: 	CEmployee * _Employee
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoStaffMemberLeavingClub(CEmployee* _Employee)
{
#ifdef USESCOUTS
	if (m_Scout1.GetNumber() == Who)
	{
		m_Scout1.SetNumber(static_cast<byte> (NOMANAGER));
	}
	if (m_Scout2.GetNumber() == Who)
	{
		m_Scout2.SetNumber(static_cast<byte> (NOMANAGER));
	}

	// Scout numbers will change if above the deleted employee
	if (m_Scout1.GetNumber() > Who && m_Scout1.Active() == true)
	{
		m_Scout1.SetNumber(m_Scout1.GetNumber() - 1);
	}
	if (m_Scout2.GetNumber() > Who && m_Scout2.Active() == TRUE)
	{
		m_Scout2.SetNumber(m_Scout2.GetNumber() - 1);
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoFindID
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::DoFindID()
{
	return  m_ID; //WorldData().GetClubList().DoFindID(this);
}


 //--------------------------------------------------------------------------
bool CClub::AskSackEmployee(CEmployee* pEmployee)
{
	EXT_ASSERT(pEmployee != null);
	CString str;
	str.Format(IDS_CONFEMPLOYEESACK, pEmployee->GetName(), pEmployee->ContractValueStr(GetCountry()));
	if (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		DoSackStaff(pEmployee, pEmployee->ContractValue());
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSackStaff
	Access:    	public
	Parameter: 	CEmployee * _Employee
	Parameter: 	const uint _ContractValue
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSackStaff(CEmployee* _Employee, const uint _ContractValue)
{
	// Move the employee to the countries unemployed list
	m_StaffList.DoMoveToAvailableEmployees(_Employee, *this);
    DoStaffMemberLeavingClub(_Employee);

    if (IsRanking0Club())
	{
		m_pAccounts->GetAt(CAccountsItemList::GENERALEXPENDITURE).Plus(_ContractValue);
	}
	m_CashBalance -= _ContractValue;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AreBuyTransfersAllowed
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::AreBuyTransfersAllowed()
{
	return  GetDivision().CanBuyPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AreLoanTransfersAllowed
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::AreLoanTransfersAllowed()
{
	return GetDivision().CanLoanPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryBuyPlayer
	Access:    	public
	Parameter: 	CPlayerSearch & _PlayerSearch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoTryBuyPlayer(CPlayerSearch& _PlayerSearch)
{
	EXT_ASSERT(AreBuyTransfersAllowed() == true);
	// Any available players suitable ?
	CNumericArray<ushort>& theList = _PlayerSearch.GetPlayerList();
	if (theList.GetSize() > 0)
	{
		bool Quit = false;
		ushort PlayerListIndex;
		while (Quit != true)
		{
			// Get a player at random if more than 1 in list
			if (theList.GetUpperBound() > 0)
			{
				PlayerListIndex = RandomNumber.IntLessThan(theList.GetUpperBound());
			}
			else
			{
				// Only 1 in lst so bid for him
				PlayerListIndex = 0;
			}
			if (DoBidForPlayer(theList[PlayerListIndex], _PlayerSearch.GetPlayerClubNumberList()[PlayerListIndex]) == true)
			{
				// Bought the player, rare to buy more than 1 player per week
				if (RandomNumber.IntLessThan(CHANCEBUYTWOPLAYERS) != 1)
				{
					Quit = true;
				}
			}
            _PlayerSearch.DoRemovePlayer(PlayerListIndex);		// Ensure don't bid for him again

            // Should we try to bid for another?
            if (RandomNumber.IntLessThan(BIDATTEMPTS) == 1 || m_squadFirstTeamPlayers.TotalSquadSize() == PLAYERSINSQUAD || theList.GetSize() == 0)
			{
				Quit = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCreateBuyPlayerSearch
	Access:    	public
	Parameter: 	CPlayerSearch& PlayerSearch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoCreateBuyPlayerSearch(CPlayerSearch& _PlayerSearch)
{
	_PlayerSearch.DoInitSearch(16 + RandomNumber.IntLessThan(3), 38 - RandomNumber.IntLessThan(5), GetPopularity().GetMinimumPlayerSkill(GetDivisionStrengthID()), \
							  GetPopularity().GetMaximumPlayerSkill(), 0, \
							 GetPopularity().GetMaximumPlayerPurchaseValue(GetCountry().GetFinancialRatioPercentage()), 0);

	// Build a list of suitable players
	if (RandomNumber.IntLessThan(100) > GetDivision().GetCountrySellBuyChances()->GetPercentageChanceBuy())
	{
		// Buy from own country
		GetActiveCountry().GetAllTransferListed(this, _PlayerSearch);
	}
	else
	{
		// Buy from a random country
		WorldData().GetCountryList().DoSelectRandomActive().GetActiveCountry().GetAllTransferListed(this, _PlayerSearch);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoFindFreeTransferPlayer
	Access:    	public
	Parameter: 	ePlayerPosition Posn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoFindFreeTransferPlayer(ePlayerPosition _Posn)
{
	bool Found = false;
    CClub* pClub;
	int Count = 0;
	while (Found == false && Count++ < 30)
	{
		pClub = GetActiveCountry().DoFindSuitableTransferDivision(m_DivisionID)->DoFindRandomNonUserClub(this);
		// Check if non user club will give a free transfer player
		Found = pClub->m_squadFirstTeamPlayers.DoGivePlayerFreeTransferToClub(_Posn, this);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoEnsureEnoughFitPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoEnsureEnoughFitPlayers()
{
	if (AreBuyTransfersAllowed() == false /* && AreLoanTransfersAllowed() == false */)
	{
		return;
	}
	uint Count = 0;

	CPlayerSearch PlayerSearch;

	// Check all positions
	for (uint nPosition = GOALKEEPER; nPosition < MAXPLAYERPOSITIONS; nPosition++)
	{
		PlayerSearch.DoEmpty();
		while  (Count < 30 && m_squadFirstTeamPlayers.TotalSquadSize() < PLAYERSINSQUAD &&
				m_squadFirstTeamPlayers.DoCountAvailablePlayersPosn(static_cast<ePlayerPosition>(nPosition)) < m_squadFirstTeamPlayers.MinimumAvailablePlayersInPosition(static_cast<ePlayerPosition>(nPosition)))
		{
			if (PlayerSearch.IsEmpty() == true)
			{
				PlayerSearch.SetFindPosition(static_cast<ePlayerPosition>(nPosition));
				DoCreateBuyPlayerSearch(PlayerSearch);
				PlayerSearch.SetListCreated(true);
			}
			if ((Count++ < 25) && (AreBuyTransfersAllowed() == true) &&
				m_squadFirstTeamPlayers.DoCountAllPlayersPosn(static_cast<ePlayerPosition>(nPosition)) < m_squadFirstTeamPlayers.MinimumAvailablePlayersInPosition(static_cast<ePlayerPosition>(nPosition)))
			{
				DoTryBuyPlayer(PlayerSearch);
			}
			else
			{
				if ((AreBuyTransfersAllowed() == true) && (m_squadFirstTeamPlayers.DoMakeFitPlayer(static_cast<ePlayerPosition>(nPosition)) == false))
				{
					DoTryBuyPlayer(PlayerSearch);
				}
				else
				{
					if (Count > 25)
					{
						DoFindFreeTransferPlayer(static_cast<ePlayerPosition>(nPosition));
					}
				}
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WeeklyFinance
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoWeeklyFinance()
{
    EXT_ASSERT(GetCountryRanking() == 0);
	EXT_ASSERT(m_pAccounts != null);

    if (IsUserControlled() != true)
	{
			return;
	}

	CPopularity& thePopularity = GetPopularity();
    uint LastWeeksProfit = 0;

    // Calculate security costs for home matches
	uint actmoney = m_MidWeekMatch.DoCalculateSecurityCosts();
    actmoney += m_WeekEndMatch.DoCalculateSecurityCosts();
	m_pAccounts->GetAt(CAccountsItemList::SECURITY).Plus(actmoney);
    LastWeeksProfit -= actmoney;

	byte FinancialRatioPercentage = GetCountry().GetFinancialRatioPercentage();
	actmoney = thePopularity.GetWeeklyTvRevenue(FinancialRatioPercentage);
    m_pAccounts->GetAt(CAccountsItemList::TELEVISIONINCOME).Plus(actmoney);
    LastWeeksProfit += actmoney;

	actmoney = thePopularity.GetWeeklySponsorship(FinancialRatioPercentage);
    m_pAccounts->GetAt(CAccountsItemList::SPONSORSHIPINCOME).Plus(actmoney);
    LastWeeksProfit += actmoney;

	actmoney = (thePopularity.GetWeeklyMerchandiseIncome(FinancialRatioPercentage) / 100) * GetMorale();
    m_pAccounts->GetAt(CAccountsItemList::MERCHANDISEINCOME).Plus(actmoney);
    LastWeeksProfit += actmoney;

    // Calculate the wage bill for the week
	uint wagebill = thePopularity.GetGeneralWage(FinancialRatioPercentage);
    m_pAccounts->GetAt(CAccountsItemList::GENERALWAGE).Plus(wagebill);

	if (m_ManagerID != NOMANAGER)
	{
        wagebill += GetManager().GetWage();
        m_pAccounts->GetAt(CAccountsItemList::MANAGERSWAGES).Plus(GetManager().GetWage());
	}

	uint totalplaywage = m_squadFirstTeamPlayers.DoCalculateWageBill();

    wagebill += totalplaywage;
   	m_pAccounts->GetAt(CAccountsItemList::PLAYERWAGES).Plus(totalplaywage);

    // employees
    totalplaywage = m_StaffList.DoCalculateWageBill();
    wagebill += totalplaywage;
    m_pAccounts->GetAt(CAccountsItemList::EMPLOYEEWAGES).Plus(totalplaywage);

    // youth team
    if (m_YouthSquad.GetSize() > 0)
	{
		m_YouthSquad.DoWeeklyTasks(*this);
    	totalplaywage = (YOUTHWEEKLYWAGE * m_YouthSquad.GetSize());
	}
	else
	{
		totalplaywage = 0;
	}

    wagebill += totalplaywage;
   	m_pAccounts->GetAt(CAccountsItemList::YOUTHWAGES).Plus(totalplaywage);

	// Gate receipts
    LastWeeksProfit += m_MidWeekMatch.GetGateReceipts() + m_WeekEndMatch.GetGateReceipts() - wagebill;

	// Running costs
    actmoney = thePopularity.GetRunningCosts(GetCountry().GetFinancialRatioPercentage());
	m_pAccounts->GetAt(CAccountsItemList::RUNNINGCOSTS).Plus(actmoney);
    LastWeeksProfit -= actmoney;

/*    if (RandomNumber.IntLessThan(40) < 8)
	{
    	lottery = actmoney * 2 + RandomNumber.IntLessThan(static_cast<uint>(actmoney));
        LastWeeksProfit += lottery;
    }*/

    // How much money has the club got after paying off loans
	uint dWeeklyLoanPayment = m_FinancialLoan.DoWeeklyPayment(GetCountry());
    actmoney = m_CashBalance - dWeeklyLoanPayment;
   	LastWeeksProfit -= dWeeklyLoanPayment;
   	m_pAccounts->GetAt(CAccountsItemList::LOANPAID).Plus(dWeeklyLoanPayment);

	// Receive interest from bank on cash, approx 5.25% per annum
    m_pAccounts->GetAt(CAccountsItemList::BANKINTEREST).Plus(m_CashBalance * .1 / 100);
    LastWeeksProfit += (m_CashBalance * .1 / 100);

    m_CashBalance += LastWeeksProfit;

    // Update the Year to date
    m_pAccounts->GetAt(CAccountsItemList::GATERECIEPTS).Plus(m_WeekEndMatch.GetGateReceipts() + m_MidWeekMatch.GetGateReceipts());
    m_WeekEndMatch.OnInitialise();
	m_MidWeekMatch.OnInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoClearTable
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoClearTable()
{
#ifdef	MATCHTRACKING
    if (IsRanking0Club())
	{
		InitialiseMatchTracking();
	}
#endif
	m_TableData.DoNewSeason();
	m_SeasonStats.DoNewSeason();
	if (m_ManagerID != NOMANAGER)
	{
		GetManager().DoClearTable();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAccountsPlus
	Access:    	public
	Parameter: 	const CAccountsItemList::eAccountItemType aWhich
	Parameter: 	const uint aHowMuch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAccountsPlus(const CAccountsItemList::eAccountItemType aWhich, const uint aHowMuch)
{
	m_pAccounts->GetAt(aWhich).Plus(aHowMuch);
	m_CashBalance += aHowMuch;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoNewSeason
	Access:    	public
	Parameter: 	const byte Position
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoNewSeason(const byte _Position)
{
    DoClearTable();
	m_TableData.SetLeaguePosition(_Position);
    if (IsRanking0Club())
	{
		m_pAccounts->DoAnnualClearDown();
	}
	DoInitialiseTicketPricesAndBonuses();
	m_squadFirstTeamPlayers.DoNewSeason();

	// Shirt Sponsorship
	DoAddToAccounts(GetDivision().GetDivisionStrength().GetSeasonShirtSponsorShipAmount(),
		IDS_ANNUALSHIRTSPONSORSHIP, CAccountsItemList::SPONSORSHIPINCOME);

	// Kit Sponsorship
	DoAddToAccounts(GetDivision().GetDivisionStrength().GetSeasonKitSponsorShipAmount(),
		IDS_ANNUALKITSPONSORSHIP, CAccountsItemList::SPONSORSHIPINCOME);

	// Television Income
	DoAddToAccounts(GetDivision().GetDivisionStrength().GetSeasonTVIncome(),
		IDS_ANNUALTELEVISIONINCOME, CAccountsItemList::TELEVISIONINCOME);

	if (IsUserControlled() != true)
	{
		m_squadFirstTeamPlayers.DoSelectNonUserControlledTeam(5, *this);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAddToAccounts
	Access:    	public
	Parameter: 	int _Amount
	Parameter: 	int _AmountStrId
	Parameter: 	const enum CAccountsItemList::eAccountItemType _Which
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAddToAccounts(int _Amount, int _AmountStrId, const enum CAccountsItemList::eAccountItemType _Which)
{
	CExtString str;
	str.Format(_AmountStrId, GetForeignStr(_Amount));
    AddToClubNews(str);
	DoAccountsPlus(_Which, _Amount);
	if (IsUserControlled() == true)
	{
//		afxDump << "Sponsorship " << m_CashBalance << " Amount "<< nAmount << "\n";
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsEmployeeSkillSuitable
	Access:    	public
	Parameter: 	const byte skill
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsEmployeeSkillSuitable(const byte _Skill)
{
    if (_Skill < GetPopularity().GetMinimumEmployeeSkill())
	{
		return false;
	}
    if (_Skill > GetPopularity().GetMaximumEmployeeSkill())
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPlayer
	Access:    	public
	Parameter: 	const ushort _SquadNumber
	Returns:   	CPlayer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer& CClub::GetPlayer(const ushort _SquadNumber)
{
	EXT_ASSERT(_SquadNumber < PLAYERSINSQUAD);
	return *WorldData().GetPlayer(m_squadFirstTeamPlayers.GetSquad(_SquadNumber));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPlayerID
	Access:    	public
	Parameter: 	const ushort _SquadNumber
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetPlayerID(const ushort _SquadNumber)
{
	EXT_ASSERT(_SquadNumber < PLAYERSINSQUAD);
	return m_squadFirstTeamPlayers.GetSquad(_SquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoEndOfSeason
	Access:    	public
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier:
	Purpose:	End of season processing,
				if the manager is human then show season  summary
------------------------------------------------------------------------------*/
void CClub::DoEndOfSeason(const ushort _ClubID)
{
    ushort nSeasonID = WorldData().GetText().DoFindStringID(GetActiveCountry().GetSeason());
	GetClubHistory().GetHighestDivision()->DoCheckIfHighest(m_DivisionID, nSeasonID);
    GetClubHistory().GetHighestPosition()->DoCheckIfHighest(m_TableData.GetLeaguePosition(), m_DivisionID, nSeasonID);
    GetClubHistory().GetLowestDivision()->DoCheckIfLowest(m_DivisionID, nSeasonID);
    GetClubHistory().GetLowestPosition()->DoCheckIfLowest(m_TableData.GetLeaguePosition(), m_DivisionID, nSeasonID);
    GetClubHistory().GetMostPoints()->DoCheckIfMost(m_TableData.GetPoints(), m_DivisionID, nSeasonID);
    GetClubHistory().GetLeastPoints()->DoCheckIfLeast(m_TableData.GetPoints(), m_DivisionID, nSeasonID);

	m_YouthSquad.DoEndOfSeason(this);
    m_StaffList.DoEndOfSeason(IsUserControlled(), this);
	m_squadFirstTeamPlayers.DoEndOfSeason(*this, _ClubID);
    if (IsUserControlled() == false)
	{
		if (GetCountryRanking() < 2)
		{
			m_squadFirstTeamPlayers.DoEnsureEnoughPlayers(*this);
		}
		// Ensure doesn't run out of money
		if (m_CashBalance < 100000)
		{
			m_CashBalance = GetPopularity().GetStartingClubBalance(GetCountry().GetFinancialRatioPercentage());
		}
		// Stop getting too much money
		if (m_CashBalance < static_cast<int>(GetPopularity().GetStartingClubBalance(GetCountry().GetFinancialRatioPercentage()) * 4))
		{
			m_CashBalance = GetPopularity().GetStartingClubBalance(GetCountry().GetFinancialRatioPercentage());
		}
	}

    DoSackStaffIfContractExpired(IsUserControlled());

	// Should we renew the manager's contract
	if (m_ManagerID != NOMANAGER)
	{
		if (GetManager().DoEndOfSeason() == true)
		{
			// Is retiring
            if (IsUserControlled() == false)
			{
				CExtString str;
				str.Format(IDS_MANRETIRED, GetName(), GetManagerName());
				GetActiveCountry().AddToNewsLists(str);			// Add to news
				GetManager().DoRetire();
				m_ManagerID = NOMANAGER;
			}
		}
		if (m_ManagerID != NOMANAGER)
		{
			if (GetManager().GetContractEndDate().SeasonsRemaining() < 1)
			{
				DoCheckSackManager();
			}
		}
	}
#ifdef ACTIVEFRIENDLIES
	GetFriendlies().OnNewSeason();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSackStaffIfContractExpired
	Access:    	public
	Parameter: 	bool _HumanManager
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSackStaffIfContractExpired(bool _HumanManager)
{
	if (m_StaffList.GetSize() > 0)
    {
	    CEmployee* pEmployee;
		for (int ListPos = m_StaffList.GetUpperBound(); ListPos >= 0; ListPos --)
		{
			pEmployee = static_cast<CEmployee*>(m_StaffList.GetAt(ListPos));
			EXT_ASSERT_POINTER(pEmployee, CEmployee);
			if (pEmployee->DoSackIfContractExpired(_HumanManager, *this) == true)
			{
				DoSackStaff(pEmployee, 0);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::NumberOfClubsInDivision
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetNumberOfClubsInDivision()
{
	return (byte)GetDivision().GetNumberOfClubsInDivision();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCheckSackManager
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	The manager's contract has expired,
				decide whether to offer him a new one
				N.B. if a human manager in scenario mode then no action taken
------------------------------------------------------------------------------*/
void CClub::DoCheckSackManager()
{
	DoCalculateManagerContractOffer();

	bool OfferNewContract = false;
	if (m_TableData.GetLeaguePosition() < (GetNumberOfClubsInDivision() / 2))
	{
		OfferNewContract = true;
	}
	else if (RandomNumber.IntLessThan((GetNumberOfClubsInDivision() - m_TableData.GetLeaguePosition()) + CHANCENOTRENEWMANAGERCONTRACT) == 1)
	{
		OfferNewContract = true;
	}

	if (GetManager().DoCalculateRating() > 80)
	{
		OfferNewContract = true;
	}

	if (OfferNewContract == true)
	{
		DoRenewManagerContract();
	}
	else
	{
		// Sack a manager
		DoSackManager();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRenewHumanManagerContract
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoRenewHumanManagerContract()
{
	if (WorldData().IsFastTest() == true)
	{
		GetManager().GetContractEndDate().IncreaseLength(2 + RandomNumber.IntLessThan(5));
		return;
	}

	int SaveManager = m_ManagerID;
	MakeCurrentUser();
	theApp.OnManagerRenewContract(this);
	if (m_ManagerID == NOMANAGER)
	{
		m_ManagerID = SaveManager;
		CExtString str;

		// Send thanks message to manager from board
		str.Format(IDS_NEWSMANAGERRESIGNED, GetManagerName(), GetName());
		DoManagerResigned();
		GetActiveCountry().AddToNewsLists(str);
		// Make computer managed immediately to stop human getting old job back
		if (GetCoach() != null)
		{
			DoTryCoachForManager();
		}
		else
		{
			DoTryPlayerForManager();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoConsiderManagerPositionAfterLeagueMatch
	Access:    	public
	Parameter: 	CFixture& aFixture
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoConsiderManagerPositionAfterLeagueMatch(CFixture& _Fixture)
{
	if (m_ManagerID != NOMANAGER)
	{
		if (GetManager().IsManagingClub() == false)
		{
			// Bug fix
			EXT_ASSERT(false);
			GetManager().DoNewClub(*this);
		}
        if (GetManager().DoPlayedLeagueMatch(_Fixture) == true)
		{
			DoSackManager();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetManager
	Access:    	public
	Parameter: 	void
	Returns:   	CManager&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CManager& CClub::GetManager()
{
	EXT_ASSERT(m_ManagerID != NOMANAGER);
    return WorldData().GetManager(m_ManagerID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRenewManagerContract
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoRenewManagerContract()
{
	EXT_ASSERT(m_ManagerID != NOMANAGER);
    if (IsUserControlled() == false)
	{
		// Is non human manager so renew contract
		GetManager().GetContractEndDate().IncreaseLength(2 + RandomNumber.IntLessThan(5));
	}
	else
	{
		DoRenewHumanManagerContract();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoHomeUpdateTables
	Access:    	public
	Parameter: 	CFixture& _Fixture
	Parameter: 	int PointsForAWin
	Parameter: 	bool HomeWinPenShootout
	Returns:   	void
	Qualifier:
	Purpose:	Update the league tables data
------------------------------------------------------------------------------*/
void CClub::DoHomeUpdateTables(CFixture& _Fixture, int _PointsForAWin, bool _HomeWinPenShootout)
{
	int OurGoals = _Fixture.GetNumberGoals(HOME);
	int TheirGoals = _Fixture.GetNumberGoals(AWAY);
	DoUpdateTables(_Fixture, OurGoals, TheirGoals, _PointsForAWin);
	m_TableData.DoHomeUpdateTable(OurGoals, TheirGoals, _PointsForAWin, GetDivision().GetPointsForDraw(), _HomeWinPenShootout);
#ifdef	MATCHTRACKING
	SetGameAttendance(GetTableData().GetHomeGames()->GetNumberGamesPlayed() - 1, _Fixture.GetAttendance());
	SetGateReceipts(GetTableData().GetHomeGames()->GetNumberGamesPlayed() - 1, _Fixture.GetAttendance() * _Fixture.GetTicketPrice());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAwayUpdateTables
	Access:    	public
	Parameter: 	CFixture& _Fixture
	Parameter: 	int PointsForAWin
	Parameter: 	bool HomeWinPenShootout
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAwayUpdateTables(CFixture& _Fixture, int _PointsForAWin, bool _HomeWinPenShootout)
{
	int OurGoals = _Fixture.GetNumberGoals(AWAY);
	int TheirGoals = _Fixture.GetNumberGoals(HOME);
	DoUpdateTables(_Fixture, OurGoals, TheirGoals, _PointsForAWin);
	m_TableData.DoAwayUpdateTable(TheirGoals, OurGoals, _PointsForAWin, GetDivision().GetPointsForDraw(), _HomeWinPenShootout);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoUpdateTables
	Access:    	public
	Parameter: 	CFixture & _Fixture
	Parameter: 	int _OurGoals
	Parameter: 	int _TheirGoals
	Parameter: 	int _PointsForAWin
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoUpdateTables(CFixture& _Fixture, int _OurGoals, int _TheirGoals, int _PointsForAWin)
{
	// Update manager records
	DoConsiderManagerPositionAfterLeagueMatch(_Fixture);
	if (IsRanking0Club())
	{
		m_pCurrentForm->DoUpdateCurrentForm(_OurGoals, _TheirGoals, _PointsForAWin, GetDivision().GetPointsForDraw());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetSeasonStats
	Access:    	public
	Parameter: 	void
	Returns:   	CClubSeasonStats&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClubSeasonStats& CClub::GetSeasonStats()
{
	return m_SeasonStats;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetYouthSquad
	Access:    	public
	Parameter: 	void
	Returns:   	CYouthList&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CYouthList&	CClub::GetYouthList()
{
	return m_YouthSquad;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetEmployeeList
	Access:    	public
	Parameter: 	void
	Returns:   	CEmployeeList&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeList& CClub::GetEmployeeList()
{
	return m_StaffList;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetEmployee
	Access:    	public
	Parameter: 	const ushort _ListPos
	Returns:   	CEmployee*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CEmployee* CClub::GetEmployee(const ushort _ListPos)
{
	EXT_ASSERT(m_StaffList.GetAt(_ListPos) != null);
	return static_cast<CEmployee*>(m_StaffList.GetAt(_ListPos));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetFirstTeamSquad
	Access:    	public
	Parameter: 	void
	Returns:   	CPlayerSquad&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSquad& CClub::GetFirstTeamSquad()
{
	return m_squadFirstTeamPlayers;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetManagerWageOfferStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetManagerWageOfferStr()
{
	return GetForeignStr(m_ManagerContract.GetWageOffer());;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::PromoteYouth
	Access:    	public
	Parameter: 	const ushort YouthNum
	Parameter: 	CContract* Contract
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::PromoteYouth(const ushort YouthNum, CContract* Contract)
{
	m_squadFirstTeamPlayers.PromoteYouth(Contract, m_YouthSquad.GetAt(YouthNum));
	DoPaySignOnFee(Contract->GetSignOnFee());
	m_YouthSquad.Remove(YouthNum);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSelectRandomTactics
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSelectRandomTactics()
{
	static const eFormation sceForms[] =
	{
		FORMATION_442, FORMATION_433, FORMATION_442, FORMATION_433,
		FORMATION_442, FORMATION_334, FORMATION_433, FORMATION_433,
		FORMATION_343, FORMATION_532, FORMATION_532
	};
	SetCurrentTacticsID(sceForms[RandomNumber.IntLessThan(10)]);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::OnInitialise
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::PostInitialise()
{
	m_squadFirstTeamPlayers.DoCalculateAverageStrengths();
	m_squadFirstTeamPlayers.DoSelectTeam(5, *this);
	if (IsRanking0Club())
	{
		// Initialise the employees
		m_StaffList.Add(new CEmployee(CEmployeeBase::COACH, GetCountry(), GetPopularity()));
		m_StaffList.Add(new CEmployee(CEmployeeBase::PHYSIO, GetCountry(), GetPopularity()));
#ifdef USESCOUTS
			m_StaffList.Add(new CEmployee(CEmployeeBase::SCOUT, rCountry, pPtr));
			if (RandomNumber.IntLessThan(10) < 9)
			{
				m_StaffList.Add(new CEmployee(CEmployeeBase::SCOUT, rCountry, pPtr));
			}
#endif
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::ManagerResigned
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoManagerResigned()
{
	EXT_ASSERT(m_ManagerID != NOMANAGER);
    if (IsUserControlled() == true)
	{
		GetUser().LeftClub();
	}
	GetManager().DoLeaveClub();
	m_ManagerID = NOMANAGER;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsYouthAtClub
	Access:    	public
	Parameter: 	CYouth* pYouth
	Returns:   	bool
	Qualifier:
	Purpose:	Is the requested youth at this club?
------------------------------------------------------------------------------*/
bool CClub::IsYouthAtClub(CYouth* _Youth)
{
    EXT_ASSERT_POINTER(_Youth, CYouth);
    for (int LoopCount = 0 ; LoopCount < m_YouthSquad.GetSize(); LoopCount ++)
	{
        if (m_YouthSquad.GetAt(LoopCount) == _Youth)
		{
			return true;
		}
	}

    return false;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::IsEmployeeAtClub
	Access:    	public
	Parameter: 	CEmployee* _pEmployee
	Returns:   	bool
	Qualifier:
	Purpose:	Is the requested employee at this club?
------------------------------------------------------------------------------*/
bool CClub::IsEmployeeAtClub(CEmployee* _pEmployee)
{
	EXT_ASSERT(_pEmployee != null);

    for (int LoopCount = 0 ; LoopCount < m_StaffList.GetSize(); LoopCount ++)
	{
        if (static_cast<CEmployee*>(m_StaffList.GetAt(LoopCount)) == _pEmployee)
		{
			return true;
		}
	}

    return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetManagerName
	Access:    	public
	Parameter: 	void
	Returns:   	CString - manager's name, if the club has no manager then returns ""
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetManagerName()
{
	if (m_ManagerID == NOMANAGER)
	{
		return TEXT("");
	}
	else
	{
        return GetManager().GetName();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsGoodClub
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:	Determine whether club is a leading club or not
				Used for deciding whether employees will join or not
------------------------------------------------------------------------------*/
bool CClub::IsGoodClub()
{
    if (GetPopularity().IsGoodClub() || GetDivision().IsTopDivision() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::GetUser
	Access:    	public
	Parameter: 	void
	Returns:   	CUser&
	Qualifier:
	Purpose:	DoFindID the human manager pointer
------------------------------------------------------------------------------*/
CUser& CClub::GetUser()
{
	return WorldData().GetUserList().FindManager(m_ManagerID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetNonUser
	Access:    	public
	Parameter: 	void
	Returns:   	CUser&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CUser& CClub::GetNonUser()
{
	return WorldData().GetUserList().GetAt(0);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::UserNumber
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetUserNumber()
{
	ushort iRet = NOMANAGER;
	for (int LoopCount = 0; LoopCount < MAXHUMANMANAGERS; LoopCount++)
	{
		{
			iRet = LoopCount;
			break;
		}
	}
	EXT_ASSERT(iRet != NOMANAGER);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::MakeCurrentUser
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::MakeCurrentUser()
{
	WorldData().SetCurrentUserID(GetUserNumber());
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsUserControlled
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:	Is the club managed by a human?
------------------------------------------------------------------------------*/
bool CClub::IsUserControlled()
{
	if (m_ManagerID == NOMANAGER)
	{
		return false;
	}
	return GetManager().IsUser();
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::InSeason
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::InSeason()
{
	if (m_TableData.GetNumberGamesPlayed() == 0 || GetDivision().GetNumberGamesPerSeason() == m_TableData.GetNumberGamesPlayed())
	{
		return false;
	}
	return true;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTrySellHumanManagedPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	If the club is managed by a user then get other clubs to make bids
				for any transfer listed players
------------------------------------------------------------------------------*/
void CClub::DoTrySellHumanManagedPlayers()
{
    // Only if club is user managed
    if (IsUserControlled() == false || m_squadFirstTeamPlayers.DoCountAvailablePlayers() < MINSQUADSIZE)
	{
		return;
    }

    CPlayer* pPlayer;
	CContract ContractRequired;

	// Bid for non-listed 1st team player?
	uint dBidAmount = 0;
	if (RandomNumber.IntLessThan(BID1STCHANCE) == 2)
	{
		ushort count = RandomNumber.IntLessThan(PLAYERSINTEAM);
        pPlayer = &GetPlayer(count);
		// Ensure player is not on loan etc
		if (pPlayer->WillingToTransfer() == true)
		{
			dBidAmount = pPlayer->GetValuation(GetCountry());
			// Make bid attractive(large)
			if (RandomNumber.IntLessThan(CHANCEBIGBID) == 3 && pPlayer->GetTransferStatus().GetAskingPrice() < 1 && dBidAmount < 3000000)
			{
				dBidAmount *= 2;
			}
			CClub* pBuyingClub = GetDivision().DoFindRandomNonUserBuyingClub(this, pPlayer, dBidAmount);
			if (pBuyingClub != null)
			{
				if (pBuyingClub->AreBuyTransfersAllowed() == true)
				{
					CClubTransferOffer TheOffer(pPlayer, dBidAmount, pBuyingClub);
					if (DoTryAcceptBidFromNonUser(TheOffer) == true)
					{
						DoSoldPlayer(*pPlayer, TheOffer, ContractRequired, false);	// Sell the player
					}
				}
				else if (pBuyingClub->AreLoanTransfersAllowed() == true)
				{
					CClubTransferOffer TheOffer(pPlayer, dBidAmount, pBuyingClub);
					if (DoTryAcceptBidFromNonUser(TheOffer) == true)
					{
						DoSoldPlayer(*pPlayer, TheOffer, ContractRequired, false);	// Sell the player
					}
				}
			}
		}
	}

	uint dLastBid;
	CClubTransferOffer TheOffer;
	CClub* pLoaningClub;
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
		dLastBid = 0;
		if (m_squadFirstTeamPlayers.DoCountAvailableNonLoanedPlayers() > MINSQUADSIZE && m_squadFirstTeamPlayers.GetSquad(count) != NOPLAYER && GetPlayer(count).IsAvailableForMatch() == true)
		{
			if (RandomNumber.IntLessThan(SELLATTEMPTS) == 1)
			{
				pPlayer = &GetPlayer(count);
				if (pPlayer->IsTransferListed() == true && AreBuyTransfersAllowed() == true)
				{
					for (uint Try = 0; Try < 4; Try++)
					{
						TheOffer.DoInitialise(pPlayer, dLastBid);
						dLastBid = DoTrySellPlayerIfListed(TheOffer);
						if (dLastBid == 0)
						{
							break;			// Sold
						}
					}
				}
				else if (pPlayer->IsLoanListed() == true)
				{
					pLoaningClub = pPlayer->DoFindSuitableNewClub(this);
					if (pLoaningClub != null)
					{
						TheOffer.DoInitialise(pPlayer, 4 + RandomNumber.IntLessThan(4), pLoaningClub);
						theApp.OnReceiveOfferForPlayer(&TheOffer);

						WorldData().RestoreProgressDuringDailyTasks();
						if (TheOffer.IsDealCompleted() == true)
						{
							pPlayer->DoGoingOnLoan(this, TheOffer.GetLoanPeriod());
							DoPlayerGoingOutOnLoan(m_squadFirstTeamPlayers.GetSquad(count));
							pLoaningClub->GotPlayerOnLoan(m_squadFirstTeamPlayers.GetSquad(count), TheOffer.GetLoanPeriod());
						}
					}
				}
			}
		}
	}					// Next player in squad

	WorldData().RestoreProgressDuringDailyTasks();
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTrySellPlayerIfListed
	Access:    	public
	Parameter: 	CClubTransferOffer& rTheOffer
	Returns:   	uint - the bid amount if unsuccessful or zero or 'NOTFOUND'
				if no buying club found
	Qualifier:
	Purpose:	If player is on transfer list then try to sell him (transfer fee
				and buying club are generated here)
				Selling club is human and buying clubs are non human managed
------------------------------------------------------------------------------*/
 uint CClub::DoTrySellPlayerIfListed(CClubTransferOffer& _TheOffer)
{
	EXT_ASSERT(IsUserControlled() == true);
	EXT_ASSERT(AreBuyTransfersAllowed() == true);
	uint dRet = NOTFOUND;
	CPlayer* pPlayer = &_TheOffer.GetPlayer();
	if (pPlayer->IsTransferListed() == true)
	{
		CClub* pBuyingClub = pPlayer->DoFindSuitableNewClub(this);
		if (pBuyingClub != null)
		{
			_TheOffer.SetBidClub(pBuyingClub);
			CContract ContractRequired;
			if (pPlayer->QualifiesForBosman() == true && pBuyingClub->InterestedInPlayer(pPlayer, 0) == true)
			{
				// No fee required;
				_TheOffer.SetAmountOffered(0);
				DoSoldPlayer(*pPlayer, _TheOffer, ContractRequired, true);	// Player leaves under the Bosman ruling
				dRet = 0;
			}
			else
			{
				uint BidAmount = pPlayer->DoCalculateMaximumTransferFee(GetCountry());
				if (BidAmount > 0 && BidAmount > pPlayer->GetTransferStatus().GetAskingPrice() * .8)
				{
					BidAmount = pPlayer->DoCalculateAdjustedFee(GetCountry());
					if (pBuyingClub->InterestedInPlayer(pPlayer, BidAmount) == true)
					{
						BidAmount = pBuyingClub->GetCountry().GetCurrency().RoundedPounds(min(BidAmount, max((uint)0, pPlayer->GetTransferStatus().GetAskingPrice())));
						// Ask the user if he wishes to accept the bid
						if (BidAmount > _TheOffer.GetAmountOffered())
						{
							_TheOffer.SetAmountOffered(BidAmount);
							if (DoTryAcceptBidFromNonUser(_TheOffer) == true)
							{
								DoSoldPlayer(*pPlayer, _TheOffer, ContractRequired, false);	// Sell the player
								dRet = 0;
							}
							else
							{
								dRet = BidAmount;
							}
						}
					}
				}
			}
		}
	}
	return dRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::InterestedInPlayer
	Access:    	public
	Parameter: 	CPlayer* pPlayer
	Parameter: 	const uint BidAmount
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::InterestedInPlayer(CPlayer* _Player, const uint _BidAmount)
{
	EXT_ASSERT(IsUserControlled() == false);
	EXT_ASSERT_POINTER(_Player, CPlayer);
	// Check if room in squad
	if (m_squadFirstTeamPlayers.AllowToHirePlayer(_Player) != 0 ||
		GetPopularity().IsPlayerSkillInRangeNonUserBid(_Player->GetSkills().GetOverallSkill(), GetDivisionStrengthID()) == false)
	{
		return false;
	}

	// Check if better than those in team already
	if (m_squadFirstTeamPlayers.DoCalculateAverageSkill(_Player->GetSkills().GetPosition()) <= _Player->GetSkills().GetOverallSkill())
	{
//		return false;
	}

	// Check if got enough money
	if (static_cast<int>(_BidAmount) > (m_CashBalance + m_CashBalance / 4))
	{
		DoApplyForFinancialLoan();
	}
	if (static_cast<int>(_BidAmount) > (m_CashBalance + m_CashBalance / 4))
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSoldPlayer
	Access:    	public
	Parameter: 	CPlayer& _Player
	Parameter: 	CClubTransferOffer& rTheOffer
	Parameter: 	CContract& _ContractRequired
	Parameter: 	bool IsBosman
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSoldPlayer(CPlayer& _Player, CClubTransferOffer& _TheOffer, CContract& _ContractRequired, bool _IsBosman /*= false*/)
{
	EXT_ASSERT(_TheOffer.GetBidClub().AreBuyTransfersAllowed() == true);
	EXT_ASSERT(&_TheOffer.GetBidClub() != this && _TheOffer.GetBidClubID() != NOCLUB);

	_TheOffer.GetPlayer().DoGenerateRandomContract(_ContractRequired, GetCountry());
	if (&_TheOffer.GetBidClub().GetCountry() != &GetCountry())
	{
		// Player has moved countries
		_TheOffer.GetPlayer().DoNewSeason();
	}

    bool RecordSigning = _TheOffer.GetBidClub().DoBuyPlayer(_Player, _TheOffer.GetAmountOffered(), _ContractRequired, GetNameID());
    if (_TheOffer.GetBidClub().IsUserControlled() == true)
	{
	    CExtString str;
        str.Format(IDS_SIGNFORCLUB, _Player.GetName());
		_TheOffer.GetBidClub().AddToClubNews(str);
        _TheOffer.GetBidClub().GetFirstTeamSquad().DoFansReactionToSigning(_Player);
    }
	DoSellPlayer(_Player, _TheOffer.GetAmountOffered(), _TheOffer.GetBidClub(), RecordSigning, _IsBosman);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoApplyForFinancialLoan
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:	Non user managed club requires a loan
------------------------------------------------------------------------------*/
bool CClub::DoApplyForFinancialLoan()
{
    EXT_ASSERT(IsUserControlled() == false);
	// Ensure not got a loan already
	int HowLong = 1 + RandomNumber.IntLessThan(4);
    uint HowMuch = (GetPopularity().GetLoanAmount() / 2) * HowLong - m_FinancialLoan.GetLoanOwed();
	if (HowMuch < 100)
	{
		return false;
	}
	HowLong += RandomNumber.IntLessThan(HowLong);
 	m_CashBalance += HowMuch;
	m_FinancialLoan.DoAcceptLoan(HowMuch, GetCountry(), HowLong);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryBuyPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	If the club is not managed by a user then make bids for any
				transfer listed players
------------------------------------------------------------------------------*/
void CClub::DoTryBuyPlayers()
{
	if (AreBuyTransfersAllowed() == false)
	{
		return;
	}

	// Only if club is not user managed and if there is room in the squad
    if (IsUserControlled() == true || m_squadFirstTeamPlayers.TotalSquadSize() == PLAYERSINSQUAD)
	{
		return;
    }

	if (TooManyPointsPerGame() == true && m_squadFirstTeamPlayers.DoCountAvailablePlayers() > MINSQUADSIZE)
	{
		if (RandomNumber.IntLessThan(100) < 6)
		{
			DoSellARandomPlayer(true);
			return;
		}
		else if (RandomNumber.IntLessThan(100) < 20)
		{
			// Give a player a serious injury
			CPlayer* pPlayer = m_squadFirstTeamPlayers.DoFindRandomAttackingPlayer();
			if (pPlayer != null)
			{
				pPlayer->DoTryInjurePlayerInTraining(*this, CInjuryType::MAJOR);
			}
			return;
		}
	}
	CPlayerSearch PlayerSearch;
	PlayerSearch.SetFindPosition(m_squadFirstTeamPlayers.DoFindWeakestPosition());
	DoCreateBuyPlayerSearch(PlayerSearch);
	DoTryBuyPlayer(PlayerSearch);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSubmitCounterAskingPrice
	Access:    	public
	Parameter: 	CClubTransferOffer* pBidDetails
	Parameter: 	CClubTransferOffer& CounterBidDetails
	Returns:   	bool - true if prepared to meet the new asking price
	Qualifier:
	Purpose:	CounterBidDetails deal is set to complete if there is a new offer
				in pBidDetails
------------------------------------------------------------------------------*/
bool CClub::DoSubmitCounterAskingPrice(CClubTransferOffer* _BidDetails, CClubTransferOffer& _CounterBidDetails)
{
	bool bRet = false;
	if (_CounterBidDetails.GetSellOnPercentOffered() <= _BidDetails->GetSellOnPercentOffered() &&
		_BidDetails->GetPlayer().DoGenerateSellOnClauseForPlayer() >= _BidDetails->GetSellOnPercentOffered())
	{
		if (_CounterBidDetails.GetAmountOffered() <= _BidDetails->GetAmountOffered())
		{
			bRet = true;
			*_BidDetails = _CounterBidDetails;
		}
		else if((_BidDetails->GetPlayer().DoCalculateMaximumTransferFee(GetCountry()) > _CounterBidDetails.GetAmountOffered())) // &&
		{
			// If we can afford the counter price
			if (m_CashBalance >= static_cast<int>(_CounterBidDetails.GetAmountOffered()))
			{
				// If the counter offer is less than 50% more than we offered
				if (RandomNumber.IntLessThan(CHANCEREFUSECOUNTER) != 1 && (_CounterBidDetails.GetAmountOffered() < _BidDetails->GetAmountOffered() * 1.5))
				{
					// Agree to meet the counter demand
					bRet = true;
					*_BidDetails = _CounterBidDetails;
				}
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSubmitCounterLoanPeriod
	Access:    	public
	Parameter: 	CClubTransferOffer * _OriginalOfferDetails
	Parameter: 	const int _NewPeriod
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoSubmitCounterLoanPeriod(CClubTransferOffer* _OriginalOfferDetails, const int _NewPeriod)
{
	bool bRet = true;
	// When refusal code is added remember to modify Screen44DLlg to suit
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TooManyPointsPerGame
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::TooManyPointsPerGame()
{
	if (DoCountLeagueGamesPlayedThisSeason() < MINGAMESBEFORESACKING)
	{
		return  false;
	}
	if (m_TableData.GetPoints() / DoCountLeagueGamesPlayedThisSeason() > 2.4)
	{
		return true;
	}
	if (m_TableData.GetLeaguePosition() == 0 && (m_TableData.GetPoints() > GetDivision().PointsOfClubInPosition(1) + 12))
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoBidForPlayer
	Access:    	public
	Parameter: 	ushort _PlayerBiddingFor
	Parameter: 	ushort _PlayerToBidForClub
	Returns:   	bool
	Qualifier:
	Purpose:	Bid for specified player from specified club
------------------------------------------------------------------------------*/
bool CClub::DoBidForPlayer(ushort _PlayerBiddingFor, ushort _PlayerToBidForClub)
{
	EXT_ASSERT(WorldData().GetPlayer(_PlayerBiddingFor)->GetContractRenewalStatus() != RETIRING);
	if (WorldData().GetPlayer(_PlayerBiddingFor)->GetClub().IsUserControlled() == true && WorldData().GetPlayer(_PlayerBiddingFor)->GetClub().GetManager().GetWeeksHere() < 2)
	{
		// Dont bid for user players too early into his career
		return false;
	}
	bool bRet = false;
	uint HowMuch = GetCountry().GetCurrency().RoundedPounds(WorldData().GetPlayer(_PlayerBiddingFor)->GetValuation(GetCountry()));
	CClubTransferOffer TheOffer(WorldData().GetPlayer(_PlayerBiddingFor), HowMuch, this);
	if (WorldData().GetPlayer(_PlayerBiddingFor)->WillingToTransfer() == true)
	{
		// Check first to ensure player hasn't refused
		if (m_RefusedDeals.DoCheckIfPlayerRefusedTransfer(_PlayerBiddingFor) == false)
		{
			if (WorldData().GetPlayer(_PlayerBiddingFor)->QualifiesForBosman() == true)
			{
				// Sign the player on a Bosman
				TheOffer.SetAmountOffered(0);
				bRet = true;
			}
			else if (WorldData().GetClub(_PlayerToBidForClub).DoTryAcceptBidFromNonUser(TheOffer) == true)
			{
				// Bought the player
				bRet = true;
			}
			if (bRet == true)
			{
				CContract ContractRequired;
				WorldData().GetClub(_PlayerToBidForClub).DoSoldPlayer(*WorldData().GetPlayer(_PlayerBiddingFor), TheOffer, ContractRequired, false);
			}
		}
	}
	return bRet;
}


 /*------------------------------------------------------------------------------
	Method:   	CClub::CountPlayersOnPitch
	Access:    	public
	Parameter: 	void
	Returns:   	ushort - number of players of onfield squad positions who are fit
				and have not been suspended or sent off
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::DoCountPlayersOnPitch()
{
	ushort Count = 0;
	for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		Count += (GetPlayer(LoopCount).IsAvailableForMatch() == true);
	}
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsValidUserControlledTeam
	Access:    	public
	Parameter: 	const byte aNumSubs
	Returns:   	bool
	Qualifier:
	Purpose:	Has got a human manager so warn if players selected for the match
				are not available due to injury or suspension
------------------------------------------------------------------------------*/
bool CClub::IsValidUserControlledTeam(const byte _NumSubs)
{
	CString str;
    CPlayer* pPlayer;
	bool bRet = true;
    if (m_squadFirstTeamPlayers.DoCountAvailablePlayers() >= (PLAYERSINTEAM + _NumSubs))
	{
		for (uint nSquadNumber = 0; nSquadNumber < PLAYERSINTEAM + _NumSubs; nSquadNumber++)
		{
            pPlayer = &GetPlayer(nSquadNumber);
			if (pPlayer->IsInjured() == true)
			{
				// Notify user of injured players in the team
				str.Format(IDS_INJPLAYTEAM, nSquadNumber + 1, pPlayer->GetName());
			}
			else if (pPlayer->GetDiscipline().GetMatchesSuspended() > 0)
			{
				// Notify user of suspended players in the team
				str.Format(IDS_SUSPPLAYTEAM, nSquadNumber + 1, pPlayer->GetName());
			}

			if (str.IsEmpty() == FALSE)
			{
				UserMessageBox(str, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1);
				bRet = false;
				break;
			}
		}
	}
	else
	{
		UserMessageBox(IDS_INSUFFICIENTFITPLAYERS);
		bRet = false;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSelectTeam
	Access:    	public
	Parameter: 	const byte _NumSubs
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSelectTeam(const byte _NumSubs)
{
    if (IsUserControlled() == true && WorldData().IsFastTest() == false)
	{
    //WorldData().SaveCurrentUser(m_ManagerID);
		while (IsValidUserControlledTeam(_NumSubs) == false)
		{
			theApp.OnViewSquadDetails();
		}
	//WorldData().RestoreCurrentUser();
    }
	else
	{
		m_squadFirstTeamPlayers.DoSelectNonUserControlledTeam(_NumSubs, *this);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::NewContractRequests
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	See if any player or employee wishes to request a contract
				extension or announces he is going to retire at the end of his
				contract
------------------------------------------------------------------------------*/
void CClub::NewContractRequests()
{
	m_squadFirstTeamPlayers.NewContractRequests();
	m_StaffList.DoNewContractRequests(*this);
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::TryRetirePlayersThroughInjury
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Player retiring through injury immediately,
				Must have at least 15 players in squad
------------------------------------------------------------------------------*/
void CClub::TryRetirePlayersThroughInjury()
{
	ushort ThePlayerID;
	if (RandomNumber.IntLessThan(CHANCERETIREPLAYERSTHROUGHINJURY) == 1)
	{
    	// have three attempts
        for (uint num = 0; num < RETIREPLAYERSTHROUGHINJURYATTEMPTS; num ++)
		{
            ThePlayerID = RandomNumber.IntLessThan(PLAYERSINSQUAD - 1);
			if (m_squadFirstTeamPlayers.GetSquad(ThePlayerID) != NOPLAYER)
			{
                GetPlayer(ThePlayerID).DoesPlayerRetireDoToInjury();
              	break;          // Only 1 player
			}
      	}
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::OnPlayerAskedForMove
	Access:    	public
	Parameter: 	CPlayer * _Player
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::OnPlayerAskedForMove(CPlayer* _Player)
{
    if (m_squadFirstTeamPlayers.DoCountAvailableNonLoanedPlayers() <= MINSQUADSIZE || WorldData().IsFastTest() == true)
	{
		return;
	}
	EXT_ASSERT_POINTER(_Player, CPlayer);

	// Is the manager willing to grant the request ?
    if (IsUserControlled() == true)
	{
		theApp.OnPlayerRequestTransfer(_Player);
	}
	else
	{
		bool bRequestGranted = false;
		if (_Player->GetSkills().GetPosition() != GOALKEEPER && GetFirstTeamSquad().DoCountAvailablePlayersPosn(GOALKEEPER) > 2)
		{
			bRequestGranted = true;
		}
		else if (RandomNumber.IntLessThan(50) < 25)
		{
			bRequestGranted = true;
		}

		if (bRequestGranted == true)
		{
			_Player->OnTransferRequestGranted(GetCountry());
			// transfer request granted
			CExtString str;
			if (RandomNumber.IntLessThan(10) < 6)
			{
				str.Format(IDS_REQUESTTRANSFER, _Player->GetName(), GetName());
			}
			else
			{
				str.Format(IDS_UNHAPPYLISTED, _Player->GetName(), GetName());
			}
			AddToNewsList(str);
		}
		else
		{
			_Player->OnTransferRequestRefused();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AddToNewsList
	Access:    	public
	Parameter: 	CExtString Message
	Returns:   	void
	Qualifier:
	Purpose:	Add the message to the country news list if there is a user
				managing a club in the same division
------------------------------------------------------------------------------*/
void CClub::AddToNewsList(const CExtString& _Message)
{
	if (IsUserControlled() == false && GetDivision().DoFindUserClub() != NULL)
	{
		GetActiveCountry().AddToNewsLists(_Message);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TryPlayerForManager
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoTryPlayerForManager()
{
    CPlayer* pPlayer;
    bool bRet = false;
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_squadFirstTeamPlayers.GetSquad(count) != NOPLAYER)
		{
            pPlayer = &GetPlayer(count);
			if (pPlayer->BecomeManager() == true)
			{
                m_ManagerID = WorldData().GetManagerList().DoAddNewManager(pPlayer, m_ManagerContract.GetWageOffer() / 52, m_ManagerContract.GetContractOffer(), DoFindID());
				m_squadFirstTeamPlayers.DoRemoveFromSquad(count);
                pPlayer->DoRetired(DoFindID());
                bRet = true;
				break;
			}
		}
	}
    return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::pCoach
	Access:    	public
	Parameter: 	void
	Returns:   	CEmployee*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CEmployee* CClub::GetCoach()
{
	ushort num = GetCoachStaffNumber();
	if (num != wNOTFOUND)
	{
        return static_cast<CEmployee*>(m_StaffList.GetAt(num));
	}
	return null;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetCoachStaffNumber
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetCoachStaffNumber()
{
	ushort RetVal = wNOTFOUND;
    for (int num = 0; num < m_StaffList.GetSize(); num ++)
	{
		if (static_cast<CEmployee*>(m_StaffList.GetAt(num))->GetType() == CEmployeeBase::COACH)
		{
			RetVal = num;
			break;
		}
	}
	return RetVal;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPhysioStaffNumber
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::GetPhysioStaffNumber()
{
	ushort RetVal = wNOTFOUND;
    for (int num = 0; num < m_StaffList.GetSize(); num ++)
	{
        if (static_cast<CEmployee*>(m_StaffList.GetAt(num))->GetType() == CEmployeeBase::PHYSIO)
		{
        	RetVal = num;
			break;
		}
	}
	return RetVal;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryCoachForManager
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	If the club coach is ok then may make him manager
------------------------------------------------------------------------------*/
void CClub::DoTryCoachForManager()
{
 	if (GetCoachStaffNumber() != wNOTFOUND && RandomNumber.IntLessThan(OFFERCOACHMANAGERJOB) == 1)
	{
		// Create manager object from the employee object
		m_ManagerID = WorldData().GetManagerList().Add(new CManager(GetCoach(), m_ManagerContract.GetWageOffer() / 52, m_ManagerContract.GetContractOffer(), DoFindID()));
		// Delete employee object
		CEmployee * coach = GetCoach();
		// Remove from club employee list
		m_StaffList.RemoveAt(GetCoachStaffNumber());
		delete coach;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsUserGoodEnoughToManageClub
	Access:    	public
	Parameter: 	int UserNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsUserGoodEnoughToManageClub(int UserNumber)
{
	return WorldData().GetUserList().GetAt(UserNumber).IsGoodEnoughToManageClub(m_PopularityID);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSackManager
	Access:    	public
	Parameter: 	bool AnnounceUserSacking
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoSackManager(bool _AnnounceUserSacking /*= true*/)
{
	bool WasUserControlled = IsUserControlled();
	if (WorldData().IsFastTest() == true && WasUserControlled == true)
	{
		GetManager().GetContractEndDate().IncreaseLength(2 + RandomNumber.IntLessThan(5));
		return;
	}
	if (_AnnounceUserSacking == true)
	{
		CExtString str;
		if (WasUserControlled == true)
		{
			// Sack the human manager
			if (GetUser().GetManager().CheckIfGotContract() == false)
			{
				str.Format(IDS_HUMANMANAGERSACKED, GetManagerName(), GetName());
			}
			else
			{
				str.Format(IDS_SACKUSERMANAGER, GetName());
			}
			theApp.OnUserManagerLeaveClubDlg(str);
			if (GetCountryRanking() < 2)
			{
				m_squadFirstTeamPlayers.DoEnsureEnoughPlayers(*this);
			}
			GetUser().LeftClub();
		}
		else
		{
			static const int MessIDs[] =
			{
				IDS_MANLEFT1, IDS_MANLEFT2, IDS_MANLEFT3,
				IDS_MANLEFT4, IDS_MANLEFT5, IDS_MANLEFT6
			};
			str.Format(MessIDs[RandomNumber.IntLessThan(5)], GetManagerName(), GetName());
			GetActiveCountry().AddToNewsLists(str);
		}
	}
#ifdef MANAGEREXPENDITURE
	if (GetManager().NextWeeksBalanceIfNoClub() > 0)
	{
		// Can't add to news if will be retired from game as the manager will be deleted
		//@@GetDivision().NewsPaperEventPtr()->SetManagerSackedNumber(m_ManagerID);
	}
#endif
	GetManager().DoLeaveClub();
	m_ManagerID = NOMANAGER;

    if (WasUserControlled == true)
	{
		GetActiveCountry().RemoveStaffYouths();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTrySackManager
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoTrySackManager()
{
	if (m_TableData.GetNumberGamesPlayed() > MINGAMESBEFORESACKING && ManagerWeeksAtClub() > MINWEEKSATCLUB)
	{
		if (RandomNumber.IntLessThan(MANAGERSACKCHANCE) == MANAGERSACKCHANCE / 2 ||
			(GetDivision().NumberOfPointsToEscapeRelegation(this) > MANAGERSACKIFPOORLEAGUEPOSITION && RandomNumber.IntLessThan(10) < 3))
		{
            // Check to see if at bottom of division and how far adrift
			if (m_TableData.GetLeaguePosition() == GetNumberOfClubsInDivision() && ((m_TableData.GetPoints() + 12) < GetDivision().PointsOfClubInPosition(m_TableData.GetLeaguePosition() - 1)))
			{
				SetCurrentTacticsID(FORMATION_433);
				DoTryBuyPlayers();
			}
			else if (RandomNumber.IntLessThan(100) < 15)
			{
				DoSackManager();
			}
			else if (GetCurrentTacticsID() == FORMATION_442)
			{
				SetCurrentTacticsID(FORMATION_433);
			}
			else
			{
				DoTryBuyPlayers();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoEndOfWeek
	Access:    	public
	Parameter: 	bool NoWarnings
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoEndOfWeekTasks(bool _NoWarnings)
{
	if (IsRanking0Club())
	{
		m_RefusedDeals.DoEndOfWeekTasks();
#ifdef	ACTIVESTADIUMPLANS
		GetStadium()->DoEndOfWeek();
#endif
		DoWeeklyFinance();
	}
	m_BidsThisWeek = 0;
	ushort num = 0;
	// Number of players available for next match is low!!
	if (WorldData().IsFastTest() != true)
	{
		m_squadFirstTeamPlayers.DoWeeklyUpdate(*this);
	    if (IsUserControlled() != true)
		{
		    // Buy enough players to bring the squad upto size
			while (num++ < 4 && m_squadFirstTeamPlayers.TotalSquadSize() < PLAYERSINSQUAD && m_squadFirstTeamPlayers.DoCountAvailablePlayers() <= MINSQUADSIZE)
			{
				DoTryBuyPlayers();
			}
			DoEnsureEnoughFitPlayers();
		}
	}

	if (IsUserControlled() == true)
	{
		GetUser().GetTacticsList().GetAt(m_SquadTrainingDetails.GetTactics().GetSelection()).DoTrain(m_SquadTrainingDetails.GetTacticsToTrainHours());
		GetActiveCountry().DoStaffAndYouthEndOfWeek(GetCountry(), &GetPopularity(), GetDivisionStrengthID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetRecommendedLoanRepayment
	Access:    	public
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetRecommendedLoanRepayment()
{
	if (m_CashBalance > 0)
	{
		return min(m_FinancialLoan.GetLoanOwed(), (uint)m_CashBalance);
	}
	// Got no cash
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::AgreeToExtendPlayerLoan
	Access:    	public
	Parameter: 	CPlayer& aPlayer
	Parameter: 	CString ClubName
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoAgreeToExtendPlayerLoan(CPlayer& _Player, const CString &_ClubName)
{
	// Not if no contract
	if (_Player.GetContractEndDate().SeasonsRemaining() < 1)
	{
		return false;
	}

    if (IsUserControlled() != true)
	{
		if (RandomNumber.IntLessThan(CHANCENOTEXTENDPLAYERLOANPERIOD) == 1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		CString Str;
		Str.Format(IDS_AGREETOEXTENDPLAYERLOAN, GetManagerName(), _Player.GetName(), _ClubName);
		if (UserMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDNO)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCheckSendPlayerBackFromLoan
	Access:    	public
	Parameter: 	const ushort SquadNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::DoCheckSendPlayerBackFromLoan(ushort _SquadNumber)
{
	ushort nPlayerID = m_squadFirstTeamPlayers.GetSquad(_SquadNumber);
	CPlayer& thePlayer = *WorldData().GetPlayer(nPlayerID);
	if (thePlayer.GetWeeksOnLoan() == MAXLOANPLAYERWEEKS && GetDivision().WeeksToEndOfSeason() > 0)
	{
		return false;
	}
	else if (thePlayer.GetWeeksOnLoan() > 0)
	{
		return false;
	}
	bool DeletedOne = false;

	CClub* pClub = NULL;
	if (thePlayer.GetClubIDOnLoanFrom() != NOCLUB)
	{
		pClub = &WorldData().GetClub(thePlayer.GetClubIDOnLoanFrom());
	}
	CExtString Str;

	// Loan period finished
	if (IsUserControlled() != true)
	{
		// No longer a user controlled club so don't extend player's loan
		DeletedOne = true;
	}
	else
	{
		if (pClub == NULL)
		{
			Str.Format(IDS_PLAYERLEAVINGAFTERLOANNOCLUB, thePlayer.GetName());
			UserMessageBox(Str);
			DeletedOne = true;
		}
		else if (pClub->DoAgreeToExtendPlayerLoan(thePlayer, GetName()) == false)
		{
			Str.Format(IDS_PLAYERLEAVINGAFTERLOAN2, thePlayer.GetName(), pClub->GetName());
			UserMessageBox(Str);
			DeletedOne = true;
		}
		else
		{
			// Offer to extend loan by 4 weeks
			Str.Format(IDS_PLAYERLEAVINGAFTERLOAN3, thePlayer.GetName(), pClub->GetName());
			if (UserMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDNO)
			{
				Str.Format(IDS_PLAYERLEAVINGAFTERLOAN, thePlayer.GetName(), pClub->GetName());
				UserMessageBox(Str);
				DeletedOne = true;
			}
			else
			{
				// Extend the loan period
				thePlayer.SetWeeksOnLoan(4);
			}
		}
	}

	if (DeletedOne == true)
	{
		// Loan period finished
		if (pClub != NULL)
		{
			Str.Format(IDS_PLAYERLEAVINGAFTERLOAN, thePlayer.GetName(), pClub->GetName());
		}
		else
		{
			Str.Format(IDS_PLAYERLEAVINGAFTERLOANNOCLUBNEWS, thePlayer.GetName());
		}
		AddToClubNews(Str);
		thePlayer.DoLoanComplete();
		m_squadFirstTeamPlayers.DoRemoveFromSquad(_SquadNumber);	// No longer at this club
		m_squadFirstTeamPlayers.DoDecrementPlayersInOnLoan();
		if (pClub == NULL)
		{
			thePlayer.SetContractRenewalStatus(RETIRED);
			thePlayer.DoRegenerate(nPlayerID, GetCountry());
		}
		else
		{
			m_RefusedDeals.DoAddTransfer(nPlayerID);			// Won't be able to negotiate with him for a while
			pClub->DoPlayerBackFromLoan(nPlayerID);
		}
	}
	return DeletedOne;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetPersonalTrainingHours
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::GetPersonalTrainingHours()
{
	return TOTALTRAININGHOURS - m_SquadTrainingDetails.GetTeamTrainingHours();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoDailyTasks
	Access:    	public
	Parameter: 	bool WeekEnd
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoDailyTasks(bool _IsWeekEnd)
{
	uint TimeTillNextMatch = GetDivision().GetHowLongTillNextLeagueMatch();
	m_squadFirstTeamPlayers.DoDailyTasks(this, _IsWeekEnd, (GetCountryRanking() == 0), TimeTillNextMatch);
	m_FinancialLoan.SetAlreadyBorrowedToday(false);
	if (m_ManagerID == NOMANAGER)
	{
		if (RandomNumber.IntLessThan(CHANCETRYHIREMANAGER) == 1)
		{
			DoTryHireManager();
        }
	}
    else
	{
		DoTrySackManager();
    }

    if (IsUserControlled() == true)
	{
		int SaveCurrentManager = WorldData().GetCurrentUserID();
		MakeCurrentUser();
		GetUser().DailyClubTasks();

		if (GetDivision().GetWeeksToEndOfSeason() < NEARENDOFSEASON)
		{
			m_squadFirstTeamPlayers.DoCheckIfRequestNewContracts(*this);
			m_StaffList.DoRequestNewContracts(*this);
			m_YouthSquad.DoRequestNewContracts(*this);
		}

		if (RandomNumber.IntLessThan(CHANCEWARNDISCIPLINARY) == 1 && TimeTillNextMatch < 4 && GetTeamTactic().GetAggression() > 4)
		{
			DoCheckDisciplinaryPosition();
		}
		DoTrySellHumanManagedPlayers();		// User managed clubs will receive offers for listed players
		DoCheckIfSquadLacking();			// Warnings from board if squad size small

		// Warnings from board if no coach or physio
		if (GetCoachStaffNumber() == wNOTFOUND && RandomNumber.IntLessThan(CHANCEWARNSMALLSQUAD) == 1 && WorldData().IsFastTest() == false)
		{
			AddToClubNews(IDS_WARNNOCOACH);
		}

		if (GetPhysioStaffNumber() == wNOTFOUND && RandomNumber.IntLessThan(CHANCEWARNSMALLSQUAD) == 1 && WorldData().IsFastTest() == false)
		{
			AddToClubNews(IDS_WARNNOPHYSIO);
		}
		WorldData().SetCurrentUserID(SaveCurrentManager);

		// Employee background activities
#ifdef USESCOUTS
		if (m_Scout1.Active() == true)
		{
			m_Scout1.DoEndOfWeek(this);
		}
		if (m_Scout2.Active() == true)
		{
			m_Scout2.DoEndOfWeek(this);
		}
#endif

		// Check on the financial situation and make the board take action if necessary
		if (WorldData().IsFastTest() == false)
		{
			if (m_CashBalance < GetPopularity().GetMinimumClubBalance(GetCountry().GetFinancialRatioPercentage()))
			{
				DEBUG_OUTPUT(("Minimum Cash Balance - %d"), (m_CashBalance, GetPopularity().GetMinimumClubBalance(GetCountry().GetFinancialRatioPercentage())));
				AddToClubNews(IDS_WARNNOMONEY);
				if (RandomNumber.IntLessThan(CHANCEBOARDSELLPLAYER) == 1 && m_squadFirstTeamPlayers.DoCountAvailableNonLoanedPlayers() > MINSQUADSIZE)
				{
					DoSellARandomPlayer();
				}
			}
		}
		// Random event strikes a player?
		if (RandomNumber.IntLessThan(100) < 3 && WorldData().IsFastTest() == false)
		{
			m_squadFirstTeamPlayers.DoFindRandomPlayer()->DoTryCreateRandomEventPlayer(this);
		}
	}
	else
	{
		if (IsRanking0Club() && WorldData().GetMasterState() == CWorld::IN_INITIALISATION)
		{
			// Non human managed club bid for players etc.
			if (m_ManagerID == NOMANAGER)
			{
				DoTryHireCoach();
				if (RandomNumber.IntLessThan(CHANCETRYBID) == 1 || (m_CashBalance > static_cast<int>(GetPopularity().GetPlayerPurchaseThreshold(GetCountry().GetFinancialRatioPercentage()) * 2) && RandomNumber.IntLessThan(100) < 25))
				{
					DoTryBuyPlayers();
				}
			}
			m_squadFirstTeamPlayers.OnOffListPlayers();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryHireManager
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Try and appoint a new manager
------------------------------------------------------------------------------*/
void CClub::DoTryHireManager()
{
	int Count = 0;
	DoCalculateManagerContractOffer();

	// Offer job to user managers
#ifdef OFFERUSERMANAGERJOB
	if (WorldData().IsFastTest() == false)
	{
		byte PopularityDifferenceChance[] =
		{
			50, 45, 40, 35, 30, 25, 15, 5
		};
		byte PopularityDifference;
		CUser* pUser;
		for (Count = 0; Count < WorldData().CountActiveUsers(); Count++)
		{
			pUser = &WorldData().getUser(Count);
			if (pUser->IsActive() == true)
			{
				if (pUser->GetManager().GetLastClub() != this &&
					&pUser->GetClub() != this &&
					pUser->GetManager().GetGamesPlayedClub() > MINWEEKSATCLUB)
				{
					PopularityDifference = min(7, abs(m_PopularityID - pUser->GetPopularityID()));
					PopularityDifference += 10 * (pUser->IsManagingAClub() == false);
					PopularityDifference += 10 * (pUser->GetCountryID() == GetCountryID());
					if (WorldData().IsFastTest() == false && irnd(100) < PopularityDifferenceChance[PopularityDifference] && pUser->IsGoodEnoughToManageClub(Count) == true)
					{
						// Offer job
						str.Format(IDS_INTERESTEDINMANAGER, pUser->GetManager().GetName(), GetName());
						if (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
						{
							// Call manager Contract Neg.- MODAL
							WorldData().SetCurrentUserID(Count);
							WorldData().RestoreCurrentUser();
						}
					}
				}
			}
		}
	}
#endif
	while (Count++ < 10 && m_ManagerID == NOMANAGER)
	{
		switch(RandomNumber.IntLessThan(10))
		{
			case 3:
				DoTryPlayerForManager();
				break;
			case 4:
			case 5:
				DoTryCoachForManager();
				break;
			default:
				// Try Hire UnemployedManager
				{
					int GotJob = NOTFOUND;
					CManagerList UnemployedList;
					WorldData().GetManagerList().DoBuildUnemployedList(UnemployedList, GetCountryID());
					if (UnemployedList.GetSize() > 1)
					{
						GotJob = RandomNumber.IntLessThan(UnemployedList.GetUpperBound());
					}
					if (UnemployedList.GetSize() == 1)
					{
						GotJob = 0;
					}
					if (GotJob > NOTFOUND && RandomNumber.IntLessThan(ACCEPTMANAGERJOB) == 1)
					{
						m_ManagerID = WorldData().GetManagerList().DoFindID(UnemployedList.GetAt(GotJob));
						if (GetManager().GetLastClub() != this && GetManager().GetCurrentClub() != this)
						{
							GetManager().DoNewClub(*this);
						}
						else
						{
							// Managed this club recently
							m_ManagerID = NOMANAGER;
						}
					}
				}
				break;
		}
	};

	if (m_ManagerID != NOMANAGER)
	{
		CExtString str;
		str.Format(IDS_APPOINTEDMANAGER, GetManagerName(), GetName());
        AddToNewsList(str);
		//@@getDivision().NewsPaperEventPtr()->SetManagerAppointedNumber(m_ManagerID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoTryHireCoach
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Non user controlled club tries to hire an available coach from
				own country
------------------------------------------------------------------------------*/
void CClub::DoTryHireCoach()
{
	CEmployee* pAvailableEmployee = null;

	if (GetCoachStaffNumber() == wNOTFOUND)
	{
		// Need a coach
		pAvailableEmployee = GetActiveCountry().GetRandomAvailableEmployee(CEmployeeBase::COACH, GetPopularity().GetMinimumEmployeeSkill(), GetPopularity().GetMaximumEmployeeSkill());
		if (pAvailableEmployee != null)
		{
			pAvailableEmployee->GetContractEndDate().IncreaseLength(1 + RandomNumber.IntLessThan(5)); 	// Give him a random contract
			DoEmployStaff(pAvailableEmployee);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoSellARandomPlayer
	Access:    	public
	Parameter: 	bool TeamPlayer
	Returns:   	void
	Qualifier:
	Purpose:	If a human managed club is seriously in debt then the board will
				attempt to sell a player
------------------------------------------------------------------------------*/
void CClub::DoSellARandomPlayer(bool TeamPlayer)
{
    CPlayer* pPlayer;
	if (TeamPlayer == true )
	{
		// Sell attacker or midfielder in team
		pPlayer = m_squadFirstTeamPlayers.DoFindRandomAttackingPlayer();
	}
	else if (RandomNumber.IntLessThan(CHANCESELLTEAMPLAYER) == 1)
	{
		// First team player
		pPlayer = m_squadFirstTeamPlayers.DoFindRandomPlayer();
	}
	else
	{
		// any player from squad
		pPlayer = m_squadFirstTeamPlayers.DoFindRandomSquadPlayer();
	}
	EXT_ASSERT_POINTER(pPlayer, CPlayer);
	CClubTransferOffer TheOffer(pPlayer, pPlayer->GetValuation(GetCountry()));
	if (TheOffer.GetAmountOffered() > 0 && pPlayer->WillingToTransfer() == true)
	{
		TheOffer.SetBidClub(pPlayer->GetBidCountry(GetCountry(), WorldData().GetCountryList()).GetActiveCountry().DoFindRandomDivision().DoFindRandomNonUserClub(this));
		if (TheOffer.GetBidClubID() != NOCLUB)
		{
			if (TheOffer.GetBidClub().InterestedInPlayer(&TheOffer.GetPlayer(), TheOffer.GetAmountOffered()) == true && \
				DoTryAcceptBidFromNonUser(TheOffer) == true)
			{
				CContract ContractRequired;
				DoSoldPlayer(*pPlayer, TheOffer, ContractRequired, false);	// Sell the player
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCheckIfSquadLacking
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	If the squad is weak in an area or overall then advise
				the manager
------------------------------------------------------------------------------*/
void CClub::DoCheckIfSquadLacking()
{
	if (WorldData().IsFastTest() == true || m_squadFirstTeamPlayers.DoCountAvailablePlayers() == PLAYERSINSQUAD)
	{
		return;
	}
	CExtString str, str1;
    CPlayer* pPlayer;
	if (m_squadFirstTeamPlayers.DoCountAvailablePlayersPosn(GOALKEEPER) < 2 &&
		RandomNumber.IntLessThan(CHANCEWARNSMALLSQUAD) == 1 && WorldData().IsFastTest() == false)
	{
        str1.LoadString(IDS_WARNNOGKCOVER);
		AddToClubNews(str);
	}

	// Check all positions
	for (uint LoopCount = DEFENDER; LoopCount < MAXPLAYERPOSITIONS; LoopCount++)
	{
		if (m_squadFirstTeamPlayers.DoCountAvailablePlayersPosn(static_cast<ePlayerPosition>(LoopCount)) < m_squadFirstTeamPlayers.MinimumAvailablePlayersInPosition(static_cast<ePlayerPosition>(LoopCount)) && \
			RandomNumber.IntLessThan(CHANCEWARNSMALLSQUAD) == 1 && WorldData().IsFastTest() == false)
		{
            str1.LoadString(PlayerPositionText[LoopCount]);
			if (LoopCount != ATTACKER)
			{
				str.Format(IDS_WARNSQUADSMALLINPOSN, str1);
			}
			else
			{
				str.Format(IDS_WARNSQUADSMALLINPOSNAN, str1);
			}
			AddToClubNews(str);
			break;
		}
	}

	// Check overall squad size
	if (m_squadFirstTeamPlayers.DoCountAvailablePlayers() < (MINSQUADSIZE + 2) &&
		RandomNumber.IntLessThan(CHANCEWARNSMALLSQUAD) == 1 && WorldData().IsFastTest() == false)
	{
        str.Format(IDS_WARNSQUADSMALL, GetName());
		AddToClubNews(str);
	}

	// Check if there are any unfit players in the team
    for (uint num = 0; num < PLAYERSINTEAM; num ++)
	{
        if (m_squadFirstTeamPlayers.GetSquad(num) != NOPLAYER)
		{
            pPlayer = &GetPlayer(num);
            if (pPlayer->GetSkills().GetEnergy() < WARNENERGYLOW && RandomNumber.IntLessThan(CHANCEWARNPLAYERTIRED) == 1)
			{
                str.Format(IDS_WARNPLAYERFITESSLOW, pPlayer->GetName());
				AddToClubNews(str);
			}
		}
  	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCheckDisciplinaryPosition
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	How well is the team behaving on the field ?
				Issue a message if poor
------------------------------------------------------------------------------*/
void CClub::DoCheckDisciplinaryPosition()
{
	if (WorldData().IsFastTest() == true)
	{
		return;
	}
    // Not near start or end of season
	if (m_TableData.GetNumberGamesPlayed() < 6 || \
		(GetDivision().GetNumberGamesPerSeason() -  m_TableData.GetNumberGamesPlayed()) < 3)
	{
		return;
	}
	int StrNum = 0;
	CIndexedIntArray TheList;
	int Count = 0, LoopCount;
	for (LoopCount = 0; LoopCount < GetDivision().GetNumberOfClubsInDivision(); LoopCount++)
	{
		TheList.AddNew(Count++, GetDivision().GetClub(LoopCount).GetSeasonStats().GetSeasonDisciplinaryPoints());
	}
	TheList.SortHigh();

    CClub* pClub;
	// DoFindID the clubs position
	for (LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
        pClub = &WorldData().GetClub(TheList.GetIndex(LoopCount));
		if (pClub == this && LoopCount == 0)
		{
			StrNum = IDS_TOPDISC;
			break;
		}
		if (pClub == this && LoopCount < 5)
		{
			StrNum = IDS_TOPDISC5;
			break;
		}
		if (pClub == this && LoopCount < 10)
		{
			StrNum = IDS_TOPDISC10;
			break;
		}
	}
	// Got a bad record, chaiman expresses his concern.
	if (StrNum != 0 && WorldData().IsFastTest() == false)
	{
		CExtString str;
		str.Format(StrNum, GetManagerName());
		AddToClubNews(str);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::DoLowerMorale
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoLowerMorale(const byte _Value)
{
	m_TeamMorale -= _Value;
	m_TeamMorale = min(max(GetMorale(), MINIMUMMORALE), MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoRaiseMorale
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoRaiseMorale(const byte _Value)
{
	m_TeamMorale += _Value;
	m_TeamMorale = min(max(GetMorale(), MINIMUMMORALE), MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::CoachSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::CoachSkill()
{
	ushort num = GetCoachStaffNumber();
	if (num != wNOTFOUND)
	{
		return static_cast<CEmployee*>(m_StaffList.GetAt(num))->GetSkill();
	}
    return 0;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::PhysioSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::PhysioSkill()
{
	ushort num = GetPhysioStaffNumber();
	if (num != wNOTFOUND)
	{
        return static_cast<CEmployee*>(m_StaffList.GetAt(num))->GetSkill();
	}
    return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TransferListRandomPlayers
	Access:    	public
	Parameter: 	const uint HowMany
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TransferListRandomPlayers(const uint _HowMany)
{
    EXT_ASSERT(_HowMany < 8);
    if (IsUserControlled() == false && m_squadFirstTeamPlayers.DoCountAvailablePlayers() > MINSQUADSIZE && m_squadFirstTeamPlayers.DoCountListedPlayers() < MAXCLUBLISTEDPLAYERS)
	{
		CPlayer* pPlayer;
		for (uint LoopCount = 0; LoopCount < _HowMany; LoopCount++)
		{
			if (RandomNumber.IntLessThan(4) == 1)
			{
				pPlayer = m_squadFirstTeamPlayers.DoFindRandomPlayer();
			}
			else
			{
				pPlayer = m_squadFirstTeamPlayers.DoFindRandomSquadPlayer();
			}
            if ((pPlayer->GetTransferStatus().IsTransferListed() == false) && pPlayer->GetAge() < 34 && pPlayer->WillingToTransfer() == true)
			{
				pPlayer->DoClubTransferListPlayer(GetCountry());
            }
        }
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::Write
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->Write(&m_ID, sizeof m_ID);
	_pDataFile->Write(&m_NameID, sizeof CStringID);
    _pDataFile->Write(&m_StadiumID, sizeof m_StadiumID);
    m_YouthSquad.Write(_pDataFile);
    m_StaffList.Write(_pDataFile);
    m_ClubHistory.Write(_pDataFile);
    m_TableData.Write(_pDataFile);
    _pDataFile->Write(&m_CurrentTacticsID, sizeof m_CurrentTacticsID);
    _pDataFile->Write(&m_ManagerID, sizeof m_ManagerID);
    _pDataFile->Write(&m_RegionID, sizeof m_RegionID);
    _pDataFile->Write(&m_PopularityID, sizeof m_PopularityID);
	_pDataFile->Write(&m_TeamMorale, sizeof m_TeamMorale);
	m_TeamTactic.Write(_pDataFile);
    _pDataFile->Write(&m_WeekEndMatch, sizeof m_WeekEndMatch);
    _pDataFile->Write(&m_MidWeekMatch, sizeof m_MidWeekMatch);
	_pDataFile->Write(&m_SeasonStats, sizeof CClubSeasonStats);
	_pDataFile->Write(&m_FinancialLoan, sizeof  m_FinancialLoan);
    _pDataFile->Write(&m_TicketPrice, sizeof m_TicketPrice);
    _pDataFile->Write(&m_CashBalance, sizeof m_CashBalance);
    _pDataFile->Write(&m_DivisionID, sizeof m_DivisionID);
	m_SquadTrainingDetails.Write(_pDataFile);
	_pDataFile->Write(&m_squadFirstTeamPlayers, sizeof CPlayerSquad);
	_pDataFile->Write(&m_Friendlies, sizeof CClubFriendlies);
    if (IsRanking0Club())
	{
		m_pAccounts->Write(_pDataFile);
		_pDataFile->Write(m_pCurrentForm, sizeof CCurrentForm);
		m_RefusedDeals.Write(_pDataFile);
	}
#ifdef USELONGCLUBNAME
	m_LongClubName.Write(_pDataFile);
#endif
#ifdef USESCOUTS
	Scout1.Write(_pDataFile);
	Scout2.Write(_pDataFile);
#endif
#ifdef PLAYERTRANSFERHISTORY
	m_PlayersBought.Write(_pDataFile);
	m_PlayersSold.Write(_pDataFile);
#endif
#ifdef TROPHYHISTORY
	m_TrophyHistory.Write(_pDataFile);
#endif
#ifdef	MATCHTRACKING
  m_GameAttendance.Write( _pDataFile );
  m_GateReceipts.Write( _pDataFile );
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CClub::Read
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->Read(&m_ID, sizeof m_ID);
	_pDataFile->Read(&m_NameID, sizeof CStringID);
    _pDataFile->Read(&m_StadiumID, sizeof m_StadiumID);
	m_YouthSquad.Read(_pDataFile);
    m_StaffList.Read(_pDataFile);
    m_ClubHistory.Read(_pDataFile);
    m_TableData.Read(_pDataFile);
    _pDataFile->Read(&m_CurrentTacticsID, sizeof m_CurrentTacticsID);
    _pDataFile->Read(&m_ManagerID, sizeof m_ManagerID);
    _pDataFile->Read(&m_RegionID, sizeof m_RegionID);
    _pDataFile->Read(&m_PopularityID, sizeof m_PopularityID);
	_pDataFile->Read(&m_TeamMorale, sizeof m_TeamMorale);
	m_TeamTactic.Read(_pDataFile);
    _pDataFile->Read(&m_WeekEndMatch, sizeof m_WeekEndMatch);
    _pDataFile->Read(&m_MidWeekMatch, sizeof m_MidWeekMatch);
	_pDataFile->Read(&m_SeasonStats, sizeof CClubSeasonStats);
	_pDataFile->Read(&m_FinancialLoan, sizeof m_FinancialLoan);
    _pDataFile->Read(&m_TicketPrice, sizeof m_TicketPrice);
    _pDataFile->Read(&m_CashBalance, sizeof m_CashBalance);
    _pDataFile->Read(&m_DivisionID, sizeof m_DivisionID);
	m_SquadTrainingDetails.Read(_pDataFile);
	_pDataFile->Read(&m_squadFirstTeamPlayers, sizeof CPlayerSquad);
	_pDataFile->Read(&m_Friendlies, sizeof CClubFriendlies);
	m_BidsThisWeek = 0;
	if (IsRanking0Club())
	{
		EXT_ASSERT(m_pAccounts != null);
		m_pAccounts->Read(_pDataFile);
		EXT_ASSERT(m_pCurrentForm != null);
		_pDataFile->Read(m_pCurrentForm, sizeof CCurrentForm);
		m_RefusedDeals.Read(_pDataFile);
	}
#ifdef USELONGCLUBNAME
	m_LongClubName.Read(_pDataFile);
#endif
#ifdef USESCOUTS
	Scout1.Read(_pDataFile);
	Scout2.Read(_pDataFile);
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
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetManagerID
	Access:    	public
	Parameter: 	const ushort x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetManagerID(const ushort _NewID)
{
	m_ManagerID = _NewID;
    if (IsUserControlled() == true)
	{
		GetActiveCountry().DoStaffAndYouthEndOfWeek(GetCountry(), &GetPopularity(), GetDivisionStrengthID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::WeeklyClearDown
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoWeeklyClearDown()
{
    m_WeekEndMatch.SetVenue(NOMATCH);
    m_MidWeekMatch.SetVenue(NOMATCH);
    if (IsRanking0Club())
	{
		m_pAccounts->DoWeeklyClearDown();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::IsNonLeagueClub
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClub::IsNonLeagueClub()
{
	if (m_DivisionID == NODIVISION)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetTicketPrice
	Access:    	public
	Parameter: 	const uint x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetTicketPrice(const uint x)
{
	m_TicketPrice = x;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoAfterMatchUpdate
	Access:    	public
	Parameter: 	const byte _PrimaryTactics
	Parameter: 	CFixture & _Fixture
	Parameter: 	eWhichTeam _Venue
	Parameter: 	const byte _AwayGatePercentage
	Parameter: 	const byte _FAGatePercentage
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::DoAfterMatchUpdate(const ushort _PrimaryTactics, CFixture & _Fixture, eWhichTeam _Venue, const byte _AwayGatePercentage, const byte _FAGatePercentage)
{
	if (IsNonLeagueClub() == false)
	{
		byte ourgoals, theirgoals;
		if (WorldData().GetCurrentDate().GetMatchDay() & 1)
		{
			// Mid week match
			m_MidWeekMatch.DoUpdate(_Fixture, _Venue, _AwayGatePercentage, _FAGatePercentage);
			ourgoals = m_MidWeekMatch.GetScored();
			theirgoals = m_MidWeekMatch.GetConceded();
		}
		else
		{
			// Weekend match
			m_WeekEndMatch.DoUpdate(_Fixture, _Venue, _AwayGatePercentage, _FAGatePercentage);
			ourgoals = m_WeekEndMatch.GetScored();
			theirgoals = m_WeekEndMatch.GetConceded();
        }
		// raise morale if last match won or drawn, lower if lost,
		m_TeamMorale += (ourgoals > theirgoals) * 30 - (ourgoals < theirgoals) * 30 + (ourgoals == theirgoals) * 10 \
			+ (ourgoals > (theirgoals + 3)) * 5 - ((ourgoals + 3) < theirgoals) * 5;
		m_TeamMorale = min(max(GetMorale(), MINIMUMMORALE), MAXIMUMMORALE);

		if (IsUserControlled() == true)
		{
			GetUser().GetTacticsList().GetAt(_PrimaryTactics).DoPlayedMatch(ourgoals, theirgoals);
#ifdef CUSTOMFORMATIONS
			CTactic* pPrimaryTactics = GetTactics(_PrimaryTactics);
			if (pPrimaryTactics->IsBizarre() == true)
			{
				if (pPrimaryTactics->GetBizarreWarningIssued() == true)
				{
					// Sack manager
					UserMessageBox(IDS_SACKBIZARRETACTIC);
					DoSackManager(false);
				}
				else
				{
					// Warn manager
					UserMessageBox(IDS_WARNBIZARRETACTIC);
					pPrimaryTactics->SetBizarreWarningIssued();
				}
			}
#endif
		}
		m_squadFirstTeamPlayers.DoAfterMatchUpdate(_PrimaryTactics, ourgoals, theirgoals, _Fixture, _Venue, *this);
    }
}


#ifdef	MATCHTRACKING


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCountHomeLeagueGamesPlayedThisSeason
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CClub::DoCountHomeLeagueGamesPlayedThisSeason()
{
	return m_TableData.GetHomeGames()->GetNumberGamesPlayed();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::DoCalculateAverageAttendance
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CClub::DoCalculateAverageAttendance()
{
	int TotalAttendance = m_GameAttendance.TotalSum();
	return TotalAttendance / DoCountHomeLeagueGamesPlayedThisSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::InitialiseMatchTracking
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::InitialiseMatchTracking()
{
	m_GameAttendance.SetSize(23, 0);
	m_GateReceipts.SetSize(23, 0);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetGateReceipts
	Access:    	public
	Parameter: 	const ushort aNum
	Parameter: 	const uint aAmount
	Returns:   	void
	Qualifier:
	Purpose:	Set's the home attendance of the club after home match[Num]
------------------------------------------------------------------------------*/
void CClub::SetGateReceipts(const ushort aNum, const uint aAmount)
{
    if (IsRanking0Club())
	{
		m_GateReceipts.SetAt(aNum, aAmount);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TurnMatchTrackingOff
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TurnMatchTrackingOff()
{
	m_GameAttendance.RemoveAll();
	m_GateReceipts.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetGateReceipts
	Access:    	public
	Parameter: 	const int num
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetGateReceipts(const int num)
{
    if (IsRanking0Club())
	{
		EXT_ASSERT(num > -1 && num < m_GateReceipts.GetSize());
		return m_GateReceipts.GetAt(num);
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetGameAttendance
	Access:    	public
	Parameter: 	const int num
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CClub::GetGameAttendance(const int num)
{
	EXT_ASSERT(num > -1 && num < m_GameAttendance.GetSize());
    if (IsRanking0Club())
	{
		return m_GameAttendance.GetAt(num);
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::MaxAttendance
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:	Return the maximum attendance according to popularity or highest
				league`home match attendance
------------------------------------------------------------------------------*/
uint CClub::MaxAttendance()
{
    return max(GetPopularity().GetMaximumAttendance(), static_cast<int>(m_GameAttendance.HighestValue()));
}


/*------------------------------------------------------------------------------
	Method:   	CClub::SetGameAttendance
	Access:    	public
	Parameter: 	int Num
	Parameter: 	int x
	Returns:   	void
	Qualifier:
	Purpose:	Set's the home attendance of the club after home match[Num]
------------------------------------------------------------------------------*/
void CClub::SetGameAttendance(int Num, int x)
{
    if (IsRanking0Club())
	{
		EXT_ASSERT(Num > -1 && Num < m_GameAttendance.GetSize());
		EXT_ASSERT(x > -1);
		m_GameAttendance.SetAt(Num, x);
	}
}
#endif


#ifdef ALLOWSTADIUMBUILDING
/*------------------------------------------------------------------------------
	Method:   	CClub::SetBuildingCosts
	Access:    	public
	Parameter: 	const uint Cost
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::SetBuildingCosts(const uint Cost)
{
	uint ActCost = GetCountry().GetCurrency().ConvToPounds(Cost);
   	m_pAccounts->Plus(CAccountsItemList::BUILDINGWORK, ActCost);
	m_CashBalance -= ActCost;
}
#endif


#ifdef ACTIVEFEDERATIONS
/*------------------------------------------------------------------------------
	Method:   	CClub::GetFederation
	Access:    	public
	Parameter: 	void
	Returns:   	CFederation&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CClub::GetFederation()
{
	return WorldData().GetFederation(GetCountry().GetFederationID());
}
#endif


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
	Method:   	CClub::QualifiedForFederationCup
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::QualifiedForFederationCup()
{
	if (m_ManagerID != NOMANAGER)
	{
        GetManager().QualifiedForFederationCup();
    }
	if (GetCountryRanking() == 2)
	{
		m_squadFirstTeamPlayers.DoCreateSquad(this);
	}
}
#endif


#ifdef ACTIVEFRIENDLIES
/*------------------------------------------------------------------------------
	Method:   	CClub::MatchNumberToFriendlyNumber
	Access:    	public
	Parameter: 	const ushort MatchNumber
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CClub::MatchNumberToFriendlyNumber(const ushort MatchNumber)
{
    return GetDivision().WhichFriendlyNumberToday(MatchNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::FriendlyDateStr
	Access:    	public
	Parameter: 	const ushort FriendlyNum
	Returns:   	CString
	Qualifier:
	Purpose:	Return the date of the friendly match as a text string
------------------------------------------------------------------------------*/
CString CClub::FriendlyDateStr(const ushort FriendlyNum)
{
	return GetDivision().FriendlyDateStr(FriendlyNum);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::GetFriendlies
	Access:    	public
	Parameter: 	void
	Returns:   	CClubFriendlies&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClubFriendlies& CClub::GetFriendlies()
{
	return m_Friendlies;
}
#endif


#ifdef	ACTIVESTADIUMPLANS
/*------------------------------------------------------------------------------
	Method:   	CClub::StandsUnderRefurbishment
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int	CClub::StandsUnderRefurbishment()
{
	return WorldData().GetStadium(m_StadiumID)->StandsUnderRefurbishment();
}


/*------------------------------------------------------------------------------
	Method:   	CClub::RaiseCapacity
	Access:    	public
	Parameter: 	int NewCapacity
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::RaiseCapacity(int NewCapacity)
{
	EXT_ASSERT(NewCapacity > -1);
	GetStadium()->RaiseCapacity(NewCapacity);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::LowerCapacity
	Access:    	public
	Parameter: 	int NewCapacity
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::LowerCapacity(int NewCapacity)
{
	EXT_ASSERT(NewCapacity > -1);
	GetStadium()->LowerCapacity(NewCapacity);
}
#endif


 #ifdef USESCOUTS
*------------------------------------------------------------------------------
	Method:   	CClub::ScoutSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::ScoutSkill()
{
    byte Skill = 0;
    CEmployee* pEmployee;
    for (uint num = 0; num < m_StaffList.GetSize(); num ++)
	{
        pEmployee = static_cast<CEmployee*>(m_StaffList.GetAt(num));
		EXT_ASSERT(pEmployee != null);
        if (pEmployee->GetType() == CEmployeeBase::SCOUT)
		{
        	Skill += pEmployee->GetSkill();
		}
	}
    return Skill;
 }


/*------------------------------------------------------------------------------
	Method:   	CClub::NumScouts
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CClub::NumScouts()
{
    byte Skill = 0;
    for (uint num = 0; num < m_StaffList.GetSize(); num ++)
	{
        if (static_cast<CEmployee*>(m_StaffList.GetAt(num))->GetType() == CEmployeeBase::SCOUT)
		{
        	Skill ++;
		}
	}
    return Skill;
 }
#endif


#ifdef USECLUBYEARFOUNDED
/*------------------------------------------------------------------------------
	Method:   	CClub::GetYearFoundedStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClub::GetYearFoundedStr()
{
	CString str;
	if (m_YearFounded == 0)
	{
		str.LoadString(IDS_STAD_NOYEAR);
	}
	else
	{
		str.Format(TEXT("%d"), m_YearFounded);
	}
	return str;
}
#endif


#ifdef TROPHYHISTORY
/*------------------------------------------------------------------------------
	Method:   	CClub::TrophyWinners
	Access:    	public
	Parameter: 	CString title
	Parameter: 	int Id
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TrophyWinners(const CString &title, int Id, bool IsCup)
{
	m_TrophyHistory.AddChampions(title, GetName(), GetActiveCountry().GetSeasonStr(), Id, IsCup);
    if (IsUserControlled() == true)
	{
        GetUser().TrophyWinners(title, Id, GetActiveCountry().GetSeasonStr(), IsCup);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClub::TrophyRunnersUp
	Access:    	public
	Parameter: 	CString title
	Parameter: 	int Id
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClub::TrophyRunnersUp(const CString &title, int Id, bool IsCup)
{
	m_TrophyHistory.AddRunnersUp(title, GetName(), GetActiveCountry().GetSeasonStr(), Id, IsCup);
    if (IsUserControlled() == true)
	{
        GetUser().TrophyRunnersUp(title, Id, GetActiveCountry().GetSeasonStr(), IsCup);
	}
}
#endif
