#ifndef __PLAYERDISCIPLINE_H__
 #define __PLAYERDISCIPLINE_H__


/*

    Player Discipline class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


	static const uint		BOOKINGPOINTS = 3;
	static const uint		SENDINGOFFPOINTS = 6;
	static const uint		POINTSFORSUSPENSION = 22;

	
class CPlayerDiscipline
{
public:
	CPlayerDiscipline() 
		: m_MatchesSuspended(0)
		, m_DisciplinaryPoints(0)
		, m_DisciplinaryPointsThisMatch(0)
	{};

	CString					PointsStr();
    byte					GetDisciplinaryPoints() const;
    byte					GetDisciplinaryPointsThisMatch() const;
    void					SetDiscMatch(const byte x);
	void					SetSuspended(const byte x);
	byte					GetMatchesSuspended() const;
	void					DoNewSeason();
	void DoAfterMatchInTeamUpdate(CClub& _Club, CPlayer& _Player);
	void					AfterMatchInSquadUpdate(CClub& _Club, CPlayer& _Player);
	void DoBeenSuspended(const byte _HowMany, CClub& _Club, CPlayer& _Player);
	void					DoBookPlayer();
	void					DoResetVariables();
	void					DoSendPlayerOff();
	bool					IsNearSuspension() const;
	CString					SuspensionStr();

protected:
	ushort					m_MatchesSuspended:6;
    ushort					m_DisciplinaryPoints:6;
    ushort					m_DisciplinaryPointsThisMatch:4;           // Number of disciplinary points this match
};
#endif