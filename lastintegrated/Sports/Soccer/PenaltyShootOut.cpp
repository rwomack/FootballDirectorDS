
/*

	File:- PenaltyShootOut.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CPenaltyShootOut::DoCreateTakersList
	Access:    	public 
	Parameter: 	CClub & _Club
	Parameter: 	CNumericArray<byte> & _listTakers
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPenaltyShootOut::DoCreateTakersList(CClub& _Club, CNumericArray<byte>& _listTakers)
{
	byte nSquadPosition = 6;
	while(_listTakers.GetSize() < MAXPENALTYSHOOTOUTSHOTS)
	{
		if (_Club.GetPlayer(nSquadPosition).IsAvailableForMatch() == true && _Club.GetPlayer(nSquadPosition).GetSkills().GetPosition() != GOALKEEPER)
		{
			_listTakers.Add(nSquadPosition);
		}
		nSquadPosition++;
		if (nSquadPosition > PLAYERSINTEAM)
		{
			nSquadPosition = 2;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPenaltyShootOut::GetPlayerEffectiveSkill
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Parameter: 	int _PlayerSquadNum
	Parameter: 	ePlayerWhichSkill _WhichSkill
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPenaltyShootOut::GetPlayerEffectiveSkill(eWhichTeam _Who, int _PlayerSquadNum, ePlayerWhichSkill _WhichSkill)
{
	return WorldData().GetMatchHandler().GetMatchVars().GetClub(_Who).GetPlayer(_PlayerSquadNum).DoCalculateEffectiveSkill(_WhichSkill);
}


/*------------------------------------------------------------------------------
	Method:   	CPenaltyShootOut::DoCalculateShot
	Access:    	public 
	Parameter: 	int _GoalKeeperSkill
	Parameter: 	int _ShotSkill
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPenaltyShootOut::DoCalculateShot(int _GoalKeeperSkill, int _ShotSkill)
{
	// Does the shot succeed?
	int ShotDiff = RandomNumber.IntLessThan(_ShotSkill + _GoalKeeperSkill + SHOTADVANTAGE);
	int Scored = 1;
	if (ShotDiff <= _GoalKeeperSkill && RandomNumber.IntLessThan(10) < 9)
	{
		Scored = 0;
	}
	return Scored;
}


/*------------------------------------------------------------------------------
	Method:   	CPenaltyShootOut::DoShootout
	Access:    	public 
	Parameter: 	CMatchPlay & _MatchPlay
	Returns:   	void
	Qualifier: 	
	Purpose:	If skipping to end of match then process the penalty shootout
------------------------------------------------------------------------------*/
void CPenaltyShootOut::DoShootout(CMatchPlay& _MatchPlay)
{
	int AwayGoalKeeperEffectiveSkill = GetPlayerEffectiveSkill(AWAY, 0, SKILL_HANDLING);
	int HomeGoalKeeperEffectiveSkill = GetPlayerEffectiveSkill(HOME, 0, SKILL_HANDLING);
	eMatchEvent HomePenaltyResultType[] = 
	{
		HOMEPENALTYSHOOTOUTGOAL, HOMEPENALTYSHOOTOUTMISS, HOMEPENALTYSHOOTOUTSAVED
	};
	eMatchEvent AwayPenaltyResultType[] = 
	{
		AWAYPENALTYSHOOTOUTGOAL, AWAYPENALTYSHOOTOUTMISS, AWAYPENALTYSHOOTOUTSAVED
	};

	// Select the players to take the penalties
	m_PenaltyShootoutAwaySquadNumbers.RemoveAll();
	m_PenaltyShootoutHomeSquadNumbers.RemoveAll();
	DoCreateTakersList(_MatchPlay.GetFixture().GetClub(HOME), m_PenaltyShootoutHomeSquadNumbers);
	DoCreateTakersList(_MatchPlay.GetFixture().GetClub(AWAY), m_PenaltyShootoutAwaySquadNumbers);

	// Ensure not tied after {MAXPENALTYSHOOTOUTSHOTS} penalties each
	do
	{
		m_PenaltyShootoutHomeResult.RemoveAll();
		m_PenaltyShootoutAwayResult.RemoveAll();

		for (int LoopCount = 0; LoopCount < MAXPENALTYSHOOTOUTSHOTS; LoopCount++)
		{
			m_PenaltyShootoutHomeResult.Add(DoCalculateShot(AwayGoalKeeperEffectiveSkill, GetPlayerEffectiveSkill(HOME, m_PenaltyShootoutHomeSquadNumbers.GetAt(LoopCount), SKILL_SHOOTING)));
			m_PenaltyShootoutAwayResult.Add(DoCalculateShot(HomeGoalKeeperEffectiveSkill, GetPlayerEffectiveSkill(AWAY, m_PenaltyShootoutAwaySquadNumbers.GetAt(LoopCount), SKILL_SHOOTING)));
		}
	}
	while(m_PenaltyShootoutHomeResult.NumberZero() == m_PenaltyShootoutAwayResult.NumberZero());

	int HomeScored = 0;
	int AwayScored = 0;
	int HomeMaxScore = MAXPENALTYSHOOTOUTSHOTS;
	int AwayMaxScore = MAXPENALTYSHOOTOUTSHOTS;
	for (int LoopCount = 0; LoopCount < MAXPENALTYSHOOTOUTSHOTS; LoopCount++)
	{
		HomeScored += (m_PenaltyShootoutHomeResult[LoopCount] == 0);
// 		g_DebugStr.Format(_T("Home Penalty %d - %d"), LoopCount + 1, HomePenaltyResultType[m_PenaltyShootoutHomeResult[LoopCount]]);
// 		g_DebugStr.OutputDebug();
		_MatchPlay.GetFixture().DoAddMatchEvent(255, HomePenaltyResultType[m_PenaltyShootoutHomeResult[LoopCount]], 
			static_cast<CPersonName&>(*WorldData().GetPlayer(_MatchPlay.GetFixture().GetClub(HOME).GetFirstTeamSquad().GetSquad(m_PenaltyShootoutHomeSquadNumbers[LoopCount]))));

		if (HomeScored > AwayScored + AwayMaxScore)
		{
			break;
		}
		HomeMaxScore--;

		AwayScored += (m_PenaltyShootoutAwayResult[LoopCount] == 0);
		AwayMaxScore--;

// 		g_DebugStr.Format(_T("Away Penalty %d - %d"), LoopCount + 1, AwayPenaltyResultType[m_PenaltyShootoutAwayResult[LoopCount]]);
// 		g_DebugStr.OutputDebug();
		_MatchPlay.GetFixture().DoAddMatchEvent(255, AwayPenaltyResultType[m_PenaltyShootoutAwayResult[LoopCount]],
			static_cast<CPersonName&>(*WorldData().GetPlayer(_MatchPlay.GetFixture().GetClub(AWAY).GetFirstTeamSquad().GetSquad(m_PenaltyShootoutAwaySquadNumbers[LoopCount]))));
		if (AwayScored > HomeScored + HomeMaxScore)
		{
			break;
		}
	}
}


// 0-goal / 1-saved / 2-miss
//---------------------------------------------------------------------------------------
void CPenaltyShootOut::AwayPenaltyCommentary(CMatchPlay& _MatchPlay, const ushort _Index, CClub& _Club)
{
	EXT_ASSERT(_Index < m_PenaltyShootoutAwayResult.GetSize());
	DoPenaltyCommentary(_MatchPlay, m_PenaltyShootoutAwayResult[_Index],
						WorldData().GetPlayer(_Club.GetFirstTeamSquad().GetSquad(m_PenaltyShootoutAwaySquadNumbers[_Index]))->GetName(),
						_Club.GetName());
}


// 0-goal / 1-saved / 2-miss
//---------------------------------------------------------------------------------------
void CPenaltyShootOut::DoPenaltyCommentary(CMatchPlay& _MatchPlay, const byte _ResultType, CString& _PlayerName, CString& _ClubName)
{
	switch(_ResultType)
	{
		case 0:
			// Scored
			_MatchPlay.GetMatchActionInfo().DoScoredPenaltyGoal(_PlayerName, _ClubName);
			break;
		case 1:
			// Saved
			_MatchPlay.GetMatchActionInfo().DoPenaltySaved(_PlayerName, _ClubName);
			break;
		case 2:
			// Missed
			_MatchPlay.GetMatchActionInfo().DoPenaltyMissed(_PlayerName, _ClubName);
		default :
			EXT_ASSERT(false);
	}
}


// 0-goal / 1-saved / 2-miss
//---------------------------------------------------------------------------------------
void CPenaltyShootOut::HomePenaltyCommentary(CMatchPlay& _MatchPlay, const ushort _Index, CClub& _Club)
{
	EXT_ASSERT(_Index < m_PenaltyShootoutHomeResult.GetSize());
	DoPenaltyCommentary(_MatchPlay, m_PenaltyShootoutHomeResult[_Index], 
		WorldData().GetPlayer(_Club.GetFirstTeamSquad().GetSquad(m_PenaltyShootoutHomeSquadNumbers[_Index]))->GetName(), 
		_Club.GetName());
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
