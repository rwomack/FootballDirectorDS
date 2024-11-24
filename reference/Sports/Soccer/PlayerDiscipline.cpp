
 /*

    Class:- Player Discipline

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoResetVariables()
{
	m_MatchesSuspended = 0;
	m_DisciplinaryPoints = 0;
    m_DisciplinaryPointsThisMatch = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoNewSeason()
{
	m_DisciplinaryPoints = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::PointsStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerDiscipline::PointsStr()
{	
	CString work_str;
    work_str.Format(_T("%3d"), m_DisciplinaryPoints);
    return(work_str);
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::AfterMatchInSquadUpdate
	Access:    	public 
	Parameter: 	CClub & _Club
	Parameter: 	CPlayer & _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::AfterMatchInSquadUpdate(CClub& _Club, CPlayer& _Player)
{
	m_DisciplinaryPointsThisMatch = 0;
    if (m_MatchesSuspended > 0)
	{
		m_MatchesSuspended --;
		// If has served suspension
        if (m_MatchesSuspended == 0)
		{
 			CExtString Str;
			// Club news
			Str.Format(IDS_SUSPENSIONOVER, _Player.GetName());
			_Club.AddToClubNews(Str);

			// Media report
// 			CString strPlayerAndClubName;
// 			strPlayerAndClubName.Format(TEXT("%s (%s)"), _Player.GetName(), _Club.GetName());
// 			Str.Format(IDS_SUSPENSIONOVER, strPlayerAndClubName);
// 			_Club.AddToNewsList(Str, NEWSDISCIPLINE);			// Add to league headquarters news
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::IsNearSuspension
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerDiscipline::IsNearSuspension() const
{
	if (m_DisciplinaryPoints >= (POINTSFORSUSPENSION + BOOKINGPOINTS))
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoAfterMatchInTeamUpdate
	Access:    	public 
	Parameter: 	CClub & _Club
	Parameter: 	CPlayer & _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoAfterMatchInTeamUpdate(CClub& _Club, CPlayer& _Player)
{
	// Suspend him if for 2 matches if has reached 22 disciplinary points
	if (m_DisciplinaryPoints >= POINTSFORSUSPENSION)
	{
		if (m_DisciplinaryPointsThisMatch == SENDINGOFFPOINTS)
		{
			DoBeenSuspended(3, _Club, _Player);
		}
		else
		{
			DoBeenSuspended(2, _Club, _Player);
		}
        // Remove 22 disciplinary points if just suspended
        m_DisciplinaryPoints -= POINTSFORSUSPENSION;
	}
	else if (m_DisciplinaryPointsThisMatch == SENDINGOFFPOINTS)
	{
		// Suspend for a match if sent off in this match
		DoBeenSuspended(1, _Club, _Player);
	}
	m_DisciplinaryPointsThisMatch = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoBeenSuspended
	Access:    	public 
	Parameter: 	const byte _HowMany
	Parameter: 	CClub & _Club
	Parameter: 	CPlayer & _Player
	Returns:   	void
	Qualifier: 	
	Purpose:	Suspend the player for specified number of matches
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoBeenSuspended(const byte _HowMany, CClub& _Club, CPlayer& _Player)
{
	CExtString strClubNews, strMediaNews;
	m_MatchesSuspended += _HowMany;

	CString str = _Player.PlayerAndClubName();
	if (_HowMany > 1)
	{
		strClubNews.Format(IDS_SUSPENSIONSTARTED, _Player.GetName(), m_MatchesSuspended);
		strMediaNews.Format(IDS_SUSPENSIONSTARTED, str, m_MatchesSuspended);
	}
	else
	{
		strClubNews.Format(IDS_SUSPENDED, _Player.GetName());
		// Dont swamp the news with trivial suspensions
//		strMediaNews.Format(IDS_SUSPENDED, str);
	}
	_Club.AddToClubNews(strClubNews);
	_Club.AddToNewsList(strMediaNews);
//@@	pClub->NewsPaperEventPtr()->CheckIfNewsWorthySuspension(WorldData().PlayerList()->Find(this));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoBookPlayer
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoBookPlayer()
{
    m_DisciplinaryPointsThisMatch = BOOKINGPOINTS;
	m_DisciplinaryPoints += BOOKINGPOINTS;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::DoSendPlayerOff
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::DoSendPlayerOff()
{
    m_DisciplinaryPointsThisMatch = SENDINGOFFPOINTS;
    m_DisciplinaryPoints += SENDINGOFFPOINTS;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::SuspensionStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CPlayerDiscipline::SuspensionStr()
{
	CString szStr;
	if (GetMatchesSuspended() > 1)
	{
		szStr.Format(IDS_SUSPENSIONDETAILS, GetMatchesSuspended());
	}
	if (GetMatchesSuspended() == 1)
	{
		szStr.Format(IDS_SUSPENSIONDETAIL);
	}
	return szStr;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::GetDisciplinaryPoints
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerDiscipline::GetDisciplinaryPoints() const				
{ 
	return m_DisciplinaryPoints; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::GetDisciplinaryPointsThisMatch
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerDiscipline::GetDisciplinaryPointsThisMatch() const				
{ 
	return m_DisciplinaryPointsThisMatch; 
} 


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::SetDiscMatch
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::SetDiscMatch(const byte x)                           
{ 
	EXT_ASSERT(x >= 0); 
	m_DisciplinaryPointsThisMatch = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::SetSuspended
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerDiscipline::SetSuspended(const byte x)
{ 
	EXT_ASSERT(x >= 0); 
	m_MatchesSuspended = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerDiscipline::GetMatchesSuspended
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerDiscipline::GetMatchesSuspended() const
{
	return m_MatchesSuspended;
}
