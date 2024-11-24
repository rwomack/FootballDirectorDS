
 /*

    Class:- CMatchVars


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::CMatchVarsData
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchVarsData::CMatchVarsData()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoInitialise
	Access:    	public 
	Parameter: 	const byte _SubsSelect
	Parameter: 	CClub & _OpponentClub
	Parameter: 	const eMatchCompetitionType _MatchType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoInitialise(const byte _SubsSelect, CClub& _OpponentClub, const eMatchCompetitionType _MatchType)
{
	GetClub().DoSelectTeam(_SubsSelect);
	if (GetClub().IsUserControlled() == true)
	{
		GetClub().GetFirstTeamSquad().DoSelectSpecialPlayers();
	}
	else
	{
		GetClub().DoSelectTactics(&_OpponentClub, _MatchType);
	}
    SetTeamMorale(GetClub().GetMorale());
    m_TeamTactic.Copy(GetClub().GetTeamTactic());
	m_PlayerPerformance.DoInitialise(&GetClub());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoResetVariables()
{
	m_pClub = NULL;
	m_NumberPlayersToBook = 0;
	m_NumberPlayersToSendOff = 0;
	m_GoalAttempts = 0;
	m_TeamTactic.SetDefaults();
#ifdef PLAYER_MATCH_STATS
	for (int LoopCount = 0; LoopCount < PLAYERSINTEAM + MAXSUBS; LoopCount++)
	{
		m_PlayerMatchStats[LoopCount].DoResetVariables();
	}
#endif
	m_DefenceZonalStats.DoInitialise();
	m_MidfieldZonalStats.DoInitialise();
	m_AttackZonalStats.DoInitialise();
	m_DayType = NODAYTYPE;
	m_TeamMorale = 0;
	m_Temperament = 0;
	m_PopularityID = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetClub
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetClub(const ClubID _ClubID)
{
	m_pClub = &WorldData().GetClubByID(_ClubID); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CMatchVarsData::GetClub()
{ 
//	EXT_ASSERT_POINTER(m_pClub, CClub);
	return *m_pClub; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetDayType
	Access:    	public 
	Returns:   	eMatchDayEffect
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eMatchDayEffect CMatchVarsData::GetDayType()								
{ 
	return static_cast<eMatchDayEffect>(m_DayType); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetDayType
	Access:    	public 
	Parameter: 	const eMatchDayEffect _eEffect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetDayType(const eMatchDayEffect _eEffect)			
{ 
	m_DayType = static_cast<byte>(_eEffect); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetTeamTactic
	Access:    	public 
	Returns:   	CTeamTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CMatchVarsData::GetTeamTactic()								
{ 
	return m_TeamTactic; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetTeamMorale
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVarsData::GetTeamMorale() const						
{ 
	return m_TeamMorale; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetNumberPlayersToSendOff
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVarsData::GetNumberPlayersToSendOff() const			
{ 
	return m_NumberPlayersToSendOff; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetNumberPlayersToBook
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVarsData::GetNumberPlayersToBook() const				
{ 
	return m_NumberPlayersToBook; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetTemperament
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVarsData::GetTemperament() const						
{ 
	return m_Temperament; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetTeamMorale
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetTeamMorale(const byte _Value)
{ 
	EXT_ASSERT(_Value >= MINIMUMMORALE && _Value <= MAXIMUMMORALE);
	m_TeamMorale = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetNumberPlayersToSendOff
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetNumberPlayersToSendOff(const byte _Value)		
{ 
	EXT_ASSERT(_Value <= PLAYERSINTEAM);
	m_NumberPlayersToSendOff = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetNumberPlayersToBook
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetNumberPlayersToBook(const byte _Value)			
{ 
	EXT_ASSERT(_Value <= PLAYERSINTEAM);
	m_NumberPlayersToBook = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetTemperament
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetTemperament(const byte _Value)					
{ 
	EXT_ASSERT(_Value >= MINTEMPER && _Value <= MAXTEMPER);
	m_Temperament = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetShootingSkills
	Access:    	public 
	Returns:   	CNumericArray<byte>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<byte>& CMatchVarsData::GetShootingSkills()							
{ 
	return m_ShootingSkills; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetShootingSkillsWhere
	Access:    	public 
	Returns:   	CNumericArray<uint>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<uint>&  CMatchVarsData::GetShootingSkillsWhere()						
{ 
	return m_ShootingSkillsWhere; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetGoalAttempts
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchVarsData::GetGoalAttempts()							
{ 
	return m_GoalAttempts; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetGoalAttempts
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetGoalAttempts(const sbyte _Value)
{ 
	EXT_ASSERT(_Value <= 20);
	m_GoalAttempts = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetDefenceZonalStats
	Access:    	public 
	Returns:   	CMatchZonalStats&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchZonalStats& CMatchVarsData::GetDefenceZonalStats()
{
	return m_DefenceZonalStats;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetMidfieldZonalStats
	Access:    	public 
	Returns:   	CMatchZonalStats&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchZonalStats& CMatchVarsData::GetMidfieldZonalStats()
{
	return m_MidfieldZonalStats;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetAttackZonalStats
	Access:    	public 
	Returns:   	CMatchZonalStats&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchZonalStats& CMatchVarsData::GetAttackZonalStats()
{
	return m_AttackZonalStats;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::ClearShootingSkillLists
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::ClearShootingSkillLists()
{
	m_ShootingSkills.RemoveAll();
	m_ShootingSkillsWhere.RemoveAll();
#ifdef MATCH_HEADING_SKILLS
	m_HeadingSkills.RemoveAll();
	m_HeadingSkillsWhere.RemoveAll();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::AddToShootingAndHeadingSkillList
	Access:    	public 
	Parameter: 	const byte _ShotSkill
	Parameter: 	const byte _HeadSkill
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::AddToShootingAndHeadingSkillList(const byte _ShotSkill, const byte _HeadSkill)
{
	m_ShootingSkills.Add(_ShotSkill);
	m_ShootingSkillsWhere.Add(m_ShootingSkills.TotalSum());
#ifdef MATCH_HEADING_SKILLS
	m_HeadingSkills.Add(_HeadSkill);
	m_HeadingSkillsWhere.Add(m_ShootingSkills.TotalSum());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::AddDefenceVars
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::AddDefenceVars(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	GetDefenceZonalStats().AddVars(_Player);
	AddToShootingAndHeadingSkillList(_Player->DoCalculateEffectiveSkill(SKILL_SHOOTING), _Player->DoCalculateEffectiveSkill(SKILL_HEADING));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::AddMidfieldVars
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::AddMidfieldVars(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	GetMidfieldZonalStats().AddVars(_Player);
	AddToShootingAndHeadingSkillList(_Player->DoCalculateEffectiveSkill(SKILL_SHOOTING) * 2, _Player->DoCalculateEffectiveSkill(SKILL_HEADING) * 2);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::AddAttackVars
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::AddAttackVars(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	GetAttackZonalStats().AddVars(_Player);
	AddToShootingAndHeadingSkillList(_Player->DoCalculateEffectiveSkill(SKILL_SHOOTING) * 8, _Player->DoCalculateEffectiveSkill(SKILL_HEADING) * 8);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoAdjustDisciplinaryEventsForExtraTime
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoAdjustDisciplinaryEventsForExtraTime()
{
	SetNumberPlayersToBook(GetNumberPlayersToBook() / 3);
    SetNumberPlayersToSendOff(GetNumberPlayersToSendOff () / 3);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoAssistedShot
	Access:    	public 
	Parameter: 	const ushort _AssistPlayerID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoAssistedShot(const ushort _AssistPlayerID, const sbyte _ShotSaveSkillDifference, const byte _Time)
{
	m_PlayerPerformance.AssistedShot(_AssistPlayerID, _ShotSaveSkillDifference, _Time);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoAttemptGoal
	Access:    	public 
	Parameter: 	const ushort _who
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoAttemptGoal(const ushort _who, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	DoIncrementGoals(_who);
	DoIncrementShotsOnTarget(_who, _ShotSaveSkillDifference, _TheTime);
	m_PlayerPerformance.AttemptGoal(_who, _ShotSaveSkillDifference, _TheTime);
}


#ifdef PLAYER_MATCH_STATS
/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SwapPlayers
	Access:    	public 
	Parameter: 	const ushort _PlayerOffSquadID
	Parameter: 	const ushort _PlayerOnSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SwapPlayers(const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID)
{
	CPlayerMatchStats PlayerStats(m_PlayerMatchStats[_PlayerOnSquadID]);
	m_PlayerMatchStats[_PlayerOnSquadID] = m_PlayerMatchStats[_PlayerOffSquadID];
	m_PlayerMatchStats[_PlayerOffSquadID] = PlayerStats;
	m_PlayerPerformance.Swap(_PlayerOffSquadID, _PlayerOnSquadID);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoConcededShot
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoConcededShot(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	m_PlayerPerformance.DoConcededShot(_PlayerSquadID, _ShotSaveSkillDifference, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoAttemptSave
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoAttemptSave(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
	m_PlayerPerformance.DoAttemptSave(_PlayerSquadID, _ShotSaveSkillDifference, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoInitialiseExtraTime
	Access:    	public 
	Parameter: 	const byte _ExtraTimeLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoInitialiseExtraTime(const byte _ExtraTimeLength)
{
	m_PlayerPerformance.DoInitialiseExtraTime(_ExtraTimeLength);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoPlayerDefendAttempt
	Access:    	public 
	Parameter: 	const ushort _PlayerNum
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Parameter: 	const bool _WasMidfielder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoPlayerDefendAttempt(const ushort _PlayerNum, const sbyte _ShotSaveSkillDifference, const byte _TheTime, const bool _WasMidfielder)
{
	m_PlayerPerformance.DoDefendAttempt(_PlayerNum, _ShotSaveSkillDifference, _TheTime, _WasMidfielder);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoTakePlayerOff
	Access:    	public 
	Parameter: 	byte _TheTime
	Parameter: 	const ushort _SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoTakePlayerOff(byte _TheTime, const ushort _SquadNumber)
{
	m_PlayerPerformance.DoClearSkillFrom(_SquadNumber, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoMakeSubstitution
	Access:    	public 
	Parameter: 	byte _TheTime
	Parameter: 	const ushort _PlayerOffSquadID
	Parameter: 	const ushort _PlayerOnSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoMakeSubstitution(byte _TheTime, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID)
{
	m_PlayerPerformance.DoClearSkillFrom(_PlayerOffSquadID, _TheTime);
	m_PlayerPerformance.DoClearSkillTo(_PlayerOnSquadID, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementAssists
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	sbyte _ShotSaveSkillDifference
	Parameter: 	byte _When
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementAssists(const ushort _PlayerSquadID, sbyte _ShotSaveSkillDifference, byte _When)
{ 
	m_PlayerPerformance.AssistedShot(_PlayerSquadID, _ShotSaveSkillDifference, _When);
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementAssists();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementShotsOnTarget
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementShotsOnTarget(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _Time)
{ 
	m_PlayerPerformance.AttemptGoal(_PlayerSquadID, _ShotSaveSkillDifference, _Time);
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementShotsOnTarget();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementShotsOffTarget
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementShotsOffTarget(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _Time)
{ 
	m_PlayerPerformance.AttemptGoal(_PlayerSquadID, _ShotSaveSkillDifference, _Time);
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementShotsOffTarget(); 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementGoals
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementGoals(const ushort _PlayerSquadID)
{
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementGoals();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementBookings
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementBookings(const ushort _PlayerSquadID)				
{
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementBookings();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoIncrementSendingOffs
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoIncrementSendingOffs(const ushort _PlayerSquadID)				
{
	m_PlayerMatchStats[_PlayerSquadID].DoIncrementSendingOffs();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetPlayerPerformanceList
	Access:    	public 
	Returns:   	CPlayerPerformanceList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformanceList& CMatchVarsData::GetPlayerPerformanceList()
{
	return m_PlayerPerformance;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetPlayerPerformance
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Returns:   	CPlayerPerformance&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformance& CMatchVarsData::GetPlayerPerformance(const ushort _PlayerSquadID)
{
	return *m_PlayerPerformance[_PlayerSquadID];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::GetPlayerMatchStats
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Returns:   	CPlayerMatchStats*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerMatchStats* CMatchVarsData::GetPlayerMatchStats(const ushort _PlayerSquadID)
{
	return &m_PlayerMatchStats[_PlayerSquadID];
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerMatchDetails::CPlayerMatchDetails
	Access:    	public 
	Parameter: 	CMatchVars * _MatchVars
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerMatchDetails::CPlayerMatchDetails(CMatchVars* _MatchVars)
{
	EXT_ASSERT_POINTER(_MatchVars, CMatchVars);
	DoResetVariables();
	m_MatchVars = _MatchVars;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerMatchDetails::DoCalculatePerformanceRating
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CPlayerMatchDetails::DoCalculatePerformanceRating()
{
	if (m_PlayerMatchStats != NULL)
	{
// 		g_DebugStr.Format(_T("%s  Perform %d Rating %d Skill %d"), m_ClubAndPlayerID.GetPlayer().GetName(), m_PlayerMatchStats->DoCalculatePerformanceRating(), m_PlayerPerformance->GetOverallRating(), m_ClubAndPlayerID.GetPlayer().GetSkills().GetOverallSkill());
// 		g_DebugStr.OutputDebug();
		int Rating = m_PlayerMatchStats->DoCalculatePerformanceRating() + m_PlayerPerformance->GetOverallRating() + m_ClubAndPlayerID.GetPlayer().GetSkills().GetOverallSkill();
		return Rating;
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerMatchDetails::DoInitialise
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const ushort _ClubID
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerMatchDetails::DoInitialise(const ushort _PlayerSquadID, const ClubID _ClubID, const eWhichTeam _Who)
{
	EXT_ASSERT(_PlayerSquadID != NOPLAYER && _PlayerSquadID < PLAYERSINSQUAD);
	m_PlayerSquadID = _PlayerSquadID;
	EXT_ASSERT(_ClubID.id != NOCLUB);
	m_ClubAndPlayerID.SetPlayerID(WorldData().GetClubByID(_ClubID).GetPlayerID(_PlayerSquadID));
	m_ClubAndPlayerID.SetClubID(_ClubID);
	m_PlayerMatchStats = m_MatchVars->GetMatchVarsTeamData(_Who).GetPlayerMatchStats(_PlayerSquadID);
	m_PlayerPerformance = &m_MatchVars->GetMatchVarsTeamData(_Who).GetPlayerPerformance(_PlayerSquadID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerMatchDetails::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerMatchDetails::DoResetVariables()
{
	m_PlayerSquadID = NOPLAYER;
	m_ClubAndPlayerID.DoResetVariables();
	m_PlayerMatchStats = NULL;
	m_PlayerPerformance = NULL;
	m_MatchVars = NULL;
}


ushort CPlayerMatchDetails::GetPlayerSquadID()
{
	EXT_ASSERT(m_PlayerSquadID != NOPLAYER);
	return m_PlayerSquadID;
}
