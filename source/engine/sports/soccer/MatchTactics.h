
/*

    File:- MatchTactics.h

    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __MATCHTACTICS_H__
#define __MATCHTACTICS_H__

class CMatchTactics
{
public:
	CMatchTactics& operator =(const CMatchTactics &p);

	void					DoChanged(const eWhichTeam eWho, CMatchTactics& PreviousTactic);
	void					DoIncrementTime();
	void					DoInitialise(const eWhichTeam _Who, const ushort _TacticsID, const byte _Time);
	void					DoInitialise(const ushort _HomeTacticsID, const ushort _AwayTacticsID);
	ushort					GetTacticsID(const eWhichTeam _Who) const;
	byte					GetTime(const eWhichTeam _Who) const;

protected:
	ushort					m_TacticsID[2];
	byte					m_TacticTime[2];
};
#endif
