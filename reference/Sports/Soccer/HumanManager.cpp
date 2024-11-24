/*

	File:- HumanManager.cpp

	Human Manager class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "MessageDisplay.h"

#include "SoccerSquadTraining.h"
#include "TransferNegotiation.h"
#include "PlayerSearch.h"
#include "MatchInfo.h"

#include "FootballDirector.h"


const uint				MAXIMUMCLUBNEWSITEMS = 20;


/*------------------------------------------------------------------------------
	Method:   	CUser::CUser
	Access:    	public 
	Parameter: 	ushort _nClub
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUser::CUser(ushort _nClub)
	: m_WeeksPlayed(0)
#ifdef ACTIVESTADIUMPLANS
	, m_BoxPrice = 0;
#endif
{
    m_ManagerID = _nClub;
	CreateClasses();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CUser
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUser::CUser()
{
	CreateClasses();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::~CUser
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUser::~CUser()
{
    delete m_pTransferNegotiationList;
	delete m_pTacticsList;
    delete m_pPlayerSearch;
    delete m_pNewsList;
#ifdef YOUTHTRAINING
    delete m_pYouthSquadTrainingDetails;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetCurrentAggression
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetCurrentAggression() 
{
	return GetTacticsList().GetCurrentTeamTactic().GetAggression();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetCurrentMarkingStyle
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetCurrentMarkingStyle()
{
	return GetTacticsList().GetCurrentTeamTactic().GetMarkingStyle();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetCurrentStyleOfPlay
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetCurrentStyleOfPlay()
{
	return GetTacticsList().GetCurrentTeamTactic().GetStyleOfPlay();
}

/*------------------------------------------------------------------------------
	Method:   	CUser::GetOffsideTrap
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::GetOffsideTrap()
{
	return GetTacticsList().GetCurrentTeamTactic().GetOffsideTrap();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetCounterAttack
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::GetCounterAttack()
{
	return GetTacticsList().GetCurrentTeamTactic().GetCounterAttack();
}


void CUser::SetCurrentAggression(byte _Value)
{
	return GetTacticsList().GetCurrentTeamTactic().SetAggression(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetCurrentMarkingStyle
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetCurrentMarkingStyle(byte _Value)
{
	return GetTacticsList().GetCurrentTeamTactic().SetMarkingStyle(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetCurrentStyleOfPlay
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetCurrentStyleOfPlay(byte _Value)
{
	return GetTacticsList().GetCurrentTeamTactic().SetStyleOfPlay(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetOffsideTrap
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetOffsideTrap(bool _Value)
{
	GetTacticsList().GetCurrentTeamTactic().SetOffsideTrap(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetCounterAttack
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetCounterAttack(bool _Value)
{
	GetTacticsList().GetCurrentTeamTactic().SetCounterAttack(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::MonthlyTasks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoMonthlyTasks()
{
//	m_Chairman.DoMonthlyReport(getClub().getDivision(), GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetManager
	Access:    	public 
	Parameter: 	void
	Returns:   	CManager&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager& CUser::GetManager()
{
	return WorldData().GetManager(m_ManagerID);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::AddTransferNegotiation
	Access:    	public 
	Parameter: 	CClubTransferOffer& _rTheBid
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CUser::AddTransferNegotiation(CClubTransferOffer& _rTheBid)
{
	int DatesToNegotiation = 1 + RandomNumber.IntLessThan(2);
	CCalendar CurrentDate;
	CurrentDate = WorldData().GetCurrentDate();

	for (int LoopCount = 0; LoopCount < DatesToNegotiation; LoopCount++)
	{
		WorldData().GetCurrentDate().NextMatchDay();
	}
	while (_rTheBid.GetBidClub().AreBuyTransfersAllowed() == false)
	{
		WorldData().GetCurrentDate().PreviousMatchDay();
	}
	WorldData().GetCurrentDate() = CurrentDate;

	m_pTransferNegotiationList->AddOne(_rTheBid, DatesToNegotiation);
	CString str;
	str.Format(IDS_PERSONALTERMS, _rTheBid.GetPlayer().GetName(), WorldData().GetForwardDateShortStr(DatesToNegotiation));
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::RemoveTransferNegotiation
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::RemoveTransferNegotiation(const ushort _PlayerID)
{
	m_pTransferNegotiationList->RemoveOne(_PlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CreateClasses
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::CreateClasses()
{
#ifdef YOUTHTRAINING
    m_pYouthSquadTrainingDetails = new CSquadTraining();
#endif
	m_pTacticsList = new CTacticList();
    m_pPlayerSearch = new CPlayerSearch();
    m_pNewsList = new CNewsList();
    m_pTransferNegotiationList = new CTransferNegotiationList();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoEndOfWeek
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoEndOfWeek()
{
	if (IsManagingAClub() == true)
	{
	    m_pNewsList->DoShrinkList(MAXIMUMCLUBNEWSITEMS);
#ifdef YOUTHTRAINING
		m_pYouthSquadTrainingDetails->DoEndOfWeekTasks(&GetClub());
#endif
    }
	m_WeeksPlayed++;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetWage
	Access:    	public 
	Parameter: 	void
	Returns:   	double
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
double CUser::GetWage()
{
	return GetManager().GetWage();
}


#ifdef MANAGEREXPENDITURE
/*------------------------------------------------------------------------------
	Method:   	CUser::WeeklyExpenditureStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CUser::WeeklyExpenditureStr()
{
	return GetManager().WeeklyExpenditureStr(GetClub().GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::WeeklyProfitLossStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CUser::WeeklyProfitLossStr()
{
	return GetManager().WeeklyProfitLossStr(GetClub().GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::BalanceStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CUser::BalanceStr()
{
	return GetManager().BalanceStr(GetClub().GetCountry());

}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetLifeStyle
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte  CUser::GetLifeStyle()
{
	return GetManager().GetLifeStyle();
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CUser::GetSalaryStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CUser::GetSalaryStr()
{
	if (IsManagingAClub() == true)
	{
		return GetManager().SalaryStr(GetClub().GetCountry());
	}
	else
	{
		return TEXT("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetFullName
	Access:    	public 
	Parameter: 	CString& str
	Parameter: 	CString& str1
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetFullName(CString& str, CString& str1)
{
    GetManager().SetForename(str);
    GetManager().SetSurname(str1);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoManageNewClub
	Access:    	public 
	Parameter: 	CClub& _rClubToManage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoManageNewClub(CClub& _ClubToManage, const uint _Amount, const ushort _Years)
{
	_ClubToManage.GetManagerContract().SetDetails(_Amount, _Years);
    WorldData().GetManager(m_ManagerID).DoNewClub(_ClubToManage);      // Manager controls this club
	_ClubToManage.SetManagerID(m_ManagerID);
	_ClubToManage.DoMakeUserControlled();
	m_pPlayerSearch->SetDefaults();
    m_pNewsList->RemoveList();
    m_pTransferNegotiationList->RemoveList();
#ifdef CUSTOMFORMATIONS
	GetTacticsList().ReCreate();
#endif
#ifdef ACTIVESTADIUMPLANS
	CPopularity* pPopularity = &_ClubToManage.GetPopularity();
	m_PriceBand1 = pPopularity->GetPriceBand(0);
	m_PriceBand2 = pPopularity->GetPriceBand(1);
	m_PriceBand3 = pPopularity->GetPriceBand(2);
	m_BoxPrice = pPopularity->GetBoxPrice();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CUser::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Read(&m_ManagerID, sizeof m_ManagerID);
    _pDataFile->Read(&m_WeeksPlayed, sizeof m_WeeksPlayed);
    m_pPlayerSearch->Read(_pDataFile);
    m_pNewsList->Read(_pDataFile);
    GetTacticsList().Read(_pDataFile);
    m_pTransferNegotiationList->Read(_pDataFile);
	_pDataFile->Read(&m_Chairman, sizeof CChairman);
#ifdef TROPHYHISTORY
	m_TrophyHistory.Read(_pDataFile);
#endif
#ifdef YOUTHTRAINING
    m_pYouthSquadTrainingDetails->Read(_pDataFile);
#endif
#ifdef ACTIVESTADIUMPLANS
    m_PriceBand1.Read(_pDataFile);
    m_PriceBand2.Read(_pDataFile);
    m_PriceBand3.Read(_pDataFile);
    _pDataFile->Read(&m_BoxPrice, sizeof m_BoxPrice);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CUser::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Write(&m_ManagerID, sizeof m_ManagerID);
    _pDataFile->Write(&m_WeeksPlayed, sizeof m_WeeksPlayed);
    m_pPlayerSearch->Write(_pDataFile);
    m_pNewsList->Write(_pDataFile);
    GetTacticsList().Write(_pDataFile);
    m_pTransferNegotiationList->Write(_pDataFile);
	_pDataFile->Read(&m_Chairman, sizeof CChairman);
#ifdef TROPHYHISTORY
	m_TrophyHistory.Write(_pDataFile);
#endif
#ifdef YOUTHTRAINING
    m_pYouthSquadTrainingDetails->Write(_pDataFile);
#endif
#ifdef ACTIVESTADIUMPLANS
    m_PriceBand1.Write(_pDataFile);
    m_PriceBand2.Write(_pDataFile);
    m_PriceBand3.Write(_pDataFile);
    _pDataFile->Write(&m_BoxPrice, sizeof m_BoxPrice);
#endif
 }


/*------------------------------------------------------------------------------
	Method:   	CUser::GetSupportersConfidence
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetSupportersConfidence()
{
	return GetManager().GetSupportersConfidence();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetBoardRating
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetBoardRating()
{
	return GetManager().GetBoardConfidence();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::QualifiedTopPlayOff
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoQualifiedTopPlayOff()
{
}


/*------------------------------------------------------------------------------
	Method:   	CUser::QualifiedBottomPlayOff
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoQualifiedBottomPlayOff()
{
}


/*------------------------------------------------------------------------------
	Method:   	CUser::Champions
	Access:    	public 
	Parameter: 	CDivision& _Division
	Parameter: 	CString _Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::Champions(CDivision& _Division, const CString &_Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
#ifdef TROPHYHISTORY
	m_TrophyHistory.AddChampions(_Division.GetName(), GetClubName(), _Season, _Division.GetTrophyID(), false);
#endif
	CExtString str;
	if (_Division.IsTopDivision() == true)
	{
		str.Format(IDS_USERCHAMPIONS, GetManager().GetName(), _Division.GetName());
	}
	else
	{
		str.Format(IDS_USERWONDIVISON, GetManager().GetName(), _Division.GetName());
	}
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::Champions
	Access:    	public 
	Parameter: 	CCup* pCup
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::Champions(CCup* _Cup, const CString &_Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
#ifdef TROPHYHISTORY
	m_TrophyHistory.AddChampions(_Cup->GetName(), GetClubName(), _Season, _Cup->GetTrophyID(), true);
#endif
	CExtString str;
	str.Format(IDS_USERWONCUP, GetManager().GetName(), _Cup->GetName());
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::WonCharityShield
	Access:    	public 
	Parameter: 	CString title
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::WonCharityShield(const CString& title)
{
	EXT_ASSERT(IsManagingAClub() == true);
	CExtString str;
	str.Format(IDS_USERWONSHIELD, GetManager().GetName(), title);
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::BeenPromotedViaPlayOff
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::BeenPromotedViaPlayOff ()
{
	CExtString str;
	str.Format(IDS_USERWONPLAYOFFPROMOTION, GetManager().GetName());
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoFinishedBottomOfDivision
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoFinishedBottomOfDivision()
{
	CExtString str;
	str.Format(IDS_USERFINISHEDBOTTOMOFDIVISION, GetManager().GetName());
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::AvoidedRelegation
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::AvoidedRelegation()
{
	CExtString str;
	str.Format(IDS_USERAVOIDEDRELEGATION, GetManager().GetName());
	AddToNews(str);
}


#ifdef TROPHYHISTORY
/*------------------------------------------------------------------------------
	Method:   	CUser::WonPlayoff
	Access:    	public 
	Parameter: 	CPlayOff* pPlayOff
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::WonPlayoff(CPlayOff* pPlayOff, CString Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddChampions(pPlayOff->GetName(), GetClubName(), Season, pPlayOff->GetTrophyID(), true);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::RunnersUpPlayoff
	Access:    	public 
	Parameter: 	CPlayOff* pPlayOff
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::RunnersUpPlayoff(CPlayOff* pPlayOff, const CString &Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddRunnersUp(pPlayOff->GetName(), GetClubName(), Season, pPlayOff->GetTrophyID(), true);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::RunnersUp
	Access:    	public 
	Parameter: 	CDivision& _Division
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::RunnersUp(CDivision& _Division, const CString &Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddRunnersUp(_Division.GetName(), GetClubName(), Season, _Division.GetTrophyID(), false);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::RunnersUp
	Access:    	public 
	Parameter: 	CCup* pCup
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::RunnersUp(CCup* pCup, const CString &Season)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddRunnersUp(pCup->GetName(), GetClubName(), Season, pCup->GetTrophyID(), true);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::TrophyWinners
	Access:    	public 
	Parameter: 	CString title
	Parameter: 	const byte Id
	Parameter: 	CString Season
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::TrophyWinners(const CString &title, const byte Id, const CString &Season, bool IsCup)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddChampions(title, GetClubName(), Season, Id, IsCup);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::TrophyRunnersUp
	Access:    	public 
	Parameter: 	CString title
	Parameter: 	const byte Id
	Parameter: 	CString Season
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::TrophyRunnersUp(const &CString title, const byte Id, const CString &Season, bool IsCup)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_TrophyHistory.AddRunnersUp(title, GetClubName(), Season, Id, IsCup);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CUser::Relegated
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::BeenRelegated()
{
	EXT_ASSERT(IsManagingAClub() == true);
	CExtString str;
	str.Format(IDS_USERRELEGATED, GetManager().GetName());
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::Promoted
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::BeenPromoted()
{
	EXT_ASSERT(IsManagingAClub() == true);
	CExtString str;
	str.Format(IDS_USERPROMOTED, GetManager().GetName());
	AddToNews(str);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetManagerID
	Access:    	public 
	Parameter: 	ushort Num
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetManagerID(const ushort _Num)
{
	m_ManagerID = _Num;
	if (m_ManagerID != NOMANAGER)
	{
		GetManager().SetIsUser(true);
		m_pPlayerSearch->SetDefaults();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetClubName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CUser::GetClubName()
{
	if (IsManagingAClub() == true)
	{
		return GetClub().GetName();
	}
	else
	{
		return TEXT("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::DoNewSeason()
{
	bool bRet = false;
	if ((IsManagingAClub() == true) && WorldData().IsFastTest() == false && GetClub().GetActiveCountry().GetSeasonStart() == WorldData().GetCurrentDate().GetMatchDay())
	{
		m_Chairman.DoNewSeason(GetClub().GetDivision(), GetClub());
#ifdef ACTIVEFRIENDLIES
		// Warn if new season and friendlies not arranged
		if (GetClub().GetFriendlies().CountAvailableFriendlies() == MAXFRIENDLIES)
		{
			bRet = true;
			CString str;
			str.Format(IDS_ARRANGEFRIENDLY, GetManager().GetName());
			UserMessageBox(str);
			theApp.OnArrangeFriendlies();	// Arrange friendlies
		}
#endif
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CloseSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::DoCloseSeason()
{
	bool bRet = false;
	if (WorldData().IsFastTest() == false && GetClub().GetDivision().IsCloseSeasonToday(WorldData().GetCurrentDate().GetMatchDay()) == true)
	{
		bRet = true;
		CString str;
		str.Format(IDS_CLOSESEASON, GetManager().GetName());
		UserMessageBox(str);
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::AddToNews
	Access:    	public 
	Parameter: 	CExtString Message
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::AddToNews(const CExtString &_Message)
{
	EXT_ASSERT(IsManagingAClub() == true);
	m_pNewsList->Add(new CNewsItem(_Message, WorldData().GetCurrentDate()));
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CheckIfValidTeam
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::CheckIfValidTeam()
{
	CMatchInfo Match;
	int SquadSize;
	CString cInvalidityReason;
	bool bReturnValue = true;

	if (IsManagingAClub() == true)
	{
		if (GetManager().IsMatchToday(WorldData().GetCurrentDate(), Match) == true)
		{
			SquadSize = PLAYERSINTEAM + Match.GetSubsSelect();
			if (GetClub().GetFirstTeamSquad().DoCountAvailablePlayers() < SquadSize)
			{
				cInvalidityReason.Format(IDS_NOTENOUGHFIT);
				bReturnValue = false;
			}
			else if (GetClub().IsValidUserControlledTeam(Match.GetSubsSelect()) == false)
			{
				bReturnValue = false;
			}
		}
	}

	if (bReturnValue == false)
	{
		UserMessageBox(cInvalidityReason, MB_OK | MB_ICONEXCLAMATION | MB_DEFBUTTON1);
	}

	return bReturnValue;	// Continue
}


/*------------------------------------------------------------------------------
	Method:   	CUser::IsMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime & _rCurrentDate
	Parameter: 	CMatchInfo & _rMatch
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match)
{
	if (IsActive() == true)
	{
		return GetManager().IsMatchToday(_CurrentDate, _Match);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DeleteAll
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DeleteAll()
{
	m_pNewsList->RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoCheckIfGotNoClub
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::DoCheckIfGotNoClub()
{
	if (GetManager().CheckIfGotContract() == false && GetManager().IsManagingClub() == true)
	{
		GetClub().DoRenewHumanManagerContract();
	}

	if (IsManagingAClub() == false)
	{
		theApp.OnSelectUserClubToManage(GetClubID());
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::IsManagingAClub
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::IsManagingAClub()
{
	if (IsActive() == true)
	{
		return GetManager().IsManagingClub();
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::IsManagingAClubInCountry
	Access:    	public 
	Parameter: 	CActiveCountry & _Country
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::IsManagingAClubInCountry(CActiveCountry& _Country)
{
	if (IsActive() == true)
	{
		return GetManager().IsManagingAClubInCountry(_Country);
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::IsActive
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::IsActive()
{
	return (m_ManagerID != NOMANAGER);
}


#ifdef CUSTOMFORMATIONS
/*------------------------------------------------------------------------------
	Method:   	CUser::ChangeTacticTitle
	Access:    	public 
	Parameter: 	int Which
	Parameter: 	CString String
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::ChangeTacticTitle(int Which, const CString &String)
{
	GetTacticsList().SetTitle(Which, String);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CUser::IsGoodEnoughToManageClub
	Access:    	public 
	Parameter: 	const ushort _PopularityID
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUser::IsGoodEnoughToManageClub(const ushort _PopularityID)
{
	return WorldData().GetPopularityList()[_PopularityID].IsUserGoodEnoughToManageClub((byte)GetManager().GetRating().GetRating());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetClub
	Access:    	public 
	Parameter: 	void
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CUser::GetClub()
{
	EXT_ASSERT(IsManagingAClub() == true);
	return WorldData().GetClub(GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetClubID
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CUser::GetClubID()
{
	if (IsManagingAClub() == true)
	{
		return GetManager().GetCurrentClubID();
	}
	else
	{
		return GetManager().GetLastClubID();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetCountryID
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CUser::GetCountryID()
{
	return GetClub().GetCountryID();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetPopularityID
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetPopularityID()
{
	return GetClub().GetPopularityID();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetTransferNegotiationList
	Access:    	public 
	Parameter: 	void
	Returns:   	CTransferNegotiationList*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiationList* CUser::GetTransferNegotiationList()
{
	return m_pTransferNegotiationList;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetNewsList
	Access:    	public 
	Parameter: 	void
	Returns:   	CNewsList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNewsList& CUser::GetNewsList()
{
	return *m_pNewsList;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetWeeksPlayed
	Access:    	public const 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
const ushort CUser::GetWeeksPlayed() const
{
	return m_WeeksPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GotNoClub
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::GotNoClub()
{
	SetManagerID(NOMANAGER); // 	No manager
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetGamesPlayedClub
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CUser::GetGamesPlayedClub()
{
	return GetManager().GetGamesPlayedClub();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetTacticsList
	Access:    	public 
	Parameter: 	void
	Returns:   	CTacticList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTacticList& CUser::GetTacticsList()
{
	return *m_pTacticsList;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoTraining
	Access:    	public 
	Parameter: 	const ushort _TacticID
	Parameter: 	const byte _Hours
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoTraining(const ushort _TacticID, const byte _Hours)
{
	GetTacticsList().GetAt(_TacticID).DoTrain(_Hours);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetManagerID
	Access:    	public const 
	Parameter: 	void
	Returns:	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CUser::GetManagerID() const
{
	return m_ManagerID;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetPlayerSearch
	Access:    	public 
	Parameter: 	void
	Returns:   	CPlayerSearch&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSearch& CUser::GetPlayerSearch()
{
	EXT_ASSERT(m_pPlayerSearch != null);
	return *m_pPlayerSearch;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DailyClubTasks
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DailyClubTasks()
{
	m_pTransferNegotiationList->DailyTasks(&GetClub());
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CheckTransferNegotiationsForSwapClubs
	Access:    	public 
	Parameter: 	const ushort _Club1ID
	Parameter: 	const ushort _Club2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::DoCheckTransferNegotiationsForSwapClubs(const ushort _Club1ID, const ushort _Club2ID)
{
	m_pTransferNegotiationList->DoCheckForSwapClubs(_Club1ID, _Club2ID);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::CheckIfPlayerInTransferNegotiations
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	CClub* pBiddingClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::CheckIfPlayerInTransferNegotiations(const ushort _PlayerID, CClub* _BiddingClub)
{
	m_pTransferNegotiationList->CheckIfPlayerInTransferNegotiations(_PlayerID, _BiddingClub, &GetClub());
}


#ifdef YOUTHTRAINING
/*------------------------------------------------------------------------------
	Method:   	CUser::SetYouthTeamTrainingHours
	Access:    	public 
	Parameter: 	const byte z
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetYouthTeamTrainingHours(const byte z)
{
    m_pYouthSquadTrainingDetails->SetTeamTrainingHours(z);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthsTrainingSkillsPercent
	Access:    	public 
	Parameter: 	const uint _YouthNum
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const byte CUser::GetYouthsTrainingSkillsPercent(const uint _YouthNum)
{
	return GetYouth(_YouthNum)->GetTrainingSkillsPercent();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetYouthsTrainingSkillsPercent
	Access:    	public 
	Parameter: 	const int x
	Parameter: 	const byte z
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetYouthsTrainingSkillsPercent(const int x, const byte z)
{
	GetYouth(x)->SetTrainingSkillsPercent(z);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetYouthsTrainingPacePercent
	Access:    	public 
	Parameter: 	const int x
	Parameter: 	const byte z
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetYouthsTrainingPacePercent(const int x, const byte z)
{
	GetYouth(x)->SetTrainingPacePercent(z);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::SetYouthsTrainingStrengthPercent
	Access:    	public 
	Parameter: 	const int x
	Parameter: 	const byte z
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::SetYouthsTrainingStrengthPercent(const int x, const byte z)
{
	EXT_ASSERT(IsManagingAClub() == true);
	GetYouth(x)->SetTrainingStrengthPercent(z);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthTeamTacticsToTrainHours
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetYouthTeamTacticsToTrainHours()
{
	EXT_ASSERT(IsManagingAClub() == true);
	return m_pYouthSquadTrainingDetails->GetTacticsToTrainHours();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthsTrainingPacePercent
	Access:    	public 
	Parameter: 	const int x
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetYouthsTrainingPacePercent(const int x)
{
	EXT_ASSERT(IsManagingAClub() == true);
	return GetYouth(x)->GetTrainingPacePercent();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthsTrainingStrengthPercent
	Access:    	public 
	Parameter: 	const int x
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetYouthsTrainingStrengthPercent(const int x)
{
	EXT_ASSERT(IsManagingAClub() == true);
	return GetYouth(x)->GetTrainingStrengthPercent();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthTeamTrainingHours
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CUser::GetYouthTeamTrainingHours()
{
	EXT_ASSERT(IsManagingAClub() == true);
    return m_pYouthSquadTrainingDetails->GetTeamTrainingHours();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouthSquadTrainingDetails
	Access:    	public 
	Parameter: 	void
	Returns:   	CSoccerSquadTraining*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSquadTraining* CUser::GetYouthSquadTrainingDetails()
{
	EXT_ASSERT(IsManagingAClub() == true);
	return m_pYouthSquadTrainingDetails;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CUser::TotalYouths
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CUser::TotalYouths()
{
	EXT_ASSERT(IsManagingAClub() == true);
	return GetClub().TotalYouths();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetChairman
	Access:    	public 
	Parameter: 	void
	Returns:   	CChairman&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CChairman& CUser::GetChairman()
{
	EXT_ASSERT(IsManagingAClub() == true);
	return m_Chairman;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::GetYouth
	Access:    	public 
	Parameter: 	int x
	Returns:   	CYouth*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CYouth* CUser::GetYouth(const uint _YouthNum)
{
	EXT_ASSERT(IsManagingAClub() == true);
	return GetClub().GetYouth(_YouthNum);
}


/*------------------------------------------------------------------------------
	Method:   	CUser::LeftClub
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUser::LeftClub()
{
	m_pTransferNegotiationList->RemoveList();
	GetChairman().DoManagerSacked();
}


/*------------------------------------------------------------------------------
	Method:   	CUser::DoPreMatchReport
	Access:    	public 
	Parameter: 	CMessageDisplay* _pMessageDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CUser::DoPreMatchReport(CMessageDisplay* _pMessageDisplay)
{
    CMatchInfo MatchDetails;
	CString szStr;
#ifdef TEST_MATCH_REPORT
	_pMessageDisplay->AddString(IDS_WONALLGAMES);
	_pMessageDisplay->AddString(IDS_NOTWONAGAME);
	_pMessageDisplay->AddString(IDS_WONAGAME);
	szStr.Format(IDS_SCOREDALOT, 10);
	_pMessageDisplay->AddString(szStr);
	szStr.Format(IDS_SCOREDFEW, 11);
	_pMessageDisplay->AddString(szStr);
	szStr.Format(IDS_CONCEDEDALOT, 12);
	_pMessageDisplay->AddString(szStr);
	szStr.Format(IDS_CONCEDEDFEW, 13);
	_pMessageDisplay->AddString(szStr);
	WorldData().GetClub(44).DoPreMatchReport(_pMessageDisplay);
#else
	if (IsMatchToday(WorldData().GetCurrentDate(), MatchDetails) == true)
	{
		CClub& OpponentClub = *MatchDetails.GetOpponentClub(&GetClub());
		if (OpponentClub.IsNonLeagueClub() == false && GetClub().DoCountLeagueGamesPlayedThisSeason() > 1)
		{
			CDivision&	OpponentDivision = OpponentClub.GetDivision();
			// if in first 5 games of season
			if (GetClub().DoCountLeagueGamesPlayedThisSeason() < 6)
			{
				if (OpponentClub.GetTableData().GetNumberGamesWon() == GetClub().DoCountLeagueGamesPlayedThisSeason())
				{
					_pMessageDisplay->AddString(IDS_WONALLGAMES);
				}
				else if (GetClub().DoCountLeagueGamesPlayedThisSeason() > 1 && OpponentClub.GetTableData().GetNumberGamesWon() == 0)
				{
					_pMessageDisplay->AddString(IDS_NOTWONAGAME);
				}
				else
				{
					switch(OpponentClub.GetTableData().GetNumberGamesWon())
					{
						case 0:
							_pMessageDisplay->AddString(IDS_NOTWONAGAME);
							break;
						case 1:
							_pMessageDisplay->AddString(IDS_WONAGAME);
							break;
						default:
							szStr.Format(IDS_WONGAMES, OpponentClub.GetTableData().GetNumberGamesWon());
							_pMessageDisplay->AddString(szStr);
							break;
					}
				}
			}
			else
			{
				// If in 5 highest scorers
				if (OpponentDivision.DoCountClubsScoredMore(OpponentClub) < 6)
				{
					szStr.Format(IDS_SCOREDALOT, OpponentClub.GetTableData().GetNumberGoalsScored());
					_pMessageDisplay->AddString(szStr);
				}

				// If in 5 lowest scorers
				if (OpponentDivision.DoCountClubsScoredLess(OpponentClub) < 6)
				{
					szStr.Format(IDS_SCOREDFEW, OpponentClub.GetTableData().GetNumberGoalsScored());
					_pMessageDisplay->AddString(szStr);
				}

				// If in 5 weakest defences
				if (OpponentDivision.DoCountClubsConcededMore(OpponentClub) < 6)
				{
					szStr.Format(IDS_CONCEDEDALOT, OpponentClub.GetTableData().GetNumberGoalsConceded());
					_pMessageDisplay->AddString(szStr);
				}

				// If in 5 best defences
				if (OpponentDivision.DoCountClubsConcededLess(OpponentClub) < 6)
				{
					szStr.Format(IDS_CONCEDEDFEW, OpponentClub.GetTableData().GetNumberGoalsConceded());
					_pMessageDisplay->AddString(szStr);
				}

				/*int iCurrentFormPosition = OpponentDivision.CurrentFormPosition(pOpponentClub);
				// If in top 5 form
				if (iCurrentFormPosition < 6)
				{
					strcat(szReport, "<l>- They're on a good run at the moment.");
					pMessageDisplay->AddString(szStr);
				}
				// If in bottom 5 form
				if (iCurrentFormPosition > OpponentDivision.NumTeams() - 5)
				{
					strcat(szReport, "<l>- They're not doing too well at the moment.");
					pMessageDisplay->AddString(szStr);
				} */
		OpponentClub.DoPreMatchReport(_pMessageDisplay);
			}
		}
	}
#endif
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CUserList::CUserList
	Access:    	public 
	Parameter: 	uint _HowMany
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUserList::CUserList(uint _HowMany)
{
	SetSize(_HowMany);
	for (uint n = 0; n < _HowMany; n++)
	{
        SetAt(n, new CUser(NOMANAGER));
    }
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::~CUserList
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUserList::~CUserList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::MakeInActive
	Access:    	public 
	Parameter: 	ushort ManagerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::MakeInActive(ushort _ManagerID)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos).GetManagerID() == _ManagerID)
		{
			GetAt(ListPos).SetManagerID(NOMANAGER);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::FindManager
	Access:    	public 
	Parameter: 	const ushort _ManagerID
	Returns:   	CUser&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUser& CUserList::FindManager(const ushort _ManagerID)
{
	int ListPos;
	for (ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos).GetManagerID() == _ManagerID)
		{
			break;
		}
	}
	EXT_ASSERT(ListPos < GetSize());
	return GetAt(ListPos);
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::DoMakeAllInActive
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::DoMakeAllInActive()
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        GetAt(ListPos).SetManagerID(NOMANAGER);
	}
}



/*------------------------------------------------------------------------------
	Method:   	CUserList::DoEndOfWeek
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::DoEndOfWeek()
{
	int SaveCurrentManager = WorldData().GetCurrentUserID();
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			WorldData().SetCurrentUserID(ListPos);
	        GetAt(ListPos).DoEndOfWeek();
		}
    }
	WorldData().SetCurrentUserID(SaveCurrentManager);
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::MonthlyTasks
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::DoMonthlyTasks()
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
	        GetAt(ListPos).DoMonthlyTasks();
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort nCount;
	nCount = GetSize();
    _pDataFile->Write(&nCount, sizeof nCount);

    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		GetAt(ListPos).Write(_pDataFile);
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CUserList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	RemoveList();
	ushort nCount;
    _pDataFile->Read(&nCount, sizeof nCount);

	EXT_ASSERT(nCount > 0 && nCount <= MAXHUMANMANAGERS);
    CUser* pUser;
	for (uint LoopCount = 0; LoopCount < nCount; LoopCount++)
	{
        pUser = new CUser();
		EXT_ASSERT(pUser != null);
		pUser->Read(_pDataFile);
        SetAtGrow(LoopCount, pUser);
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CUserList::RemoveAllManagers
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::RemoveAllManagers()
{
    // Ensure all clubs are unmanaged
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        GetAt(ListPos).GetClub().SetManagerID(NOMANAGER);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CUserList::NewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::DoNewSeason()
{
	bool bRet = false;
    // Ensure all clubs are unmanaged
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			if (GetAt(ListPos).DoNewSeason() == true)
			{
				bRet = true;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::DoCloseSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::DoCloseSeason()
{
	bool bRet = false;
    // Ensure all clubs are unmanaged
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			if (GetAt(ListPos).DoCloseSeason() == true)
			{
				bRet = true;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::CheckIfValidTeams
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:	Check if all human managed clubs have sufficient fit players 
				for today's matches
------------------------------------------------------------------------------*/
bool CUserList::CheckIfValidTeams()
{
	bool bRet = true;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			if (GetAt(ListPos).CheckIfValidTeam() == false)
			{
				bRet = false;
			}
		}
	}
	return bRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CUserList::IsMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime& CurrentDate
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match)
{
	bool bRet = false;
	int SaveMan = WorldData().GetCurrentUserID();
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (WorldData().SetCurrentUserID(ListPos) == true)
		{
	       if (GetAt(ListPos).IsMatchToday(_CurrentDate, _Match) == true)
		   {
				bRet = true;
		   }
		}
	}
	WorldData().SetCurrentUserID(SaveMan);
	return bRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CUserList::CheckIfGotNoClubs
	Access:    	public 
	Parameter: 	void
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::DoCheckIfGotNoClubs()
{
	bool bRet = false;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			if (GetAt(ListPos).DoCheckIfGotNoClub() == true)
			{
				bRet = true;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::CountActiveUsers
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CUserList::CountActiveUsers()
{
	ushort Count = 0;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		Count += (GetAt(ListPos).GetManagerID() != NOMANAGER);
	}
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::CheckTransferNegotiationsForSwapClubs
	Access:    	public 
	Parameter: 	ushort ClubA
	Parameter: 	ushort ClubB
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::DoCheckTransferNegotiationsForSwapClubs(const ushort _Club1ID, const ushort _Club2ID)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			GetAt(ListPos).DoCheckTransferNegotiationsForSwapClubs(_Club1ID, _Club2ID);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::CheckIfPlayerInTransferNegotiations
	Access:    	public 
	Parameter: 	ushort PlayerNum
	Parameter: 	CClub* pBiddingClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::CheckIfPlayerInTransferNegotiations(ushort _PlayerID, CClub* _pBiddingClub)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).GetManagerID() != NOMANAGER)
		{
			GetAt(ListPos).CheckIfPlayerInTransferNegotiations(_PlayerID, _pBiddingClub);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::IsUserManager
	Access:    	public 
	Parameter: 	const ushort _ManagerID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::IsUserManager(const ushort _ManagerID)
{
	bool Found = false;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        if (GetAt(ListPos).GetManagerID() == _ManagerID)
		{
            Found = true;
			break;
		}
	}
	return Found;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::AddToNews
	Access:    	public 
	Parameter: 	CActiveCountry & _Country
	Parameter: 	CExtString _Message
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CUserList::AddToNews(CActiveCountry& _Country, const CExtString &_Message)
{
	bool Found = false;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).IsActive() && GetAt(ListPos).IsManagingAClubInCountry(_Country))
		{
			GetAt(ListPos).AddToNews(_Message);
			Found = true;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::AnyActiveUsers
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CUserList::AnyActiveUsers()
{
	bool Found = false;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).IsActive())
		{
			Found = true;
		}
	}
	return Found;
}


/*------------------------------------------------------------------------------
	Method:   	CUserList::FindFirstActive
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CUserList::FindFirstActive()
{
	int iRet = NOMANAGER;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos).IsActive())
		{
			iRet = ListPos;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
Method:   	CUser::GetAt
Access:    	public 
Parameter: 	const ushort _Index
Returns:   	CUser&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CUser& CUserList::GetAt(const ushort _Index)                               
{ 
	return *(CPointerArray<CUser>::GetAt(_Index));
}
