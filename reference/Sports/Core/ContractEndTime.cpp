
/*

	Program:- ContractEndTime.cpp

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::CContractEndTime
	Access:    	public 
	Parameter: 	CDate & SomeTime
	Returns:  
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndTime::CContractEndTime(CDate& SomeTime)
{
	CDate::m_JulianDate = SomeTime.GetDate();
	DoEndOfSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::CContractEndTime
	Access:    	public 
	Parameter: 	CContractEndTime & SomeTime
	Returns: 
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndTime::CContractEndTime(CContractEndTime& SomeTime)
{
	CDate::m_JulianDate = SomeTime.CDate::GetDate();
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::CContractEndTime
	Access:    	public 
	Parameter: 	const uint _nSomeTime
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndTime::CContractEndTime(const uint _nSomeTime)
{
	CDate::m_JulianDate = _nSomeTime;
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::DoEndOfSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractEndTime::DoEndOfSeason()
{
	if (CDate::GetMonth() != JUNE)
	{
		while(CDate::GetMonth() != JUNE)
		{
			CDate::AddAMonth();
		}
	}
	if (CDate::GetDay() != 30)
	{
		if (CDate::GetDay() < 30)
		{
			CDate::AddDays(30 - CDate::GetDay());
		}
		while(CDate::GetDay() != 30)
		{
			CDate::AddADay();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::HasExpired
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContractEndTime::HasExpired()
{
	if (SeasonsRemaining() < 1)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::WeeksRemaining
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CContractEndTime::WeeksRemaining()
{
	int iRet = 0;
	CDate TempTime(CDate::m_JulianDate);
	while(TempTime.GetDate() > WorldData().GetCurrentDate().GetDate())
	{
		TempTime.SubAWeek();
		iRet++;
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::SeasonsRemaining
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CContractEndTime::SeasonsRemaining()
{
	ushort iRet = 0;
	CDate TempTime(CDate::m_JulianDate);
	while(TempTime.GetDate() > WorldData().GetCurrentDate().GetDate())
	{
		TempTime.SubAYear();
		iRet++;
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndTime::IncreaseLength
	Access:    	public 
	Parameter: 	const ushort _Years
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractEndTime::IncreaseLength( const ushort _Years )
{
	if (_Years > 0)
	{
		CDate::AddMonths(12 * _Years);
	}
	else
	{
		CDate::m_JulianDate = WorldData().GetCurrentDate().GetDate();
	}
}
