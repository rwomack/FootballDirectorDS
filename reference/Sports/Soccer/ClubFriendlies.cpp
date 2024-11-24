 /*

        Class:- clubFriendlies

		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "MatchInfo.h"


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::CClubFriendlies
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubFriendlies::CClubFriendlies()
{
	OnNewSeason();
};


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::GetOpponents
	Access:    	public 
	Parameter: 	const ushort _Index
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubFriendlies::GetOpponents(const ushort _Index)
{
	return m_FriendlyOpponentsClubID[_Index];
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::GetVenue
	Access:    	public 
	Parameter: 	const ushort nIndex
	Returns:   	eWhichTeam
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CClubFriendlies::GetVenue(const ushort nIndex)
{
	return static_cast<eWhichTeam>(m_FriendlyVenue[nIndex]);
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::CreateFriendlyMatch
	Access:    	public 
	Parameter: 	const ushort FriendlyNum
	Parameter: 	const ushort Opponent
	Parameter: 	const eWhichTeam Venue
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFriendlies::CreateFriendlyMatch(const ushort FriendlyNum, const ushort _OpponentClubID, const eWhichTeam Venue)
{
	EXT_ASSERT(FriendlyNum < MAXFRIENDLIES);
	m_FriendlyOpponentsClubID[FriendlyNum] = _OpponentClubID;
	m_FriendlyVenue[FriendlyNum]  = Venue;
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::OnNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFriendlies::OnNewSeason()
{
	for (int LoopCount = 0; LoopCount < MAXFRIENDLIES; LoopCount++)
	{
		m_FriendlyOpponentsClubID[LoopCount] = NOCLUB;
   		m_FriendlyVenue[LoopCount] =  HOME;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::FriendlyOpponentName
	Access:    	public 
	Parameter: 	const ushort FriendlyNum
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CClubFriendlies::FriendlyOpponentName(const ushort FriendlyNum)
{
	EXT_ASSERT(FriendlyNum < MAXFRIENDLIES);
	if (m_FriendlyOpponentsClubID[FriendlyNum] == NOCLUB)
	{
		return TEXT("");
	}
	else
	{
        return WorldData().GetClub(m_FriendlyOpponentsClubID[FriendlyNum]).GetName();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::GetStadiumNameStr
	Access:    	public 
	Parameter: 	const ushort FriendlyNum
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CClubFriendlies::GetStadiumNameStr(const ushort FriendlyNum, CClub* _ThisClub)
{
	EXT_ASSERT(FriendlyNum < MAXFRIENDLIES);
	if (m_FriendlyOpponentsClubID[FriendlyNum] == NOCLUB)
	{
		return TEXT("");
	}
	else
	{
		if (m_FriendlyVenue[FriendlyNum] == HOME)
		{
			return _ThisClub->GetStadium()->GetName();
		}
		else
		{
			return WorldData().GetClub(m_FriendlyOpponentsClubID[FriendlyNum]).GetStadium()->GetName();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::IsFriendlyMatchToday
	Access:    	public 
	Parameter: 	const ushort _MatchNumber
	Parameter: 	CMatchInfo & _Match
	Parameter: 	CClub * _ThisClub
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubFriendlies::IsFriendlyMatchToday(const ushort _MatchNumber, CMatchInfo& _Match, CClub* _ThisClub)
{
    int FriendlyNum = _ThisClub->GetDivision().WhichFriendlyNumberToday(_MatchNumber);
	if (FriendlyNum == NOFRIENDLY || m_FriendlyOpponentsClubID[FriendlyNum] == NOCLUB)
	{
		return false;
	}

	ushort HomeClubID, AwayClubID;
	if (m_FriendlyVenue[FriendlyNum] == HOME)
	{
        HomeClubID = WorldData().GetClubList().Find(_ThisClub);
		AwayClubID = m_FriendlyOpponentsClubID[FriendlyNum];
	}
	else {
        AwayClubID = WorldData().GetClubList().Find(_ThisClub);
		HomeClubID = m_FriendlyOpponentsClubID[FriendlyNum];
	}

	_Match.DoInitialiseFriendlyMatch(HomeClubID, AwayClubID);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::CanPlayFriendly
	Access:    	public 
	Parameter: 	const ushort MatchNumber
	Parameter: 	const ushort ClubNumber
	Parameter: 	CDivision* pDivision
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubFriendlies::CanPlayFriendly(const ushort MatchNumber, const ushort _ClubID, CDivision* pDivision)
{
	EXT_ASSERT(pDivision != null);
	bool bRet = true;
	ushort FriendlyNum = pDivision->WhichFriendlyNumberToday(MatchNumber);
	if (FriendlyNum == NOFRIENDLY)
	{
		return false;
	}
	// Already arranged for this date
	if (m_FriendlyOpponentsClubID[FriendlyNum] != NOCLUB)
	{
		return false;
	}
	// Can't play same club twice
	if (_ClubID != NOCLUB)
	{
		for (int LoopCount = 0; LoopCount < MAXFRIENDLIES; LoopCount++)
		{
			if (m_FriendlyOpponentsClubID[LoopCount] == _ClubID)
			{
				bRet = false;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubFriendlies::CountAvailableFriendlies
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubFriendlies::CountAvailableFriendlies()
{
	ushort iRet = 0;
	for (uint LoopCount = 0; LoopCount < MAXFRIENDLIES; LoopCount++)
	{
		iRet += (m_FriendlyOpponentsClubID[LoopCount] == NOCLUB);
	}
	return iRet;
}
