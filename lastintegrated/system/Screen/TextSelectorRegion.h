#if !defined(TEXTSELECTORREGION_INCLUDED_)
#define TEXTSELECTORREGION_INCLUDED_

/*

	TextSelectorRegion.h: interface for the TextSelectorRegion class.

	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


class CTextSelector
{
public:
	CTextSelector(void);
	virtual ~CTextSelector(void);

	void					DoAddDates(const ushort _SeasonsToAddFirst = 1, const ushort _nHowMany = 6, const ushort _nCursel = 3);
	void					DoAddItem(CString& _String, void* _Pointer = NULL);
	void					DoAddItem(int _StringID, void* _Pointer = NULL);
	void					DoAddItemList(CSortedStringList& _SortedTextList);
	void					DoAddStrID(int _StrID, void* _Pointer /*= NULL*/);
	void					DoAddStrIDList(int* _IDList, int _HowMany, int _CurSel = 0);
	void					DoDrawHighlight();
	void					DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	void					DoRemoveAllData();
	int						GetCount();
	void*					GetCurPointer();
	int						GetCurSel();
	int						GetCurValue();
	CString					GetCurValueStr();
	CTextRegion*			GetTextRegion();
	bool					IsDownButton(const int _ButtonID);
	bool					IsUpButton(const int _ButtonID);
	void					MoveWindow(int _XPos, int _YPos);
	void					MoveWindow(CRect _Rect);
	void					OnDecCurSel();
	void					OnDecMoneyValue();
	void					OnDecPercentageValue();
	void					OnIncCurSel();
	bool					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void					OnIncMoneyValue();
	void					OnIncPercentageValue();
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool					OnLButtonUp(int _ButtonPressed);
	void					OnPaint();
	void					SetCurSel(int _Value);
	void					SetCurValue(int _Value);

protected:
	CTextRegion*			m_TextRegion;
	int						m_ButtonDownID;
	int						m_ButtonUpID;
	CGridColumn				m_Column;
	int						m_CurSel;
	int						m_ButtonDownKey;
	int						m_ButtonUpKey;
	int						m_CurValue;
	int						m_NumberSeasonsFirst;
};
#endif