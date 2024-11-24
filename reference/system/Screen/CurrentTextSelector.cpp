
/*

	CurrentTextSelector.cpp: implementation for the CCurrentTextSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"



/*------------------------------------------------------------------------------
	Method:   	CCurrentTextSelector::CCurrentTextSelector
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCurrentTitledTextSelector::CCurrentTitledTextSelector()
	: m_CurSel(0)
	, m_List(NULL)
	, m_HowMany(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTitledTextSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCurrentTitledTextSelector::GetCurSel()
{
	return m_CurSel;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTitledTextSelector::GetListSize
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCurrentTitledTextSelector::GetListSize()
{
	EXT_ASSERT_POINTER(m_List, CTitledTextSelector);
	return m_List->GetCount();
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTextSelector::OnInitialise
	Access:    	public 
	Parameter: 	CTextSelector * _List
	Parameter: 	const int _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentTitledTextSelector::OnInitialise(CTitledTextSelector* _List, const int _HowMany)
{
	EXT_ASSERT_POINTER(_List, CTextSelector);
	m_List = _List;
	EXT_ASSERT(_HowMany > 0);
	m_HowMany = _HowMany;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTextSelector::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentTitledTextSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	EXT_ASSERT_POINTER(m_List, CTitledTextSelector);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_List[m_CurSel].OnKeyDown(nChar, nRepCnt, nFlags);
			break;

		case VK_UP:
			if (m_CurSel > 0)
			{
				m_CurSel--;
			}
			else
			{
				m_CurSel = m_HowMany - 1;
			}
			break;

		case VK_DOWN:
			if (m_CurSel < m_HowMany - 1)
			{
				m_CurSel++;
			}
			else
			{
				m_CurSel = 0;
			}
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTextSelector::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentTitledTextSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	EXT_ASSERT_POINTER(m_List, CTextSelector);
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			m_List[m_CurSel].OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentTextSelector::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentTitledTextSelector::OnPaint()
{
	EXT_ASSERT_POINTER(m_List, CTextSelector);
	m_List[m_CurSel].DoDrawHighlight();
}