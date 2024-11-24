
 /*
    File:- CupWinnersInfo.cpp

    CupWinnersInfo class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CCupWinnersInfo::CCupWinnersInfo
	Access:    	public 
	Returns:   	
	Qualifier: 	
------------------------------------------------------------------------------*/
CCupWinnersInfo::CCupWinnersInfo()
	: m_CupWinnersClubID(NOCLUB)
	, m_CupRunnersUpID(NOCLUB)
{
	m_CupSemiLosers[0] = NOCLUB;
	m_CupSemiLosers[1] = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CCupWinnersInfo::SetWinnersAndRunnersUp
	Access:    	public 
	Parameter: 	const ushort _nWinnersClubID
	Parameter: 	const ushort _nRunnersUpClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupWinnersInfo::SetWinnersAndRunnersUp(const ushort _nWinnersClubID, const ushort _nRunnersUpClubID)
{
	EXT_ASSERT(_nWinnersClubID < NOCLUB && _nRunnersUpClubID < NOCLUB);
	m_CupWinnersClubID = _nWinnersClubID;
	m_CupRunnersUpID = _nRunnersUpClubID;
}


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
	Method:   	CCupWinnersInfo::SetSemiLosers
	Access:    	public 
	Parameter: 	const ushort _Which
	Parameter: 	const ushort _nClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupWinnersInfo::SetSemiLosers(const ushort _Club1ID, const ushort _Club2ID)
{
	m_CupSemiLosers[0] = _Club1ID;
	m_CupSemiLosers[1] = _Club2ID;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CCupWinnersInfo::SwapClubs
	Access:    	public 
	Parameter: 	const ushort _nClub1ID
	Parameter: 	const ushort _nClub2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupWinnersInfo::DoSwapClubIDs(const ushort _Club1ID, const ushort _Club2ID)
{
	if (m_CupWinnersClubID == _Club1ID)
	{
		m_CupWinnersClubID = _Club2ID;
	}
	else if (m_CupWinnersClubID == _Club2ID)
	{
		m_CupWinnersClubID = _Club1ID;
	}

	if (m_CupRunnersUpID == _Club1ID)
	{
		m_CupRunnersUpID = _Club2ID;
	}
	else if (m_CupRunnersUpID == _Club2ID)
	{
		m_CupRunnersUpID = _Club1ID;
	}
	if (m_CupSemiLosers[0] == _Club1ID)
	{
		m_CupSemiLosers[0] = _Club2ID;
	}
	else if (m_CupSemiLosers[0] == _Club2ID)
	{
		m_CupSemiLosers[0] = _Club1ID;
	}

	if (m_CupSemiLosers[1] == _Club1ID)
	{
		m_CupSemiLosers[1] = _Club2ID;
	}
	else if (m_CupSemiLosers[1] == _Club2ID)
	{
		m_CupSemiLosers[1] = _Club1ID;
	}
}
