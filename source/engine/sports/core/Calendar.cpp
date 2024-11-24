
/*

	Program:- Calendar.cpp

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"
#include "BaseCompetition.h"

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#include <nitro/itcm_begin.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CCalendar::CCalendar
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CCalendar::CCalendar()
	: m_Week(TOTALMATCHES + 1)
	, m_MatchDay(0)
	, m_Season(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::CCalendar
	Access:    	public 
	Parameter: 	const CCalendar & SomeTime
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar::CCalendar(const CCalendar& SomeTime)
{
    m_Week = SomeTime.m_Week;
    m_MatchDay = SomeTime.m_MatchDay;
    m_Season = SomeTime.m_Season;
    SetDate(SomeTime.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::CCalendar
	Access:    	public 
	Parameter: 	ushort Week
	Parameter: 	const byte Match
	Parameter: 	ushort Season
	Parameter: 	CDate Date
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar::CCalendar(ushort Week, const byte Match, ushort Season, CDate Date)
{
    m_Week = Week;
    m_MatchDay = Match;
	m_Season = Season;
	SetDate(Date.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::CCalendar
	Access:    	public 
	Parameter: 	const int nYear
	Parameter: 	const int nMonth
	Parameter: 	const int nDay
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar::CCalendar(const int nYear, const int nMonth, const int nDay)
{
	DoInitialise(0, 0, 0, nYear, nMonth, nDay);
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator++
	Access:    	public 
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CCalendar::operator ++ ()
{
	NextMatchDay();
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator++
	Access:    	public 
	Parameter: 	int
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CCalendar::operator ++ (int)
{
	NextMatchDay();
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator--
	Access:    	public 
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CCalendar::operator -- ()
{
	PreviousMatchDay();
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator--
	Access:    	public 
	Parameter: 	int
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CCalendar::operator -- (int)
{
	PreviousMatchDay();
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator=
	Access:    	public 
	Parameter: 	const CCalendar & SomeTime
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CCalendar::operator = (const CCalendar& SomeTime)
{
    m_Week = SomeTime.m_Week;
    m_MatchDay = SomeTime.m_MatchDay;
    m_Season = SomeTime.m_Season;
    SetDate(SomeTime.GetDate());
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator<
	Access:    	public 
	Parameter: 	CCalendar Time
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::operator < (const CCalendar &Time) const
{
	return (GetDate() < Time.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator>
	Access:    	public 
	Parameter: 	CCalendar Time
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::operator > (const CCalendar &Time) const
{
	return (GetDate() > Time.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator==
	Access:    	public 
	Parameter: 	CCalendar Time
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::operator == (const CCalendar &Time) const
{
	return (GetDate() == Time.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::operator!=
	Access:    	public 
	Parameter: 	CCalendar Time
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::operator != (const CCalendar &Time) const
{
	return (GetDate() != Time.GetDate());
}

bool CCalendar::operator <=(const CCalendar &Time) const
{
	return (GetDate() <= Time.GetDate());
}

bool CCalendar::operator >=(const CCalendar &Time) const
{
	return (GetDate() >= Time.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::DoInitialise
	Access:    	public 
	Parameter: 	ushort Week
	Parameter: 	const byte Match
	Parameter: 	ushort Season
	Parameter: 	int nYear
	Parameter: 	int nMonth
	Parameter: 	int nDay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCalendar::DoInitialise(ushort Week, const byte Match, ushort Season, int nYear, int nMonth, int nDay)
{
    m_Week = Week;
    m_MatchDay = Match;
	m_Season = Season;
	CDate::ConvertToJulian(nMonth, nDay, nYear);
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::DoInitialise
	Access:    	public 
	Parameter: 	const CCalendar & SomeTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCalendar::DoInitialise(const CCalendar& SomeTime)
{
    m_Week = SomeTime.m_Week;
    m_MatchDay = SomeTime.m_MatchDay;
    m_Season = SomeTime.m_Season;
    SetDate(SomeTime.GetDate());
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::GetWeek
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CCalendar::GetWeek() const							
{ 
	return m_Week;  
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::GetSeason
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CCalendar::GetSeason() const							
{ 
	return m_Season; 
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::GetMatchDay
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCalendar::GetMatchDay() const
{ 
	return m_MatchDay; 
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::DoSetToMatchDayNumber
	Access:    	public 
	Parameter: 	const byte _MatchNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCalendar::DoSetToMatchDay(const byte _MatchDay)
{
	while (GetMatchDay() < _MatchDay)
	{
		NextMatchDay();
	};
	while (GetMatchDay() > _MatchDay)
	{
		PreviousMatchDay();
	};
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::IsWeekEnd
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::IsWeekEnd()
{
    if (CDate::GetDayOfWeek() == SATURDAY)
	{
        return true;
    }
    else
	{
        return false;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::IsNull
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::IsNull()
{
	if (m_Week == TOTALMATCHES + 1)
	{
        return true;
    }
    else
	{
        return false;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::SetMatchDay
	Access:    	public 
	Parameter: 	const byte _MatchDay
	Parameter: 	bool _Backwards
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCalendar::SetMatchDay(const byte _MatchDay, bool _Backwards /*= false*/)
{
	if (_Backwards == true)
	{
		while (m_MatchDay != _MatchDay)
		{
			PreviousMatchDay();
		}
	}
	else
	{
		while (m_MatchDay != _MatchDay)
		{
			NextMatchDay();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::SetMatchDayDirect
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCalendar::SetMatchDayDirect(const byte _Value)
{
	EXT_ASSERT(_Value < TOTALMATCHES);
	m_MatchDay = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::NextMatchDay
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCalendar::NextMatchDay()
{
    bool bNewYear = false;
	do
	{
		CDate::AddADay();
		// Check for new year starting
		if (CDate::GetDay() == 1 && CDate::GetMonth() == 7)
		{
			bNewYear = true;
		}
	}
	while (CDate::GetDayOfWeek() != SATURDAY && CDate::GetDayOfWeek() != TUESDAY);
	m_MatchDay++;
	m_Week += (GetDayOfWeek() == SATURDAY);

    if (bNewYear == true || m_MatchDay == TOTALMATCHES)
	{
		m_MatchDay = 0;
		m_Week = 0;
		m_Season ++;
		// Ensure the first English game is played on a Saturday
		if (CDate::GetDayOfWeek() > TUESDAY)
		{
			m_MatchDay++;
		}
	}
	return bNewYear;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::PreviousMatchDay
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:	Move to the last match day, with wraparound
------------------------------------------------------------------------------*/
bool CCalendar::PreviousMatchDay()
{
	bool bRet = false;
    bool NewYear = false;
	do
	{
		// Check if going back into previous year
		if (CDate::GetDay() == 1 && CDate::GetMonth() == 7)
		{
			NewYear = true;
		}
		CDate::SubADay();
	}
	while (CDate::GetDayOfWeek() != SATURDAY && CDate::GetDayOfWeek() != TUESDAY);
	m_MatchDay--;
	m_Week -= (CDate::GetDayOfWeek() == SATURDAY);

    if (NewYear == true)
	{
		m_MatchDay = TOTALMATCHES - 1;
		m_Week = 52;
		m_Season --;
		// Ensure the first English game is played on a Saturday
		m_MatchDay -= (CDate::GetDayOfWeek() > TUESDAY);
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::FirstDateOfMonth
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:	Is this the first match date of the month ?
------------------------------------------------------------------------------*/
bool CCalendar::FirstDateOfMonth()
{
	CDate t_Date(GetDate());
	// Move back a day at a time until the last match date
	do
	{
		t_Date.SubADay();
	}
	while (t_Date.GetDayOfWeek() != SATURDAY && t_Date.GetDayOfWeek() != TUESDAY);

	// Is it the same month?
	if (t_Date.GetMonth() == CDate::GetMonth())
	{
		return false;
	}
	else
	{
		return true;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::LastMonthStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:	Return the previous month's name
------------------------------------------------------------------------------*/
CString CCalendar::LastMonthStr()
{
	CDate t_Date(GetDate());
	t_Date.SubAMonth();
	return t_Date.MonthStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCalendar::GetSeasonStr
	Access:    	public 
	Parameter: 	const bool _OneYearSeason
	Returns:   	CString
	Qualifier: 	
	Purpose:	Create the season title. eg 03-04
------------------------------------------------------------------------------*/
CString CCalendar::GetSeasonStr(const bool _OneYearSeason)
{
	CString rStr;
	if (_OneYearSeason == false)
	{
		uint Year = CDate::GetYear() - 2000;
		// If after July then this year is first year
		if (CDate::GetMonth() > JUNE)
		{
			Year++;
		}
		rStr.Format(_T("%02d-%02d"), Year - 1, Year);
	}
	else
	{
		rStr.Format(_T("%d"), CDate::GetYear() - 1);
	}
	return rStr;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif
