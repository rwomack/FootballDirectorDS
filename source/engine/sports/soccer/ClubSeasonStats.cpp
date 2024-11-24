
 /*

        Class:- club season stats

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::CClubSeasonStats
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CClubSeasonStats::CClubSeasonStats()
	: m_NumberPlayersBookedThisSeason(0)
	, m_NumberPlayersSentOffThisSeason(0)
	, m_TotalAttendanceThisSeason(0)
	, m_BestAttendanceThisSeason(0)
	, m_HomeLeagueAndCupGamesPlayed(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetSeasonDisciplinaryPoints
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubSeasonStats::GetSeasonDisciplinaryPoints()
{
	return m_NumberPlayersBookedThisSeason * BOOKINGPOINTS + m_NumberPlayersSentOffThisSeason * SENDINGOFFPOINTS;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetSeasonBookings
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubSeasonStats::GetSeasonBookings() const					
{ 
	return m_NumberPlayersBookedThisSeason; 
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetSeasonSentOff
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubSeasonStats::GetSeasonSentOff() const 					
{ 
	return m_NumberPlayersSentOffThisSeason; 
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetTotalAttend
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubSeasonStats::GetTotalAttend() const 						
{ 
	return m_TotalAttendanceThisSeason; 
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetBestAttendance
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubSeasonStats::GetBestAttendance() const
{
	return m_BestAttendanceThisSeason;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetHomeLeagueAndCupGamesPlayed
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubSeasonStats::GetHomeLeagueAndCupGamesPlayed() const  	
{ 
	return m_HomeLeagueAndCupGamesPlayed; 
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::SetBestAttendance
	Access:    	public 
	Parameter: 	const uint Num
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubSeasonStats::SetBestAttendance(const uint Num)
{
	m_BestAttendanceThisSeason = Num;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubSeasonStats::DoNewSeason()
{
    m_NumberPlayersBookedThisSeason = 0;
	m_NumberPlayersSentOffThisSeason = 0;
	m_TotalAttendanceThisSeason = 0;
	m_BestAttendanceThisSeason = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::DoAddTotalAttend
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:	Total attendance for home league and cup games this season
------------------------------------------------------------------------------*/
void CClubSeasonStats::DoAddTotalAttend(const uint x)
{
	m_HomeLeagueAndCupGamesPlayed++;
	m_TotalAttendanceThisSeason += x;
	// Check if the best this season
	if (static_cast<uint> (x) < GetBestAttendance())
	{
		m_BestAttendanceThisSeason = x;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetAverageAttendance
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CClubSeasonStats::GetAverageAttendance()
{
	if (GetHomeLeagueAndCupGamesPlayed() > 0)
	{
		return GetTotalAttend() / GetHomeLeagueAndCupGamesPlayed();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::GetAverageAttendanceStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CExtString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString CClubSeasonStats::GetAverageAttendanceStr()
{
	CExtString str;
	str.FormatLargeNumber(GetAverageAttendance());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::DoAddSeasonBookings
	Access:    	public 
	Parameter: 	const ushort aBooked
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubSeasonStats::DoAddSeasonBookings(const ushort _Booked)
{
    m_NumberPlayersBookedThisSeason += _Booked;
}


/*------------------------------------------------------------------------------
	Method:   	CClubSeasonStats::DoAddSeasonSendingOffs
	Access:    	public 
	Parameter: 	const ushort aSentOff
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubSeasonStats::DoAddSeasonSendingOffs(const ushort aSentOff)
{
    m_NumberPlayersSentOffThisSeason += aSentOff;
}
