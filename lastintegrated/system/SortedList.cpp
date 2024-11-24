
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
CIndexedString::CIndexedString(CString strString, ushort nID)
{
	m_strString = strString;
	m_nID = nID;
}


/*------------------------------------------------------------------------------
	Method:   	CIndexedString::GetData
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CIndexedString::GetData()
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
ushort CIndexedString::GetID()
{
	return m_nID;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSortedList::CSortedList
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSortedList::CSortedList()
{
	m_nCount = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::~CSortedList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSortedList::~CSortedList() 
{
	RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::GetSortedID
	Access:    	public 
	Parameter: 	ushort nID
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CSortedList::GetSortedID(ushort nID)
{
	return m_arrIndString.GetAt(nID)->GetID();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::GetSize
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CSortedList::GetSize()
{
	return m_arrIndString.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::GetSortedStr
	Access:    	public 
	Parameter: 	ushort nID
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CSortedList::GetSortedStr(ushort nID)
{
	return m_arrIndString.GetAt(nID)->GetData();
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::Add
	Access:    	public 
	Parameter: 	CString strString
	Parameter: 	ushort nID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSortedList::Add(CString strString, ushort nID)
{
	m_arrIndString.Add(new CIndexedString(strString, nID));
	m_nCount++;
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::SortAscending
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSortedList::SortAscending()
{
	bool swap = true;
	while(swap == true)
	{
		swap = false;
		for (int y = 0; y < m_nCount - 1; y++)
		{
			if (m_arrIndString.GetAt(y)->GetData() > m_arrIndString.GetAt(y + 1)->GetData())
			{
				m_arrIndString.DoSwap(y, y + 1);
				swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSortedList::RemoveAll
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSortedList::RemoveAll()
{
	m_arrIndString.removeElementAll();
	m_nCount = 0;
}

