
/*

	FinancialSelector.cpp: implementation for the FinancialSelector class.


	(c) Sports Director Ltd 2007

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "RoundMoney.h"


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::CFinancialSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFinancialSelector::CFinancialSelector(void)
	: m_ButtonDownKey(VK_LEFT)
	, m_ButtonUpKey(VK_RIGHT)
{
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::~CFinancialSelectorRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFinancialSelector::~CFinancialSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::DoInitialise
	Access:    	public 
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownGfxID
	Parameter: 	int _ButtonUpGfxID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	ASSERT_POINTER(_Wnd, CWnd);
	CTextRegion::SetParentWnd(_Wnd);
	ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	CTextRegion::SetBasicDisplay(_BasicDisplay);
	m_ButtonDownID = _BasicDisplay->DoAddButton(_ButtonDownGfxID, _Wnd);
	m_ButtonUpID = _BasicDisplay->DoAddButton(_ButtonUpGfxID, _Wnd);
	_BasicDisplay->DoAddTextRegion(this);
	CTextRegion::SetStyle(COL_ALIGNC | COL_MONEY);
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::GetCurValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CFinancialSelector::GetCurValue()
{
	return m_CurValue;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelector::GetCurValueStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFinancialSelector::GetCurValueStr()
{
	CString Temp;
	Temp.Format(TEXT("%u"), m_CurValue);
	Temp.MakeReverse();
	uint Total = 0;
	CString Newstr;
	uint Lenth = Temp.GetLength();
	for (uint Count = 0; Count < Lenth; Count ++)
	{
		if (Total == 3)
		{
			Total = 1;
			Newstr += TEXT(",");
		}
		else
		{
			Total++;
		}
		Newstr += Temp.GetAt(Count);
	}
	Newstr.MakeReverse();
	CString szNumBuff;
	szNumBuff.Format(TEXT("£%s"), Newstr);
	return szNumBuff;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::SetCurValue
	Access:    	public 
	Parameter: 	int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::SetCurValue(int _Value)
{
	m_CurValue = _Value;
	CTextRegion::SetText(GetCurValueStr());
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::MoveWindow(CRect _Rect)
{
	CTextRegion::MoveWindow(_Rect);
	// List buttons
	int ButtonTop = _Rect.top - 2;
	CTextRegion::GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(_Rect.left - eUPDOWNWIDTH - 2, ButtonTop, _Rect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	CTextRegion::GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(_Rect.right + 2, ButtonTop, _Rect.right + 2 + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::MoveWindow
	Access:    	public 
	Parameter: 	int _XPos
	Parameter: 	int _YPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::MoveWindow(int _XPos, int _YPos)
{
	CRect TheRect(_XPos, _YPos, _XPos + 85, _YPos + 20);
	CTextRegion::MoveWindow(TheRect);
	// List buttons
	int ButtonTop = TheRect.top - 2;
	CTextRegion::GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(TheRect.left - eUPDOWNWIDTH - 2, ButtonTop, TheRect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	CTextRegion::GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(TheRect.right + 2, ButtonTop, TheRect.right + 2 + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::OnDecValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::OnDecValue()
{
	if (m_CurValue > 0)
	{
		SetCurValue(CRoundMoney::RoundMoneyDecrement((uint)m_CurValue));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::OnIncValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::OnIncValue()
{
	SetCurValue(CRoundMoney::RoundMoneyIncrement((uint)m_CurValue));
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::OnLButtonUp
	Access:    	public 
	Parameter: 	int _ButtonPressed
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFinancialSelector::OnLButtonUp(int _ButtonPressed)
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
	Method:   	CFinancialSelectorRegion::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == m_ButtonDownKey)
	{
		OnDecValue();
		CTextRegion::GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATEDOWN);
	}
	else if (nChar == m_ButtonUpKey)
	{
		OnIncValue();
		CTextRegion::GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATEDOWN);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialSelectorRegion::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == m_ButtonDownKey)
	{
		CTextRegion::GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATENORMAL);
	}
	else if (nChar == m_ButtonUpKey)
	{
		CTextRegion::GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATENORMAL);
	}
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CFinancialTitledSelector::CFinancialTitledSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFinancialTitledSelector::CFinancialTitledSelector(void)
	: CFinancialSelector()
{
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialTitledSelector::~CFinancialTitledSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFinancialTitledSelector::~CFinancialTitledSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialTitledSelector::DoInitialise
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
void CFinancialTitledSelector::DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	m_Ypos = _YPos;
	m_TitleTextRegion = new CTextRegion;
	m_TitleTextRegion->DoInitialise(_BasicDisplay, _TitleStrID, COL_ALIGNR);
	_BasicDisplay->DoAddTextRegion(m_TitleTextRegion);
	CFinancialSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownGfxID, _ButtonUpGfxID);
}


void CFinancialTitledSelector::MoveWindow()
{
	CRect theWindow;
	GetParentWnd()->GetClientRect(&theWindow);

	int XPos = theWindow.right / 2 - eUPDOWNWIDTH;
	m_TitleTextRegion->MoveWindow(CRect(0, m_Ypos, XPos - 6, m_Ypos + 16));
	CFinancialSelector::MoveWindow(CRect(XPos + eUPDOWNWIDTH, m_Ypos, XPos + eUPDOWNWIDTH + 100, m_Ypos + 16));
}
