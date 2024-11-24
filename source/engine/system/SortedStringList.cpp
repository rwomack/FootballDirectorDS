
/*

	SortedStringList.cpp: implementation for the SortedStringList class.


	(c) Rw Software 2007

---------------------------------------------------------------
*/

#include "stdafx.h"

#include "SortedStringList.h"



/*------------------------------------------------------------------------------
	Method:   	CIndexedString::CIndexedString
	Access:    	public
	Parameter: 	CString strString
	Parameter: 	ushort nID
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CIndexedString::CIndexedString(const CString &strString, const ushort nID)
{
	m_strString = strString;
	m_nID = nID;
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedString::GetStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const CString &CIndexedString::GetStr() const
{
	return m_strString;
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedString::GetID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CIndexedString::GetID() const
{
	return m_nID;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::CSortedStringList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSortedStringList::CSortedStringList()
{
	m_nCount = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::~CSortedStringList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSortedStringList::~CSortedStringList()
{
	RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::GetSortedID
	Access:    	public
	Parameter: 	ushort nID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CSortedStringList::GetSortedID(ushort nID) const
{
	return m_arrIndString.GetAt(nID)->GetID();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::GetSize
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSortedStringList::GetSize() const
{
	return m_arrIndString.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::GetSortedStr
	Access:    	public
	Parameter: 	ushort nID
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const CString &CSortedStringList::GetSortedStr(ushort nID) const
{
	return m_arrIndString.GetAt(nID)->GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::Add
	Access:    	public
	Parameter: 	CString strString
	Parameter: 	ushort nID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSortedStringList::Add(const CString &strString, ushort nID)
{
	m_arrIndString.Add(FD_ENGINE_NEW CIndexedString(strString, nID));
	m_nCount++;
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::SortAscending
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSortedStringList::SortAscending()
{
	bool swap = true;
	while(swap == true)
	{
		swap = false;
		for (int y = 0; y < m_nCount - 1; y++)
		{
			if (m_arrIndString.GetAt(y)->GetStr() > m_arrIndString.GetAt(y + 1)->GetStr())
			{
				m_arrIndString.DoSwap(y, y + 1);
				swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSortedStringList::RemoveAll
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSortedStringList::RemoveAll()
{
  m_arrIndString.RemoveAll();
	m_nCount = 0;
}

