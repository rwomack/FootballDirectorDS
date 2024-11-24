
/*

	TextSelectorRegion.cpp: implementation for the TextSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "RoundMoney.h"



/*------------------------------------------------------------------------------
	Method:   	CTextSelector::CTextSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextSelector::CTextSelector(void)
	: m_CurSel(0)
	, m_ButtonDownKey(VK_LEFT)
	, m_ButtonUpKey(VK_RIGHT)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::~CTextSelector
	Access:    	virtual public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextSelector::~CTextSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddDates
	Access:    	public 
	Parameter: 	const ushort _SeasonsToAddFirst
	Parameter: 	const ushort _nHowMany
	Parameter: 	const ushort _nCursel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddDates(const ushort _SeasonsToAddFirst /*= 1*/, const ushort _nHowMany /*= 6*/, const ushort _nCursel /*= 3*/)
{
	DoRemoveAllData();
	m_NumberSeasonsFirst = _SeasonsToAddFirst;
	CContractEndTime TheDate(WorldData().GetCurrentDate());
	TheDate.DoEndOfSeason();
	uint LoopCount;
	for (LoopCount = 0; LoopCount < _SeasonsToAddFirst; LoopCount++)
	{
		TheDate.AddAYear();
	}
	for (LoopCount = 0; LoopCount < _nHowMany; LoopCount++)
	{
		DoAddItem(TheDate.LongDateStr());
		TheDate.AddAYear();
	}
	SetCurSel(_nCursel);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddItem
	Access:    	public 
	Parameter: 	CString & _String
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddItem(CString& _String, void* _Pointer /*= NULL*/)
{
	CGridRow* pRow = new CGridRow();
	EXT_ASSERT_POINTER(pRow, CGridRow);
	m_Column.m_Rows.Add(pRow);
	pRow->DoCreate(_String);
	SetCurSel(GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddItem
	Access:    	public 
	Parameter: 	int _StringID
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddItem(int _StringID, void* _Pointer /*= NULL*/)
{
	CString str;
	str.LoadString(_StringID);
	DoAddItem(str, _Pointer);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddItemList
	Access:    	public 
	Parameter: 	CSortedStringList & _SortedTextList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddItemList(CSortedStringList& _SortedTextList)
{
	for (int LoopCount = 0; LoopCount < _SortedTextList.GetSize(); LoopCount++)
	{
		DoAddItem(_SortedTextList.GetSortedStr(LoopCount), NULL);
	}
	GetTextRegion()->SetText(m_Column.GetRow(m_CurSel)->GetText());
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddStrID
	Access:    	public 
	Parameter: 	int _StrID
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddStrID(int _StrID, void* _Pointer /*= NULL*/)
{
	CGridRow* pRow = new CGridRow();
	EXT_ASSERT_POINTER(pRow, CGridRow);
	m_Column.m_Rows.Add(pRow);
	CString str;
	str.LoadString(_StrID);
	pRow->DoCreate(str);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoAddStrIDList
	Access:    	public 
	Parameter: 	int * _IDList
	Parameter: 	int _HowMany
	Parameter: 	int _CurSel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoAddStrIDList(int* _IDList, int _HowMany, int _CurSel /*= 0*/)
{
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		DoAddStrID(*_IDList, NULL);
		_IDList++;
	}
	SetCurSel(_CurSel);
	GetTextRegion()->SetText(m_Column.GetRow(m_CurSel)->GetText());
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoDrawHighlight
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoDrawHighlight()
{
	CRect theRect(GetTextRegion()->GetWindowRect());
	theRect.top -= 3;
	theRect.bottom = theRect.top + eUPDOWNHEIGHT + 2;
	theRect.left -= (eUPDOWNWIDTH + 2);
	theRect.right += eUPDOWNWIDTH + 1;
	GetTextRegion()->GetBasicDisplay()->GetOffScreenBuffer()->DrawBox(theRect, RED, 2);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoInitialise
	Access:    	public 
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownGfxID
	Parameter: 	int _ButtonUpGfxID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID)
{
	m_TextRegion = new CTextRegion;
	GetTextRegion()->SetParentWnd(_Wnd);
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	GetTextRegion()->SetBasicDisplay(_BasicDisplay);
	m_ButtonDownID = _BasicDisplay->DoAddButton(_ButtonDownGfxID, _Wnd);
	m_ButtonUpID = _BasicDisplay->DoAddButton(_ButtonUpGfxID, _Wnd);
	_BasicDisplay->DoAddTextRegion(m_TextRegion);
	GetTextRegion()->SetStyle(COL_ALIGNC);
	SetCurSel(0);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::DoRemoveAllData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::DoRemoveAllData()
{
	m_Column.DoRemoveAllData();
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetCount
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTextSelector::GetCount()
{
	return m_Column.GetRowCount();
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetCurPointer
	Access:    	public 
	Returns:   	void*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void* CTextSelector::GetCurPointer()
{
	return m_Column.GetRow(m_CurSel)->GetPointer();
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTextSelector::GetCurSel()
{
	if (GetTextRegion()->IsDate() == true)
	{
		// Seasons
		return m_CurSel + m_NumberSeasonsFirst;
	}
	return m_CurSel;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetCurValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTextSelector::GetCurValue()
{
	return m_CurValue;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetCurValueStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CTextSelector::GetCurValueStr()
{
	CString szNumBuff;
	if (GetTextRegion()->IsMoney() == true)
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
		szNumBuff.Format(TEXT("£%s"), Newstr);
	}
	else if (GetTextRegion()->IsPercentage() == true)
	{
		szNumBuff.Format(TEXT("%d%%"), m_CurValue);
	}
	if (szNumBuff.IsEmpty() == true)
	{
		int ab = 0;
	}
	return szNumBuff;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::GetTextRegion
	Access:    	public 
	Returns:   	CTextRegion*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion* CTextSelector::GetTextRegion()
{
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	return m_TextRegion;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::IsUpButton
	Access:    	public 
	Parameter: 	const int _ButtonID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextSelector::IsUpButton(const int _ButtonID)
{
	if (m_ButtonUpID == _ButtonID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::IsDownButton
	Access:    	public 
	Parameter: 	const int _ButtonID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextSelector::IsDownButton(const int _ButtonID)
{
	if (m_ButtonDownID == _ButtonID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::MoveWindow(CRect _Rect)
{
	GetTextRegion()->MoveWindow(_Rect);
	// List buttons
	int ButtonTop = _Rect.top - 2;
	GetTextRegion()->GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(_Rect.left - eUPDOWNWIDTH - 2, ButtonTop, _Rect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	GetTextRegion()->GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(_Rect.right, ButtonTop, _Rect.right + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::MoveWindow
	Access:    	public 
	Parameter: 	int _XPos
	Parameter: 	int _YPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::MoveWindow(int _XPos, int _YPos)
{
	CRect TheRect(_XPos, _YPos, _XPos + 85, _YPos + 20);
	GetTextRegion()->MoveWindow(TheRect);
	// List buttons
	int ButtonTop = TheRect.top - 2;
	GetTextRegion()->GetBasicDisplay()->DoMoveButton(m_ButtonDownID, CRect(TheRect.left - eUPDOWNWIDTH - 2, ButtonTop, TheRect.left + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
	GetTextRegion()->GetBasicDisplay()->DoMoveButton(m_ButtonUpID, CRect(TheRect.right + 2, ButtonTop, TheRect.right + 2 + eUPDOWNWIDTH, ButtonTop + eUPDOWNHEIGHT));
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnDecCurSel
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnDecCurSel()
{
	if (GetTextRegion()->IsMoney() == true)
	{
		OnDecMoneyValue();
	}
	else if (GetTextRegion()->IsPercentage() == true)
	{
		OnDecPercentageValue();
	}
	else
	{
		if (m_CurSel > 0)
		{
			m_CurSel--;
		}
		else
		{
			m_CurSel = m_Column.GetRowCount() - 1;
		}
		GetTextRegion()->SetText(m_Column.GetRow(m_CurSel)->GetText());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnDecMoneyValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnDecMoneyValue()
{
	if (m_CurValue > 0)
	{
		SetCurValue(CRoundMoney::RoundMoneyDecrement((uint)m_CurValue));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnDecPercentageValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnDecPercentageValue()
{
	if (m_CurValue > 0)
	{
		SetCurValue(--m_CurValue);
	}
}

/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnIncCurSel
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnIncCurSel()
{
	if (GetTextRegion()->IsMoney() == true)
	{
		OnIncMoneyValue();
	}
	else if (GetTextRegion()->IsPercentage() == true)
	{
		OnIncPercentageValue();
	}
	else
	{
		if (m_CurSel < m_Column.GetRowCount() - 1)
		{
			m_CurSel++;
		}
		else
		{
			m_CurSel = 0;
		}
		GetTextRegion()->SetText(m_Column.GetRow(m_CurSel)->GetText());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnIncValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnIncMoneyValue()
{
	SetCurValue(CRoundMoney::RoundMoneyIncrement((uint)m_CurValue));
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnIncPercentageValue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnIncPercentageValue()
{
	if (m_CurValue < 100)
	{
		SetCurValue(++m_CurValue);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnKeyDown
	Access:    	public 
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextSelector::OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags)
{
	if (_Char == m_ButtonDownKey)
	{
		OnDecCurSel();
		GetTextRegion()->GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATEDOWN);
		return true;
	}
	else if (_Char == m_ButtonUpKey)
	{
		OnIncCurSel();
		GetTextRegion()->GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATEDOWN);
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == m_ButtonDownKey)
	{
		GetTextRegion()->GetBasicDisplay()->SetButtonStatus(m_ButtonDownID, BUTTONSTATENORMAL);
	}
	else if (nChar == m_ButtonUpKey)
	{
		GetTextRegion()->GetBasicDisplay()->SetButtonStatus(m_ButtonUpID, BUTTONSTATENORMAL);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::OnPaint()
{
	CRect theRect(GetTextRegion()->GetWindowRect());

	CPoint LHPoint, RHPoint;
	LHPoint.x = theRect.left - 2;
	LHPoint.y = theRect.top - 2;
	RHPoint.x = theRect.right;
	RHPoint.y = LHPoint.y;
	GetTextRegion()->GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
	LHPoint.y += eUPDOWNHEIGHT - 1;
	RHPoint.y += eUPDOWNHEIGHT - 1;
	GetTextRegion()->GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::OnLButtonUp
	Access:    	public 
	Parameter: 	int _ButtonPressed
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextSelector::OnLButtonUp(int _ButtonPressed)
{
	bool bRet = false;
	if (_ButtonPressed == m_ButtonDownID)
	{
		OnDecCurSel();
		bRet = true;
	}	
	if (_ButtonPressed == m_ButtonUpID)
	{
		OnIncCurSel();
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::SetCurSel
	Access:    	public 
	Parameter: 	int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::SetCurSel(int _Value)
{
	m_CurSel = _Value;
	if (_Value < m_Column.GetRowCount())
	{
		GetTextRegion()->SetText(m_Column.GetRow(m_CurSel)->GetText());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextSelector::SetCurValue
	Access:    	public 
	Parameter: 	int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextSelector::SetCurValue(int _Value)
{
	m_CurValue = _Value;
	GetTextRegion()->SetText(GetCurValueStr());
}
