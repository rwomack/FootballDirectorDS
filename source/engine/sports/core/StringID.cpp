
 /*

	Class:- clubname

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CStringID::operator=
	Access:    	public 
	Parameter: 	const CStringID & p
	Returns:   	CStringID&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStringID& CStringID::operator = (const CStringID& p)
{
	m_ID = p.m_ID;
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::operator==
	Access:    	public 
	Parameter: 	const CStringID & p
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CStringID::operator == (const CStringID& p)
{
	if (m_ID == p.m_ID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::GetID
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CStringID::GetID() const
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::SetID
	Access:    	public 
	Parameter: 	const ushort wID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStringID::SetID(const ushort wID)
{
	EXT_ASSERT(m_ID != NOCLUB);
	m_ID = wID;
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::GetStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString&
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CString& CStringID::GetStr() const
{
	EXT_ASSERT(m_ID != NOCLUB);
	ClubID cID(m_ID);
	return WorldData().GetString(cID);
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::DoFindID
	Access:    	public 
	Parameter: 	const CString _strName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStringID::DoFindID(const CString &_strName)
{
	EXT_ASSERT(_strName.IsEmpty() == false);
	m_ID = WorldData().DoFindStrID(_strName);
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::DoReset
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStringID::DoReset()
{
	m_ID = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CStringID::IsEmpty
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CStringID::IsEmpty()
{
	if (m_ID == NOCLUB)
	{
		return true;
	}
	return false;
}