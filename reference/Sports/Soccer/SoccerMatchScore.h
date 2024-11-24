
/*

    File:- SoccerMatchScore.h

    Match to be viewed


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __MATCHSCORE_H__
#define __MATCHSCORE_H__

class CMatchScore
{
public:
	void					DoReset()
	{
		m_AwayGoals = 0;
		m_HomeGoals = 0;
	}
	void					SetScores(const byte nHomeGoals, const byte nAwayGoals)
	{
		m_AwayGoals = nAwayGoals;
		m_HomeGoals = nHomeGoals;
	}
	byte					GetHomeGoals()
	{
		return m_HomeGoals;
	}
	byte					GetAwayGoals()
	{
		return m_AwayGoals;
	}

	// Variables
private:
	byte                   m_AwayGoals;
    byte                   m_HomeGoals;
};
#endif