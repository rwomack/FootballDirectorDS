
/*
    File:- CupRound.h

	C++ Classes

	Cup round class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __CUPROUND_H__
#define __CUPROUND_H__


class CCupDisplayEventList;
class CLeagueTableList;


	static const byte		IGNOREDATE = 255;					// No action on this event date in cup round
    
	enum
    {
		ISDRAW, ISMATCH, ISREPLAY
	};


	// Round types
	enum eCupRoundTypes
	{
		NORMALMATCH,
		EXTRATIME,
		REVERSEDNORMAL,
		REVERSEDEXTRATIME,
		LEG1,
		LEG2,
		LEG2EXTRATIME,
		LEG1AWAYDOUBLE,
		LEG2AWAYDOUBLE,
		LEG2EXTRATIMEAWAYDOUBLE,    // 9
		GROUPMATCH,
		NEUTRAL,
		GROUP3ONCE2REGIONS,
		NORMALMATCH2REGIONS,
		EXTRATIME2REGIONS,
		REVERSEDNORMAL2REGIONS,
		REVERSEDEXTRATIME2REGIONS,
		LEG12REGIONS,
		LEG22REGIONS,
		LEG2EXTRATIME2REGIONS,      // 19
		LEG1AWAYDOUBLE2REGIONS,
		LEG2AWAYDOUBLE2REGIONS,
		LEG2EXTRATIMEAWAYDOUBLE2REGIONS,
		NEUTRAL2REGIONS,
		NEUTRALEXTRATIME,
		ENDOFCUP,
		LEG1AWAYDOUBLE1423,
		LEG1AWAYDOUBLE1324,
		EXTRATIME1423,
		EXTRATIME1432,
		GROUP415372648,
		MAXROUNDTYPES
	};

	class CLeagueTable;
    class CSoccerResultList;

class CCupRound
{
	friend class CCupRoundList;

public:
    CCupRound() {};
    ~CCupRound();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
#ifdef LATESTSCORES
	void					BuildCurrentNonUserFixtureList(CFixtureList& _LatestScores);
#endif
	void					BuildMatchDisplayList(CSoccerResultList& _ResultList);
    void					BuildReplayDisplayList(CSoccerResultList& ResultList);
	void					BuildDisplayList(CFixtureList& _GamesList, CSoccerResultList& _ResultList);
	ushort					DoAddLastRoundWinners(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup);   // returns the number of winners found
	bool					DoAwayGoalsCountDouble();
	void					DoBuildReplayList(CCupRound* _PreviousRound, CCup* _Cup);
	void 					DoCopyDrawLeg2(CCupRound& _LastCupRound, const byte _SubsSelect, const byte _SubsUse);
	void 					DoCopyTo3Rounds(CCup& _Cup, CNumericArray<ushort>& _DrawClubs, CCupRound* _NextRound1, CCupRound* _NextRound2);
	void 					DoCreateMatchListAndRelatedRounds(CCup& _Cup, CNumericArray<ushort>& _DrawClubs);
	void					DoCreateMatchList(CCup& _Cup, CNumericArray<ushort>& _DrawClubs);
    void					CreateRelatedRounds(CCupRound* ListPtr);
	void					DoDisplayDraw(CNumericArray<ushort>& _DrawClubs, CCup& _Cup, bool _IsContinentalCup);
	CCalendar				DoCreateDate(const byte _DayNumber);
    void					DoCalculateAggregateGoals(CCupRound* pLastCupRound);       // Of was a 2 leg round the {GoalsToScore[]} is given aggregate result
	void					DoDraw(CCup& pCup, CCupRound* _LastCupRound, CNumericArray<ushort>& _DrawClubs);
	void					DoBuildEventsStrList(CCupDisplayEventList& EventList);
	void					DoFindChampionsAndRunnersUp(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup);
    void					DoEndOfSeason();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	void					GetDrawClubs(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, CCup& _Cup, CCupRound* _LastCupRound);
	eCupRoundTypes			GetRoundType();							
	ushort					GetHomeTeam(const ushort _MatchNum);
	ushort					GetAwayTeam(const ushort _MatchNum);
	ushort					GetAwayReplayTeam(const int _MatchNum);
	ushort					GetHomeReplayTeam(const int _MatchNum);
	byte					GetNumInRound() const;
	byte					GetNumberNonLeagueClubsToAdd() const;
	uint					GetRoundLosers(const int _MatchNum);
    CFixture*				GetMatchDetailsPtr(const ushort MatchNum);
	CFixture*				GetClubMatchDetailsPtr(const ushort _ClubID);
	void					GetMatchDetails(CCup& _Cup, CFixture* _Fixture, CMatchInfo& _MatchDetails);
	void					GetMatchDetails(CCup& _Cup, const ushort _MatchNum, CMatchInfo& _MatchDetails);
	void					GetReplayDetails(CFixture* _Fixture, CMatchInfo& _MatchDetails);
    CFixture*				GetReplayDetailsPtr(const ushort MatchNum);
	ushort					GetRoundFixtureWinners(const ushort _MatchNum, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup, const uint _HowManyWinners = 1);
	CString&				GetTitle();
	CString&				GetReplayTitle();
	CString&				GetName();
	CString&				GetDrawName();
    CString					LongReplayDateStr();
    CString					LongMatchDateStr();
	CString					LongDrawDateStr();
	CCalendar				DrawDate();
	byte					GetMatchDate() const;
	byte					GetDrawDate();
	byte					GetReplayDate();
	int						GetReplayListSize();
	bool					HasDrawBeenMade();
	bool					HaveAllRoundMatchesBeenPlayed();
	bool					Is2Regions();
	bool 					IsClubMatchToday(const CCalendar& _CurrentDate, CCup& _Cup, const ushort _ClubID, CMatchInfo& _Match);
	bool					IsClubInRound(const ushort _ClubID);
	bool					IsExtraTimeMatch();
    bool					IsGroupMatch();
    bool					IsNeutralVenue();
	bool					IsFinal();
	bool					IsFirstLeg();
	bool					IsSecondLeg();
	bool					IsFixtureInRound(CFixture* pFixture);
	int						MatchListSize();
	CFixtureList&			GetMatchList();
	CFixtureList&			GetReplayList();
	void					MoveMatchToReplay(const ushort MatchNumber);
	void					MoveLastMatchToReplay();
	CString					NewsPaperWinnersHeadline();
	byte					NumberClubGoals(const ushort _ClubID);
	void					OutOfCupMessage(const ushort _ClubID, CString _CupTitle, bool _IsCharityShield, CClub& _WinningClub);
    void					RemoveList();
	void					SetRoundSize(const byte _NumInRound);
	void					SetNumberNonLeagueClubsToAdd(const byte nNumberNonLeagueClubsToAdd);
	bool					ToPlayAfterWeek(const byte MatchNumber);
	eMatchCompetitionType	VenueType();
	CString					WhoWonOnAwayGoalsCountDoublestr(CFixture* _Fixture, CCup& _Cup);
	bool					WasWonOnAwayGoals(CFixture* _Fixture, CCup& _Cup);
	ushort					WhoWonOnAwayGoalsCountDoubleClubID(CFixture* _Fixture, CCup& _Cup);
	CString					DrawTitleStr();
	CCupRound*				RoundToday(const CCalendar& _CurrentDate);
#ifdef CUPGROUPS
	void					DoGroup3Draw1(CCup& pCup, CCupRound* pLastCupRound, CCupRound* NextRound1Ptr, CCupRound* NextRound2Ptr);
	byte					GetNumberOfClubsInGroup() const;
	int						GetNumberOfGroups();
	int						GetNumberOfGroupMatches(void);
	void					GetGroupClubNumbers(const int _GroupNumber, CNumericArray<ushort>* _ClubIDList, CCupRound* _SecondCupRound);
#endif

	// Variables
private:
    CFixtureList			m_MatchList;
    CFixtureList			m_ReplayList;
	CStringID				m_DrawNameID;
	CStringID				m_TitleID;
    CStringID				m_NameID;
    CStringID				m_ReplayNameID;
	byte					m_NumberOfClubsInRound;
	byte					m_NumberOfClubsInGroup;
	byte					m_NumberNonLeagueClubsToAdd;
    byte					m_Type;
    byte					m_MatchDate;
    byte					m_DrawDate;
    byte					m_ReplayDate;
#ifdef	ACTIVEREFEREES
    byte					m_MinimumRefereeRating;
    byte					m_MaximumRefereeRating;
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CCupRoundList : public CExtArray
{
	// Variables
private:
	CCupRound*				m_pList;

public:
	// Constructor
    CCupRoundList()    {};
    ~CCupRoundList();

	CCupRound&              operator [] (ushort x);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
    void					DoInitialiseFromResource(CBinaryReader& _Resource);
    void					DoEndOfSeason();
	ushort					DoFindRoundNumber(CCupRound* _CupRound);
	CCupRound*				DoFindCupRoundFromFixture(CFixture* _Fixture);
	CCupRound&				GetLastRound();
	CCupRound*				GetNextRound(CCupRound* _CupRound);
	void					SetRoundSize(ushort RoundNum, const byte NumInRound);
	ushort					TotalRoundsWithReplaysExisting();
#ifdef CUPGROUPS
	void					DoBuildGroupTables(CCup& _Cup, int _GroupNumber, CLeagueTableList& _PointsTable, int _NumRounds = 0);
	ushort					FindFirstGroupMatchNumber();
	CCupRound&				DoFindFirstGroupMatch();
	ushort					NumberOfRoundsInGroupStage();
#endif
};
#endif
