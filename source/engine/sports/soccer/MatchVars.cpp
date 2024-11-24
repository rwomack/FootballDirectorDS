
 /*

        Class:- CMatchVars


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


	// Chance of goal attempt in each 15 min time sector
	const int MATCHTIMESCTORSIZE = 15;
	const int GOALCHANCESECTOR1 = 110;
	const int GOALCHANCESECTOR2 = 119;
	const int GOALCHANCESECTOR3 = 162;
	const int GOALCHANCESECTOR4 = 147;
	const int GOALCHANCESECTOR5 = 139;
	const int GOALCHANCESECTOR6 = 229;
	const int TOTALGOALCHANCE = GOALCHANCESECTOR1 + GOALCHANCESECTOR2 + GOALCHANCESECTOR3 +
					GOALCHANCESECTOR4 + GOALCHANCESECTOR5 + GOALCHANCESECTOR6;

	// If in extra time
	const int GOALCHANCESECTOR7 = 139;
	const int GOALCHANCESECTOR8 = 229;
	const int TOTALEXTRATIMEGOALCHANCE = GOALCHANCESECTOR7 + GOALCHANCESECTOR8;


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::CMatchVars
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchVars::CMatchVars()
{
	m_nGoalChanceTime.Add(GOALCHANCESECTOR1);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR2);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR3);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR4);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR5);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR6);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR7);
	m_nGoalChanceTime.Add(GOALCHANCESECTOR8);

	///m_TheEvents.SetSize(125);
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoInitialiseClubs
	Access:    	public 
	Parameter: 	const byte _SubsSelect
	Parameter: 	const eMatchCompetitionType _MatchType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoInitialiseClubs(const byte _SubsSelect, const eMatchCompetitionType _MatchType)
{
	m_MatchVars[HOME].DoInitialise(_SubsSelect, GetClub(AWAY), _MatchType);
    m_MatchVars[AWAY].DoInitialise(_SubsSelect, GetClub(HOME), _MatchType);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetClub
	Access:    	public 
	Parameter: 	eWhichTeam whichteam
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchVars::GetClub(const eWhichTeam _Who)
{
	return m_MatchVars[_Who].GetClub();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetCup
	Access:    	public 
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CMatchVars::GetCup()
{
	EXT_ASSERT_POINTER(m_Cup, CCup);
	return m_Cup;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetTeamAggression
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVars::GetTeamAggression(eWhichTeam _Who)				
{ 
	return m_MatchVars[_Who].GetTeamTactic().GetAggression(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetStyleOfPlay
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVars::GetStyleOfPlay(eWhichTeam _Who)					
{ 
	return m_MatchVars[_Who].GetTeamTactic().GetStyleOfPlay(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetCounterAttack
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchVars::GetCounterAttack(eWhichTeam _Who)				
{ 
	return m_MatchVars[_Who].GetTeamTactic().GetCounterAttack(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetOffsideTrap
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchVars::GetOffsideTrap(eWhichTeam _Who)
{ 
	return m_MatchVars[_Who].GetTeamTactic().GetOffsideTrap(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetTeamMorale
	Access:    	public 
	Parameter: 	eWhichTeam Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVars::GetTeamMorale(eWhichTeam Who)					
{ 
	return m_MatchVars[Who].GetTeamMorale(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetNumberPlayersToSendOff
	Access:    	public 
	Parameter: 	eWhichTeam Who
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CMatchVars::GetNumberPlayersToSendOff(eWhichTeam Who)		
{ 
	return m_MatchVars[Who].GetNumberPlayersToSendOff(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetNumberPlayersToBook
	Access:    	public 
	Parameter: 	eWhichTeam Who
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CMatchVars::GetNumberPlayersToBook(eWhichTeam Who)			
{ 
	return m_MatchVars[Who].GetNumberPlayersToBook(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetTemperament
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVars::GetTemperament(eWhichTeam _Who)                  
{ 
	return m_MatchVars[_Who].GetTemperament(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetCompetitionInfo
	Access:    	public 
	Parameter: 	CDivision* _pDivision
	Parameter: 	CCupRound* _pCupRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetCompetitionInfo(CDivision* _pDivision, CCupRound* _pCupRound, CCup* _Cup /*= NULL*/)
{
    m_pCurrentDivision = _pDivision;     // Should point to the division being used
	m_pCurrentCupRound = _pCupRound;     // Should point to the Cup round being used
	m_Cup = _Cup;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetMatchTimeSectorSize
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchVars::GetMatchTimeSectorSize()
{
	return MATCHTIMESCTORSIZE;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::IsCupMatch
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchVars::IsCupMatch() const
{
	if (m_pCurrentCupRound != NULL)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::IsLeagueMatch
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchVars::IsLeagueMatch() const
{
	if (m_pCurrentDivision != NULL)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::pCupRound
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CMatchVars::GetCupRound()
{
	EXT_ASSERT_POINTER(m_pCurrentCupRound, CCupRound);
	return m_pCurrentCupRound;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetLastFoulerName
	Access:    	public const 
	Parameter: 	void
	Returns:   	CPersonName*
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CPersonName* CMatchVars::GetLastFoulerName() const					
{ 
	EXT_ASSERT_POINTER(m_pLastFoulerName, CPersonName);
	return m_pLastFoulerName; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetLastVictimName
	Access:    	public const 
	Parameter: 	void
	Returns:   	CPersonName*
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CPersonName* CMatchVars::GetLastVictimName() const					
{ 
	EXT_ASSERT_POINTER(m_pLastVictimName, CPersonName);
	return m_pLastVictimName;	
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetLastScorer
	Access:    	public 
	Parameter: 	void
	Returns:   	CPersonName*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPersonName* CMatchVars::GetLastScorer() 							
{ 
	EXT_ASSERT_POINTER(m_pLastScorer, CPersonName);
	return m_pLastScorer; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetLastAssistScorerName
	Access:    	public 
	Parameter: 	void
	Returns:   	CPersonName*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPersonName* CMatchVars::GetLastAssistScorerName()					
{ 
	EXT_ASSERT_POINTER(m_pLastAssistScorerName, CPersonName);
	return m_pLastAssistScorerName; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetDivision
	Access:    	public 
	Parameter: 	void
	Returns:   	CDivision*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision* CMatchVars::GetDivision()
{ 
	EXT_ASSERT_POINTER(m_pCurrentDivision, CDivision);
	return m_pCurrentDivision; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoAdjustDisciplinaryEventsForExtraTime
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoAdjustDisciplinaryEventsForExtraTime()
{
    m_MatchVars[HOME].DoAdjustDisciplinaryEventsForExtraTime();
    m_MatchVars[AWAY].DoAdjustDisciplinaryEventsForExtraTime();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetMatchVarsTeamData
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Returns:   	CMatchVarsData&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchVarsData& CMatchVars::GetMatchVarsTeamData(eWhichTeam _Who)
{ 
	return m_MatchVars[_Who]; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetHomeAwayClubs
	Access:    	public 
	Parameter: 	CFixture& aFixture
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetHomeAwayClubs(CFixture& _Fixture)
{
	m_MatchVars[HOME].SetClub(_Fixture.GetHomeClubID());
	m_MatchVars[AWAY].SetClub(_Fixture.GetAwayClubID());
	m_MatchVars[HOME].SetDayType(GetClub(HOME).GetPopularity().RandomMatchDayType());
	m_MatchVars[AWAY].SetDayType(GetClub(AWAY).GetPopularity().RandomMatchDayType());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::AssistedGoal
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _AssisterPlayerID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::AssistedGoal(const eWhichTeam _Who, const ushort _AssisterPlayerID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	m_MatchVars[_Who].DoIncrementAssists(_AssisterPlayerID, _ShotSaveSkillDifference, _TheTime);
    m_pLastAssistScorerName = WorldData().GetPlayer(_AssisterPlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::AssistedShot
	Access:    	public 
	Parameter: 	const eWhichTeam who
	Parameter: 	const ushort Assister
	Parameter: 	const sbyte ShotSaveSkillDifference
	Parameter: 	const byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::AssistedShot(const eWhichTeam _Who, const ushort _AssistPlayerID, const sbyte _ShotSaveSkillDifference, const byte _Time)
{
	m_MatchVars[_Who].DoAssistedShot(_AssistPlayerID, _ShotSaveSkillDifference, _Time);
    m_pLastAssistScorerName = WorldData().GetPlayer(_AssistPlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::ShotOnTarget
	Access:    	public 
	Parameter: 	eWhichTeam who
	Parameter: 	int x
	Parameter: 	const byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:	Called by view match not result only
------------------------------------------------------------------------------*/
void CMatchVars::DoShotOnTarget(eWhichTeam _Who, ushort _PlayerID, byte _Time)
{
	sbyte ShotSaveSkillDifference = 255 - RandomNumber.IntLessThan(60);	// Negative shot rating
#ifdef PLAYER_MATCH_STATS
	m_MatchVars[_Who].DoIncrementShotsOnTarget(_PlayerID, ShotSaveSkillDifference, _Time);
#endif
	if (_Who == HOME)
	{
		m_MatchVars[AWAY].DoAttemptSave(0, abs(ShotSaveSkillDifference), _Time);
	}
	else
	{
		m_MatchVars[HOME].DoAttemptSave(0, abs(ShotSaveSkillDifference), _Time);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoShotOffTarget
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Parameter: 	ushort PlayerID
	Parameter: 	byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoShotOffTarget(eWhichTeam _Who, ushort PlayerID, byte _Time)
{
	sbyte ShotSaveSkillDifference = 240 - RandomNumber.IntLessThan(60);	// Negative shot rating
#ifdef PLAYER_MATCH_STATS
	m_MatchVars[_Who].DoIncrementShotsOffTarget(PlayerID, ShotSaveSkillDifference, _Time);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetLastFoulerName
	Access:    	public 
	Parameter: 	CPersonName* _pName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetLastFoulerName(CPersonName* _Name)
{ 
	EXT_ASSERT(_Name != NULL); 
	m_pLastFoulerName = _Name; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetLastVictimName
	Access:    	public 
	Parameter: 	CPersonName* _pName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetLastVictimName(CPersonName* _Name)
{ 
	EXT_ASSERT(_Name != NULL); 
	m_pLastVictimName = _Name;	
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetLastScorer
	Access:    	public 
	Parameter: 	CPersonName* _pName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetLastScorer(CPersonName* _Name)
{ 
	EXT_ASSERT(_Name != NULL); 
	m_pLastScorer = _Name; 
	SetLastAssistScorerName(NULL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetLastAssistScorerName
	Access:    	public 
	Parameter: 	CPersonName* _pName - Can be NULL when it is a penalty
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetLastAssistScorerName(CPersonName* _Name)
{ 
	m_pLastAssistScorerName = _Name; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoResetVariables()
{	
    m_GoalTimesList.RemoveAll();
	m_pLastScorer = NULL;
	m_pLastAssistScorerName = NULL;
	m_pLastFoulerName = NULL;
	m_pLastVictimName = NULL;
	m_LastInjuredSquadNumber = NOPLAYER;
	m_MatchVars[HOME].DoResetVariables();
	m_MatchVars[AWAY].DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetPlayerSkill
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _SquadNumber
	Parameter: 	const byte _Time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchVars::GetPlayerSkill(const eWhichTeam _Who, const ushort _SquadNumber, byte _TheTime)
{
	return GetSquadPerformance(_Who).GetPlayerSkill(_SquadNumber, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SwapPlayersOnPitch
	Access:    	public 
	Parameter: 	eWhichTeam _Who
	Parameter: 	const ushort PlayerOffSquadID
	Parameter: 	const ushort PlayerOnSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SwapPlayersOnPitch(eWhichTeam _Who, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID)
{
#ifdef PLAYER_MATCH_STATS
	m_MatchVars[_Who].SwapPlayers(_PlayerOffSquadID, _PlayerOnSquadID);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::MakeSubstitution
	Access:    	public 
	Parameter: 	const byte TheTime
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort PlayerOffSquadID
	Parameter: 	const ushort PlayerOnSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::MakeSubstitution(byte _TheTime, const eWhichTeam _Who, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID)
{
	m_MatchVars[_Who].DoMakeSubstitution(_TheTime, _PlayerOffSquadID, _PlayerOnSquadID);
	SwapPlayersOnPitch(_Who, _PlayerOffSquadID, _PlayerOnSquadID);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::TakePlayerOff
	Access:    	public 
	Parameter: 	const byte TheTime
	Parameter: 	eWhichTeam _Who
	Parameter: 	int PlayerOff
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoTakePlayerOff(byte _TheTime, eWhichTeam _Who, const ushort _SquadNumber)
{
	m_MatchVars[_Who].DoTakePlayerOff(_TheTime, _SquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetTemperament
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetTemperament(const eWhichTeam _Who, const byte _Value)
{ 
	m_MatchVars[_Who].SetTemperament(_Value); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetSquadPerformance
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	CSoccerPlayerPerformanceList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformanceList& CMatchVars::GetSquadPerformance(const eWhichTeam _Who)
{
	return m_MatchVars[_Who].GetPlayerPerformanceList();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::PlayerScoredGoal
	Access:    	public 
	Parameter: 	eWhichTeam whichteam
	Parameter: 	int who
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	const byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:	Called by view match and result only
------------------------------------------------------------------------------*/
void CMatchVars::PlayerScoredGoal(eWhichTeam _WhichTeam, int _who, sbyte _ShotSaveSkillDifference, byte _TheTime)
{
#ifdef PLAYER_MATCH_STATS
	m_MatchVars[_WhichTeam].DoAttemptGoal(_who, _ShotSaveSkillDifference, _TheTime);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::PlayerConcededShot
	Access:    	public 
	Parameter: 	eWhichTeam _WhichTeam
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::PlayerConcededShot(eWhichTeam _WhichTeam, const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	m_MatchVars[_WhichTeam].DoConcededShot(_PlayerSquadID, _ShotSaveSkillDifference, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::PlayerDefendAttempt
	Access:    	public 
	Parameter: 	eWhichTeam whichteam
	Parameter: 	int who
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	const byte TheTime
	Parameter: 	bool WasMidfielder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoPlayerDefendAttempt(eWhichTeam _Who, int _PlayerNum, sbyte _ShotSaveSkillDifference, byte _TheTime, bool _WasMidfielder)
{
	m_MatchVars[_Who].DoPlayerDefendAttempt(_PlayerNum, _ShotSaveSkillDifference, _TheTime, _WasMidfielder);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GoalkeeperAttemptSave
	Access:    	public 
	Parameter: 	const eWhichTeam _WhichTeam
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::GoalkeeperAttemptSave(const eWhichTeam _WhichTeam, const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	m_MatchVars[_WhichTeam].DoAttemptSave(_PlayerSquadID, _ShotSaveSkillDifference, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::SetLastInjuredPlayer
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::SetLastInjuredPlayerSquadNumber(const ushort _SquadNumber)
{
    m_LastInjuredSquadNumber = _SquadNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::LastInjuredPlayerID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchVars::GetLastInjuredPlayerSquadNumber()
{
	return m_LastInjuredSquadNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoInitialiseFromTactics
	Access:    	public 
	Parameter: 	CTactic * _Tactics
	Parameter: 	const eWhichTeam _Which
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoInitialiseFromTactics(CTactic* _Tactics, const eWhichTeam _Which)
{
    m_MatchVars[_Which].GetTeamTactic().Copy(_Tactics->GetTeamTactic());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::InitExtraTime
	Access:    	public 
	Parameter: 	const byte ExtraTimeLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DoInitialiseExtraTime(const byte _ExtraTimeLength)
{
	m_MatchVars[HOME].DoInitialiseExtraTime(_ExtraTimeLength);
	m_MatchVars[AWAY].DoInitialiseExtraTime(_ExtraTimeLength);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DoCalculateShotTime
	Access:    	public 
	Parameter: 	ushort nEndTime
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchVars::DoCalculateShotTime(ushort _EndTime)
{
	ushort nMatchLength = TOTALGOALCHANCE;
	if (_EndTime < 45)
	{
		nMatchLength = TOTALEXTRATIMEGOALCHANCE;
	}
	return m_nGoalChanceTime.PositionLessThanValue(RandomNumber.IntLessThan(nMatchLength)) * MATCHTIMESCTORSIZE;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::GetShootingSkills
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchVars::GetShootingSkills(const eWhichTeam _Who)
{ 
	return m_MatchVars[_Who].GetShootingSkills().TotalSum(); 
}


#ifdef MATCH_HEADING_SKILLS
/*------------------------------------------------------------------------------
	Method:   	CMatchVars::TotalHomeHeadingSkills
	Access:    	public 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const uint CMatchVars::TotalHomeHeadingSkills()					
{ 
	return m_MatchVars[HOME].getHeadingSkills().TotalSum(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::TotalAwayHeadingSkills
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchVars::TotalAwayHeadingSkills()					
{ 
	return m_MatchVars[AWAY].getHeadingSkills().TotalSum(); 
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CMatchVars::NearestAboveShootingSkills
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const uint _ShotTaker
	Returns:   	const int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchVars::NearestAboveShootingSkills(const eWhichTeam _Who, const uint _ShotTaker)	
{ 
	return m_MatchVars[_Who].GetShootingSkillsWhere().NearestAbove(_ShotTaker); 
}


#ifdef TESTSTATS
/*------------------------------------------------------------------------------
	Method:   	CMatchVars::DumpTestStats
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVars::DumpTestStats( FILE* _pFile )
{
    _pDataFile->Write(&m_HomeDayType, sizeof(m_HomeDayType));
    _pDataFile->Write(&m_AwayDayType, sizeof(m_AwayDayType));
	m_PlayerPerformance[HOME].Write(_pDataFile);
	m_PlayerPerformance[AWAY].Write(_pDataFile);
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////
