
 /*

    Class:- PlayerLoan

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::SetWeeksOnLoan
	Access:    	public 
	Parameter: 	const byte _WeeksOnLoan
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::SetWeeksOnLoan(const byte _WeeksOnLoan)
{ 
	m_WeeksOnLoan = _WeeksOnLoan; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::SetClubOnLoanFrom
	Access:    	public 
	Parameter: 	const ushort _ClubOnLoanFromID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::SetClubOnLoanFrom(const ushort _ClubOnLoanFromID)
{ 
	m_ClubOnLoanFromID = _ClubOnLoanFromID; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::DoGoingOnLoan
	Access:    	public 
	Parameter: 	const ushort _ClubOnLoanFromID
	Parameter: 	const byte _WeeksOnLoan
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::DoGoingOnLoan(const ushort _ClubOnLoanFromID, const byte _WeeksOnLoan)
{
	m_WeeksOnLoan = _WeeksOnLoan;
	m_ClubOnLoanFromID = _ClubOnLoanFromID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::DoWeeklyUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::DoWeeklyUpdate()
{
	// If on loan for the season dont modify the weeks left counter
	if (m_WeeksOnLoan != MAXLOANPLAYERWEEKS)
	{
		m_WeeksOnLoan--;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::IsOnLoan
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoan::IsOnLoan()
{
	bool bRet = false;
	if (m_WeeksOnLoan > 0)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::IsLoanedUntilEndOfSeason
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoan::IsLoanedUntilEndOfSeason()
{
	bool bRet = false;
	if (m_WeeksOnLoan == MAXLOANPLAYERWEEKS)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::DoInitialise()
{
	m_WeeksOnLoan = 0;
	m_ClubOnLoanFromID = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::IsLoanedOut
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoan::IsLoanedOut(const ushort _ClubID)
{
	if (m_ClubOnLoanFromID == _ClubID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::GetClubOnLoanFrom
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerLoan::GetClubOnLoanFrom() const
{ 
	return m_ClubOnLoanFromID; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::GetWeeksOnLoan
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerLoan::GetWeeksOnLoan() const
{
	return m_WeeksOnLoan;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoan::DoCheckForSwapClubs
	Access:    	public 
	Parameter: 	const ushort _Club1ID
	Parameter: 	const ushort _Club2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoan::DoCheckForSwapClubs(const ushort _Club1ID, const ushort _Club2ID)
{
	if (m_ClubOnLoanFromID == _Club1ID)
	{
		m_ClubOnLoanFromID = _Club2ID;
	}
	else if (m_ClubOnLoanFromID == _Club2ID)
	{
		m_ClubOnLoanFromID = _Club1ID;
	}
}
