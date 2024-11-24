
/*

    File:- MatchPlay.h

    Match to be viewed


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __MATCHPLAY_H__
#define __MATCHPLAY_H__

class CMatchHandler;

	enum eSubstitutionAction
	{
		eNOSUBSLEFT,
		eSWAP1SET,
		eSWAPPEDONPITCH,
		eSWAPPEDONBENCH,
		eSUBSTITUTIONMADE,
		eSUBSTITUTIONCANCELLED
	};

	enum ePlayerMatchStatus
	{
		NOSTATUS,
		YELLOWCARD,
		INJURED,
		INJUREDYELLOWCARD,
		REDCARD
	};

	enum eMatchStage
	{
		MATCHNOTSTARTED,
		MATCHINPROGRESS,
		MATCHHALFTIME,
		MATCHENDOFNORMALTIME,
		MATCHEXTRATIMEHALFTIME,
		MATCHENDOFEXTRATIME,
		MATCHREQUIREPENALTYSHOOTOUT,
		MATCHENDED
	};

	enum eSubstitutionResult
	{
		NOSUBST, 
		SUBSTMADE, 
		SWAPPEDONPITCH, 
		NOSUBSLEFT, 
		SUSPENDEDPLAYER,
		INJUREDPLAYER
	};

	class CDivision;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMatchPlayInfoStrs
{
public:

	void					Empty()
	{
		m_strHome.Empty();
		m_strAway.Empty();
	}
	CEventString& GetStr(const eWhichTeam _Who)
	{
		if (_Who == HOME)
		{
			return m_strHome;
		}
		else
		{
			return m_strAway;
		}
	}
	CEventString&			GetHomeStr()
	{
		return m_strHome;
	}

	CEventString&			GetAwayStr()
	{
		return m_strAway;
	}

protected:
	CEventString			m_strHome;
	CEventString			m_strAway;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMatchPlayInfo
{
public:
	void					Empty();
	CEventString& 			GetYellowCardInfoStr(const eWhichTeam _Who);
	CEventString& 			GetRedCardInfoStr(const eWhichTeam _Who);
	CEventString& 			GetOffTargetInfoStr(const eWhichTeam _Who);
	CEventString& 			GetOnTargetInfoStr(const eWhichTeam _Who);
	CEventString& 			GetGoalInfoStr(const eWhichTeam _Who);

    // Variables
private:
    CMatchPlayInfoStrs     	m_YellowCardInfoStr;
    CMatchPlayInfoStrs      m_RedCardInfoStr;
    CMatchPlayInfoStrs      m_OffTargetInfoStr;
    CMatchPlayInfoStrs      m_OnTargetInfoStr;
    CMatchPlayInfoStrs      m_GoalInfoStr;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFixtureMatchEvents : public CNumericArray<byte>
{
public:
	ushort					DoFindEvent(eMatchEvent _Event, eWhichTeam _Who, int _When)
	{
		return Find((byte)_Event + (byte)_Who, _When);
	}
	ushort					DoFindRandomFreeEvent()
	{
		return RandomPositionOfValue(NOEVENT);
	}

	void SetEvent(int _When, eMatchEvent _theEvent)
	{
		SetAt(_When, (int)_theEvent);
	}


private:

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMatchPlay
{
    // Variables
private:
	CMatchHandler*			m_pMatchHandler;
    uint					m_TimePlayed:8;
	uint					m_InjuryTimePlayed:8;
	bool					m_bUserSubstitutionRequired;
    CSoccerMatchPlayerList	m_HomePlayerList;
    CSoccerMatchPlayerList	m_AwayPlayerList;
	
	CFixtureMatchEvents		m_FixtureEvents;
	CPenaltyShootOut		m_PenaltyShootOut;
	CString					m_strPlayerName;
	ushort					m_LastShotTakerSquadNum;
    CPlayer*				m_pLastShotTaker;
	ushort					m_SwpSquadPlayer1;
	ushort					m_SwpSquadPlayer2;
    byte	                m_ActualPlayPercent[2];

	CTactic					m_Tactic[2];
	CMatchTactics			m_CurrentTactic;
	CMatchTactics			m_PrimaryTactic;
	bool					m_TacticsChanged;

	CMatchPlayInfo			m_MatchInfoStr;
	CMatchActionItem		m_actionInfo;

	CFixture*               m_pFixture;
	CFixture*				m_pLastFixture;
    CDivision*              m_pDivision;
    CCup*                   m_pCup;
    CCupRound*              m_pCupRound;
    CString					m_GroundName;
	CMatchScore				m_FirstLegScore;
	CMatchScore				m_HalfTimeScore;
    eWhichTeam              m_TeamCurrentPosession;
    eWhichTeam              m_WhoKicksOff;
	CClub*					m_ClubCurrentPosession;
	byte					m_TimeSinceLastChange;
	byte					m_TimeSinceLastNonUserSubstitution;
    byte					m_InjuryTime;
	eWhichTeam				m_CurrentUserVenue;
	byte				   	m_MatchActionStage;
	bool					m_SkipToEndOfMatch;
#ifdef BALL_PITCH_LOCATION
	CSoccerBallPitchLocation m_BallLocation;
#endif	
#ifdef SHOWTERRITORY
	double                  m_Territory[3];                // For Territory tab
#endif
#ifdef STORESUBSTITUTIONSINFO
	CSubstitutionsInfo		m_SubstitutionsInfo;
#endif

public:
	// Constructor
    CMatchPlay();

	bool					AreLeagueTablesAvailable();
	bool					AreLatestScoresAvailable();
	void					CalculatePenaltyShootoutResult();
	void					DoCreateEvents(const eMatchEvent _WhichEvent, const uint _HowMany);
	void					DoCreateLastShotTaker();
	void					DoInitialiseIntro(CFixture* _Fixture, CDivision* _Division, CCup* _Cup = null, CCupRound* _Round = null);
	bool					DoCheckForTied2UserMatch();
	eCornerType				DoCornerEvent(eMatchEvent _Event, eCornerType _Type);
	eFreeKickType			DoFreeKickEvent(eMatchEvent _Event, eFreeKickType _Type);
	void					DoNonUserTactics(const eWhichTeam _Who);
	eSubstitutionAction		DoSwapPlayers(const eWhichTeam _Who, const ushort _SquadNumber);
	eMatchEvent				DoGenerateInjuryTimeEvent() const;
	bool                    DoMinute();						// Returns true when match is over
	void					DoMatch(CFixture* pFixture, CString groundName);
	void					DoMatch(CFixture* pFixture, CCup* pCup, CCupRound* pCupRound, CString groundName);
	void					DoMatch(CFixture* pFixture, CCup* pCup, CCupRound* pCupRound, CFixture* pLastFixture, CString groundName);
	void					DoMatch(CFixture* pFixture, CDivision* pDivision, CCup* pCup, CCupRound* pCupRound, CFixture* pLastFixture, CString groundName);
	void					DoGoalScored();
	void					DoOffSide();
	void					DoPenaltyShootout();
	void					DoClearClickedPlayers();
	void					DoCommenceHalf();
	void					DoCalculateMatchChances();
	ushort					DoBringOffGoalkeeper(const eWhichTeam _Who);
	void					DoFoulPlayer();
	void					DoResetVariables();
	void					DoCornerKick();
	void					DoFreeKick();
	void					DoIndirectFreeKick();
	void					DoOutfieldPlayerGoalkeeper();
	void					DoUpdateOffside();
	void					DoIncreasePercentageOfPlay(eWhichTeam _Who);
	void					DoInjurePlayer();
	void					DoInitialiseLastShotTaker();
	void					DoMakeSubstitution(const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber);
	void					DoNonUserTactics();
	void					DoNormalGoalScored();
	void					DoPenalty();
	void					DoPlayMatch();
	void					DoShotMissed();
	void					DoShotOffTarget();
	void					DoShotOnTarget();
	void					DoShotSaved();
	void					DoTakePlayerOff(int _SquadNumber);
	void					DoTryBookPlayer();
	void					DoTrySendPlayerOff();
	void					DoTryScoreGoal();
	CClub&					GetClub(const eWhichTeam _Who);
	CMatchEventsStats&		GetClubData(const eWhichTeam _Who);
	CClub&					GetClubCurrentPosession();
	CClub&					GetCurrentUserControlledClub();
	CSoccerMatchPlayer*		GetFreeKickTakerSoccerMatchPlayer();
	CSoccerMatchPlayer*		GetLastShotTakerSoccerMatchPlayer();
	CMatchActionItem&		GetMatchActionInfo();
	CMatchVars&				GetMatchVars();
	eWhichTeam				GetOpponent(const eWhichTeam _Who);
	eWhichTeam				GetOpponent();
	CSoccerMatchPlayerList* GetPlayerList(const eWhichTeam _Who);
	CString&				GetPlayerName();
	CPlayer&				GetPlayer(const eWhichTeam _Who, const ushort _SquadNumber);
	CSoccerMatchPlayer*		GetPenaltyTakerSoccerMatchPlayer();
	CClub&					GetOpponentClub();
	eWhichTeam				GetCurrentUserVenue();
	CSoccerMatchPlayer*		GetCornerTakerSoccerMatchPlayer();
	ushort					GetSwapSquadPlayer1() const;
	ushort					GetSwapSquadPlayer2() const;
	bool					IsUserControlledClub(const eWhichTeam _Who);
    CString					CompetitionTitle();
	void					DumpSkills();
	void					DetermineCurrentUserVenue();
	eSubstitutionResult		OnMakeSubstitution(const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber);
	CString					FirstLegScoreStr();
	void					FreeKickTakerSquadID();
    CString					FullCompetitionTitle();
	void					OnShotOnTarget();
	void					OnShotOffTarget();
	void					OnInitialise();
    void                    InitMatch();
	void					InitialiseTeams();
	bool					IsMatchOver();
	bool					IsLeagueMatch() const;
	bool					IsUserSubstitutionRequired() const;
    void                    SetLastEvent(CString TheText);
	void					SetUserSubstitutionRequired(const bool bUserSubstitutionRequired);
    void                    SavedShot();
    void                    SavedHeader();
    void                    SkipToEndOfMatch();
	void					TryBringOnSubstitute(const eWhichTeam _Who, const ushort _OffSquadNumber = PLAYERSINTEAM + 1);
	ushort					TryBringOnSubstituteForPlayer(const eWhichTeam _Who, ushort _PlayerOffSquadNumber);
    void                    ReCalculateMatchChancesImmediately();
    int						TeamCurrentPosession();
    int		                GetTimePlayed();
    bool					HasPlayerBeenSubstituted(const eWhichTeam _Who, const ushort _SquadNumber);
	int						GetPlayerSkill(const eWhichTeam _Who, const ushort _SquadNumber, int time = -1);
    CString			        ActivePlayerName(int x);
    CString				    OpponentPlayerName(int x);

	byte					GetMatchActionStage();
	CFixture&				GetFixture();
	CDivision&				GetDivision();
	CCup&					GetCup();
	CCupRound&				GetCupRound();
	void					MotivateDefence(int type);
	void					MotivateMidfield(int type);
	void					MotivateAttack(int type);
	void					MotivatePlayer(int type, int who);
	bool					IsSecondLeg();
	bool					IsMatchDrawn();
	byte					GetOverallPlayerPerformance(const eWhichTeam eWho, int who);
	bool					DoMatchOver();
	void					DoSetMarkingStyle(const eWhichTeam _Who, const byte _Value);
	void					SetStyleOfPlay(const eWhichTeam _Who, const byte _Value);
	void					DoToggleCounterAttack(const eWhichTeam eWho);
	void					DoToggleOffsideTrap(const eWhichTeam eWho);
	void					DoSetAggression(const eWhichTeam eWho, const byte x);
	void					DoFormationChanged(const eWhichTeam eWho, const ushort nTacticsID);
	byte					PlayersWithKnocks(const eWhichTeam _Who);
	CString					PenaltyShootoutWinnerStr();

	void					FormationChanged(const byte x);
	void					SwapPlayersOnPitch(const eWhichTeam _Who, int _Player1SquadNumber, int _Player2SquadNumber);

	void					PenaltyTakerSquadID();
	void					SetMatchActionStage(const byte x);
	void					SetPlayerName(CString& _PlayerName);
	void					SetTeamCurrentPosession(eWhichTeam _Who);
	eCornerType				WasCornerDeflectedOrCleared();
	eFreeKickType			WasFreeKickDeflectedOrCleared();
	
	// get current event + info
	CMatchActionItem&		GetCurrentEvent();
	void					SetCurrentEvent(const eMatchEvent _CurrentEvent);

	// normal penalty only
	void					SetPlayerTakingMatchPenalty(const byte _SquadNumber);

	int						DoCountSubsUsed(const eWhichTeam _Who);
	bool					IsMatchInProgress();
	bool					IsSkipToEndOfMatch() const;
	CMatchPlayInfo&			GetMatchInfoStr();

#ifdef BALL_PITCH_LOCATION
	ushort					GetBallX();
	ushort					GetBallY();
	void					WeightedRandomBallYPosition();
#endif
#ifdef SHOWTERRITORY
	int						AwayTerritory();
	int						HomeTerritory();
    double					TotalTerritory() const
	{
		return m_Territory[0] + m_Territory[1] + m_Territory[2];
	}
    int				HomeAttackPossesion()
	{
		return static_cast <int>(m_Territory[HOME] / TotalTerritory()) * 100;
	}
    int				MidfieldPossesion()
	{
		return static_cast <int>(m_Territory[AWAY] / TotalTerritory()) * 100;
	}
    int				AwayAttackPossesion()
	{
		return 100 - static_cast <int>(MidfieldPossesion() + HomeAttackPossesion());
	}
#endif
#ifdef	ACTIVEREFEREES
    CString					RefereeStr();
#endif
#ifdef STORE_MAN_OF_MATCH
    CString					ManOfMatchStr();
#endif
};
/////////////////////////////////////////////////////////////////////////////

#endif
