
/*

	CurrentPercentSelector.cpp: implementation for the CCurrentPercentSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"


/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::CCurrentPercentSelector
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCurrentPercentSelector::CCurrentPercentSelector()
	: m_CurSel(0)
	, m_List(NULL)
	, m_HowMany(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCurrentPercentSelector::GetCurSel() const
{
	return m_CurSel;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::GetListSize
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCurrentPercentSelector::GetListSize() const
{
	return m_ListSize;
}
	

/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::OnInitialise
	Access:    	public 
	Parameter: 	CTextSelector * _List
	Parameter: 	const int _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentPercentSelector::OnInitialise(CPercentageTitledSelector* _List, const int _HowMany, const int _ListSize)
{
	EXT_ASSERT_POINTER(_List, CPercentageTitledSelector);
	m_List = _List;
	EXT_ASSERT(_HowMany > 0);
	m_HowMany = _HowMany;
	m_ListSize = _ListSize;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentPercentSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			if (m_CurSel < m_ListSize)
			{
				m_List[m_CurSel].OnKeyDown(nChar, nRepCnt, nFlags);
			}
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
	Method:   	CCurrentPercentSelector::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentPercentSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	EXT_ASSERT_POINTER(m_List, CTextSelector);
	switch(nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			if (m_CurSel < m_ListSize)
			{
				m_List[m_CurSel].OnKeyUp(nChar, nRepCnt, nFlags);
			}
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentPercentSelector::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentPercentSelector::OnPaint()
{
	EXT_ASSERT_POINTER(m_List, CPercentageTitledSelector);
	if (m_CurSel < m_ListSize)
	{
		m_List[m_CurSel].DoDrawHighlight();
	}
}