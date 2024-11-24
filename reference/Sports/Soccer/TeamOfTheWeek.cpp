
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
	Method:   	CPlayerOfTheWeek::PreMatchClearDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerOfTheWeek::PreMatchClearDown()
{
	m_PlayerID = NOPLAYER;
	m_ClubID = NOCLUB;
	m_Rating = -10;		// Hopefully all performances will be positive but just in case
	m_Skill = 0;
}


 //---------------------------------------------------------------------------------------
CPlayer& CPlayerOfTheWeek::GetPlayer()
{
	EXT_ASSERT(GetPlayerID() != NOPLAYER);
	return *WorldData().GetPlayer(GetPlayerID());
}


 //---------------------------------------------------------------------------------------
CClub& CPlayerOfTheWeek::GetClub()
{
	EXT_ASSERT(GetClubID() != wNOTFOUND);
	return WorldData().GetClub(GetClubID());
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
	if (m_PlayerID != wNOTFOUND)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerOfTheWeek::AfterMatchCheck
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadNumber
	Parameter: 	const ushort iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerOfTheWeek::AfterMatchCheck(const ushort _PlayerSquadNumber, const ushort iClubID)
{
	CClub& theClub = WorldData().GetClub(iClubID);
	if (IsInUse() == false || (theClub.GetPlayer(_PlayerSquadNumber).GetRating() >= m_Rating && theClub.GetPlayer(_PlayerSquadNumber).BeenSentOff() == false))
	{
		m_PlayerID = theClub.GetFirstTeamSquad().GetSquad(_PlayerSquadNumber);
		m_ClubID = iClubID;
		m_Rating = theClub.GetPlayer(_PlayerSquadNumber).GetRating();
		m_Skill = theClub.GetPlayer(_PlayerSquadNumber).GetSkills().GetOverallSkill();
	}
}


#ifdef TEAMOFWEEKONMENU
 //---------------------------------------------------------------------------------------
void CPlayerOfTheWeek::Read(FILE* File)
{
    _pDataFile->Read(&m_PlayerID, sizeof m_PlayerID, 1, File);
    _pDataFile->Read(&m_ClubID, sizeof m_ClubID, 1, File);
    _pDataFile->Read(&m_Rating, sizeof m_Rating, 1, File);
    _pDataFile->Read(&m_Skill, sizeof m_Skill, 1, File);
}


 //---------------------------------------------------------------------------------------
void CPlayerOfTheWeek::Write(FILE* File)
{
    _pDataFile->Write(&m_PlayerID, sizeof m_PlayerID, 1, File);
    _pDataFile->Write(&m_ClubID, sizeof m_ClubID, 1, File);
    _pDataFile->Write(&m_Rating, sizeof m_Rating, 1, File);
    _pDataFile->Write(&m_Skill, sizeof m_Skill, 1, File);
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //  Constructor
 // ----------------------------------------------------------------------------------------------------------------------------
CTeamOfTheWeek::CTeamOfTheWeek()
{
	SetSize(PLAYERSINTEAM);
	for (uint n = 0; n < PLAYERSINTEAM; n ++)
	{
 		SetAt(n, new CPlayerOfTheWeek());
	}
}


 //  Destructor
 // ----------------------------------------------------------------------------------------------------------------------------
CTeamOfTheWeek::~CTeamOfTheWeek()
{
	RemoveList();
}


#ifdef TEAMOFWEEKONMENU
 //---------------------------------------------------------------------------------------
void CTeamOfTheWeek::Read(FILE* File)
{
	for (uint n = 0; n < PLAYERSINTEAM; n ++)
	{
        GetAt(n)->Read(File);
    }
 }


 //---------------------------------------------------------------------------------------
void CTeamOfTheWeek::Write(FILE* File)
{
    for (uint ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
        GetAt(ListPos)->Write(File);
	}
 }
#endif


/*------------------------------------------------------------------------------
	Method:   	CTeamOfTheWeek::PreMatchClearDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamOfTheWeek::PreMatchClearDown()
{
    for (uint ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
        GetAt(ListPos)->PreMatchClearDown();
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
	if (GetAt(0)->IsInUse() == true)
	{
		bRet = true;
	}
	return bRet;
}
