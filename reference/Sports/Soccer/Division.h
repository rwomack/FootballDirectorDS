
/*

	File:- Division.h

	Divison class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */

#ifndef __DIVISION_H__
#define __DIVISION_H__


	enum
	{
		JLEAGUEPOINTS = 255,
		MLSPOINTS = 254
	};

	enum eLeaguePositionStatus
	{
		NormalColor,
		RelegatedColor,
		PlayOffColor,
		PromotedColor
	};

	enum eDivisionMatchType
	{
		DIVNOMATCH = 200,
		DIVENDSEASON,
		DIVNEWSEASON,
		DIVCLOSESEASON,
		DIVTRAININGONLY,
		DIVPLAYOFFINIT,		// Create the playoffs
		DIVPLAYOFFMATCH1,
		DIVARRANGEFRIENDLIES,
		DIVFRIENDLYMATCH1,
		DIVFRIENDLYMATCH2,
		DIVFRIENDLYMATCH3,
		DIVFRIENDLYMATCH4,
		DIVFRIENDLYMATCH5,
		DIVFRIENDLYMATCH6,
		DIVFRIENDLYMATCH7,
		DIVFRIENDLYMATCH8,
		DIVPLAYOFFMATCH2,
		DIVPLAYOFFMATCH3,
		DIVPLAYOFFMATCH4,
		DIVPLAYOFFMATCH5,
		DIVPLAYOFFMATCH6,
		DIVPLAYOFFMATCH7,
		DIVPLAYOFFMATCH8,
		DIVPLAYOFFMATCH9,
		DIVPLAYOFFMATCH10,
		DIVPLAYOFFMATCH11,
		DIVPLAYOFFMATCH12,
		DIVPLAYOFFMATCH13,
		DIVPLAYOFFMATCH14,
		DIVPLAYOFFMATCH15,
		DIVPLAYOFFMATCH16,
		DIVPLAYOFFMATCH17,
		DIVPLAYOFFMATCH18,
		DIVPLAYOFFMATCH19,
		DIVPLAYOFFMATCH20
	};

	class CPlayerOfTheWeek;
	class CSoccerResult;
    class CSoccerResultList;
    class CTeamOfTheWeek;
	class CSortedStringList;

	class CScreen28View;

///////////////////////////////////////////////////////////////////////////////////////////////


class CBuySellPercentageChance
{
public:
	CBuySellPercentageChance();

    void					DoInitialiseFromResource(CBinaryReader& _Resource);
	byte					GetPercentageChanceSell() const;
	byte					GetPercentageChanceBuy() const;

	// Variables
private:
    byte					m_PercentageChanceBuy;
    byte					m_PercentageChanceSell;

};
///////////////////////////////////////////////////////////////////////////////////////////////


class CDivision : public CBaseCompetition
{
	// Variables
private:
	CFixtureList            m_MatchData;
    byte					m_MatchTypeCalendar[TOTALMATCHES];
    ushort					m_FirstClubNumberInDiv;
    ushort					m_CountryID;
    ushort					m_CharityshieldID;
	CDivsionPlayOffInfo		m_TopPlayOff;
	CDivsionPlayOffInfo		m_BottomPlayOff;
    byte					m_PointsForWin;
    byte					m_DivisionStrengthID;
    byte					m_NumberClubsPromoted;
    byte					m_NumberClubsRelegated;
    ushort					m_NumberMatchesPlayedThisSeason;
    byte					m_MatchesPlayedThisMonth;           // count of how many groups of matches played this month
    byte					m_NumberOfClubs;
	CBuySellPercentageChance m_CountrySellBuyChances;
	byte					m_HowLongTillNextLeagueMatch;
	byte					m_WeeksToEndOfSeason;
	bool					m_PlayOffForTitle;
    CManagerOfTheMonth		m_MOMWinners[12];                // Manager
	CTeamOfTheWeek			m_TeamOfTheWeek;
#ifdef FEDERATIONTRANSFERS
	CBuySellPercentageChance m_FederationSellBuyChances;
#endif

public:
    // Constructors
	CDivision() {};
	~CDivision();

private:
	ushort					GetLastClubNumberInDiv();

public:
	ushort					GetNumberOfClubsInDivision() const;
	CClub&					GetFirstClubInDivision();
	ushort					GetFirstClubNumberInDivision();
	byte					GetDivisionStrengthID();
	byte					GetNumberClubsPromoted() const;
	byte					GetNumberClubsRelegated() const;
    byte					GetNumberClubsRelegatedIncludingPlayOffs();
	byte					GetNumberClubsAutomaticallyRelegated() const;
	byte					GetNumberClubsBottomPlayOff();
	byte					GetNumberOfClubsTopPlayOff();
	CTeamOfTheWeek&			GetTeamOfTheWeek();
	CBuySellPercentageChance* GetCountrySellBuyChances();
	CFixture&				GetFixture(const ushort _Num);
	byte 					GetPointsForWin();
    byte					GetPointsForDraw();
    ushort					GetCountryID() const;
	ushort					GetCharityShieldID();

	// IO methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

    // Other methods
#ifndef TESTBED
	void					BuildSortedAlphaClubList(CSortedStringList& _ListSortedClubs);
#endif
	void					DoAddPlayOffQualifiers(int PlayOffID, CNumericArray<ushort>& _DrawClubs);
	void					DoAfterMatchUpdate(CFixture& _Fixture);
	void					DoBuildFinishedOrderList(CNumericArray<ushort>& _ClubsStartingOrder, int _FirstOffset);
	void					DoBuildPopularityClubPtrList(CNumericArray<ushort>& _TheList, const ushort _ThePopularity);
	void					DoBuildPlayerSkillClubPtrList(CNumericArray<ushort>& _TheList, const byte _PlayerSkill);
	void					DoBuildLeagueFixtureList(CCalendar& TheDate, CSoccerResultList& ResultList);
	void					DoBuildLeagueFixtureList(const byte _MatchDayNumber, CSoccerResultList& _ResultList);
	void					DoBuildManagerOfMonth();
	void					DoBuildPlayOffFixtureList(const byte _MatchDay, CSoccerResultList& _ResultList);
	int						DoCountClubsScoredMore(CClub& _Club);
	int						DoCountClubsScoredLess(CClub& _Club);
	int						DoCountClubsConcededMore(CClub& _Club);
	int						DoCountClubsConcededLess(CClub& _Club);
	int						DoCountGoodScorersForClub(CClub& _Club);
	ushort					DoCountListedPlayers();
	ushort					DoCountMOMWinners();
	void					DoClearMOM();
    void					DoClearTables();
	CClub*					DoFindUserClub();
	void					DoFindFirstMatch(CCalendar& _TheDate);
	void					DoFindEndOfSeason(CCalendar& _TheDate);
	CClub&					DoFindClubInTablePosition(ushort _Position);
	ushort					DoFindClubNumberInTablePosition(ushort _ClubToFind);
	ushort					DoFindClubNumberBottomOfTable();
	ushort					DoFindPlayerClub(ushort _PlayerID);
	CClub*					DoFindRandomNonUserClub(CClub* _ClubNotToFind);
	CClub*					DoFindRandomNonUserBuyingClub(CClub* _ClubNotToFind, CPlayer* _Player, const uint _BidAmount);
	void					DoFindStartOfNextSeason(CCalendar& _TheDate);
	void					DoFindStartOfSeason(CCalendar& TheDate);
	ushort					DoPromoteClubInTablePosition(const int _TablePosition);
	void					DoDailyTasks();
	CFixture&				DoInitialiseFixture(const ushort _HomeClubID, const ushort _AwayClubID, CMatchInfo _Match);
	bool					DoPlayFriendlies( const bool bShowSummary);
	bool					DoPlayLeagueMatches(const bool _ShowSummary);
	void					DoPlayMissedMatches();
	bool					DoEndOfSeason();            // Returns true if human manager in division
	void					DoEndOfWeekTasks(bool _NoWarnings);
	void					DoFindStartOfSeason(CCalendar* TheDate);
	void					DoInitialise();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	void					DoMidWeekTasks();
	void					DoNewSeason();
	CPlayOff*				GetBottomPlayOff();
	void					ClubMatchListForCurrentMonth(const ushort iClubID, CPointerArray<CMatchInfo> & MatchesThisMonth);
	CCountry&				GetCountry();
	byte					GetCountryRanking();
	int						CountGoodFoulersForClub(CClub* pClub);
	void					CreateTransferList();
	void					CreateClubsStartingOrder(CNumericArray<ushort>& _ClubsStartingOrder);
    void					DeleteAll();
	CDivisionStrength&		GetDivisionStrength();
	int						GetCurrentMatchDayNumber();
	int						GetNumberGamesPerSeason();
	ushort					GetNumberMatchesPlayedThisSeason() const; 	
	byte					GetMatchType(const byte _MatchDayNumber);
	CPlayOff*				GetTopPlayOff();
	CManagerOfTheMonth&		GetManagerOfMonth(const ushort _MonthNum);
	byte					GetHowLongTillNextLeagueMatch();
	byte					GetWeeksToEndOfSeason();
	CClub&					FindClubNumberInCurrentFormPosition(const ushort _FormPosition);
	void					GetAllTransferListed(CClub* _Club, CPlayerSearch& _PlayerSearch);
	CClub&					GetClub(const ushort _ListID);
	ushort					GetLeagueMatchResult(const byte _MatchDayNumber, const ushort _HomeClubID, const ushort _AwayClubID);
	void					GetClubsInRegion(int _RegionToFind, CNumericArray<ushort>& _TheClubs);
	ushort					GetClubID(const ushort _ListID);
	byte					GetNumberPromotedViaPlayOffs();
	int						GetLeadingScorerGoals();
	int						GetLeadingFoulerPoints();
    bool					HasPlayedLeagueMatch();
	int						HowLongTillNextLeagueMatch();
	bool					IsCloseSeasonToday(const int _MatchDayNumber);
	bool					IsCompetitiveMatch(const byte _MatchDay);
	bool					IsPlayOffMatchDay(const byte _MatchDayNumber);
	bool					IsFriendlyMatchDay(const byte _MatchDayNumber);
	bool					IsStartOfSeason(CCalendar* _TheDate);
	bool					IsTopDivision();
	eLeaguePositionStatus	LeaguePositionStatus(const byte iLeaguePositon);
	void					MakeCountryRanking0Clubs();
	ushort					NumberMatchClubs();
	int						NumberOfPointsToEscapeRelegation(CClub* _Club);
	void					NextMatchDateThisSeason(CCalendar& _TheDate);
	void					PrevMatchDateThisSeason(CCalendar& _TheDate);
    bool					PlayedAGame();
	byte					PointsOfClubInPosition(int Position);
	void					SeekClubFixtureForMatchNumber(const ushort _ClubID, const byte _MatchNumber, CSoccerResult& _SoccerResult);
    void					SortCurrentForm();
    void					SortLeagueTable(int iMode = 0);
	int						TodaysFixtureListStart(const byte _MatchDayNumber);
	int						WeeksToEndOfSeason();
#ifdef LATESTSCORES
	void					DoBuildCurrentFixtureList(CFixtureList& _LatestScores);
#endif
#ifdef ASSISTS
	ushort					CountGoodAssistersForClub(CClub* pClub);
	ushort					GetLeadingAssisterAssists();
#endif
#ifdef ACTIVEFRIENDLIES
	ushort					FriendlyMatchType(int DayNumber);
	CCalendar				FriendlyDate(int FriendlyNum);
	CString					FriendlyDateStr(const int _FriendlyNum);
	ushort					WhichFriendlyNumberToday(int _MatchDayNumber);
	int						FriendlyNumberToMatchNumber(int FriendlyNumber);
#endif
#ifdef	QUICKRESULTS
	void					QuickResults(const byte DayNumber);
#endif
#ifdef FEDERATIONTRANSFERS
	CBuySellPercentageChance* pFederationSellBuyChances()
	{
		return &m_FederationSellBuyChances;
	}
#endif
#ifdef ACTIVEFEDERATIONCUPS
	ushort					RandomClubNotInContinental(const ushort ClubToIgnore);
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
class CDivisionList : public CExtArray
{
	// Variables
private:
	CDivision*				m_pDivisionList;

public:
	CDivisionList();
    ~CDivisionList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoAddPlayOffQualifiers(int _PlayOffID, CNumericArray<ushort>& _DrawClubs);
	void					DoBuildManagerOfMonth();
#ifndef TESTBED
	void					DoBuildNameList(CSortedStringList& _theList);
#endif
	void					DoDailyTasks();
	void					DoEndOfWeek(bool _NoWarnings);
	ushort					DoFindID(CDivision* _Division);
	void					DoInitialise();
	void					DoNewSeason();
	bool					DoPlayLeagueMatches(bool _ShowSummary);
	void					DoPlayMissedMatches();
	void					BuildMOS();
	void					CreateTransferList();
	byte					GetDivisionStrengthID(int _IndVal);
	CDivision&				GetAt(ushort _ElementNumber);
    CDivision*				RandomPtr();
	void					UpdateTables();
};
#endif
