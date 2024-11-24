// Football Director.h : main header file for the Football Director application
//

#if !defined(AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
#define AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <SoccerDefines.h>

/////////////////////////////////////////////////////////////////////////////
// CFootballDirectorApp:
// See FootballDirector.cpp for the implementation of this class
//

class CBasicDisplay;
class CView;

  enum
  {
    HIGHLIGHT_NEWS = 1,
    HIGHLIGHT_SQUAD,
    HIGHLIGHT_FIXTURES,
    HIGHLIGHT_CUP_FIXTURES,
  };


class CFootballDirectorApp // TODO : public CWinApp
{
public:
	CFootballDirectorApp(void);

// Overrides
	virtual BOOL			InitInstance(void);

// Implementation
	void					InitDebugMessages(void);

	CWorld&					GetWorldData(void);
	CString					LongDateStr(void);
	void					UpdateTitleBar(void);
	void					DoPlayerContractNegotiations(CClub* _Club, CTransferNegotiation* _Negotiations);
	bool					HasOwnerName(const bool bDisplayWarning = false);
	CView*					SwitchToView(eView nView);
	int						WhichCountry(void);
	void					SetWhichCountry(const int x);
	void					OnHelp(void);
	void					OnExitApp(void);
	void					OnExitGame(void);
	void					OnOptions(void);
	void					OnRegister(void);
	void					OnSaveGame(void);
	CSystemPreferences&		GetSystemPreferences(void) 						{ return m_SystemPrefs; }
	CApplicationPreferences& GetApplicationPreferences(void) 				{ return m_ApplicationPrefs; }
	bool					IsRegistered(const bool bDisplayWarning = false);
  char*         szAddPath(char* File);
	CString					AddGfxPath(CString& _szFile);
	const bool				IsInitialised(void);
	CBasicDisplay&			GetBasicDisplay();

	// Player Actions
	void					OnViewOwnedPlayerInformation(void);
	void					OnViewNotOwnedPlayerInformation(void);
	void					OnNegotiatePlayerContract(void);

	// Youth Actions
	void					OnViewOwnedYouthInformation(void);
	void					OnViewNotOwnedYouthInformation(void);
	bool					OnSackYouth(void);
	void					OnPromoteYouth(void);
	void					OnHireYouth(void);

	// Employee Actions
	bool					OnSackEmployee(void);
	void					OnNegotiateEmployeeContract(void);
	void					OnHireEmployee(void);

	// Club Actions
	void					OnViewSquadDetails(CClub* _Club);
  void OnViewSquadDetails(void);

	// Fixture Actions
	void					OnViewMatchResultInformation(void);

	// Dialogs
	void					OnSelectUserClubToManage(const ClubID _LastClubID);					// Screen04
	void					OnPlayerSearch();													// Screen06
  void          OnReceiveOfferForPlayer(CClubTransferOffer* _TheOffer);       // Screen12
	void					OnPromoteYouth(const CString &_strPrompt);							// Screen13
	void					OnEmployeeContractRenewalNegotiations(CEmployee* _Employee, CString _Prompt = TEXT("")); // Screen14
	void					OnChangeTransferStatus();											// Screen16
  void          OnShowMatchSummary(CCup* _Cup, CFixture* _Fixture, bool bBackButtonToClose=false);         // Screen17
	void					OnPlayerRequestTransfer(CPlayer* _Player);							// Screen21
	void					OnNegotiatePlayerContract(const CString &_strPrompt);				// Screen23
	void					OnNegotiateOutOfContractPlayerContract(void);						// Screen23
	void					OnApproachClubLoanInPlayer(void);									// Screen24
  void          OnMakeOfferForPlayer(void);                   // Screen30
	bool					DisplayMatchIntro(const bool _bShowSummary);						// Screen32
	bool					OnSackPlayer(void);													// Screen35
  void          OnApproachClubLoanOutPlayer(CClubTransferOffer* _TheOffer);     // Screen44
	void					OnManagerRenewContract(CClub* _Club);								// Screen47
	void					OnUserManagerLeaveClubDlg(const CString &_str);						// Screen47
	void					OnArrangeFriendlies(void);											// Screen54
	void					OnDisplayMatch(CMatchPlay* _MatchPlay);								// Screen65
	void					OnShowTeamOfTheWeek();												// Screen92

  void          OnHighlightMenuIcon( int index, int subIndex = -1 );

  //void OnKnockedOutCup( CString cupName );
  void OnCupFinalResult( CString &cupName, bool bWin );
  void OnEndOfSeason( CString &divName, bool bChampions, bool bPromoted, bool bRelegated, float normalisedPosition );
  void OnWonManagerOfTheMonth(CString& _DivName);

	void					OnShowSimProgressScreen();

  // Message boxes

protected:
	bool					m_bIsInitialised;
	CBasicDisplay			m_Display;
	CWorld*					m_WorldData;
	int						m_WhichCountry;
	CSystemPreferences		m_SystemPrefs;
	CApplicationPreferences m_ApplicationPrefs;

	void					OnInitialiseGame(void);

//BAROG
//DECLARE_MESSAGE_MAP()
//BAROG

//#define TEST_DIALOGS
#ifdef TEST_DIALOGS
	void DebugDialogs();
#endif
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
