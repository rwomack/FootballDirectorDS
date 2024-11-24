
/*

	Date.h: interface for the CDate class.

	(c) RW Software 1994 - 2006

  ---------------------------------------------------------------
*/

#ifndef __DATE_H__
#define __DATE_H__

#include <cstring.h>
#include <binaryresource.h>

	enum eMonth
	{
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};

	enum eDay
	{
		SUNDAY = 1,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

class CJulianDate
{
public:

// Constructors
	CJulianDate()
		: m_Month(JANUARY)
		, m_Day(0)
		, m_Year(0)
	{
	};

    void					AddMonths(const int nMonths)					{ m_Month += nMonths; }
    void					SubYears(const int nYears)						{ m_Year -= nYears; }

	int						GetMonth() const								{ return m_Month; }
	int						GetDay() const									{ return m_Day; }
	int						GetYear() const									{ return m_Year; }
	void					SetMonth(const eMonth nMonth)					{ m_Month = (int)nMonth; }
	void					SetDay(const int nDay)							{ m_Day = nDay; }
	void					SetYear(const int nYear)						{ m_Year = nYear; }

protected:
	uint					m_Month:4;
	uint					m_Day:5;
	uint					m_Year:23;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CDate
{
public:

// Constructors
    CDate();
    CDate(const uint _time);
    CDate(const CDate& _timeSrc);
	CDate(const uint _Month, const uint _Day, const uint _Year);

// Attributes
	uint					GetDate() const;
	uint					GetShiftedDate() const;

	void					SetDate(const uint x);
	void					SetShiftedDate(const uint x);

    int						GetYear() const;
    eMonth					GetMonth() const;				// month of year(1 = Jan)
    int						GetDay() const;					// day of month
    eDay					GetDayOfWeek() const;		// 1=Sun, 2=Mon, ..., 7=Sat
#ifdef ORIGINAL_CODE
#ifdef WINDOWS
	SYSTEMTIME				GetSystemTime();
#endif
#endif
	void					AddMonths(const uint _Months);
	void					AddAMonth();
	void					AddDays(const uint _Days);
	void					AddADay();
	void					AddAYear();
	void					SubADay();
	void					SubAWeek();
	void					SubAMonth();
	void					SubAYear();

// Display strings
	CString					DayMonthStr();
	CString					MonthYearStr();
	CString					ShortDateStr();
	CString					MediumDateStr();
	CString					ShortNumDateStr();
	CString					LongDateStr();
	CString					MonthStr();

// Operations
#ifdef ORIGINAL_CODE
#ifdef WINDOWS
	void					ConvertFromJulian(SYSTEMTIME& _theDate) const;
#endif
#endif
	void					ConvertToJulian(const uint _Month, const uint _Day, const uint _Year);
	void					ConvertFromJulian(int& Month,int& Day,int& Year) const;
	void					ConvertFromJulian() const;
	int						DoCalculateDifferenceInYears(CDate& _rCurrentDate);
	void					DoGenerateDateInPast(CDate& _rCurrentDate, const ushort _Years);
	void					ReadFromResource(CBinaryReader& _Resource);



private:
	// This is January 1, 1900, in Julian form.
	static const int EPOCH_SHIFT = 2415021;

public:
	static const unsigned int MIN_DATE = EPOCH_SHIFT;
	static const unsigned int MAX_DATE = EPOCH_SHIFT + 0xFFFF - 2;
private:
    ushort					m_JulianDate;
	//uint m_JulianDate;
};
#endif
