// Football Director.h : main header file for the Football Director (Windows Mobile) application
//

#if !defined(AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
#define AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_



/////////////////////////////////////////////////////////////////////////////
// CFootballDirectorApp:
// See FootballDirector.cpp for the implementation of this class
//

class CBasicDisplay;

#include "aw_registration_includes.h"

class CRegistrationUI;
class CSystemPreferences;
class CApplicationPreferences;
class CScreen03View;


class CFootballDirectorApp : public CWinApp
{
public:
	CFootballDirectorApp(void);

// Overrides
	virtual BOOL			InitInstance(void);

// Implementation
	void					InitDebugMessages(void);
	void					DoShowClubFixtures(const ushort _ClubID, const eClubResultsType _ResultsType);
	CScreen03View*			GetActiveView();
	CWorld&					GetWorldData(void);
	CCalendar&				GetDate(void);
	void					UpdateTitleBar(void);
	void					DoPlayerContractNegotiations(CClub* _Club, CTransferNegotiation* _Negotiations);
	bool					HasOwnerName(const bool bDisplayWarning = false);
	int						WhichCountry(void);				
	void					SetWhichCountry(const int x);			
	void					OnHelp(void);
	void					OnExitApp(void);
	void					OnExitGame(void);
	void					OnOptions(void);
	void					OnSaveGame(void);
	CSystemPreferences&		GetSystemPreferences(void);
	CApplicationPreferences& GetApplicationPreferences(void);
	bool					IsRegistered(const bool bDisplayWarning = false);
	char*					szAddPath(char* _pFile);
	const bool				IsInitialised(void);
	CBasicDisplay&			GetBasicDisplay();
	CRegistrationUI*		GetRegSystem();
	void					OnAppAbout(void);
	void					OnIncrementProgress();
	CString					GetSaveDirectory();

	// Player Actions
	void					OnViewOwnedPlayerInformation(void);
	void					OnViewNotOwnedPlayerInformation(void);
	void					OnNegotiatePlayerContract(void);

	// Youth Actions
	void					OnViewOwnedYouthInformation(void);
	void					OnViewNotOwnedYouthInformation(void);
	void					OnSackYouth(void);
	void					OnPromoteYouth(void);
	void					OnHireYouth(void);

	// Employee Actions
	void					OnSackEmployee(void);
	void					OnNegotiateEmployeeContract(void);
	void					OnHireEmployee(void);

	// Club Actions
	void					OnViewSquadDetails(CClub* _Club);
	void					OnViewSquadDetails(void);

	// Fixture Actions
	void					OnViewMatchResultInformation(void);

	// Dialogs
	void					OnSelectUserClubToManage(const ushort _LastClubID = wNOTFOUND);		// Screen04
	void					OnPlayerSearch();													// Screen06
	void					OnReceiveOfferForPlayer(CClubTransferOffer* _TheOffer);				// Screen12
	void					OnPromoteYouth(CString _strPrompt);									// Screen13
	void					OnEmployeeContractRenewalNegotiations(CEmployee* _Employee, const CString& _Prompt = TEXT("")); // Screen14
	void					OnChangeTransferStatus();											// Screen16
	void					OnShowMatchSummary(CCup* _Cup, CFixture* _Fixture);					// Screen17
	void					OnViewClubFixtures(CClub* _Club, eClubResultsType _ResultToShow);	// Screen18
	void					OnPlayerRequestTransfer(CPlayer* _Player);							// Screen21
	void					OnNegotiatePlayerContract(CString _strPrompt);						// Screen23
	void					OnNegotiateOutOfContractPlayerContract(void);						// Screen23
	void					OnApproachClubLoanInPlayer(void);									// Screen24
	void					OnMakeOfferForPlayer(void);											// Screen30
	bool					DisplayMatchIntro(const bool _bShowSummary);						// Screen32
	void					OnSackPlayer(void);													// Screen35
	void					OnApproachClubLoanOutPlayer(CClubTransferOffer* _TheOffer);			// Screen44
	void					OnManagerRenewContract(CClub* _Club);								// Screen47
	void					OnUserManagerLeaveClubDlg(CString _str);							// Screen47
	void					OnArrangeFriendlies(void);											// Screen54
	void					OnViewNotOwnedEmployeeInformation(void);							// Screen60
	void					OnViewOwnedEmployeeInformation(void);								// Screen60
	void					OnDisplayMatch(CMatchPlay* _MatchPlay);								// Screen65
	void					OnShowTeamOfTheWeek();												// Screen92

#ifdef FMOD
	CSoundManager&			GetSoundManager()
	{
		return m_SoundManager;
	}
#endif

protected:
	CWorld*					m_WorldData;
	bool					m_bIsInitialised;
	int						m_WhichCountry;
	CBasicDisplay			m_Display;
	CSystemPreferences		m_SystemPrefs;
	CApplicationPreferences m_ApplicationPrefs;
	AWRegistrationParametersType m_RegParams;
	CRegistrationUI*		m_pRegSystem;
#ifdef FMOD
	CSoundManager			m_SoundManager;
#endif

	void					OnInitialiseGame(void);
	void					OnInitialiseDRM();

	DECLARE_MESSAGE_MAP()
#ifdef TEST_DIALOGS
	void DebugDialogs();
#endif
};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_POCKETPCFM_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
