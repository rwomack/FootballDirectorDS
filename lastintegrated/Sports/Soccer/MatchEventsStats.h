
/*

    Match Events Stats Class -

	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
*/


#ifndef __MATCHEVENTSSTATS_H__
#define __MATCHEVENTSSTATS_H__


class	CMatchEventsStats
{
// Variables
private:
	CMatchTeamSkills		m_TeamSkills;
    uint					m_PercentageOfPlay:7;
    uint					m_ShotsOnTarget:4;
    uint					m_ShotsOffTarget:4;
	uint					m_FreeKicks:4;
	uint					m_OffSides:4;
	uint					m_Fouls:4;
	uint					m_Corners:4;
    ushort					m_ClubID;			// Club number of team
#ifdef STOREMATCHINJURIES
	byte					m_Injuries;
#endif
#ifdef STOREMATCHPOSESSION
    double                  m_Posession;
#endif

public:
	CMatchEventsStats();

	void					CopyStats(CMatchEventsStats& pStats);

// Methods
	CMatchTeamSkills&		GetTeamSkills();
    ushort					GetClubID() const;
    sbyte					GetShotsOnTarget();
    sbyte					GetShotsOffTarget();
    byte					GetOffsides();
    byte					GetFouls();
    byte					GetCorners();
    byte					GetFreeKicks();
    byte					GetPercentageOfPlay() const;
    void					DoIncrementCorners();
    void					DoIncrementFreeKicks();		
    void					DoIncrementOffsides();
	void					DoIncrementFouls();
    void					DoIncrementShotsOnTarget();
    void					DoIncrementShotsOffTarget();
	void					DoAddShotsOnTarget(const byte _Value);
	void 					SetShotsOnTarget(const byte _Value);
	void 					SetShotsOffTarget(const byte _Value);
	void 					SetCorners(const byte _Value);
	void 					SetFreeKicks(const byte _Value);
	void 					SetOffsides(const byte _Value);
	void					SetFouls(const byte _Value);
    void					SetPercentageOfPlay(const byte _Value);
	void					SetClubID(const ushort _ClubID);
	void					DoResetVariables();
	byte					Performance();

#ifdef STOREMATCHPOSESSION
	void				DoIncrementPosession()		          	{ m_Posession++; }
	void				DoDecrementPosession()		          	{ m_Posession--; }
#endif
#ifdef STOREMATCHINJURIES
    void				SetInjuries(const byte x)	   				{ m_Injuries = x; }
#endif
};
#endif
