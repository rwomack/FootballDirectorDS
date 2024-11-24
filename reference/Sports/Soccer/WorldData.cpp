
 /*

	File:- WorldData.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "MatchInfo.h"
#include "FootballDirector.h"
#include "SaveGameHeader.h"


	const CString			FileExtension = TEXT("fd");		// File extension for save game files


/*------------------------------------------------------------------------------
	Method:   	UserMessageBox
	Access:    	public 
	Parameter: 	UINT Message
	Parameter: 	UINT Style
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int UserMessageBox(UINT Message, UINT Style)
{
	int iRet = AfxMessageBox(Message, Style);
	theApp.GetMainWnd()->UpdateWindow();
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	UserMessageBox
	Access:    	public 
	Parameter: 	CString& Message
	Parameter: 	UINT Style
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int UserMessageBox(CString& Message, UINT Style)
{
	EXT_ASSERT(Message.IsEmpty() == FALSE);
	int iRet = AfxMessageBox(Message, Style);
	theApp.GetMainWnd()->UpdateWindow();
	return iRet;
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnViewMatchResultInformation
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnViewMatchResultInformation()
{
	EXT_ASSERT(m_pMatchInfo != null);
	m_pMatchInfo->GetFixture()->DoShowSummary(null);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnSackEmployee
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::OnSackEmployee()
{
	return GetCurrentUserClub()->AskSackEmployee(m_pCurrentEmployee);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnHireYouth
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnHireYouth()
{
	CString strYouthName = GetCurrentYouth()->GetName();
	GetCurrentUserClub()->DoAddYouthToSquad(GetCurrentYouth());
	GetCurrentUserCountry()->GetActiveCountry().SackYouth(GetCurrentYouth());
	CString strText;
	strText.Format(IDS_HIREDYOUTH, strYouthName);
	AfxMessageBox(strText);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnSackYouth
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::OnSackYouth()
{
	return GetCurrentUserClub()->AskSackYouth(GetCurrentYouth());
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnViewSquadDetails
	Access:    	public 
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnViewSquadDetails(CClub* _Club)
{
	SetCurrentClub(_Club);
	theApp.OnViewSquadDetails();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnViewPlayerDetails
	Access:    	public 
	Parameter: 	CPlayer* _pPlayer 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnViewPlayerDetails(CPlayer* _pPlayer /*= null*/)
{
	if (_pPlayer != null)
	{
		SetCurrentPlayer(_pPlayer);
	}
	if (GetCurrentPlayer()->IsAtCurrentUserControlledClub() == true)
	{
		theApp.OnViewOwnedPlayerInformation();
	}
	else
	{
		theApp.OnViewNotOwnedPlayerInformation();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoNegotiatePlayerContract
	Access:    	public 
	Parameter: 	CPlayer & _Player
	Parameter: 	CClub & _Club
	Parameter: 	CString _strPrompt
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoNegotiatePlayerContract(CPlayer* _Player, CClub* _Club, const CString &_strPrompt)
{
	SetCurrentPlayer(_Player);
	SetCurrentClub(_Club);
	theApp.OnNegotiatePlayerContract(_strPrompt);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetSaveFileSuffix
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CString CWorld::GetSaveFileSuffix() const
{
	return TEXT(".FD3");
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetDataLevel
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CWorld::GetDataLevel() const			
{ 
	return 5; 
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentPlayer
	Access:    	public 
	Parameter: 	CPlayer* _pPlayer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetCurrentPlayer(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	m_pCurrentPlayer = _Player;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentPlayer
	Access:    	public 
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CWorld::GetCurrentPlayer()
{
	EXT_ASSERT_POINTER(m_pCurrentPlayer, CPlayer);
	return m_pCurrentPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentYouth
	Access:    	public 
	Parameter: 	CYouth* _pYouth
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetCurrentYouth(CYouth* _Youth)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	m_pCurrentYouth = _Youth;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentYouth
	Access:    	public 
	Returns:   	CYouth*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CYouth* CWorld::GetCurrentYouth()
{
	EXT_ASSERT_POINTER(m_pCurrentYouth, CYouth);
	return m_pCurrentYouth;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentClub
	Access:    	public 
	Parameter: 	CClub* pClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetCurrentClub(CClub* _pClub)
{
	EXT_ASSERT_POINTER(_pClub, CClub);
	m_pCurrentClub = _pClub;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CWorld::GetCurrentClub()
{
	EXT_ASSERT_POINTER(m_pCurrentClub, CClub);
	return m_pCurrentClub;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentEmployee
	Access:    	public 
	Parameter: 	CEmployee* _pEmployee
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetCurrentEmployee(CEmployee* _pEmployee)
{
	EXT_ASSERT_POINTER(_pEmployee, CEmployee);
	m_pCurrentEmployee = _pEmployee;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentEmployee
	Access:    	public 
	Returns:   	CEmployee*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployee* CWorld::GetCurrentEmployee()
{
	EXT_ASSERT(m_pCurrentEmployee != null);
	return m_pCurrentEmployee;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentMatchInfo
	Access:    	public 
	Parameter: 	CMatchInfo* _pMatchInfo
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetCurrentMatchInfo(CMatchInfo* _pMatchInfo)
{
	EXT_ASSERT(_pMatchInfo != null);
	m_pMatchInfo = _pMatchInfo;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CWorld
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CWorld::CWorld()
	: m_FastTest(false)
	, m_MasterState(IN_MENUS)
	, m_TotalClubs(0)
    , m_pPlayOffList(null)
	, m_pCharityShieldList(null)
	, m_pCupList(null)
    , m_pUserList(new CUserList(MAXHUMANMANAGERS))
    , m_pCurrentPlayer(null)
    , m_pCurrentYouth(null)
    , m_pCurrentEmployee(null)
    , m_pCurrentClub(null)
    , m_CurrentUserID(NOMANAGER)
	, m_eResultTypeToShow(MATCHES_NONE)
	, m_pMatchInfo(null)
#ifdef ACTIVEINTERFEDERATIONCUPS
	, m_pInterFederationCupList(null) 
#endif
 #ifdef	ACTIVEREFEREES
    , m_pRefereeList(new CSoccerRefereeList())
 #endif
 #ifdef	ACTIVESTADIUMPLANS
    , m_pStadiumPlanList(new CStadiumPlanList())
 #endif
{
	m_CurrentDate.DoInitialise(0, 0, 0, 2007, 7, 1);
 #ifndef USE_CURRENCIES
	// Work in £'s only
	m_Currency.DoInitialise();
 #endif
}


static bool s_bUseSpecificRandomSeed = false;
static unsigned int s_uiSpecificRandomSeed = 42;

/*------------------------------------------------------------------------------
	Method:   	CWorld::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoInitialise()
{
 	m_TransferDeadlineSystem.DoInitialise();
 	m_KitSystem.DoInitialise();
 	m_CountryList.DoInitialise();
 	m_DivisionList.DoInitialise();
 	m_InjuryTypeList.DoInitialise();
 	m_InjuryEventTypeList.DoInitialise();
 	
 	if (s_bUseSpecificRandomSeed == false)
 	{
 		RandomNumber.Randomize();		// Set random seed
 	}
 	else
 	{
 		RandomNumber.SetSeed(s_uiSpecificRandomSeed);	// Set a specific seed.
 	}
	CBinaryReader theResource(0, IDR_SURNAME, TEXT("FDDATA"));
	m_strSurnames.DoInitialiseFromResource(theResource, true);
    m_ManagerList.DoInitialise();
	m_ClubList.DoInitialise();
	m_StadiumList.DoInitialise();
	m_PlayerList.DoInitialise(m_TotalClubs);
 
	RandomNumber.Randomize();		// Set random seed
    m_pPlayOffList = new CPlayOffList();
    m_pCharityShieldList = new CCharityShieldList();
    m_pCupList = new CCupList();
	m_FederationCupList.DoInitialiseFromResource();
 
	// Initialise players who are currently on loan
	theResource.LoadData(0, IDR_PLAYERLOANS, TEXT("FDDATA"));
	short nOrginalListSize = theResource.ReadUInt16();
	for (int ListPos = 0; ListPos < nOrginalListSize; ListPos ++)
	{
	    ushort ClubID = theResource.ReadUInt16();
	    ushort PlayerID = theResource.ReadUInt16();
		OnLoanPlayerOut(PlayerID, ClubID, MAXLOANPLAYERWEEKS);
	}
 	
	m_ClubList.PostInitialise();
	m_pCharityShieldList->DoDraws();
	m_CurrentDate.NextMatchDay();
    m_CountryList.PostInitialise();
	m_MasterState = IN_INITIALISATION;
    m_CountryList.PlayMissedMatches();
	m_MasterState = IN_MENUS;
    m_DivisionList.UpdateTables();
 
// 	for (int ListPos = 0; ListPos < m_StadiumList.GetSize(); ListPos++)
// 	{
// 		DEBUG_OUTPUT(("%d, %s, %d"), (ListPos, GetStadium(ListPos)->GetName(), GetStadium(ListPos)->GetCapacity()));
// 	}
 
	DEBUG_ONLY(g_DebugStr.Format(_T("Player %d | %d | %d"), sizeof(CPlayer), m_PlayerList.GetSize(), sizeof(CPlayer) * m_PlayerList.GetSize()));
	DEBUG_ONLY(g_DebugStr.OutputDebug());
	DEBUG_ONLY(g_DebugStr.Format(_T("Club %d | %d | %d"), sizeof(CClub), m_ClubList.GetSize(), sizeof(CClub) * m_ClubList.GetSize()));
	DEBUG_ONLY(g_DebugStr.OutputDebug());
	DEBUG_ONLY(g_DebugStr.Format(_T("Fixture %d"), sizeof(CFixture)));
	DEBUG_ONLY(g_DebugStr.OutputDebug());
	OnCreateUser();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnLoanPlayerOut
	Access:    	public 
	Parameter: 	const ushort PlayerID
	Parameter: 	const ushort ClubID
	Parameter: 	const int _NumberOfWeeks
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnLoanPlayerOut(const ushort _PlayerID, const ushort _ToClubID, const int _NumberOfWeeks)
{
	GetPlayer(_PlayerID)->GetClub().GetFirstTeamSquad().GotPlayerOnLoan();
	if (_ToClubID != NOCLUB)
	{
		// Only use original club if has room in squad for player
		if (GetPlayer(_PlayerID)->GetClub().GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD -  1)
		{
			GetClub(_ToClubID).GetFirstTeamSquad().DoIncrementPlayersOutOnLoan();
			GetPlayer(_PlayerID)->DoGoingOnLoan(&GetClub(_ToClubID), _NumberOfWeeks);
		}
	}
	else
	{
		GetPlayer(_PlayerID)->DoGoingOnLoan(NULL, _NumberOfWeeks);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::~CWorld
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWorld::~CWorld()
{
	SetCurrentUserID(NOMANAGER);
    m_PlayerList.RemoveList();
#ifdef	ACTIVEREFEREES
    delete m_pRefereeList;
#endif
    m_InjuryList.RemoveList();
#ifdef	ACTIVESTADIUMPLANS
    delete m_pStadiumPlanList;
#endif
    delete m_pPlayOffList;
    delete m_pCharityShieldList;
    m_ClubList.RemoveList();
    delete m_pUserList;
	m_ManagerList.RemoveList();
    delete m_pCupList;
	m_CurrentUserID = NOMANAGER;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoBuildCupNameList
	Access:    	public 
	Parameter: 	CSortedStringList & _List
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoBuildCupNameList(CSortedStringList& _List)
{
	GetCupList().DoBuildNameList(_List);
	GetFederationCupList().DoBuildNameList(_List);
}

/*------------------------------------------------------------------------------
	Method:   	CWorld::OnCreateUser
	Access:    	public 
	Parameter: 	const ushort _UserID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnCreateUser(const ushort _UserID /*= 0*/)
{
	CUserList &cUserList = GetUserList();

	// Ensure that if we're overwriting a user, they don't leak.
	CUser *pExistingUser = cUserList.CPointerArray<CUser>::GetAt(_UserID);
	if (pExistingUser != NULL)
	{
		// Since we're going to replace that array entry momentarily anyway,
		// we won't remove it.
		delete pExistingUser;
	}

	cUserList.SetAt(_UserID, new CUser());
	SetCurrentUserID(_UserID);
	GetCurrentUser()->SetManagerID(GetManagerList().Add(new CManager(6000, 5)));
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::OnInitialiseUser
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Parameter: 	const ushort _nUser
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::OnInitialiseUser(const ushort _ClubID, const ushort _UserID /*= 0*/)
{
	uint Wage;
	//GetClub(_ClubID).GetFirstTeamSquad().DoSelectNonUserControlledTeam(5, GetClub(_ClubID));
	if (GetClub(_ClubID).GetManagerID() != NOMANAGER)
	{
		Wage = GetClub(_ClubID).GetManager().GetWage() * 52;
		GetClub(_ClubID).DoSackManager(false);
	}
	else
	{
		Wage = GetClub(_ClubID).GetManagerContract().GetWageOffer() * 52;
	}
	SetCurrentUserID(_UserID);
	GetCurrentUser()->DoManageNewClub(GetClub(_ClubID), Wage, 4);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetProcessingHalt
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetProcessingHalt(bool _Value)
{
	m_bProcessingHalt = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetFixturesToView
	Access:    	public 
	Parameter: 	const eClubResultsType _MatchType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetFixturesToView(const eClubResultsType _MatchType)
{
	m_eResultTypeToShow = _MatchType;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFixturesToView
	Access:    	public 
	Parameter: 	void
	Returns:   	eClubResultsType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eClubResultsType CWorld::GetFixturesToView()
{
	return m_eResultTypeToShow;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetTotalClubs
	Access:    	public const 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::GetTotalClubs() const
{
	return m_TotalClubs;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetTotalClubs
	Access:    	public 
	Parameter: 	const ushort _Number
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetTotalClubs(const ushort _Number)
{
	m_TotalClubs = _Number;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetPlayerList
	Access:    	public 
	Parameter: 	void
	Returns:   	CPlayerList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerList& CWorld::GetPlayerList()
{
    return m_PlayerList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetContractClauseList
	Access:    	public 
	Parameter: 	void
	Returns:   	CContractClauseList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractClauseList& CWorld::GetContractClauseList()
{
    return m_ContractClauseList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetPlayOffList
	Access:    	public 
	Parameter: 	void
	Returns:   	CPlayOffList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOffList& CWorld::GetPlayOffList()
{
	EXT_ASSERT_POINTER(m_pPlayOffList, CPlayOffList);
    return *m_pPlayOffList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCharityShieldList
	Access:    	public 
	Parameter: 	void
	Returns:   	CCharityShieldList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCharityShieldList& CWorld::GetCharityShieldList()
{
	EXT_ASSERT_POINTER(m_pCharityShieldList, CCharityShieldList);
    return *m_pCharityShieldList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFreeFixturesList
	Access:    	public 
	Parameter: 	void
	Returns:   	CFixtureList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList& CWorld::GetFreeFixturesList()
{
    return m_FreeFixtureList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFreeMatchEventsList
	Access:    	public 
	Parameter: 	void
	Returns:   	CMatchEventList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchEventList& CWorld::GetFreeMatchEventsList()
{
    return m_FreeMatchEventList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetClubList
	Access:    	public 
	Parameter: 	void
	Returns:   	CClubList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubList& CWorld::GetClubList()
{
    return m_ClubList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCountryList
	Access:    	public 
	Parameter: 	void
	Returns:   	CCountryList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountryList& CWorld::GetCountryList()
{
    return m_CountryList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetManagerList
	Access:    	public 
	Returns:   	CManagerList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerList& CWorld::GetManagerList()
{
    return m_ManagerList;
}


#ifdef USE_CURRENCIES
//---------------------------------------------------------------------------------------
CCurrencyList& CWorld::GetCurrencyList()
{
    return m_CurrencyList;
}
#else

/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrency
	Access:    	public 
	Returns:   	CCurrency&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCurrency& CWorld::GetCurrency()
{
	return m_Currency;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetDivisionList
	Access:    	public 
	Returns:   	CDivisionList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionList& CWorld::GetDivisionList()
{
    return m_DivisionList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetOutOfContractPlayerList
	Access:    	public 
	Returns:   	COutOfContractPlayerList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COutOfContractPlayerList& CWorld::GetOutOfContractPlayerList()
{
	return m_OutOfContractPlayerList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetInjuryList
	Access:    	public 
	Returns:   	CInjuryList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryList& CWorld::GetInjuryList()
{
	return m_InjuryList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetInjuryTypeList
	Access:    	public 
	Returns:   	CInjuryTypeList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryTypeList& CWorld::GetInjuryTypeList()
{
	return m_InjuryTypeList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetInjuryEventTypeList
	Access:    	public 
	Returns:   	CInjuryEventTypeList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryEventTypeList& CWorld::GetInjuryEventTypeList()
{
	return m_InjuryEventTypeList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFixtureController
	Access:    	public 
	Returns:   	CFixtureController&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureController& CWorld::GetFixtureController()
{
	return m_FixtureController;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCupList
	Access:    	public 
	Returns:   	CCupList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupList& CWorld::GetCupList()
{
	EXT_ASSERT_POINTER(m_pCupList, CCupList);
    return *m_pCupList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetDivisionStrengthList
	Access:    	public 
	Returns:   	CDivisionStrengthList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionStrengthList& CWorld::GetDivisionStrengthList()
{
    return m_DivisionStrengthList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetPopularityList
	Access:    	public 
	Returns:   	CPopularityList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPopularityList& CWorld::GetPopularityList()
{
    return m_PopularityList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetAggressionList
	Access:    	public 
	Returns:   	CAggressionList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CAggressionList& CWorld::GetAggressionList()
{
    return m_AggressionList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetUserList
	Access:    	public 
	Returns:   	CUserList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUserList& CWorld::GetUserList()
{
	EXT_ASSERT_POINTER(m_pUserList, CUserList);
    return *m_pUserList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetKitSystem
	Access:    	public 
	Returns:   	CKitSystem&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKitSystem& CWorld::GetKitSystem()
{
	return m_KitSystem;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetTransferDeadlineSystem
	Access:    	public 
	Returns:   	CTransferDeadlineSystem&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTransferDeadlineSystem& CWorld::GetTransferDeadlineSystem()
{
	return m_TransferDeadlineSystem;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetPlayer
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CWorld::GetPlayer(const ushort _PlayerID)
{
	if (_PlayerID >= m_PlayerList.GetSize())
	{
		g_DebugStr.Format(_T("Requesting invalid playerID %d"), _PlayerID);
		g_DebugStr.OutputDebug();
	}
	EXT_ASSERT(_PlayerID < m_PlayerList.GetSize());
    return &m_PlayerList[_PlayerID];
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetClub
	Access:    	public 
	Parameter: 	const ushort _ClubIndex
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CWorld::GetClub(const ushort _ClubIndex)
{
	EXT_ASSERT(_ClubIndex < m_ClubList.GetSize());
    return m_ClubList.GetAt(_ClubIndex);
}

/*------------------------------------------------------------------------------
	Method:   	CWorld::GetClubByID
	Access:    	public
	Parameter: 	const ushort _ClubID
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CWorld::GetClubByID(const ushort _ClubID)
{
	return m_ClubList.GetByID(_ClubID);
}

/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCountry
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CWorld::GetCountry(const ushort _ID)
{
    return m_CountryList.GetAt(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCup
	Access:    	public 
	Parameter: 	ushort _CupID
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CWorld::GetCup(ushort _CupID)
{
	// Find dometsic or federation cup
	if (_CupID < m_pCupList->GetSize())
	{
		return m_pCupList->GetAt(_CupID);
	}
	else
	{
		return m_FederationCupList.GetAt(_CupID - m_pCupList->GetSize());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetStadium
	Access:    	public 
	Parameter: 	ushort _ID
	Returns:   	CStadium*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStadium* CWorld::GetStadium(ushort _ID)
{
	if (_ID >= m_StadiumList.GetSize())
	{
		_ID = RandomNumber.IntLessThan(m_StadiumList.GetSize() - 1);
	}
	EXT_ASSERT(_ID < m_StadiumList.GetSize());
    return static_cast<CStadium*>(m_StadiumList.GetAt(_ID));
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetManager
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	CManager&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager& CWorld::GetManager(const ushort _ID)
{
	EXT_ASSERT(_ID < m_ManagerList.GetSize());
    return *m_ManagerList.GetAt(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentDate
	Access:    	public 
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CWorld::GetCurrentDate()
{
    return m_CurrentDate;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetForename
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CWorld::GetForename(const ushort _ID)
{
	return m_strForenames.GetString(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetSurname
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CWorld::GetSurname(const ushort _ID)
{
	return m_strSurnames.GetString(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::FindForenameID
	Access:    	public 
	Parameter: 	const CString & _NewName
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::DoFindForenameID(const CString& _NewName)
{
	return m_strForenames.DoFindStringID(_NewName);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::FindSurnameID
	Access:    	public 
	Parameter: 	const CString & _NewName
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::DoFindSurnameID(const CString& _NewName)
{
	return m_strSurnames.DoFindStringID(_NewName);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::FindStrID
	Access:    	public 
	Parameter: 	const CString & _nNewName
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::DoFindStrID(const CString& _nNewName)
{
	return m_strIDs.DoFindStringID(_nNewName);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCareerPaths
	Access:    	public 
	Parameter: 	void
	Returns:   	CPlayerCareerPathList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerCareerPathList& CWorld::GetCareerPaths()
{
	return m_CareerPathList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetSkillMoneyValueItem
	Access:    	public 
	Parameter: 	const short _Index
	Returns:   	CSkillMoneyValue*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSkillMoneyValue& CWorld::GetSkillMoneyValueItem(const short _Index)
{
	EXT_ASSERT(_Index <= MAXPLAYSKILL);
	return m_SkillMoneyValueList[_Index];
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserName
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CWorld::CurrentUserName()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (GetCurrentUser()->IsActive() == true)
	{
		return GetCurrentUser()->GetManager().GetName();
	}
	else
	{
		return TEXT("");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CWorld::GetCurrentUserClub()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (m_CurrentUserID == NOMANAGER || GetCurrentUser()->IsManagingAClub() == false)
	{
		return null;
	}
	return &GetCurrentUser()->GetClub();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::CurrentUserClubID()
{
	if (m_CurrentUserID == NOMANAGER)
	{
		return NOMANAGER;
	}

	return GetCurrentUser()->GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserDivisionID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CWorld::CurrentUserDivisionID()
{
	if (m_CurrentUserID == NOMANAGER || GetCurrentUser()->IsManagingAClub() == false)
	{
		return NODIVISION;
	}

	return GetCurrentUser()->GetClub().GetDivisionID();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserClubPlayer
	Access:    	public 
	Parameter: 	const ushort _SquadNum
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CWorld::GetCurrentUserClubPlayer(const ushort _SquadNum)
{
	return &GetCurrentUser()->GetClub().GetPlayer(_SquadNum);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetString
	Access:    	public 
	Parameter: 	const ushort _nClubID
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CWorld::GetString(const ushort _nClubID)
{
	return m_strIDs.GetString(_nClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetText
	Access:    	public 
	Parameter: 	void
	Returns:   	CNonDuplicateStringList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNonDuplicateStringList& CWorld::GetText()
{
	return m_TextList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserDivision
	Access:    	public 
	Parameter: 	void
	Returns:   	CDivision*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision* CWorld::GetCurrentUserDivision()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (GetCurrentUser()->IsManagingAClub() == true)
	{
		return &GetCurrentUser()->GetClub().GetDivision();
	}
	else
	{
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserCountry
	Access:    	public 
	Parameter: 	void
	Returns:   	CCountry*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry* CWorld::GetCurrentUserCountry()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (GetCurrentUser()->IsManagingAClub() == true)
	{
		return &GetCurrentUser()->GetClub().GetCountry();
	}
	else
	{
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserManager
	Access:    	public 
	Parameter: 	void
	Returns:   	CManager*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager* CWorld::GetCurrentUserManager()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (GetCurrentUser()->IsActive() == true)
	{
		return &GetCurrentUser()->GetManager();
	}
	else
	{
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::FindFreeFixture
	Access:    	public 
	Parameter: 	void
	Returns:   	CFixture*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CWorld::FindFreeFixture()
{
	CFixture* pFixture;
	if (GetFreeFixturesList().GetSize() > 0)
	{
		pFixture = &GetFreeFixturesList()[GetFreeFixturesList().GetUpperBound()];
		GetFreeFixturesList().RemoveAt(GetFreeFixturesList().GetUpperBound());
	}
	else
	{
		pFixture = new CFixture();
	}
	EXT_ASSERT(pFixture != null);
	return pFixture;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetForwardDateShortStr
	Access:    	public 
	Parameter: 	const uint _DatesToNegotiation
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CWorld::GetForwardDateShortStr(const uint _DatesToNegotiation)
{
	CCalendar CurrentDate;
	CurrentDate = m_CurrentDate;
	for (uint LoopCount = 0; LoopCount < _DatesToNegotiation; LoopCount++)
	{
		CurrentDate.NextMatchDay();
	}
	return CurrentDate.LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetCurrentUserID
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::SetCurrentUserID(const ushort _ID)
{
	//EXT_ASSERT(_ID < MAXHUMANMANAGERS);
	//if (_ID != NOMANAGER)
	{
		m_CurrentUserID = _ID;
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::MakeUserInActive
	Access:    	public 
	Parameter: 	const ushort _ManagerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::MakeUserInActive(const ushort _ManagerID)
{
	GetUserList().MakeInActive(_ManagerID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::GetCurrentUserID()
{
	return m_CurrentUserID;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoCountryJoinFederation
	Access:    	public 
	Parameter: 	const byte _CountryID
	Parameter: 	const byte _FederationID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoCountryJoinFederation(const byte _CountryID, const byte _FederationID)
{
    m_FederationList.DoCountryJoinFederation(_CountryID, _FederationID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUser
	Access:    	public 
	Returns:   	CUser*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CUser* CWorld::GetCurrentUser()
{
	if (m_CurrentUserID != NOMANAGER)
	{
		return &(GetUserList().GetAt(m_CurrentUserID));
	}
	EXT_ASSERT(false); // Need to stop it getting here
	return null;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserCanProgress
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:	Is it ok for the progress loop to process this date in
				skip to next match mode?
------------------------------------------------------------------------------*/
bool CWorld::CurrentUserCanProgress()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	CMatchInfo MatchDetails;
	if (m_pUserList->IsMatchToday(m_CurrentDate, MatchDetails) == false && GetCurrentUser()->IsManagingAClub() == true)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetProcessingHalt
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::GetProcessingHalt() const
{
	return m_bProcessingHalt;
}

 // -----------------------------------------------------------------------
void CWorld::RestoreProgressDuringDivisionMatches()
{
//	MatchProgressScreen(MATCHPROGRESSBARLENGTH, m_CurrentDate.ShortDateStr(), 3, true);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::RestoreProgressDuringDailyTasks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::RestoreProgressDuringDailyTasks()
{
	theApp.GetMainWnd()->Invalidate();
	theApp.GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserCantProgress
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::CurrentUserCantProgress()
{
#ifdef FMOD	// On a live device, not during dev device
	if (m_CurrentDate.GetMonth() > SEPTEMBER && m_CurrentDate.GetYear() > 2006 && theApp.IsRegistered(true) == false)
    {
		return true;
	}
#endif

	CMatchInfo MatchDetails;
	if (GetCurrentUserManager()->IsMatchToday(GetCurrentDate(), MatchDetails) == true)
	{
		EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
		if (GetCurrentUserClub()->IsValidUserControlledTeam(MatchDetails.GetSubsSelect()) == false)
		{
			theApp.OnViewSquadDetails();
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetMasterState
	Access:    	public 
	Returns:   	CWorld::eMasterState
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWorld::eMasterState CWorld::GetMasterState()
{
	return m_MasterState;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SetMasterState
	Access:    	public 
	Parameter: 	const eMasterState _eState
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::SetMasterState(const eMasterState _eState)
{
	m_MasterState = _eState;
}


/*------------------------------------------------------------------------------
Method:   	CWorld::DoSkipToEndOfSeason
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoSkipToEndOfSeason()
{
	return;
	CMatchInfo MatchDetails;
	SetFastTest(true);
	do
	{
		//  Play all matches, view any matches involving humans if requested
		//pView->GetProgressCtl()->SetPos(0);
		DoProcessMatches(false);
		//pView->OnSetDate();
		Sleep(2);
		theApp.GetMainWnd()->UpdateWindow();

		if (m_bProcessingHalt == true)
		{
			break;
		}
//  		DEBUG_OUTPUT(("Matchnumber %d Date %s"), (GetCurrentDate().GetMatchDay(), GetCurrentDate().LongDateStr()));
	} 
	//while (m_CurrentDate.GetSeason() < 12);
	while (GetCurrentDate().GetMatchDay() != 8);  // 24 cl g1
	SetFastTest(false);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoProcessMatches
	Access:    	public 
	Parameter: 	const bool _ViewEm
	Parameter: 	const bool _ShowSummary
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoProcessMatches(const bool _ViewEm, const bool _ShowSummary /*= false*/)
{
	if (m_CurrentDate.FirstDateOfMonth() == true)
	{
		DoAutoSave();
	}
	m_bProcessingHalt = false;

	SetMasterState(IN_PROCESSING);
	theApp.OnIncrementProgress();		// 1
	if (m_CountryList.DoEndOfSeason() == true)
	{
		// Stop progress if season ended
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}
	m_CountryList.DoNewSeason();

	if (m_pUserList->DoNewSeason() == true)
	{
// 		if (IsFastTest() == false)
// 		{
// 			m_bProcessingHalt = true;
// 		}
	}
	if (m_pUserList->DoCloseSeason() == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}
#ifdef	ACTIVEREFEREES
	// Only used for referee info at the moment
    m_CountryList.DoDailyClearDown();
#endif
#ifdef ACTIVEFEDERATIONCUPS
	m_FederationCupList.DoNewSeason();
	TRACE(TEXT("Federation Cup New Season\n"));
#endif
	theApp.OnIncrementProgress();	// ******************
    // If not midweek match
    if (m_CurrentDate.IsWeekEnd() != true)
	{
        m_ClubList.DoWeeklyClearDown();  // Clear all last weeks data out
    }
	theApp.OnIncrementProgress();	// ******************
	DEBUG_OUTPUT(("%d | Playing Federation Cup Matches"), (GetCurrentDate().GetMatchDay()));
#ifdef ACTIVEFEDERATIONCUPS
    if (m_FederationCupList.DoPlayMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}
#endif

	DEBUG_OUTPUT(("%d | Playing Charity Shield Matches"), (GetCurrentDate().GetMatchDay()));
    if (m_pCharityShieldList->DoPlayMatches(_ShowSummary) == true)
	{
// 		if (IsFastTest() == false)
// 		{
// 			m_bProcessingHalt = true;
// 		}
	}

	DEBUG_OUTPUT(("%d | Playing Playoffs Matches"), (GetCurrentDate().GetMatchDay()));
    if (m_pPlayOffList->DoPlayMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

	DEBUG_OUTPUT(("%d | Playing Cup Matches"), (GetCurrentDate().GetMatchDay()));
	theApp.OnIncrementProgress();	// 2
	if (m_pCupList->DoPlayMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

	DEBUG_OUTPUT(("%d | Playing League Matches"), (GetCurrentDate().GetMatchDay()));
	theApp.OnIncrementProgress();	// 3
    if (m_DivisionList.DoPlayLeagueMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

	DEBUG_OUTPUT(("%d | Showing match summary"), (GetCurrentDate().GetMatchDay()));
	theApp.OnIncrementProgress();	// 4
	// Show Match Summary Screen
	bool bShowMatchSummary = false;
    CMatchInfo MatchDetails;
	if (_ViewEm == false && (_ShowSummary == true))
	{
		if (GetCurrentUserManager()->IsMatchToday(m_CurrentDate, MatchDetails) == true)
		{
			if (IsFastTest() == false)
			{
				bShowMatchSummary = true;
				m_bProcessingHalt = true;
			}
		}
	}

	DEBUG_OUTPUT(("%d | Cup Draws"), (GetCurrentDate().GetMatchDay()));
	m_pCupList->DoDraws();

	DEBUG_OUTPUT(("%d | PlayOff Draws"), (GetCurrentDate().GetMatchDay()));
	m_pPlayOffList->DoDraws();

	DEBUG_OUTPUT(("%d | Charity Shield Draws"), (GetCurrentDate().GetMatchDay()));
	m_pCharityShieldList->DoDraws();

#ifdef ACTIVEFEDERATIONCUPS
	DEBUG_OUTPUT(("%d | Federation Cup Draws"), (GetCurrentDate().GetMatchDay()));
    m_FederationCupList.DoDraws();
#endif

	SetMasterState(IN_MENUS);
	if (IsFastTest() == false && GetCurrentUser()->IsManagingAClub() == true && GetCurrentUserDivision()->GetTeamOfTheWeek().IsAvailable() == true)
	{
		theApp.OnShowTeamOfTheWeek();
	}
    m_CurrentDate.NextMatchDay();   // Update the date

	theApp.OnIncrementProgress();	// 5
	DEBUG_OUTPUT(("%d | Division Daily Tasks"), (GetCurrentDate().GetMatchDay()))
	m_DivisionList.DoDailyTasks();

	DEBUG_OUTPUT(("%d | Club Daily Tasks"), (GetCurrentDate().GetMatchDay()))
    m_ClubList.DoDailyTasks();

	theApp.OnIncrementProgress();

    if (m_CurrentDate.FirstDateOfMonth() == true)
	{
		DEBUG_OUTPUT(("%d | User Monthly Tasks"), (GetCurrentDate().GetMatchDay()))
        m_pUserList->DoMonthlyTasks();
		m_DivisionList.DoBuildManagerOfMonth();
	}
	theApp.OnIncrementProgress();

    if (m_CurrentDate.IsWeekEnd() == true)
	{
		DEBUG_OUTPUT(("%d | Country End Of Week"), (GetCurrentDate().GetMatchDay()))
        m_CountryList.DoEndOfWeek(true);

		theApp.OnIncrementProgress();
        m_pUserList->DoEndOfWeek();

		theApp.OnIncrementProgress();
		DEBUG_OUTPUT(("%d | Manager End Of Week"), (GetCurrentDate().GetMatchDay()));
		m_ManagerList.DoEndOfWeekTasks();
		m_OutOfContractPlayerList.DoEndOfWeekTasks();
    }
    else
	{
		DEBUG_OUTPUT(("%d | Country Midweek"), (GetCurrentDate().GetMatchDay()));
        m_CountryList.DoMidWeekTasks(GetCurrentDate().GetMatchDay());
		theApp.OnIncrementProgress();
    }

#ifdef ACTIVEFEDERATIONCUPS
	DEBUG_OUTPUT(("%d | Federation Cups End Of Season"), (GetCurrentDate().GetMatchDay()));
    m_FederationCupList.DoEndOfSeason();
#endif

	DEBUG_OUTPUT(("%d | End Of Loop Stuff"), (GetCurrentDate().GetMatchDay()));
	// Show Match Summary Screen
	if (IsFastTest() == true)
	{
		if ((_ViewEm == false) && (_ShowSummary == true))
		{
			if (bShowMatchSummary == true)
			{
				MatchDetails.GetFixture()->DoShowSummary(MatchDetails.GetCup());
			}
		}
	}

	// Allow unemployed users to select new jobs
    if (m_pUserList->DoCheckIfGotNoClubs() == true)
	{
		// Dont progress if a user has got a new club
		m_bProcessingHalt = true;
	}

	// Stop progress if match next turn
	if (IsFastTest() == false && GetCurrentUserManager()->IsMatchToday(m_CurrentDate, MatchDetails) == true)
	{
		m_bProcessingHalt = true;
	}

	if (m_CurrentDate.GetSeason() == 30)
	{
		// Game over
//@@		UserMessageBox(IDS_GAMECOMPLETE, OKBOX);
		m_pUserList->DoMakeAllInActive();
		m_bProcessingHalt = true;
	}
	DEBUG_OUTPUT(("%s | Processing Completed"), (GetCurrentDate().ShortDateStr()));
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoAutoSave
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoAutoSave()
{
	CString SaveGame;
	SaveGame.LoadString(IDS_AUTOSAVEGAMEFILE);
	SaveGameFile(theApp.GetSaveDirectory() + SaveGame, false);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::SaveGameFile
	Access:    	public 
	Parameter: 	CString & _Str
	Parameter: 	const bool _AskOverWrite
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::SaveGameFile(CString& _Str, const bool _AskOverWrite /*= true*/)
{
	CDataFile DataFile;
	DataFile.OpenForWrite(_Str, _AskOverWrite);

	CSaveGameHeader GameHeader;
	GameHeader.SetDetails(GetCurrentUser()->GetClub().GetHomeKitID(), GetCurrentUser()->GetClubName(), m_CurrentDate);
	GameHeader.Write(&DataFile);

	CFileID fileID;
	m_TextList.Write(&DataFile);
	m_strForenames.Write(&DataFile);
	m_strSurnames.WriteNew(&DataFile);
    fileID.Write(&DataFile);             // Identifier for file

    m_FederationList.Write(&DataFile);
    m_CountryList.Write(&DataFile);
#ifdef ACTIVEFEDERATIONCUPS
    fileID.Write(&DataFile);             // Identifier for file
    m_FederationCupList.Write(&DataFile);
#endif

    fileID.Write(&DataFile);             // Identifier for file
    m_PlayerList.Write(&DataFile);

    fileID.Write(&DataFile);             // Identifier for file
    m_ManagerList.Write(&DataFile);

    fileID.Write(&DataFile);             // Identifier for file
    m_ClubList.Write(&DataFile);
    fileID.Write(&DataFile);             // Identifier for file
    m_pUserList->Write(&DataFile);
    m_InjuryList.Write(&DataFile);
	m_ContractClauseList.Write(&DataFile);
    fileID.Write(&DataFile);             // Identifier for file
    m_pPlayOffList->Write(&DataFile);
    fileID.Write(&DataFile);             // Identifier for file
    m_pCharityShieldList->Write(&DataFile);
    fileID.Write(&DataFile);             // Identifier for file
	m_pCupList->Write(&DataFile);
	fileID.Write(&DataFile);             // Identifier for file
    m_DivisionList.Write(&DataFile);
	fileID.Write(&DataFile);             // Identifier for file
	m_StadiumList.Write(&DataFile);
	fileID.Write(&DataFile);             // Identifier for file
    m_OutOfContractPlayerList.Write(&DataFile);
#ifdef	ACTIVEREFEREES
	m_pRefereeList->Write(&DataFile);
#endif

    DataFile.Close();
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetGameFromDisk
	Access:    	public 
	Parameter: 	CString & _FileName
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::GetGameFromDisk(CString& _FileName)
{
	CDataFile DataFile;
	if (DataFile.OpenForRead(_FileName) == false) return false;

	CSaveGameHeader GameHeader;
	GameHeader.Read(&DataFile);
	m_CurrentDate = GameHeader.GetCalendar();

	CFileID fileID;	// Check is a saved game file
	m_TextList.Read(&DataFile);
	m_strForenames.Read(&DataFile);
	m_strSurnames.ReadNew(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;

    m_FederationList.Read(&DataFile);
    m_CountryList.Read(&DataFile);
#ifdef ACTIVEFEDERATIONCUPS
    if (fileID.CheckID(&DataFile) != true) return false;
    m_FederationCupList.Read(&DataFile);
#endif
    if (fileID.CheckID(&DataFile) != true) return false;
    m_PlayerList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_ManagerList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_ClubList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_pUserList->Read(&DataFile);
    m_InjuryList.Read(&DataFile);
	m_ContractClauseList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_pPlayOffList->Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_pCharityShieldList->Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
	m_pCupList->Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_DivisionList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
	m_StadiumList.Read(&DataFile);
    if (fileID.CheckID(&DataFile) != true) return false;
    m_OutOfContractPlayerList.Read(&DataFile);
#ifdef	ACTIVEREFEREES
	m_pRefereeList->Read(&DataFile);
#endif

	DataFile.Close();
	SetCurrentUserID(m_pUserList->FindFirstActive());
    return true;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetMatchPlay
	Access:    	public 
	Returns:   	CMatchPlay&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchPlay& CWorld::GetMatchPlay()
{
	return m_MatchHandler.GetMatchPlay();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetMatchHandler
	Access:    	public 
	Returns:   	CMatchHandler&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchHandler& CWorld::GetMatchHandler()
{
	return m_MatchHandler;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetMatchSubs
	Access:    	public 
	Returns:   	CMatchSubs&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchSubs& CWorld::GetMatchSubs()
{
	return m_MatchHandler.GetMatchSubs();
}


#ifdef	ACTIVEREFEREES
/*------------------------------------------------------------------------------
Method:   	CWorld::RefereeList
Access:    	public 
Parameter: 	void
Returns:   	CSoccerRefereeList*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CSoccerRefereeList* CWorld::RefereeList()
{
	return m_pRefereeList;
}


/*------------------------------------------------------------------------------
Method:   	CWorld::GetRefereeID
Access:    	public 
Parameter: 	int _ID
Returns:   	CSoccerReferee*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CSoccerReferee* CWorld::GetRefereeID(int _ID)
{
	EXT_ASSERT(_ID < m_pRefereeList->GetSize());
	return m_pRefereeList->GetAt(_ID);
}
#endif


#ifdef ACTIVEFEDERATIONS
/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFederationList
	Access:    	public 
	Parameter: 	void
	Returns:   	CFederationList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationList& CWorld::GetFederationList()
{
    return m_FederationList;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetCurrentUserFederation
	Access:    	public 
	Returns:   	CFederation*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation* CWorld::GetCurrentUserFederation()
{
	EXT_ASSERT_POINTER(GetCurrentUser(), CUser);
	if (GetCurrentUser()->IsManagingAClub() == true)
	{
		return &GetCurrentUser()->GetClub().GetFederation();
	}
	else
	{
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFederation
	Access:    	public 
	Parameter: 	byte _FedID
	Returns:   	CFederation&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CWorld::GetFederation(const ushort _FedID)
{
	return m_FederationList.GetAt(_FedID);
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFederation
	Access:    	public 
	Parameter: 	CString _FedName
	Returns:   	CFederation&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CWorld::GetFederation(const CString& _FedName)
{
	return m_FederationList.Find(_FedName);
}
#endif


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
Method:   	CWorld::GetFederationCupList
Access:    	public 
Parameter: 	void
Returns:   	CFederationCupList*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CFederationCupList& CWorld::GetFederationCupList()
{
	return m_FederationCupList;
}


/*------------------------------------------------------------------------------
Method:   	CWorld::FederationCupListSize
Access:    	public 
Parameter: 	void
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int CWorld::FederationCupListSize()
{
	return m_FederationCupList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::GetFederationCup
	Access:    	public 
	Parameter: 	ushort _CupID
	Returns:   	CFederationCup&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCup& CWorld::GetFederationCup(ushort _CupID)
{
	EXT_ASSERT(_CupID < m_FederationCupList.GetSize());
	return *m_FederationCupList.GetAt(_CupID);
}
#endif


#ifdef ACTIVEINTERFEDERATIONCUPS
/*------------------------------------------------------------------------------
	Method:   	CWorld::GetInterFederationCupList
	Access:    	public 
	Parameter: 	void
	Returns:   	CInterFederationCupList*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInterFederationCupList* CWorld::GetInterFederationCupList()
{
	EXT_ASSERT(false);
	return null;	// InterFederationCups
}


//---------------------------------------------------------------------------------------
int CWorld::InterFederationCupListSize()
{
	EXT_ASSERT(false);
	return 0;	//InterFederationCups->GetSize();
}


//---------------------------------------------------------------------------------------
CInterFederationCup* CWorld::GetInterFederationCup(int x)
{
	EXT_ASSERT(false);
	return null;
}
#endif




