
/*

	ExtCheckBox.cpp: implementation for the ExtCheckBox class.


	(c) Sports Director 2008-

  ---------------------------------------------------------------
*/

#include "stdafx.h"

#include "UI.h"





/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::CExtCheckBox
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtCheckBox::CExtCheckBox()
{
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::~CExtCheckBox
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtCheckBox::~CExtCheckBox()
{
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::DoToggle
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtCheckBox::DoToggle()
{
	if (m_CurrentState == true)
	{
		m_CurrentState = false;
	}
	else
	{
		m_CurrentState = true;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::OnInitialUpdate
	Access:    	protected 
	Parameter: 	CBasicDisplay * _Display
	Parameter: 	const bool _State
	Parameter: 	HWND _hWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtCheckBox::OnInitialUpdate(CBasicDisplay* _Display, const bool _State, HWND _hWnd)
{
	EXT_ASSERT_POINTER(_Display, CBasicDisplay);
	m_BasicDisplay = _Display;
	HDC dc = ::GetDC(_hWnd);
	m_pngCheckBox.DoInitialise(dc, IDR_PNGCHECKBOX, 2);
	::ReleaseDC(_hWnd, dc);
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtCheckBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (point.x >= m_Location.x && point.x < m_Location.x + 18 &&
		point.y >= m_Location.y && point.y < m_Location.y + 18)
	{
		DoToggle();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtCheckBox::OnPaint()
{
	m_pngCheckBox.DoDrawTransparent(m_BasicDisplay->GetOffScreenBuffer()->operator HDC(), m_Location.x, m_Location.y, (int)m_CurrentState);
}


/*------------------------------------------------------------------------------
	Method:   	CExtCheckBox::OnSize
	Access:    	public 
	Parameter: 	const CPoint _Location
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtCheckBox::OnSize(const CPoint _Location)
{
	m_Location = _Location;
}