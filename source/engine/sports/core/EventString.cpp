

/*

	Program:- Eventing.cpp

	The CEventString class routines


	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"



/*------------------------------------------------------------------------------
Method:   	CEventString::GetStr
Access:    	public 
Returns:   	CExtString
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CExtString CEventString::GetStr()
{
	return m_String;
}


/*------------------------------------------------------------------------------
Method:   	CEventString::Empty
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CEventString::Empty()
{
	m_String.Empty();
}


/*------------------------------------------------------------------------------
Method:   	CEventString::Add
Access:    	public 
Parameter: 	int When
Parameter: 	CString Name
Returns:   	void
Qualifier: 	
Purpose:	Add the time of the event and who it relates to to the event string

Event string format : - " (nn) xx, (nn, nn) xx"
------------------------------------------------------------------------------*/
void CEventString::Add(int _When, const CString &_Name)
{
	ushort Duplicate;
	CString Str;
	if (m_String.IsEmpty() != TRUE)
	{
		// Is the player already in the string?
		Duplicate = m_String.Find(_Name);
		// Yes
		if (Duplicate > 0)
		{
			Str.Format(TEXT("%s, %d) %s"), m_String.Left(Duplicate - 2), _When, m_String.Right(m_String.GetLength() - Duplicate));
			m_String =  Str;
		}
		// No
		else
		{
			// Add time and player to string, with a coma after the last one
			Str.Format(TEXT("(%d) %s"), _When, _Name);
			m_String = m_String + TEXT(", ") + Str;
		}
	}
}
