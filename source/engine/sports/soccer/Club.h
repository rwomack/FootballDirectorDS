

/*

	Class - C++ Classes

	Club class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __CLUB_H__
#define __CLUB_H__

#ifdef NDS
#include <StringID.h>
#include <TeamTactic.h>
#include <defines.h>
#include <extstring.h>
#include <SoccerDefines.h>
#endif

	class CActiveCountry;
	class CAccountsItemList;
    class CClubHistory;
	class CGrid;
	class CMessageDisplay;
	class CLeadingGoalScorerList;
	class CLeadingDisciplinaryList;
	class CLeagueTable;
	class CMatchInfo;
    class CSquadTraining;
	class CCurrentForm;
	class CPlayOff;
	class CUser;
	class CManager;
	class CStadium;
	class CDivisionStrength;
	class CFederation;
	class CFinancialLoan;

	static const int		MAXCLUBLISTEDPLAYERS = 5;

	
class CClub
{
#ifdef NDS
  DECLARE_FDMEMORY_BINALLOC(CClub);
#endif
public:
    // Constructors
    CClub();
    ~CClub();

  // Data Access Methods
	CActiveCountry&			GetActiveCountry();
    CString&				GetName();
	CStringID&				GetNameID();
	CString&				GetNickname();
    CString					GetLongClubName();
    CString					GetUpperCaseName();
    ushort					GetStadiumID()const;
    ushort					GetCurrentTacticsID();
	int						GetTacticsPlayerPositionX(int _Value);
	int						GetTacticsPlayerPositionY(int _Value);
	ushort					GetManagerID() const;
    byte					GetRegionID()	const;
    byte					GetPopularityID()	const;
    byte					GetMorale()	const;
	CTeamTactic&			GetTeamTactic();
	CFinancialLoan*			GetFinancialLoan();
    uint					GetMidWeekMatchGateReceipts();
    uint					GetWeekEndMatchGateReceipts();
	CString					GetManagerWageOfferStr();
    uint					GetTicketPrice();
    int						GetCashBalance() const;
    byte					GetDivisionID() const;
	void					SetName(const CString &cNewName);
	void 					SetCurrentTacticsID(const byte _TacticsID);
	void					SetManagerID(const ushort _NewID);
	void					SetPopularityID(const byte _NewID);
	void					SetMorale(const byte _Value);
    void					SetTicketPrice(const uint x);
	void					SetDivisionID(const byte _Value);

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Other methods
	void					AddToNewsList(const CExtString& _Message);
	void                    AddToClubNews(const int aStrID);
	void					AddToClubNews(const CExtString& str);
	void					AfterCupMatchUpdate(const eWhichTeam _Venue, CFixture& _Fixture, const ushort _strTitleID, const ushort _strVenueID);
	int						AllowToHirePlayer(CYouth* pYouth, bool ShowMessage);
	bool					AreBuyTransfersAllowed();
	bool					AreLoanTransfersAllowed();
	bool					AskSackYouth(CYouth* _Youth);
	bool					AskSackEmployee(CEmployee* pEmployee);
	void					AvoidedRelegation();
	void					BeenPromoted(bool _AsChampions);
	void					BeenPromotedViaPlayOff();
	void                    BeenRelegated();
	bool                    CheckIfPlayerRefusedTransfer(const ushort a_nPlayerID);
	bool					CheckIfPlayerRefusedTransfer(CPlayer* pPlayer);
	bool                    CheckIfPlayerRefusedLoan(const ushort a_nPlayerID);
	bool					CheckIfPlayerRefusedLoan(CPlayer* pPlayer);
	void                    CheckAwayRecords(CFixture& aFixture, const ushort strTitleID, const ushort strVenueID);
	void					CheckHomeRecords(CFixture& _Fixture, const ushort _strTitleID, const ushort _strVenueID);
	void					DoAccountsPlus(const CAccountsItemList::eAccountItemType _Which, const uint _HowMuch);
	void 					DoAddPlayerToSquad(const ushort _PlayerID);
	void					DoAddToAccounts(int _Amount, int _AmountStrId, const enum CAccountsItemList::eAccountItemType _Which);
	void 					DoAddYouthToSquad(CYouth* _AvailableYouth);
	void					DoAddTransferExpenditure(const uint _Cost, CContract* _PlayerContract = NULL);
	void					DoAddTransferIncome(const int _Cost);
	void					DoAfterMatchUpdate(const ushort _PrimaryTactics, CFixture & _Fixture, eWhichTeam _Venue, const byte _AwayGatePercentage, const byte _FAGatePercentage);
	bool					DoAgreeToExtendPlayerLoan(CPlayer& _Player, const CString &_ClubName);
	bool					DoApplyForFinancialLoan();
	void					DoAwayUpdateTables(CFixture& _Fixture, int _PointsForAWin, bool _HomeWinPenShootout);
	bool                    DoBidForPlayer(ushort _PlayerBiddingFor, ClubID _PlayerToBidForClub);
#ifndef TESTBED
	void					DoBuildLeagueTableRow(const ushort _Row, CGrid& _Grid);
#endif
	bool 					DoBuyPlayer(CPlayer& _Player, const uint _Cost, CContract& _PlayerContract, const CStringID& _ClubNameID);
	void 					DoBuyTransferPlayer(CPlayer& _Player, const uint _Cost, CContract& _PlayerContract, CClub* _FromClub, byte _SellOnPercentage);
	void					DoCalculateManagerContractOffer();
	void 					DoCheckDisciplinaryPosition();
	void					DoCheckIfSquadLacking();
	bool 					DoCheckSendPlayerBackFromLoan(ushort _SquadNumber);
	void					DoConsiderManagerPositionAfterLeagueMatch(CFixture& aFixture);
	void					DoCreateBuyPlayerSearch(CPlayerSearch& _PlayerSearch, bool _IncludeNonListed = true);
	void					DoFinancialLoanAccepted(const uint HowMuch);
	ClubID					DoFindID() const;
	void					DoHomeUpdateTables(CFixture& _Fixture, int _PointsForAWin, bool _HomeWinPenShootout);
	CExtString				DoHumanManagerWelcomeStr();
	void					DoHumanManagerWelcome();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	void					DoInitialiseCupClubFromResource(const CString& _szName, CBinaryReader& _Resource, CCountry& _Country);
	void					DoInitialiseTicketPrices();
	void					DoManagerJoinedClub(const ushort _NewID);
	void 					DoPaySignOnFee(const uint _SignOnFee);
#ifndef TESTBED
	void					DoPreMatchReport(CMessageDisplay* pMessageDisplay);
#ifdef NDS
	void					DoPreMatchReportFDL(lArray<CString> * _pStringList);
#endif
#endif
	void					DoQualifiedBottomPlayOff();
	void 					DoQualifiedTopPlayOff();
	void 					DoRaiseMorale(const byte _Value);
	void					DoReceivedSellOnClauseMoney(const uint _Cost, byte _SellOnClausePercentage, const CString &_PlayerName, const CString &_ClubName);
	void 					DoRemoveFromSquad(const ushort _PlayerToRemoveSquadNumber);
	void 					DoRemovePlayerFromSquad(const ushort _PlayerID);
	void 					DoRenewManagerContract();
	void 					DoSackManager(bool _AnnounceUserSacking = true);
	void 					DoSackStaff(CEmployee* _Employee, const uint _ContractValue);
	bool					DoSubmitCounterAskingPrice(CClubTransferOffer* _BidDetails, CClubTransferOffer& _CounterBidDetails);
	bool					DoSubmitCounterLoanPeriod(CClubTransferOffer* _OriginalOfferDetails, const int _NewPeriod);
	void					DoSelectTactics(CClub* _OpponentClub, const eMatchCompetitionType _MatchType);
	bool					DoTryAcceptBidFromNonUser(CClubTransferOffer& _TheOffer);
	void 					DoTryBuyPlayer(CPlayerSearch& _PlayerSearch);
	void                    DoTryBuyPlayers();
	void 					DoTryHireCoach();
	void 					DoTryHireManager();
	bool 					DoTryPlayerForManager();
	void 					DoTryCoachForManager();
	void                    DoTrySackManager();
	void					DoTrySellHumanManagedPlayers();
	uint					DoTrySellPlayerIfListed(CClubTransferOffer& _TheOffer);
	void					DoUpdateTables(CFixture& _Fixture, int _OurGoals, int _TheirGoals, int _PointsForAWin);
	bool					DoUserApplyFinancialLoan(const uint HowMuch);
	void					DoUserRepayFinancialLoan(const uint dValue);
    bool                    IsUserControlled();
	CClubHistory&			GetClubHistory();
	CKit&					GetHomeKit();
	byte					GetHomeKitID();
	bool					CanSparePlayer(CPlayer* _pPlayer);
    byte					CoachSkill();
	ushort					DoCountPlayersOnPitch();
    void                    DoCheckSackManager();
    int                     CountAvailableFriendlies();
	bool					IsRanking0Club();
    void                    DoClearTable();
	ushort 					GetCountryID();
    CCountry&				GetCountry();
	byte					GetCountryRanking();
    void                    PostInitialise();
    void                    CalcManagerContractOffer();
    CDivision&				GetDivision();
	byte					GetDivisionStrengthID();
	void					DoDailyTasks(bool _IsWeekEnd);
    void                    DeleteAll();
    void                    DeleteStaff();
	void					DoRepayAllLoan();
    void                    DoEnsureEnoughFitPlayers();
    void                    DoEmployStaff(CEmployeeBase::eEmployeeType Type);
    int                     DoEmployStaff(CEmployee* pEmployee);
    void                    ExtendGround();
	void					DoEndOfWeekTasks(bool _NoWarnings);
	void					DoEndOfSeason();
	void					DoFindFreeTransferPlayer(ePlayerPosition _Posn);
    int                     FindSuitablePlayerForManager();
	int						FindYouthNum(CYouth* pYouth);
	int						FindEmployeeNum(CEmployee* pEmployee);
	void					DoFinishedBottomOfDivision();
	CString					FormationStr();
	int						DoCountLeagueGamesPlayedThisSeason();
	void					GetNextMatchDetails(CMatchInfo& Match);
	bool					GetLeagueFixtureDetails(CMatchInfo& _MatchInfo, const int _TheWeek, const eClubResultsType _eResultToShow);
	uint					GetRecommendedLoanRepayment();
    CString                 GetCashStr();
    ushort					GetYouthListNo(ePlayerPosition Position, int No);
	byte					GetPersonalTrainingHours();
    const uint				GetWageAmountLength();
    const uint				GetLoanAmountLength();
    ushort					GetCoachStaffNumber();
    CEmployee*              GetCoach();
    ushort					GetPhysioStaffNumber();
	CPlayer&				GetPlayer(const ushort _SquadNumber);
	ushort					GetPlayerID(const ushort _SquadNumber);
    CString                 GetTicketPriceStr(bool CurrencyStr);
	bool					GotEnoughForeignMoneyMsg(const uint Amount);
    bool                    GotEnoughMoneyMsg(const uint Amount);
    bool                    GotEnoughMoney(const uint Amount);
    void                    GotPlayerOnLoan(const ushort player, const int iWeeks);
	bool					InterestedInPlayer(CPlayer* _Player, const uint _BidAmount);
    bool					IsNonLeagueClub();
    bool                    IsEmployeeSkillSuitable(const byte skill);
    bool                    IsGoodClub();
	bool					IsManaged();
	bool					IsYouthAtClub(CYouth* _Youth);
	bool                    IsEmployeeAtClub(CEmployee* pEmployee);
    bool                    IsValidUserControlledTeam(const byte aNumSubs, CString &cReason);
	bool					IsUserGoodEnoughToManageClub(int UserNumber);
    CString                 LoanPaidThisWeekStr();
	void					DoRepayLoan(uint HowMuch);
    void                    LowerPlayerMorales(int x);
	void 					DoTransferListRandomPlayers(const uint _HowMany);
	bool 					IsLeagueMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match);
    void                    MakeFriendliesNull();
	void					MakeUnPopular();
	void					MakeCountryRanking0();
	void					MakeCurrentUser();
    CManager&               GetManager();
    CString                 GetManagerName();
	void					DoManagerResigned();
	int						ManagerWeeksAtClub();
    byte					MinPlayerSkill();
	void					DoMakeUserControlled();
	ushort					MatchNumberToFriendlyNumber(const ushort MatchNumber);
	bool					IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match);
    bool                    NeedEmployee(CEmployeeBase::eEmployeeType WhatType , bool bWarnings =  true);
	void					DoNewSeason(const byte _Position);
	byte					GetNumberOfClubsInDivision();
    int                     NumberForeignDigits(int Value);
	byte					NumberSubsSelect();
    byte					PhysioSkill();
	void					OnPlayerAskedForMove(CPlayer* _Player);
	void					DoPlayerGoingOutOnLoan(const ushort _PlayerID);
	void					DoPlayerBackFromLoan(const ushort _PlayerID);
    CPopularity&			GetPopularity();
	void                    PromoteYouth(const ushort YouthNum, CContract* Contract);
    void                    RaisePlayerMorales(int x);
    void                    DoRenewHumanManagerContract();
	void					RemoveFromUserControlledSquad(ushort PlayerToRemove);
	void					RunnersUp(CDivision& _Div);
	void					RunnersUp(CCup* pCup);
	void					RunnersUpPlayoff(CPlayOff* _Cup);
    byte					ScoutSkill();
    CStadium*				GetStadium();
	int						StandsUnderRefurbishment();
	void					DoSackStaffIfContractExpired(bool _HumanManager);
	void					SackPlayer(CPlayer* _Player);
	void					SackYouth(CYouth* _Youth);
    void					DoSelectRandomTactics();
	void					DoSelectTeam(const byte _NumSubs);
	void					DoSellPlayer(CPlayer& _Player, const uint _Cost, CClub& _BiddingClub, bool _WasRecordSigning, bool _IsBosman);
    void                    DoSellARandomPlayer(bool TeamPlayer = false);
    void					SetThisWeeksLeaguePosition();
	void					DoSoldPlayer(CPlayer& _Player, CClubTransferOffer& _TheOffer, CContract& _ContractRequired, bool _IsBosman = false);
	void					DoStaffMemberLeavingClub(CEmployee* _Employee);
	void					DoLowerMorale(const byte _Value);
	CLeagueTable&			GetTableData();
	CTactic&				GetTactics();
	CTactic&				GetTactics(const ushort _TacticsID);
	CString					TacticTitleAndFamiliarityStr();
    ushort					TotalStaff();
	void					TurnRanking0Off();
	void					TurnRanking0On();
	void					TurnAccountsOn();
	bool					TooManyPointsPerGame();
    ushort					TotalYouths();
	bool 					DoTryAcceptTransferBid(CClubTransferOffer& _TheBid);
    bool					TryAcceptLoanTransferBid(CPlayer* pPlayer, CClub* pBiddingClub, int HowLong, CString* str);
	void                    TryRetirePlayersThroughInjury();
    CUser&                  GetUser();
	CUser&					GetNonUser();
	ushort					GetUserNumber();
	void					DoWeeklyFinance();
	void 					WonCup(CCup* _Cup);
	void 					WonPlayoff(CPlayOff* _Cup);
	void 					WonTitle(bool _IsTopDivision);
	void					WonCharityShield(const CString &title, int Id, bool IsCup);
    void                    CheckEndOfSeasonRecords();
	CClubSeasonStats&		GetSeasonStats();
	CYouthList&				GetYouthList();
	CEmployeeList&			GetEmployeeList();
	CEmployee*				GetEmployee(const ushort _ListPos);
	CPlayerSquad&			GetFirstTeamSquad();
	void					DoWeeklyClearDown();
	void					WonMOM(const CString& _MonthStr, CString& _DivisionName);
	void					WonTrophy(const CString &title, int Id);
	void					TrophyWinners(CString _CompetitionTitle, int _TrophyID, const bool _IsCup);
	void					TrophyRunnersUp(CString _CompetitionTitle, const int _TrophyID, const bool _IsCup);
	CYouth*					GetYouth(ushort _ListPos);
	CSquadTraining&			GetFirstTeamSquadTrainingDetails();
	CAccountsItemList*		GetAccounts();
    CCurrentForm*           GetCurrentForm();
	CSeasonLeagueTableData*	GetLeaguePositionAfterMatch();
	CManagerContractOffer&	GetManagerContract();
	CString					GetForeignStr(int _Amount);
	CCurrency&				GetCurrency();
	void					SetID(ClubID _Value);
#ifdef NDS	
  	ClubID        			GetID() 										{ return m_ID; }
#endif
	//    CNewsPaperEvents*       NewsPaperEventPtr();

#ifdef TROPHYHISTORY
	CTrophyHistoryList*		GetTrophyHistory()
	{
		return &m_TrophyHistory;
	}
#endif
#ifdef ALLOWSTADIUMBUILDING
	void					SetBuildingCosts(const uint Cost);
#endif
#ifdef	ACTIVESTADIUMPLANS
    void					RaiseCapacity(int NewCapacity);
	void					LowerCapacity(int NewCapacity);
#endif
#ifdef ACTIVEFEDERATIONCUPS
	void					QualifiedForFederationCup();
#endif
#ifdef ACTIVEFEDERATIONS
	CFederation& GetFederation();
#endif
#ifdef	MATCHTRACKING
	int						DoCalculateAverageAttendance();
	int						DoCountHomeLeagueGamesPlayedThisSeason();
	uint					GetGameAttendance(int num);
	uint					GetGateReceipts(int num);
	uint					MaxAttendance();
	void                    SetGameAttendance(int Num, int x);
	void                    SetGateReceipts(const ushort aNum, const uint aAmount);
	void					InitialiseMatchTracking();
	void					TurnMatchTrackingOff();
#endif
#ifdef ACTIVEFRIENDLIES
	CString                 FriendlyDateStr(const ushort FriendlyNum);
	CClubFriendlies&		GetFriendlies();
#endif
#ifdef USECLUBYEARFOUNDED
	CString					GetYearFoundedStr();
#endif

    // Variables
 private:
	ClubID					m_ID;
	ushort					m_StadiumID;
	int						m_CashBalance;
	uint					m_TicketPrice;
	byte					m_RegionID;
	byte					m_DivisionID;
	byte					m_BidsThisWeek;
	byte					m_TeamMorale;                   // When drops below certain value, manager is sacked
	byte					m_CurrentTacticsID;
	byte					m_Temperament;
	byte					m_PopularityID;
	byte					m_HomeKitID;
	ushort					m_ManagerID;
	CStringID				m_NameID;
	CStringID				m_NicknameID;
	CYouthList              m_YouthSquad;
    CEmployeeList           m_StaffList;
    CSquadTraining   		m_SquadTrainingDetails;
    CCurrentForm*           m_pCurrentForm;
	CPlayerSquad			m_squadFirstTeamPlayers;
	CClubHistory			m_ClubHistory;
	CLeagueTable			m_TableData;
	CRefusedDealList		m_RefusedDeals;
	CFinancialLoan			m_FinancialLoan;
    CAccountsItemList*		m_pAccounts;
	CManagerContractOffer	m_ManagerContract;
	CTeamTactic				m_TeamTactic;
	CClubMatchDetails		m_WeekEndMatch;
	CClubMatchDetails		m_MidWeekMatch;
	CClubSeasonStats		m_SeasonStats;

#ifdef TROPHYHISTORY
	CTrophyHistoryList		m_TrophyHistory;
#endif
#ifdef PLAYERTRANSFERHISTORY
    CTransferHistoryList    m_PlayersBought;
	CTransferHistoryList    m_PlayersSold;
#endif
#ifdef USELONGCLUBNAME
    CExtString              m_LongClubName;
#endif
#ifdef USECLUBYEARFOUNDED
	ushort					m_YearFounded;
#endif
#ifdef ACTIVEFRIENDLIES
	CClubFriendlies			m_Friendlies;
#endif
#ifdef	MATCHTRACKING
    CNumericArray<uint>		m_GameAttendance;
    CNumericArray<uint>		m_GateReceipts;
#endif
};
#endif
