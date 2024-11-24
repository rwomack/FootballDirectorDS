
 /*

    Class:- CMatchHandler


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::GetMatchPlay
	Access:    	public 
	Parameter: 	void
	Returns:   	CMatchPlay&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchPlay& CMatchHandler::GetMatchPlay()								
{ 
	return m_MatchPlay; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::GetMatchVars
	Access:    	public 
	Parameter: 	void
	Returns:   	CMatchVars&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchVars& CMatchHandler::GetMatchVars()								
{ 
	return m_MatchVars; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::GetMatchSubs
	Access:    	public 
	Parameter: 	void
	Returns:   	CMatchSubs&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchSubs& CMatchHandler::GetMatchSubs()								
{ 
	return m_MatchSubs; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::IsCupMatch
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchHandler::IsCupMatch()
{
	return GetMatchVars().IsCupMatch();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::GetCupRound
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CMatchHandler::GetCupRound()
{
	return GetMatchVars().GetCupRound();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::SetCompetitionInfo
	Access:    	public 
	Parameter: 	CDivision * _Division
	Parameter: 	CCupRound * _CupRound
	Parameter: 	CCup * _Cup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchHandler::SetCompetitionInfo(CDivision* _Division, CCupRound* _CupRound /*= NULL*/, CCup* _Cup /*= NULL*/)
{
	GetMatchVars().SetCompetitionInfo(_Division, _CupRound, _Cup);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::DoMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CCup * _Cup
	Parameter: 	CCupRound * _CupRound
	Parameter: 	CFixture * _LastFixture
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchHandler::DoMatch(CFixture* _Fixture, CCup* _Cup /*= null*/, CCupRound* _CupRound /*= null*/, CFixture* _LastFixture /*= null*/)
{
	GetMatchVars().SetHomeAwayClubs(*_Fixture);
	GetMatchPlay().DoMatch(_Fixture, _Cup, _CupRound, _LastFixture, GetMatchVars().GetMatchVarsTeamData(HOME).GetClub().GetStadium()->GetName());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchHandler::DoDivisionMatch
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CDivision * _Division
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchHandler::DoDivisionMatch(CFixture* _Fixture, CDivision* _Division)
{
	GetMatchVars().SetCompetitionInfo(_Division, null);
	GetMatchVars().SetHomeAwayClubs(*_Fixture);
    GetMatchPlay().DoMatch(_Fixture, _Division, null, null, null, GetMatchVars().GetMatchVarsTeamData(HOME).GetClub().GetStadium()->GetName());
}
