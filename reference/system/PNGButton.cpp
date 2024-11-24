
/*

	PNGButton.cpp - Graphic button

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "FootballDirector.h"

/*------------------------------------------------------------------------------
	Method:   	CPNGButton::CPNGButton
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	: m_Status(BUTTONSTATENORMAL)
	Purpose:
------------------------------------------------------------------------------*/
CPNGButton::CPNGButton(CWnd* pParent /*null*/)
	: m_Status(BUTTONSTATENORMAL)
	, m_Highlight(false)
{
	m_pParentWnd = pParent;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::DoDrawHighlight
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::DoDrawHighlight(COffScreenBuffer* _dc)
{
	EXT_ASSERT(GetStatus() != BUTTONSTATEHIDDEN);
	CRect theRect(m_theRect);
	theRect.top -= 2;
	theRect.bottom ++;
	theRect.left -= 2;
	theRect.right ++;
	_dc->DrawBox(theRect, RED, 2);
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::DoSetDrawInfo
	Access:    	public 
	Parameter: 	const int nNormalID
	Parameter: 	CWnd * pParent
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::DoSetDrawInfo(const int nNormalID, CWnd* pParent, const uint _NumImages /*= 2*/)
{
	EXT_ASSERT_POINTER(pParent, CWnd);
	m_pParentWnd = pParent;
	m_pngButton.DoInitialise(::GetDC(pParent->GetSafeHwnd()), nNormalID, _NumImages);
	m_Status = BUTTONSTATENORMAL;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::GetStatus
	Access:    	public 
	Returns:   	ePNGButtonStatus
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ePNGButtonStatus CPNGButton::GetStatus() const
{
	return m_Status;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::GetWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPNGButton::GetWidth()
{
	return m_pngButton.GetWidth();
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::IsHighLight
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPNGButton::IsHighLight()
{
	return m_Highlight;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::IsPointInRect
	Access:    	virtual public 
	Parameter: 	CPoint hitPoint
	Parameter: 	const ePNGButtonStatus nStatusIfTrue
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPNGButton::IsPointInRect(CPoint hitPoint, const ePNGButtonStatus nStatusIfTrue)
{
	if (GetStatus() != BUTTONSTATEHIDDEN)
	{
		// If button up then force up
		if (nStatusIfTrue == BUTTONSTATERELEASED)
		{
			SetStatus(BUTTONSTATENORMAL);			// Button up
#ifdef FMOD
			theApp.GetSoundManager().DoPlaySoundFX(SOUND_BEEP);
#endif
		}
// 		DEBUG_OUTPUT(("Button Left %d, Right, %d Top %d, Bottom %d"), (m_theRect.left, m_theRect.right, m_theRect.top, m_theRect.bottom));
// 		DEBUG_OUTPUT(("Point X %d, Point Y %d"), (hitPoint.x, hitPoint.y));
		if (m_theRect.PtInRect(hitPoint) == TRUE)
		{
			SetStatus(nStatusIfTrue);
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::OnPaint
	Access:    	public 
	Parameter: 	CDC * _dc
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::OnPaint(COffScreenBuffer* _dc)
{
	EXT_ASSERT_POINTER(_dc, COffScreenBuffer);
	switch (GetStatus())
	{
		case BUTTONSTATECURRENT:
			m_pngButton.DoDraw(_dc->operator HDC(), m_theRect.left, m_theRect.top, 2);
			break;
		case BUTTONSTATEDOWN:
			m_pngButton.DoDraw(_dc->operator HDC(), m_theRect.left, m_theRect.top, 1);
			break;
		case BUTTONSTATENORMAL:
		case BUTTONSTATERELEASED:
			m_pngButton.DoDraw(_dc->operator HDC(), m_theRect.left, m_theRect.top, 0);
			break;
	}
	if (m_Highlight == true)
	{
		DoDrawHighlight(_dc);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::MoveWindow
	Access:    	public 
	Parameter: 	CRect & _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::MoveWindow(CRect& _Rect)
{
	EXT_ASSERT(_Rect.left < _Rect.right);
	EXT_ASSERT(_Rect.top < _Rect.bottom);
	m_theRect = _Rect;
	m_theRect.right = m_theRect.left + m_pngButton.GetWidth();
	m_theRect.bottom = m_theRect.top + m_pngButton.GetHeight();
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::MoveWindow
	Access:    	public 
	Parameter: 	const uint32 _XPos
	Parameter: 	const uint32 _YPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::MoveWindow(const uint _XPos, const uint _YPos)
{
	m_theRect.left = _XPos;
	m_theRect.top = _YPos;
	m_theRect.right = m_theRect.left + m_pngButton.GetWidth();
	m_theRect.bottom = m_theRect.top + m_pngButton.GetHeight();
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::SetStatus
	Access:    	public 
	Parameter: 	const ePNGButtonStatus status
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::SetStatus(const ePNGButtonStatus status)
{
	m_Status = status;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGButton::ToggleHighlight
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGButton::ToggleHighlight()
{
	if (m_Highlight == true)
	{
		m_Highlight = false;
	}
	else
	{
		m_Highlight = true;
	}
}