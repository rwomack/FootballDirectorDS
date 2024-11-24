
 /*

	File:- WorldData.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

//#define LIST_PLAYERSCLUBS

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#endif

#include "stdafx.h"

#include "voimage.h"
#include "DateDisplay.h"
#include "BasicDisplay.h"
#include "BitmapFormView.h"

//#include "UI.h"
//#include "Form.h"
#include "Stadium.h"
#include "CharityShield.h"
#include "Competition.h"
#include "MatchInfo.h"
#include "TransferNegotiation.h"
#include <SortedStringList.h>

#ifdef ORIGINAL_CODE
#include "Screen28View.h"
#endif                                  // #ifdef ORIGINAL_CODE

#include "FootballDirector.h"
#include "SaveGameHeader.h"


	const CString			FileExtension = TEXT("fd");		// File extension for save game files

//#include <crtdbg.h>


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
  strText.Format(lText::Text(IDS_HIREDYOUTH), strYouthName);
  lLogFmt("OnHireYouth message box: %s\n", strText.CStr());
  UserMessageBox( strText );
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
//void CWorld::OnViewPlayerDetails(CPlayer* _pPlayer /*= null*/)
//{
//  if (_pPlayer != null)
//  {
//    SetCurrentPlayer(_pPlayer);
//  }
//  if (GetCurrentPlayer()->IsAtCurrentUserControlledClub() == true)
//  {
//    theApp.OnViewOwnedPlayerInformation();
//  }
//  else
//  {
//    theApp.OnViewNotOwnedPlayerInformation();
//  }
//}


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
    , m_pUserList(FD_ENGINE_NEW CUserList(MAXHUMANMANAGERS))
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
    , m_pRefereeList(FD_ENGINE_NEW CSoccerRefereeList())
 #endif
 #ifdef	ACTIVESTADIUMPLANS
    , m_pStadiumPlanList(FD_ENGINE_NEW CStadiumPlanList())
 #endif
{
	m_CurrentDate.DoInitialise(0, 0, 0, 2008, 7, 1);
 #ifndef USE_CURRENCIES
	// Work in £'s only
	m_Currency.DoInitialise();
 #endif
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoInitialise
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/


bool s_bUseSpecificRandomSeed = false;
unsigned int s_uiSpecificRandomSeed = 42;


void CWorld::DoInitialise()
{
  ProcessingCallback( "DoInitTop" );

  m_TransferDeadlineSystem.DoInitialise();
	m_KitSystem.DoInitialise();

  ProcessingCallback( "PreCountry" );

	m_CountryList.DoInitialise();

  ProcessingCallback( "PreDiv" );

  m_DivisionList.DoInitialise();

  ProcessingCallback( "PreInj" );

  m_InjuryTypeList.DoInitialise();
	m_InjuryEventTypeList.DoInitialise();

  	if (s_bUseSpecificRandomSeed == false)
	{
		RandomNumber.Randomize();		// Set random seed based on the system clock
 	}
	else
  	{
	  	// Set a specific seed.
	  	RandomNumber.SetSeed(s_uiSpecificRandomSeed);
  	}

  ProcessingCallback( "PreSur" );

  CBinaryReader theResource(0, IDR_SURNAME, TEXT("FDDATA"));
	m_strSurnames.DoInitialiseFromResource(theResource, false);

  ProcessingCallback( "PreMan" );

  m_ManagerList.DoInitialise();

  ProcessingCallback( "PreClub" );

  m_ClubList.DoInitialise();

  ProcessingCallback( "PreStad" );

	m_StadiumList.DoInitialise();

  ProcessingCallback( "PrePlayer" );

	m_PlayerList.DoInitialise(m_TotalClubs);

  ProcessingCallback( "PreList" );

  m_pPlayOffList = FD_ENGINE_NEW CPlayOffList();
  m_pCharityShieldList = FD_ENGINE_NEW CCharityShieldList();
  m_pCupList = FD_ENGINE_NEW CCupList();
	m_FederationCupList.DoInitialiseFromResource();

  m_TransferListManager.Reset();
  m_PlayerLoanManager.Reset();

  ProcessingCallback( "PreLoan" );

	// Initialise players who are currently on loan
	theResource.LoadData(0, IDR_PLAYERLOANS, TEXT("FDDATA"));
	short nOrginalListSize = theResource.ReadUInt16();
	for (int ListPos = 0; ListPos < nOrginalListSize; ListPos ++)
	{
		ClubID _ClubID;
		_ClubID.id = theResource.ReadUInt16();
		ushort PlayerID = theResource.ReadUInt16();
		OnLoanPlayerOut(PlayerID, _ClubID, MAXLOANPLAYERWEEKS);
	}

  ProcessingCallback( "PrePost" );

	m_ClubList.PostInitialise();
	m_pCharityShieldList->DoDraws();
	m_CurrentDate.NextMatchDay();
  m_CountryList.PostInitialise();

	// Gillingham and Hereford are in the wrong divisions
	ClubID _Club1ID;
	_Club1ID.id = 50; // Gillingham, relegate them
	ClubID _ClubI2D;
	_ClubI2D.id = 81; // Hereford, promote them
	GetClubList().DoPromoteAndRelegateClubs(_ClubI2D, _Club1ID);

	m_MasterState = IN_INITIALISATION;

  m_CountryList.PlayMissedMatches();
	m_MasterState = IN_MENUS;

	// Rebuild the division club lists.
	for (int i = 0; i < m_DivisionList.GetSize(); i++)
	{
		m_DivisionList.GetAt(i).RemoveAllClubs();
	}
	for (int i = 0; i < m_ClubList.GetSize(); i++)
	{
		CClub &cClubToAdd = m_ClubList[i];
		if (cClubToAdd.GetDivisionID() != NODIVISION)
			m_DivisionList.GetAt(cClubToAdd.GetDivisionID()).AddClub(cClubToAdd.DoFindID());
	}

  m_DivisionList.UpdateTables();

  g_DebugStr.Format(_T("Player %d | %d | %d"), sizeof(CPlayer), m_PlayerList.GetSize(), sizeof(CPlayer) * m_PlayerList.GetSize());
	g_DebugStr.OutputDebug();
	g_DebugStr.Format(_T("Club %d | %d | %d"), sizeof(CClub), m_ClubList.GetSize(), sizeof(CClub) * m_ClubList.GetSize());
	g_DebugStr.OutputDebug();
	g_DebugStr.Format(_T("Fixture %d"), sizeof(CFixture));
	g_DebugStr.OutputDebug();


#ifdef LIST_PLAYERSCLUBS
  {
    lLogFmt("\n%d players\n", m_PlayerList.GetSize() );
    int maxLen = 0;
    int maxLenIdx = -1;
    for(int i = 0; i < m_PlayerList.GetSize(); i++)
    {
      int len = strlen( m_PlayerList[i].GetName().CStr() );
      if( len > maxLen ) { maxLen = len; maxLenIdx = i; }
      lLogFmt(" %d : %s\n", i, m_PlayerList[i].GetName().CStr());
    }
    lLogFmt("\nMax Len: %d (%s)\n", maxLen, m_PlayerList[maxLenIdx].GetName().CStr());
  }

  {
    lLogFmt("\n\n%d clubs\n", m_ClubList.GetSize() );
    int maxLen = 0;
    int maxLenIdx = -1;
    for(int i = 0; i < m_ClubList.GetSize(); i++)
    {
      int len = strlen( m_ClubList[i].GetName().CStr() );
      if( len > maxLen ) { maxLen = len; maxLenIdx = i; }
      lLogFmt(" %d : %s\n", i, m_ClubList[i].GetName().CStr());
    }
    lLogFmt("\nMax Len: %d (%s)\n", maxLen, m_ClubList[maxLenIdx].GetName().CStr());
  }

  {
    lLogFmt("\n\n%d stadiums\n", m_StadiumList.GetSize() );
    int maxLen = 0;
    int maxLenIdx = -1;
    for(int i = 0; i < m_StadiumList.GetSize(); i++)
    {
      int len = strlen( m_StadiumList.GetAt(i)->GetName().CStr() );
      if( len > maxLen ) { maxLen = len; maxLenIdx = i; }
      lLogFmt(" %d : %s\n", i, m_StadiumList.GetAt(i)->GetName().CStr());
    }
    lLogFmt("\nMax Len: %d (%s)\n", maxLen, m_StadiumList.GetAt(maxLenIdx)->GetName().CStr());
  }

#endif


  OnCreateUser();

  ProcessingCallback( "PreCreate" );

/*
  lLogFmt("\n\n");
  lLogFmt("CPerson: %d\n", sizeof(CPerson));
  lLogFmt("CSoccerPlayer: %d\n", sizeof(CSoccerPlayer));
  lLogFmt("CPlayerTransferStatus: %d\n", sizeof(CPlayerTransferStatus));
  lLogFmt("CPlayerLoan: %d\n", sizeof(CPlayerLoan));

  lLogFmt("CPlayerCareer: %d x 8 = %d\n", sizeof(CPlayerCareer), sizeof(CPlayerCareer)*8);

  lLogFmt("CPlayerDiscipline: %d\n", sizeof(CPlayerDiscipline));

  lLogFmt("\n");
*/
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
void CWorld::OnLoanPlayerOut(const ushort _PlayerID, const ClubID _ToClubID, const int _NumberOfWeeks)
{
	GetPlayer(_PlayerID)->GetClub().GetFirstTeamSquad().GotPlayerOnLoan();
	if (_ToClubID.id != NOCLUB)
	{
		// Only use original club if has room in squad for player
		if (GetPlayer(_PlayerID)->GetClub().GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD -  1)
		{
			CClub &cToClub = GetClubByID(_ToClubID);
			cToClub.GetFirstTeamSquad().DoIncrementPlayersOutOnLoan();
			GetPlayer(_PlayerID)->DoGoingOnLoan(&cToClub, _NumberOfWeeks);
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
void CWorld::DoBuildCupNameList(CStringArray &_List)
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

	cUserList.SetAt(_UserID, FD_ENGINE_NEW CUser());
	SetCurrentUserID(_UserID);
	GetCurrentUser()->SetManagerID(GetManagerList().Add(FD_ENGINE_NEW CManager(6000, 5, ClubID())));
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
void CWorld::OnInitialiseUser(const ClubID _ClubID, const ushort _UserID /*= 0*/)
{
	uint Wage;
	CClub &cClub = GetClubByID(_ClubID);
	//GetClub(_ClubID).GetFirstTeamSquad().DoSelectNonUserControlledTeam(5, GetClub(_ClubID));
	if (cClub.GetManagerID() != NOMANAGER)
	{
		Wage = cClub.GetManager().GetWage() * 52;
		cClub.DoSackManager(false);
	}
	else
	{
		Wage = cClub.GetManagerContract().GetWageOffer() * 52;
	}
	SetCurrentUserID(_UserID);
	GetCurrentUser()->DoManageNewClub(cClub, Wage, 4);
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

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

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

CTransferListManager &CWorld::GetTransferListManager()
{
	return m_TransferListManager;
}

CPlayerLoanManager &CWorld::GetPlayerLoanManager()
{
	return m_PlayerLoanManager;
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
CClub& CWorld::GetClubByID(const ClubID _ClubID)
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
  Method:     CWorld::GetCupID
	Access:    	public
  Parameter:  CCup*Cup
  Returns:    ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CWorld::GetCupID(CCup *_Cup)
{
  // search cup list
  for(int i = 0; i < m_pCupList->GetSize(); i++)
  {
    if( m_pCupList->GetAt(i) == _Cup )
    {
      return i;
    }
  }

  // search fed cup list
  for(int i = 0; i < m_FederationCupList.GetSize(); i++)
  {
    if( m_FederationCupList.GetAt(i) == _Cup )
    {
      return m_pCupList->GetSize() + i;
    }
  }

  return -1;
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
		_ID = RandomNumber.IntLessThan(m_StadiumList.GetSize());
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

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoFindNeutralVenueNameForFixture
	Access:    	public
	Parameter: 	CFixture * _Fixture
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CWorld::DoFindNeutralVenueNameForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr = GetCupList().DoFindNeutralVenueNameForFixture(_Fixture);
	if (TitleStr.IsEmpty() == true)
	{
		TitleStr = GetCharityShieldList().DoFindNeutralVenueNameForFixture(_Fixture);
	}
	if (TitleStr.IsEmpty() == true)
	{
		TitleStr = GetFederationCupList().DoFindNeutralVenueNameForFixture(_Fixture);
	}
	return TitleStr;
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoFindCompetitionTitleForFixture
	Access:    	public
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CWorld::DoFindCompetitionTitleForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr = GetCupList().DoFindCompetitionTitleForFixture(_Fixture);
	if (TitleStr.IsEmpty() == true)
	{
		TitleStr = GetCharityShieldList().DoFindCompetitionTitleForFixture(_Fixture);
	}
	if (TitleStr.IsEmpty() == true)
	{
		TitleStr = GetFederationCupList().DoFindCompetitionTitleForFixture(_Fixture);
	}
	return TitleStr;
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
ClubID CWorld::CurrentUserClubID()
{
	if (m_CurrentUserID == NOMANAGER)
	{
		return ClubID();
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
CString& CWorld::GetString(const ClubID _nClubID)
{
	return m_strIDs.GetString(_nClubID.id);
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

CNonDuplicateStringList &CWorld::GetSurnameList()
{
	return m_strSurnames;
}

CNonDuplicateStringList &CWorld::GetForenameList()
{
	return m_strForenames;
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

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

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
		pFixture = FD_BIN_NEW CFixture();
	}
	EXT_ASSERT(pFixture != null);
	return pFixture;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif



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
	m_CurrentUserID = _ID;
	return true;
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


/*------------------------------------------------------------------------------
	Method:   	CWorld::RestoreProgressDuringDailyTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::RestoreProgressDuringDailyTasks()
{
//  theApp.GetMainWnd()->Invalidate();
//  theApp.GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::CurrentUserCantProgress
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CWorld::CurrentUserCantProgress(CString &cInvalidityReason)
{
	cInvalidityReason = "";

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
		if (GetCurrentUserClub()->IsValidUserControlledTeam(MatchDetails.GetSubsSelect(), cInvalidityReason) == false)
			return true;
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
	Method:   	CWorld::DoCupDraws
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoCupDraws()
{
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
}


/*------------------------------------------------------------------------------
	Method:   	CWorld::DoProcessMatches
	Access:    	public
	Parameter: 	CScreen28View * _View
	Parameter: 	const bool _ViewEm
	Parameter: 	const bool _ShowSummary
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CWorld::DoProcessMatches(CScreen28View* _View, const bool _ViewEm, const bool _ShowSummary /*= false*/)
{
  lLogFmt("Process %x\n", _View);

  _View->OnIncrementProgress( 0.0f / 12.0f );   // 1

	// Allow unemployed users to select new jobs
  if (m_pUserList->DoCheckIfGotNoClubs() == true)
	{
		// Dont progress if a user has got a new club
    m_bProcessingHalt = true;
    return;
	}

  //NOTE: No autosave
  //if (m_CurrentDate.FirstDateOfMonth() == true)
  //{
  //  DoAutoSave();
  //}
	m_bProcessingHalt = false;

	SetMasterState(IN_PROCESSING);
  _View->OnIncrementProgress( 1.0f / 12.0f );   // 1
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
  _View->OnIncrementProgress( 2.0f / 12.0f );   // 1
    // If not midweek match
    if (m_CurrentDate.IsWeekEnd() != true)
	{
        m_ClubList.DoWeeklyClearDown();  // Clear all last weeks data out
    }
  _View->OnIncrementProgress( 3.0f / 12.0f );   // 1
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

  _View->OnIncrementProgress( 3.5f / 12.0f );   // 1
	DEBUG_OUTPUT(("%d | Playing Charity Shield Matches"), (GetCurrentDate().GetMatchDay()));
    if (m_pCharityShieldList->DoPlayMatches(_ShowSummary) == true)
	{
	}

  _View->OnIncrementProgress( 3.75f / 12.0f );   // 1
	DEBUG_OUTPUT(("%d | Playing Playoffs Matches"), (GetCurrentDate().GetMatchDay()));
    if (m_pPlayOffList->DoPlayMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

	DEBUG_OUTPUT(("%d | Playing Cup Matches"), (GetCurrentDate().GetMatchDay()));
  _View->OnIncrementProgress( 4.0f / 12.0f );   // 1
	if (m_pCupList->DoPlayMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

	DEBUG_OUTPUT(("%d | Playing League Matches"), (GetCurrentDate().GetMatchDay()));
  _View->OnIncrementProgress( 5.0f / 12.0f );   // 1
    if (m_DivisionList.DoPlayLeagueMatches(_ShowSummary) == true)
	{
		if (IsFastTest() == false)
		{
			m_bProcessingHalt = true;
		}
	}

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 1\n");
    m_bProcessingHalt = true;
    return;
  }


	DEBUG_OUTPUT(("%d | Showing match summary"), (GetCurrentDate().GetMatchDay()));
  	_View->OnIncrementProgress( 6.0f / 12.0f );   // 1
	// Show Match Summary Screen
	bool bShowMatchSummary = false;
    CMatchInfo MatchDetails;
	if ((_ViewEm == false) && (_ShowSummary == true))
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

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 2\n");
    m_bProcessingHalt = true;
    return;
  }

    _View->OnIncrementProgress( 6.5f / 12.0f );   // 1

	DoCupDraws();
	SetMasterState(IN_MENUS);
	if (IsFastTest() == false && GetCurrentUser()->IsManagingAClub() == true && GetCurrentUserDivision()->GetTeamOfTheWeek().IsAvailable() == true)
	{
		theApp.OnShowTeamOfTheWeek();
	}
  _View->OnIncrementProgress( 7.0f / 12.0f );
  if (m_CurrentDate.NextMatchDay() == true && m_CurrentDate.GetMatchDay() == 1)   // Update the date
	{
		// New season required to skip week 0
		m_CurrentDate.SetMatchDayDirect(0);
		DoCupDraws();
		GetCurrentDate().SetMatchDayDirect(1);
	}

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 3\n");
    m_bProcessingHalt = true;
    return;
  }

   _View->OnIncrementProgress( 7.25f / 12.0f );
	DEBUG_OUTPUT(("%d | Division Daily Tasks"), (GetCurrentDate().GetMatchDay()))
	m_DivisionList.DoDailyTasks();

  _View->OnIncrementProgress( 7.5f / 12.0f );

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 4\n");
    m_bProcessingHalt = true;
    return;
  }

	DEBUG_OUTPUT(("%d | Club Daily Tasks"), (GetCurrentDate().GetMatchDay()))
    m_ClubList.DoDailyTasks();

	 _View->OnIncrementProgress( 8.0f / 12.0f );   // 1

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 5\n");
    m_bProcessingHalt = true;
    return;
  }


    if (m_CurrentDate.FirstDateOfMonth() == true)
	{
		DEBUG_OUTPUT(("%d | User Monthly Tasks"), (GetCurrentDate().GetMatchDay()))
        m_pUserList->DoMonthlyTasks();
		m_DivisionList.DoBuildManagerOfMonth();
	}
  _View->OnIncrementProgress( 9.0f / 12.0f );   // 1


    if (m_CurrentDate.IsWeekEnd() == true)
	{
		DEBUG_OUTPUT(("%d | Country End Of Week"), (GetCurrentDate().GetMatchDay()))

    // update loan players
    GetPlayerLoanManager().DoWeeklyUpdate();

    //
    m_CountryList.DoEndOfWeek(true);
    _View->OnIncrementProgress( 10.0f / 12.0f );   // 1

        m_pUserList->DoEndOfWeek();

    _View->OnIncrementProgress( 11.0f / 12.0f );   // 1
		DEBUG_OUTPUT(("%d | Manager End Of Week"), (GetCurrentDate().GetMatchDay()));
		m_ManagerList.DoEndOfWeekTasks();
		m_OutOfContractPlayerList.DoEndOfWeekTasks();
    }
    else
	{
		DEBUG_OUTPUT(("%d | Country Midweek"), (GetCurrentDate().GetMatchDay()));
        m_CountryList.DoMidWeekTasks(GetCurrentDate().GetMatchDay());
    _View->OnIncrementProgress( 11.5f / 12.0f );   // 1
    }

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 6\n");
    m_bProcessingHalt = true;
    return;
  }

#ifdef ACTIVEFEDERATIONCUPS
	DEBUG_OUTPUT(("%d | Federation Cups End Of Season"), (GetCurrentDate().GetMatchDay()));
    m_FederationCupList.DoEndOfSeason();
#endif

	DEBUG_OUTPUT(("%d | End Of Loop Stuff"), (GetCurrentDate().GetMatchDay()));

  if (m_pUserList->DoCheckIfGotNoClubs() == true)
  {

lLogFmt("** SACKED USER 7\n");
    m_bProcessingHalt = true;
    return;
  }

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

  _View->OnIncrementProgress( 11.75f / 12.0f );   // 1

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

 //static int countdown = 5;
 //countdown--;
 //lLogFmt("EndGame %d\n", countdown);

  if (m_CurrentDate.GetSeason() == 15)
	{
		// Game over
    lLogFmt("Endgame\n");
    UserMessageBox("Congratulations on making it to your 15th season in management!\n"
                   "The time has come for you to retire from football\n"
                   "You will now be returned to the main menu to create a new game", MB_ENDOFGAME);
    //m_pUserList->DoMakeAllInActive();
		m_bProcessingHalt = true;
	}

	// Rebuild the division club lists.
	for (int i = 0; i < m_DivisionList.GetSize(); i++)
	{
		m_DivisionList.GetAt(i).RemoveAllClubs();
	}

	for (int i = 0; i < m_ClubList.GetSize(); i++)
	{
		CClub &cClubToAdd = m_ClubList[i];
		if (cClubToAdd.GetDivisionID() != NODIVISION)
			m_DivisionList.GetAt(cClubToAdd.GetDivisionID()).AddClub(cClubToAdd.DoFindID());
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
	//
	//CString SaveGame;
  //SaveGame.LoadString(IDS_AUTOSAVEGAMEFILE);
	//SaveGameFile(theApp.GetSystemPreferences().GetSaveDirectory() + SaveGame, false);
	//TODO: Autosave
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
bool CWorld::SaveGameFile(CDataFile *File)
{
  if(File == NULL)
	{
 //   UserMessageBox(IDS_FILENOTSAVED, MB_OK + MB_ICONEXCLAMATION);
    return false;
  }

  File->Debug("SaveTop");

	CSaveGameHeader GameHeader;
	GameHeader.SetDetails(GetCurrentUser()->GetClub().GetHomeKitID(), GetCurrentUser()->GetClubName(), m_CurrentDate);
	GameHeader.Write(File);
  File->Debug("AfterHeader");

  m_TextList.Write(File);
  File->Debug("TextList");
  m_strForenames.Write(File);
  File->Debug("Forenames");
  m_strSurnames.WriteNew(File);
  File->Debug("Surnames");
	CFileID fileID;
  fileID.Write(File);             // Identifier for file

  m_FederationList.Write(File);
  File->Debug("FederationList");
  m_CountryList.Write(File);
  File->Debug("CountryList");
#ifdef ACTIVEFEDERATIONCUPS
  fileID.Write(File);             // Identifier for file
  m_FederationCupList.Write(File);
  File->Debug("FedCupList");
#endif

  fileID.Write(File);             // Identifier for file
  m_PlayerList.Write(File);
  File->Debug("PlayerList");

  fileID.Write(File);             // Identifier for file
  m_ManagerList.Write(File);
  File->Debug("ManagerList");

  fileID.Write(File);             // Identifier for file

  m_ClubList.Write(File);
  fileID.Write(File);             // Identifier for file
  File->Debug("ClubList");

  m_pUserList->Write(File);
  File->Debug("UserList");

  m_InjuryList.Write(File);
  File->Debug("InjList");

  m_ContractClauseList.Write(File);
  fileID.Write(File);             // Identifier for file
  m_pPlayOffList->Write(File);
  fileID.Write(File);             // Identifier for file
  m_pCharityShieldList->Write(File);
  fileID.Write(File);             // Identifier for file

  File->Debug("Misc");

  m_pCupList->Write(File);
  fileID.Write(File);             // Identifier for file
  File->Debug("CupList");

  m_DivisionList.Write(File);
  fileID.Write(File);             // Identifier for file
  File->Debug("DivList");

  //m_StadiumList.Write(File);
  //fileID.Write(File);             // Identifier for file
  //File->Debug("StadiumList");

  m_TransferListManager.Write(File);
  m_PlayerLoanManager.Write(File);
  m_OutOfContractPlayerList.Write(File);
#ifdef	ACTIVEREFEREES
  m_pRefereeList->Write(File);
#endif

  File->Debug("End");

//    DataFile.Close();
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
bool CWorld::GetGameFromDisk(CDataFile *File)
{
  if (File == NULL )
	{
    //UserMessageBox(IDS_FILENOTFOUND, MB_OK + MB_ICONEXCLAMATION);
    return false;
  }

  ProcessingCallback( "GetGameTop" );

	CSaveGameHeader GameHeader;
	//GameHeader.SetDetails(GetCurrentUser()->GetClub().GetHomeKitID(), GetCurrentUser()->GetClubName(), m_CurrentDate);
  GameHeader.Read(File);
	m_CurrentDate = GameHeader.GetCalendar();

  ProcessingCallback( "GetGameBeforeNames" );

  m_TextList.Read(File);
  m_strForenames.Read(File);
  m_strSurnames.ReadNew(File);
	CFileID fileID;	// Check is a saved game file
  if (fileID.CheckID(File) != true) return false;

  ProcessingCallback( "GetGameBeforeCountry" );

  m_FederationList.Read(File);
  m_CountryList.Read(File);
#ifdef ACTIVEFEDERATIONCUPS
  if (fileID.CheckID(File) != true) return false;
  m_FederationCupList.Read(File);
#endif

  ProcessingCallback( "GetGameBeforePlayer" );

  if (fileID.CheckID(File) != true) return false;
  m_PlayerList.Read(File);
  if (fileID.CheckID(File) != true) return false;
  m_ManagerList.Read(File);
  if (fileID.CheckID(File) != true) return false;

  ProcessingCallback( "GetGameBeforeClub" );

  File->Debug("ClubList");
  m_ClubList.Read(File);
  File->Debug("ClubList End");
  if (fileID.CheckID(File) != true) return false;

  ProcessingCallback( "GetGameBeforeUser" );

  File->Debug("UserList");
  m_pUserList->Read(File);

  File->Debug("InjList");
  m_InjuryList.Read(File);
  m_ContractClauseList.Read(File);
  if (fileID.CheckID(File) != true) return false;
  m_pPlayOffList->Read(File);
  if (fileID.CheckID(File) != true) return false;
  m_pCharityShieldList->Read(File);
  if (fileID.CheckID(File) != true) return false;

  ProcessingCallback( "GetGameBeforeCup" );

  m_pCupList->Read(File);
  if (fileID.CheckID(File) != true) return false;

  ProcessingCallback( "GetGameBeforeDiv" );

  m_DivisionList.Read(File);
  if (fileID.CheckID(File) != true) return false;
  //m_StadiumList.Read(File);
  //if (fileID.CheckID(File) != true) return false;
  m_TransferListManager.Read(File);
  m_PlayerLoanManager.Read(File);
  m_OutOfContractPlayerList.Read(File);
#ifdef	ACTIVEREFEREES
  m_pRefereeList->Read(File);
#endif

  ProcessingCallback( "GetGamePost" );

//  DataFile.Close();
	SetCurrentUserID(m_pUserList->FindFirstActive());

	// Rebuild the division club lists.
	for (int i = 0; i < m_DivisionList.GetSize(); i++)
	{
		m_DivisionList.GetAt(i).RemoveAllClubs();
	}
	for (int i = 0; i < m_ClubList.GetSize(); i++)
	{
		CClub &cClubToAdd = m_ClubList[i];
		if (cClubToAdd.GetDivisionID() != NODIVISION)
			m_DivisionList.GetAt(cClubToAdd.GetDivisionID()).AddClub(cClubToAdd.DoFindID());
	}

	// Post-process regenerate non-league unsaved players.
	for (int i = 0; i < m_PlayerList.GetSize(); i++)
	{
		if (m_PlayerList[i].IsDontSave() == true)
		{
			m_PlayerList[i].DoGenerateNonLeaguePlayer(*WorldData().GetCurrentUserCountry());
		}
	}

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
CFederation& CWorld::GetFederation(byte _FedID)
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



void CScreen28View::OnIncrementProgress( float pct )
{
  lLogFmt("Screen28View:: Progress\n");
}

