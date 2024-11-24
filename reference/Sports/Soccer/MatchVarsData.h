
/*

	MatchVarsData.h: interface for the CMatchVarsData class.


	Football Director

	(c) Rw Software 1994 - 2008


  ---------------------------------------------------------------
*/


#if !defined MATCHVARSDATA
#define MATCHVARSDATA

class CFixture;

class CMatchVarsData
{
private:
	// Variables
	CNumericArray<byte>		m_ShootingSkills;
	CNumericArray<uint>		m_ShootingSkillsWhere;
	CClub*                  m_pClub;
	CMatchZonalStats		m_DefenceZonalStats;
	CMatchZonalStats		m_MidfieldZonalStats;
	CMatchZonalStats		m_AttackZonalStats;
	CTeamTactic				m_TeamTactic;
	byte					m_TeamMorale;
	byte					m_PopularityID;
	byte					m_NumberPlayersToBook;
	byte					m_NumberPlayersToSendOff;
	byte					m_GoalAttempts;
	byte					m_Temperament;
	byte					m_DayType;

#ifdef MATCH_HEADING_SKILLS
	CNumericArray<byte>		m_HeadingSkills;
	CNumericArray<uint>		m_HeadingSkillsWhere;
#endif
#ifdef PLAYER_MATCH_STATS
	CNumericArray<byte>		m_AssistTotals;
	CNumericArray<byte>		m_ShotsOnTarget;
	CNumericArray<byte>		m_ShotsOffTarget;
	CNumericArray<byte>		m_Goals;
#endif

public:
	CMatchVarsData();

	void					AddDefenceVars(CPlayer* _Player);
	void					AddAttackVars(CPlayer* _Player);
	void					AddMidfieldVars(CPlayer* _Player);
	CMatchZonalStats&		GetDefenceZonalStats();
	CMatchZonalStats&		GetMidfieldZonalStats();
	CMatchZonalStats&		GetAttackZonalStats();
	eMatchDayEffect			GetDayType();								
	void					SetDayType(const eMatchDayEffect _eEffect);			
	void					DoInitialise(const byte _SubsSelect, CClub& _OpponentClub);
	void					DoResetVariables();
	CClub&					GetClub();
	void					SetClub(const ushort _ClubID);
	void					ClearShootingSkillLists();
	void					AddToShootingAndHeadingSkillList(const byte _ShotSkill, const byte _HeadSkill);
	void					DoAdjustDisciplinaryEventsForExtraTime();
	CTeamTactic&			GetTeamTactic();								
    byte					GetTeamMorale() const;						
	byte					GetNumberPlayersToSendOff() const;			
	byte					GetNumberPlayersToBook() const;				
    byte					GetTemperament() const;						
    void					SetTeamMorale(const byte x);					
	void					SetNumberPlayersToSendOff(const byte _Value);		
	void					SetNumberPlayersToBook(const byte _Value);			
    void					SetTemperament(const byte _Value);					
    CNumericArray<byte>&	GetShootingSkills();							
	CNumericArray<uint>&	GetShootingSkillsWhere();						
	byte					GetGoalAttempts();							
	void					SetGoalAttempts(const byte _Value);					

#ifdef MATCH_HEADING_SKILLS
    CNumericArray<byte>&	getHeadingSkills()							
	{ 
		return m_HeadingSkills; 
	}
    CNumericArray<uint>&  getHeadingSkillsWhere()						
	{ 
		return m_HeadingSkillsWhere; 
	}
#endif
#ifdef PLAYER_MATCH_STATS
	void				SwapPlayers(const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID);
	void				IncAssistTotals(const ushort Which)			
	{ 
		m_AssistTotals.Increment(Which); 
	}
	void				IncShotsOnTarget(const ushort x)				
	{ 
		m_ShotsOnTarget.Increment(x); 
	}
	void				IncShotsOffTarget(const ushort x)				
	{ 
		m_ShotsOffTarget.Increment(x); 
	}
    CNumericArray<byte>&	getAssistTotals()							
	{ 
		return m_AssistTotals; 
	}
    CNumericArray<byte>&	getShotsOnTarget()							
	{ 
		return m_ShotsOnTarget; 
	}
    CNumericArray<byte>&	getShotsOffTarget()							
	{ 
		return m_ShotsOffTarget;	
	}
    CNumericArray<byte>&  getGoals()									
	{ 
		return m_Goals; 
	}
#endif
};
#endif