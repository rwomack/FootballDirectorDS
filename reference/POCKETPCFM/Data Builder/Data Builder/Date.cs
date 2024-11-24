using System;
using System.Collections.Generic;
using System.Text;

namespace Data_Builder
{
	class Date
	{

		public enum eMonth
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

		public enum eDay
		{
			SUNDAY = 1,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY
		};

		

		int m_JulianDate;
		int m_nMonth;
		int m_nDay;
		int m_nYear;


		//--------------------------------------------------------------
		public int getJulianDate()
		{
			return m_JulianDate;
		}


		//--------------------------------------------------------------
		public Date(DateTime _Date)
		{
			ConvertToJulian(_Date);
		}


		//--------------------------------------------------------------
		void ConvertToJulian(DateTime _Date)
		{
			m_nMonth = _Date.Month;
			m_nDay = _Date.Day;
			m_nYear = _Date.Year;
			if ((Date.eMonth)m_nMonth < Date.eMonth.MARCH)
			{
				AddMonths(12);
				SubYears(1);
			}
			m_JulianDate = m_nDay + (153 * m_nMonth - 457) / 5 + 365 * m_nYear + (m_nYear / 4) - (m_nYear / 100) + (m_nYear / 400) + 1721119;
		}


		//--------------------------------------------------------------
		public void AddMonths(int _Months)
		{
			m_nMonth += _Months;
		}


		//--------------------------------------------------------------
		public void SubYears(int _Years)
		{
			m_nYear -= _Years;
		}

	}
}
