//
// CMessageDisplay class
//

#include "KeyScrollBar.h"

#ifndef __MESSAGEDISPLAY_H__
    #define __MESSAGEDISPLAY_H__

#ifndef ORIGINAL_CODE

class CMessageDisplay
{
public:
    CMessageDisplay()
    {
    }

    ~CMessageDisplay()
    {
    }

    bool        AddString(CString& _String, COLORREF _Textcolour = WHITE)
    {
      lLogFmt("CMessageDisplay: AddString %s\n", _String.CStr());
      //TODO
      return true;
    }

    bool        AddString(UINT _nID)
    {
      lLogFmt("CMessageDisplay: AddString ID%d\n", _nID);
      //TODO
      return true;
    }

    bool        AddString(UINT _nID, COLORREF _Textcolour)
    {
      lLogFmt("CMessageDisplay: AddString ID%d\n", _nID);
      //TODO
      return true;
    }
};

#else                                   //  #ifdef ORIGINAL_CODE

class CMessageDisplay
{
public:
		CMessageDisplay();
		~CMessageDisplay();


		enum
		{
			SCROLLBARWIDTH = 14
		};

		void				SetDrawInfo(CWnd* _pParent, const uint _ScrollID, CRect& _Rect, const COLORREF _Colour = BLACK, const bool _ShowBorder = false, const bool _BlankLines = false);
		bool				AddString(CString& _String, COLORREF _Textcolour = WHITE);
		bool				AddString(UINT _nID);
		bool				AddString(UINT _nID, COLORREF _Textcolour);
		bool				AddStringList(const CDWordArray& _nArray);
#ifdef USE_STL
		bool				AddStringList(CStringArray& Strings);
		bool				AddStringList(CStringArray& Strings, const COLORREF HeadColour);
#endif
		bool				CheckIfPointInRect(const CPoint _Point);
		bool				DoAddBlankLine();
		void				DoShowStringImmediate(CString& _String, const COLORREF _Textcolour = WHITE);
		CWnd*				GetParentWnd();
		void				RemoveAll();
		bool				ShowLastLine();
		bool				UpdatePointers();

		bool				OnPageDown();
		bool				OnPageUp();
		void				OnPaint(CDC* _pDC);
		bool				OnVScroll(const UINT _nSBCode, const UINT _nPos, CScrollBar* _pScrollBar);
		bool				IsScrollBarVisible();
		bool				IsVScrollBar(CScrollBar* _pScrollBar);
		void				InvalidateRect();
		void				SetTopY(const int _nTopY);
		void				MoveWindow(CRect& _Rect, bool _bInvalidateRect = false);
		CKeyScrollBar*		GetScrollBar();
		void				SetDrawLastFirst(const bool _bDrawLastFirst);

private:
	CFont					m_NormalFont;
	CRect					m_rectBounding;
	CRect					m_rectBorder;
	CStringList				m_StringList;
	CDWordArray				m_StringColor;
	CWordArray				m_StringHeight;
	CKeyScrollBar			m_ScrollBar;
	HFONT					m_hTextFont;
	COLORREF				m_TextColour;
	CWnd*					m_pParentWnd;
	int						m_nTopY;
	UINT					m_ScrollID;
	bool					m_ScrollBarVisible;
	bool					m_bBorderVisible;
	bool					m_ScrollBarToShow;
	ushort					m_nRowCount;
	int						m_RowHeight;
	int						m_FirstVisibleRow;
	int						m_VisibleRowCount;
	int						m_MaxScrollRange;
	bool					m_bBlankLines;
	bool					m_bDrawLastFirst;

	void					CalculateRowCount();
};

#endif

#endif
