//
// CMessageDisplay class
//

#include "stdafx.h"
#include "MessageDisplay.h"

const int MAXSTRINGS = 256;

/////////////////////////////////////////////////////////////////////
// CMessageDisplay class
//


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::CMessageDisplay
	Access:    	public 
	Returns:   
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMessageDisplay::CMessageDisplay()
	: m_nTopY(0)
 	, m_pParentWnd(null)
	, m_TextColour(BLACK)
	, m_ScrollID(0)
	, m_ScrollBarVisible(false)
	, m_bBorderVisible(false)
	, m_ScrollBarToShow(false)
	, m_nRowCount(0)
	, m_RowHeight(0)
	, m_FirstVisibleRow(0)
	, m_VisibleRowCount(0)
	, m_MaxScrollRange(0)
	, m_bBlankLines(false)
	, m_bDrawLastFirst(false)
	, m_Visible(SW_SHOW)
{
	m_StringHeight.SetSize(MAXSTRINGS);
	m_StringColor.SetSize(MAXSTRINGS);
	m_NormalFont.CreateFont(12, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_hTextFont = (HFONT)m_NormalFont.GetSafeHandle();
	m_rectBorder = CRect(0, 0, 0, 0);
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::~CMessageDisplay
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMessageDisplay::~CMessageDisplay()
{
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::SetDrawInfo
	Access:    	public 
	Parameter: 	CWnd* _pParent
	Parameter: 	const uint _ScrollId
	Parameter: 	CRect& _Rect
	Parameter: 	const COLORREF _Colour 
	Parameter: 	const bool _ShowBorder 
	Parameter: 	const bool _BlankLines
	Returns:   	void
	Qualifier: 	
	Purpose:	Set up drawing information
------------------------------------------------------------------------------*/
void CMessageDisplay::SetDrawInfo(CWnd* _pParent, const uint  _ScrollID, 
								  CRect& _Rect, const COLORREF _Colour, 
								  const bool _ShowBorder, const bool _BlankLines)
{
	m_TextColour  = _Colour;
	m_bBorderVisible = _ShowBorder;
	m_bBlankLines = _BlankLines;
	m_pParentWnd   = _pParent;
	m_ScrollID = _ScrollID;

	m_ScrollBar.Create(WS_CHILD | SBS_VERT, _Rect, m_pParentWnd, _ScrollID);
	m_ScrollBar.ShowScrollBar(FALSE);
	m_Visible = SW_SHOW;
	MoveWindow(_Rect);
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::SetSize
	Access:    	public 
	Parameter: 	CRect& _Rect
	Parameter: 	bool _bInvalidateRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::MoveWindow(CRect& _Rect, bool _bInvalidateRect /*= false*/)
{
	if (_bInvalidateRect == true)
	{
		InvalidateRect();			// Display needs updating, clear old rect
	}
	if (m_pParentWnd != null)
	{
		m_rectBorder = _Rect;
		CRect rectScroll = m_rectBorder;
		rectScroll.left = rectScroll.right - SCROLLBARWIDTH;

		m_ScrollBar.MoveWindow(rectScroll);
		m_FirstVisibleRow = 0;
		m_ScrollBarVisible = false;
		m_ScrollBarToShow  = false;

		HDC hDC = ::CreateCompatibleDC(null);

		// Determine font's row height
		if(hDC != null)
		{
			HFONT hOldFont;
			TEXTMETRIC TextMetric;

			if(m_hTextFont == null)
			{
				hOldFont = (HFONT)::SelectObject(hDC, ::GetStockObject(SYSTEM_FONT));
			}
			else
			{
				hOldFont = (HFONT)::SelectObject(hDC, m_hTextFont);
			}
			::GetTextMetrics(hDC, &TextMetric);
			m_RowHeight = TextMetric.tmHeight + TextMetric.tmExternalLeading;
			::SelectObject(hDC, hOldFont);
			::DeleteDC(hDC);
			m_VisibleRowCount = (_Rect.Height() - 2) / m_RowHeight;
		}
		else
		{
			m_RowHeight	= 0;
			m_VisibleRowCount = 0;
		}
		if (_bInvalidateRect == true)
		{
			InvalidateRect();			// Display needs updating, clear old rect
		}
		UpdatePointers();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::RemoveAll
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Remove all strings from the list
------------------------------------------------------------------------------*/
void CMessageDisplay::RemoveAll()
{
	m_StringList.RemoveAll();
	m_ScrollBarVisible = false;
	m_ScrollBar.ShowScrollBar(false);
	m_ScrollBar.SetScrollPos(0);
	m_MaxScrollRange = 0;
	m_FirstVisibleRow = 0;
	m_nRowCount = 0;
	InvalidateRect();
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::SetTopY
	Access:    	public 
	Parameter: 	const int _nTopY
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::SetTopY(const int _nTopY)
{
	InvalidateRect();			// Display needs updating, clear old rect
	m_nTopY = _nTopY;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::InvalidateRect
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::InvalidateRect()
{
	m_rectBorder.top -= m_nTopY;
	m_pParentWnd->InvalidateRect(m_rectBorder, false);
	m_rectBorder.top += m_nTopY;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddStringList
	Access:    	public 
	Parameter: 	const CDWordArray& _nArray
	Returns:   	bool
	Qualifier: 	
	Purpose:	Add an array of strings from the string table to list
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddStringList(const CDWordArray& _nArray)
{
	CString szStr;
	for (int LoopCount = 0; LoopCount < _nArray.GetSize(); LoopCount++ )
	{
		szStr.LoadString(_nArray.GetAt(LoopCount));
		m_StringList.AddTail(szStr);
		m_StringColor.SetAt(m_StringList.GetCount() - 1, m_TextColour);
	}
	return UpdatePointers();
}


#ifdef USE_STL
/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddStringList
	Access:    	public 
	Parameter: 	CStringArray& Strings
	Returns:   	bool
	Qualifier: 	
	Purpose:	Add an array of strings to list
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddStringList(CStringArray& _Strings)
{
	for (int LoopCount = 0; LoopCount < _Strings.GetSize(); LoopCount++)
	{
		m_StringList.AddTail(_Strings[LoopCount]);
		m_StringColor.SetAt(m_StringList.GetCount() - 1, m_TextColour);
	}
	return UpdatePointers();
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddStringList
	Access:    	public 
	Parameter: 	CStringArray& Strings
	Parameter: 	const COLORREF HeadColour
	Returns:   	bool
	Qualifier: 	
	Purpose:	Add an array of strings to list
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddStringList(CStringArray& _Strings, const COLORREF _HeadColour)
{
	for (int LoopCount = 0; LoopCount < _Strings.GetSize(); LoopCount += 2)
	{
		m_StringList.AddTail(_Strings[LoopCount]);
		m_StringColor.SetAt(m_StringList.GetCount() - 1, _HeadColour);
		m_StringList.AddTail(_Strings[LoopCount + 1]);
		m_StringColor.SetAt(m_StringList.GetCount() - 1, m_TextColour);
	}
	return UpdatePointers();
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::DoAddBlankLine
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::DoAddBlankLine()
{
	EXT_ASSERT(m_StringList.GetCount() < MAXSTRINGS);
	m_StringList.AddTail(TEXT("\n"));
	return UpdatePointers();
}	
	

/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddString
	Access:    	public 
	Parameter: 	CString& _String
	Parameter: 	COLORREF _Textcolour
	Returns:   	bool
	Qualifier: 	
	Purpose:	Add string to list
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddString(CString& _String, COLORREF _Textcolour )
{
	EXT_ASSERT(m_StringList.GetCount() < MAXSTRINGS);
	m_StringList.AddTail(_String);
	if (_Textcolour == WHITE)
	{
		_Textcolour = m_TextColour;
	}
	m_StringColor.SetAt(m_StringList.GetCount() - 1, _Textcolour);
	return UpdatePointers();
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::DoShowStringImmediate
	Access:    	public 
	Parameter: 	CString& _String
	Parameter: 	const COLORREF _Textcolour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::DoShowStringImmediate(CString& _String, const COLORREF _Textcolour )
{
	AddString(_String, _Textcolour);
	ShowLastLine();
	InvalidateRect();
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::GetScrollBar
	Access:    	public 
	Parameter: 	void
	Returns:   	CKeyScrollBar*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKeyScrollBar* CMessageDisplay::GetScrollBar()
{
	return &m_ScrollBar;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::SetDrawLastFirst
	Access:    	public 
	Parameter: 	const bool _bDrawLastFirst
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::SetDrawLastFirst(const bool _bDrawLastFirst)
{
	m_bDrawLastFirst = _bDrawLastFirst;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::GetParentWnd
	Access:    	public 
	Returns:   	CWnd*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWnd* CMessageDisplay::GetParentWnd()
{
	return m_pParentWnd;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::CheckIfPointInRect
	Access:    	public 
	Parameter: 	const CPoint _Point
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::CheckIfPointInRect(const CPoint _Point)
{
	bool bRet = false;

	if(_Point.x >= m_rectBounding.left && _Point.x <= m_rectBounding.right)
	{
		if(_Point.y >= m_rectBounding.top && _Point.y <= m_rectBounding.bottom)
		{
			return true;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::ShowWindow
	Access:    	public 
	Parameter: 	int _Visible
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::ShowWindow(int _Visible)
{
	EXT_ASSERT(_Visible == SW_SHOW || _Visible == SW_HIDE);
	m_Visible = _Visible;
	if (_Visible == SW_SHOW && m_ScrollBarVisible == true)
	{
		m_ScrollBar.ShowScrollBar(TRUE);
	}
	else
	{
		m_ScrollBar.ShowScrollBar(FALSE);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::UpdatePointers
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::UpdatePointers()
{
	if (m_rectBorder != CRect(0, 0, 0, 0))
	{
		EXT_ASSERT(m_rectBorder.right > m_rectBorder.left);
		EXT_ASSERT(m_rectBorder.bottom > m_rectBorder.top);
		m_rectBounding.left   = m_rectBorder.left + 1;
		m_rectBounding.top    = m_rectBorder.top + 1;
		m_rectBounding.right  = m_rectBorder.right - 1;
		m_rectBounding.bottom = m_rectBorder.bottom - 1;

		if (m_ScrollBarVisible)
		{
			CRect rectScroll;
			m_ScrollBar.GetWindowRect(&rectScroll);
			m_rectBounding.right -= rectScroll.Width();
			CalculateRowCount();
			m_MaxScrollRange = m_nRowCount - m_VisibleRowCount;
			m_ScrollBar.SetScrollRange(0, m_MaxScrollRange);
		}
		else
		{
			CalculateRowCount();
			if(m_nRowCount > m_VisibleRowCount)
			{
				CRect rectScroll;
				m_ScrollBarVisible = true;
				m_ScrollBar.GetWindowRect(&rectScroll);
				m_rectBounding.right -= rectScroll.Width();
				CalculateRowCount();
				m_MaxScrollRange = m_nRowCount - m_VisibleRowCount;
				m_ScrollBar.SetScrollRange(0, m_MaxScrollRange);
				m_ScrollBarToShow = true;
			}
		}
	}
	return(m_ScrollBarToShow);
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddString
	Access:    	public 
	Parameter: 	UINT nID
	Returns:   	bool
	Qualifier: 	
	Purpose:	Add string from resource ID to list
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddString(const UINT _nID)
{
	CString ResourceString;
	if (ResourceString.LoadString(_nID))
	{
		return AddString(ResourceString);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::AddString
	Access:    	public 
	Parameter: 	UINT nID
	Parameter: 	COLORREF _TextColour
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::AddString(const UINT _nID, const COLORREF _TextColour)
{
	CString ResourceString;
	if (ResourceString.LoadString(_nID))
	{
		return AddString(ResourceString, _TextColour);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::CalculateRowCount
	Access:    	private 
	Returns:   	void
	Qualifier: 	
	Purpose:	Work out how many rows generated by word-wrapping text
------------------------------------------------------------------------------*/
void CMessageDisplay::CalculateRowCount()
{
	HDC hDC = ::CreateCompatibleDC(null);

	// Determine font's row height
	if(hDC != null)
	{
		int TotalHeight = 0, MsgNum = 0;
		HFONT hOldFont;

		if(m_hTextFont == null)
		{
			hOldFont = (HFONT)::SelectObject(hDC, ::GetStockObject(SYSTEM_FONT));
		}
		else
		{
			hOldFont = (HFONT)::SelectObject(hDC, m_hTextFont);
		}

		POSITION StrPosn;
		if (m_bDrawLastFirst == false)
		{
			StrPosn = m_StringList.GetHeadPosition();
		}
		else
		{
			StrPosn = m_StringList.GetTailPosition();
		}
		CString Str;
		while(StrPosn != null)
		{
			if (m_bDrawLastFirst == false)
			{
				Str = m_StringList.GetNext(StrPosn);
			}
			else
			{
				Str = m_StringList.GetPrev(StrPosn);
			}
			RECT TextRect;
			int MessageHeight;

			TextRect.left = 0;
			TextRect.right = m_rectBounding.Width();
			TextRect.top  = 0;
			TextRect.bottom = 0;

			MessageHeight = ::DrawText(hDC, (LPCTSTR)Str, Str.GetLength(), &TextRect, DT_LEFT | DT_WORDBREAK | DT_CALCRECT | DT_EXTERNALLEADING | DT_NOPREFIX);
			TotalHeight += MessageHeight + m_RowHeight * m_bBlankLines;
			m_StringHeight.SetAtGrow(MsgNum, MessageHeight / m_RowHeight);
			MsgNum++;
		}

		::SelectObject(hDC, hOldFont);
		::DeleteDC(hDC);

		m_nRowCount = TotalHeight / m_RowHeight;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_Visible == SW_SHOW)
	{
		switch (nChar)
		{
			case VK_UP:
				OnScroll(SB_LINEUP);
				break;
			case VK_DOWN:
				OnScroll(SB_LINEDOWN);
				break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnPaint
	Access:    	public 
	Parameter: 	CDC* _pDC
	Returns:   	void
	Qualifier: 	
	Purpose:	Drawing method
------------------------------------------------------------------------------*/
void CMessageDisplay::OnPaint(CDC* _pDC)
{
	if (m_Visible == SW_SHOW)
	{
		HDC hDC = _pDC->GetSafeHdc();
		COLORREF OldTxtColour = ::SetTextColor(hDC, m_TextColour);
		int OldBkMode = ::SetBkMode(hDC, TRANSPARENT);
		HFONT hOldFont;

		// Set up DC here
		if(m_hTextFont == null)
		{
			hOldFont = (HFONT)::SelectObject(hDC, ::GetStockObject(SYSTEM_FONT));
		}
		else
		{
			hOldFont = (HFONT)::SelectObject(hDC, m_hTextFont);
		}

		m_rectBounding.top -= m_nTopY;

		// Do drawing here
		if(m_bBorderVisible)
		{
			CPen borderpen(PS_SOLID, 1, m_TextColour);
			CPen *old = _pDC->SelectObject(&borderpen);

			HBRUSH hOldBrush = (HBRUSH) ::SelectObject(hDC, ::GetStockObject(NULL_BRUSH));
			::Rectangle(hDC, m_rectBorder.left - 2, m_rectBorder.top, m_rectBorder.right, m_rectBorder.bottom);
			::SelectObject(hDC, hOldBrush);
			_pDC->SelectObject(old);
		}

		HRGN hClipRect = ::CreateRectRgn(m_rectBounding.left, m_rectBounding.top,
										 m_rectBounding.right, m_rectBounding.top + (m_VisibleRowCount * m_RowHeight));
		::SelectClipRgn(hDC, hClipRect);
		::DeleteObject(hClipRect);

		RECT TextRect(m_rectBounding);
		TextRect.top  = m_rectBounding.top - (m_FirstVisibleRow * m_RowHeight);

		POSITION StrPosn;
		if (m_bDrawLastFirst == false)
		{
			StrPosn = m_StringList.GetHeadPosition();
		}
		else
		{
			StrPosn = m_StringList.GetTailPosition();
		}
		int MsgNum = 0;
		CString DrawStr;
		while(StrPosn != null)
		{
			if (m_bDrawLastFirst == false)
			{
				DrawStr = m_StringList.GetNext(StrPosn);
			}
			else
			{
				DrawStr = m_StringList.GetPrev(StrPosn);
			}
			int MessageHeight = m_StringHeight [MsgNum] * m_RowHeight;

			TextRect.bottom = TextRect.top + MessageHeight + m_RowHeight * m_bBlankLines;
			if(::RectVisible(hDC, &TextRect))
			{
				::SetTextColor(hDC, m_StringColor[MsgNum]);
				::DrawText(hDC, (LPCTSTR) DrawStr, DrawStr.GetLength(), &TextRect, DT_LEFT | DT_WORDBREAK | DT_EXTERNALLEADING | DT_NOPREFIX);
			}

			TextRect.top = TextRect.bottom;
			MsgNum++;
		}

		// Restore DC
		::SelectClipRgn(hDC, null);
		::SetBkMode(hDC, OldBkMode);
		::SetTextColor(hDC, OldTxtColour);
		::SelectObject(hDC, hOldFont);

		if (m_ScrollBarToShow == true)
		{
			m_ScrollBar.ShowScrollBar(TRUE);
			m_ScrollBarToShow = false;
		}
		m_rectBounding.top += m_nTopY;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::ShowLastLine
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::ShowLastLine()
{
	if (m_FirstVisibleRow != m_MaxScrollRange)
	{
		m_FirstVisibleRow = m_MaxScrollRange;
		m_ScrollBar.SetScrollPos(m_FirstVisibleRow);
		InvalidateRect();
		return true;
	}
	else
	{
		InvalidateRect();
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnPageDown
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::OnPageDown()
{
	if (m_FirstVisibleRow < m_MaxScrollRange)
	{
		m_FirstVisibleRow = min(m_FirstVisibleRow + m_VisibleRowCount, m_MaxScrollRange);
		m_ScrollBar.SetScrollPos(m_FirstVisibleRow);
		InvalidateRect();
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnPageUp
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::OnPageUp()
{
	if (m_FirstVisibleRow > 0)
	{
		m_FirstVisibleRow = max(m_FirstVisibleRow - m_VisibleRowCount, 0);
		m_ScrollBar.SetScrollPos(m_FirstVisibleRow);
		InvalidateRect();
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::CheckScrollBarVisible
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::IsScrollBarVisible()
{
	return m_ScrollBarVisible;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnScroll
	Access:    	public 
	Parameter: 	const UINT _nSBCode
	Parameter: 	const UINT _nPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMessageDisplay::OnScroll(const UINT _nSBCode, const UINT _nPos /*= 0*/)
{
	switch (_nSBCode)
	{
		case SB_LINEUP:
			m_FirstVisibleRow--;
			break;
		case SB_LINEDOWN:
			m_FirstVisibleRow++;
			break;
		case SB_PAGEUP:
			m_FirstVisibleRow -= m_VisibleRowCount;
			break;
		case SB_PAGEDOWN:
			m_FirstVisibleRow += m_VisibleRowCount;
			break;
		case SB_THUMBPOSITION:
			m_FirstVisibleRow = _nPos;
			break;
		case SB_THUMBTRACK:
			m_FirstVisibleRow = _nPos;
			break;
	}

	if (m_FirstVisibleRow < 0)
	{
		m_FirstVisibleRow = 0;
	}
	if (m_FirstVisibleRow > m_MaxScrollRange)
	{
		m_FirstVisibleRow = m_MaxScrollRange;
	}

	if (m_FirstVisibleRow != m_ScrollBar.GetScrollPos())
	{
		m_ScrollBar.SetScrollPos(m_FirstVisibleRow);
		InvalidateRect();
	}
}


 /*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::OnVScroll
	Access:    	public 
	Parameter: 	const UINT _nSBCode
	Parameter: 	const UINT _nPos
	Parameter: 	CScrollBar* _pScrollBar
	Returns:   	bool- True if a redraw is required
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::OnVScroll(const UINT _nSBCode, const UINT _nPos, CScrollBar* _pScrollBar)
{
	if (m_Visible == SW_SHOW && IsVScrollBar(_pScrollBar) == true || _pScrollBar == null)
	{
		OnScroll(_nSBCode, _nPos);
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMessageDisplay::IsVScrollBar
	Access:    	public 
	Parameter: 	CScrollBar* _pScrollBar
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMessageDisplay::IsVScrollBar(CScrollBar* _pScrollBar)
{
	if(_pScrollBar == &m_ScrollBar)
	{
		return true;
	}
	return false;
}
