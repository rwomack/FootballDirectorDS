
/*

	TextRegion.cpp: implementation for the TextRegion class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::CTextRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion::CTextRegion(void)
	: m_Font(NULL)
	, m_WindowRect(CRect(0, 0, 0, 0))
	, m_BasicDisplay(NULL)
	, m_Style(0)
	, m_Visible(SW_SHOW)
	, m_ParentWnd(NULL)
	, CGridRow()
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::CTextRegion
	Access:    	public 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	const uint _Style
	Returns:   	
	Qualifier: 
------------------------------------------------------------------------------*/
CTextRegion::CTextRegion(CBasicDisplay* _BasicDisplay, CWnd* _ParentWnd, const uint _Style)
	: m_Font(NULL)
	, m_WindowRect(CRect(0, 0, 0, 0))
	, m_Visible(SW_SHOW)
	, CGridRow()
{
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	EXT_ASSERT_POINTER(_ParentWnd, CWnd);
	m_ParentWnd = _ParentWnd;
	m_Style  = _Style;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::~CTextRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion::~CTextRegion(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CString _Text
	Parameter: 	uint _Style
	Parameter: 	COLORREF _Color
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::DoInitialise(CBasicDisplay* _BasicDisplay, const CString& _Text, uint _Style /*= COL_NORMAL*/, COLORREF _Color /*= BLACK*/, void* _Pointer /*= NULL*/)
{
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	DoInitialise(_Text, _Style, BLACK, _Pointer);
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	CString _Text
	Parameter: 	uint _Style
	Parameter: 	COLORREF _Color
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::DoInitialise(const CString& _Text, uint _Style /*= COL_NORMAL*/, COLORREF _Color /*= BLACK*/, void* _Pointer /*= NULL*/)
{
	CGridRow::SetText(_Text, _Pointer);
	m_Style = _Style;
	m_Visible = SW_SHOW;
	SetTextColour(_Color);
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _TextID
	Parameter: 	uint _Style
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::DoInitialise(int _TextID, uint _Style /*= COL_NORMAL*/, COLORREF _Color /*= BLACK*/)
{
	CString strMess;
	strMess.LoadString(_TextID);
	DoInitialise(strMess, _Style, _Color);
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::DoInitialiseNum
	Access:    	public 
	Parameter: 	int _Value
	Parameter: 	uint _Style
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::DoInitialiseNum(int _Value, uint _Style /*= COL_NORMAL*/, COLORREF _Color /*= BLACK*/)
{
	CString strMess;
	strMess.Format(TEXT("%d"), _Value);
	DoInitialise(strMess, _Style, _Color);
}


/*------------------------------------------------------------------------------
	Method:	CTextRegion::DoInitialise
	Access:    public 
	Parameter: CBasicDisplay * _BasicDisplay
	Parameter: int _TextID
	Parameter: uint _Style
	Returns:   void
	Qualifier: 	
	Purpose:
 ------------------------------------------------------------------------------*/
void CTextRegion::DoInitialise(CBasicDisplay* _BasicDisplay, int _TextID, uint _Style /*= COL_NORMAL*/)
{
	CString strMess;
	strMess.LoadString(_TextID);
	DoInitialise(_BasicDisplay, strMess, _Style);
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetAlignment
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTextRegion::GetAlignment()
{
	return m_Style & COL_ALIGNMENT;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetBasicDisplay
	Access:    	public 
	Returns:   	CBasicDisplay*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay* CTextRegion::GetBasicDisplay()
{
	EXT_ASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return m_BasicDisplay;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetParentWnd
	Access:    	public 
	Returns:   	CWnd*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWnd* CTextRegion::GetParentWnd()
{
	EXT_ASSERT_POINTER(m_ParentWnd, CWnd);
	return m_ParentWnd;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetTextExtent
	Access:    	public 
	Returns:   	CSize
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSize CTextRegion::GetTextExtent()
{
	return m_BasicDisplay->GetOffScreenBuffer()->GetTextExtent(CGridRow::GetText());
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetWindowRect
	Access:    	public 
	Returns:   	CRect&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRect& CTextRegion::GetWindowRect()
{
	return m_WindowRect;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::IsDate
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextRegion::IsDate()
{
	if (m_Style & COL_DATE)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::IsMoney
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextRegion::IsMoney()
{
	if (m_Style & COL_MONEY)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::IsPercentage
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTextRegion::IsPercentage()
{
	if (m_Style & COL_PERCENTAGE)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
Method:   	CTextRegion::MoveWindow
Access:    	public 
Parameter: 	CRect _Rect
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::MoveWindow(CRect _Rect)
{
	EXT_ASSERT(_Rect.left < _Rect.right);
	EXT_ASSERT(_Rect.top < _Rect.bottom);
	m_WindowRect = _Rect;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::OnPaint()
{
	CString Str = CGridRow::GetText();
	if (m_Visible == SW_SHOW && Str.IsEmpty() != true)
	{
		COffScreenBuffer* memDC =  m_BasicDisplay->GetOffScreenBuffer();
		EXT_ASSERT_POINTER(memDC, COffScreenBuffer);
		CRect DataRect(m_WindowRect);
		if (GetBackgroundColour() != TRANSCOLOUR)
		{
			memDC->FillSolidRect(&m_WindowRect, GetBackgroundColour());
		}
		if (m_Font == null)
		{
			m_BasicDisplay->SetFontNormal();
		}
		else
		{
			m_BasicDisplay->SetFont(*m_Font);
		}

		memDC->SetTextColor(m_TextColour);
		int LeftX;
		switch (GetAlignment())
		{
			case COL_ALIGNR:
				LeftX = DataRect.right - memDC->GetTextExtent(Str).cx;
				break;
			case COL_ALIGNL:
				LeftX = DataRect.left + 2;
				break;
			case COL_ALIGNC:
				LeftX = DataRect.left + (DataRect.right - DataRect.left) / 2 - memDC->GetTextExtent(Str).cx / 2;
				break;
			default :
				EXT_ASSERT(false);
		}
		LeftX = max(LeftX, DataRect.left + 1);
		memDC->ExtTextOut(LeftX, DataRect.top, ETO_CLIPPED, DataRect, Str, Str.GetLength(), null);
		//DEBUG_OUTPUT(("TextRegion, %s"), (Str));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::SetBasicDisplay
	Access:    	public 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::SetBasicDisplay(CBasicDisplay* _BasicDisplay)
{
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::SetFont
	Access:    	public 
	Parameter: 	CFont * _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::SetFont(CFont* _Font)
{
	EXT_ASSERT_POINTER(_Font, CFont);
	m_Font = _Font;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::SetParentWnd
	Access:    	public 
	Parameter: 	CWnd * _Wnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::SetParentWnd(CWnd* _Wnd)
{
	EXT_ASSERT_POINTER(_Wnd, CWnd);
	m_ParentWnd = _Wnd;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::SetStyle
	Access:    	public 
	Parameter: 	int _Style
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::SetStyle(const uint _Style)
{
	m_Style = _Style;
}


/*------------------------------------------------------------------------------
	Method:    CTextRegion::ShowWindow
	Access:    public 
	Parameter: int _Visible
	Returns:   void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::ShowWindow(int _Visible)
{
	EXT_ASSERT(_Visible == SW_SHOW || _Visible == SW_HIDE);
	m_Visible = _Visible;
}