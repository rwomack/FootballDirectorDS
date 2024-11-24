
/*
	File:- MatchPlay.cpp

	Matches to be viewed list class


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


#include "stdafx.h"

#ifndef TESTBED
#include "UI.h"
#endif
#include "FootballDirector.h"


	const int				TIMEBETWEENSUBSTITUTIONS = 8;
	const int				TIMEFIRSTSUBSTITUTIONATTEMPT = 60;
	const int				CHANCEMAKESUBSTITUTION = 35;


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::Empty
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlayInfo::Empty()
{
    m_YellowCardInfoStr.Empty();
    m_RedCardInfoStr.Empty();
    m_OffTargetInfoStr.Empty();
    m_OnTargetInfoStr.Empty();
    m_GoalInfoStr.Empty();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::GetGoalInfoStr
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CEventString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEventString& CMatchPlayInfo::GetGoalInfoStr(const eWhichTeam _Who)						
{ 
	return m_GoalInfoStr.GetStr(_Who); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::GetOnTargetInfoStr
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CEventString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEventString& CMatchPlayInfo::GetOnTargetInfoStr(const eWhichTeam _Who)					
{ 
	return m_OnTargetInfoStr.GetStr(_Who); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::GetOffTargetInfoStr
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CEventString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEventString& CMatchPlayInfo::GetOffTargetInfoStr(const eWhichTeam _Who)					
{ 
	return m_OffTargetInfoStr.GetStr(_Who); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::GetYellowCardInfoStr
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CEventString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEventString& CMatchPlayInfo::GetYellowCardInfoStr(const eWhichTeam _Who)
{ 
	return m_YellowCardInfoStr.GetStr(_Who); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlayInfo::GetRedCardInfoStr
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CEventString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEventString& CMatchPlayInfo::GetRedCardInfoStr(const eWhichTeam _Who)		
{ 
	return m_RedCardInfoStr.GetStr(_Who); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------
CMatchPlay::CMatchPlay()
{
	m_pMatchHandler = null;
	m_pFixture = null;
	OnInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCheckForTied2UserMatch
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::DoCheckForTied2UserMatch()
{
	if (IsUserControlledClub(HOME) == false || IsUserControlledClub(AWAY) == false)
	{
		return false;
	}
	if (IsMatchDrawn() == true)
	{
		if (m_TimePlayed > (FULLTIMEEXTRATIME - 5) || ((m_pDivision != null && m_pDivision->GetPointsForWin() == MLSPOINTS) && m_TimePlayed > (FULLTIME - 5)))
		{
			eMatchEvent theEvent = HOMEGOAL;
			if (RandomNumber.IntLessThan( 10 ) < 5)
			{
				theEvent = AWAYGOAL;
			}
			m_FixtureEvents.SetEvent( m_TimePlayed, theEvent);
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoClearClickedPlayers
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoClearClickedPlayers()
{
	m_SwpSquadPlayer1 = NOPLAYER;
    m_SwpSquadPlayer2 = NOPLAYER;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCommenceHalf
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoCommenceHalf()
{
	EXT_ASSERT(m_MatchActionStage != MATCHINPROGRESS);
#ifdef FMOD
	m_actionInfo.SetSoundFXID(SOUND_FULLWHST);
#endif
	m_actionInfo.DoResetVariables();
	m_TimeSinceLastChange = 0;
	m_TimeSinceLastNonUserSubstitution = 0;
	m_InjuryTimePlayed = 0;
	m_InjuryTime = 1 + RandomNumber.IntLessThan(4);
	m_MatchActionStage = MATCHINPROGRESS;
	m_actionInfo.DoKickOff(GetClub(m_WhoKicksOff).GetName());
	if (m_WhoKicksOff == HOME)
	{
		m_WhoKicksOff = AWAY;
	}
	else
	{
		m_WhoKicksOff = HOME;
	}

	m_TimePlayed++;				// Start next half
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCountSubsUsed
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchPlay::DoCountSubsUsed(const eWhichTeam _Who)
{
	return GetFixture().GetMatchSubs().DoCountSubsUsed(_Who);
}


//---------------------------------------------------------------------------------------
void CMatchPlay::DoFormationChanged(const eWhichTeam eWho, const ushort nTacticsID)
{
	m_CurrentUserVenue = eWho;
	if (m_SkipToEndOfMatch == false && m_CurrentTactic.GetTacticsID(m_CurrentUserVenue) != nTacticsID)
	{
		// If its the one used longest
		if (m_CurrentTactic.GetTime(m_CurrentUserVenue) > m_PrimaryTactic.GetTime(m_CurrentUserVenue))
		{
			m_PrimaryTactic.DoChanged(m_CurrentUserVenue, m_CurrentTactic);
		}
		m_Tactic[m_CurrentUserVenue].Copy(GetClub(m_CurrentUserVenue).GetTactics(nTacticsID));
		m_CurrentTactic.DoInitialise(m_CurrentUserVenue, nTacticsID, 0);
		m_TacticsChanged = true;
		m_TimeSinceLastChange = 3;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoGenerateInjuryTimeEvent
	Access:    	public 
	Returns:   	eMatchEvent
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eMatchEvent CMatchPlay::DoGenerateInjuryTimeEvent() const
{
	static eMatchEvent InjuryTimeEvents[] =
	{
        HOMEOFFSIDEATTEMPT, AWAYOFFSIDEATTEMPT,             // 1
        HOMEFOULPLAYERATTEMPT, AWAYFOULPLAYERATTEMPT,       // 3
        HOMEMISSATTEMPT, AWAYMISSATTEMPT,					// 5
        HOMESHOTATTEMPT, AWAYSHOTATTEMPT,                   // 7
        HOMEMISSATTEMPT, AWAYMISSATTEMPT,                   // 9
        HOMECORNERATTEMPT, AWAYCORNERATTEMPT,               // 11
	};
	const int FILLEREVENTCOUNT = 11;		// Upper bound of injury time events
	return InjuryTimeEvents[RandomNumber.IntLessThan(FILLEREVENTCOUNT)];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoIncreasePercentageOfPlay
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoIncreasePercentageOfPlay(eWhichTeam _Who)
{
	m_ActualPlayPercent[_Who]++;
	m_ActualPlayPercent[GetOpponent(_Who)]--;
#ifdef _DEBUGv3
        afxDump << "DoIncreasePercentageOfPlay " << " Home % of Play " << m_ActualPlayPercent[HOME] << " Away % of Play " << m_ActualPlayPercent[AWAY] << "\n";
#endif // !_DEBUG
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoInitialiseIntro
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CDivision * _Division
	Parameter: 	CCup * _Cup
	Parameter: 	CCupRound * _Round
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoInitialiseIntro(CFixture* _Fixture, CDivision* _Division, CCup* _Cup /*= null*/, CCupRound* _Round /*= null*/)
{
	m_pMatchHandler = null;
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	m_pFixture = _Fixture;
	OnInitialise();
	m_pDivision = _Division;
	m_pCup = _Cup;
	m_pCupRound = _Round;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CCup * _Cup
	Parameter: 	CCupRound * _CupRound
	Parameter: 	CString _StadiumName
	Returns:   	void
	Qualifier: 	
	Purpose:	Used by single leg cup match
------------------------------------------------------------------------------*/
void CMatchPlay::DoMatch(CFixture* _Fixture, CCup* _Cup, CCupRound* _CupRound, CString _StadiumName)
{
    DoMatch(_Fixture, null, _Cup, _CupRound, null, _StadiumName);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CString _StadiumName
	Returns:   	void
	Qualifier: 	
	Purpose:	Used by friendly match
------------------------------------------------------------------------------*/
void CMatchPlay::DoMatch(CFixture* _Fixture, CString _StadiumName)
{
    DoMatch(_Fixture, null, null, null, null, _StadiumName);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CCup * _Cup
	Parameter: 	CCupRound * _CupRound
	Parameter: 	CFixture * _LastFixture
	Parameter: 	CString _StadiumName
	Returns:   	void
	Qualifier: 	
	Purpose:	Used by second leg cup match
------------------------------------------------------------------------------*/
void CMatchPlay::DoMatch(CFixture* _Fixture, CCup* _Cup, CCupRound* _CupRound, CFixture* _LastFixture, CString _StadiumName)
{
    DoMatch(_Fixture, null, _Cup, _CupRound, _LastFixture, _StadiumName);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CDivision * _Division
	Parameter: 	CCup * _Cup
	Parameter: 	CCupRound * _CupRound
	Parameter: 	CFixture * _LastFixture
	Parameter: 	CString _StadiumName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoMatch(CFixture* _Fixture, CDivision* _Division, CCup* _Cup, CCupRound* _CupRound, CFixture* _LastFixture, CString _StadiumName)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
    m_pFixture = _Fixture;

	DetermineCurrentUserVenue();
    m_pDivision = _Division;
	m_pCup = _Cup;
	m_pCupRound = _CupRound;
	if (_LastFixture != null)
	{
		m_FirstLegScore.SetScores(_LastFixture->GetNumberGoals(AWAY), _LastFixture->GetNumberGoals(HOME));		//@@@ deliberately swap around
	}
	m_pLastFixture = _LastFixture;
	m_GroundName = _StadiumName;

	DoPlayMatch();
	if (_Fixture->IsUserMatch() == true)
	{
		_Fixture->DoShowSummary(true, _Cup);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::NonUserTactics
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoNonUserTactics()
{
	DoNonUserTactics(HOME);
	DoNonUserTactics(AWAY);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoNonUserTactics
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoNonUserTactics(const eWhichTeam _Who)
{
	ushort PlayerToComeOffSquadNumber = wNOTFOUND;
	// Any players with match injuries
	if (GetClub(_Who).IsUserControlled() == false)
	{
		PlayerToComeOffSquadNumber = PlayersWithKnocks(_Who);
		if (PlayerToComeOffSquadNumber < PLAYERSINTEAM && RandomNumber.IntLessThan(10) < 3)
		{
			TryBringOnSubstitute(_Who, PlayerToComeOffSquadNumber);
		}
	}
	if (m_TimePlayed > TIMEFIRSTSUBSTITUTIONATTEMPT && m_TimeSinceLastNonUserSubstitution == 0)
	{
		if (RandomNumber.IntLessThan(100) < CHANCEMAKESUBSTITUTION)
		{
			TryBringOnSubstitute(_Who);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoOffSide
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoOffSide()
{
	DoUpdateOffside();
	DoIndirectFreeKick();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoPlayMatch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoPlayMatch()
{
	CClub& HomeClub = GetClub(HOME);
	CClub& AwayClub = GetClub(AWAY);

	byte StartTactic[2];
	StartTactic[HOME] = HomeClub.GetCurrentTacticsID();		// Restore at end of match
	StartTactic[AWAY] = AwayClub.GetCurrentTacticsID();
	CPlayerSquad squadPlayers[2];
	squadPlayers[HOME] = HomeClub.GetFirstTeamSquad();
	squadPlayers[AWAY] = AwayClub.GetFirstTeamSquad();

	m_WhoKicksOff = HOME;
	if (RandomNumber.IntLessThan(100) < 50)
	{
		m_WhoKicksOff = AWAY;
	}
    DoCommenceHalf();
	DoResetVariables();

	InitialiseTeams();
	InitMatch();

    // Match view
	theApp.OnDisplayMatch(this);

	HomeClub.SetCurrentTacticsID(StartTactic[HOME]);
	AwayClub.SetCurrentTacticsID(StartTactic[AWAY]);
	HomeClub.GetFirstTeamSquad() = squadPlayers[HOME];
	AwayClub.GetFirstTeamSquad() = squadPlayers[AWAY];

	// After match update
	GetFixture().DoAfterMatchUpdate(m_PrimaryTactic.GetTacticsID(HOME), m_PrimaryTactic.GetTacticsID(AWAY));

	// Clear the data tips
	m_MatchInfoStr.Empty();
#ifdef STORESUBSTITUTIONSINFO
	m_SubstitutionsInfo.DoInitialise();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoResetVariables()
{
    m_ActualPlayPercent[HOME] = m_ActualPlayPercent[AWAY] = 50;
	m_SkipToEndOfMatch = false;
	m_TimeSinceLastChange = 0;
	m_TacticsChanged = true;		// So will calculate events
	m_actionInfo.DoResetVariables();
	m_TimePlayed = 0;
#ifdef SHOWTERRITORY
	m_Territory[0] = 0;
	m_Territory[1] = 1;
	m_Territory[2] = 0;
#endif
    m_MatchActionStage = MATCHNOTSTARTED;
	m_InjuryTimePlayed = 0;
	m_bUserSubstitutionRequired = false;
	m_LastShotTakerSquadNum = wNOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoSwapPlayers
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _SquadNumber
	Returns:   	eSubstitutionAction
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eSubstitutionAction CMatchPlay::DoSwapPlayers(const eWhichTeam _Who, const ushort _SquadNumber)
{
	eSubstitutionAction eAction = eNOSUBSLEFT;
	if (_SquadNumber >= PLAYERSINTEAM)
	{
		if (GetFixture().GetMatchSubs().AnySubstitutionsLeft(_Who) == false)
		{
			return eNOSUBSLEFT;
		}
		if (GetPlayer(_Who, _SquadNumber).GetMatchStatus() != MAXPLAYERSTATUSES)
		{
			return eNOSUBSLEFT;
		}

	}

	if (GetClub(_Who).GetPlayer(_SquadNumber).GetMatchStatus() == PLAYERSTATUS_SUSPENDED)
	{
		// Take no action if the player has been sent off
		eAction = eNOSUBSLEFT;
	}
	else if (m_SwpSquadPlayer1 != NOPLAYER && m_SwpSquadPlayer1 == _SquadNumber)
	{
		DoClearClickedPlayers();
		eAction = eSUBSTITUTIONCANCELLED;
	}
    else if (m_SwpSquadPlayer1 == NOPLAYER)
	{
        // This is first player clicked
        m_SwpSquadPlayer1 = _SquadNumber;                   // First player
		eAction = eSWAP1SET;
    }
    else if (m_SwpSquadPlayer1 != _SquadNumber)
	{
		EXT_ASSERT(m_SwpSquadPlayer1 < PLAYERSINSQUAD);
		// Not clicked on same player, this is second player clicked
		m_SwpSquadPlayer2 = _SquadNumber;                 // Second player
		if (m_SwpSquadPlayer1 < PLAYERSINTEAM && m_SwpSquadPlayer2 < PLAYERSINTEAM)
		{
			// Both players are on pitch already
			OnMakeSubstitution(_Who, m_SwpSquadPlayer1, m_SwpSquadPlayer2);
			eAction = eSWAPPEDONPITCH;
		}
		else if (m_SwpSquadPlayer1 >= PLAYERSINTEAM && m_SwpSquadPlayer2 >= PLAYERSINTEAM)
		{
			// Both players are substitutes!!
			m_SwpSquadPlayer2 = NOPLAYER;			// Ignore the latest one
			eAction = eSWAPPEDONBENCH;
		}
		else if (GetFixture().GetMatchSubs().AnySubstitutionsLeft(_Who) == true)
		{
			// Make the substitution
			OnMakeSubstitution(_Who, m_SwpSquadPlayer1, m_SwpSquadPlayer2);
			eAction = eSUBSTITUTIONMADE;
		}
		DoClearClickedPlayers();
    }
	return eAction;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetMatchActionStage
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchPlay::GetMatchActionStage()
{
	return m_MatchActionStage;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetCurrentUserControlledClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchPlay::GetCurrentUserControlledClub()
{
	return GetClub(m_CurrentUserVenue);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetCurrentUserVenue
	Access:    	public 
	Returns:   	eWhichTeam
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam  CMatchPlay::GetCurrentUserVenue()
{
	return m_CurrentUserVenue;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetOpponentClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchPlay::GetOpponentClub()
{
	return GetClub(GetOpponent());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetOpponent
	Access:    	public 
	Returns:   	eWhichTeam
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CMatchPlay::GetOpponent()
{
	eWhichTeam WhichTeam = HOME;
	if (m_CurrentUserVenue == HOME)
	{
		WhichTeam = AWAY;
	}
	return WhichTeam;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetSwapSquadPlayer1
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::GetSwapSquadPlayer1() const
{
	return m_SwpSquadPlayer1;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetSwapSquadPlayer2
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::GetSwapSquadPlayer2() const
{
	return m_SwpSquadPlayer2;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::OnInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::OnInitialise()
{
    m_pDivision = null;
	m_pCupRound = null;
	m_pCup = null;             // Set both to Null as only 1 will be active
	m_FirstLegScore.DoReset();
	if (m_pFixture != null)
	{
		GetClubData(HOME).DoResetVariables();
		GetClubData(AWAY).DoResetVariables();
	}
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsUserSubstitutionRequired
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsUserSubstitutionRequired() const
{
	return m_bUserSubstitutionRequired;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::InitialiseTeams
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::InitialiseTeams()
{
    GetFixture().DoInitialiseMatch(m_FixtureEvents);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::InitMatch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::InitMatch()
{
	DoClearClickedPlayers();

	CClub& HomeClub = GetClub(HOME);
	CClub& AwayClub = GetClub(AWAY);

	// Formation, aggression, style of play etc.
	m_Tactic[HOME].Copy(HomeClub.GetTactics());
	m_Tactic[AWAY].Copy(AwayClub.GetTactics());
	m_CurrentTactic.DoInitialise(HomeClub.GetCurrentTacticsID(), AwayClub.GetCurrentTacticsID());
	m_PrimaryTactic = m_CurrentTactic;

#ifdef BALL_PITCH_LOCATION
   // Ball position
	m_BallLocation.DoInitialise(GetFixture().GetMatchLength() + 1);
#endif

    m_HomePlayerList.OnInitialise(HomeClub, GetFixture().GetSubsSelect());
    m_AwayPlayerList.OnInitialise(AwayClub, GetFixture().GetSubsSelect());

 	GetFixture().GetMatchSubs().OnInitialise(GetFixture().GetSubsUse());
	m_TimeSinceLastNonUserSubstitution = 0;
    SetTeamCurrentPosession(HOME);
    m_TimeSinceLastChange = 0;

	ReCalculateMatchChancesImmediately();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsSecondLeg
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsSecondLeg()
{
    if (m_pCupRound !=  null)
	{
		return GetCupRound().IsSecondLeg();
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMinute
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::DoMinute()
{
	m_actionInfo.GetCommentary().Empty();
	if (m_TimePlayed != HALFTIME && m_TimePlayed != FULLTIME && m_TimePlayed != HALFTIMEEXTRATIME && m_TimePlayed != FULLTIMEEXTRATIME)
	{
		// Normal minute
		m_TimePlayed++;
		DoCheckForTied2UserMatch();
	}
	else
	{
		if (m_InjuryTime > 0)
		{
			if (m_InjuryTimePlayed == 0)
			{
				CString Commentary;
				Commentary.Format(IDS_INJURYTIME, m_InjuryTime);
				m_actionInfo.SetInformationStr(Commentary);
			}
			m_InjuryTime--;
			m_InjuryTimePlayed++;
			if (m_TimePlayed ==	FULLTIMEEXTRATIME && DoCheckForTied2UserMatch() == true)
			{
			}
			else
			{
				if (RandomNumber.IntLessThan(10) < 7)
				{
					// Use last minute of normal time as time is not incremented
					// Generate a random event, weighted towards no event
					m_FixtureEvents.SetEvent(m_TimePlayed, NOEVENT);
				}
				else
				{
					m_FixtureEvents.SetEvent(m_TimePlayed, DoGenerateInjuryTimeEvent());
					EXT_ASSERT(m_FixtureEvents[m_TimePlayed] <= AWAYCORNERATTEMPT);
				}
			}
		}
		else
		{
			// Half finished
			m_BallLocation.SetBallLocation(m_TimePlayed, PITCHCENTREX, PITCHCENTREY);
			switch(m_TimePlayed)
			{
				case HALFTIME:
					// Half time
					m_actionInfo.DoHalfTime();
					m_HalfTimeScore.SetScores(GetFixture().GetNumberGoals(HOME), GetFixture().GetNumberGoals(AWAY));
					m_MatchActionStage = MATCHHALFTIME;
					return true;
				case FULLTIME:
					// Match over
#ifdef _DEBUGv3
        afxDump << "FinalPercentageOfPlay " << " Home % of Play " << m_ActualPlayPercent[HOME] << " Away % of Play " << m_ActualPlayPercent[AWAY] << "\n";
#endif // !_DEBUG
					m_actionInfo.DoFullTime();
					m_MatchActionStage = MATCHENDOFNORMALTIME;
					// Set up extra time if neccessary
					if (IsMatchDrawn() == true)
					{
						if (m_pDivision != null && m_pDivision->GetPointsForWin() == MLSPOINTS)
						{
							// Run-ins at end of normal time if drawn in MLS
							m_MatchActionStage = MATCHREQUIREPENALTYSHOOTOUT;
							if (m_SkipToEndOfMatch == true)
							{
								m_PenaltyShootOut.DoShootout(*this);
							}
							return DoMatchOver();
						}
						else
						{
							// Initialise extra time
							GetFixture().SetMatchLength(125);
							m_FixtureEvents.SetSize(125);
#ifdef BALL_PITCH_LOCATION
							m_BallLocation.DoInitialise(GetFixture().GetMatchLength() + 1);
#endif
							///m_actionInfo.SetSize(125);
							GetFixture().DoInitialiseExtraTime(30);
							ReCalculateMatchChancesImmediately();
						}
					}
					else
					{
						return DoMatchOver();
					}
					return true;

				case HALFTIMEEXTRATIME:
					m_MatchActionStage = MATCHEXTRATIMEHALFTIME;
					return true;

				case FULLTIMEEXTRATIME:
					m_MatchActionStage = MATCHENDOFEXTRATIME;
					// Set up penalty shoot out if neccessary
					if (IsMatchDrawn() == true)
					{
						m_MatchActionStage = MATCHREQUIREPENALTYSHOOTOUT;
						m_PenaltyShootOut.DoShootout(*this);
						return true;
					}
					return DoMatchOver();
			}
		}
	}

    EXT_ASSERT(m_TimePlayed < 130);
    m_actionInfo.SetCurrentEvent((eMatchEvent)m_FixtureEvents[m_TimePlayed], HOME);
#ifdef BALL_PITCH_LOCATION
    m_BallLocation.DoCalculateBallLocation(m_TimePlayed, m_actionInfo.GetCurrentEvent());		// Default ball _Value, y if can be random then the event handler will modify them
#endif
    SetTeamCurrentPosession(m_actionInfo.TeamInPosession());
#ifdef STOREMATCHPOSESSION
	GetClubData(m_TeamCurrentPosession)->DoIncrementPosession();
#endif
	if (m_actionInfo.GetTimeSinceLastEvent() > 5 && m_actionInfo.GetCurrentEvent() == NOEVENT)
	{
		m_actionInfo.SetCurrentEvent(DoGenerateInjuryTimeEvent(), HOME);
	}

	m_bUserSubstitutionRequired = false;
	EXT_ASSERT(GetFixture().GetMidfielderSkill(HOME) > 0);
	if (m_TimePlayed == 4)
	{
		m_actionInfo.SetCurrentEvent(HOMEINJUREPLAYERATTEMPT, HOME);
	}
    switch(m_actionInfo.GetCurrentEvent())
	{
		case NOEVENT:
			SetTeamCurrentPosession(GetFixture().DoDetermineCurrentPossesion());
#ifdef BALL_PITCH_LOCATION
			WeightedRandomBallYPosition();
#endif
			GetFixture().DoPlayerDefendAttempt(m_TeamCurrentPosession, m_TimePlayed);
			DoNonUserTactics();
			break;

		case HOMEGOAL:
		case AWAYGOAL:
			DoTryScoreGoal();
			break;

		case HOMEBOOKING:
		case AWAYBOOKING:
			DoTryBookPlayer();
			break;

		case HOMESENDINGOFF:
		case AWAYSENDINGOFF:
			DoTrySendPlayerOff();
			break;

		case HOMEOFFSIDEATTEMPT:
		case AWAYOFFSIDEATTEMPT:
			DoOffSide();
			break;

		case HOMEFOULPLAYERATTEMPT:
		case AWAYFOULPLAYERATTEMPT:
			DoFoulPlayer();
			break;

		case HOMEOWNGOAL:
		case AWAYOWNGOAL:
        case HOMEPENALTYGOAL:
		case AWAYPENALTYGOAL:
			m_actionInfo.DoClearCurrentEvent();
			break;

		case HOMEINJUREPLAYERATTEMPT:
		case AWAYINJUREPLAYERATTEMPT:
			DoInjurePlayer();
			break;

		case HOMESHOTATTEMPT:
		case AWAYSHOTATTEMPT:
			OnShotOnTarget();
			break;

		case HOMEMISSATTEMPT:
		case AWAYMISSATTEMPT:
			OnShotOffTarget();
			break;

		case HOMESUBATTEMPT:
			EXT_ASSERT(m_TeamCurrentPosession == HOME);
			TryBringOnSubstitute(HOME);
			break;
		case AWAYSUBATTEMPT:
			EXT_ASSERT(m_TeamCurrentPosession == AWAY);
            TryBringOnSubstitute(AWAY);
			break;

		case HOMECORNERATTEMPT:
		case AWAYCORNERATTEMPT:
			DoCornerKick();
			break;
	}

#ifdef MOTIVATEPLAYERSINMATCH
    m_HomePlayerList.DoMinute(m_TimePlayed);
    m_AwayPlayerList.DoMinute(m_TimePlayed);
#endif

#ifdef SHOWTERRITORY
    if (m_BallLocation.GetBallXLocation(m_TimePlayed) < PITCHX1)
	{
		m_Territory[0]++;
	}
	else if (m_BallLocation.GetBallXLocation(m_TimePlayed) < PITCHX2)
	{
		m_Territory[1]++;
	}
	else
	{
		m_Territory[2]++;
	}
#endif

    GetFixture().SetHomePercentageOfPlay(m_ActualPlayPercent[HOME]);
    GetFixture().SetAwayPercentageOfPlay(m_ActualPlayPercent[AWAY]);

	m_TimeSinceLastChange -= (m_TimeSinceLastChange > 0);
	m_TimeSinceLastNonUserSubstitution -= (m_TimeSinceLastNonUserSubstitution > 0);
	m_CurrentTactic.DoIncrementTime();
	m_actionInfo.DoIncrementTime();

	DoCalculateMatchChances();
#ifdef _DEBUGv3
        afxDump << "Exiting Time " << m_TimePlayed << " " <<
			" Event " << m_actionInfo.GetCurrentEvent() <<
			" Home % of Play " << m_ActualPlayPercent[HOME] <<
			"\n";
#endif // !_DEBUG

	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMatchOver
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::DoMatchOver()
{
#ifdef FMOD
	m_actionInfo.SetSoundFXID(SOUND_FULLWHST);
#endif
	m_MatchActionStage = MATCHENDED;
#ifdef STORE_MAN_OF_MATCH
	GetFixture().DoSelectManOfMatch();
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsMatchDrawn
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsMatchDrawn()
{
    if (m_pDivision != null)
	{
		switch(m_pDivision->GetPointsForWin())
		{
			case MLSPOINTS:
			case JLEAGUEPOINTS:
				if (GetFixture().IsMatchDrawn() == true)
				{
					return true;
				}
		}
		return false;
	}
	else if (m_pCup !=  null)
	{
		return m_pCup->DoCheckIfTied(*m_pCupRound, m_pLastFixture, *m_pFixture);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::PenaltyShootoutWinnerStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchPlay::PenaltyShootoutWinnerStr()
{
	return GetFixture().PenaltyShootoutWinnerStr();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::PlayersWithKnocks
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchPlay::PlayersWithKnocks(const eWhichTeam _Who)
{
	byte bRet = 255;
	if (IsUserControlledClub(_Who) == false || m_SkipToEndOfMatch == true)
	{
		CClub& theClub = GetClub(_Who);
		for (int LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
		{
			if (theClub.GetPlayer(LoopCount).IsAvailableForMatch() == true && theClub.GetPlayer(LoopCount).InjuryMatchPerformanceEffect() > 30 && RandomNumber.IntLessThan(20) < 5)
			{
				bRet = LoopCount;
				break;
			}
		}
	}
	return bRet;
}


 //---------------------------------------------------------------------------
byte CMatchPlay::GetOverallPlayerPerformance(const eWhichTeam eWho, int who)
{
	return GetFixture().GetOverallPlayerPerformance(eWho, who);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetStyleOfPlay
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetStyleOfPlay(const eWhichTeam _Who, const byte _Value)
{
	if (m_SkipToEndOfMatch == false && m_Tactic[_Who].GetTeamTactic().GetStyleOfPlay  () != _Value)
	{
		m_Tactic[_Who].GetTeamTactic().SetStyleOfPlay(_Value);
		m_TacticsChanged = true;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoSetMarkingStyle
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoSetMarkingStyle(const eWhichTeam _Who, const byte _Value)
{
	if (m_SkipToEndOfMatch == false && m_Tactic[_Who].GetTeamTactic().GetMarkingStyle  () != _Value)
	{
		m_Tactic[_Who].GetTeamTactic().SetMarkingStyle(_Value);
		m_TacticsChanged = true;
	}
}


 //------------------------------------------------------------------------------
void CMatchPlay::DoSetAggression(const eWhichTeam eWho, const byte x)
{
	if (m_SkipToEndOfMatch == false && m_Tactic[eWho].GetTeamTactic().GetAggression  () != x)
	{
		m_Tactic[eWho].GetTeamTactic().SetAggression(x);
		m_TacticsChanged = true;
	}
}


 //------------------------------------------------------------------------------
void CMatchPlay::DoToggleCounterAttack(const eWhichTeam eWho)
{
	if (m_SkipToEndOfMatch == false)
	{
		m_Tactic[eWho].GetTeamTactic().ToggleCounterAttack();
		m_TacticsChanged = true;
	}
}


 //------------------------------------------------------------------------------
void CMatchPlay::DoToggleOffsideTrap(const eWhichTeam eWho)
{
	if (m_SkipToEndOfMatch == false)
	{
		m_Tactic[eWho].GetTeamTactic().ToggleOffsideTrap();
		m_TacticsChanged = true;
	}
}


 //------------------------------------------------------------------------------
CPlayer& CMatchPlay::GetPlayer(const eWhichTeam _Who, const ushort _SquadNumber)
{
	return GetClub(_Who).GetPlayer(_SquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TryBringOnSubstitute
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _OffSquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::TryBringOnSubstitute(const eWhichTeam _Who, const ushort _OffSquadNumber /*= PLAYERSINTEAM + 1*/)
{
	// Not if club is user controlled
	if ((IsUserControlledClub(_Who) == true && m_SkipToEndOfMatch == false) || GetFixture().GetMatchSubs().AnySubstitutionsLeft(_Who) == false)
	{
		return;
	}
	CClub& theClub = GetClub(_Who);
	ushort PlayerOffSquadNumber;
	if (_OffSquadNumber > PLAYERSINTEAM)
	{
		PlayerOffSquadNumber = theClub.GetFirstTeamSquad().DoFindWorstPlayingOutfieldPlayer(DoCountSubsUsed(_Who));
	}
	else
	{
		PlayerOffSquadNumber = _OffSquadNumber;
	}
	ushort PlayerOnSquadNumber = TryBringOnSubstituteForPlayer(_Who, PlayerOffSquadNumber);
	if (PlayerOnSquadNumber == NOPLAYER || PlayerOffSquadNumber == NOPLAYER)
	{
		// No substitution
		return;
	}

	if (theClub.GetPlayer(PlayerOffSquadNumber).GetSkills().GetPosition() !=  theClub.GetPlayer(PlayerOnSquadNumber).GetSkills().GetPosition())
	{
		// The sub is different position to player on pitch, select different player on pitch
		PlayerOffSquadNumber = theClub.GetFirstTeamSquad().DoFindWorstPlayingOutfieldPlayerPosition(DoCountSubsUsed(_Who),
																					  theClub.GetPlayer(PlayerOnSquadNumber).GetSkills().GetPosition());
	}
	if (PlayerOffSquadNumber == NOPLAYER)
	{
		// No substitution
		return;
	}
	switch (theClub.GetPlayer(PlayerOffSquadNumber).GetSkills().GetPosition())
	{
		case GOALKEEPER:
			m_actionInfo.SetCurrentEvent(HOMEGOALKEEPERSUBSTITUTED, _Who);
			break;

		case DEFENDER:
			m_actionInfo.SetCurrentEvent(HOMEDEFENDERSUBSTITUTED, _Who);
			break;

		case MIDFIELDER:
			m_actionInfo.SetCurrentEvent(HOMEMIDFIELDERSUBSTITUTED, _Who);
			break;

		case ATTACKER:
			m_actionInfo.SetCurrentEvent(HOMEATTACKERSUBSTITUTED, _Who);
			break;

		default:
			EXT_ASSERT(false);
			break;
	}
	OnMakeSubstitution(_Who, PlayerOnSquadNumber, PlayerOffSquadNumber);
	m_TimeSinceLastNonUserSubstitution = TIMEBETWEENSUBSTITUTIONS;		// Won't try again until zero
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoBringOffGoalkeeper
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::DoBringOffGoalkeeper(const eWhichTeam _Who)
{
	ushort PlayerOnSquadNumber = NOPLAYER;
	CPlayer* pPlayerOn;
	for (uint LoopCount = PLAYERSINTEAM; LoopCount < PLAYERSINTEAM + GetFixture().GetSubsSelect(); LoopCount++)
	{
		pPlayerOn = &GetClub(_Who).GetPlayer(LoopCount);
		if (GetFixture().GetMatchSubs().HasPlayerBeenSubstitutedOn(GetClub(_Who).GetFirstTeamSquad().GetSquad(LoopCount)) == false && 
            pPlayerOn->IsAvailableForMatch() == true && pPlayerOn->GetSkills().GetPosition() == GOALKEEPER)
		{
			PlayerOnSquadNumber = LoopCount;
			break;
        }
	}
	return PlayerOnSquadNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TryBringOnSubstituteForPlayer
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	ushort _PlayerOffSquadNumber
	Returns:   	ushort - squad number of player to bring on
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::TryBringOnSubstituteForPlayer(const eWhichTeam _Who, ushort _PlayerOffSquadNumber)
{
	ushort LoopCount;
	CNumericArray<ushort> SubsAvailableSquadNumbers;

	CClub& theClub = GetClub(_Who);
	CPlayer* pPlayerOff = &theClub.GetPlayer(_PlayerOffSquadNumber);
	ushort PlayerOnSquadNumber = NOPLAYER;
	ePlayerPosition PlayerOffPosition = pPlayerOff->GetSkills().GetPosition();
	if (_PlayerOffSquadNumber == 0)
	{
		PlayerOffPosition = GOALKEEPER;
	}

    // Bringing goalkeeper off?
    if (_PlayerOffSquadNumber == 0)
	{
		PlayerOnSquadNumber = DoBringOffGoalkeeper(_Who);
		if (PlayerOnSquadNumber != NOPLAYER)
		{
			return PlayerOnSquadNumber;
		}
    }

    // Build a list of all substitutes available
	CPlayer* pPlayerOn;
	for (LoopCount = PLAYERSINTEAM; LoopCount < PLAYERSINTEAM + GetFixture().GetSubsSelect(); LoopCount++)
	{
		pPlayerOn = &theClub.GetPlayer(LoopCount);
		if (GetFixture().GetMatchSubs().HasPlayerBeenSubstitutedOn(theClub.GetFirstTeamSquad().GetSquad(LoopCount)) == false && \
			pPlayerOn->IsAvailableForMatch() == true && pPlayerOn->GetSkills().GetPosition() != GOALKEEPER)
		{
			// Is ok to bring on
			SubsAvailableSquadNumbers.Add(LoopCount);
		}
	}
	if (SubsAvailableSquadNumbers.GetSize() > 0)
	{
		// only 1 sub available
		if (SubsAvailableSquadNumbers.GetSize() == 1)
		{
			PlayerOnSquadNumber = SubsAvailableSquadNumbers[0];
		}
		else
		{
			// More than 1 sub available, First try for 1 in same position
			for (LoopCount = 0; LoopCount < SubsAvailableSquadNumbers.GetSize(); LoopCount++)
			{
				if (PlayerOffPosition == theClub.GetPlayer(SubsAvailableSquadNumbers[LoopCount]).GetSkills().GetPosition())
				{
					PlayerOnSquadNumber = SubsAvailableSquadNumbers[LoopCount];
					break;
				}
			}
			if (PlayerOnSquadNumber == NOPLAYER)
			{
				// No one found in same position, defender or attacker going off find a midfielder
				if (PlayerOffPosition !=MIDFIELDER)
				{
					for (LoopCount = 0; LoopCount < SubsAvailableSquadNumbers.GetSize(); LoopCount++)
					{
						if (theClub.GetPlayer(SubsAvailableSquadNumbers[LoopCount]).GetSkills().GetPosition() ==MIDFIELDER)
						{
							PlayerOnSquadNumber = SubsAvailableSquadNumbers[LoopCount];
							break;
						}
					}
				}
				if (PlayerOnSquadNumber == NOPLAYER && pPlayerOff->IsInjured() == true)
				{
					// Still no one so select a random one
					PlayerOnSquadNumber = SubsAvailableSquadNumbers[RandomNumber.IntLessThan(SubsAvailableSquadNumbers.GetSize())];
				}
			}
		}
	}
	return PlayerOnSquadNumber;
}


// normal penalty only
//---------------------------------------------------------------------------------------
void CMatchPlay::SetPlayerTakingMatchPenalty(const byte _SquadNumber)
{
	m_LastShotTakerSquadNum = _SquadNumber;
    m_pLastShotTaker = &GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum);
	GetClubCurrentPosession().GetFirstTeamSquad().SetPenaltyTakerSquadNum(_SquadNumber);

	//SetUpLastShotTaker();
	m_actionInfo.DoScoredPenaltyGoal(m_pLastShotTaker->GetName(), GetClubCurrentPosession().GetName());
	GetFixture().DoScoredPenaltyGoal(m_TeamCurrentPosession, m_TimePlayed);
	DoGoalScored();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoPenalty
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoPenalty()
{
	PenaltyTakerSquadID();
	eWhichTeam eOpponent = GetOpponent(m_TeamCurrentPosession);
    int SaveSkill = GetClub(eOpponent).GetPlayer(0).DoCalculateEffectiveSkill(SKILL_HANDLING);
    int ShotSkill = GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).DoCalculateEffectiveSkill(SKILL_SHOOTING);
	// Try to produce an upset
    switch(GetMatchVars().GetMatchVars(HOME).GetDayType())
	{
		case GOODDAY:
			if (m_TeamCurrentPosession == HOME)
			{
				ShotSkill *= 2;
			}
			else
			{
				SaveSkill *= 2;
			}
			break;
		case BADDAY:
			if (m_TeamCurrentPosession == HOME)
			{
				ShotSkill /= 2;
			}
			else
			{
				SaveSkill /= 2;
			}
			break;
	}
    switch(GetMatchVars().GetMatchVars(AWAY).GetDayType())
	{
		case GOODDAY:
			if (m_TeamCurrentPosession == AWAY)
			{
				ShotSkill *= 2;
			}
			else
			{
				SaveSkill *= 2;
			}
			break;
		case BADDAY:
			if (m_TeamCurrentPosession == AWAY)
			{
				ShotSkill /= 2;
			}
			else
			{
				SaveSkill /= 2;
			}
			break;
	}
	// Doe's the shot succeed?
	m_actionInfo.SetGfxID(EVENTGFX_PENALTY);
	if (RandomNumber.IntLessThan(15) < 13)
	{
		// Scored
		SetCurrentEvent(HOMEPENALTYGOAL);
		if (IsUserControlledClub(m_TeamCurrentPosession) == false)
		{
			m_actionInfo.DoScoredPenaltyGoal(m_pLastShotTaker->GetName(), GetClubCurrentPosession().GetName());
			GetFixture().DoScoredPenaltyGoal(m_TeamCurrentPosession, m_TimePlayed);
			DoGoalScored();
		}
	}
	else
	{
		if (RandomNumber.IntLessThan(4) < 3)
		{
			// Saved
			m_actionInfo.DoPenaltySaved(m_pLastShotTaker->GetName(), GetClubCurrentPosession().GetName(), m_TeamCurrentPosession);
			DoShotSaved();
		}
		else
		{
			// Missed
			m_actionInfo.DoPenaltyMissed(m_pLastShotTaker->GetName(), GetClubCurrentPosession().GetName(), m_TeamCurrentPosession);
			DoShotMissed();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::ShotMissed
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoShotMissed()
{
	m_MatchInfoStr.GetOffTargetInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, m_pLastShotTaker->GetName());
	GetFixture().PlayerShotOffTarget(m_TeamCurrentPosession, m_LastShotTakerSquadNum, m_TimePlayed);
	GetLastShotTakerSoccerMatchPlayer()->IncShotsOffTarget();
#ifdef FMOD
	m_actionInfo.SetSoundFXID(SOUND_MISS);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoShotSaved
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoShotSaved()
{
	m_MatchInfoStr.GetOnTargetInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, m_pLastShotTaker->GetName());
	GetFixture().PlayerShotOnTarget(m_TeamCurrentPosession, m_LastShotTakerSquadNum, m_TimePlayed);
	GetLastShotTakerSoccerMatchPlayer()->IncShotsOnTarget();
#ifdef FMOD
	m_actionInfo.SetSoundFXID(SOUND_ROAR);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GoalScored
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoGoalScored()
{
	m_MatchInfoStr.GetGoalInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, m_pLastShotTaker->GetName());
    m_MatchInfoStr.GetOnTargetInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, m_pLastShotTaker->GetName());
	GetLastShotTakerSoccerMatchPlayer()->DoScoredGoal(m_TimePlayed);
	GetClubData(m_TeamCurrentPosession).DoIncrementShotsOnTarget();
#ifdef FMOD
	m_actionInfo.SetSoundFXID(SOUND_CROWDLRG);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TryBookPlayer
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoTryBookPlayer()
{
	ushort WhichPlayer = GetFixture().DoDisciplinePlayer(m_TeamCurrentPosession, BOOKPLAYER, m_TimePlayed);
	if (WhichPlayer == NOPLAYER)
	{
		m_actionInfo.DoClearCurrentEvent();
		return;
	}
    EXT_ASSERT(WhichPlayer < PLAYERSINTEAM);
    GetClubData(m_TeamCurrentPosession).DoIncrementFouls();

	m_MatchInfoStr.GetYellowCardInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, GetClubCurrentPosession().GetPlayer(WhichPlayer).GetName());

	m_actionInfo.DoBookPlayer(GetClubCurrentPosession().GetPlayer(WhichPlayer).GetName(), GetClubCurrentPosession().GetName());
	if (RandomNumber.IntLessThan(CHANCEBOOKINGPENALTY) == 2)
	{
		DoPenalty();

		if (WhichPlayer > 0)
		{
			m_actionInfo.DoOutfieldPlayerPenaltyEvent();
		}
		else
		{
			m_actionInfo.DoGoalkeeperPenaltyEvent();
		}
	}
	else
	{
		DoFreeKick();
		if (WhichPlayer > 0)
		{
			m_actionInfo.DoAddCurrentEvent(HOMEFREEKICKINDIRECTYELLOWOUTFIELDPLAYER - HOMEFREEKICKINDIRECT);	// Show player being booked first
		}
		else
		{
			m_actionInfo.DoAddCurrentEvent(HOMEFREEKICKINDIRECTYELLOWGOALKEEPER - HOMEFREEKICKINDIRECT);	// Show player being booked first
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TrySendPlayerOff
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoTrySendPlayerOff()
{
	ushort PlayerSquadNumber = GetFixture().DoDisciplinePlayer(m_TeamCurrentPosession, SENDPLAYEROFF, m_TimePlayed);
	if (PlayerSquadNumber == NOPLAYER || PlayerSquadNumber == 0)
	{		// No red cards for goalkeepers
		m_actionInfo.DoClearCurrentEvent();
		return;
	}

    GetClubData(m_TeamCurrentPosession).DoIncrementFouls();
	DoTakePlayerOff(PlayerSquadNumber);

	m_MatchInfoStr.GetRedCardInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, GetClubCurrentPosession().GetPlayer(PlayerSquadNumber).GetName());
	m_actionInfo.DoSendPlayerOff(GetClubCurrentPosession().GetPlayer(PlayerSquadNumber).GetName(), GetClubCurrentPosession().GetName());

	if (RandomNumber.IntLessThan(CHANCESENDINGOFFPENALTY) == 2)
	{
		DoPenalty();

		if (PlayerSquadNumber > 0)
		{
			m_actionInfo.DoOutfieldPlayerOffPenaltyEvent();
		}
		else
		{
			m_actionInfo.DoGoalkeeperOffPenaltyEvent();
		}
	}
	else
	{
		DoFreeKick();
		if (PlayerSquadNumber > 0)
		{
			m_actionInfo.DoAddCurrentEvent(HOMEFREEKICKINDIRECTREDOUTFIELDPLAYER - HOMEFREEKICKINDIRECT);	// Show player being booked first
		}
		else
		{
			m_actionInfo.DoAddCurrentEvent(HOMEFREEKICKINDIRECTREDGOALKEEPER - HOMEFREEKICKINDIRECT);	// Show player being booked first
		}
	}
    ReCalculateMatchChancesImmediately();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoTryScoreGoal
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoTryScoreGoal()
{
	CString OgStr;
	OgStr.LoadString(IDS_OG);
	CString playername;
	m_LastShotTakerSquadNum = GetFixture().OwnGoalScorerSquadNumber(GetOpponent(m_TeamCurrentPosession));

	if (GetFixture().DoTryScoreOwnGoal(m_TeamCurrentPosession, m_TimePlayed, m_LastShotTakerSquadNum) == true)
	{
		// Own goal scored
        m_MatchInfoStr.GetGoalInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, GetOpponentClub().GetPlayer(m_LastShotTakerSquadNum).GetName() + OgStr);
        m_MatchInfoStr.GetOnTargetInfoStr(m_TeamCurrentPosession).Add(m_TimePlayed, m_pLastShotTaker->GetName());
		m_actionInfo.DoOwnGoal(m_TeamCurrentPosession, GetOpponentClub().GetPlayer(m_LastShotTakerSquadNum).GetName(), GetClubCurrentPosession().GetName());
		return;
    }

	DoCreateLastShotTaker();
	playername = GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).GetName();
	CString clubname = GetClubCurrentPosession().GetName();
	if (GetFixture().DoTryScoreGoal(m_TeamCurrentPosession, m_LastShotTakerSquadNum, m_TimePlayed, true) == true)
	{
		DoNormalGoalScored();
	}
	else
	{
		SetCurrentEvent(static_cast<eMatchEvent>(HOMESHOTATTEMPTSHOT + 2 * (RandomNumber.IntLessThan(10) < 4)));
		DoShotOnTarget();
	}
#ifdef _DEBUGv3
        afxDump << "DoIncreasePercentageOfPlay TryScoreGoal\n";
#endif // !_DEBUG
	DoIncreasePercentageOfPlay(m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::WasCornerDeflectedOrCleared
	Access:    	public 
	Returns:   	eCornerType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eCornerType CMatchPlay::WasCornerDeflectedOrCleared()
{
	eCornerType eRet = CORNERTYPEDEFLECTED;
	if (RandomNumber.IntLessThan(10) < 6)
	{
		eRet = CORNERTYPECLEARED;
	}
	return eRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::WasFreeKickDeflectedOrCleared
	Access:    	public 
	Returns:   	eFreeKIckType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eFreeKickType CMatchPlay::WasFreeKickDeflectedOrCleared()
{
	eFreeKickType eRet = FREEKICKTYPEDEFLECTED;
	if (RandomNumber.IntLessThan(10) < 6)
	{
		eRet = FREEKICKTYPECLEARED;
	}
	return eRet;
}

/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::NormalGoalScored
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoNormalGoalScored()
{
	DoGoalScored();
	m_actionInfo.DoNormalGoalScored(GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).GetName(),
									GetClubCurrentPosession().GetName(), m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCreateLastShotTaker
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoCreateLastShotTaker()
{
	m_LastShotTakerSquadNum = GetFixture().DoSelectShotTaker(m_TeamCurrentPosession);
    m_pLastShotTaker = &GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetOpponent
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	eWhichTeam
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CMatchPlay::GetOpponent(const eWhichTeam _Who)
{
	return GetFixture().GetOpponent(_Who);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoUpdateOffside
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoUpdateOffside()
{
	GetClubData(m_TeamCurrentPosession).DoIncrementOffsides();
	CPlayer* pPlayer = GetFixture().DoCreateOffsidePlayer(m_TeamCurrentPosession);
	m_actionInfo.DoOffside(pPlayer->PlayerAndClubName());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::OnShotOnTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::OnShotOnTarget()
{
	SetCurrentEvent(static_cast<eMatchEvent>(HOMESHOTATTEMPTSHOT + 2 * (RandomNumber.IntLessThan(10) < 4)));
	DoShotOnTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::OnShotOffTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::OnShotOffTarget()
{
    SetCurrentEvent(HOMEMISSATTEMPTSHOT);
	DoShotOffTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetCurrentEvent
	Access:    	public 
	Parameter: 	const eMatchEvent _CurrentEvent
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetCurrentEvent(const eMatchEvent _CurrentEvent)
{
	m_actionInfo.SetCurrentEvent(_CurrentEvent, m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCornerKick
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoCornerKick()
{
	GetClubData(m_TeamCurrentPosession).DoIncrementCorners();
	m_actionInfo.DoCorner(GetFixture().GetClubName(m_TeamCurrentPosession));

    CNumericArray<byte> nCornerPercentChanceEvent;
    nCornerPercentChanceEvent.Add(CORNERDEFLECTED);
    nCornerPercentChanceEvent.Add(CORNERDEFLECTED + CORNERCLEARED);
    nCornerPercentChanceEvent.Add(CORNERDEFLECTED + CORNERCLEARED + CORNEROFFTARGET);
    nCornerPercentChanceEvent.Add(CORNERDEFLECTED + CORNERCLEARED + CORNEROFFTARGET + CORNERONTARGET);
    nCornerPercentChanceEvent.Add(CORNERDEFLECTED + CORNERCLEARED + CORNEROFFTARGET + CORNERONTARGET + CORNERGOAL);
	byte CornerType = nCornerPercentChanceEvent.NearestAbove((byte)RandomNumber.IntLessThan(100));

    // Ensure it's ok for the end result to happen
    switch(CornerType)
	{
        case CORNERTYPEDEFLECTED:
        case CORNERTYPECLEARED:
            break;

        case CORNERTYPEOFFTARGET:
    		CornerType = DoCornerEvent(HOMEMISSATTEMPT, CORNERTYPEOFFTARGET);
            break;

        case CORNERTYPEONTARGET:
            CornerType = DoCornerEvent(HOMESHOTATTEMPT, CORNERTYPEONTARGET);
            break;
        case CORNERTYPEGOAL:
            CornerType = DoCornerEvent(HOMEGOAL, CORNERTYPEGOAL);
            break;
    }

    // Do it
	bool IsBottomCorner;
	if (RandomNumber.IntLessThan(10) < 5)
	{
		IsBottomCorner = false;
#ifdef BALL_PITCH_LOCATION
		m_BallLocation.SetBallLocationY(m_TimePlayed, 0);
#endif
	}
	else
	{
		IsBottomCorner = true;
#ifdef BALL_PITCH_LOCATION
		m_BallLocation.SetBallLocationY(m_TimePlayed, MAXPITCHY);
#endif
	}
    switch(CornerType)
	{
        case CORNERTYPEDEFLECTED:
			if (IsBottomCorner == false)
			{
				SetCurrentEvent(HOMEBOTTOMCORNERDEFLECTED);
			}
			else
			{
				SetCurrentEvent(HOMETOPCORNERDEFLECTED);
			}
            break;

		case CORNERTYPECLEARED:
			if (IsBottomCorner == false)
			{
				SetCurrentEvent(HOMETOPCORNERCLEARED);
			}
			else
			{
				SetCurrentEvent(HOMEBOTTOMCORNERCLEARED);
			}
            break;

		case CORNERTYPEOFFTARGET:
			if (IsBottomCorner == false)
			{
				SetCurrentEvent(HOMETOPCORNEROFFTARGET);
			}
			else
			{
				SetCurrentEvent(HOMEBOTTOMCORNEROFFTARGET);
			}
			DoShotOffTarget();
            break;

		case CORNERTYPEONTARGET:
			DoCreateLastShotTaker();
			DoShotOnTarget();
            break;

        case CORNERTYPEGOAL:
			DoCreateLastShotTaker();
			if (GetFixture().DoTryScoreGoal(m_TeamCurrentPosession, m_LastShotTakerSquadNum, m_TimePlayed, true) == true)
			{
				// Scored
				DoGoalScored();
                m_actionInfo.DoScoredGoalFromCorner(m_TeamCurrentPosession, IsBottomCorner, 
					GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).GetName(), GetClubCurrentPosession().GetName());
			}
			else
			{
				// Goalkeeper saved
				DoShotSaved();
				if (IsBottomCorner == false)
				{
					m_actionInfo.SetCurrentEvent(HOMETOPCORNERONTARGET, m_TeamCurrentPosession);
				}
				else
				{
					m_actionInfo.SetCurrentEvent(HOMEBOTTOMCORNERONTARGET, m_TeamCurrentPosession);
				}
			}
            break;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCornerEvent
	Access:    	public 
	Parameter: 	eMatchEvent _Event
	Parameter: 	eCornerType _Type
	Returns:   	eCornerType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eCornerType CMatchPlay::DoCornerEvent(eMatchEvent _Event, eCornerType _Type)
{
	eCornerType CornerType = _Type;
	ushort Where = m_FixtureEvents.DoFindEvent(_Event, m_TeamCurrentPosession, m_TimePlayed);
	if (Where == wNOTFOUND)
	{
		// None to come so do either deflected or cleared
		CornerType = WasCornerDeflectedOrCleared();
	}
	else
	{
		m_FixtureEvents.SetEvent(Where, NOEVENT);
	}
	return CornerType;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoFreeKickEvent
	Access:    	public 
	Parameter: 	eMatchEvent _Event
	Parameter: 	eFreeKickType _Type
	Returns:   	eFreeKickType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eFreeKickType CMatchPlay::DoFreeKickEvent(eMatchEvent _Event, eFreeKickType _Type)
{
	eFreeKickType FreeKickType = _Type;
	ushort Where = m_FixtureEvents.DoFindEvent(_Event, m_TeamCurrentPosession, m_TimePlayed);
	if (Where == wNOTFOUND)
	{
		// None to come so do either deflected or cleared
		FreeKickType = WasFreeKickDeflectedOrCleared();
	}
	else
	{
		m_FixtureEvents.SetEvent(Where, NOEVENT);
	}
	return FreeKickType;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoFoulPlayer
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoFoulPlayer()
{
    GetClubData(m_TeamCurrentPosession).DoIncrementFouls();
	DoIndirectFreeKick();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoIndirectFreeKick
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoIndirectFreeKick()
{
	SetTeamCurrentPosession(GetOpponent(m_TeamCurrentPosession));
    GetClubData(m_TeamCurrentPosession).DoIncrementFreeKicks();
	FreeKickTakerSquadID();
	DoCreateLastShotTaker();

	m_actionInfo.DoFreeKick(GetFixture().GetClubName(m_TeamCurrentPosession));
#ifdef BALL_PITCH_LOCATION
	WeightedRandomBallYPosition();
	m_BallLocation.DoLocateX(m_TimePlayed, RandomNumber.IntLessThan(MAXPITCHX));
#endif
	SetCurrentEvent(HOMEFREEKICKINDIRECT);
	DoIncreasePercentageOfPlay(m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoFreeKick
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoFreeKick()
{
	SetTeamCurrentPosession(GetOpponent(m_TeamCurrentPosession));
    GetClubData(m_TeamCurrentPosession).DoIncrementFreeKicks();
#ifdef BALL_PITCH_LOCATION
	WeightedRandomBallYPosition();
#endif
    CNumericArray<byte> nFreeKickPercentChanceEvent;
	nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT);
    nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT + FREEKICKDEFLECTED);
    nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT + FREEKICKDEFLECTED + FREEKICKCLEARED);
    nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT + FREEKICKDEFLECTED + FREEKICKCLEARED + FREEKICKOFFTARGET);
    nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT + FREEKICKDEFLECTED + FREEKICKCLEARED + FREEKICKOFFTARGET + FREEKICKONTARGET);
    nFreeKickPercentChanceEvent.Add(FREEKICKINDIRECT + FREEKICKDEFLECTED + FREEKICKCLEARED + FREEKICKOFFTARGET + FREEKICKONTARGET + FREEKICKGOAL);
    byte FreeKickType = nFreeKickPercentChanceEvent.NearestAbove((byte)RandomNumber.IntLessThan(100));

    // Ensure it's ok for the end result to happen
    switch(FreeKickType)
	{
        case FREEKICKTYPEINDIRECT:
        case FREEKICKTYPEDEFLECTED:
        case FREEKICKTYPECLEARED:
			// Always legal
            break;

		case FREEKICKTYPEOFFTARGET:
			FreeKickType = DoFreeKickEvent(HOMEMISSATTEMPT, FREEKICKTYPEOFFTARGET);
            break;

		case FREEKICKTYPEONTARGET:
			FreeKickType = DoFreeKickEvent(HOMESHOTATTEMPT, FREEKICKTYPEONTARGET);
            break;

		case FREEKICKTYPEGOAL:
			FreeKickType = DoFreeKickEvent(HOMEGOAL, FREEKICKTYPEGOAL);
            break;
    }

    // Do it
	FreeKickTakerSquadID();
	DoCreateLastShotTaker();

	m_actionInfo.DoFreeKick(GetFixture().GetClubName(m_TeamCurrentPosession));

    switch(FreeKickType)
	{
        case FREEKICKTYPEINDIRECT:
#ifdef BALL_PITCH_LOCATION
			m_BallLocation.DoLocateX(m_TimePlayed, RandomNumber.IntLessThan(MAXPITCHX));
#endif
            SetCurrentEvent(HOMEFREEKICKINDIRECT);
            break;

        case FREEKICKTYPEDEFLECTED:
            SetCurrentEvent(HOMEFREEKICKDEFLECTED);
            break;

        case FREEKICKTYPECLEARED:
            SetCurrentEvent(HOMEFREEKICKCLEARED);
            break;

        case FREEKICKTYPEOFFTARGET:
            SetCurrentEvent(HOMEFREEKICKOFFTARGET);
			DoShotMissed();
            break;

        case FREEKICKTYPEONTARGET:
			DoShotSaved();
			m_actionInfo.DoFreeKickOnTarget(m_TeamCurrentPosession);
            break;

        case FREEKICKTYPEGOAL:
			GetFreeKickTakerSoccerMatchPlayer()->DoScoredGoal(m_TimePlayed);
			DoGoalScored();
			GetFixture().DoScoredFreeKickGoal(m_TeamCurrentPosession, m_LastShotTakerSquadNum, m_TimePlayed);
			m_actionInfo.DoFreekickGoal(m_pLastShotTaker->GetName(), GetClubCurrentPosession().GetName(), m_TeamCurrentPosession);
            break;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::FreeKickTakerSquadID
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::FreeKickTakerSquadID()
{
	m_LastShotTakerSquadNum = GetClubCurrentPosession().GetFirstTeamSquad().GetFreeKickTakerSquadNum();
	DoInitialiseLastShotTaker();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetUpLastShotTaker
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoInitialiseLastShotTaker()
{
	EXT_ASSERT(m_LastShotTakerSquadNum != wNOTFOUND);
    m_pLastShotTaker = &GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::PenaltyTakerSquadID
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::PenaltyTakerSquadID()
{
	m_LastShotTakerSquadNum = GetClubCurrentPosession().GetFirstTeamSquad().GetPenaltyTakerSquadNum();
	DoInitialiseLastShotTaker();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetPlayerList
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CSoccerMatchPlayerList*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayerList* CMatchPlay::GetPlayerList(const eWhichTeam _Who)
{
	if (_Who == HOME)
	{
		return &m_HomePlayerList;
	}
	else
	{
		return &m_AwayPlayerList;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetFreeKickTakerSoccerMatchPlayer
	Access:    	public 
	Returns:   	CSoccerMatchPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayer* CMatchPlay::GetFreeKickTakerSoccerMatchPlayer()
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_TeamCurrentPosession)->GetAt(GetClubCurrentPosession().GetFirstTeamSquad().GetFreeKickTakerSquadNum()));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetPenaltyTakerSoccerMatchPlayer
	Access:    	public 
	Returns:   	CSoccerMatchPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayer* CMatchPlay::GetPenaltyTakerSoccerMatchPlayer()
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_TeamCurrentPosession)->GetAt(GetClubCurrentPosession().GetFirstTeamSquad().GetPenaltyTakerSquadNum()));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetCornerTakerSoccerMatchPlayer
	Access:    	public 
	Returns:   	CSoccerMatchPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayer* CMatchPlay::GetCornerTakerSoccerMatchPlayer()
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_TeamCurrentPosession)->GetAt(GetClubCurrentPosession().GetFirstTeamSquad().GetCornerTakerSquadNum()));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetLastShotTakerSoccerMatchPlayer
	Access:    	public 
	Returns:   	CSoccerMatchPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayer* CMatchPlay::GetLastShotTakerSoccerMatchPlayer()
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_TeamCurrentPosession)->GetAt(m_LastShotTakerSquadNum));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoShotOffTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoShotOffTarget()
{
	DoCreateLastShotTaker();
	DoShotMissed();
	CString& PlayerName = GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).GetName();
	m_actionInfo.DoShotOffTarget(PlayerName, GetClubCurrentPosession().GetName());

#ifdef _DEBUGv3
        afxDump << "DoIncreasePercentageOfPlay Shot Off Target\n";
#endif // !_DEBUG
	DoIncreasePercentageOfPlay(m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoShotOnTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoShotOnTarget()
{
	DoCreateLastShotTaker();
	DoShotSaved();
	m_actionInfo.DoShotOnTarget(m_TeamCurrentPosession, GetClubCurrentPosession().GetPlayer(m_LastShotTakerSquadNum).GetName(), 
		GetClubCurrentPosession().GetName());
#ifdef _DEBUGv3
        afxDump << "DoIncreasePercentageOfPlay Shot On Target\n";
#endif // !_DEBUG
	DoIncreasePercentageOfPlay(m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCalculateMatchChances
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoCalculateMatchChances()
{
	EXT_ASSERT_POINTER(m_pFixture, CFixture);
	if (m_TimeSinceLastChange != 0 || m_TacticsChanged == false || m_TimePlayed >= m_FixtureEvents.GetUpperBound())
	{
		return;
	}

	CMatchEventsStats& HomeStats = GetClubData(HOME);
	CMatchEventsStats& AwayStats = GetClubData(AWAY);

	// Save the current stats
	CMatchEventsStats HomeSaveStats;
	CMatchEventsStats AwaySaveStats;
	HomeSaveStats.CopyStats(HomeStats);
	AwaySaveStats.CopyStats(AwayStats);

	m_TacticsChanged = false;
	m_TimeSinceLastChange = 5;

	GetMatchVars().DoInitialiseFromTactics(&m_Tactic[HOME], HOME);
	GetMatchVars().DoInitialiseFromTactics(&m_Tactic[AWAY], AWAY);
	GetFixture().DoCalculateTeamStrength(HOME, m_TimePlayed);
	GetFixture().DoCalculateTeamStrength(AWAY, m_TimePlayed);
    GetFixture().DoCalculateMatchEvents(m_TimePlayed, m_FixtureEvents);		// Sets goal and discipline event times
    GetFixture().DoCalculateMatchStatistics();
	EXT_ASSERT(GetFixture().GetMidfielderSkill(HOME) > 0);

	EXT_ASSERT(m_FixtureEvents.CountInstances(HOMEGOAL) < 10);
	EXT_ASSERT(m_FixtureEvents.CountInstances(AWAYGOAL) < 10);
	DoCreateEvents(HOMESHOTATTEMPT, HomeStats.GetShotsOnTarget() /*- m_FixtureEvents.CountInstances(HOMEGOAL)*/);
	DoCreateEvents(AWAYSHOTATTEMPT, AwayStats.GetShotsOnTarget() /*- m_FixtureEvents.CountInstances(AWAYGOAL)*/);
	DoCreateEvents(HOMEMISSATTEMPT, HomeStats.GetShotsOffTarget());
	DoCreateEvents(AWAYMISSATTEMPT , AwayStats.GetShotsOffTarget());
	DoCreateEvents(HOMECORNERATTEMPT, HomeStats.GetCorners());
	DoCreateEvents(AWAYCORNERATTEMPT, AwayStats.GetCorners());
	DoCreateEvents(HOMEOFFSIDEATTEMPT, HomeStats.GetOffsides());
	DoCreateEvents(AWAYOFFSIDEATTEMPT, AwayStats.GetOffsides());
	DoCreateEvents(HOMEINJUREPLAYERATTEMPT, GetFixture().PlayersToInjure(GetMatchVars().GetTeamAggression(AWAY)));
	DoCreateEvents(AWAYINJUREPLAYERATTEMPT, GetFixture().PlayersToInjure(GetMatchVars().GetTeamAggression(HOME)));

	// Restore the current stats
	HomeStats.CopyStats(HomeSaveStats);
	AwayStats.CopyStats(AwaySaveStats);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoCreateEvents
	Access:    	public 
	Parameter: 	const eMatchEvent _WhichEvent
	Parameter: 	const uint _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoCreateEvents(const eMatchEvent _WhichEvent, const uint _HowMany)
{
	EXT_ASSERT(_HowMany < 50); // Improbable number of events requested
	uint time;
    for (uint LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		// Generate random event time
		if (RandomNumber.IntLessThan(m_FixtureEvents.GetUpperBound()) > m_TimePlayed)
		{
			time = m_FixtureEvents.DoFindRandomFreeEvent();
			if (time < 5 && (_WhichEvent == HOMEINJUREPLAYERATTEMPT || _WhichEvent == AWAYINJUREPLAYERATTEMPT))
			{
			}
			else if (time > m_TimePlayed)
			{
				m_FixtureEvents.SetEvent(time, _WhichEvent);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TakePlayerOff
	Access:    	public 
	Parameter: 	int PlayerOff
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoTakePlayerOff(int _SquadNumber)
{
	GetMatchVars().DoTakePlayerOff(m_TimePlayed, m_TeamCurrentPosession, _SquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoInjurePlayer
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoInjurePlayer()
{
	if (GetFixture().DoInjurePlayer(m_TeamCurrentPosession) == false)
	{
		m_actionInfo.DoClearCurrentEvent();
	}
    else
    {
		CPlayer* pPlayerOn;
		ushort PlayerSquadNumber;
		ushort SubSquadNumber;
#ifdef _DEBUGv3
        afxDump << "DoIncreasePercentageOfPlay Injure Player\n";
#endif // !_DEBUG
		DoIncreasePercentageOfPlay(m_TeamCurrentPosession);
        m_actionInfo.SetGfxID(EVENTGFX_INJURY);
		PlayerSquadNumber = GetMatchVars().GetLastInjuredPlayerSquadNumber();
		CPlayer* pPlayerOff = &GetClubCurrentPosession().GetPlayer(PlayerSquadNumber);
		SetPlayerName(pPlayerOff->GetName());

		// 100% injured or heavily injured and non user team
        if (pPlayerOff->InjuryMatchPerformanceEffect() == 100 || (pPlayerOff->InjuryMatchPerformanceEffect() > 40 && GetClubCurrentPosession().IsUserControlled() != true))
		{
			// Zero the player's performance for the rest of the match
			pPlayerOff->SetInjuryMatchPerformanceEffect(100);		// Fully inured
			DoTakePlayerOff(PlayerSquadNumber);
			m_actionInfo.DoTakeInjuredPlayerOff(m_TeamCurrentPosession, PlayerSquadNumber, pPlayerOff->InjuryCarriedOff(), GetPlayerName(), GetClubCurrentPosession().GetName());

            if (GetClubCurrentPosession().IsUserControlled() == false || m_SkipToEndOfMatch == true)
			{
                // Bring a replacement player on
				if (GetFixture().GetMatchSubs().AnySubstitutionsLeft(m_TeamCurrentPosession) == true)
				{
                    SubSquadNumber = TryBringOnSubstituteForPlayer(m_TeamCurrentPosession, PlayerSquadNumber);
					if (SubSquadNumber < PLAYERSINSQUAD)
					{
						pPlayerOn = &GetClubCurrentPosession().GetPlayer(SubSquadNumber);
						OnMakeSubstitution(m_TeamCurrentPosession, SubSquadNumber, PlayerSquadNumber);
						CString str1 = GetPlayerName();
						CString str2 = pPlayerOn->GetName();
						m_actionInfo.DoBringSubOnAfterInjury(str1, GetClubCurrentPosession().GetName(), str2);
                    }
					else
					{
						// All subs have been used so just bring player off
						m_actionInfo.DoNoSubOnAfterInjury(GetPlayerName(), GetClubCurrentPosession().GetName(), GetClubCurrentPosession().DoCountPlayersOnPitch());
                        if (PlayerSquadNumber == 0)
						{
							DoOutfieldPlayerGoalkeeper();
						}
					}
				}
				else
				{
					// All subs have been used so just bring player off
					m_actionInfo.DoNoSubOnAfterInjury(GetPlayerName(), GetClubCurrentPosession().GetName(), GetClubCurrentPosession().DoCountPlayersOnPitch());
                    if (PlayerSquadNumber == 0)
					{
						DoOutfieldPlayerGoalkeeper();
					}
				}
			}
			else
			{
				EXT_ASSERT(GetClubCurrentPosession().IsUserControlled() == true);
				m_bUserSubstitutionRequired = true;
				m_actionInfo.DoSubstituteUserInjuredPlayer(pPlayerOff->GetName(), GetClubCurrentPosession().GetName());
            }
        }
		else
		{
            // Able to continue
			m_actionInfo.DoInjuredPlayerOK(pPlayerOff->GetName(), GetClubCurrentPosession().GetName(), (PlayerSquadNumber > 0), m_TeamCurrentPosession);
        }
        ReCalculateMatchChancesImmediately();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoOutfieldPlayerGoalkeeper
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoOutfieldPlayerGoalkeeper()
{
	ushort nSubSquadNumber;

	// Goalkeeper, place a random player in goal
	do
	{
		nSubSquadNumber = 1 + RandomNumber.IntLessThan(5);
	}
	while(GetClubCurrentPosession().GetPlayer(nSubSquadNumber).IsAvailableForMatch() == false);
	SwapPlayersOnPitch(m_TeamCurrentPosession, 0, nSubSquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::HasPlayerBeenSubstituted
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _SquadNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::HasPlayerBeenSubstituted(const eWhichTeam _Who, const ushort _SquadNumber)
{
	return GetFixture().GetMatchSubs().HasPlayerBeenSubstitutedOn(GetClub(_Who).GetFirstTeamSquad().GetSquad(_SquadNumber));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::CompetitionTitle
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchPlay::CompetitionTitle()
{
	CString sRet;
    if (m_pDivision != null)
	{
        return m_pDivision->GetName();
    }
    if (m_pCup !=  null)
	{
        return m_pCup->GetName();
    }
	else
	{
		sRet.LoadString(IDS_FRIENDLYMATCH);
		return sRet;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsLeagueMatch
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsLeagueMatch() const
{
    if (m_pDivision != null)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::FullCompetitionTitle
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchPlay::FullCompetitionTitle()
{
	CString sRet;
    if (m_pDivision != null)
	{
        return m_pDivision->GetName();
    }
    if (m_pCup !=  null)
	{
		return m_pCup->CupAndRoundName(*m_pCupRound);
    }
	else
	{
		sRet.LoadString(IDS_FRIENDLYMATCH);
		return sRet;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::AreLeagueTablesAvailable
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::AreLeagueTablesAvailable()
{
	if (m_pDivision != null)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::AreLatestScoresAvailable
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::AreLatestScoresAvailable()
{
	if (m_pCupRound != NULL)
	{
		CFixtureList LatestScores;
		m_pCupRound->BuildCurrentNonUserFixtureList(LatestScores);
		if (LatestScores.GetSize() > 1) 
		{
			return true;
		}
	}
	if (m_pDivision != null)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsMatchInProgress
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsMatchInProgress()
{
	if (m_MatchActionStage == MATCHNOTSTARTED || m_MatchActionStage == MATCHINPROGRESS || 
		m_MatchActionStage == MATCHHALFTIME || m_MatchActionStage == MATCHENDOFNORMALTIME ||
		m_MatchActionStage == MATCHEXTRATIMEHALFTIME)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SkipToEndOfMatch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SkipToEndOfMatch()
{
	if (IsMatchInProgress() == true)
	{
		m_SkipToEndOfMatch = true;
		while(DoMinute() == true)
		{
			if (m_MatchActionStage == MATCHHALFTIME ||
				m_MatchActionStage == MATCHEXTRATIMEHALFTIME ||
				m_MatchActionStage == MATCHENDOFNORMALTIME)
			{
				DoCommenceHalf();
			}
	   }
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetClubCurrentPosession
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchPlay::GetClubCurrentPosession()
{
	return *m_ClubCurrentPosession;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetTeamCurrentPosession
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetTeamCurrentPosession(eWhichTeam _Who)
{
	m_TeamCurrentPosession = _Who;
	m_ClubCurrentPosession = &GetClub(m_TeamCurrentPosession);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DetermineCurrentUserVenue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DetermineCurrentUserVenue()
{
	if (WorldData().GetCurrentUserClub() == &GetClub(HOME))
	{
		m_CurrentUserVenue = HOME;
	}
	else
	{
		m_CurrentUserVenue = AWAY;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::ActivePlayerName
	Access:    	public 
	Parameter: 	int x
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchPlay::ActivePlayerName(int x)
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_CurrentUserVenue)->GetAt(x))->PlayerName();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::OpponentPlayerName
	Access:    	public 
	Parameter: 	int x
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchPlay::OpponentPlayerName(int x)
{
    return static_cast<CSoccerMatchPlayer*>(GetPlayerList(m_CurrentUserVenue)->GetAt(x))->PlayerName();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SwapPlayersOnPitch
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	int _Player1SquadNumber
	Parameter: 	int _Player2SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SwapPlayersOnPitch(const eWhichTeam _Who, int _Player1SquadNumber, int _Player2SquadNumber)
{
	GetMatchVars().SwapPlayersOnPitch(_Who, _Player1SquadNumber, _Player2SquadNumber);
	GetPlayerList(_Who)->MakeSubstitution(m_TimePlayed, _Player1SquadNumber, _Player2SquadNumber);
	GetClub(_Who).GetFirstTeamSquad().DoSwapPlayers(_Player1SquadNumber, _Player2SquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsMatchOver
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsMatchOver()
{
	if (m_MatchActionStage == MATCHENDED)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::OnMakeSubstitution
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	ushort _Player1SquadNumber
	Parameter: 	ushort _Player2SquadNumber
	Returns:   	eSubstitutionResult - 0 none, 1 substitution, 2 swapped on pitch
	Qualifier: 	
	Purpose:	Bring a player off
------------------------------------------------------------------------------*/
eSubstitutionResult CMatchPlay::OnMakeSubstitution(const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber)
{
	eSubstitutionResult iRet = NOSUBST;
    if (IsMatchOver() == true)
	{
		// Not if match over
		return iRet;
	}

	CString str;
	str.Format(IDS_PLAYCOMINGON, m_TimePlayed, GetClub(_Who).GetPlayer(_Player1SquadNumber).GetName(), GetClub(_Who).GetPlayer(_Player2SquadNumber).GetName());
    if (_Player1SquadNumber < PLAYERSINTEAM && _Player2SquadNumber < PLAYERSINTEAM)
	{
        // Both players are in the team
        if (GetClub(_Who).GetPlayer(_Player1SquadNumber).IsAvailableForMatch() == true && \
            GetClub(_Who).GetPlayer(_Player2SquadNumber).IsAvailableForMatch() == true)
		{
            // Both still playing so swap positions on pitch
			SwapPlayersOnPitch(_Who, _Player1SquadNumber, _Player2SquadNumber);
		}
        ReCalculateMatchChancesImmediately();
		return SWAPPEDONPITCH;
    }

    // Ensure player 2 is sub, player 1 in team
    if (_Player1SquadNumber > PLAYERSINTEAM - 1 && _Player2SquadNumber < PLAYERSINTEAM)
	{
		// Swap around
		ushort SwapPl = _Player1SquadNumber;
        _Player1SquadNumber = _Player2SquadNumber;
        _Player2SquadNumber = SwapPl;
    }

	ushort Player1ListId = GetClub(_Who).GetFirstTeamSquad().GetSquad(_Player1SquadNumber);
	CPlayer& thePlayer = GetClub(_Who).GetPlayer(_Player1SquadNumber);
	// Check not replacing a sent off player
	if (thePlayer.GetMatchStatus() == PLAYERSTATUS_SUSPENDED)
	{
		return SUSPENDEDPLAYER;
	}

	// Check not bringing a player back on
	if (GetFixture().GetMatchSubs().HasPlayerBeenSubstitutedOn(Player1ListId) == false)
	{
		ePlayerPosition PlayerOffPosition = thePlayer.GetSkills().GetPosition();
		// Check if outfield player is goalkeeper
		if (_Player1SquadNumber == 0)
		{
			PlayerOffPosition = GOALKEEPER;
		}
		if (GetFixture().GetMatchSubs().MakeSubstitution(m_TimePlayed, _Who, GetClub(_Who).GetFirstTeamSquad().GetSquad(_Player2SquadNumber), Player1ListId) == true)
		{
			iRet = SUBSTMADE;
			DoMakeSubstitution(_Who, _Player1SquadNumber, _Player2SquadNumber);
		}
	}
   	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::DoMakeSubstitution
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	ushort _Player1SquadNumber
	Parameter: 	ushort _Player2SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::DoMakeSubstitution(const eWhichTeam _Who, ushort _Player1SquadNumber, ushort _Player2SquadNumber)
{
	// Save player coming on's name
#ifdef STORESUBSTITUTIONSINFO
	m_SubstitutionsInfo.DoAdd(m_TimePlayed, GetClub(_Who).GetFirstTeamSquad().GetSquad(_Player1SquadNumber), GetClub(_Who).GetFirstTeamSquad().GetSquad(_Player2SquadNumber));
#endif
	GetMatchVars().MakeSubstitution(m_TimePlayed, _Who, _Player1SquadNumber, _Player2SquadNumber);
	GetPlayerList(_Who)->MakeSubstitution(m_TimePlayed, _Player1SquadNumber, _Player2SquadNumber);
	GetClub(_Who).GetFirstTeamSquad().DoSwapPlayers(_Player1SquadNumber, _Player2SquadNumber);

	m_actionInfo.DoSubstituteUserPlayer(GetClub(_Who).GetPlayer(_Player2SquadNumber), GetClub(_Who).GetPlayer(_Player1SquadNumber), GetClub(_Who).GetName());
	ReCalculateMatchChancesImmediately();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::ReCalculateMatchChancesImmediately
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::ReCalculateMatchChancesImmediately()
{
    m_TacticsChanged = true;
    m_TimeSinceLastChange = 0;      // Force immediate re-calc
    DoCalculateMatchChances();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetPlayerSkill
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _SquadNumber
	Parameter: 	int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchPlay::GetPlayerSkill(const eWhichTeam _Who, const ushort _SquadNumber, int time )
{
	if (time == -1)
	{
		time = m_TimePlayed;
	}
	return GetMatchVars().GetPlayerSkill(_Who, _SquadNumber, time);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsUserControlledClub
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsUserControlledClub(const eWhichTeam _Who)
{
	return GetClub(_Who).IsUserControlled();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::IsSkipToEndOfMatch
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchPlay::IsSkipToEndOfMatch() const
{
	return m_SkipToEndOfMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetMatchInfoStr
	Access:    	public 
	Returns:   	CMatchPlayInfo&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchPlayInfo& CMatchPlay::GetMatchInfoStr()
{
	return m_MatchInfoStr;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetMatchActionInfo
	Access:    	public 
	Returns:   	CMatchActionItem&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchActionItem& CMatchPlay::GetMatchActionInfo()
{
	return m_actionInfo;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetPlayerName
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchPlay::GetPlayerName()
{
	return m_strPlayerName;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetPlayerName
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetPlayerName(CString& _PlayerName)
{
	m_strPlayerName = _PlayerName;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::TeamCurrentPosession
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchPlay::TeamCurrentPosession()
{
	return m_TeamCurrentPosession;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetTimePlayed
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchPlay::GetTimePlayed()
{
	return m_TimePlayed + m_InjuryTimePlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetClub
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchPlay::GetClub(const eWhichTeam _Who)
{
	return GetMatchVars().GetClub(_Who);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetClubData
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CMatchEventsStats&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchEventsStats& CMatchPlay::GetClubData(const eWhichTeam _Who)
{
	return GetFixture().GetClubData(_Who);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetFixture
	Access:    	public 
	Returns:   	CFixture&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture& CMatchPlay::GetFixture()
{
	EXT_ASSERT_POINTER(m_pFixture, CFixture);
	return *m_pFixture;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetDivision
	Access:    	public 
	Returns:   	CDivision&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CMatchPlay::GetDivision()
{
	EXT_ASSERT_POINTER(m_pDivision, CDivision);
	return *m_pDivision;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetCup
	Access:    	public 
	Returns:   	CCup&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup& CMatchPlay::GetCup()
{
	EXT_ASSERT_POINTER(m_pCup, CCup);
	return *m_pCup;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetCupRound
	Access:    	public 
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CMatchPlay::GetCupRound()
{
	EXT_ASSERT_POINTER(m_pCupRound, CCupRound);
	return *m_pCupRound;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetMatchVars
	Access:    	public 
	Returns:   	CMatchVars&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchVars& CMatchPlay::GetMatchVars()
{
	return GetFixture().GetMatchVars();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetMatchActionStage
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetMatchActionStage(const byte x)
{
	m_MatchActionStage = x;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::SetUserSubstitutionRequired
	Access:    	public 
	Parameter: 	const bool bUserSubstitutionRequired
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::SetUserSubstitutionRequired(const bool bUserSubstitutionRequired)
{
	m_bUserSubstitutionRequired = bUserSubstitutionRequired;
}


#ifdef	ACTIVEREFEREES
//---------------------------------------------------------------------------------------
CString	CMatchPlay::RefereeStr()
{
	return GetFixture().RefereeStr();
}
#endif


#ifdef STORE_MAN_OF_MATCH
//---------------------------------------------------------------------------------------
CString	CMatchPlay::ManOfMatchStr()
{
	return GetFixture().ManOfMatchStr();
}
#endif


#ifdef BALL_PITCH_LOCATION
/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::WeightedRandomBallYPosition
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchPlay::WeightedRandomBallYPosition()
{
	m_BallLocation.SetBallLocation(m_TimePlayed, 2 + RandomNumber.IntLessThan(4), RandomNumber.IntLessThan(MAXPITCHY / 2) + (m_TeamCurrentPosession ^ 1) * (MAXPITCHY / 2));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetBallX
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::GetBallX()
{
	return m_BallLocation.GetBallXLocation(m_TimePlayed);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchPlay::GetBallY
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchPlay::GetBallY()
{
	return m_BallLocation.GetBallYLocation(m_TimePlayed);
}
#endif


#ifdef SHOWTERRITORY
//---------------------------------------------------------------------------------------
int CMatchPlay::HomeTerritory()
{

	double a = ((m_Territory[2] + (m_Territory[1] / 2)) / TotalTerritory()) * 100;
	return static_cast<int>(a);
}


//---------------------------------------------------------------------------------------
int CMatchPlay::AwayTerritory()
{
	return 100 - HomeTerritory();
}
#endif


