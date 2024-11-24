
/*

    Match Handler Class -


	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
 */


#ifndef __MATCHHANDLER_H__
#define __MATCHHANDLER_H__


class CMatchHandler
{
public:
	CMatchHandler() {};


	void					DoMatch(CFixture* _Fixture, CCup* _Cup = null, CCupRound* _CupRound = null, CFixture* _LastFixture = null);
	void					DoDivisionMatch(CFixture* _Fixture, CDivision* _Division);
	CCupRound*				GetCupRound();
	CMatchPlay&				GetMatchPlay();					
	CMatchVars&				GetMatchVars();
	CMatchSubs&				GetMatchSubs();
	bool					IsCupMatch();
	void					SetCompetitionInfo(CDivision* _Division, CCupRound* _CupRound = NULL, CCup* _Cup = NULL);

   // Variables
private:
	CMatchPlay				m_MatchPlay;
	CMatchVars				m_MatchVars;
	CMatchSubs				m_MatchSubs;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif