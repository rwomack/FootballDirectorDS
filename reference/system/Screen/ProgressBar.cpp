
/*

	ProgressBar.cpp : implementation file

	(c) Sports Director Ltd 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "ProgressBar.h"


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::CProgressBar
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CProgressBar::CProgressBar()
{
	m_PNG = NULL;
	DoReset(0);
}


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::DoReset
	Access:    	public 
	Parameter: 	const byte _Numsteps
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CProgressBar::DoReset(const byte _Numsteps)
{
	m_CurrentStep = 0;
	m_NumSteps = _Numsteps;
}


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::DoStepIt
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CProgressBar::DoStepIt()
{
	EXT_ASSERT(m_CurrentStep < m_NumSteps);
	m_CurrentStep++;
}


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::MoveWindow
	Access:    	public 
	Parameter: 	const CRect & _TheRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CProgressBar::MoveWindow(const CRect& _TheRect)
{
	m_WindowRect = _TheRect;
}


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::OnPaint
	Access:    	public 
	Parameter: 	HDC _HDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CProgressBar::OnPaint(HDC _HDC)
{
	EXT_ASSERT_POINTER(m_PNG, CVOImage);
	int BarLength = (m_WindowRect.right - m_WindowRect.left) - (PROGRESSBARWIDTH * 2) - 2;
	int InnerBarLength = 0;
	if (m_CurrentStep > 1)
	{
		InnerBarLength = (BarLength / (m_NumSteps - 2)) * (m_CurrentStep - 1);
	}
	// Draw outer ends
	m_PNG->DoDrawTransparent(_HDC, m_WindowRect.left, m_WindowRect.top, PROGRESSBARWIDTH);
	m_PNG->DoDrawTransparent(_HDC, m_WindowRect.right - PROGRESSBARWIDTH - 4, m_WindowRect.top, PROGRESSBARWIDTH, 0, PROGRESSBARWIDTH + 3);
	// Draw bars
	for (int LoopCount = 0; LoopCount < BarLength; LoopCount++)
	{
		m_PNG->DoDrawTransparent(_HDC, m_WindowRect.left + PROGRESSBARWIDTH + LoopCount, m_WindowRect.top, 1, 0, PROGRESSBARWIDTH + 1, 0);
		if (LoopCount < InnerBarLength)
		{
			m_PNG->DoDrawTransparent(_HDC, m_WindowRect.left + PROGRESSBARWIDTH + LoopCount, m_WindowRect.top, 1, 0, PROGRESSBARWIDTH * 2 + 4 + (PROGRESSBARWIDTH - 2), 0);
		}
	}
	// Draw inner ends
	if (m_CurrentStep > 0)
	{
		m_PNG->DoDrawTransparent(_HDC, m_WindowRect.left + 3, m_WindowRect.top, PROGRESSBARWIDTH - 2, 0, PROGRESSBARWIDTH * 2 + 4);
	}
	if (m_CurrentStep >= m_NumSteps - 1)
	{
		m_PNG->DoDrawTransparent(_HDC, m_WindowRect.right - PROGRESSBARWIDTH - 4, m_WindowRect.top, PROGRESSBARWIDTH - 2, 0, PROGRESSBARWIDTH * 2 + 16);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CProgressBar::SetDrawInfo
	Access:    	public 
	Parameter: 	CVOImage * _PNG
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CProgressBar::SetDrawInfo(CVOImage* _PNG)
{
	EXT_ASSERT_POINTER(_PNG, CVOImage);
	m_PNG = _PNG;
}
