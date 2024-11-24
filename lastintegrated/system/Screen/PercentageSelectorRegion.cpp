
/*

	PercentageSelector.cpp: implementation for the PercentageSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "PercentageSelectorRegion.h"


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::CPercentageSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPercentageSelector::CPercentageSelector(void)
	: m_ButtonDownKey(VK_LEFT)
	, m_ButtonUpKey(VK_RIGHT)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::~CPercentageSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPercentageSelector::~CPercentageSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::DoInitialise
	Access:    	public 
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownGfxID
	Parameter: 	int _ButtonUpGfxID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	m_TextRegion = new CTextRegion(_BasicDisplay, _Wnd, COL_ALIGNC);
	_BasicDisplay->DoAddTextRegion(m_TextRegion);
	m_ButtonDownID = _BasicDisplay->DoAddButton(_ButtonDownGfxID, _Wnd);
	m_ButtonUpID = _BasicDisplay->DoAddButton(_ButtonUpGfxID, _Wnd);
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::GetCurValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPercentageSelector::GetCurValue()
{
	return m_CurValue;
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::IsUpButton
	Access:    	public 
	Parameter: 	const int _ButtonID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPercentageSelector::IsUpButton(const int _ButtonID)
{
	if (m_ButtonUpID == _ButtonID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::IsDownButton
	Access:    	public 
	Parameter: 	const int _ButtonID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPercentageSelector::IsDownButton(const int _ButtonID)
{
	if (m_ButtonDownID == _ButtonID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::SetCurValue
	Access:    	public 
	Parameter: 	int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::SetCurValue(int _Value)
{
	EXT_ASSERT(_Value < 101 && _Value >= 0);
	m_CurValue = _Value;
	OnDisplayCurValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::OnDisplayCurValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnDisplayCurValue()
{
	CRect theRect(m_TextRegion->GetWindowRect());
	m_TextRegion->SetItemPercentage(m_CurValue);
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::MoveWindow(CRect _Rect)
{
	m_TextRegion->MoveWindow(_Rect);
	// List buttons
	int ButtonTop = _Rect.top - 2;
	m_TextRegion->GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(_Rect.left - eUPDOWNWIDTH - 2, ButtonTop, _Rect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	m_TextRegion->GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(_Rect.right, ButtonTop, _Rect.right + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::MoveWindow
	Access:    	public 
	Parameter: 	int _XPos
	Parameter: 	int _YPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::MoveWindow(int _XPos, int _YPos)
{
	CRect TheRect(_XPos, _YPos, _XPos + 85, _YPos + 20);
	m_TextRegion->MoveWindow(TheRect);
	// List buttons
	int ButtonTop = TheRect.top - 2;
	m_TextRegion->GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(TheRect.left - eUPDOWNWIDTH - 2, ButtonTop, TheRect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	m_TextRegion->GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(TheRect.right + 2, ButtonTop, TheRect.right + 2 + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::OnDecValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnDecValue()
{
	if (m_CurValue > 0)
	{
		m_CurValue--;
	}
	OnDisplayCurValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::OnDecreaseValue
	Access:    	public 
	Parameter: 	const int _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnDecreaseValue(const int _Amount)
{
	m_CurValue -= _Amount;
	OnDisplayCurValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::OnIncValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnIncValue()
{
	if (m_CurValue < 100)
	{
		m_CurValue++;
	}
	OnDisplayCurValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::OnIncreaseValue
	Access:    	public 
	Parameter: 	const int _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnIncreaseValue(const int _Amount)
{
	m_CurValue += _Amount;
	OnDisplayCurValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::OnLButtonUp
	Access:    	public 
	Parameter: 	int _ButtonPressed
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPercentageSelector::OnLButtonUp(int _ButtonPressed)
{
	bool bRet = false;
	if (_ButtonPressed == m_ButtonDownID)
	{
		OnDecValue();
		bRet = true;
	}	
	if (_ButtonPressed == m_ButtonUpID)
	{
		OnIncValue();
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == m_ButtonDownKey)
	{
		OnDecValue();
		m_TextRegion->GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATEDOWN);
	}
	else if (nChar == m_ButtonUpKey)
	{
		OnIncValue();
		m_TextRegion->GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATEDOWN);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelectorRegion::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == m_ButtonDownKey)
	{
		m_TextRegion->GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATENORMAL);
	}
	else if (nChar == m_ButtonUpKey)
	{
		m_TextRegion->GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATENORMAL);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::OnPaint()
{
	CRect theRect(m_TextRegion->GetWindowRect());
	CPoint LHPoint, RHPoint;
	LHPoint.x = theRect.left - 2;
	LHPoint.y = theRect.top - 2;
	RHPoint.x = theRect.right;
	RHPoint.y = LHPoint.y;
	m_TextRegion->GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
	LHPoint.y += eUPDOWNHEIGHT - 1;
	RHPoint.y += eUPDOWNHEIGHT - 1;
	m_TextRegion->GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageSelector::DoDrawHighlight
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageSelector::DoDrawHighlight()
{
	CRect theRect(m_TextRegion->GetWindowRect());
	theRect.top -= 3;
	theRect.bottom = theRect.top + eUPDOWNHEIGHT + 2;
	theRect.left -= (eUPDOWNWIDTH + 2);
	theRect.right += eUPDOWNWIDTH + 1;
	m_TextRegion->GetBasicDisplay()->GetOffScreenBuffer()->DrawBox(theRect, RED, 2);
}

/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPercentageTitledSelector::CPercentageTitledSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPercentageTitledSelector::CPercentageTitledSelector(void)
	: CPercentageSelector()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageTitledSelector::~CPercentageTitledSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPercentageTitledSelector::~CPercentageTitledSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageTitledSelector::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	int _TitleStrID
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownGfxID
	Parameter: 	int _ButtonUpGfxID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageTitledSelector::DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	m_Ypos = _YPos;
	m_TitleTextRegion = new CTextRegion;
	_BasicDisplay->DoAddTextRegion(m_TitleTextRegion);
	m_TitleTextRegion->DoInitialise(_BasicDisplay, _TitleStrID, COL_ALIGNR);
	CPercentageSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownGfxID, _ButtonUpGfxID);
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageTitledSelector::MoveWindow
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageTitledSelector::MoveWindow()
{
	CRect theWindow;
	m_TextRegion->GetParentWnd()->GetClientRect(&theWindow);

	int XPos = theWindow.right / 2 - eUPDOWNWIDTH;
	m_TitleTextRegion->MoveWindow(CRect(0, m_Ypos, XPos - 6, m_Ypos + 16));
	CPercentageSelector::MoveWindow(CRect(XPos + eUPDOWNWIDTH, m_Ypos, XPos + eUPDOWNWIDTH + 40, m_Ypos + 16));
}
