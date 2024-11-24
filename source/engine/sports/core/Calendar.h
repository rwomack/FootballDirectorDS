
/*

	Calendar.h: interface for the CCalendar class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __CALENDAR_H__
#define __CALENDAR_H__


class CCalendar : public CDate
{
    // Variables
private:
    uint					m_Week:16;
    uint					m_Season:8;
    uint					m_MatchDay:8;

public:
    CCalendar();
    CCalendar(ushort Week, const byte Match, ushort Season, CDate Date);
    CCalendar(const CCalendar& SomeTime);
    CCalendar(const int nYear, const int nMonth, const int nDay);

	CCalendar&				operator ++(); 		// Prefix version
	CCalendar&				operator ++(int); 		// Postfix version
	CCalendar&				operator --(); 		// Prefix version
	CCalendar&				operator --(int);		// Postfix version
    CCalendar&				operator =(const CCalendar& SomeTime);
	bool					operator <(const CCalendar &Time) const;
	bool					operator >(const CCalendar &Time) const;
	bool					operator ==(const CCalendar &Time) const;
	bool					operator <=(const CCalendar &Time) const;
	bool					operator >=(const CCalendar &Time) const;
	bool					operator !=(const CCalendar &Time) const;

	// Accessors
    ushort					GetWeek() const;							
    ushort					GetSeason() const;							
	byte					GetMatchDay() const;							

    // Methods
	void					DoInitialise(ushort Week, const byte Match, ushort Season, int nYear, int nMonth, int nDay);
	void					DoInitialise(const CCalendar& SomeTime);
	void					DoSetToMatchDay(const byte _MatchDay);
    bool					FirstDateOfMonth();
	CString					GetSeasonStr(const bool _OneYearSeason);
    bool					IsNull();
	bool					IsWeekEnd();
	CString					LastMonthStr();
	bool					NextMatchDay();
	bool					PreviousMatchDay();
	void					SetMatchDay(const byte _MatchDay, bool _Backwards = false);
	void					SetMatchDayDirect(const byte _Value);
};
#endif
