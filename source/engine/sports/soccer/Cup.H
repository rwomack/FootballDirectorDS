
/*
	File:- Cup.h

	Cup class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#ifndef __CUP_H__
#define __CUP_H__


	class CActiveCountry;
	class CLeagueTableList;
    class CSoccerResultList;

	// Special(strange behaviour) cups
	enum eSpecialCupTypes
	{
		NOSPECIALEVENT,
		ENGLISHLEAGUECUPSPECIALEVENT,		// Federation comp qualifiers  excluded until round 3
		DUTCHCUPSPECIALEVENT,
		SPANISHCOPADELRAYSPECIALEVENT,
		GERMANCHARITYSHIELDSPECIALEVENT,
		MLSCHARITYSHIELDSPECIALEVENT,		// Playoff winners
		MLSPLAYOFFSPECIALEVENT				// Winners to charity shield only
	};


class CCup : public CBaseCompetition
{
public:
    // Constructor
    CCup();
    ~CCup();

    // Variables
private:
	CCupRoundList           m_TheRounds;
    CNumericArray<ClubID>	m_NonLeagueClubs;
    ushort					m_NeutralVenueStadiumID;
	CNumericArray<ClubID>	m_LeagueClubsList;
    ushort					m_CountryID;
	ushort					m_CharityShieldID;
	byte					m_RunnersUpManagerRatingPoints;
	byte					m_WinnersManagerRatingPoints;
	CClubAndPlayerIDList	m_CupTiedPlayerList;
    byte					m_SpecialActionFlag;
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	byte					m_PercentagePointsForPlay0ffs;
#endif
#ifdef ACTIVEFEDERATIONCUPS
    CNumericArray<ClubID>   m_FederationQualifiersList;
    byte					m_ExcludeFederationQualifiersUntilRound;
#endif

public:
    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	void					AfterMatchUpdate(const eCupRoundTypes _eRoundType, CFixture& _Fixture, CMatchInfo* _MatchDetails);
	void                    AfterCupMatchUpdate(CCalendar* CurrentDate, CFixture& aFixture, CMatchInfo* MatchDetails);
	CExtString				CupAndReplayTitle(CCupRound& _CupRound);
	CExtString				CupAndRoundTitle(CCupRound& _CupRound);
	CExtString				CupAndRoundName(CCupRound& _CupRound);
	void					DoAddCupHistory(ClubID _WinningClubID, ClubID _RunnersUpClubID, ClubID _SemiLoserClub1ID, ClubID _SemiLoserClub2ID, ushort Id);
	void					DoAnnounceDraw(CCupRound& _CupRound);
	void					DoBuildCupFixtureList(const ushort MatchNumber, CSoccerResultList& FixtList);
	void					DoBuildEventsStrList(CCupDisplayEventList& EventList);
	void					DoCreateRelatedRounds(CCupRound& _CupRound, CNumericArray<ClubID>& _DrawClubIDList);
	CString					DoFindCompetitionTitleForFixture(CFixture* _Fixture);
	CString					DoFindNeutralVenueNameForFixture(CFixture* _Fixture);
	int						DoFindRoundNumber(CCupRound& _CupRound);
	void					DoEndOfSeason(bool _AddTrophy = true);
	CCalendar				DoFindRoundDate(CCupRound& _CupRound, bool _ReplayFlag);
	CCupRound*				DoFindRoundFromFixture(CFixture* _Fixture);
	void					DoNewSeason();
	void					DoPlayMissedMatches();
	void					DoPlayGames(CCupRound& _CupRound, const bool _bShowSummary);
	void					DoPlay2LegGames(CCupRound& _CupRound, const bool _bShowSummary);
	void					DoPlayNormalGames(CCupRound& _CupRound, const bool _bShowSummary);
	void					DoPlayExtraTimeNormalGames(CCupRound& _CupRound, const bool _bShowSummary);
	bool					DoPlayReplays(const eMatchCompetitionType _MatchType, const bool _ShowSummary);
	bool					DoPlayMatches(const bool _bShowSummary);
 	void					DoInitialiseFromResource(CBinaryReader& _Resource, ushort Id, const byte RunnersUpManagerRatingPoints, const byte WinnersManagerRatingPoints);
	void					DoInitialiseBaseFromResource(CBinaryReader& _Resource, const byte RunnersUpManagerRatingPoints, const byte WinnersManagerRatingPoints);
	bool					DoCheckIfTied(CCupRound& _CupRound, CFixture* _LastFixture, CFixture& _Fixture);
    void                    DeleteAll();
	bool					DisplayMatchIntro(CFixture& _Fixture, const bool _bShowSummary);
	CCupRound*				DoFindFinalRound();
	void                    DoDraws();
	void					DoTheDraw(CNumericArray<ClubID>& _DrawClubIDList, eCupRoundTypes _RoundType);
	void					DoAnnounceFinal(CCupRound& _CupRound);
	CFederation&			GetFederation();
	CActiveCountry&			GetActiveCountry();
	CCupRound*				GetFinalRound();
	ushort					GetCountryID() const;
    CNumericArray<ClubID>&  GetLeagueClubsList();
    eSpecialCupTypes		GetSpecialActionFlag();
	CCupRound*				GetFirstLeg(CCupRound& _CupRound);
	uint					GetNeutralVenueStadiumCapacity();
	ushort					GetNeutralVenueStadiumID();
	CString&				GetNeutralVenueStadiumName();
	ClubID					GetNonLeagueClub(ushort _ID);
	CCupRound*				GetPreviousRound(CCupRound& _CupRound);
    CCupRound*              GetLastRoundPlayedPtr(CCalendar* CurrentDate);
	ushort					GetNextRoundNumber(CCalendar& _CurrentDate);
	CClubAndPlayerIDList&	GetCupTiedPlayerList();		
	CCupRound&				GetRound(int _CurrentRound);
    CCupRoundList&			GetRounds();
    int                     GetRounds(CCalendar* CurrentDate);
    int                     GetRoundsPlayed(CCalendar* CurrentDate);
	byte					GetRunnersUpManagerRatingPoints();
	byte					GetWinnersManagerRatingPoints();
	CCountry*				GetCountry();
	byte					GetCountryRanking();
	bool					HaveAllRoundsBeenPlayed();
	bool					IsClubMatchToday(const CCalendar& _CurrentDate, const ClubID _ClubID, CMatchInfo& _Match);
	CCupRound*				IsDrawThisWeek();
	bool					IsNonLeagueClub(ClubID _ClubID);
	ushort					LeagueClubsListSize();
    int                     LastRoundPlayed(CCalendar* CurrentDate);
	bool					HasPlayedRound(const ushort _WeekNumber, int _RoundNum);         // Has a round been played yet?
    CFixtureList*           RoundPlayedThisWeek(CCalendar* CurrentDate);
    CFixtureList*           ReplayPlayedThisWeek(CCalendar* CurrentDate);
	CCupRound*				RoundToday(const CCalendar& _CurrentDate);
	CString					RunnerUpNameStr();
    CString		            SeasonStr();
	void					SetNeutralVenueStadiumID(const ushort _NeutralStadiumID);
    CString		            TodaysTitleStr(const ushort DayNumber);
	int						TotalNonLeagueClubs();
	int						TotalRounds();
	CString					WinnerNameStr();

    struct StPoints
	{
		ClubID ClubNum;
		int	Points;
		int GoalsFor;
		int	GoalsAgg;
	};
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	byte					GetPercentagePointsForPlay0ffs();
	void					SetPercentagePointsForPlay0ffs(const byte x);
#endif
#ifdef ACTIVEFEDERATIONCUPS
    CNumericArray<ClubID>&  GetFederationQualifiersList();
#endif
#ifdef SPANISHCUP
	void					SetUpSpanishCopaDelRay();
#endif
#ifdef NEWSPAPERON
	CString					NewsPaperWinnersHeadline();
#endif
#ifdef CUPGROUPS8
	void					CreateGroup8MatchList(CCupRound& pCupRound);
#endif
#ifdef CUPGROUPS5
	void                    CreateGroup5MatchList(CCupRound& pCupRound, CNumericArray<ClubID>& aDrawClubIDList);
#endif
#ifdef CUPGROUPS6
	void                    CreateGroup6MatchList(CCupRound& pCupRound);
	void                    ReverseGroup6MatchList(CCupRound& pCupRound);
#endif
#ifdef CUPGROUPS
    void                    GetWinners2RegionsGroup3Once(const ushort NextRoundNum, CNumericArray<ClubID>& aTheClubs);
    void                    AddClubsToGroup3(const ushort RoundNum, CNumericArray<ClubID>& aTheClubs);
	void					DoCreateGroup4MatchList(CCupRound& _CupRound);
    void                    CreateGroupMatches(CCupRound& pCupRound, CNumericArray<ClubID>& aDrawClubIDList);
	void					DoBuildGroupTables(CLeagueTableList& _TheTable, int _NumRounds = 0);
	byte					GetNumberOfClubsInGroup();
	byte					GetNumberOfGroups();
    int                     GetNumberGroupMatchesPlayed(const ushort RoundNum);
    void                    GetGroupMatchWinners(CNumericArray<ClubID>& _TheClubID, const int _NumInNextRound);
	void					ReverseGroupMatchList(CCupRound& pCupRound);
    void                    ReverseGroup4MatchList(CCupRound& pCupRound);
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CCupList : public CExtArray
{
public:
    // Constructor
	CCupList();
    ~CCupList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoBuildNameList(CStringArray& _theList);
    void					DoDraws();
	void					DoEndOfSeason();
	CString					DoFindNeutralVenueNameForFixture(CFixture* _Fixture);
	CString					DoFindCompetitionTitleForFixture(CFixture* _Fixture);
	void					DoNewSeason();
    int						Find(CCup* pCup);
	CCup*					GetAt(const ushort _ElementNumber);
	bool					IsClubMatchToday(const CCalendar& _CurrentDate, const ClubID _ClubID, CMatchInfo& _Match);
    bool                    IsPtrInList(CCup* pCup);
	bool					DoPlayMatches(const bool _ShowSummary);
	void					DoPlayMissedMatches();

	// Variables
private:
	CCup*					m_pList;
};
#endif
