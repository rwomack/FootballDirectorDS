
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
{
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
void CCupWinnersInfo::SetWinnersAndRunnersUp(const ClubID _nWinnersClubID, const ClubID _nRunnersUpClubID)
{
	EXT_ASSERT((_nWinnersClubID.id != NOCLUB) && (_nRunnersUpClubID.id != NOCLUB));
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
void CCupWinnersInfo::SetSemiLosers(const ClubID _Club1ID, const ClubID _Club2ID)
{
	m_CupSemiLosers[0] = _Club1ID;
	m_CupSemiLosers[1] = _Club2ID;
}
#endif
