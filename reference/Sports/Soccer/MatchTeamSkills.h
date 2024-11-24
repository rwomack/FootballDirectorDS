
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
	virtual ~CMatchTeamSkills();

	void					CopyStats(CMatchTeamSkills& pStats);

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
    byte					m_GoalKeeperSkill;
    byte					m_DefenderSkill;
    byte					m_MidfielderSkill;
    byte					m_AttackerSkill;
};

#endif // !defined(AFX_MATCHTEAMSKILLS_H__6E950E58_E474_455D_94F2_DA671592E8BD__INCLUDED_)
