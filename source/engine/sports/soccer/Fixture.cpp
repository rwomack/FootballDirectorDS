
 /*

    Class:- CFixture


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "MatchInfo.h"
#include "Stadium.h"
#include "FootballDirector.h"


	// Added or subtracted to all zones depending on team aggression(as a percentage of cumulative total)
    static const byte AGGRESSIONCOMPENSATOR0 = 10;
    static const byte AGGRESSIONCOMPENSATOR1 = 20;
    static const byte AGGRESSIONCOMPENSATOR2 = 30;
    static const byte AGGRESSIONCOMPENSATOR3 = 75;
    static const byte AGGRESSIONCOMPENSATOR4 = 0;
    static const byte AGGRESSIONCOMPENSATOR5 = 0;
    static const byte AGGRESSIONCOMPENSATOR6 = 75;
    static const byte AGGRESSIONCOMPENSATOR7 = 30;
    static const byte AGGRESSIONCOMPENSATOR8 = 20;
    static const byte AGGRESSIONCOMPENSATOR9 = 10;

	// Added to zones depending on team defensize / attacking style(as a percentage of cumulative total)
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR0 = 10;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR0 = 10;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR0 = 10;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR1 = 20;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR1 = 20;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR1 = 20;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR2 = 30;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR2 = 30;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR2 = 30;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR3 = 75;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR3 = 75;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR3 = 75;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR4 = 0;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR4 = 0;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR4 = 0;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR5 = 0;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR5 = 0;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR5 = 0;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR6 = 75;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR6 = 75;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR6 = 75;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR7 = 30;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR7 = 30;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR7 = 30;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR8 = 20;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR8 = 20;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR8 = 20;
    static const byte DEFENDERSTYLEOFPLAYCOMPENSATOR9 = 10;
    static const byte MIDFIELDERSTYLEOFPLAYCOMPENSATOR9 = 10;
    static const byte ATTACKERSTYLEOFPLAYCOMPENSATOR9 = 10;

	static const byte MINOFFSIDES = 1;
	static const byte MINFREEKICKS = 2;
	static const byte MAXFREEKICKS = 16;
	static const byte MAXIMUMGOALSINMATCH = 9;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NDS
IMPLEMENT_FDMEMORY_BINALLOC(CFixture);
#endif

/*------------------------------------------------------------------------------
	Method:   	CFixture::CFixture
	Access:    	public
	Parameter: 	CMatchInfo& _Match
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture::CFixture(CMatchInfo& _Match)
{
	OnInitialise(_Match);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::CFixture
	Access:    	public
	Returns:   		Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture::CFixture()
{
	SetSubsUse(5);
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::~CFixture
	Access:    	public
	Returns:   		Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture::~CFixture()
{
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::AttendanceStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::AttendanceStr()
{
	CExtString strAttend;
	strAttend.FormatLargeNumber(GetAttendance());
	return strAttend;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAddShotsOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoAddShotsOnTarget(const eWhichTeam _Who, const byte _Num)
{
	GetClubData(_Who).DoAddShotsOnTarget(_Num);
}


/*------------------------------------------------------------------------------
Method:   	CFixture::DoCalculateMatchStatistics
Access:    	public
Parameter: 	void
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoCalculateMatchStatistics()
{
	// Can't have negative number of shots on target
	DoGenerateShotsOnTarget(HOME);
	DoGenerateShotsOnTarget(AWAY);

	SetShotsOffTarget(HOME, min(15, 2 + RandomNumber.IntLessThan(GetClubData(HOME).GetShotsOnTarget() + 3)));
	SetShotsOffTarget(AWAY, min(15, 1 + RandomNumber.IntLessThan(GetClubData(AWAY).GetShotsOnTarget() + 3)));

	SetCorners(HOME, min(15, 2 + RandomNumber.IntLessThan(GetClubData(HOME).GetShotsOnTarget() + 2)));
	SetCorners(AWAY, min(15, 1 + RandomNumber.IntLessThan(GetClubData(AWAY).GetShotsOnTarget() + 2)));

	SetOffsides(HOME, min(15, MINOFFSIDES + RandomNumber.IntLessThan(2) + RandomNumber.IntLessThan(3) * WorldData().GetMatchHandler().GetMatchVars().GetOffsideTrap(HOME)));
	SetOffsides(AWAY, min(15, MINOFFSIDES + RandomNumber.IntLessThan(2) + RandomNumber.IntLessThan(3) * WorldData().GetMatchHandler().GetMatchVars().GetOffsideTrap(AWAY)));
	DoGenerateFreeKicks(HOME);
	DoGenerateFreeKicks(AWAY);
	m_ClubData[HOME].SetPercentageOfPlay(min(76, (47 + (GetMidfielderSkill(HOME) - GetMidfielderSkill(AWAY) ) / 2) + RandomNumber.IntLessThan(7)));
	m_ClubData[AWAY].SetPercentageOfPlay(100 - GetPercentageOfPlay(HOME));
}


/*------------------------------------------------------------------------------
Method:   	CFixture::DoCalculateTeamStrength
Access:    	public
Parameter: 	const eWhichTeam _Who
Parameter: 	const byte _Time
Returns:   	void
Qualifier:
Purpose:	Calculate the current team's skills.
			Skill List is NULL if not a viewed match
------------------------------------------------------------------------------*/
void CFixture::DoCalculateTeamStrength(const eWhichTeam _Who, const byte _Time)
{
	CClub* pClub = &GetClub(_Who);
	CTacticList* pTactics = NULL;
	if (pClub->IsUserControlled() == true)
	{
		// Only require tactics for user clubs
		pTactics = &pClub->GetUser().GetTacticsList();
		EXT_ASSERT(pTactics != NULL);
	}

	CPlayer* pPlayer = WorldData().GetPlayer(pClub->GetFirstTeamSquad().GetSquad(0));
	EXT_ASSERT_POINTER(pPlayer, CPlayer);
	GetClubData(_Who).GetTeamSkills().SetGoalKeeperSkill(pPlayer->DoCalculateEffectiveSkill(SKILL_HANDLING));

	WorldData().GetMatchHandler().GetMatchVars().GetSquadPerformance(_Who).SetPlayerCurrentSkill(0, GetGoalKeeperSkill(_Who), _Time);

	CMatchVarsData& MatchVarsData = WorldData().GetMatchHandler().GetMatchVars().GetMatchVarsTeamData(_Who);
	MatchVarsData.ClearShootingSkillLists();

	// Calculate all the outfield player effective skills
	int TotalTemper = 0;
	ushort Defstren = 0, Midstren = 0, Attstren = 0;
	ushort DefCount = 0, MidCount = 0, AttCount = 0;

	float dDefstren, dMidstren, dAttstren;
	float EffectiveSkill;
	for (uint LoopCount = 1; LoopCount < PLAYERSINTEAM; LoopCount ++)
	{
		pPlayer = WorldData().GetPlayer(pClub->GetFirstTeamSquad().GetSquad(LoopCount));
		EXT_ASSERT_POINTER(pPlayer, CPlayer);
		if (pPlayer->IsPlaying() == true)
		{
			if (pTactics != NULL)
			{
				// User controlled clubs use more sophisticated formations
        		dDefstren = (static_cast<float>(pPlayer->DoCalculateEffectiveSkill(SKILL_TACKLING)) / 100 ) *
					pTactics->GetDefenceEffectiveness(LoopCount - 1);
        		dMidstren = (static_cast<float>(pPlayer->DoCalculateEffectiveSkill(SKILL_PASSING)) / 100 ) *
					pTactics->GetMidfieldEffectiveness(LoopCount - 1);
        		dAttstren = (static_cast<float>(pPlayer->DoCalculateEffectiveSkill(SKILL_SHOOTING)) / 100 ) *
					pTactics->GetAttackEffectiveness(LoopCount - 1);
				EXT_ASSERT(dDefstren + dMidstren + dAttstren < 100);

				MatchVarsData.GetDefenceZonalStats().AddEnergy(pPlayer->GetSkills().GetEnergy());
				MatchVarsData.GetDefenceZonalStats().AddPace(pPlayer->GetSkills().GetPace());
				MatchVarsData.AddToShootingAndHeadingSkillList( \
					pPlayer->DoCalculateEffectiveSkill(SKILL_SHOOTING) * pTactics->GetShootingEffectiveness(LoopCount - 1),
					pPlayer->DoCalculateEffectiveSkill(SKILL_HEADING) * pTactics->GetHeadingEffectiveness(LoopCount - 1));
				switch (pTactics->GetCurrentTactics().PlayerPosition(LoopCount - 1))
				{
					case DEFENDER:
						DefCount++;
						break;
					case MIDFIELDER:
						MidCount++;
						break;
					case ATTACKER:
						AttCount++;
						break;
				}
			}
			else
			{
				dDefstren = dMidstren = dAttstren = 0;
				EffectiveSkill = pPlayer->DoCalculateEffectiveSkill(SKILL_OVERALL);
				switch(pPlayer->GetSkills().GetPosition())
				{
					case GOALKEEPER:
						MatchVarsData.AddToShootingAndHeadingSkillList(0, 0);
						break;
					case DEFENDER:
						dDefstren = EffectiveSkill;
						MatchVarsData.AddDefenceVars(pPlayer);
						DefCount++;
						break;
					case MIDFIELDER:
						dMidstren = EffectiveSkill;
						MatchVarsData.AddMidfieldVars(pPlayer);
						MidCount++;
						break;
					case ATTACKER:
						dAttstren = EffectiveSkill;
						MatchVarsData.AddAttackVars(pPlayer);
						AttCount++;
						break;
					default:
						EXT_ASSERT(false);
				}
			}

			WorldData().GetMatchHandler().GetMatchVars().GetSquadPerformance(_Who).SetPlayerCurrentSkill(LoopCount, (byte)(dDefstren + dMidstren + dAttstren), _Time);
			Defstren += (ushort)dDefstren;
			Midstren += (ushort)dMidstren;
			Attstren += (ushort)dAttstren;
			TotalTemper += pPlayer->GetTemperament();
		}
		else
		{
			// Player is not playing
			MatchVarsData.AddToShootingAndHeadingSkillList(0, 0);
			WorldData().GetMatchHandler().GetMatchVars().GetSquadPerformance(_Who).SetPlayerCurrentSkill(LoopCount, 0, _Time);   // Clear performance by minute
		}
	}

	WorldData().GetMatchHandler().GetMatchVars().SetTemperament(_Who, TotalTemper);

	// Adjust according to team aggression
	static const byte nAggressionCompensators[] =
	{
		AGGRESSIONCOMPENSATOR0, AGGRESSIONCOMPENSATOR1,
		AGGRESSIONCOMPENSATOR2, AGGRESSIONCOMPENSATOR3,
		AGGRESSIONCOMPENSATOR4, AGGRESSIONCOMPENSATOR5,
		AGGRESSIONCOMPENSATOR6, AGGRESSIONCOMPENSATOR7,
		AGGRESSIONCOMPENSATOR8, AGGRESSIONCOMPENSATOR9
	};
	byte nAggressionCompensator = nAggressionCompensators[WorldData().GetMatchHandler().GetMatchVars().GetTeamAggression(_Who)];
	byte nGoalKeeperSkill = GetClubData(_Who).GetTeamSkills().GetGoalKeeperSkill();
	switch(WorldData().GetMatchHandler().GetMatchVars().GetTeamAggression(_Who))
	{
		case 0:
		case 1:
		case 2:
		case 3:
			Attstren -= Attstren / nAggressionCompensator;
			Midstren -= Midstren / nAggressionCompensator;
			Defstren -= Defstren / nAggressionCompensator;
			nGoalKeeperSkill -= (nGoalKeeperSkill / nAggressionCompensator);
			break;
		case 4:
		case 5:
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			EXT_ASSERT(AGGRESSIONCOMPENSATOR9 > 0);		// Ensure divide by positive amount
			Attstren += Attstren / nAggressionCompensator;
			Midstren += Midstren / nAggressionCompensator;
			Defstren += Defstren / nAggressionCompensator;
			nGoalKeeperSkill += (nGoalKeeperSkill / nAggressionCompensator);
			break;
		default :
			EXT_ASSERT(false);		// Team aggression out of range
	}
	GetClubData(_Who).GetTeamSkills().SetGoalKeeperSkill(min(MAXPLAYSKILL, max(nGoalKeeperSkill, MINPLAYSKILL)));

	// Compensate for Attacking / Defensive
	switch(GetMatchVars().GetStyleOfPlay(_Who))
	{
	case 0:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR0 > 0);		// Ensure divide by positive amount
		Defstren += Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR0;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR0 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR0;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR0 > 0);		// Ensure divide by positive amount
		Attstren -= Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR0;
		break;
	case 1:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR1 > 0);		// Ensure divide by positive amount
		Defstren += Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR1;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR1 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR1;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR1 > 0);		// Ensure divide by positive amount
		Attstren -= Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR1;
		break;
	case 2:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR2 > 0);		// Ensure divide by positive amount
		Defstren += Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR2;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR2 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR2;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR2 > 0);		// Ensure divide by positive amount
		Attstren -= Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR2;
		break;
	case 3:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR3 > 0);		// Ensure divide by positive amount
		Defstren += Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR3;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR3 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR3;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR3 > 0);		// Ensure divide by positive amount
		Attstren -= Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR3;
		break;
	case 4:
	case 5:
		break;
	case 6:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR6 > 0);		// Ensure divide by positive amount
		Defstren -= Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR6;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR6 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR6;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR6 > 0);		// Ensure divide by positive amount
		Attstren += Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR6;
		break;
	case 7:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR7 > 0);		// Ensure divide by positive amount
		Defstren -= Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR7;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR7 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR7;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR7 > 0);		// Ensure divide by positive amount
		Attstren += Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR7;
		break;
	case 8:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR8 > 0);		// Ensure divide by positive amount
		Defstren -= Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR8;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR8 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR8;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR8 > 0);		// Ensure divide by positive amount
		Attstren += Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR8;
		break;
	case 9:
		EXT_ASSERT(DEFENDERSTYLEOFPLAYCOMPENSATOR9 > 0);		// Ensure divide by positive amount
		Defstren -= Defstren / DEFENDERSTYLEOFPLAYCOMPENSATOR9;
		EXT_ASSERT(MIDFIELDERSTYLEOFPLAYCOMPENSATOR9 > 0);		// Ensure divide by positive amount
		Midstren -= Midstren / MIDFIELDERSTYLEOFPLAYCOMPENSATOR9;
		EXT_ASSERT(ATTACKERSTYLEOFPLAYCOMPENSATOR9 > 0);		// Ensure divide by positive amount
		Attstren += Attstren / ATTACKERSTYLEOFPLAYCOMPENSATOR9;
		break;
	default :
		EXT_ASSERT(false);		// Team aggression out of range
	}
	GetMatchVars().GetSquadPerformance(_Who).SetCurrentSkills(GetGoalKeeperSkill(_Who), Defstren, Midstren, Attstren, _Time);

	// Actual zonal effectivenesses
	Defstren /= max(1, DefCount);
	Midstren /= max(1, MidCount);
	Attstren /= max(1, AttCount);
	GetClubData(_Who).GetTeamSkills().SetAttackerSkill(min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte>(Attstren))));
	GetClubData(_Who).GetTeamSkills().SetMidfielderSkill(min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte>(Midstren))));
	GetClubData(_Who).GetTeamSkills().SetDefenderSkill(min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte>(Defstren))));

	GetFirstTeamSquad(_Who).DoUpdateTeamStrengths(GetGoalKeeperSkill(_Who), GetDefenderSkill(_Who), GetMidfielderSkill(_Who), GetAttackerSkill(_Who));
}


/*------------------------------------------------------------------------------
Method:   	CFixture::DoCalculateDisciplinaryTotals
Access:    	public
Parameter: 	const eWhichTeam _Who
Parameter: 	CMatchVarsData& _MatchVarsData
Returns:   	void
Qualifier:
Purpose:	Determine how many players to discipline
------------------------------------------------------------------------------*/
void CFixture::DoCalculateDisciplinaryTotals(const eWhichTeam _Who, CMatchVarsData& _MatchVarsData)
{
	CAggression& Aggression = WorldData().GetAggressionList().GetAt(GetMatchVars().GetTeamAggression(_Who));
	sbyte CalcHowManyPlayers = Aggression.HowManyPlayersToBook() - GetNumberOfPlayersBooked(_Who);
	CalcHowManyPlayers = max(0, CalcHowManyPlayers);
	_MatchVarsData.SetNumberPlayersToBook(CalcHowManyPlayers);
	CalcHowManyPlayers = Aggression.HowManyPlayersToSendOff() - GetNumberOfPlayersSentOff(_Who);
	CalcHowManyPlayers = max(0, CalcHowManyPlayers);
	_MatchVarsData.SetNumberPlayersToSendOff(CalcHowManyPlayers);
#ifdef	ACTIVEREFEREES
	_MatchVarsData.SetNumberPlayersToBook(GetReferee()->AdjustBookings(GetMatchVars().GetNumberHomePlayersToBook()));
	_MatchVarsData.SetNumberPlayersToSendOff(GetReferee()->AdjustSendingsOff(GetMatchVars().GetNumberHomePlayersToSendOff()));
#endif
}


/*------------------------------------------------------------------------------
Method:   	CFixture::DoCreateDisciplinaryEvent
Access:    	public
Parameter: 	const byte _CurrentTime
Parameter: 	const enumMatchEvent _eEventType
Parameter: 	const uint _ePercentChance
Parameter: 	CNumericArray<byte>& _TheEvents
Returns:   	void
Qualifier:
Purpose:	Generate discipline time
------------------------------------------------------------------------------*/
void CFixture::DoCreateDisciplinaryEvent(const byte _CurrentTime, const eMatchEvent _eEventType, const uint _ePercentChance, CFixtureMatchEvents& _TheEvents)
{
	if (_TheEvents.RandomPosition() > _CurrentTime)
	{
		ushort time = _TheEvents.DoFindRandomFreeEvent();
		if (RandomNumber.IntLessThan(100) < _ePercentChance && time > _CurrentTime)
		{
			_TheEvents.SetEvent(time, _eEventType);
		}
	}
}

extern bool g_suspensionDisabled;

/*------------------------------------------------------------------------------
Method:   	CFixture::DoCalculateDisciplinaryActions
Access:    	public
Parameter: 	const byte _CurrentTime
Parameter: 	CNumericArray<byte>& _TheEvents
Returns:   	void
Qualifier:
Purpose:	Determine how many players to discipline
------------------------------------------------------------------------------*/
void CFixture::DoCalculateDisciplinaryActions(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents)
{
  if( g_suspensionDisabled )
    return;

	CMatchVarsData& pHomeMatchVars = GetMatchVars().GetMatchVarsTeamData(HOME);
	DoCalculateDisciplinaryTotals(HOME, pHomeMatchVars);
	CMatchVarsData& pAwayMatchVars = GetMatchVars().GetMatchVarsTeamData(AWAY);
	DoCalculateDisciplinaryTotals(AWAY, pAwayMatchVars);

	if (GetExtraTimeFlag() == true)
	{
		// Playing extra time
		GetMatchVars().DoAdjustDisciplinaryEventsForExtraTime();
	}
	for (uint LoopCount = 0; LoopCount < 10; LoopCount++)
	{
		if (LoopCount < pHomeMatchVars.GetNumberPlayersToBook())
		{
			DoCreateDisciplinaryEvent(_CurrentTime, HOMEBOOKING, CHANCEBOOK, _TheEvents);
		}
		if (LoopCount < pAwayMatchVars.GetNumberPlayersToBook())
		{
			DoCreateDisciplinaryEvent(_CurrentTime, AWAYBOOKING, CHANCEBOOK, _TheEvents);
		}
		if (LoopCount < pHomeMatchVars.GetNumberPlayersToSendOff() && RandomNumber.IntLessThan(CHANCESENDINGOFF) == 2)
		{
			DoCreateDisciplinaryEvent(_CurrentTime, HOMESENDINGOFF, CHANCESENDOFF, _TheEvents);
		}
		if (LoopCount < pAwayMatchVars.GetNumberPlayersToSendOff() && RandomNumber.IntLessThan(CHANCESENDINGOFF) == 2)
		{
			DoCreateDisciplinaryEvent(_CurrentTime, AWAYSENDINGOFF, CHANCESENDOFF, _TheEvents);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoIncrementFreeKicks
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoIncrementFreeKicks(const eWhichTeam _Who)
{
	GetClubData(_Who).DoIncrementFreeKicks();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoIncrementShotsOffTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoIncrementShotsOffTarget(const eWhichTeam _Who)
{
	GetClubData(_Who).DoIncrementShotsOffTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoIncrementOffsides
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoIncrementOffsides(const eWhichTeam _Who)
{
	GetClubData(_Who).DoIncrementOffsides();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoIncrementShotsOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoIncrementShotsOnTarget(const eWhichTeam _Who)
{
	GetClubData(_Who).DoIncrementShotsOnTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetHomeGoalsToScore
	Access:    	public
	Parameter: 	void
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetHomeGoalsToScore()
{
	return GetSubsSelect();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetAwayGoalsToScore
	Access:    	public
	Parameter: 	void
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetAwayGoalsToScore()
{
	return GetSubsUse();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetClub
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CFixture::GetClub(const eWhichTeam _Who)
{
	if (_Who == HOME)
	{
		return WorldData().GetClubByID(m_ClubData[HOME].GetClubID());
	}
	else
	{
		return WorldData().GetClubByID(m_ClubData[AWAY].GetClubID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetCompetitionTitle
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::GetCompetitionTitle()
{
	CString CompetitionTitle;
	switch (GetMatchType())
	{
		case LEAGUEMATCH:
			CompetitionTitle = GetClub(HOME).GetDivision().GetName();
			break;
		case REPLAYMATCH:
		case REPLAYMATCHNEUTRALVENUE:
		case CUPMATCH:
		case CUPMATCHNEUTRALVENUE:
			CompetitionTitle = WorldData().DoFindCompetitionTitleForFixture(this);
			break;
		case FRIENDLYMATCH:
        CompetitionTitle.LoadString(IDS_FRIENDLYMATCH);
			break;
		case NOTAMATCH:
			EXT_ASSERT(false);	// Should not be here!!
			break;
	}
	EXT_ASSERT(CompetitionTitle.IsEmpty() == false);
	return CompetitionTitle;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetFirstTeamSquad
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	CPlayerSquad&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSquad& CFixture::GetFirstTeamSquad(const eWhichTeam _Who)
{
	return GetClub(_Who).GetFirstTeamSquad();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::ClubName
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CFixture::GetClubName(const eWhichTeam _Who)
{
	return GetClub(_Who).GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchVars
	Access:    	public
	Parameter: 	void
	Returns:   	CMatchVars&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchVars& CFixture::GetMatchVars()
{
	return WorldData().GetMatchHandler().GetMatchVars();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchSubs
	Access:    	public
	Parameter: 	void
	Returns:   	CMatchSubs&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchSubs& CFixture::GetMatchSubs()
{
	return WorldData().GetMatchSubs();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::LosingClubName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::LosingClubName()
{
	return GetLosingClub()->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::pWinningClub
	Access:    	public
	Parameter: 	void
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CFixture::GetWinningClub()
{
	return &WorldData().GetClubByID(GetWinningClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::pLosingClub
	Access:    	public
	Parameter: 	void
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CFixture::GetLosingClub()
{
	return &WorldData().GetClubByID(GetLosingClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::WinningClubUpperCaseName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::WinningClubUpperCaseName()
{
	return WorldData().GetClubByID(GetWinningClubID()).GetUpperCaseName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::LosingClubUpperCaseName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::LosingClubUpperCaseName()
{
	return WorldData().GetClubByID(GetLosingClubID()).GetUpperCaseName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetClubData
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	CMatchEventsStats&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchEventsStats& CFixture::GetClubData(const eWhichTeam _Who)
{
	if (_Who == HOME)
	{
		return m_ClubData[HOME];
	}
	return m_ClubData[AWAY];
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::OnInitialise
	Access:    	public
	Parameter: 	CMatchInfo& _Match
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::OnInitialise(CMatchInfo& _Match)
{
	// Set the number of subs allowed and set them to indicate they have not been used yet
	SetSubsSelect(_Match.GetSubsSelect());
	SetSubsUse(_Match.GetSubsUse());
	DoResetVariables();
#ifdef	ACTIVEREFEREES
	m_RefereeNum = RefereeNum;
#endif
    SetMatchType(_Match.GetCompetitionType());
	m_ClubData[HOME].SetClubID(_Match.GetHomeClubID());
	m_ClubData[AWAY].SetClubID(_Match.GetAwayClubID());
    GetMatchVars().SetHomeAwayClubs(*this);
    SetAttendance(_Match.GetStadiumCapacity());
    SetMatchDate(WorldData().GetCurrentDate());
 }


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoResetVariables()
{
	GetMatchSubs().OnInitialise(GetSubsUse());
  m_TheEvents.DoResetVariables();
    SetMatchType(NOTAMATCH);
	m_ClubData[HOME].DoResetVariables();
	m_ClubData[AWAY].DoResetVariables();
#ifdef STORE_MAN_OF_MATCH
    m_ManOfMatch = NOPLAYER;
#endif
    SetMatchLength(0);            // How many minutes injury time to play
    SetAttendance(0);
	SetMatchDate(CDate(CDate::MAX_DATE));
	m_Data.m_TicketPrice = 0;
	m_Data.m_ExtraTimePlayed = 0;

#ifdef	ACTIVEREFEREES
	m_RefereeNum = 0;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::IsMatchDrawn
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::IsMatchDrawn()
{
	if (GetEventList()->WasPenaltyShootOut() == true)
	{
		if (GetEventList()->NumberHomePenaltyShootOutGoals() == GetEventList()->NumberAwayPenaltyShootOutGoals())
		{
			return true;
		}
	}
	else if (GetNumberGoals(HOME) == GetNumberGoals(AWAY))
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::IsUserMatch
	Access:    	public
	Parameter: 	void
	Returns:   	const bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::IsUserMatch()
{
  	if (m_ClubData[HOME].GetClubID().id == NOCLUB || m_ClubData[AWAY].GetClubID().id == NOCLUB)
  	{
    	return false;
  	}

	bool bRet = false;
	if (GetClub(HOME).IsUserControlled() == true || GetClub(AWAY).IsUserControlled() == true)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::RandomPlayerName
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const int _Posn
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::RandomPlayerName(const eWhichTeam _Who, const int _Posn)
{
	int Junk;
    CPlayer* pPlayer;
	do
	{
		do
		{
            Junk = 1 + RandomNumber.IntLessThan(PLAYERSINTEAM - 1);		// Not the goalie
            pPlayer = &GetClub(_Who).GetPlayer(Junk);
		}
		while(pPlayer->GetSkills().GetPosition() != static_cast<int> (_Posn));
	}
	while(pPlayer->IsPlaying() != true);
	return pPlayer->GetSurname();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchDetails
	Access:    	public
	Parameter: 	CMatchInfo& _MatchDetails
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::GetMatchDetails(CMatchInfo& _MatchDetails)
{
	_MatchDetails.SetSubstitutes(GetSubsSelect(), GetSubsUse());
	_MatchDetails.DoInitialiseClubs(m_ClubData[HOME].GetClubID(), m_ClubData[AWAY].GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAdjustGoalAttemptsForPaceSupremacy
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CFixture::DoAdjustGoalAttemptsForPaceSupremacy(const eWhichTeam _Who)
{
	int Adjustment = ((WorldData().GetMatchHandler().GetMatchVars().GetMatchVarsTeamData(_Who).GetAttackZonalStats().GetPace() / 3) > ((WorldData().GetMatchHandler().GetMatchVars().GetMatchVarsTeamData(GetOpponent(_Who)).GetDefenceZonalStats().GetPace() / 4) +  20));
	Adjustment -= ((WorldData().GetMatchHandler().GetMatchVars().GetMatchVarsTeamData(_Who).GetAttackZonalStats().GetPace() / 3) < ((WorldData().GetMatchHandler().GetMatchVars().GetMatchVarsTeamData(GetOpponent(_Who)).GetDefenceZonalStats().GetPace() / 4) +  40));
	return Adjustment;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAdjustGoalAttemptsForLongBallPassing
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _LongBallPassing
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CFixture::DoAdjustGoalAttemptsForLongBallPassing(const eWhichTeam _Who, const byte _LongBallPassing)
{
	eWhichTeam eOpponent = HOME;
	if (_Who == HOME)
	{
		eOpponent = AWAY;
	}
	int Adjustment = 0;
	switch(_LongBallPassing)
	{
		case 0:
		case 1:
			// Extra goal attempts if stronger defence than opponent attack
			Adjustment += (GetDefenderSkill(_Who) > GetAttackerSkill(eOpponent)) * 2;
			// Less goal attempts if weaker defence than opponent attack
			Adjustment -= (GetAttackerSkill(_Who) < GetDefenderSkill(eOpponent)) * 2;
			break;
		case 2:
		case 3:
			// Extra goal attempts if stronger defence than opponent attack
			Adjustment += (GetDefenderSkill(_Who) > GetAttackerSkill(eOpponent));
			// Less goal attempts if weaker defence than opponent attack
			Adjustment -= (GetAttackerSkill(_Who) < GetDefenderSkill(eOpponent));
			break;
		case 4:
		case 5:
			break;
		case 6:
		case 7:
			// Extra goal attempts if stronger midfield than opponent
			Adjustment += (GetMidfielderSkill(_Who) > GetMidfielderSkill(eOpponent));
			// Less goal attempts if weaker midfield than opponent
			Adjustment -= (GetMidfielderSkill(_Who) < GetMidfielderSkill(eOpponent));
			break;
		case 8:
		case 9:
			// Extra goal attempts if stronger midfield than opponent
			Adjustment += (GetMidfielderSkill(_Who) > GetMidfielderSkill(eOpponent)) * 2;
			// Less goal attempts if weaker midfield than opponent
			Adjustment -= (GetMidfielderSkill(_Who) < GetMidfielderSkill(eOpponent)) * 2;
			break;
	}
	return Adjustment;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoGenerateShotsOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoGenerateShotsOnTarget(const eWhichTeam _Who)
{
    if (GetShotsOnTarget(_Who) < 0)
	{
        SetShotsOnTarget(_Who, RandomNumber.IntLessThan(3));
	}
    SetShotsOnTarget(_Who, RandomNumber.IntLessThan(max(2, GetShotsOnTarget(_Who))) + GetNumberGoals(_Who));
	eWhichTeam eOpponent = HOME;
	if (_Who == eOpponent)
	{
		eOpponent = AWAY;
	}
	if (GetMidfielderSkill(_Who) > GetMidfielderSkill(eOpponent))
	{
		DoAddShotsOnTarget(_Who, RandomNumber.IntLessThan(1));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoGenerateFreeKicks
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoGenerateFreeKicks(const eWhichTeam _Who)
{
	eWhichTeam eOpponent = HOME;
	if (_Who == eOpponent)
	{
		eOpponent = AWAY;
	}

	if (GetNumberOfPlayersBooked(_Who) > 0 )
	{
		int NumFreeKicks = GetNumberOfPlayersBooked(eOpponent) +
			RandomNumber.IntLessThan(max(2, GetNumberOfPlayersBooked(eOpponent) * 3 +
			WorldData().GetMatchHandler().GetMatchVars().GetTeamAggression(eOpponent)));
		SetFreeKicks(_Who, min(MAXFREEKICKS, NumFreeKicks));
	}
	else
	{
		SetFreeKicks(_Who, min(MAXFREEKICKS, MINFREEKICKS + RandomNumber.IntLessThan(2 + WorldData().GetMatchHandler().GetMatchVars().GetTeamAggression(eOpponent))));
	}
	SetFreeKicks(_Who, min(MAXFREEKICKS, GetFreeKicks(_Who) + GetNumberOfPlayersSentOff (eOpponent) + GetOffsides(eOpponent)));
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::CountSubsUsed
	Access:    	public
	Parameter: 	const eWhichTeam Who
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CFixture::DoCountSubsUsed(const eWhichTeam _Who)
{
	return GetMatchSubs().DoCountSubsUsed(_Who);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetGoalKeeperSkill
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetGoalKeeperSkill(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetTeamSkills().GetGoalKeeperSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetDefenderSkill
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetDefenderSkill(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetTeamSkills().GetDefenderSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMidfielderSkill
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetMidfielderSkill(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetTeamSkills().GetMidfielderSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetAttackerSkill
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetAttackerSkill(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetTeamSkills().GetAttackerSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::PlayersToInjure
	Access:    	public
	Parameter: 	const uint _Aggression - Aggression of opponents
	Returns:   	const byte
	Qualifier:
	Purpose:	Determine how many players to attempt to injure during the
				match for one team
------------------------------------------------------------------------------*/
int CFixture::PlayersToInjure(const uint _Aggression)
{
	EXT_ASSERT(_Aggression < 10);
	static const byte PlayersToInjure[] =
	{
		0, 0, 0, 0, 1,
		1, 1, 1, 2, 2
	};
	if (WorldData().IsFastTest() == true)
	{
		return 0;
	}
	else
	{
		return PlayersToInjure[_Aggression];
	}
}

extern bool g_injuriesDisabled;

/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateInjuryActions
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoCalculateInjuryActions(const eWhichTeam _Who)
{

  if( g_injuriesDisabled )
    return;

	if (GetClub(_Who).IsUserControlled() == true || GetFirstTeamSquad(_Who).AllowInjuries() == true)
	{
		byte InjCount = 0;
		uint HowManyToInjure = PlayersToInjure(GetMatchVars().GetTeamAggression(GetOpponent(_Who)));
		for (uint LoopCount = 0; LoopCount < HowManyToInjure; LoopCount++)
		{
			InjCount += DoInjurePlayer(_Who);
		}
#ifdef STOREMATCHINJURIES
		SetInjuries(_Who, InjCount);
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoInjurePlayer
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	bool
	Qualifier:
	Purpose:	Find and injure a player, check he's actually playing
				(not suspended or injured)
------------------------------------------------------------------------------*/
bool CFixture::DoInjurePlayer(const eWhichTeam _Who)
{
    CPlayer* pPlayer;
	ushort SquadNumber = 0;
	if (RandomNumber.IntLessThan(10) > CHANCEINJUREPLAYER)
	{
		return false;
	}
	if (RandomNumber.IntLessThan(CHANCEGOALKEEPERINJURED) == 1)
	{
		// Goalkeeper
		pPlayer = &GetClub(_Who).GetPlayer(0);
		if (pPlayer->DoTryInjurePlayer(GetClub(_Who)) != true)
		{
			return false;
		}
	}
	else
	{
		SquadNumber = GetFirstTeamSquad(_Who).RandomPlayingSquadNumber();
		if (SquadNumber != NOCLUB && (SquadNumber > 0 || RandomNumber.IntLessThan(CHANCEGOALKEEPERINJURED) == 1))
		{
			GetClub(_Who).GetPlayer(SquadNumber).DoTryInjurePlayer(GetClub(_Who));
		}
		else
		{
			return false;
		}
	}
    GetMatchVars().SetLastInjuredPlayerSquadNumber(SquadNumber);
    return true;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCreateOffsidePlayer
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CFixture::DoCreateOffsidePlayer(const eWhichTeam _Who)
{
	CPlayer* rpPlayer;
	ePlayerPosition ePosition;
	ushort nPlayerID;
	do
	{
		ePosition = ATTACKER;
		if (RandomNumber.IntLessThan(CHANCEMIDFIELDEROFFSIDE) == 1)
		{
			ePosition = MIDFIELDER;
		}
		nPlayerID = GetFirstTeamSquad(_Who).RandomPlayingPosition(false, ePosition);
		if (nPlayerID == NOPLAYER)
		{
			ePosition = (ePosition == ATTACKER) ? MIDFIELDER : ATTACKER;
			nPlayerID = GetFirstTeamSquad(_Who).RandomPlayingPosition(false, ePosition);
		}
		if (nPlayerID == NOPLAYER)
		{
			// starting to get ridiculous...but in the interests of not crashing...
			ePosition = DEFENDER;
			nPlayerID = GetFirstTeamSquad(_Who).RandomPlayingPosition(false, ePosition);
		}

		EXT_ASSERT(nPlayerID != NOPLAYER);
		rpPlayer = WorldData().GetPlayer(nPlayerID);
	}
	while(rpPlayer->IsPlaying() != true);
	return rpPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoDisciplinePlayer
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const eDisciplineEvent _eDiscType
	Parameter: 	const int _When
	Returns:   	const ushort -  the squad number of the player booked
	Qualifier:
	Purpose:	Select and discipline a player
------------------------------------------------------------------------------*/
ushort CFixture::DoDisciplinePlayer(const eWhichTeam _Who, const eDisciplineEvent _eDiscType, const int _When)
{
// 	if (WorldData().IsFastTest() == true)
// 	{
// 		return NOPLAYER;
// 	}
    CPlayer* pDiscPlayer;
    CPlayer* pAgainstPlayer;
    ushort PlayerToBook, PlayerToFoul, attempts = 0;
    CNumericArray<byte> PlayerTempers;
    // Type of player most likely to be fouled depending on fouling player's position
    ePlayerPosition LikelyFoul[] =
	{
		ATTACKER, ATTACKER,MIDFIELDER, DEFENDER
    };
	CClub& Club = GetClub(_Who);
	GetFirstTeamSquad(_Who).DoBuildTeamTempersArray(&PlayerTempers);
    int TotalPlayerTempers = PlayerTempers.TotalSum();

    // Find a player who hasn't been booked yet(this match)
    // Can only be booked once per match, check he's actually playing(not suspended or injured)
	byte BookValue;
    do
	{
		BookValue = RandomNumber.IntLessThan(TotalPlayerTempers);
        PlayerToBook = PlayerTempers.PositionLessThanValue(BookValue);
        EXT_ASSERT(PlayerToBook < PLAYERSINTEAM);
        pDiscPlayer = &Club.GetPlayer(PlayerToBook);
        attempts++;
    }
	while(pDiscPlayer->DoTryBookPlayer() != true && attempts < 100);

	// Can't find a valid player to discipline
	if (attempts == 100)
	{
		return NOPLAYER;
	}

    // Try to find a player to foul
	PlayerToFoul = GetFirstTeamSquad(_Who).RandomPlayingSquadNumber(LikelyFoul[pDiscPlayer->GetSkills().GetPosition()]);
	if (PlayerToFoul == NOPLAYER)
	{
		PlayerToFoul = GetFirstTeamSquad(_Who).RandomPlayingSquadNumber();
		// Can't find a valid player to foul
		if (PlayerToFoul == NOPLAYER)
		{
			EXT_ASSERT(false);		// Should never happen
			return NOPLAYER;
		}
	}
	pAgainstPlayer = &Club.GetPlayer(PlayerToFoul);
    GetMatchVars().SetLastFoulerName(pDiscPlayer);
    GetMatchVars().SetLastVictimName(pAgainstPlayer);

	switch(_eDiscType)
	{
		case BOOKPLAYER:
			pDiscPlayer->DoBookPlayer(static_cast<eMatchCompetitionType>(GetMatchType()));
            DoAddMatchEvent(_When, (eMatchEvent)(HOMEBOOKING + _Who), *pDiscPlayer);
			Club.GetSeasonStats().DoAddSeasonBookings(1);
			break;

		case SENDPLAYEROFF:
			pDiscPlayer->DoSendPlayerOff(static_cast<eMatchCompetitionType>(GetMatchType()));
            DoAddMatchEvent(_When, (eMatchEvent)(HOMESENDINGOFF + _Who), *pDiscPlayer);
			Club.GetSeasonStats().DoAddSeasonSendingOffs(1);
			break;

		default:
			EXT_ASSERT(false);
	}
    return PlayerToBook;
 }


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCreateDefender
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away
	Parameter: 	const ePlayerPosition _ePosition - defender or midfielder if
					require specific position, if not supplied selects randomly
	Returns:   	const ushort - squad number of a random playing player in the position
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::DoCreateDefender(const eWhichTeam _Who, const ePlayerPosition _ePosition /*= GOALKEEPER*/)
{
	ushort iRet = NOPLAYER;
	if ((RandomNumber.IntLessThan(10) < 7 && _ePosition == 0) || _ePosition == DEFENDER)
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(true, DEFENDER);
	}
	if (iRet == NOPLAYER)
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(true,MIDFIELDER);
	}
	if (iRet == NOPLAYER)
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(true, ATTACKER);
	}
	if (iRet == NOPLAYER)
	{
		// Shouldn't need to but just in case no midfielders or attackers
		// try for defender(Don't always try first time)
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(true, DEFENDER);
	}
	EXT_ASSERT(iRet != NOPLAYER);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCreateAssister
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away
	Parameter: 	const ushort _Scorer - squad number of the player taking the shot
				(won't select him)
	Returns:   	const ushort
	Qualifier:
	Purpose:	Select a player to assist the shot
------------------------------------------------------------------------------*/
ushort CFixture::DoCreateAssister(const eWhichTeam _Who, const ushort _Scorer)
{
	CClub& theClub = GetClub(_Who);
    CPlayer* pScorer = &theClub.GetPlayer(_Scorer);
    CPlayer* pAssist;
	ushort iRet = NOPLAYER;
	// Select player who supplied the pass etc.
	do
	{
		iRet = NOPLAYER;
		if (RandomNumber.IntLessThan(10) > 7)
		{
			iRet = theClub.GetFirstTeamSquad().RandomPlayingPosition(true, DEFENDER);
		}
		if (iRet == NOPLAYER && RandomNumber.IntLessThan(10) < 7)
		{
			iRet = theClub.GetFirstTeamSquad().RandomPlayingPosition(true,MIDFIELDER);
		}
		if (iRet == NOPLAYER)
		{
			iRet = theClub.GetFirstTeamSquad().RandomPlayingPosition(true, ATTACKER);
		}
		if (iRet == NOPLAYER)
		{
			iRet = theClub.GetFirstTeamSquad().RandomPlayingPosition(true,MIDFIELDER);
		}
		if (iRet == NOPLAYER)
		{
			iRet = theClub.GetFirstTeamSquad().RandomPlayingPosition(true, DEFENDER);
		}
		EXT_ASSERT(iRet != NOPLAYER);
		pAssist = &theClub.GetPlayer(iRet);
	}
	while(pAssist == pScorer);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoScoredGoal
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const byte _When - time in minutes
	Parameter: 	const ushort _Scorer
	Parameter: 	const bool _bPenaltyFlag
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Returns:   	void
	Qualifier:
	Purpose:	A goal has been scored
------------------------------------------------------------------------------*/
void CFixture::DoScoredGoal(const eWhichTeam _Who, const byte _When, const ushort _Scorer, const bool _bPenaltyFlag, const sbyte _ShotSaveSkillDifference)
{
	CPlayer* pScorer = &GetClub(_Who).GetPlayer(_Scorer);
	GetMatchVars().SetLastScorer(pScorer);
	if (_bPenaltyFlag == false)
	{
		ushort Assister = DoCreateAssister(_Who, _Scorer);
#ifdef ASSISTS
		CPlayer* pAssist = &GetClub(_Who).GetPlayer(Assister);
        pAssist->AssistedGoal(static_cast<eMatchCompetitionType>(GetMatchType()));           // Update the assisters totals
#endif
		int AssistQuality = RandomNumber.IntLessThan(9);
		GetMatchVars().AssistedGoal(_Who, Assister, AssistQuality, _When);
		// Now the defender who let him through
		ushort Defender = DoCreateDefender(GetOpponent(_Who));
		GetMatchVars().PlayerConcededShot(GetOpponent(_Who), Defender, AssistQuality, _When);
		DoAddMatchEvent(_When, (eMatchEvent)(HOMEGOAL + _Who), *pScorer);
	}
	else
	{
		DoAddMatchEvent(_When, (eMatchEvent)(HOMEPENALTYGOAL + _Who), *pScorer);
	}
    pScorer->DoScoredGoal(static_cast<eMatchCompetitionType>(GetMatchType()));                      // Update the scorers totals
	GetMatchVars().PlayerScoredGoal(_Who, _Scorer, _ShotSaveSkillDifference, _When);
    DoIncrementShotsOnTarget(_Who);

	// The opposing goalkeeper has conceded a goal
	DoGoalkeeperConcededGoal(GetOpponent(_Who), 0, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::eOpponent
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const eWhichTeam
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CFixture::GetOpponent(const eWhichTeam _Who)
{
	eWhichTeam eOpp = HOME;
	if (_Who == HOME)
	{
		eOpp = AWAY;
	}
	return eOpp;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetUserOpponentClub
	Access:    	public
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CFixture::GetUserOpponentClub()
{
	if (&GetClub(HOME) == WorldData().GetCurrentUserClub())
	{
		return &GetClub(AWAY);
	}
	if (&GetClub(AWAY) == WorldData().GetCurrentUserClub())
	{
		return &GetClub(HOME);
	}
	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoGoalkeeperConcededGoal
	Access:    	public
	Parameter: 	eWhichTeam _Who
	Parameter: 	sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _When
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoGoalkeeperConcededGoal(eWhichTeam _Who, sbyte _ShotSaveSkillDifference, const byte _When)
{
	GetMatchVars().GoalkeeperAttemptSave(_Who, 0, _ShotSaveSkillDifference, _When);
    GetClub(_Who).GetPlayer(0).DoConcedeGoal(static_cast<eMatchCompetitionType>(GetMatchType()));         // Add goal conceded if is goalkeeper
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::OwnGoalScorerSquadNumber
	Access:    	public
	Parameter: 	eWhichTeam _Who
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::OwnGoalScorerSquadNumber(const eWhichTeam _Who)
{
	int PosnToFind = DEFENDER;
	ushort iRet;
	do
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingSquadNumber(static_cast<ePlayerPosition>(PosnToFind++));
	}
	while(iRet == NOPLAYER);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoTryScoreOwnGoal
	Access:    	public
	Parameter: 	const eWhichTeam _Who = home or away team
	Parameter: 	const byte _When - time in minutes
	Parameter: 	const ushort _ShotTaker
	Returns:   	const bool
	Qualifier:
	Purpose:	Has an own goal has been scored?
------------------------------------------------------------------------------*/
bool CFixture::DoTryScoreOwnGoal(const eWhichTeam _Who, const byte _When, const ushort _ShotTaker)
{
	if (RandomNumber.IntLessThan(CHANCEOWNGOAL) == 5)
	{
		DoIncrementShotsOnTarget(_Who);
		GetMatchVars().SetLastScorer(&GetClub(_Who).GetPlayer(_ShotTaker));
		DoAddMatchEvent(_When, (eMatchEvent)(HOMEOWNGOAL + _Who), *GetMatchVars().GetLastScorer());
		// The opposing goalkeeper has conceded a goal
		DoGoalkeeperConcededGoal(_Who, 0, _When);
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAddMatchEvent
	Access:    	public
	Parameter: 	const byte _Time - time in minutes
	Parameter: 	const enumMatchEvent _eType
	Parameter: 	CPersonName& _Name
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoAddMatchEvent(const byte _Time, const eMatchEvent _eType, CPersonName& _Name)
{
	EXT_ASSERT(m_TheEvents.DoCountEvents(HOMEGOAL) + m_TheEvents.DoCountEvents(AWAYGOAL) < MAXIMUMGOALSINMATCH);
	EXT_ASSERT(m_TheEvents.GetSize() < 150);
	m_TheEvents.Add(FD_BIN_NEW CMatchEvent(_Time, _eType, _Name));
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoScoredPenaltyGoal
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const byte _When - time in minutes
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoScoredPenaltyGoal(const eWhichTeam _Who, const byte _When)
{
    GetMatchVars().SetLastScorer(WorldData().GetPlayer(GetFirstTeamSquad(_Who).GetPenaltyTakerID()));
	DoAddMatchEvent(_When, (eMatchEvent)(HOMEPENALTYGOAL + _Who), *WorldData().GetPlayer(GetFirstTeamSquad(_Who).GetPenaltyTakerID()));
    GetFirstTeamSquad(_Who).GetPenaltyTaker()->DoScoredGoal(static_cast<eMatchCompetitionType>(GetMatchType()));         // Update the scorers totals
	//                                                                                    Av shot
	GetMatchVars().PlayerScoredGoal(_Who, GetFirstTeamSquad(_Who).PenaltyTakerSquadNumber(), 0, _When);
	// The opposing goalkeeper has conceded a goal
	DoGoalkeeperConcededGoal(GetOpponent(_Who), 0, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoScoredFreeKickGoal
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const ushort _ShotTakerSquadNumber - squad number of the player
	Parameter: 	const byte _When - time in minutes
	Returns:   	void
	Qualifier:
	Purpose:	A free kick goal has been scored
------------------------------------------------------------------------------*/
void CFixture::DoScoredFreeKickGoal(const eWhichTeam _Who, const ushort _ShotTakerSquadNumber, const byte _When)
{
    DoIncrementShotsOnTarget(_Who);

    CPlayer& Player = GetClub(_Who).GetPlayer(_ShotTakerSquadNumber);
    GetMatchVars().SetLastScorer(static_cast<CPersonName*>(&Player));
    DoAddMatchEvent(_When, (eMatchEvent)(HOMEGOAL + _Who), static_cast<CPersonName&>(Player));
    Player.DoScoredGoal(static_cast<eMatchCompetitionType>(GetMatchType()));         // Update the scorers totals
	//                                                        Av shot
	GetMatchVars().PlayerScoredGoal(_Who, _ShotTakerSquadNumber, 0, _When);
	// The opposing goalkeeper has conceded a goal
	DoGoalkeeperConcededGoal(GetOpponent(_Who), 0, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::PlayerShotOffTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const ushort _PlayerID - squad number of the player
	Parameter: 	const byte _When - time in minutes
	Returns:   	void
	Qualifier:
	Purpose:	A player has had a shot off target
------------------------------------------------------------------------------*/
void CFixture::PlayerShotOffTarget(const eWhichTeam _Who, const ushort _PlayerID, const byte _When)
{
	DoIncrementShotsOffTarget(_Who);
	GetMatchVars().DoShotOffTarget(_Who, _PlayerID, _When);
	DoUpdatePlayerShot(_Who, _PlayerID, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::PlayerShotOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const ushort _PlayerID - squad number of the player
	Parameter: 	const byte _When - time in minutes
	Returns:   	void
	Qualifier:
	Purpose:	A player has had a shot on target
------------------------------------------------------------------------------*/
void CFixture::PlayerShotOnTarget(const eWhichTeam _Who, const ushort _PlayerID, const byte _When)
{
	DoIncrementShotsOnTarget(_Who);
	GetMatchVars().DoShotOnTarget(_Who, _PlayerID, _When);
	DoUpdatePlayerShot(_Who, _PlayerID, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoUpdatePlayerShot
	Access:    	public
	Parameter: 	const eWhichTeam _Who - home or away team
	Parameter: 	const ushort _PlayerID - squad number of the player
	Parameter: 	const byte _When - time in minutes
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoUpdatePlayerShot(const eWhichTeam _Who, const ushort _PlayerID, const byte _When)
{
	ushort Assister = DoCreateAssister(_Who, _PlayerID);
	CPlayer* pAssist = &GetClub(_Who).GetPlayer(Assister);
	byte AssistQuality = RandomNumber.IntLessThan(9);
	GetMatchVars().AssistedShot(_Who, Assister, AssistQuality, _When);
	// Now the defender who let him through
	GetMatchVars().PlayerConcededShot(GetOpponent(_Who), DoCreateDefender(GetOpponent(_Who)), AssistQuality, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::PlayerDefendAttempt
	Access:    	public
	Parameter:	const eWhichTeam _Who - home or away team
	Parameter: 	const byte _When - time in minutes
	Returns:   	const eWhichTeam
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoPlayerDefendAttempt(const eWhichTeam _Who, const byte _When)
{
	byte AssistQuality = RandomNumber.IntLessThan(9);
	if (RandomNumber.IntLessThan(10) < 7)
	{
		// Defender broke the attack down
		GetMatchVars().DoPlayerDefendAttempt(_Who, DoCreateDefender(_Who, DEFENDER), AssistQuality, _When, false);
	}
	else
	{
		// Midfielder broke the attack down
		GetMatchVars().DoPlayerDefendAttempt(_Who, DoCreateDefender(_Who,MIDFIELDER), AssistQuality, _When, true);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoDetermineCurrentPossesion
	Access:    	public
	Returns:   	eWhichTeam
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CFixture::DoDetermineCurrentPossesion()
{
	eWhichTeam rWhichTeam = HOME;
#ifdef STOREMATCHPOSESSION
	GetClubData(m_TeamCurrentPosession).DoDecrementPosession();
#endif
	if (RandomNumber.IntLessThan(GetMidfielderSkill(HOME) + GetMidfielderSkill(AWAY)) > GetMidfielderSkill(HOME))
	{
		rWhichTeam = AWAY;
	}
#ifdef STOREMATCHPOSESSION
	GetClubData(m_TeamCurrentPosession).DoIncrementPosession();
#endif
	return rWhichTeam;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::HasGoalBeenScored
	Access:    	public
	Parameter: 	const int _Time
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::HasGoalBeenScored(const int _Time)
{
	CString sRet;
	if (_Time > 0 && m_TheEvents.HasGoalBeenScored(_Time) == true)
	{
		sRet = LatestScoreStr(_Time);
	}
	return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::LatestScoreStr
	Access:    	public
	Parameter: 	const uint _Time - time in match to bring score for
	Parameter: 	const bool _bClubNames
	Returns:   	CString - Format : - home club name, score, away club name
	Qualifier:
	Purpose:	Return the latest score of the match as a string
------------------------------------------------------------------------------*/
CString CFixture::LatestScoreStr(const uint _Time, const bool _bClubNames )
{
	CString sRet;
	if (_bClubNames == true)
	{
		sRet.Format(TEXT("%s %d - %d %s"), GetClub(HOME).GetName(), GetEventList()->HomeScore(_Time),
										 GetEventList()->AwayScore(_Time), GetClub(AWAY).GetName());
	}
	else
	{
		sRet.Format(TEXT("%d - %d"), GetEventList()->HomeScore(_Time), GetEventList()->AwayScore(_Time));
	}
	return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAfterMatchUpdate
	Access:    	public
	Parameter: 	const byte _HomePrimaryTactics
	Parameter: 	const byte _AwayPrimaryTactics
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoAfterMatchUpdate(const ushort _HomePrimaryTactics, const ushort _AwayPrimaryTactics)
{
    EXT_ASSERT(GetAttendance() != 0);
	bool bHomeAllowTeamOfWeek = true;
	if (GetLosingClubID() == GetHomeClubID() && DoCalculateGoalDifference() > 2)
	{
		bHomeAllowTeamOfWeek = false;
	}
	bool bAwayAllowTeamOfWeek = true;
	if (GetLosingClubID() == GetAwayClubID() && DoCalculateGoalDifference() > 2)
	{
		bAwayAllowTeamOfWeek = false;
	}
	for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		WorldData().GetPlayer(GetFirstTeamSquad(HOME).GetSquad(LoopCount))->DoIncrementAppearances(static_cast<eMatchCompetitionType>(GetMatchType()), GetClub(HOME), GetMatchLength());
		WorldData().GetPlayer(GetFirstTeamSquad(AWAY).GetSquad(LoopCount))->DoIncrementAppearances(static_cast<eMatchCompetitionType>(GetMatchType()), GetClub(AWAY), GetMatchLength());

		if (GetMatchType() == LEAGUEMATCH)
		{
			EXT_ASSERT_POINTER(GetMatchVars().GetDivision(), CDivision);
			CPlayerMatchDetails PlayerMatchDetails(&GetMatchVars());
			if (bHomeAllowTeamOfWeek == true)
			{
 				PlayerMatchDetails.DoInitialise(LoopCount, GetHomeClubID(), HOME);
				GetMatchVars().GetDivision()->GetTeamOfTheWeek().DoAfterMatchCheck(PlayerMatchDetails);
			}
			if (bAwayAllowTeamOfWeek == true)
			{
				PlayerMatchDetails.DoInitialise(LoopCount, GetAwayClubID(), AWAY);
				GetMatchVars().GetDivision()->GetTeamOfTheWeek().DoAfterMatchCheck(PlayerMatchDetails);
			}
		}
	}

#ifdef	ACTIVEREFEREES
    GetReferee()->AfterMatchUpdate(TotalNumberOfPlayersBooked(), TotalNumberOfPlayersSentOff (), GetMatchType());
#endif
	int AwayGateReceiptsPercentage = 0;
	int FAGateReceiptsPercentage = 0;
	switch (GetMatchType())
	{
		case LEAGUEMATCH:
			AwayGateReceiptsPercentage = GetMatchVars().GetDivision()->GetPercentageAwayGateReceipts();
			FAGateReceiptsPercentage = GetMatchVars().GetDivision()->GetPercentageFAGateReceipts();
			break;

		case CUPMATCH:
			AwayGateReceiptsPercentage = GetMatchVars().GetCup()->GetPercentageAwayGateReceipts();
			FAGateReceiptsPercentage = GetMatchVars().GetCup()->GetPercentageFAGateReceipts();
			break;
	}
	GetClub(HOME).DoAfterMatchUpdate(_HomePrimaryTactics, *this, HOME, AwayGateReceiptsPercentage, FAGateReceiptsPercentage);
	GetClub(AWAY).DoAfterMatchUpdate(_AwayPrimaryTactics, *this, AWAY, AwayGateReceiptsPercentage, FAGateReceiptsPercentage);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetOverallPlayerPerformance
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _PlayerSquadNumber
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetOverallPlayerPerformance(const eWhichTeam _Who, const ushort _PlayerSquadNumber)
{
	return GetMatchVars().GetSquadPerformance(_Who).GetOverallRating(_PlayerSquadNumber, GetMatchLength());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoUpdatePlayerPerformances
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoUpdatePlayerPerformances(const eWhichTeam _Who)
{
	CPlayerPerformanceList& SquadPerformance = GetMatchVars().GetSquadPerformance(_Who);

	// Calculate all the outfield player effective skills
    for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount ++)
	{
        GetClub(_Who).GetPlayer(LoopCount).DoAddPerformance(SquadPerformance.GetOverallRating(LoopCount, GetMatchLength()));
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateTeamStrengths
	Access:    	public
	Parameter: 	const byte _Time
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoCalculateTeamStrengths(const byte _Time )
{
    DoCalculateTeamStrength(HOME, _Time);
    DoCalculateTeamStrength(AWAY, _Time);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoProcessMatch
	Access:    	public
	Parameter: 	CFixtureEvents & _TheEvents
	Parameter: 	const uint _MatchLength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoProcessMatch(CFixtureMatchEvents& _TheEvents, const uint _MatchLength)
{
	EXT_ASSERT(_MatchLength <= _TheEvents.GetSize());
	for (uint Time = 0; Time < _MatchLength; Time++)
	{
		switch(_TheEvents[Time])
		{
			case HOMEGOAL:
				DoGoalAttempt(Time + (FULLTIME * GetExtraTimeFlag()), HOME);
				break;
			case AWAYGOAL:
				DoGoalAttempt(Time + (FULLTIME * GetExtraTimeFlag()), AWAY);
				break;
			case HOMEBOOKING:
	            DoDisciplinePlayer(HOME, BOOKPLAYER, Time + (FULLTIME * GetExtraTimeFlag()));
				if (RandomNumber.IntLessThan(CHANCEBOOKINGPENALTY) == 2)
				{
					DoScoredPenaltyGoal(AWAY, Time + (FULLTIME * GetExtraTimeFlag()));
				}
				break;
			case AWAYBOOKING:
	            DoDisciplinePlayer(AWAY, BOOKPLAYER, Time + (FULLTIME * GetExtraTimeFlag()));
				if (RandomNumber.IntLessThan(CHANCEBOOKINGPENALTY) == 2)
				{
					DoScoredPenaltyGoal(HOME, Time + (FULLTIME * GetExtraTimeFlag()));
				}
				break;
			case HOMESENDINGOFF:
	            DoDisciplinePlayer(HOME, SENDPLAYEROFF, Time + (FULLTIME * GetExtraTimeFlag()));
				if (RandomNumber.IntLessThan(CHANCESENDINGOFFPENALTY) == 2)
				{
					DoScoredPenaltyGoal(AWAY, Time + (FULLTIME * GetExtraTimeFlag()));
				}
				DoCalculateMatchEvents(Time, _TheEvents);
				break;
			case AWAYSENDINGOFF:
	            DoDisciplinePlayer(AWAY, SENDPLAYEROFF, Time + (FULLTIME * GetExtraTimeFlag()));
				if (RandomNumber.IntLessThan(CHANCESENDINGOFFPENALTY) == 2)
				{
					DoScoredPenaltyGoal(HOME, Time + (FULLTIME * GetExtraTimeFlag()));
				}
				DoCalculateMatchEvents(Time, _TheEvents);
				break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoPlayMatch
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Match Loop if not a viewed match
------------------------------------------------------------------------------*/
void CFixture::DoPlayMatch(bool _PenaltyShootOut /*= false*/)
{
	CFixtureMatchEvents TheEvents;
	DoInitialiseMatch(TheEvents);
	DoCalculateTeamStrengths();
	DoCalculateMatchEvents(0, TheEvents);

	// Process the match
	DoProcessMatch(TheEvents, GetMatchLength());
	//@afxDump << "Actual Goal Attempts " << m_ShotsOnTarget[HOME] << " " << GetNumberGoals(HOME) << " " << m_ShotsOnTarget[AWAY] << " " << GetNumberGoals(AWAY) << "\n";

	if (GetMatchType() == LEAGUEMATCH)
    {
		switch(GetMatchVars().GetDivision()->GetPointsForWin())
		{
			case JLEAGUEPOINTS:
				if (IsMatchDrawn() == true)
				{
					DoExtraTime();
				}
				if (IsMatchDrawn() == true)
				{
					DoPenaltyShootOut();
				}
				break;
			case MLSPOINTS:
				if (IsMatchDrawn() == true)
				{
					DoPenaltyShootOut();
				}
				break;
		}
	}
	else if (_PenaltyShootOut == true && IsMatchDrawn() == true)
	{
		DoExtraTime();
		if (IsMatchDrawn() == true)
		{
			DoPenaltyShootOut();
		}
	}

	DoCalculateMatchStatistics();
#ifdef STORE_MAN_OF_MATCH
	DoSelectManOfMatch();
#endif
	DoCalculateInjuryActions(HOME);
	DoCalculateInjuryActions(AWAY);
	DoAfterMatchUpdate(GetClub(HOME).GetCurrentTacticsID(), GetClub(AWAY).GetCurrentTacticsID());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoShowSummary
	Access:    	public
	Parameter: 	const bool _bShowSummary
	Parameter: 	CCup* _pCup
	Returns:   	void
	Qualifier:
	Purpose:	Show the match summary screen,
				shows the score, attendance, goalscorers etc.
------------------------------------------------------------------------------*/
void CFixture::DoShowSummary(const bool _bShowSummary, CCup* _pCup)
{
	if (IsUserMatch() == true && _bShowSummary == true)
	{
		DoShowSummary(_pCup);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoShowSummary
	Access:    	public
	Parameter: 	CCup* _pCup
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoShowSummary(CCup* _pCup)
{
	if (HasMatchBeenPlayed() == true)
	{
		theApp.OnShowMatchSummary(_pCup, this);
#ifdef NDS
    	ProcessingCallback( "PostSummary" );
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoTakePenaltyShootoutShot
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _ShotTaker
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::DoTakePenaltyShootoutShot(const eWhichTeam _Who, const ushort _ShotTaker)
{
    if (DoTryScoreGoal(_Who,  _ShotTaker, 255, false) == true)
	{
        DoAddMatchEvent(255, (eMatchEvent)(HOMEPENALTYSHOOTOUTGOAL + _Who), static_cast<CPersonName&>(GetClub(_Who).GetPlayer(_ShotTaker)));
		return true;
    }
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoPenaltyShootOut
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoPenaltyShootOut()
{
    uint HomeScored = 0;
    uint AwayScored = 0;
    CNumericArray<ushort> HomePenaltySquadIdList;
    CNumericArray<ushort> AwayPenaltySquadIdList;

    // Build squad list	in reverse order(i.e. player 0 is team player 10)
	short LoopCount;
    for (LoopCount = PLAYERSINTEAM - 1; LoopCount > 0; LoopCount--)
	{
        if (WorldData().GetPlayer(GetFirstTeamSquad(HOME).GetSquad(LoopCount))->IsPlaying() == true)
		{
            HomePenaltySquadIdList.Add(LoopCount);
        }
        if (WorldData().GetPlayer(GetFirstTeamSquad(AWAY).GetSquad(LoopCount))->IsPlaying() == true)
		{
            AwayPenaltySquadIdList.Add(LoopCount);
        }
    }

	// Take the shots, in team player decreasing order
    for (LoopCount = 0; LoopCount < MAXPENALTYSHOOTOUTSHOTS; LoopCount++)
	{
        // Home
        HomeScored += DoTakePenaltyShootoutShot(HOME, HomePenaltySquadIdList[0]);
        HomePenaltySquadIdList.RemoveAt(0);

        // Away
        AwayScored += DoTakePenaltyShootoutShot(AWAY, AwayPenaltySquadIdList[0]);
        AwayPenaltySquadIdList.RemoveAt(0);

        // Check if either side has an unassailable lead
        if (HomeScored > AwayScored + (MAXPENALTYSHOOTOUTSHOTS - LoopCount) || \
			AwayScored > HomeScored + (MAXPENALTYSHOOTOUTSHOTS - LoopCount))
		{
            break;
        }
    }
    // If still tied then continue until a player misses
    while(HomeScored == AwayScored)
	{
        // Is the next penalty the last one?
        if (HomePenaltySquadIdList.GetSize() == 1 || AwayPenaltySquadIdList.GetSize() == 1)
		{
            // Award home team a goal
            HomeScored++;
			DoAddMatchEvent(255, HOMEPENALTYSHOOTOUTGOAL, static_cast<CPersonName&>(*WorldData().GetPlayer(GetFirstTeamSquad(HOME).GetSquad(HomePenaltySquadIdList[0]))));
        }
		else
		{
			// Home
			HomeScored += DoTakePenaltyShootoutShot(HOME, HomePenaltySquadIdList[0]);
			HomePenaltySquadIdList.RemoveAt(0);

	        // Away
		    AwayScored += DoTakePenaltyShootoutShot(AWAY, AwayPenaltySquadIdList[0]);
			AwayPenaltySquadIdList.RemoveAt(0);
		}

    }
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoExtraTime
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Play extra time
------------------------------------------------------------------------------*/
void CFixture::DoExtraTime()
{
    int ExtraTimeLength = 31 + RandomNumber.IntLessThan(2);
    CFixtureMatchEvents TheEvents;
    TheEvents.SetSize(ExtraTimeLength);
	DoInitialiseExtraTime(ExtraTimeLength);
    DoCalculateMatchEvents(0, TheEvents);

	DoProcessMatch(TheEvents, ExtraTimeLength);	// Process the match

	for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		GetFirstTeamSquad(HOME).GetPlayer(LoopCount).SetHowLongPlayedLastMatch(GetMatchLength() + ExtraTimeLength);
		GetFirstTeamSquad(AWAY).GetPlayer(LoopCount).SetHowLongPlayedLastMatch(GetMatchLength() + ExtraTimeLength);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::InitExtraTime
	Access:    	public
	Parameter: 	const byte _ExtraTimeLength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoInitialiseExtraTime(const byte _ExtraTimeLength)
{
    SetExtraTimeFlag(true);
	GetMatchVars().DoInitialiseExtraTime(_ExtraTimeLength);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateMatchEvents
	Access:    	public
	Parameter: 	const byte _CurrentTime
	Parameter: 	CNumericArray<byte>& _TheEvents
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoCalculateMatchEvents(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents)
{
	if (_CurrentTime < (GetMatchLength() - 1))
	{
		_TheEvents.FillWithValue(0, _CurrentTime + 1);			// Clear future events
		DoCalculateGoalAttempts(_CurrentTime + 1, _TheEvents);
		// If is a non user team and going to cruise it then drop aggression to low
		DoAdjustAggression(GetClub(HOME), GetMatchVars().GetMatchVarsTeamData(HOME), GetMatchVars().GetMatchVarsTeamData(AWAY));
		DoAdjustAggression(GetClub(AWAY), GetMatchVars().GetMatchVarsTeamData(AWAY), GetMatchVars().GetMatchVarsTeamData(HOME));
		DoCalculateDisciplinaryActions(_CurrentTime + 1, _TheEvents);	// Decide how many players will be booked or sent off and when this game
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAdjustAggression
	Access:    	public
	Parameter: 	CClub& _Club
	Parameter: 	CMatchVarsData& _MatchVarsData
	Parameter: 	CMatchVarsData& _OpponentData
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoAdjustAggression(CClub& _Club, CMatchVarsData& _MatchVarsData, CMatchVarsData& _OpponentData)
{
	if (_Club.IsUserControlled() == false)
	{
		if (_MatchVarsData.GetGoalAttempts() > 3 && _OpponentData.GetGoalAttempts() < 1)
		{
			// If is a non user team and going to cruise it then drop aggression to low
			_MatchVarsData.GetTeamTactic().SetAggression(3);
		}
		else if (_MatchVarsData.GetGoalAttempts() < 2 && _OpponentData.GetGoalAttempts() > 4)
		{
			// If is a non user team and struggling then raise aggression
			_MatchVarsData.GetTeamTactic().SetAggression(9);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoGoalAttempt
	Access:    	public
	Parameter: 	const byte _CurrentTime
	Parameter: 	eWhichTeam _Who
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::DoGoalAttempt(const byte _CurrentTime, eWhichTeam _Who)
{
  if (GetNumberGoals(HOME) + GetNumberGoals(AWAY) < (MAXIMUMGOALSINMATCH - 2) )
	{
		if (DoTryScoreOwnGoal(_Who, _CurrentTime, OwnGoalScorerSquadNumber(GetOpponent(_Who))) == true)
		{
			return true;
		}
		return DoTryScoreGoal(_Who, DoSelectShotTaker(_Who), _CurrentTime, true);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoTryScoreGoal
  Access:     public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _ShotTaker
	Parameter: 	const byte _CurrentTime
	Parameter: 	const bool _bAllowHeaders
	Returns:   	const bool
	Qualifier:
	Purpose:	Does the goal attempt end in a goal?
------------------------------------------------------------------------------*/
bool CFixture::DoTryScoreGoal(const eWhichTeam _Who, const ushort _ShotTaker, const byte _CurrentTime, const bool _bAllowHeaders)
{
    byte SaveSkill = GetGoalKeeperSkill(_Who);
    byte ShotSkill;
    byte ShotDiff;
	bool bRet = false;

	// Shot or header?
	if (_bAllowHeaders == true && RandomNumber.IntLessThan(10) >= HEADERCHANCE)
	{
		ShotSkill = GetClub(_Who).GetPlayer(_ShotTaker).DoCalculateEffectiveSkill(SKILL_HEADING);
	}
	else
	{
		ShotSkill = GetClub(_Who).GetPlayer(_ShotTaker).DoCalculateEffectiveSkill(SKILL_SHOOTING);
	}

	// Try to produce an upset
	switch(GetMatchVars().GetMatchVarsTeamData(HOME).GetDayType())
	{
		case GOODDAY:
			if (_Who == HOME)
			{
				ShotSkill *= 2;
			}
			else
			{
				SaveSkill *= 2;
			}
			break;
		case BADDAY:
			if (_Who == HOME)
			{
				ShotSkill /= 2;
			}
			else
			{
				SaveSkill /= 2;
			}
			break;
	}
	switch(GetMatchVars().GetMatchVarsTeamData(AWAY).GetDayType())
	{
		case GOODDAY:
			if (_Who == AWAY)
			{
				ShotSkill *= 2;
			}
			else
			{
				SaveSkill *= 2;
			}
			break;
		case BADDAY:
			if (_Who == AWAY)
			{
				ShotSkill /= 2;
			}
			else
			{
				SaveSkill /= 2;
            }
			break;
	}
    // Does the shot succeed?
    ShotDiff = RandomNumber.IntLessThan(ShotSkill + SaveSkill + SHOTADVANTAGE);
    if (ShotDiff > SaveSkill)
	{
        // Current time is 255 (unsigned byte) during penalty shoot out
        if (_CurrentTime != 255)
		{
            DoScoredGoal(_Who, _CurrentTime, _ShotTaker, false, ShotDiff - SaveSkill);
        }
        bRet= true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoSelectShotTaker
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::DoSelectShotTaker(const eWhichTeam _Who)
{
	ushort ShotTaker;
	ushort TotalSum = GetMatchVars().GetShootingSkills(_Who);
	CClub& theClub = GetClub(_Who);
	do
	{
		ShotTaker = GetMatchVars().NearestAboveShootingSkills(_Who, RandomNumber.IntLessThan(TotalSum));
		++ShotTaker;			// Goalkeeper has no entry
	}
	while(theClub.GetPlayer(ShotTaker).IsPlaying() == false || \
			(theClub.GetPlayer(ShotTaker).GetSkills().GetPosition() ==MIDFIELDER && RandomNumber.IntLessThan(3) == 2));
	return ShotTaker;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAdjustGoalAttemptsForMorale
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CFixture::DoAdjustGoalAttemptsForMorale(const eWhichTeam _Who)
{
	uint Attempts = 0;
	int ra = 1 + RandomNumber.IntLessThan(3);
	// Chance of an extra shot if team morale is high
	if (ra == 1 && GetMatchVars().GetTeamMorale(_Who) > 80)
	{
		Attempts++;
	}
	if (GetMatchVars().GetTeamMorale(_Who) > 90)
	{
		Attempts++;
	}
	// Chance of a shot less if team morale is low
	if (ra == 3 && GetMatchVars().GetTeamMorale(_Who) < 30)
	{
		Attempts--;
	}
	return Attempts;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateGoalAttempts
	Access:    	public
	Parameter: 	const byte _CurrentTime
	Parameter: 	CFixtureEvents& _TheEvents
	Returns:   	void
	Qualifier:
	Purpose:	Calculate how many goal attempts and at what times
------------------------------------------------------------------------------*/
void CFixture::DoCalculateGoalAttempts(const byte _CurrentTime, CFixtureMatchEvents& _TheEvents)
{
#ifdef TESTBED
	// Add temperment, morale, and skills
//	theApp.GetTestBed().GetFixtureStats().AddCount(, AwayGoalAttempts);
#endif

	CMatchVarsData&	HomeClubMatchVars = GetMatchVars().GetMatchVarsTeamData(HOME);
	CMatchVarsData&	AwayClubMatchVars = GetMatchVars().GetMatchVarsTeamData(AWAY);

	// Calculate home goal attempts
    int HomeGoalAttempts = ((GetAttackerSkill(HOME) >= GetMidfielderSkill(HOME)) * GetMidfielderSkill(HOME)
        + (GetAttackerSkill(HOME) < GetMidfielderSkill(HOME)) * GetAttackerSkill(HOME) - GetDefenderSkill(AWAY) / 2) / 10;
	EXT_ASSERT(HomeGoalAttempts <= 20);

	HomeGoalAttempts++;
    if ((GetMatchVars().GetTemperament(AWAY) - GetMatchVars().GetTemperament(HOME)) > 29)
	{
		HomeGoalAttempts++;
	}
	EXT_ASSERT(HomeGoalAttempts <= 20);

	// Calculate away goal attempts
	int ra = 1 + RandomNumber.IntLessThan(5);
	int AwayGoalAttempts = ((GetAttackerSkill(AWAY) >= GetMidfielderSkill(AWAY)) * GetMidfielderSkill(AWAY) \
        + (GetAttackerSkill(AWAY) < GetMidfielderSkill(AWAY)) * GetAttackerSkill(AWAY) - GetDefenderSkill(HOME) / 2) / 10;
	EXT_ASSERT(AwayGoalAttempts <= 20);
	if (GetDefenderSkill(HOME) == GetMidfielderSkill(HOME) && GetMidfielderSkill(HOME) == GetAttackerSkill(HOME))
	{
		AwayGoalAttempts--;
	}
	if ((GetMatchVars().GetTemperament(HOME) - GetMatchVars().GetTemperament(AWAY)) > 29)
	{
		AwayGoalAttempts++;
	}

    // If very weak attack or no attack then halve the number of shots
    if (GetAttackerSkill(HOME) < 15)
	{
		HomeGoalAttempts /= 2;
	}
    if (GetAttackerSkill(AWAY) < 15)
	{
        AwayGoalAttempts /= 2;
	}
	EXT_ASSERT(AwayGoalAttempts <= 20);

	HomeGoalAttempts += DoAdjustGoalAttemptsForMorale(HOME);
	AwayGoalAttempts += DoAdjustGoalAttemptsForMorale(AWAY);

	// Long ball / passing bias
	HomeGoalAttempts += DoAdjustGoalAttemptsForLongBallPassing(HOME, HomeClubMatchVars.GetTeamTactic().GetMarkingStyle());
	AwayGoalAttempts += DoAdjustGoalAttemptsForLongBallPassing(AWAY, AwayClubMatchVars.GetTeamTactic().GetMarkingStyle());
	EXT_ASSERT(HomeGoalAttempts <= 20 && AwayGoalAttempts <= 20);

	// Midfield player flair
	HomeGoalAttempts += GetFirstTeamSquad(HOME).DoCountGoodFlairMidfieldersPlaying();
	AwayGoalAttempts += GetFirstTeamSquad(AWAY).DoCountGoodFlairMidfieldersPlaying();
	EXT_ASSERT(HomeGoalAttempts <= 20 && AwayGoalAttempts <= 20);

	// Try to produce an upset
	switch(HomeClubMatchVars.GetDayType())
	{
		case GOODDAY:
			if (HomeGoalAttempts < AwayGoalAttempts)
			{
				HomeGoalAttempts = AwayGoalAttempts + RandomNumber.IntLessThan(3);
			}
			else
			{
				// No need for good day if going to win anyway
				HomeClubMatchVars.SetDayType(NODAYTYPE);
			}
			break;
		case BADDAY:
			if (HomeGoalAttempts > AwayGoalAttempts)
			{
				HomeGoalAttempts -= RandomNumber.IntLessThan(3);
			}
			else
			{
				// No need for bad day if going to lose anyway
				HomeClubMatchVars.SetDayType(NODAYTYPE);
			}
			break;
	}
	switch(AwayClubMatchVars.GetDayType())
	{
		case GOODDAY:
			if (HomeGoalAttempts > AwayGoalAttempts)
			{
				AwayGoalAttempts += RandomNumber.IntLessThan(3);
			}
			else
			{
				// No need for good day if going to win anyway
				AwayClubMatchVars.SetDayType(NODAYTYPE);
			}
			break;
		case BADDAY:
			if (HomeGoalAttempts < AwayGoalAttempts)
			{
				AwayGoalAttempts -= RandomNumber.IntLessThan(3);
			}
			else
			{
				// No need for bad day if going to lose anyway
				AwayClubMatchVars.SetDayType(NODAYTYPE);
			}
			break;
	}
	EXT_ASSERT(HomeGoalAttempts <= 20);
	EXT_ASSERT(AwayGoalAttempts <= 20);

    if (GetExtraTimeFlag() == true)
	{
        // Extra time lasts as long as a 1/3 of a full match
        HomeGoalAttempts /= 3;
        AwayGoalAttempts /= 3;
    }
	EXT_ASSERT(HomeGoalAttempts <= 20);
	EXT_ASSERT(AwayGoalAttempts <= 20);

    // Calculate GoalAttempt Times
	ushort time;
    for (int LoopCount = 0; LoopCount < max(HomeGoalAttempts, AwayGoalAttempts); LoopCount++)
	{
		if (LoopCount < HomeGoalAttempts)
		{
			// Generate random goal time
			ushort nTime = GetMatchVars().DoCalculateShotTime(_TheEvents.GetSize());
			if (nTime > _CurrentTime)
			{
				ushort nEndtime = nTime + GetMatchVars().GetMatchTimeSectorSize();
				if (nEndtime > _TheEvents.GetSize())
				{
					nEndtime = _TheEvents.GetUpperBound();
				}
				if (nTime >= nEndtime)
				{
					time = _TheEvents.RandomPositionOfValue(NOEVENT, nTime, nEndtime);
				}
				else
				{
					time = nEndtime;
				}
				if (nTime > _CurrentTime)
				{
					_TheEvents.SetEvent(time, HOMEGOAL);
				}
			}
		}
		if (LoopCount < AwayGoalAttempts)
		{
			// Generate random goal time
			if (_TheEvents.RandomPosition() > _CurrentTime)
			{
				time = _TheEvents.DoFindRandomFreeEvent();
				if (time > _CurrentTime)
				{
					_TheEvents.SetEvent(time, AWAYGOAL);
				}
			}
		}
    }
	HomeGoalAttempts = max(0, HomeGoalAttempts);
	HomeClubMatchVars.SetGoalAttempts(HomeGoalAttempts);
	AwayGoalAttempts = max(0, AwayGoalAttempts);
	AwayClubMatchVars.SetGoalAttempts(AwayGoalAttempts);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoDefenderManOfMatch
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::DoDefenderManOfMatch(const eWhichTeam _Who)
{
	ushort iRet = NOPLAYER;
	if (RandomNumber.IntLessThan(100) < 75)
	{
		iRet = GetFirstTeamSquad(_Who).BestPlayingPosition(false, DEFENDER);
	}
	else
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(false, DEFENDER);
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoMidfielderManOfMatch
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::DoMidfielderManOfMatch(const eWhichTeam _Who)
{
	ushort iRet = NOPLAYER;
	if (RandomNumber.IntLessThan(100) < 75)
	{
		iRet = GetFirstTeamSquad(_Who).BestPlayingPosition(false,MIDFIELDER);
	}
	else
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(false,MIDFIELDER);
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoAttackerManOfMatch
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::DoAttackerManOfMatch(const eWhichTeam _Who)
{
	ushort iRet = NOPLAYER;
	if (RandomNumber.IntLessThan(100) < 75)
	{
		iRet = GetFirstTeamSquad(_Who).BestPlayingPosition(false, ATTACKER);
	}
	else
	{
		iRet = GetFirstTeamSquad(_Who).RandomPlayingPosition(false, ATTACKER);
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::HatTrickPlayer
	Access:    	public
	Parameter: 	eWhichTeam _Who
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixture::HatTrickPlayer(const eWhichTeam _Who)
{
	ushort iRet = NOPLAYER;
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::IsDerby
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::IsDerby()
{
	return static_cast<bool>(GetClub(HOME).GetRegionID() == GetClub(AWAY).GetRegionID());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoInitialiseMatch
	Access:    	public
	Parameter: 	CNumericArray<byte>& _TheEvents
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoInitialiseMatch(CFixtureMatchEvents& _TheEvents)
{
	GetMatchVars().DoResetVariables();
    SetExtraTimeFlag(false);
	GetMatchVars().SetHomeAwayClubs(*this);

	CClub& HomeClub = GetClub(HOME);
	CClub& AwayClub = GetClub(AWAY);

#ifdef	ACTIVEREFEREES
	if (HomeClub.GetCountry() == AwayClub.GetCountry())
	{
		// Domestic match
		m_RefereeNum = HomeClub.GetCountry()->AllocateReferee(min(HomeClub.GetWhichDivision(), AwayClub.GetWhichDivision()));
		EXT_ASSERT(m_RefereeNum != 0);
	}
	else
	{
		// Federation match
		pCountry = HomeClub.GetFederation()->GetRandomCountry(HomeClub.GetCountry(), AwayClub.pCountry());
		m_RefereeNum = pCountry->AllocateTopReferee();
	}
#endif
	SetMatchLength(91 + RandomNumber.IntLessThan(4));            // How many minutes injury time to play
	_TheEvents.SetSize(GetMatchLength());
	SetTicketPrice(HomeClub.GetTicketPrice());

	// Non user team selection
	GetMatchVars().DoInitialiseClubs(GetSubsSelect(),  GetMatchType());

    switch(GetMatchType())
	{
		case LEAGUEMATCH:
			DoCalculateAttendance(GetMatchVars().GetDivision()->GetMatchType(WorldData().GetCurrentDate().GetMatchDay()),
					HomeClub.GetTableData().GetLeaguePosition(), AwayClub.GetTableData().GetLeaguePosition(),
					GetMatchVars().GetDivision()->GetNumberOfClubsInDivision(), GetMatchVars().GetDivision()->GetNumberOfClubsInDivision(),
					HomeClub.GetStadium()->GetCapacity());
            HomeClub.GetSeasonStats().DoAddTotalAttend(GetAttendance());
			break;

		case CUPMATCH:
            DoCalculateAttendance(5, 1, 1, 30, 30, GetClub(HOME).GetStadium()->GetCapacity());
            HomeClub.GetSeasonStats().DoAddTotalAttend(GetAttendance());
			break;

		case REPLAYMATCH:
            DoCalculateAttendance(5, 1, 1, 30, 30, GetMatchVars().GetMatchVarsTeamData(HOME).GetClub().GetStadium()->GetCapacity());
            HomeClub.GetSeasonStats().DoAddTotalAttend(GetAttendance());
			break;

		case CUPMATCHNEUTRALVENUE:
            EXT_ASSERT(GetAttendance() > 0);      // attendance is venue capacity
			if (GetMatchVars().GetCupRound()->IsFinal() == true)
			{
				SetAttendance((GetAttendance() / 100) * (90 + RandomNumber.IntLessThan(10)));
			}
			else
			{
				SetAttendance((GetAttendance() / 4) * 3 + RandomNumber.IntLessThan(GetAttendance() / 4));
			}
			break;

		case FRIENDLYMATCH:
            DoCalculateAttendance(5, 10, 10, 10, 10, HomeClub.GetStadium()->GetCapacity());
			SetAttendance(GetAttendance() / 3);
			break;

		default:
			EXT_ASSERT(false);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateAttendance
	Access:    	public
	Parameter: 	const byte _WeekNum
	Parameter: 	const byte _lpos1
	Parameter: 	const byte _lpos2
	Parameter: 	const byte _HomeNumInDiv
	Parameter: 	const byte _AwayNumInDiv
	Parameter: 	const uint _Capacity
	Returns:   	void
	Qualifier:
	Purpose:	Calculate attendance and gate money for a League match
------------------------------------------------------------------------------*/
void CFixture::DoCalculateAttendance(const byte _WeekNum, const byte _lpos1, const byte _lpos2, const ushort _HomeNumInDiv, const ushort _AwayNumInDiv, const uint _Capacity)
{
	EXT_ASSERT(_Capacity > 0);
    // if ticket price is ridiculously high then ensure a poor attendance!!
    if (GetTicketPrice() > 99.9)
	{
		EXT_ASSERT(false);
        SetAttendance(1);
		return;
    }

	CClub& HomeClub = GetClub(HOME);
	CClub& AwayClub = GetClub(AWAY);

	uint nBaseAttendance = HomeClub.GetPopularity().GetBaseAttendance();
	uint nAwayBaseAttendance = AwayClub.GetPopularity().GetBaseAttendance();

    // 50% home average +rnd 25%
    uint howmany = nBaseAttendance / 2 + RandomNumber.IntLessThan(nBaseAttendance / 4);

    // 40% away average +rnd 20%
    howmany += (int) (nAwayBaseAttendance / 2.5) + RandomNumber.IntLessThan(nAwayBaseAttendance / 5);

    // add 10% if home club in top 3 and played at least 3 games
    // minus 10% if home club in bottom 3 and played at least 3 games
    if (_WeekNum > 2 )
	{
        // Home club 2nd or 3rd
        if (_lpos1 > 1 && _lpos1 < 4 )
		{
            howmany += nBaseAttendance / 10;
        }
        // Home club league leaders ?
        if (_lpos1 == 1 )
		{
            howmany += nBaseAttendance / 5;
        }
        // Away club in top 3 ?
        if (_lpos2 < 4 )
		{
            howmany += nAwayBaseAttendance / 12;
        }
        // Home club in bottom 3 ?
        if (_lpos1 > _HomeNumInDiv - 3 )
		{
            howmany -= nBaseAttendance / 10;
        }
        if (_lpos2 > _AwayNumInDiv - 3 )
		{
            howmany -= nAwayBaseAttendance / 12;
        }
        if (_lpos1 < 4 && _lpos2 < 4 )
		{
            howmany += howmany / 10;
        }
        if (_lpos2 == 1 )
		{
            howmany += nAwayBaseAttendance / 10;
        }
    }
    else
	{
        howmany += RandomNumber.IntLessThan(howmany / 10);
    }

    // Adjust for local derby
    howmany += RandomNumber.IntLessThan(howmany / 5) * IsDerby();

	if (HomeClub.GetPopularityID() > AwayClub.GetPopularityID())
	{
        howmany -= RandomNumber.IntLessThan(howmany / 10);
    }

	// Adjust ticket price depending on match profile and ticket price
	if (HomeClub.IsNonLeagueClub() == false)
	{
		howmany = HomeClub.GetDivision().GetDivisionStrength().DoAdjustAttendanceForTicketPrice(howmany, GetTicketPrice());
	}
	else
	{
		// Default to this if at non league ground
		howmany = WorldData().GetDivisionStrengthList().GetAt(3).DoAdjustAttendanceForTicketPrice(howmany, GetTicketPrice());
	}

	EXT_ASSERT(howmany > 50);
    // Make sure ground capacity is ok
    SetAttendance(max((uint)1, min((uint)(_Capacity - RandomNumber.IntLessThan(30)), howmany)));
 }


/*------------------------------------------------------------------------------
	Method:   	CFixture::StadiumName
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::StadiumName()
{
	CString strName;
	switch(GetMatchType())
	{
		case FRIENDLYMATCH:
		case LEAGUEMATCH:
		case CUPMATCH:
		case REPLAYMATCH:
			strName = GetClub(HOME).GetStadium()->GetName();
			break;

		case CUPMATCHNEUTRALVENUE:
			strName = WorldData().DoFindNeutralVenueNameForFixture(this);
			break;

		default:
			EXT_ASSERT(false);
	}
	return strName;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GoalsStr
	Access:    	public
	Parameter: 	const int _CurrentTime
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::GoalsStr(const int _CurrentTime)
{
	CString str;
	str.Format(_T("%d-%d"), GetEventList()->HomeScore(_CurrentTime), GetEventList()->AwayScore(_CurrentTime));
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetNumberGoals
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const int _CurrentTime
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetNumberGoals(const eWhichTeam _Who, const int _CurrentTime /*= 200*/)
{
	if (_Who == HOME)
	{
		return  GetEventList()->HomeScore(_CurrentTime);
	}
	else
	{
		return GetEventList()->AwayScore(_CurrentTime);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoCalculateGoalDifference
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::DoCalculateGoalDifference()
{
	return GetEventList()->DoCalculateGoalDifference();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::TotalNumberOfPlayersSentOff
	Access:    	public
	Parameter: 	void
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::TotalNumberOfPlayersSentOff()
{
	return GetNumberOfPlayersSentOff(HOME) + GetNumberOfPlayersSentOff(AWAY);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetNumberOfPlayersSentOff
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetNumberOfPlayersSentOff(const eWhichTeam _Who)
{
	if (_Who == HOME)
	{
		return  GetEventList()->HomeNumberPlayersSentOff();
	}
	else
	{
		return GetEventList()->AwayNumberPlayersSentOff();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetNumberOfPlayersBooked
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetNumberOfPlayersBooked(const eWhichTeam _Who)
{
	if (_Who == HOME)
	{
		return  GetEventList()->HomeNumberPlayersBooked();
	}
	else
	{
		return GetEventList()->AwayNumberPlayersBooked();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchDate
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDate CFixture::GetMatchDate()
{
	return m_Data.m_MatchDate;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::HasMatchBeenPlayed
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::HasMatchBeenPlayed()
{
	if (GetMatchLength() < 1)
	{
		return false;				// Match not started yet
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::AddToCurrentNonUserFixtureList
	Access:    	public
	Parameter: 	CFixtureList& _LatestScores
	Parameter: 	CDate& _Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::AddToCurrentNonUserFixtureList(CFixtureList& _LatestScores, CDate& _Date)
{
	if (GetMatchDate().GetDate() == _Date.GetDate())
	{
		if (IsUserMatch() == false)
		{
			_LatestScores.Add(this);		// Add to list
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::AddToCurrentFixtureList
	Access:    	public
	Parameter: 	CFixtureList& _LatestScores
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::AddToCurrentFixtureList(CFixtureList& _LatestScores)
{
	if (GetMatchDate().GetDate() == WorldData().GetCurrentDate().GetDate())
	{
		_LatestScores.Add(this);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::PenaltyShootoutWinnerStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::PenaltyShootoutWinnerStr()
{
	return GetEventList()->PenaltyShootoutWinnerStr(GetClub(HOME).GetName(), GetClub(AWAY).GetName());
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetLosingClubID
	Access:    	public
	Parameter: 	void
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CFixture::GetLosingClubID()
{
	if (GetNumberGoals(HOME) > GetNumberGoals(AWAY))
	{
		return GetAwayClubID();
	}
	if (GetNumberGoals(AWAY) > GetNumberGoals(HOME))
	{
		return GetHomeClubID();
	}

	if (GetEventList()->WasPenaltyShootOut() == true)
	{
		return GetClubData(GetOpponent(GetEventList()->PenaltyShootoutWinner())).GetClubID();
	}
	return ClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetAwayClubID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CFixture::GetAwayClubID()
{
	return GetClubData(AWAY).GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetHomeClubID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CFixture::GetHomeClubID()
{
	return GetClubData(HOME).GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetWinningClubID
	Access:    	public
	Parameter: 	void
	Returns:   	const ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CFixture::GetWinningClubID()
{
	if (GetNumberGoals(HOME) > GetNumberGoals(AWAY))
	{
		return GetHomeClubID();
	}
	if (GetNumberGoals(AWAY) > GetNumberGoals(HOME))
	{
		return GetAwayClubID();
	}
	if (GetEventList()->WasPenaltyShootOut() == true)
	{
		return GetClubData(GetEventList()->PenaltyShootoutWinner()).GetClubID();
	}
	return ClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetSubsSelect
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetSubsSelect()
{
	return m_Data.m_SubsSelect;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetSubsUse
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetSubsUse()
{
	return m_Data.m_SubsUse;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchType
	Access:    	public const
	Parameter: 	void
	Returns:   	eMatchCompetitionType
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eMatchCompetitionType CFixture::GetMatchType() const
{
	return (eMatchCompetitionType)m_Data.m_MatchType;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::IsClubInvolved
	Access:    	public
	Parameter: 	const ushort _ClubID
	Returns:   	const bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::IsClubInvolved(const ClubID _ClubID)
{
	bool bRet = (m_ClubData[HOME].GetClubID() == _ClubID) || (m_ClubData[AWAY].GetClubID() == _ClubID);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetEventList
	Access:    	public
	Parameter: 	void
	BookPlayer
	Returns:   	CMatchEventList*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchEventList* CFixture::GetEventList()
{
	return &m_TheEvents;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetOffsides
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetOffsides(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetOffsides();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetShotsOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	sbyte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
sbyte CFixture::GetShotsOnTarget(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetShotsOnTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetShotsOffTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const sbyte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
sbyte CFixture::GetShotsOffTarget(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetShotsOffTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetCorners
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetCorners(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetCorners();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetFreeKicks
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetFreeKicks(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetFreeKicks();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetPercentageOfPlay
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CFixture::GetPercentageOfPlay(const eWhichTeam _Who)
{
	return GetClubData(_Who).GetPercentageOfPlay();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetMatchLength
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CFixture::GetMatchLength() const
{
	return m_Data.m_MatchLength;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetAttendance
	Access:    	public const
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CFixture::GetAttendance() const
{
	return m_Data.m_Attendance;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetTicketPrice
	Access:    	public const
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CFixture::GetTicketPrice() const
{
	return m_Data.m_TicketPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetExtraTimeFlag
	Access:    	public const
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::GetExtraTimeFlag() const
{
	return m_Data.m_ExtraTimePlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetShotsOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetShotsOnTarget(const eWhichTeam _Who, const byte _Num)
{
	EXT_ASSERT(_Num < 20);
	GetClubData(_Who).SetShotsOnTarget(_Num);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetShotsOffTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetShotsOffTarget(const eWhichTeam _Who, const byte _Num)
{
	EXT_ASSERT(_Num < 20);
	GetClubData(_Who).SetShotsOffTarget(_Num);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetCorners
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetCorners(const eWhichTeam _Who, const byte _Num)
{
	GetClubData(_Who).SetCorners(_Num);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetOffsides
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetOffsides(const eWhichTeam _Who, const byte _Num)
{
	GetClubData(_Who).SetOffsides(_Num);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetFreeKicks
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte _Num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetFreeKicks(const eWhichTeam _Who, const byte _Num)
{
	GetClubData(_Who).SetFreeKicks(_Num);
}


/*------------------------------------------------------------------------------
Method:   	CFixture::SetAwayGoalsToScore
Access:    	public
Parameter: 	const byte _Goals
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetAwayGoalsToScore(const byte _Goals)
{
	EXT_ASSERT(_Goals < 20);
	SetSubsUse(_Goals);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetAwayPercentageOfPlay
	Access:    	public
	Parameter: 	const byte _AwayPercentageOfPlay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetAwayPercentageOfPlay(const byte _AwayPercentageOfPlay)
{
	EXT_ASSERT(_AwayPercentageOfPlay < 90);
	m_ClubData[AWAY].SetPercentageOfPlay(_AwayPercentageOfPlay);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetHomeGoalsToScore
	Access:    	public
	Parameter: 	const byte _Goals
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetHomeGoalsToScore(const byte _Goals)
{
	EXT_ASSERT(_Goals < 20);
	SetSubsSelect(_Goals);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetHomePercentageOfPlay
	Access:    	public
	Parameter: 	const byte _HomePercentageOfPlay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetHomePercentageOfPlay(const byte _HomePercentageOfPlay)
{
	EXT_ASSERT(_HomePercentageOfPlay < 90);
	m_ClubData[HOME].SetPercentageOfPlay(_HomePercentageOfPlay);
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetMatchLength
	Access:    	public
	Parameter: 	const byte _Length
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetMatchLength(const byte _Length)
{
	m_Data.m_MatchLength = _Length;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetMatchType
	Access:    	public
	Parameter: 	const eMatchCompetitionType _MatchType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetMatchType(const eMatchCompetitionType _MatchType)
{
	m_Data.m_MatchType = (byte)_MatchType;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetSubsSelect
	Access:    	public
	Parameter: 	const byte _SubsSelect
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetSubsSelect(const byte _SubsSelect)
{
	m_Data.m_SubsSelect = _SubsSelect;
}


/*------------------------------------------------------------------------------
Method:   	CFixture::SetSubsUse
Access:    	public
Parameter: 	const byte _SubsUse
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetSubsUse(const byte _SubsUse)
{
	m_Data.m_SubsUse = _SubsUse;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetExtraTimeFlag
	Access:    	public
	Parameter: 	bool _Played
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetExtraTimeFlag(const bool _Played)
{
	m_Data.m_ExtraTimePlayed = _Played;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetAttendance
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetAttendance(const uint _Value)
{
	m_Data.m_Attendance = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetTicketPrice
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetTicketPrice(const uint _Value)
{
	m_Data.m_TicketPrice = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::SetMatchDate
	Access:    	public
	Parameter: 	CDate _MatchDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetMatchDate(CDate _MatchDate)
{
	m_Data.m_MatchDate = _MatchDate;
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::WinningClubName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::WinningClubName()
{
	return GetWinningClub()->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	m_Data.m_MatchDate.SetShiftedDate(_pDataFile->ReadBits(16));
	m_Data.m_TicketPrice = _pDataFile->ReadBits(TICKETPRICEBITS);
	m_Data.m_Attendance = _pDataFile->ReadBits(ATTENDANCEBITS);
	m_Data.m_SubsSelect = _pDataFile->ReadBits(SUBSSELECTBITS);
	m_Data.m_SubsUse = _pDataFile->ReadBits(SUBSUSEBITS);
	m_Data.m_MatchLength = _pDataFile->ReadBits(MATCHLENGTHBITS);
	m_Data.m_ExtraTimePlayed = _pDataFile->ReadBits(EXTRATIMEPLAYEDBITS);
	m_Data.m_MatchType = _pDataFile->ReadBits(MATCHTYPEBITS);
	m_ClubData[0].Read(_pDataFile);
	m_ClubData[1].Read(_pDataFile);

#ifdef STORE_MAN_OF_MATCH
	_pDataFile->ReadBits(m_ManOfMatch, 16);
#endif
#ifdef	ACTIVEREFEREES
	_pDataFile->ReadBits(m_RefereeNum, 16);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->WriteBits(m_Data.m_MatchDate.GetShiftedDate(), 16);
	_pDataFile->WriteBits(m_Data.m_TicketPrice, TICKETPRICEBITS);
	_pDataFile->WriteBits(m_Data.m_Attendance, ATTENDANCEBITS);
	_pDataFile->WriteBits(m_Data.m_SubsSelect, SUBSSELECTBITS);
	_pDataFile->WriteBits(m_Data.m_SubsUse, SUBSUSEBITS);
	_pDataFile->WriteBits(m_Data.m_MatchLength, MATCHLENGTHBITS);
	_pDataFile->WriteBits(m_Data.m_ExtraTimePlayed, EXTRATIMEPLAYEDBITS);
	_pDataFile->WriteBits(m_Data.m_MatchType, MATCHTYPEBITS);
	m_ClubData[0].Write(_pDataFile);
	m_ClubData[1].Write(_pDataFile);

#ifdef STORE_MAN_OF_MATCH
	_pDataFile->WriteBits(m_ManOfMatch, 16);
#endif
#ifdef	ACTIVEREFEREES
	_pDataFile->WriteBits(m_RefereeNum, 16);
#endif
}

void CFixture::WriteEvents1(CDataFile *_pDataFile)
{
	m_TheEvents.WriteTime(_pDataFile);
}

void CFixture::WriteEvents2(CDataFile *_pDataFile)
{
	m_TheEvents.WriteType(_pDataFile);
}

void CFixture::WriteEvents3(CDataFile *_pDataFile)
{
	m_TheEvents.WriteName(_pDataFile);
}

void CFixture::ReadEvents1(CDataFile *_pDataFile)
{
	m_TheEvents.ReadTime(_pDataFile);
}

void CFixture::ReadEvents2(CDataFile *_pDataFile)
{
	m_TheEvents.ReadType(_pDataFile);
}

void CFixture::ReadEvents3(CDataFile *_pDataFile)
{
	m_TheEvents.ReadName(_pDataFile);
}


#ifdef STOREMATCHINJURIES
/*------------------------------------------------------------------------------
	Method:   	CFixture::SetInjuries
	Access:    	public
	Parameter: 	const eWhichTeam x
	Parameter: 	const byte num
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::SetInjuries(const eWhichTeam x, const byte num)
{
	GetClubData(x).SetInjuries(num);
}
#endif


#ifdef	QUICKRESULTS
/*------------------------------------------------------------------------------
	Method:   	CFixture::QuickResultGenerator
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::QuickResultGenerator()
{
	EXT_ASSERT(false);
	m_ClubData[HOME].SetNumberGoals(max(0, RandomNumber.IntLessThan(8) - 2));
	m_ClubData[AWAY].SetNumberGoals(max(0, RandomNumber.IntLessThan(7) - 2));
}
#endif


#ifdef	ACTIVEREFEREES
/*------------------------------------------------------------------------------
	Method:   	CFixture::RefereeStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::RefereeStr()
{
	return GetReferee()->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::GetReferee
	Access:    	public
	Parameter: 	void
	Returns:   	CSoccerReferee*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerReferee* CFixture::GetReferee()
{
	EXT_ASSERT(m_RefereeNum != NOPLAYER);
	return WorldData().GetRefereeID(m_RefereeNum);
}
#endif


#ifdef STORE_MAN_OF_MATCH
/*------------------------------------------------------------------------------
	Method:   	CFixture::ManOfMatchStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixture::ManOfMatchStr()
{
	EXT_ASSERT(GetManOfMatch() != NOPLAYER);
	return WorldData().GetPlayer(GetManOfMatch())->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::DoSelectManOfMatch
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixture::DoSelectManOfMatch()
{
	eWhichTeam WhichTeam;
	GetMatchVars().SetHomeAwayClubs(*this);
	m_ManOfMatch = NOPLAYER;
	if (GetNumberGoals(HOME) == GetNumberGoals(AWAY))
	{
		// Player from team who 'stopped' most shots
		WhichTeam = HOME;
		if (GetClubData(HOME).GetShotsOnTarget() > GetClubData(AWAY).GetShotsOnTarget())
		{
			WhichTeam = AWAY;
		}
		if (RandomNumber.IntLessThan(100) < 40)
		{
			// Goalkeeper who kept out most shots on target
			m_ManOfMatch = GetMatchVars().GetClub(WhichTeam).GetFirstTeamSquad().GetSquad(0);
		}
		else
		{
			WhichTeam = HOME;
			if (GetClubData(HOME).GetShotsOnTarget() + GetClubData(HOME).GetShotsOffTarget() < GetClubData(AWAY).GetShotsOnTarget() + GetClubData(AWAY).GetShotsOffTarget())
			{
				WhichTeam = AWAY;
			}
			// Defender from team who conceded least shots
			m_ManOfMatch = DoDefenderManOfMatch(WhichTeam);
		}
	}
	else
	{
		WhichTeam = HOME;
		if (GetNumberGoals(HOME) < GetNumberGoals(AWAY))
		{
			WhichTeam = AWAY;
		}
		m_ManOfMatch = HatTrickPlayer(WhichTeam);
		if (GetManOfMatch() == NOPLAYER)
		{
			if (RandomNumber.IntLessThan(100) < 50)
			{
				m_ManOfMatch = DoMidfielderManOfMatch(WhichTeam);
			}
			else
			{
				m_ManOfMatch = DoAttackerManOfMatch(WhichTeam);
				// Attacker not found
				if (GetManOfMatch() == NOPLAYER)
				{
					m_ManOfMatch = DoMidfielderManOfMatch(WhichTeam);
				}
			}
		}
	}
	if (GetManOfMatch() == NOPLAYER)
	{
		m_ManOfMatch = GetClub(WhichTeam).GetFirstTeamSquad().GetSquad(RandomNumber.IntLessThan(PLAYERSINTEAM));
	}
#ifdef STOREMANOFMATCH
	WorldData().GetPlayer(GetManOfMatch())->AwardedManOfMatch();
#endif
}
#endif


#ifdef NDS
/*------------------------------------------------------------------------------
	Method:   	CFixture::ConvertMatchEventsToSummary
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Converts the list of match events into a summary that
				only stores the final score.
------------------------------------------------------------------------------*/
void CFixture::ConvertMatchEventsToSummary()
{
	m_TheEvents.ConvertToSummary();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::IsSummary
	Access:    	public
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::IsSummary() const
{
	return m_TheEvents.IsSummary();
}


/*------------------------------------------------------------------------------
	Method:   	CFixture::HasBeenSetupOrUsed
	Access:    	public
	Returns:   	bool - whether or not this fixture has ever been used or set up.
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CFixture::HasBeenSetupOrUsed() const
{
	return m_Data.m_MatchDate.GetDate() != CDate::MAX_DATE;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::~CFixtureList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList::~CFixtureList()
{
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::LatestScoreStr
	Access:    	public
	Parameter: 	const uint _Time
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CFixtureList::LatestScoreStr(const uint _Time)
{
	CString sRet;
	if (_Time > 0)
	{
		CFixture* pFixture;
		for (short LoopCount = GetUpperBound(); LoopCount >= 0; LoopCount--)
		{
            pFixture = GetAt(LoopCount);
			EXT_ASSERT(pFixture != NULL);
			sRet = pFixture->HasGoalBeenScored(_Time);
			if (sRet.IsEmpty() != TRUE)
			{
				break;
			}
		}
	}
	return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::Write
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Save all the fixtures to file
------------------------------------------------------------------------------*/
void CFixtureList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
    CFixture* pFixture;
	ushort nHowMany = GetSize();
	_pDataFile->EnterBitMode(true);

	EXT_ASSERT(nHowMany < 4096);
    _pDataFile->WriteBits(nHowMany, 12);

	for (int i = 0; i < GetSize(); i++)
	{
        pFixture = GetAt(i);
		if ((pFixture != null) && (pFixture->HasBeenSetupOrUsed() == true))
		{
			byte bFlag = 1;
			_pDataFile->WriteBits(bFlag, 1);
		}
		else
		{
			byte bFlag = 0;
			_pDataFile->WriteBits(bFlag, 1);
		}
	}

  _pDataFile->ExitBitMode();

	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        pFixture = GetAt(LoopCount);
		if ((pFixture != null) && (pFixture->HasBeenSetupOrUsed() == true))
		{
      _pDataFile->EnterBitMode(true);
			pFixture->Write(_pDataFile);
      _pDataFile->ExitBitMode();
		}
	}

	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        pFixture = GetAt(LoopCount);
		if ((pFixture != null) && (pFixture->HasBeenSetupOrUsed() == true))
		{
      _pDataFile->EnterBitMode(true);
			pFixture->WriteEvents1(_pDataFile);
      _pDataFile->ExitBitMode();
		}
	}
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        pFixture = GetAt(LoopCount);
		if ((pFixture != null) && (pFixture->HasBeenSetupOrUsed() == true))
		{
      _pDataFile->EnterBitMode(true);
			pFixture->WriteEvents2(_pDataFile);
      _pDataFile->ExitBitMode();
		}
	}
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        pFixture = GetAt(LoopCount);
		if ((pFixture != null) && (pFixture->HasBeenSetupOrUsed() == true))
		{
      _pDataFile->EnterBitMode(true);
			pFixture->WriteEvents3(_pDataFile);
      _pDataFile->ExitBitMode();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::Read
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Create new list from file
------------------------------------------------------------------------------*/
void CFixtureList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	_pDataFile->EnterBitMode(false);

    ushort nHowMany = _pDataFile->ReadBits(12);
	if (nHowMany > GetSize())
	{
		SetSize(nHowMany);
		for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
		{
            if (GetAt(LoopCount) == NULL)
			{
				SetAt(LoopCount, WorldData().FindFreeFixture());
			}
		}
	}
	if (nHowMany < GetSize())
	{
		for (ushort LoopCount = nHowMany; LoopCount < GetSize(); LoopCount++)
		{
            WorldData().GetFreeFixturesList().Add(GetAt(LoopCount));
		}
		SetSize(nHowMany);
		FreeExtra();
	}

	bool *bUsed = FD_ENGINE_NEW bool[nHowMany];
	for (int i = 0; i < nHowMany; i++)
	{
		bUsed[i] = (_pDataFile->ReadBits(1) == 1);
	}

  _pDataFile->ExitBitMode();

    for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
    _pDataFile->EnterBitMode(true);
		if (bUsed[LoopCount] == true)
			GetAt(LoopCount)->Read(_pDataFile);
    _pDataFile->ExitBitMode();
    }

    for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
    _pDataFile->EnterBitMode(true);
		if (bUsed[LoopCount] == true)
			GetAt(LoopCount)->ReadEvents1(_pDataFile);
    _pDataFile->ExitBitMode();
    }
    for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
    _pDataFile->EnterBitMode(true);
		if (bUsed[LoopCount] == true)
			GetAt(LoopCount)->ReadEvents2(_pDataFile);
    _pDataFile->ExitBitMode();
    }
    for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
    _pDataFile->EnterBitMode(true);
		if (bUsed[LoopCount] == true)
			GetAt(LoopCount)->ReadEvents3(_pDataFile);
    _pDataFile->ExitBitMode();
    }

	delete[] bUsed;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::FindID
	Access:    	public
	Parameter: 	CFixture* _pFixture
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixtureList::FindID(CFixture* _pFixture)
{
    EXT_ASSERT(_pFixture != null);
    ushort iRet = wNOTFOUND;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (GetAt(LoopCount) == _pFixture)
		{
            iRet = LoopCount;
            break;
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::FindID
	Access:    	public
	Parameter: 	const ushort a_HomeClubID
	Parameter: 	const ushort a_AwayClubID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CFixtureList::FindID(const ClubID _HomeClubID, const ClubID _AwayClubID)
{
    ushort iRet = wNOTFOUND;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (GetAt(LoopCount)->GetHomeClubID() == _HomeClubID && GetAt(LoopCount)->GetAwayClubID() == _AwayClubID)
		{
            iRet = LoopCount;
            break;
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::FindClubFixture
	Access:    	public
	Parameter: 	const uint _FixtureNumber
	Parameter: 	const ushort _ClubID
	Returns:   	CFixture*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CFixtureList::FindClubFixture(const uint _FixtureNumber, const ClubID _ClubID)
{
	EXT_ASSERT(GetSize() > 0);
    CFixture* pRet = null;
	int Count = 0;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (GetAt(LoopCount)->IsClubInvolved(_ClubID) == true)
		{
			if (Count == _FixtureNumber)
			{
                pRet = GetAt(LoopCount);
				break;
			}
			else
			{
				Count++;
			}
        }
    }
    return pRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::FindFixture
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	CFixture*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CFixtureList::FindFixture(const ClubID _HomeClubID, const ClubID _AwayClubID)
{
	EXT_ASSERT(GetSize() > 0);
    CFixture* pRet = NULL;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (GetAt(LoopCount)->GetHomeClubID() == _HomeClubID && GetAt(LoopCount)->GetAwayClubID() == _AwayClubID)
		{
            pRet = GetAt(LoopCount);
		}
    }
    EXT_ASSERT_POINTER(pRet, CFixture);
    return pRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::Create
	Access:    	public
	Parameter: 	const uint _Count
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::DoCreate(const uint _Count)
{
	SetSize(_Count);
    for (uint LoopCount = 0; LoopCount < _Count; LoopCount++)
	{
		CFixture *pNewFixture = FD_BIN_NEW CFixture();
        SetAt(LoopCount, pNewFixture);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::AddNew
	Access:    	public
	Parameter: 	CMatchInfo& Match
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::AddNew(CMatchInfo& _Match)
{
	Add(FD_BIN_NEW CFixture(_Match));
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::AddNew
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::AddNew()
{
	CFixture *pNewFixture = FD_BIN_NEW CFixture();
	Add(pNewFixture);
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::NewSeason
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::DoNewSeason()
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        GetAt(LoopCount)->DoResetVariables();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::operator[]
	Access:    	public
	Parameter: 	const ushort nIndex
	Returns:   	CFixture&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture& CFixtureList::operator[](const ushort _nIndex)
{
	return *GetAt(_nIndex);
}


#ifdef LATESTSCORES
/*------------------------------------------------------------------------------
	Method:   	CFixtureList::BuildCurrentNonUserFixtureList
	Access:    	public
	Parameter: 	CFixtureList& _LatestScores
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::BuildCurrentNonUserFixtureList(CFixtureList& _LatestScores)
{
	EXT_ASSERT(GetSize() > 0);
	CFixture* pFixture;
	for (int LoopCount = GetUpperBound(); LoopCount >= 0; LoopCount--)
	{
		pFixture = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pFixture, CFixture);
		pFixture->AddToCurrentNonUserFixtureList(_LatestScores, WorldData().GetCurrentDate());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureList::BuildCurrentFixtureList
	Access:    	public
	Parameter: 	CFixtureList& _LatestScores
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CFixtureList::BuildCurrentFixtureList(CFixtureList& _LatestScores)
{
	EXT_ASSERT(GetSize() > 0);
	CFixture* pFixture;
	for (int LoopCount = GetUpperBound(); LoopCount >= 0; LoopCount--)
	{
		pFixture = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pFixture, CFixture);
		pFixture->AddToCurrentFixtureList(_LatestScores);
	}
}
#endif
