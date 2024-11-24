
 /*

		Program:- MatchTactics.cpp

		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::operator=
	Access:    	public 
	Parameter: 	const CMatchTactics & p
	Returns:   	CMatchTactics&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchTactics& CMatchTactics::operator =(const CMatchTactics &p)
{
	m_TacticsID[HOME] = p.m_TacticsID[HOME];
	m_TacticsID[AWAY] = p.m_TacticsID[AWAY];
	m_TacticTime[HOME] = p.m_TacticTime[HOME];
	m_TacticTime[AWAY] = p.m_TacticTime[AWAY];
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::DoChanged
	Access:    	public 
	Parameter: 	const eWhichTeam eWho
	Parameter: 	CMatchTactics & PreviousTactic
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTactics::DoChanged(const eWhichTeam eWho, CMatchTactics& PreviousTactic)
{
	m_TacticsID[eWho] = PreviousTactic.GetTacticsID(eWho);
	m_TacticTime[eWho] = PreviousTactic.GetTime(eWho);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::DoInitialise
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _TacticsID
	Parameter: 	const byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTactics::DoInitialise(const eWhichTeam _Who, const ushort _TacticsID, const byte _Time)
{
	m_TacticsID[_Who] = _TacticsID;
	m_TacticTime[_Who] = _Time;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::DoInitialise
	Access:    	public 
	Parameter: 	const ushort _HomeTacticsID
	Parameter: 	const ushort _AwayTacticsID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTactics::DoInitialise(const ushort _HomeTacticsID, const ushort _AwayTacticsID)
{
	m_TacticsID[HOME] = _HomeTacticsID;
	m_TacticsID[AWAY] = _AwayTacticsID;
	m_TacticTime[HOME] = 0;
	m_TacticTime[AWAY] = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::GetTacticsID
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CMatchTactics::GetTacticsID(const eWhichTeam _Who) const
{
	return m_TacticsID[_Who];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::GetTime
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchTactics::GetTime(const eWhichTeam _Who) const
{
	return m_TacticTime[_Who];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTactics::DoIncrementTime
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTactics::DoIncrementTime()
{
	m_TacticTime[HOME]++;
	m_TacticTime[AWAY]++;
}
