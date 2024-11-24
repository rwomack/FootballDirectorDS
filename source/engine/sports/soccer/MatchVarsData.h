
/*

	MatchVarsData.h: interface for the CMatchVarsData class.


	Football Director

	(c) Rw Software 1994 - 2008


  ---------------------------------------------------------------
*/


#if !defined MATCHVARSDATA
#define MATCHVARSDATA

class CFixture;
class CPlayerMatchStats;
class CMatchVars;


class CPlayerMatchStats
{
public:
	CPlayerMatchStats()
	{
		DoResetVariables();
	}
	CPlayerMatchStats(const CPlayerMatchStats& _PlayerStats)
	{
		*this = _PlayerStats;
	}

	int	DoCalculatePerformanceRating()
	{
		int Rating = m_Assists + (m_Goals * 3) + m_ShotsOffTarget + (m_ShotsOnTarget * 2) - m_Booked - (m_SentOff *3);
		return Rating;
	}
	CPlayerMatchStats&		operator = (const CPlayerMatchStats& _PlayerStats)
	{
		m_Assists = _PlayerStats.m_Assists;
		m_ShotsOnTarget = _PlayerStats.m_ShotsOnTarget;
		m_ShotsOffTarget = _PlayerStats.m_ShotsOffTarget;
		m_Goals = _PlayerStats.m_Goals;
		m_Booked = _PlayerStats.m_Booked;
		m_SentOff = _PlayerStats.m_SentOff;
		return *this;
	}

	void					DoResetVariables()
	{
		m_Assists = 0;
		m_ShotsOnTarget = 0;
		m_ShotsOffTarget = 0;
		m_Goals = 0;
		m_Booked = 0;
		m_SentOff = 0;
	}
	void					DoIncrementAssists()
	{
		m_Assists++; 
	}
	void DoIncrementShotsOnTarget()
	{
		m_ShotsOnTarget++; 
	}
	void DoIncrementShotsOffTarget()
	{
		m_ShotsOffTarget++;
	}
	void				DoIncrementGoals()
	{
		m_Goals++;
	}
	void				DoIncrementBookings()				
	{
		EXT_ASSERT(m_Booked == 0);
		m_Booked++;
	}
	void				DoIncrementSendingOffs()
	{
		EXT_ASSERT(m_SentOff == 0);
		m_SentOff++;
	}
	uint					GetAssists()
	{
		return m_Assists;
	}
	uint					GetShotsOnTarget()
	{
		return m_ShotsOnTarget;
	}
	uint					GetShotsOffTarget()
	{
		return m_ShotsOffTarget;
	}
	uint					GetGoals()
	{
		return m_Goals;
	}
	uint					GetBooked()
	{
		return m_Booked;
	}
	uint					GetSentOff()
	{
		return m_SentOff;
	}

private:
	// Variables
	uint					m_Assists:4;
	uint					m_ShotsOnTarget:4;
	uint					m_ShotsOffTarget:4;
	uint					m_Goals:3;
	uint					m_Booked:1;
	uint					m_SentOff:1;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CPlayerMatchDetails
{
public:
	CPlayerMatchDetails()
	{
		DoResetVariables();
	}
	CPlayerMatchDetails(CMatchVars* _MatchVars);
	
	int						DoCalculatePerformanceRating();
	void					DoInitialise(const ushort _PlayerSquadID, const ClubID _ClubID, const eWhichTeam _Who);
	void					DoResetVariables();
	ushort					GetPlayerSquadID();

	ushort					m_PlayerSquadID;
	CClubAndPlayerID		m_ClubAndPlayerID;
	CPlayerMatchStats*		m_PlayerMatchStats;
	CPlayerPerformance*		m_PlayerPerformance;
	CMatchVars*				m_MatchVars;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	sbyte					m_GoalAttempts;
	byte					m_Temperament;
	byte					m_DayType;
	CPlayerPerformanceList	m_PlayerPerformance;

#ifdef MATCH_HEADING_SKILLS
	CNumericArray<byte>		m_HeadingSkills;
	CNumericArray<uint>		m_HeadingSkillsWhere;
#endif
#ifdef PLAYER_MATCH_STATS
	CPlayerMatchStats		m_PlayerMatchStats[PLAYERSINTEAM + MAXSUBS];
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
	void 					DoInitialise(const byte _SubsSelect, CClub& _OpponentClub, const eMatchCompetitionType _MatchType);
	void					DoResetVariables();
	CClub&					GetClub();
	void					SetClub(const ClubID _ClubID);
	void					ClearShootingSkillLists();
	void					AddToShootingAndHeadingSkillList(const byte _ShotSkill, const byte _HeadSkill);
	void					DoAdjustDisciplinaryEventsForExtraTime();
	CTeamTactic&			GetTeamTactic();								
    byte					GetTeamMorale() const;						
	byte					GetNumberPlayersToSendOff() const;			
	byte					GetNumberPlayersToBook() const;				
    byte					GetTemperament() const;						
	void					SetTeamMorale(const byte _Value);					
	void					SetNumberPlayersToSendOff(const byte _Value);		
	void					SetNumberPlayersToBook(const byte _Value);			
    void					SetTemperament(const byte _Value);					
    CNumericArray<byte>&	GetShootingSkills();							
	CNumericArray<uint>&	GetShootingSkillsWhere();						
	byte					GetGoalAttempts();							
	void					SetGoalAttempts(const sbyte _Value);					
	CPlayerMatchStats*		GetPlayerMatchStats(const ushort _PlayerSquadID);
	CPlayerPerformance&		GetPlayerPerformance(const ushort _PlayerSquadID);
	CPlayerPerformanceList& GetPlayerPerformanceList();

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
	void					SwapPlayers(const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID);
	void					DoIncrementAssists(const ushort _PlayerSquadID, sbyte _ShotSaveSkillDifference, byte _When);			
	void					DoIncrementShotsOnTarget(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _Time);				
	void					DoIncrementShotsOffTarget(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _Time);				
	void					DoIncrementGoals(const ushort _PlayerSquadID);			
	void					DoIncrementBookings(const ushort _PlayerSquadID);				
	void					DoIncrementSendingOffs(const ushort _PlayerSquadID);				
	void 					DoAssistedShot(const ushort _AssistPlayerID, const sbyte _ShotSaveSkillDifference, const byte _Time);
	void 					DoMakeSubstitution(byte _TheTime, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID);
	void 					DoTakePlayerOff(byte _TheTime, const ushort _SquadNumber);
	void 					DoAttemptGoal(const ushort _who, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void 					DoInitialiseExtraTime(const byte _ExtraTimeLength);
	void 					DoPlayerDefendAttempt(const ushort _PlayerNum, const sbyte _ShotSaveSkillDifference, const byte _TheTime, const bool _WasMidfielder);
	void 					DoConcededShot(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void 					DoAttemptSave(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
#endif
};
#endif