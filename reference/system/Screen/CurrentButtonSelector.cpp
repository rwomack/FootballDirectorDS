
/*

	CurrentButtonSelector.cpp: implementation for the CCurrentButtonSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"



/*------------------------------------------------------------------------------
	Method:   	CCurrentButtonSelector::CCurrentButtonSelector
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCurrentButtonSelector::CCurrentButtonSelector()
	: m_CurSel(0)
	, m_HowMany(0)
	, m_PrevButtonKey(VK_UP)
	, m_NextButtonKey(VK_DOWN)
	, m_BasicDisplay(NULL)
{
	for (int LoopCount = 0; LoopCount < MAX_BUTTONS; LoopCount++)
	{
		m_List[LoopCount] = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentButtonSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCurrentButtonSelector::GetCurSel()
{
	return m_CurSel;
}
	

/*------------------------------------------------------------------------------
	Method:   	CCurrentButtonSelector::OnInitialise
	Access:    	public 
	Parameter: 	uint * _ResourceID
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	const int _HowMany
	Parameter: 	const UINT _PrevButtonKey
	Parameter: 	const UINT _NextButtonKey
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentButtonSelector::OnInitialise(uint* _ResourceID, CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, const int _HowMany, const UINT _PrevButtonKey /*= VK_UP*/, const UINT _NextButtonKey /*= VK_DOWN*/)
{
	EXT_ASSERT(_HowMany > 0);
	m_HowMany = _HowMany;
	m_PrevButtonKey = _PrevButtonKey;
	m_NextButtonKey = _NextButtonKey;
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	EXT_ASSERT_POINTER(_ParentWnd, CWnd);
	EXT_ASSERT_POINTER(_ResourceID, uint);
	for (int LoopCount = 0; LoopCount < m_HowMany; LoopCount++)
	{
		m_List[LoopCount] = m_BasicDisplay->DoAddButton(*_ResourceID, _ParentWnd);
		_ResourceID++;
	}
	m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
}


void CCurrentButtonSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentButtonSelector::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentButtonSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_PrevButtonKey == nChar)
	{
		m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
		do 
		{
			if (m_CurSel > 0)
			{
				m_CurSel--;
			}
			else
			{
				m_CurSel = m_HowMany - 1;
			}
		} while (m_BasicDisplay->GetButton(m_List[m_CurSel])->GetStatus() == BUTTONSTATEHIDDEN);
		m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
	}
	if (m_NextButtonKey == nChar)
	{
		m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
		do
		{
			if (m_CurSel < m_HowMany - 1)
			{
				m_CurSel++;
			}
			else
			{
				m_CurSel = 0;
			}
		} while (m_BasicDisplay->GetButton(m_List[m_CurSel])->GetStatus() == BUTTONSTATEHIDDEN);
		m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentButtonSelector::SetCurSel
	Access:    	public 
	Parameter: 	int _CurSel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentButtonSelector::SetCurSel(int _CurSel)
{
	EXT_ASSERT(_CurSel < m_HowMany);
	m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();  // Remove old highlight
	m_CurSel = _CurSel;
	m_BasicDisplay->GetButton(m_List[m_CurSel])->ToggleHighlight();
}
