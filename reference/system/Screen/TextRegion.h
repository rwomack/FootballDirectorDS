#if !defined(TEXTREGION_INCLUDED_)
#define TEXTREGION_INCLUDED_

/*

	TextRegion.h: interface for the TextRegion class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CTextRegion : public CGridRow
{
public:
	CTextRegion(void);
	CTextRegion(CBasicDisplay* _BasicDisplay, CWnd* _ParentWnd, const uint _Style);
	~CTextRegion(void);

	void					DoInitialise(const CString& _Text, uint _Style = COL_NORMAL, COLORREF _Color = BLACK, void* _Pointer = NULL);
	void					DoInitialise(int _TextID, uint _Style = COL_NORMAL, COLORREF _Color = BLACK);
	void					DoInitialise(CBasicDisplay* _BasicDisplay, const CString& _Text, uint _Style = COL_NORMAL, COLORREF _Color = BLACK, void* _Pointer = NULL);
	void					DoInitialise(CBasicDisplay* _BasicDisplay, int _TextID, uint _Style = COL_NORMAL);
	void					DoInitialiseNum(int _Value, uint _Style = COL_NORMAL, COLORREF _Color = BLACK);
	int						GetAlignment();
	CBasicDisplay*			GetBasicDisplay();
	CWnd*					GetParentWnd();
	CSize					GetTextExtent();
	CRect&					GetWindowRect();
	bool					IsDate();
	bool					IsMoney();
	bool					IsPercentage();
	void					MoveWindow(CRect _Rect);
	void					OnPaint();
	void					SetBasicDisplay(CBasicDisplay* _BasicDisplay);
	void					SetFont(CFont* _Font);
	void					SetParentWnd(CWnd* _Wnd);
	void SetStyle(const uint _Style);
	void					ShowWindow(int _Visible);

protected:
	CWnd*					m_ParentWnd;
	CBasicDisplay*			m_BasicDisplay;
	uint		 			m_Style;
	int						m_Visible;
	CRect		 			m_WindowRect;
	CFont*					m_Font;
};
#endif