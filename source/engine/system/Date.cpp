
 /*

		Program:- Date.cpp

		The date class routines

		(c) RW Software 2001 - 2004

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#include <nitro/itcm_begin.h>
#endif

static int m_iMonth[] =
{
	0,
  IDS_JAN,
  IDS_FEB,
  IDS_MAR,
  IDS_APR,
  IDS_MAY,
  IDS_JUN,
  IDS_JUL,
  IDS_AUG,
  IDS_SEP,
  IDS_OCT,
  IDS_NOV,
  IDS_DEC
};

static CJulianDate s_theDate;

/*------------------------------------------------------------------------------
	Method:   	CDate::CDate
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDate::CDate()
{
	m_JulianDate = 0;
};


/*------------------------------------------------------------------------------
	Method:   	CDate::CDate
	Access:    	public
	Parameter: 	const CDate& _timeSrc
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDate::CDate(const CDate& _timeSrc)
{
	m_JulianDate = _timeSrc.m_JulianDate;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::CDate
	Access:    	public
	Parameter: 	const uint _Time
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDate::CDate(const uint _Time)
{
	m_JulianDate = _Time - EPOCH_SHIFT;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::CDate
	Access:    	public
	Parameter: 	const uint _Year
	Parameter: 	const uint _Month
	Parameter: 	const uint _Day
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDate::CDate(const uint _Month, const uint _Day, const uint _Year)
{
	ConvertToJulian(_Month, _Day, _Year);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::ReadFromResource
	Access:    	public
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::ReadFromResource(CBinaryReader& _Resource)
{
    const uint uiFullDate = _Resource.ReadUInt32();
	EXT_ASSERT((uiFullDate - EPOCH_SHIFT) < 0xFFFF);
	//EXT_ASSERT((uiFullDate - EPOCH_SHIFT) > 0);
	m_JulianDate = uiFullDate - EPOCH_SHIFT;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::ConvertToJulian
	Access:    	public
	Parameter: 	const uint _Month
	Parameter: 	const uint _Day
	Parameter: 	const uint _Year
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::ConvertToJulian(const uint _Month, const uint _Day, const uint _Year)
{
	s_theDate.SetMonth((eMonth)_Month);
	s_theDate.SetDay(_Day);
	s_theDate.SetYear(_Year);
	if (s_theDate.GetMonth() < 3)
	{
      s_theDate.AddMonths(12);
      s_theDate.SubYears(1);
	}
	const uint uiFullDate = s_theDate.GetDay() + (153 * s_theDate.GetMonth() - 457) / 5 + 365 * s_theDate.GetYear() + (s_theDate.GetYear() / 4) - (s_theDate.GetYear() / 100) + (s_theDate.GetYear() / 400) + 1721119;
	EXT_ASSERT((uiFullDate - EPOCH_SHIFT) < 0xFFFF);
	//EXT_ASSERT((uiFullDate - EPOCH_SHIFT) > 0);
	m_JulianDate = uiFullDate - EPOCH_SHIFT;
}


#ifdef WINDOWS
#ifdef ORIGINAL_CODE //TODO
/*------------------------------------------------------------------------------
	Method:   	CDate::ConvertFromJulian
	Access:    	public const
	Parameter: 	SYSTEMTIME& _theDate
	Returns:   	void
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
void CDate::ConvertFromJulian(SYSTEMTIME& _theDate) const
{
	EXT_ASSERT(m_JulianDate > 0);
	const uint uiFullDate = m_JulianDate + EPOCH_SHIFT;
	long L = uiFullDate + 68569;
    long N = (long) ((4 * L) / 146097) ;
    L = L - ((long)((146097 * N + 3) / 4) );
    long I = (long) ((4000 *(L + 1) / 1461001));
    L = L - (long)((1461 * I) / 4) + 31;
    long J = (long)((80 * L) / 2447);
    _theDate.wDay = static_cast<WORD>(L - (long)((2447 * J) / 80));
    L = static_cast<long>(J / 11);
    _theDate.wMonth = static_cast<WORD>(J + 2 - 12 * L);
    _theDate.wYear = static_cast<WORD>(100 * (N - 49) + I + L);

	_theDate.wDayOfWeek = GetDayOfWeek() - 1;
	_theDate.wHour = 0;
	_theDate.wMinute = 0;
	_theDate.wSecond = 0;
	_theDate.wMilliseconds = 0;
}
#endif
#endif


/*------------------------------------------------------------------------------
	Method:   	CDate::ConvertFromJulian
	Access:    	public
	Parameter: 	int& Month
	Parameter: 	int& Day
	Parameter: 	int& Year
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::ConvertFromJulian(int& Month,int& Day,int& Year) const
{
	EXT_ASSERT(m_JulianDate > 0);
	const uint uiFullDate = m_JulianDate + EPOCH_SHIFT;
	long L = uiFullDate + 68569;
    long N = (long) ((4 * L) / 146097) ;
    L = L - ((long)((146097 * N + 3) / 4) );
    long I = (long) ((4000 *(L + 1) / 1461001));
    L = L - (long)((1461 * I) / 4) + 31;
    long J = (long)((80 * L) / 2447);
    Day = L - (long)((2447 * J) / 80);
    L=(long)(J / 11);
    Month = J + 2 - 12*L;
    Year = 100 * (N - 49) + I + L ;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::ConvertFromJulian
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::ConvertFromJulian() const
{
	EXT_ASSERT(m_JulianDate > 0);
	const uint uiFullDate = m_JulianDate + EPOCH_SHIFT;
	long L = uiFullDate + 68569;
    long N = (long) ((4 * L) / 146097) ;
    L = L - ((long)((146097 * N + 3) / 4) );
    long I = (long) ((4000 *(L + 1) / 1461001));
    L = L - (long)((1461 * I) / 4) + 31;
    long J = (long)((80 * L) / 2447);
    s_theDate.SetDay(L - (long)((2447 * J) / 80));
    L=(long)(J / 11);
    s_theDate.SetMonth((eMonth)(J + 2 - 12*L));
    s_theDate.SetYear(100 * (N - 49) + I + L);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::DoCalculateDifferenceInYears
	Access:    	public
	Parameter: 	CDate& _rCurrentDate
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDate::DoCalculateDifferenceInYears(CDate& _rCurrentDate)
{
	int CurrentMonth, CurrentDay, CurrentYear;
	_rCurrentDate.ConvertFromJulian(CurrentMonth, CurrentDay, CurrentYear);
	int ThisMonth, ThisDay, ThisYear;
	ConvertFromJulian(ThisMonth, ThisDay, ThisYear);
	if (CurrentMonth > ThisMonth)
	{
		return CurrentYear - ThisYear;
	}
	if (CurrentMonth < ThisMonth)
	{
		return CurrentYear - ThisYear - 1;
	}
	if (CurrentDay < ThisDay)
	{
		return CurrentYear - ThisYear - 1;
	}
	return CurrentYear - ThisYear;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::DoGenerateDateInPast
	Access:    	public
	Parameter: 	CDate& _rCurrentDate
	Parameter: 	const ushort _Years
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::DoGenerateDateInPast(CDate& _rCurrentDate, const ushort _Years)
{
	EXT_ASSERT(_Years > 0);
	int ThisMonth, ThisDay, ThisYear;
	_rCurrentDate.ConvertFromJulian(ThisMonth, ThisDay, ThisYear);
	ThisYear -= _Years;
	ThisMonth = RandomNumber.IntLessThan(11);
	ThisDay = RandomNumber.IntLessThan(28);		// Ensures valid day of month
	ConvertToJulian(ThisMonth, ThisDay, ThisYear);
	if (DoCalculateDifferenceInYears(_rCurrentDate) != _Years)
	{
		SubAYear();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDate::SubAMonth
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::SubAMonth()
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	month--;
	if (month < JANUARY)
	{
		month = DECEMBER;
		year--;
	}
	ConvertToJulian(month, day, year);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::AddMonths
	Access:    	public
	Parameter: 	const uint _Months
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::AddMonths(const uint _Months)
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	month += _Months;
	while (month > DECEMBER)
	{
		month -= DECEMBER;
		year++;
	}
	ConvertToJulian(month, day, year);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::AddAMonth
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::AddAMonth()
{
	AddMonths(1);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::AddADay
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::AddADay()
{
	m_JulianDate++;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::AddDays
	Access:    	public
	Parameter: 	const uint _Days
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::AddDays(const uint _Days)
{
	m_JulianDate += _Days;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::AddAYear
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::AddAYear()
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	year++;
	ConvertToJulian(month, day, year);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::SubADay
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::SubADay()
{
	m_JulianDate--;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::SubAWeek
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::SubAWeek()
{
	m_JulianDate -= 7;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::SubAYear
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::SubAYear()
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	year--;
	ConvertToJulian(month, day, year);
}


/*------------------------------------------------------------------------------
	Method:   	CDate::GetYear
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDate::GetYear() const
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	return year;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::GetMonth
	Access:    	public
	Parameter: 	void
	Returns:   	eMonth
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eMonth CDate::GetMonth() const
{
	ConvertFromJulian();
	return (eMonth)s_theDate.GetMonth();
}


/*------------------------------------------------------------------------------
	Method:   	CDate::GetDay
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDate::GetDay() const
{
	int month, day, year;
	ConvertFromJulian(month, day, year);
	return day;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::GetDayOfWeek
	Access:    	public const
	Parameter: 	void
	Returns:   	eDay
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eDay CDate::GetDayOfWeek() const
{
	const uint uiFullDate = m_JulianDate + EPOCH_SHIFT;
	int res = 1 + (((int)(uiFullDate + 1.5)) % 7);
	return (eDay)res;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::GetDate
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CDate::GetDate() const
{
	return m_JulianDate + EPOCH_SHIFT;
}

uint CDate::GetShiftedDate() const
{
	return m_JulianDate;
}

/*------------------------------------------------------------------------------
	Method:   	CDate::SetDate
	Access:    	public
	Parameter: 	const uint x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDate::SetDate(const uint x)
{
	EXT_ASSERT((x - EPOCH_SHIFT) < 0xFFFF);
	//EXT_ASSERT((x - EPOCH_SHIFT) > 0);
	m_JulianDate = x - EPOCH_SHIFT;
}

void CDate::SetShiftedDate(const uint x)
{
	m_JulianDate = x;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::LongDateStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::LongDateStr()
{
	ConvertFromJulian();

	CString str;
  LoadText( str, m_iMonth[s_theDate.GetMonth()] );
	CString rStr;
	rStr.Format(_T("%d %s %d"), s_theDate.GetDay(), str.CStr(), s_theDate.GetYear());
	return rStr;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::ShortDateStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::ShortDateStr()
{
	ConvertFromJulian();

	CString szYear;
	szYear.Format(_T("%d"), s_theDate.GetYear());
	CString rStr;
	rStr.Format(_T("%02d.%02d.%s"), s_theDate.GetDay(), s_theDate.GetMonth(), szYear.Right(2));
	return rStr;
}

/*------------------------------------------------------------------------------
  Method:     CDate::MediumDateStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::MediumDateStr()
{
	ConvertFromJulian();

	CString rStr;
  rStr.Format(_T("%02d.%02d.%04d"), s_theDate.GetDay(), s_theDate.GetMonth(), s_theDate.GetYear());
	return rStr;
}



/*------------------------------------------------------------------------------
	Method:   	CDate::ShortNumDateStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::ShortNumDateStr()
{
#ifdef UNICODE
	wchar_t szDate[20];
#else
	char szDate[20];
#endif
#ifdef ORIGINAL_CODE
#ifdef WINDOWS
	SYSTEMTIME theTime;
	ConvertFromJulian(theTime);
	GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, &theTime, null, szDate, 20);
#else
#endif
#endif
	CString strDate(szDate);
	return strDate;
}


#ifdef WINDOWS
#ifdef ORIGINAL_CODE //TODO
/*------------------------------------------------------------------------------
	Method:   	CDate::GetSystemTime
	Access:    	public
	Parameter: 	void
	Returns:   	SYSTEMTIME
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
SYSTEMTIME CDate::GetSystemTime()
{
	SYSTEMTIME theTime;
	ConvertFromJulian(theTime);
	return theTime;
}
#endif
#endif


/*------------------------------------------------------------------------------
	Method:   	CDate::MonthYearStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::MonthYearStr()
{
	ConvertFromJulian();

	CString strPrint;
  LoadText( strPrint, m_iMonth[s_theDate.GetMonth()] );
	CString szYear;
	szYear.Format(_T("%d"), s_theDate.GetYear());
	CString rStr;
	rStr.Format(_T("%s %s"), strPrint, szYear.Right(2));
	return rStr;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::DayMonthStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::DayMonthStr()
{
	ConvertFromJulian();

	CString strPrint;
  LoadText( strPrint, m_iMonth[s_theDate.GetMonth()] );
	CString rStr;
	rStr.Format(_T("%d %s"), s_theDate.GetDay(), strPrint);
	return rStr;
}


/*------------------------------------------------------------------------------
	Method:   	CDate::MonthStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDate::MonthStr()
{
	ConvertFromJulian();

	CString strPrint;
  LoadText( strPrint, m_iMonth[s_theDate.GetMonth()] );
	return strPrint;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif
