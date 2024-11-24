
/*

    Fixture Class -


	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
 */


#ifndef __FIXTURE_H__
#define __FIXTURE_H__

#ifdef NDS
#include <SoccerDefines.h>
#endif

class CMatchVarsData;
class CMatchInfo;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CFixture
{
#ifdef NDS
	DECLARE_FDMEMORY_BINALLOC(CFixture);
#endif

	// Variables
private:
	static const int TICKETPRICEBITS = 8;
	static const int ATTENDANCEBITS = 17;
	static const int SUBSSELECTBITS = 4;
	static const int SUBSUSEBITS = 3;
	static const int MATCHLENGTHBITS = 7;
	static const int EXTRATIMEPLAYEDBITS = 1;
	static const int MATCHTYPEBITS = MATCHCOMPETITIONTYPEBITS;

	class CFixtureData
	{
	public:
    	CDate         			m_MatchDate;
    	uint          			m_TicketPrice:TICKETPRICEBITS;
		uint					m_Attendance:ATTENDANCEBITS;
		uint					m_SubsSelect:SUBSSELECTBITS;
		uint					m_SubsUse:SUBSUSEBITS;
		uint					m_MatchLength:MATCHLENGTHBITS;				// Length of match including injury time to play(is 0 if match not yet played)
		uint					m_ExtraTimePlayed:EXTRATIMEPLAYEDBITS;
		byte					m_MatchType : MATCHTYPEBITS;
	};

	CMatchEventsStats		m_ClubData[2];
	CFixtureData			m_Data;
	CMatchEventList			m_TheEvents;
#ifdef TESTBED
	CFixtureGoalAttempts	m_DBHomeGoalAttempts;
	CFixtureGoalAttempts	m_DBAwayGoalAttempts;
#endif
#ifdef STORE_MAN_OF_MATCH
	ushort					m_ManOfMatch;					// Player index number
#endif
#ifdef	ACTIVEREFEREES
	ushort					m_RefereeNum;
#endif

public:
    CFixture();
    CFixture(CMatchInfo& _Match);
    ~CFixture();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);
#ifdef NDS
	void					WriteEvents1(CDataFile *_pDataFile);
	void					WriteEvents2(CDataFile *_pDataFile);
	void					WriteEvents3(CDataFile *_pDataFile);
	void					ReadEvents1(CDataFile *_pDataFile);
	void					ReadEvents2(CDataFile *_pDataFile);
	void					ReadEvents3(CDataFile *_pDataFile);
#endif

	// Methods
	CString					AttendanceStr();
	void					AddToCurrentNonUserFixtureList(CFixtureList& _LatestScores, CDate& _Date);
	void					AddToCurrentFixtureList(CFixtureList& _LatestScores);
	void					DoAddMatchEvent(const byte _Time, const eMatchEvent _eType, CPersonName& _Name);
	void					DoAddShotsOnTarget(const eWhichTeam _Who, const byte _Num);
	ushort					DoAttackerManOfMatch(const eWhichTeam _Who);
	int						DoCountSubsUsed(const eWhichTeam _Who);
	void					DoAfterMatchUpdate(const ushort _HomePrimaryTactics, const ushort _AwayPrimaryTactics);
	uint					DoAdjustGoalAttemptsForMorale(const eWhichTeam _Who);
	int						DoAdjustGoalAttemptsForLongBallPassing(const eWhichTeam _Who, const byte _LongBallPassing);
	int						DoAdjustGoalAttemptsForPaceSupremacy(const eWhichTeam _Who);
	void					DoCalculateMatchEvents(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents);
	byte					DoCalculateGoalDifference();
	void					DoCalculateTeamStrengths(const byte _Time = 0);
	void					DoCalculateGoalAttempts(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents);
	void					DoCalculateMatchStatistics();
	void					DoCalculateTeamStrength(const eWhichTeam _Who, const byte _Time);
	void					DoCalculateDisciplinaryActions(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents);
	void					DoCalculateInjuryActions(const eWhichTeam _Who);
	eWhichTeam				DoDetermineCurrentPossesion();
	ushort					DoCreateAssister(const eWhichTeam _Who, const ushort _Scorer);
	ushort					DoCreateDefender(const eWhichTeam _Who, const ePlayerPosition _ePosition = GOALKEEPER);	// Selects d or m can be forced to select specific
	ushort					DoDefenderManOfMatch(const eWhichTeam _Who);
	ushort					DoDisciplinePlayer(const eWhichTeam _Who, const eDisciplineEvent _eDiscType, const int _When);
	void					DoShowSummary(const bool _bShowSummary, CCup* _pCup);
	void					DoShowSummary(CCup* _pCup);
	void					DoGenerateFreeKicks(const eWhichTeam _Who);
	void					DoGenerateShotsOnTarget(const eWhichTeam _Who);
	void					DoCreateDisciplinaryEvent(const byte _CurrentTime, const eMatchEvent _eEventType, const uint _ePercentChance, CFixtureMatchEvents& _TheEvents);
	void					DoCalculateDisciplinaryTotals(const eWhichTeam _Who, CMatchVarsData& _MatchVarsData);
	void					DoCalculateAttendance(const byte _WeekNum, const byte _lpos1, const byte _lpos2, const ushort _HomeNumInDiv, const ushort _AwayNumInDiv, const uint _Capacity);
	void					DoResetVariables();
    void                    DoExtraTime();
    void                    DoPenaltyShootOut();
	void					DoAdjustAggression(CClub& _Club, CMatchVarsData& _MatchVarsData, CMatchVarsData& _OpponentData);
	CPlayer*				DoCreateOffsidePlayer(const eWhichTeam _Who);
	void					DoIncrementFreeKicks(const eWhichTeam _Who);
	void					DoIncrementShotsOffTarget(const eWhichTeam _Who);
	void					DoIncrementOffsides(const eWhichTeam _Who);
	void					DoIncrementShotsOnTarget(const eWhichTeam _Who);
	bool					DoInjurePlayer(const eWhichTeam _Who);
	void					DoInitialiseMatch(CFixtureMatchEvents& _TheEvents);
	void					DoPlayMatch(bool _PenaltyShootOut = false);
	void					DoPlayerDefendAttempt(const eWhichTeam _Who, const byte _When);
	void					DoProcessMatch(CFixtureMatchEvents& _TheEvents, const uint _MatchLength);
	void					DoScoredGoal(const eWhichTeam _Who, const byte _When, const ushort _Scorer, const bool _bPenaltyFlag, const sbyte _ShotSaveSkillDifference);
	void					DoScoredFreeKickGoal(const eWhichTeam _Who, const ushort _ShotTakerSquadNumber, const byte _When);
	void					DoScoredPenaltyGoal(const eWhichTeam _Who, const byte _When);
	ushort					DoSelectShotTaker(const eWhichTeam _Who);
	bool					DoTakePenaltyShootoutShot(const eWhichTeam _Who, const ushort _ShotTaker);
	bool					DoTryScoreGoal(const eWhichTeam _Who, const ushort _ShotTaker, const byte _CurrentTime, const bool _bAllowHeaders);
	bool					DoTryScoreOwnGoal(const eWhichTeam _Who, const byte _When, const ushort _ShotTaker);
	void					DoUpdatePlayerShot(const eWhichTeam _Who, const ushort _PlayerID, const byte _When);
	ushort					DoMidfielderManOfMatch(const eWhichTeam _Who);
	void					DoUpdatePlayerPerformances(const eWhichTeam _Who);
	void					DoGoalkeeperConcededGoal(eWhichTeam _Who, sbyte _ShotSaveSkillDifference, const byte _When);
	bool					DoGoalAttempt(const byte _CurrentTime, eWhichTeam _Who);
	uint					GetAttendance() const;
	ClubID					GetAwayClubID();
	CString					GetCompetitionTitle();
	ClubID					GetHomeClubID();
	eWhichTeam				GetOpponent(const eWhichTeam _Who);
	void					GetMatchDetails(CMatchInfo& _MatchDetails);
	CMatchVars&				GetMatchVars();
	CMatchSubs&				GetMatchSubs();
	byte 					GetSubsSelect();
	byte 					GetSubsUse();
	CDate					GetMatchDate();
	eMatchCompetitionType	GetMatchType() const;
	byte					GetGoalKeeperSkill(const eWhichTeam _Who);
	byte					GetDefenderSkill(const eWhichTeam _Who);
	byte					GetMidfielderSkill(const eWhichTeam _Who);
	byte					GetAttackerSkill(const eWhichTeam _Who);
	byte					GetOffsides(const eWhichTeam _Who);
	sbyte					GetShotsOnTarget(const eWhichTeam _Who);
	sbyte					GetShotsOffTarget(const eWhichTeam _Who);
	byte 					GetCorners(const eWhichTeam _Who);
	byte 					GetFreeKicks(const eWhichTeam _Who);
	byte 					GetPercentageOfPlay(const eWhichTeam _Who);
	byte 					GetNumberOfPlayersSentOff(const eWhichTeam _Who);
	byte 					GetNumberOfPlayersBooked(const eWhichTeam _Who);
	byte					GetNumberGoals(const eWhichTeam _Who, const int _CurrentTime = 200);
	CClub&					GetClub(const eWhichTeam _Who);
	CPlayerSquad&			GetFirstTeamSquad(const eWhichTeam _Who);
	byte					GetHomeGoalsToScore();
	byte					GetAwayGoalsToScore();
	CClub*					GetLosingClub();
	CClub*					GetUserOpponentClub();
	CClub*					GetWinningClub();
	ClubID					GetWinningClubID();
	CMatchEventsStats&		GetClubData(const eWhichTeam _Who);
	uint 					GetMatchLength() const;
	uint 					GetTicketPrice() const;
	bool 					GetExtraTimeFlag() const;
	byte 					GetOverallPlayerPerformance(const eWhichTeam _Who, const ushort _PlayerSquadNumber);
	ClubID					GetLosingClubID();
	CMatchEventList*		GetEventList();
	CString&				GetClubName(const eWhichTeam _Who);
	bool					HasMatchBeenPlayed();
	CString					HasGoalBeenScored(const int _Time);
	ushort					HatTrickPlayer(const eWhichTeam _Who);
	void					OnInitialise(CMatchInfo& _Match);
	void					DoInitialiseExtraTime(const byte _ExtraTimeLength);
	bool					IsClubInvolved(const ClubID _ClubID);
	bool 					IsDerby();
	bool 					IsMatchDrawn();
	bool 					IsUserMatch();
	CString					LatestScoreStr(const uint _Time, const bool _bClubNames = true);
    CString					LosingClubName();
    CString					LosingClubUpperCaseName();
	ushort					OwnGoalScorerSquadNumber(const eWhichTeam _Who);
	CString					PenaltyShootoutWinnerStr();
	void					PlayerShotOnTarget(const eWhichTeam _Who, const ushort _PlayerID, const byte _When);
	void					PlayerShotOffTarget(const eWhichTeam _Who, const ushort _PlayerID, const byte _When);
	int						PlayersToInjure(const uint _Aggression);
	CString					RandomPlayerName(const eWhichTeam _Who, const int _Posn);
	void					SetAttendance(const uint _Value);
	void					SetCorners(const eWhichTeam _Who, const byte _Num);
	void					SetExtraTimeFlag(const bool _Played);
	void					SetFreeKicks(const eWhichTeam _Who, const byte _Num);
	void					SetHomePercentageOfPlay(const byte _HomePercentageOfPlay);
	void					SetAwayPercentageOfPlay(const byte _AwayPercentageOfPlay);
	void					SetMatchDate(CDate _MatchDate);
	void					SetMatchLength(const byte _Length);
	void					SetMatchType(const eMatchCompetitionType _MatchType);
	void					SetHomeGoalsToScore(const byte _Goals);
	void					SetAwayGoalsToScore(const byte _Goals);
	void					SetSubsSelect(const byte _SubsSelect);
	void					SetSubsUse(const byte _SubsUse);
	void					SetShotsOnTarget(const eWhichTeam _Who, const byte _Num);
	void					SetShotsOffTarget(const eWhichTeam _Who, const byte _Num);
	void					SetOffsides(const eWhichTeam _Who, const byte _Num);
	void					SetTicketPrice(const uint _Value);
	CString					StadiumName();
	byte 					TotalNumberOfPlayersSentOff();
    CString					WinningClubName();
    CString					WinningClubUpperCaseName();
#ifdef TESTBED
	CFixtureGoalAttempts&	GetHomeGoalAttempts()							{ return m_DBHomeGoalAttempts; }
	CFixtureGoalAttempts&	GetAwayGoalAttempts()							{ return m_DBAwayGoalAttempts; }
#endif
#ifdef NDS
	bool 					HasBeenSetupOrUsed() const;
	void 					ConvertMatchEventsToSummary();
	bool 					IsSummary() const;
#endif
#ifdef LATESTSCORES
	CString					GoalsStr(const int _CurrentTime);
#endif
#ifdef STOREMATCHPOSESSION
	void					DoIncrementPosession(const eWhichTeam x)		{ GetClubData(x).DoIncrementPosession(); }
	void					DoIncrementCorners(const eWhichTeam x)          { GetClubData(x).DoIncrementCorners(); }
#endif
#ifdef STOREMATCHINJURIES
	void					SetInjuries(const eWhichTeam x, const byte num);
#endif
#ifdef STORE_MAN_OF_MATCH
	const ushort			GetManOfMatch() const						{ return m_ManOfMatch; }				// Player index number#ifdef STORE_MAN_OF_MATCH
	void					DoSelectManOfMatch();
	CString					ManOfMatchStr();
#endif
#ifdef	QUICKRESULTS
	void					QuickResultGenerator();
#endif
#ifdef	ACTIVEREFEREES
	CString					RefereeStr();
	CSoccerReferee*			GetReferee();
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CFixtureList : public CPointerArray<CFixture>
{
public:
   CFixtureList() {};             // Constructor
   ~CFixtureList();               // Destructor

   CFixture&				operator[] (const ushort _nIndex);

   // IO Methods
   void  					Write(CDataFile* _pDataFile);
   void  					Read(CDataFile* _pDataFile);

	// Methods
	void					AddNew();
	void					AddNew(CMatchInfo& _Match);
	void					DoCreate(const uint _Count);
	void					DoNewSeason();
	CFixture*				FindClubFixture(const uint _FixtureNumber, const ClubID _ClubID);
	CFixture*				FindFixture(const ClubID _HomeClubID, const ClubID _AwayClubID);
    ushort                  FindID(CFixture* pFixture);
    ushort                  FindID(const ClubID _HomeClubID, const ClubID _AwayClubID);
	CString					LatestScoreStr(const uint _Time);

#ifdef LATESTSCORES
	void					BuildCurrentNonUserFixtureList(CFixtureList& _LatestScores);
	void					BuildCurrentFixtureList(CFixtureList& _LatestScores);
#endif

protected:

};
#endif
