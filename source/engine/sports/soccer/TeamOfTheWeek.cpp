
 /*

	Program:- TeamOfTheWeek.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


 //  Constructor
 // ----------------------------------------------------------------------------------------------------------------------------
CPlayerOfTheWeek::CPlayerOfTheWeek()
{
	PreMatchClearDown();
}


 //  Destructor
 // ----------------------------------------------------------------------------------------------------------------------------
CPlayerOfTheWeek::~CPlayerOfTheWeek()
{

}


/*------------------------------------------------------------------------------
Method:   	CPlayerOfTheWeek::GetClub
Access:    	public 
Returns:   	CClub&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CClub& CPlayerOfTheWeek::GetClub()
{
	EXT_ASSERT(GetClubID().id != NOCLUB);
	return WorldData().GetClubByID(GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::GetClubID
	Access:    	public 
	Returns:   	ClubID
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CPlayerOfTheWeek::GetClubID() const
{
	return m_Details.m_ClubAndPlayerID.GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::GetPlayerID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerOfTheWeek::GetPlayerID() const
{
	return m_Details.m_ClubAndPlayerID.GetPlayerID();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::GetPlayer
	Access:    	public 
	Returns:   	CPlayer&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer& CPlayerOfTheWeek::GetPlayer()
{
	EXT_ASSERT(GetPlayerID() != NOPLAYER);
	return *WorldData().GetPlayer(GetPlayerID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::GetPlayerMatchDetails
	Access:    	public 
	Returns:   	CPlayerMatchDetails*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerMatchDetails* CPlayerOfTheWeek::GetPlayerMatchDetails()
{
	return &m_Details;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::IsInUse
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerOfTheWeek::IsInUse()
{
	bool bRet = false;
	if (GetClubID().id != NOCLUB)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::PreMatchClearDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerOfTheWeek::PreMatchClearDown()
{
	m_Details.DoResetVariables();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //  Constructor
 // ----------------------------------------------------------------------------------------------------------------------------
CTeamOfTheWeek::CTeamOfTheWeek()
{
}


 //  Destructor
 // ----------------------------------------------------------------------------------------------------------------------------
CTeamOfTheWeek::~CTeamOfTheWeek()
{
}


/*------------------------------------------------------------------------------
	Method:   	CTeamOfTheWeek::GetAt
	Access:    	public 
	Parameter: 	const ushort _Index
	Returns:   	CPlayerOfTheWeek*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerOfTheWeek* CTeamOfTheWeek::GetAt(const ushort _Index)
{
	EXT_ASSERT(_Index < PLAYERSINTEAM);
	return &m_Players[_Index];
}


/*------------------------------------------------------------------------------
	Method:   	CTeamOfTheWeek::PreMatchClearDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamOfTheWeek::PreMatchClearDown()
{
	if (IsAvailable() == true)
	{
		for (uint ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
		{
			m_Players[ListPos].PreMatchClearDown();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTeamOfTheWeek::IsAvailable
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTeamOfTheWeek::IsAvailable()
{
	bool bRet = false;
	if (m_Players[0].IsInUse() == true)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamOfTheWeek::DoAfterMatchCheck
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadNumber
	Parameter: 	const ClubID iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamOfTheWeek::DoAfterMatchCheck(CPlayerMatchDetails& _PlayerMatchDetails)
{
	CPlayerMatchDetails* PlayerPerformance = m_Players[_PlayerMatchDetails.GetPlayerSquadID()].GetPlayerMatchDetails();
	if (_PlayerMatchDetails.DoCalculatePerformanceRating() > PlayerPerformance->DoCalculatePerformanceRating())
	{
		*PlayerPerformance = _PlayerMatchDetails;
	}
}
