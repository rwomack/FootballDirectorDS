
/*

	ListStatic.cpp : main header file for the ListStatic gadget


	(c) Rw Software 2003-2007

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "ExtStatic.h"
#include "ListStatic.h"


/*------------------------------------------------------------------------------
	Method:   	CListStatic::CListStatic
	Access:    	public 
	Returns:   	
	Purpose:
------------------------------------------------------------------------------*/
CListStatic::CListStatic() 
	: m_nCurSel(wNOTFOUND)
	, CExtStatic()	
{
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::GetStringList
	Access:    	public 
	Returns:   	CStringArray&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStringArray& CListStatic::GetStringList()
{
	return m_strList;
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::DoIncrement
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CListStatic::DoIncrement()
{
	if (m_nCurSel < m_strList.GetUpperBound())
	{
		m_nCurSel++;
	}
	else if (m_nCurSel == wNOTFOUND)
	{
		m_nCurSel = 0;
	}
	CStatic::SetWindowText(m_strList[m_nCurSel]);
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::DoDecrement
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CListStatic::DoDecrement()
{
	if (m_nCurSel > 0 && m_nCurSel != wNOTFOUND)
	{
		m_nCurSel--;
	}
	else if (m_nCurSel == wNOTFOUND)
	{
		m_nCurSel = 0;
	}
	CStatic::SetWindowText(m_strList[m_nCurSel]);
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::SetCurSel
	Access:    	public 
	Parameter: 	const ushort _nCurSel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CListStatic::SetCurSel(const ushort _nCurSel)
{
	ASSERT(_nCurSel < m_strList.GetSize());
	m_nCurSel = _nCurSel;
	CStatic::SetWindowText(m_strList[m_nCurSel]);
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::GetCurSel
	Access:    	public const 
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
const ushort CListStatic::GetCurSel() const
{
	return m_nCurSel;
}


/*------------------------------------------------------------------------------
	Method:   	CListStatic::AddString
	Access:    	public 
	Parameter: 	CString _strText
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CListStatic::AddString(CString _strText)
{
	m_strList.Add(_strText);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CContractDateListStatic::DoAddDates
	Access:    	public 
	Parameter: 	const ushort _SeasonsToAddFirst
	Parameter: 	const ushort _nHowMany
	Parameter: 	const ushort _nCursel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractDateListStatic::DoAddDates(const ushort _SeasonsToAddFirst, const ushort _nHowMany, const ushort _nCursel)
{
	ASSERT(_nCursel <= _nHowMany);
	m_nNumberSeasonsFirst = _SeasonsToAddFirst;
    CContractEndTime TheDate(WorldData().GetCurrentDate());
	TheDate.DoEndOfSeason();
	uint LoopCount;
	for (LoopCount = 0; LoopCount < _SeasonsToAddFirst; LoopCount++)
	{
		TheDate.AddAYear();
	}
	for (LoopCount = 0; LoopCount < _nHowMany; LoopCount++)
	{
		CListStatic::AddString(TheDate.ShortDateStr());
		TheDate.AddAYear();
	}
	CListStatic::SetCurSel(_nCursel);
}


/*------------------------------------------------------------------------------
	Method:   	CContractDateListStatic::GetNumberOfSeasons
	Access:    	public const 
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
const ushort CContractDateListStatic::GetNumberOfSeasons() const
{
	return m_nNumberSeasonsFirst + CListStatic::GetCurSel();
}
