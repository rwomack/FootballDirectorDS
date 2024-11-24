
/*

	MatchVars.h: interface for the CMatchVars class.


	Football Director

	(c) Rw Software 1994 - 2008


  ---------------------------------------------------------------
*/


#if !defined(MATCHVARS)
#define MATCHVARS


class CTeamTactic;


class CMatchVars 
{
	// Variables
private:
    CDivision*				m_pCurrentDivision;     // Should point to the division being used
	CCup*					m_Cup;
	CCupRound*				m_pCurrentCupRound;     // Should point to the Cup round being used
	CPersonName*			m_pLastScorer;
    CPersonName*            m_pLastAssistScorerName;
    CPersonName*            m_pLastFoulerName;
    CPersonName*            m_pLastVictimName;
    CNumericArray<byte>		m_GoalTimesList;              // Times the goals were scored
    int                     m_LastInjuredSquadNumber;
	CMatchVarsData			m_MatchVars[2];
	CNumericArray<ushort>	m_nGoalChanceTime;

public:
	CMatchVars();

    // Other methods
    void                    DoAdjustDisciplinaryEventsForExtraTime();
	ushort					DoCalculateShotTime(ushort _EndTime);
	CClub&					GetClub(const eWhichTeam _Who);
	ushort					GetMatchTimeSectorSize();
    CPersonName*			GetLastFoulerName() const;					
    CPersonName*			GetLastVictimName() const;					
	CPersonName*			GetLastScorer(); 							
	CPersonName*			GetLastAssistScorerName();					
	CDivision*				GetDivision(); 								
	CCup*					GetCup();
	CCupRound*				GetCupRound();     // Should point to the Cup round being used
	bool					IsCupMatch() const;
	bool					IsLeagueMatch() const;
	void					SetCompetitionInfo(CDivision* _pDivision, CCupRound* _pCupRound, CCup* _Cup = NULL);
	void 					DoInitialiseClubs(const byte _SubsSelect, const eMatchCompetitionType _MatchType);
	void 					DoInitialiseExtraTime(const byte _ExtraTimeLength);
	void 					DoInitialiseFromTactics(CTactic* _Tactics, const eWhichTeam _Which);
	ushort					GetLastInjuredPlayerSquadNumber();
	void					DoResetVariables();
	void					SetTemperament(const eWhichTeam _Who, const byte _Value);   
	void					AssistedGoal(const eWhichTeam _Who, const ushort _AssisterPlayerID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void					AssistedShot(const eWhichTeam who, const ushort _AssistPlayerID, const sbyte _ShotSaveSkillDifference, const byte _Time);
	void 					DoShotOffTarget(eWhichTeam _Who, ushort PlayerID, byte _Time);
	void 					DoShotOnTarget(eWhichTeam _Who, ushort _PlayerID, byte _Time);
	void					DoTakePlayerOff(byte _TheTime, eWhichTeam _Who, const ushort _SquadNumber);
	void 					PlayerScoredGoal(eWhichTeam _WhichTeam, int _who, sbyte _ShotSaveSkillDifference, byte _TheTime);
	void					PlayerConcededShot(eWhichTeam _WhichTeam, const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void					DoPlayerDefendAttempt(eWhichTeam _Who, int _PlayerNum, sbyte _ShotSaveSkillDifference, byte _TheTime, bool _WasMidfielder);
	void					GoalkeeperAttemptSave(const eWhichTeam _WhichTeam, const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime);
	void 					MakeSubstitution(byte _TheTime, const eWhichTeam _Who, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID);
	void 					SwapPlayersOnPitch(eWhichTeam _Who, const ushort _PlayerOffSquadID, const ushort _PlayerOnSquadID);
	int						GetPlayerSkill(const eWhichTeam _Who, const ushort _SquadNumber, byte _TheTime);
	void					SetLastInjuredPlayerSquadNumber(const ushort _SquadNumber); 					
	void					SetLastFoulerName(CPersonName* _Name);
	void 					SetLastVictimName(CPersonName* _Name);
	void 					SetLastScorer(CPersonName* _Name);
	void 					SetLastAssistScorerName(CPersonName* _Name);
	void 					SetHomeAwayClubs(CFixture& _Fixture);
	CPlayerPerformanceList& GetSquadPerformance(const eWhichTeam _Who);
	CMatchVarsData& 		GetMatchVarsTeamData(eWhichTeam _Who);

	byte					GetTeamAggression(eWhichTeam _Who);
	byte					GetStyleOfPlay(eWhichTeam _Who);
	bool					GetCounterAttack(eWhichTeam _Who);
	bool					GetOffsideTrap(eWhichTeam _Who);
    byte					GetTeamMorale(eWhichTeam Who);
	sbyte					GetNumberPlayersToSendOff(eWhichTeam Who);
	sbyte					GetNumberPlayersToBook(eWhichTeam Who);
	byte					GetTemperament(eWhichTeam _Who);

	uint					GetShootingSkills(const eWhichTeam _Who);					
    int						NearestAboveShootingSkills(const eWhichTeam _Who, const uint aShotTaker);	
#ifdef TESTSTATS
	void  					DumpTestStats(FILE* _pFile);
#endif
};

#endif
