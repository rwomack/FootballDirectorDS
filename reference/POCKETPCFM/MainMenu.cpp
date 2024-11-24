
/*

	MainMenu.cpp: implementation of the main screen pulldown menu.

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/



#include "stdafx.h"

#include "UI.h"
#include "MainFrm.h"
#include "Dialog.h"
#include "Form.h"
#include "MessageDisplay.h"
#include "IndexedIntArray.h"
#include "storagelistbox.h"
#include "PlayerNameTextRegion.h"
#include "ClubFixtureGrid.h"
#include "aw_registration_includes.h"

#include "SoccerResultGrid.h"
#include "LeadingGoalScorer.h"
#include "LeadingDisciplinaryPoints.h"
#include "Competition.h"
#include "CupDisplayEvent.h"
#include "MatchInfo.h"
#include "TransferNegotiation.h"
#include "EmployeeGrid.h"
#include "YouthGrid.h"
#include "PlayerContractDlg.h"
#include "LoanPeriodSelector.h"
#include "SaveGameHeader.h"
#include "Screen05Base.h"

#include "Screen04.h"
#include "Screen05Dlg.h"
#include "Screen06.h"
#include "Screen12.h"
#include "Screen13.h"
#include "Screen14.h"
#include "Screen16.h"
#include "Screen17.h"
#include "Screen18Dlg.h"
#include "Screen21.h"
#include "Screen23.h"
#include "Screen24.h"
#include "Screen30.h"
#include "Screen32.h"
#include "Screen35.h"
#include "Screen42.h"
#include "Screen44Dlg.h"
#include "Screen47Dlg.h"
#include "Screen54.h"
#include "Screen59.h"
#include "Screen60.h"
#include "Screen61.h"
#include "Screen65Dlg.h"
#include "Screen87View.h"
#include "Screen90.h"
#include "Screen91.h"
#include "Screen92.h"
#include "Screen99.h"

#include "FootballDirector.h"


#ifdef TEST_DIALOGS
void CFootballDirectorApp::DebugDialogs()
{
 	  	WorldData().SetCurrentClub(&WorldData().GetClub(35));
 	  	CExtString str;
// 	  	OnPlayerSearch();												// Screen06
 		CClubTransferOffer TheOffer;
 	   	TheOffer.DoInitialise(WorldData().GetPlayer(22), 40000000, &WorldData().GetClub(35), 10);
//	  	OnReceiveOfferForPlayer(&TheOffer);								// Screen12
//		str.Format(IDS_YOUTHCONTRACTNEARLYEXPIRED, _T("S.Sengh"));
//	   	OnPromoteYouth(str);											// Screen13
//	  	str.Format(IDS_EMPLOYEECONTRACTWILLEXPIRETODAY, _T("M.Dichia"));
//	  	OnEmployeeContractRenewalNegotiations(WorldData().GetCurrentUserClub()->GetEmployee(0), str);
//	  	OnChangeTransferStatus();										// Screen16
//  	OnViewClubFixtures(&WorldData().GetClub(35), MATCHES_ALL);		// Screen18
//	  	OnPlayerRequestTransfer(WorldData().GetPlayer(34));				// Screen21
//	  	OnNegotiatePlayerContract(str);									// Screen23
//	  	OnApproachClubLoanInPlayer();									// Screen24
//	  	OnMakeOfferForPlayer();											// Screen30
//	  	OnSackPlayer();													// Screen35
		TheOffer.SetLoanPeriod(5);
		OnApproachClubLoanOutPlayer(&TheOffer);							// Screen44
// 		OnManagerRenewContract(&WorldData().GetClub(35));				// Screen47RenewDlg
//		str.Format(IDS_SACKUSERMANAGER, _T("Leeds Utd"));
//	  	OnUserManagerLeaveClubDlg(str);									// Screen47SackedDlg
//	  	OnOptions();													// Screen91
// 		OnShowTeamOfTheWeek();											// Screen92
}
#endif

BEGIN_MESSAGE_MAP(CFootballDirectorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)

#ifdef POCKETPC
	ON_COMMAND(ID_MENUHELP, OnHelp)
	ON_COMMAND(ID_MENUQUIT, OnExitGame)
	ON_COMMAND(ID_MENUOPTIONS, OnOptions)
	ON_COMMAND(ID_MENUSAVE, OnSaveGame)
#endif

	// Player Actions
	ON_COMMAND(IDP_INFO, OnViewOwnedPlayerInformation)
	ON_COMMAND(IDP_NOTOWNEDPLAYERINFO, OnViewNotOwnedPlayerInformation)
	ON_COMMAND(IDP_PLAYERSELL, OnChangeTransferStatus)
	ON_COMMAND(IDP_FREE, OnSackPlayer)
	ON_COMMAND(IDP_CONTRACT, OnNegotiatePlayerContract)
	ON_COMMAND(IDP_CONTRACT1, OnNegotiateOutOfContractPlayerContract)
	ON_COMMAND(IDP_APROACHBUYPLAYER, OnMakeOfferForPlayer)
	ON_COMMAND(IDP_APROACHLOANPLAYER, OnApproachClubLoanInPlayer)

	// Youth Actions
	ON_COMMAND(IDP_YOUTHINFO, OnViewOwnedYouthInformation)
	ON_COMMAND(IDP_AVAILABLEYOUTHINFO, OnViewNotOwnedYouthInformation)
	ON_COMMAND(IDP_YOUTHSACK, OnSackYouth)
	ON_COMMAND(IDP_YOUTHPROMOTE, OnPromoteYouth)
	ON_COMMAND(IDP_YOUTHHIRE, OnHireYouth)

	// Employee Actions
	ON_COMMAND(IDP_EMPLOYEEINFO, OnViewOwnedEmployeeInformation)
	ON_COMMAND(IDP_NOTOWNEDEMPLOYEEINFO, OnViewNotOwnedEmployeeInformation)
	ON_COMMAND(IDP_EMPLOYEESACK, OnSackEmployee)
	ON_COMMAND(IDP_EMPLOYEECONTRACT, OnNegotiateEmployeeContract)
	ON_COMMAND(IDP_EMPLOYEEHIRE, OnHireEmployee)

	// Club Actions
	ON_COMMAND(IDP_CLUBINFO, OnViewSquadDetails)

	// Club Fixture Actions
	ON_COMMAND(IDP_VIEWMATCHREPORT, OnViewMatchResultInformation)
	ON_COMMAND(IDP_VIEWSQUAD, OnViewSquadDetails)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewMatchResultInformation
	Access:    public
	Parameter: void
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewMatchResultInformation()
{
	WorldData().OnViewMatchResultInformation();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnSackEmployee
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnSackEmployee()
{
	WorldData().OnSackEmployee();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnHireYouth
	Access:    public
	Returns:   void
	Qualifier:

	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnHireYouth()
{
	WorldData().OnHireYouth();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnPromoteYouth
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnPromoteYouth()
{
	OnPromoteYouth(TEXT(""));
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnSackYouth
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnSackYouth()
{
	WorldData().OnSackYouth();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnMakeOfferForPlayer
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnMakeOfferForPlayer()
{
	EXT_ASSERT_POINTER(WorldData().GetCurrentUser(), CUser);
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
#endif
	CTransferNegotiation* pNegotiation = WorldData().GetCurrentUser()->GetTransferNegotiationList()->DoFindNegotiationsForPlayer(WorldData().GetCurrentPlayer());
	if (pNegotiation == null)
	{
		CScreen30Dlg Screen30Dlg(WorldData().GetCurrentPlayer());
		Screen30Dlg.DoModal();
	}
	else
	{
		CString strText;
		strText.Format(IDS_ALREADYNEGOTIATINGWITHPLAYER, 
			WorldData().GetClub(pNegotiation->GetSellClubID()).GetName(), 
			WorldData().GetPlayer(pNegotiation->GetPlayerID())->GetName());
		AfxMessageBox(strText);
	}
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnNegotiatePlayerContract
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnNegotiatePlayerContract()
{
	OnNegotiatePlayerContract(TEXT(""));
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnSaveGame
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnSaveGame()
{
	CScreen90Save Screen90Dlg;
	Screen90Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnSelectUserClubToManage
	Access:    	public 
	Parameter: 	const ushort _LastClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnSelectUserClubToManage(const ushort _LastClubID /*= wNOTFOUND*/)
{
#ifndef NOTITLEMENU
	CScreen04 Screen04Dlg(_LastClubID);
	Screen04Dlg.DoModal();
#else
	WorldData().OnInitialiseUser(5);
#endif

	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	WorldData().GetCurrentUserClub()->DoHumanManagerWelcome();
	WorldData().SetProcessingHalt(true);
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnViewSquadDetails
	Access:    	public 
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewSquadDetails(CClub* _Club)
{
	WorldData().SetCurrentClub(_Club);
	OnViewSquadDetails();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnViewSquadDetails
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewSquadDetails()
{
	EXT_ASSERT_POINTER(WorldData().GetCurrentClub(), CClub);
	CScreen05Dlg Screen05Dlg;
	Screen05Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnPlayerSearch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnPlayerSearch()
{
	CScreen06 Screen06Dlg;
	Screen06Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnReceiveOfferForPlayer
	Access:    	public 
	Parameter: 	CClubTransferOffer * _TheOffer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnReceiveOfferForPlayer(CClubTransferOffer* _TheOffer)
{
#ifndef TEST_DIALOGS
	EXT_ASSERT(_TheOffer->GetPlayer().GetClub().IsUserControlled() == true);
	if (_TheOffer->GetPlayer().GetContractRenewalStatus() == RETIRING)
	{
		int ab = 0;
	}
#endif
	CScreen12Dlg Screen12Dlg(_TheOffer);
	Screen12Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnPromoteYouth
	Access:    	public 
	Parameter: 	CString _strPrompt
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnPromoteYouth(CString _strPrompt)
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentYouth(WorldData().GetCurrentUserClub()->GetYouth(0));
#endif
	CScreen13Dlg Screen13Dlg(WorldData().GetCurrentYouth(), _strPrompt);
	Screen13Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnHireEmployee
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnHireEmployee()
{
	CNotOwnedScreen14 Screen14Dlg(WorldData().GetCurrentEmployee(), TEXT(""));
	Screen14Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnNegotiateEmployeeContract
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnNegotiateEmployeeContract()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentEmployee(WorldData().GetCurrentUserClub()->GetEmployee(0));
#endif
	if (WorldData().GetCurrentEmployee()->GetContractEndDate().SeasonsRemaining() > 3)
	{
		CString strText;
		strText.Format(IDS_NOTINTERESTEDINCONTRACT, WorldData().GetCurrentEmployee()->GetName());
		AfxMessageBox(strText);
	}
	else
	{
		COwnedScreen14 Screen14Dlg(WorldData().GetCurrentEmployee(), TEXT(""));
		Screen14Dlg.DoModal();
	}
}


/*------------------------------------------------------------------------------
Method:   	CFootballDirectorApp::OnEmployeeContractRenewalNegotiations
Access:    	public 
Parameter: 	CEmployee * _Employee
Parameter: 	CString _strPrompt
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnEmployeeContractRenewalNegotiations(CEmployee* _Employee, const CString& _strPrompt)
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
#endif
	EXT_ASSERT_POINTER(_Employee, CEmployee);
	COwnedScreen14 Screen14Dlg(_Employee, _strPrompt);
	Screen14Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnChangeTransferStatus
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnChangeTransferStatus()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
#endif
	CScreen16Dlg Screen16Dlg(WorldData().GetCurrentPlayer());
	Screen16Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::DoShowMatchSummary
	Access:    	public 
	Parameter: 	CCup * _Cup
	Parameter: 	CFixture * _Fixture
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnShowMatchSummary(CCup* _Cup, CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CScreen17Dlg Screen17Dlg;
	Screen17Dlg.DoModal(_Cup, _Fixture);
	GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnViewClubFixtures
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	eClubResultsType _ResultToShow
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewClubFixtures(CClub* _Club, eClubResultsType _ResultToShow)
{
	WorldData().SetCurrentClub(_Club);
	CScreen18Dlg* pTheDlg = new CScreen18Dlg(_Club, _ResultToShow);
	pTheDlg->DoModal();
	delete pTheDlg;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnScreen21
	Access:    	public
	Parameter: 	CPlayer* pPlayer
	Returns:   	void
	Qualifier:
	Purpose:	Screen 21 Player Transfer Request
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnPlayerRequestTransfer(CPlayer* _Player)
{
	CScreen21Dlg Screen21Dlg(_Player);
	Screen21Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::DoPlayerContractNegotiations
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	CTransferNegotiation * _Negotiations
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::DoPlayerContractNegotiations(CClub* _Club, CTransferNegotiation* _Negotiations)
{
	WorldData().SetCurrentClub(_Club);
	CNotOwnedScreen23 Screen23Dlg(_Negotiations);
	Screen23Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnNegotiateOutOfContractPlayerContract
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnNegotiateOutOfContractPlayerContract()
{
	bool bAccepted = false;
	CContract PlayerContract;
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
#endif
	COutOfContractScreen23 Screen23Dlg(WorldData().GetCurrentPlayer(), WorldData().GetCurrentClub(), &bAccepted, &PlayerContract);
	Screen23Dlg.DoModal();
	if (bAccepted == true)
	{
		WorldData().GetOutOfContractPlayerList().DoSignedForClub(WorldData().GetOutOfContractPlayerList().DoFindListPosition(WorldData().GetCurrentPlayer()->DoFindID()),
			PlayerContract, WorldData().GetCurrentClub());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnNegotiatePlayerContract
	Access:    	public 
	Parameter: 	CString _strPrompt
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnNegotiatePlayerContract(CString _strPrompt)
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
	WorldData().SetCurrentClub(&WorldData().GetCurrentPlayer()->GetClub());
#endif
	if (WorldData().GetCurrentPlayer()->GetContractEndDate().SeasonsRemaining() > 2 && 
		WorldData().GetCurrentClub()->GetFirstTeamSquad().IsPlayerInSquad(WorldData().GetCurrentPlayer()) == true)
	{
		CString strText;
		strText.Format(IDS_NOTINTERESTEDINCONTRACT, WorldData().GetCurrentPlayer()->GetName());
		AfxMessageBox(strText);
	}
	else
	{
		COwnedScreen23 Screen23Dlg(_strPrompt, WorldData().GetCurrentPlayer(), WorldData().GetCurrentClub());
		Screen23Dlg.DoModal();
	}
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnApproachClubLoanInPlayer
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnApproachClubLoanInPlayer()
{
#ifdef TEST_DIALOGS
//	WorldData().SetCurrentPlayer(&WorldData().GetPlayer(0));
#endif
	EXT_ASSERT(WorldData().GetCurrentUserClub()->AreLoanTransfersAllowed() == true);
	CScreen24Dlg Screen24Dlg;
	Screen24Dlg.DoModal(WorldData().GetCurrentPlayer());
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnApproachClubLoanOutPlayer
	Access:    	public 
	Parameter: 	CClubTransferOffer * _TheOffer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnApproachClubLoanOutPlayer(CClubTransferOffer* _TheOffer)
{
	CScreen44Dlg Screen44Dlg(_TheOffer);
	Screen44Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::DisplayMatchIntro
	Access:    	public 
	Parameter: 	const bool _bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFootballDirectorApp::DisplayMatchIntro(const bool _bShowSummary)
{
	if (_bShowSummary == true)
	{
		// Ask user if wishes to 'View Match' or 'Result Only'
		CScreen32Dlg TheDlg;
		if (TheDlg.DoModal() == IDOK)
		{
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnSackPlayer
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnSackPlayer()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(8));
#endif
	CScreen35Dlg Screen35Dlg(WorldData().GetCurrentPlayer());
	Screen35Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:		CFootballDirectorApp::OnAppAbout
	Access:		public
	Returns:	void
	Qualifier:
	Purpose:	Credits Screen
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnAppAbout()
{
	CScreen42 Screen42Dlg;
	Screen42Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnUserManagerLeaveClubDlg
	Access:    	public 
	Parameter: 	CString _str
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnUserManagerLeaveClubDlg(CString _str)
{
	CScreen47SackedDlg Screen47Dlg(_str);
	Screen47Dlg.DoModal();
	GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnManagerRenewContract
	Access:    	public 
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnManagerRenewContract(CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	CScreen47RenewDlg Screen47Dlg(_Club);
	Screen47Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnScreen54
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Screen 54 Friendly fixtures
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnArrangeFriendlies()
{
	CScreen54 Screen54Dlg;
	Screen54Dlg.DoModal();
	GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewOwnedYouthInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewOwnedYouthInformation()
{
	COwnedScreen59 Screen59Dlg;
	Screen59Dlg.DoModal(WorldData().GetCurrentYouth());
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewNotOwnedYouthInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewNotOwnedYouthInformation()
{
	CNotOwnedScreen59 Screen59Dlg;
	Screen59Dlg.DoModal(WorldData().GetCurrentYouth());
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewOwnedEmployeeInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewOwnedEmployeeInformation()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentEmployee(WorldData().GetCurrentUserClub()->GetEmployee(0));
#endif
	COwnedScreen60 Screen60Dlg(WorldData().GetCurrentEmployee());
	Screen60Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewNotOwnedEmployeeInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewNotOwnedEmployeeInformation()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentEmployee(WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableEmployee(0));
#endif
	CNotOwnedScreen60 Screen60Dlg(WorldData().GetCurrentEmployee());
	Screen60Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewOwnedPlayerInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewOwnedPlayerInformation()
{
	COwnedScreen61 Screen61Dlg(WorldData().GetCurrentPlayer());
	Screen61Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:    CFootballDirectorApp::OnViewNotOwnedPlayerInformation
	Access:    public
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnViewNotOwnedPlayerInformation()
{
#ifdef TEST_DIALOGS
	WorldData().SetCurrentPlayer(&WorldData().GetCurrentUserClub()->GetPlayer(0));
#endif
	CNotOwnedScreen61 Screen61Dlg(WorldData().GetCurrentPlayer());
	Screen61Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::DisplayMatch
	Access:    	public
	Parameter: 	CMatchPlay * _MatchPlay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnDisplayMatch(CMatchPlay* _MatchPlay)
{
#ifdef FMOD
	theApp.GetSoundManager().SetMasterVolume(theApp.GetApplicationPreferences().GetMusicInGame().GetVolume());
#endif

	CScreen65Dlg Screen65Dlg(_MatchPlay);
	Screen65Dlg.DoModal();
	theApp.GetMainWnd()->UpdateWindow();

#ifdef FMOD
	theApp.GetSoundManager().SetMasterVolume(theApp.GetApplicationPreferences().GetMusic().GetVolume());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnOptions
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnOptions()
{
	CScreen91Dlg Screen91Dlg;
	Screen91Dlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnShowTeamOfTheWeek
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnShowTeamOfTheWeek()
{
	CScreen92Dlg Screen92Dlg;
	Screen92Dlg.DoModal();
	GetMainWnd()->UpdateWindow();
}
