

 /*

    File:- LeadingGoalscorer.h

    Leading Goalscorers class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __LEADINGG_H__
#define __LEADINGG_H__


class CPlayerClub
{
	// Variables
private:
	ushort					m_PlayerID;
	ushort					m_ClubID;

public:
    CPlayerClub() {};
	CPlayerClub(const ushort _PlayerID, const ushort _ClubID);

	ushort					GetPlayerNum() const;
	ushort					GetClubNum() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CLeadingGoalScorer : public CPlayerClub
{
	// Variables
private:
	byte					m_LeagueScored;
	byte					m_CupScored;
	ushort					m_Scored;

public:
    CLeadingGoalScorer() {};
	CLeadingGoalScorer(const ushort _PlayerID, const ushort _ClubID, byte LScore, byte CScore);

	byte				GetLeagueScored() const;
	byte				GetCupScored() const;
	ushort				GetTotalScored() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CLeadingGoalScorerList : public CPointerArray<CLeadingGoalScorer>
{
public:
	void  DoCreateList(CDivision& _Division, const ushort _ListSize);
    bool					CheckForList(CDivision& _Division);
	void Sort(const int _Left, const int _Right);
};
#endif
