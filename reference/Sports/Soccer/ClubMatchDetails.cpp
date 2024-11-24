
 /*

        Class:- ClubMatchDetails

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::CClubMatchDetails
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubMatchDetails::CClubMatchDetails() 
{
	OnInitialise();
}


 /*------------------------------------------------------------------------------
 	Method:   	CClubMatchDetails::OnInitialise
 	Access:    	public 
 	Parameter: 	void
 	Returns:   	void
 	Qualifier: 	
 	Purpose:
 ------------------------------------------------------------------------------*/
void CClubMatchDetails::OnInitialise()
{
	m_Scored = 0;
	m_Conceded = 0;
	m_Venue = NOMATCH;
	m_Attendance = 0;
	m_GateReceipts = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::SetVenue
	Access:    	public 
	Parameter: 	const int _Venue
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubMatchDetails::SetVenue(const int _Venue)
{
	m_Venue = _Venue;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::GetScored
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CClubMatchDetails::GetScored()
{
	return m_Scored;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::GetConceded
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CClubMatchDetails::GetConceded()
{
	return m_Conceded;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::DoUpdate
	Access:    	public 
	Parameter: 	CFixture & _Fixture
	Parameter: 	const eWhichTeam _Venue
	Parameter: 	const byte _AwayGatePercentage
	Parameter: 	const byte _FAGatePercentage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubMatchDetails::DoUpdate(CFixture& _Fixture, const eWhichTeam _Venue, const byte _AwayGatePercentage, const byte _FAGatePercentage)
{
	double GateRecieptsPercentage = .5;
	if (_Fixture.GetMatchType() != CUPMATCHNEUTRALVENUE)
	{
		if (_Venue == HOME)
		{
			m_Scored = _Fixture.GetNumberGoals(HOME);
			m_Conceded = _Fixture.GetNumberGoals(AWAY);
			GateRecieptsPercentage = (100 - _AwayGatePercentage - _FAGatePercentage) / 100;
		}
		else
		{
			m_Scored = _Fixture.GetNumberGoals(AWAY);
			m_Conceded = _Fixture.GetNumberGoals(HOME);
			GateRecieptsPercentage = _AwayGatePercentage / 100;
		}
	}
	m_GateReceipts = _Fixture.GetTicketPrice() * _Fixture.GetAttendance() * GateRecieptsPercentage;
	m_Venue = _Venue;
	m_Attendance = _Fixture.GetAttendance();
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::GetVenue
	Access:    	public 
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CClubMatchDetails::GetVenue() const
{
	return m_Venue;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::GetAttendance
	Access:    	public 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubMatchDetails::GetAttendance() const
{
	return m_Attendance;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::GetGateReceipts
	Access:    	public 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubMatchDetails::GetGateReceipts() const
{
	return m_GateReceipts;
}


/*------------------------------------------------------------------------------
	Method:   	CClubMatchDetails::DoCalculateSecurityCosts
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CClubMatchDetails::DoCalculateSecurityCosts()
{
	if (GetVenue() == HOME)
	{
		return ((GetAttendance() + RandomNumber.IntLessThan(max((uint)1, GetAttendance() / 10))) / 100) * 70;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
