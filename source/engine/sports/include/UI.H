// dummy version of ui.h

/*
class CGridRow
{
public:
	CGridRow();
	~CGridRow() {}

	void					DoCreate(CString _Text = _T(""), void*_Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	COLORREF				GetBackgroundColour();
//  CPNGDrawInfo*     GetBitmap();
	int						GetBitmapImageNumber();
	void*					GetPointer();
	ePointerType			GetPointerType();
	CString					GetText();
	COLORREF				GetTextColour();
	int						GetValue();
	bool					IsNumeric();
	bool					SetBackgroundColour(COLORREF _Colour);
//  void          SetBitmap(CPNGDrawInfo* _Bitmap, int _ImageNumber = 0);
	bool					SetItemNum(int _NumText = 0, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	void 					SetPointer(void* _Pointer);
	void 					SetPointerType(ePointerType _PointerType);
	void 					SetPointerDetails(void* _Pointer, ePointerType _PointerType);
	bool					SetStrId(uint _StrID, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetText(CString _Text, void *pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetTextColour(COLORREF _Colour);

protected:
//  CString           m_Text;
//  int               m_Count;
//  COLORREF        m_BackgroundColour;
//  COLORREF        m_TextColour;
//
//  int               m_ImageNumber;
//  CPNGDrawInfo*     m_Bitmap;
//
//  void*         m_Pointer;
//  ePointerType      m_PointerType;
};


class CTextRegion : public CGridRow
{
public:
	CTextRegion(void);
	~CTextRegion(void);

	void					DoInitialise(CString _Text, uint _Style = COL_NORMAL, COLORREF _Color = BLACK, void* _Pointer = NULL);
	void					DoInitialise(int _TextID, uint _Style = COL_NORMAL, COLORREF _Color = BLACK);
	void					DoInitialise(CBasicDisplay* _BasicDisplay, CString _Text, uint _Style = COL_NORMAL, COLORREF _Color = BLACK, void* _Pointer = NULL);
	void					DoInitialise(CBasicDisplay* _BasicDisplay, int _TextID, uint _Style = COL_NORMAL);
	void					DoInitialiseNum(int _Value, uint _Style = COL_NORMAL, COLORREF _Color = BLACK);
	int						GetAlignment();
	CBasicDisplay*			GetBasicDisplay();
//  CWnd*         GetParentWnd();
  //CRect&          GetWindowRect();
	bool					IsDate();
	bool					IsMoney();
	bool					IsPercentage();
  //void          MoveWindow(CRect _Rect);
	void					OnPaint();
	void					SetBasicDisplay(CBasicDisplay* _BasicDisplay);
  //void          SetFont(CFont* _Font);
//  void          SetParentWnd(CWnd* _Wnd);
	void					SetStyle(uint _Style);
	void					ShowWindow(int _Visible);

protected:
//  CWnd*         m_ParentWnd;
//  CBasicDisplay*      m_BasicDisplay;
//  uint          m_Style;
//  int           m_Visible;
//  CRect         m_WindowRect;
//  CFont*          m_Font;
};
 */

class CTextSelector /*: public CTextRegion */
{
  lArray<CString> m_strings;
  int             m_selection;
public:
  CTextSelector(void)
  {
  }

  virtual ~CTextSelector(void)
  {
  }

  void          DoAddItem(CString& _String, void* _Pointer = NULL)
  {
    m_strings.Push( _String );
  }

  void          SetCurSel(int _Value)
  {
    m_selection = _Value;
  }

#ifdef ORIGINAL_CODE
  void          DoAddDates(const ushort _SeasonsToAddFirst = 1, const ushort _nHowMany = 6, const ushort _nCursel = 3);
	void					DoAddItemList(CSortedStringList& _SortedTextList);
	void					DoAddStrID(int _StrID, void* _Pointer /*= NULL*/);
	void					DoAddStrIDList(int* _IDList, int _HowMany, int _CurSel = 0);
	void					DoDrawHighlight();
  //void          DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	void					DoRemoveAllData();
	int						GetCount();
	void*					GetCurPointer();
	int						GetCurSel();
	int						GetCurValue();
	CString					GetCurValueStr();
	void					MoveWindow(int _XPos, int _YPos);
  //void          MoveWindow(CRect _Rect);
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

	void					SetCurValue(int _Value);

protected:
	int						m_ButtonDownID;
	int						m_ButtonUpID;
//  CGridColumn       m_Column;
	int						m_CurSel;
	int						m_ButtonDownKey;
	int						m_ButtonUpKey;
	int						m_CurValue;
	int						m_NumberSeasonsFirst;
#endif
};
