
/*
    SoccerPlayerPerformance.h: interface for the CSoccerPlayerPerformance class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#if !defined(AFX_SOCCERPLAYERPERFORMANCE_H__AA11CBA4_D9C6_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_SOCCERPLAYERPERFORMANCE_H__AA11CBA4_D9C6_11D1_A3F5_00001C303EC4__INCLUDED_


class CPlayerPerformance 
{
public:
    CPlayerPerformance(byte Value, byte MatchLength);
    CPlayerPerformance() {};
    ~CPlayerPerformance();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Other methods
	sbyte 					AttemptGoal(sbyte _ShotSaveSkillDifference, byte _When);
	sbyte 					AssistedShot(sbyte _ShotSaveSkillDifference, byte _When);
	sbyte 					DoAttemptSave(const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void					DoClearSkillFrom(byte _TheTime);
	void					DoClearSkillTo(byte _TheTime);
	sbyte					DoConcededShot(const sbyte _ShotSaveSkillDifference, const byte _TheTime);
    sbyte                   DefendAttempt(byte ShotSaveSkillDifference, byte TheTime);
	byte					GetSkill(byte time);
	byte 					GetOverallRating(const int _PeriodLength = 0);
	ushort 					HowLongPlayed();
	void					DoInitialise(const byte _Value);				
	void					DoInitialiseExtraTime(byte _ExtraTimeLength);
    void                    DoResetVariables();
	void					Raise(int _When, sbyte _HowMuch);
	void					SetCurrentSkill(byte _Value, byte _Time);

    // Variables
private :
	sbyte					m_Performance;
	byte					m_Skill;
    CNumericArray<byte>		m_PerformanceByMinute;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CPlayerPerformanceList : public CPointerArray<CPlayerPerformance>
{
public:
    CPlayerPerformanceList();        // Constructor
    ~CPlayerPerformanceList();		// Destructor

    // IO Methods
	void  Write(CDataFile* _pDataFile);
	void  Read(CDataFile* _pDataFile);

	// Other methods
	void 					AdjustGoalPerformance(int _Performance, byte _When);
	void 					AdjustDefencePerformance(int _Performance, byte _When);
	void					AdjustMidfieldPerformance(int Performance, byte when);
	void 					AdjustAttackPerformance(int _Performance, byte _When);
	void 					AssistedShot(int _Who, sbyte _ShotSaveSkillDifference, byte _When);
    int						AttackSkill(const int time);						
	void 					AttemptGoal(int _Who, sbyte _ShotSaveSkillDifference, byte _When);
	void 					DoAttemptSave(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void					DoClearSkillFrom(const ushort _SquadNumber, const byte _TheTime);
	void					DoClearSkillTo(const ushort _SquadNumber, const byte TheTime);
	void					DoInitialise(CClub* _Club);
	void					DoInitialiseExtraTime(byte _ExtraTimeLength);
	void					DoConcededShot(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void 					DoDefendAttempt(int PlayerNum, byte _ShotSaveSkillDifference, byte _TheTime, bool _WasMidfielder);
	void					SetPlayerCurrentSkill(int who, byte x, byte when);
	byte					GetPlayerSkill(const ushort _SquadNumber, const byte _TheTime);
	byte					GetOverallRating(const ushort _SquadNumber, const byte PeriodLength);
	void					SetCurrentSkills( byte _GoalSkill, int _DefSkill, int _MidSkill, int _AttSkill, int _StartPosn);
    int						Skill(int time);
	int						GoalSkill(int time);
	int						DefenceSkill(int time);							
    int						MidfieldSkill(const int time);
	void					SetGoalSkill(int time);
	void					SetGoalSkill(int time, byte x);

    // Variables
private:
    CNumericArray<uint>		m_GoalSkill;
    CNumericArray<uint>		m_DefenceSkill;
    CNumericArray<uint>		m_MidfieldSkill;
    CNumericArray<uint>		m_AttackSkill;
    int						m_Performance;
    int			            m_GoalPerformance;
    int			            m_DefencePerformance;
    int			            m_MidfieldPerformance;
    int                     m_AttackPerformance;
    byte					m_GoalCurrentSkill;
    int						m_DefenceCurrentSkill;
    int						m_MidfieldCurrentSkill;
    int						m_AttackCurrentSkill;
};
#endif // !defined(AFX_SOCCERPLAYERPERFORMANCE_H__AA11CBA4_D9C6_11D1_A3F5_00001C303EC4__INCLUDED_)
