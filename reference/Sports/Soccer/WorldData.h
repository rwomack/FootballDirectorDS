

/*
	C++ Classes


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

 #ifndef __WORLD_H__
 #define __WORLD_H__

	class CScreen28View;
    class CStadiumList;
    class CFederationList;
    class CDivisionStrengthList;
    class CPopularityList;
	class CSkillMoneyValue;
	class CSkillMoneyValueList;

	static const byte		NODIVISION = 0xff;


class CWorld
{
public:

	enum eMasterState
	{
		IN_PROCESSING, 
		IN_MENUS,
		IN_INITIALISATION
	};

	// Variables
private:
	eMasterState			m_MasterState;

	CNonDuplicateStringList	m_strForenames;
	CNonDuplicateStringList m_strSurnames;
	CNonDuplicateStringList m_strIDs;
	CNonDuplicateStringList m_TextList;

	CMatchHandler			m_MatchHandler;
    CCalendar				m_CurrentDate;
    ushort                  m_TotalClubs;
	ushort					m_LastGameLoadedPath;
    ushort                  m_CurrentUserID;
	bool					m_bProcessingHalt;
	bool					m_FastTest;

	CString					m_strLastGameLoaded;

	CFixtureController		m_FixtureController;
	CPlayerCareerPathList	m_CareerPathList;
	CSkillMoneyValueList	m_SkillMoneyValueList;
    CDivisionStrengthList	m_DivisionStrengthList;
    CPopularityList			m_PopularityList;
#ifdef USE_CURRENCIES
    CCurrencyList			m_CurrencyList;
#else
	CCurrency				m_Currency;
#endif
    CFederationList			m_FederationList;
    CCountryList			m_CountryList;
	CDivisionList			m_DivisionList;
    CClubList               m_ClubList;
	CClubHistory*			m_ClubHistoryList;
    CStadiumList			m_StadiumList;
    CPlayerList				m_PlayerList;
    CPlayOffList*           m_pPlayOffList;
	CCharityShieldList*     m_pCharityShieldList;
	CCupList*				m_pCupList;
	COutOfContractPlayerList m_OutOfContractPlayerList;

    CAggressionList			m_AggressionList;
    CManagerList		    m_ManagerList;
    CUserList*              m_pUserList;
	CContractClauseList		m_ContractClauseList;
    CFixtureList			m_FreeFixtureList;
	CMatchEventList			m_FreeMatchEventList;

	CInjuryList				m_InjuryList;
	CInjuryTypeList			m_InjuryTypeList;
	CInjuryEventTypeList	m_InjuryEventTypeList;

    CPlayer*				m_pCurrentPlayer;
    CYouth*					m_pCurrentYouth;
    CEmployee*				m_pCurrentEmployee;
    CClub*					m_pCurrentClub;

	eClubResultsType		m_eResultTypeToShow;
	CMatchInfo*				m_pMatchInfo;
	CKitSystem				m_KitSystem;
	CTransferDeadlineSystem m_TransferDeadlineSystem;

#ifdef ACTIVEFEDERATIONCUPS
    CFederationCupList		m_FederationCupList;
#endif
#ifdef ACTIVEINTERFEDERATIONCUPS
	CInterFederationCupList m_pInterFederationCupList;
#endif
#ifdef	ACTIVEREFEREES
    CSoccerRefereeList*     m_pRefereeList;
#endif
#ifdef	ACTIVESTADIUMPLANS
    CStadiumPlanList*       m_pStadiumPlanList;
#endif

public:
    CWorld();
    ~CWorld();
	CWorld& operator =(const CWorld& _World);

// IO Methods
	void					Write(const FILE* _File);
	void					Read(const FILE* _File);

// Methods
	void					DoAutoSave();
	void					DoBuildCupNameList(CSortedStringList& _List);
	void					SetMasterState(const eMasterState _eState);
	eMasterState			GetMasterState();
	void					SetFixturesToView(const eClubResultsType _MatchType);
	eClubResultsType		GetFixturesToView();
	void					AddFreeMatchEvent(const CMatchEvent* _pMatchEvent);
	void					OnCreateUser(const ushort _UserID = 0);
	void					OnInitialiseUser(const ushort _ClubID, const ushort _UserID = 0);
	CUser*					GetCurrentUser();
	bool					GetProcessingHalt() const;
	bool					CurrentUserCanProgress();
	bool 					CurrentUserCantProgress();
  void                  DoNegotiatePlayerContract(CPlayer* _Player, CClub* _Club, const CString &_strPrompt);
	ushort 					DoFindForenameID(const CString& _NewName);
	ushort 					DoFindSurnameID(const CString& _NewName);
	ushort 					DoFindStrID(const CString& _nNewName);
	CFixture*				FindFreeFixture();
    CCalendar&				GetCurrentDate();
	CString&				GetString(const ushort _nClubID);
	CNonDuplicateStringList& GetText();
	CString&				GetForename(const ushort _ID);
	CString&				GetSurname(const ushort Id);
    void                    DoInitialise();
	void					DoCountryJoinFederation(const byte _CountryID, const byte _FederationID);
	void					DoProcessMatches(const bool _ViewEm, const bool _ShowSummary = false);
	void					OnLoanPlayerOut(const ushort _PlayerID, const ushort _ToClubID, const int _NumberOfWeeks);
	void					OnViewSquadDetails(CClub* _Club);
	void					RestoreProgressDuringDivisionMatches();
	void					RestoreProgressDuringDailyTasks();
    
    CClubList&              GetClubList();
	CClubHistory*			GetClubHistory(const ushort _ID);
	CPlayerList&            GetPlayerList();
    CCountryList&           GetCountryList();
	CCupList&				GetCupList();
    CManagerList&           GetManagerList();
    CDivisionList&          GetDivisionList();
    CDivisionStrengthList&  GetDivisionStrengthList();
    CPopularityList&        GetPopularityList();
    CAggressionList&        GetAggressionList();
    CUserList&              GetUserList();
	CFixtureController&		GetFixtureController();
	CInjuryList&			GetInjuryList();
	COutOfContractPlayerList& GetOutOfContractPlayerList();
	CContractClauseList&	GetContractClauseList();
    CPlayOffList&           GetPlayOffList();
    CCharityShieldList&     GetCharityShieldList();
    CFixtureList&			GetFreeFixturesList();
	CMatchEventList&		GetFreeMatchEventsList();
	CInjuryTypeList&		GetInjuryTypeList();
	CInjuryEventTypeList&	GetInjuryEventTypeList();
	CTransferDeadlineSystem& GetTransferDeadlineSystem();
	CKitSystem&				GetKitSystem();

	CPlayer*				GetPlayer(const ushort _PlayerID);
  CClub&          GetClub(const ushort _ClubIndex);
  CClub&		  GetClubByID(const ushort _ClubID);
    CCountry&               GetCountry(const ushort Id);
	CCup*					GetCup(ushort _CupID);
	CStadium*				GetStadium(ushort _ID);
	CManager&				GetManager(const ushort _ID);
  	CPlayerCareerPathList&	GetCareerPaths();
	CSkillMoneyValue&		GetSkillMoneyValueItem(const short _Index);

	ushort					GetTotalClubs() const;
	void					SetTotalClubs(const ushort _Number);
	
	void					SetCurrentPlayer(CPlayer* _Player);
	CPlayer*				GetCurrentPlayer();
	void					SetCurrentYouth(CYouth* pYouth);
	CYouth*					GetCurrentYouth();
	void					SetCurrentEmployee(CEmployee* pEmployee);
	CEmployee*				GetCurrentEmployee();
	void					SetCurrentClub(CClub* pClub);
	CClub*					GetCurrentClub();
	void					SetCurrentMatchInfo(CMatchInfo* pMatchInfo);

	// Player popup menu actions
	void					OnViewPlayerDetails(CPlayer* apPlayer = null);

	// Youth Actions
	bool					OnSackYouth();
	void					OnHireYouth();

	// Employee Actions
	bool					OnSackEmployee();

	// Fixture Actions
	void					OnViewMatchResultInformation();

	bool					SaveGameFile(CString& _Str, const bool _AskOverWrite = true);
	bool					GetGameFromDisk(CString& _FileName);

	bool					SetCurrentUserID(const ushort _ID);
	ushort					CurrentUserClubID();
	CString		            CurrentUserName();
	CClub*					GetCurrentUserClub();
	CPlayer*				GetCurrentUserClubPlayer(const ushort _SquadNum);
	CDivision*				GetCurrentUserDivision();
	CCountry*				GetCurrentUserCountry();
	CManager*				GetCurrentUserManager();
#ifdef USE_CURRENCIES
	CCurrencyList&			GetCurrencyList();
#else
	CCurrency&				GetCurrency();
#endif
	byte					CurrentUserDivisionID();
	ushort					GetCurrentUserID();
	void					MakeUserInActive(const ushort _ManagerID);

	void					DoSkipToEndOfSeason();
	CString					GetForwardDateShortStr(const uint _DatesToNegotiation);
	void					CreateFriendly(ushort _HomeClubID, ushort _AwayClubID, const byte _WeekNumber);
	void					FriendliesEndOfSeason(ushort _ClubID);
	void					ProcessDivisionMatches();
	CString					GetSaveFileSuffix() const;
	byte					GetDataLevel() const;			
	CMatchHandler&			GetMatchHandler();
	CMatchSubs&				GetMatchSubs();
	CMatchPlay&				GetMatchPlay();

	void					OnScreen21(CPlayer* _Player);

#ifdef ACTIVEFEDERATIONS
	CFederation*			GetCurrentUserFederation();
	CFederation&			GetFederation(const CString& _FedName);
	CFederation&			GetFederation(const ushort _FedID);
	CFederationList&        GetFederationList();
#endif
#ifdef	ACTIVEREFEREES
    CSoccerRefereeList*     pRefereeList();
	CSoccerReferee*			GetRefereeID(int _ID);
#endif
#ifdef ACTIVEFEDERATIONCUPS
	CFederationCupList&		GetFederationCupList();
    int                     FederationCupListSize();
	CFederationCup&			GetFederationCup(ushort _CupID);
#endif
#ifdef ACTIVEINTERFEDERATIONCUPS
	CInterFederationCupList* GetInterFederationCupList();
	int                     InterFederationCupListSize();
    CInterFederationCup*    GetInterFederationCup(int x);
#endif
#ifdef ACTIVESCENARIOS
    CScenarios*             ScenarioPtr();
#endif
#ifdef	ACTIVESTADIUMPLANS
    CStadiumPlanList*		pStadiumPlans()									{ return m_pStadiumPlanList; }
#endif

	bool					IsFastTest() const 								{ return  m_FastTest; }
	void					SetFastTest(const bool _Value)					{ m_FastTest = _Value; }
	void					SetProcessingHalt(bool _Value);
};
#endif

