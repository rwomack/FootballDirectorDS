
 /*

    Class:- CMatchVars


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


 // -----------------------------------------------------------------------
CMatchVarsData::CMatchVarsData()
{
#ifdef PLAYER_MATCH_STATS
	m_AssistTotals.SetSize(PLAYERSINTEAM + MAXSUBS);
    m_ShotsOnTarget.SetSize(PLAYERSINTEAM + MAXSUBS);
    m_ShotsOffTarget.SetSize(PLAYERSINTEAM + MAXSUBS);
    m_Goals.SetSize(PLAYERSINTEAM + MAXSUBS);
#endif

	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::DoInitialise
	Access:    	public 
	Parameter: 	const byte _SubsSelect
	Parameter: 	CClub & _OpponentClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::DoInitialise(const byte _SubsSelect, CClub& _OpponentClub)
{
	GetClub().DoSelectTeam(_SubsSelect);
	if (GetClub().IsUserControlled() == true)
	{
		GetClub().GetFirstTeamSquad().DoSelectSpecialPlayers();
	}
	else
	{
		GetClub().DoSelectTactics(&_OpponentClub);
	}
    SetTeamMorale(GetClub().GetMorale());
    m_TeamTactic.Copy(GetClub().GetTeamTactic());
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
	m_pClub = null;
	m_NumberPlayersToBook = 0;
	m_NumberPlayersToSendOff = 0;
	m_GoalAttempts = 0;
	m_TeamTactic.SetDefaults();
#ifdef PLAYER_MATCH_STATS
	m_AssistTotals.FillWithValue(0);
	m_ShotsOnTarget.FillWithValue(0);
	m_ShotsOffTarget.FillWithValue(0);
	m_Goals.FillWithValue(0);
#endif
	m_DefenceZonalStats.DoInitialise();
	m_MidfieldZonalStats.DoInitialise();
	m_AttackZonalStats.DoInitialise();
	m_DayType = NODAYTYPE;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchVarsData::SetClub
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetClub(const ushort _ClubID)
{
	EXT_ASSERT(_ClubID < WorldData().GetClubList().GetSize());
	m_pClub = &WorldData().GetClub(_ClubID); 
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
	EXT_ASSERT(m_pClub);
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
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchVarsData::SetTeamMorale(const byte x)					
{ 
	m_TeamMorale = x; 
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
void CMatchVarsData::SetGoalAttempts(const byte _Value)					
{ 
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
	Method:   	CMatchVarsData::SwapPlayers
	Access:    	public 
	Parameter: 	const ushort _PlayerOffSquadID
	Parameter: 	const ushort _PlayerOnSquadID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
#ifdef PLAYER_MATCH_STATS
void CMatchVarsData::SwapPlayers(const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID)
{
	getAssistTotals().Swap(_PlayerOffSquadID, _PlayerOnSquadID);
	getShotsOnTarget().Swap(_PlayerOffSquadID, _PlayerOnSquadID);
	getShotsOffTarget().Swap(_PlayerOffSquadID, _PlayerOnSquadID);
	getGoals().Swap(_PlayerOffSquadID, _PlayerOnSquadID);
}
#endif


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
