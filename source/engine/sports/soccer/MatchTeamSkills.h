
/*

    MatchTeamSkills.h: interface for the CMatchTeamSkills class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#if !defined(AFX_MATCHTEAMSKILLS_H__6E950E58_E474_455D_94F2_DA671592E8BD__INCLUDED_)
#define AFX_MATCHTEAMSKILLS_H__6E950E58_E474_455D_94F2_DA671592E8BD__INCLUDED_


class CMatchTeamSkills  
{
public:
	CMatchTeamSkills();
	~CMatchTeamSkills();

	void					CopyStats(CMatchTeamSkills& pStats);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

// Methods
	void					DoResetVariables();
	byte 					GetGoalKeeperSkill() const;
	byte 					GetDefenderSkill() const;
	byte 					GetMidfielderSkill() const;
	byte 					GetAttackerSkill() const;
	void 					SetGoalKeeperSkill(const byte _Value);
	void 					SetDefenderSkill(const byte _Value);
    void					SetMidfielderSkill(const byte x);
    void					SetAttackerSkill(const byte x);

protected:

	static const int GOALKEEPERSKILLBITS = 7;
	static const int DEFENDERSKILLBITS = 7;
	static const int MIDFIELDERSKILLBITS = 7;
	static const int ATTACKERSKILLBITS = 7;

	byte					m_GoalKeeperSkill : GOALKEEPERSKILLBITS;
	byte					m_DefenderSkill : DEFENDERSKILLBITS;
	byte					m_MidfielderSkill : MIDFIELDERSKILLBITS;
	byte					m_AttackerSkill : ATTACKERSKILLBITS;
};

#endif // !defined(AFX_MATCHTEAMSKILLS_H__6E950E58_E474_455D_94F2_DA671592E8BD__INCLUDED_)
