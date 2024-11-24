/*

	ExtStatic.cpp : implementation file

	(c) RW Software 2003

---------------------------------------------------------------
*/


#include "stdafx.h"
#include "ExtStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CExtStatic, CStatic)
	ON_WM_LBUTTONDOWN()
#ifdef POCKETPC
	ON_WM_CTLCOLOR_REFLECT()
#endif
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::SetValue
	Access:    	public 
	Parameter: 	const int _Vakue
	Parameter: 	bool _bUpdateStatic 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::SetValue(const int _Vakue, bool _bUpdateStatic /*= true*/)
{
	m_nValue = _Vakue;
	CString strStatic;
	if (_bUpdateStatic == true)
	{
		strStatic.Format(TEXT("%d"), m_nValue);
		CStatic::SetWindowText(strStatic);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::GetValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CExtIntStatic::GetValue() const
{
	return m_nValue;
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::DisplayPercentage
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::DisplayPercentage(const int _Value)
{
	m_nValue = _Value;
	CString strStatic;
	strStatic.Format(TEXT("%u%%"), m_nValue);
	CStatic::SetWindowText(strStatic);
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::IncValue
	Access:    	public 
	Parameter: 	const bool _bUpdateStatic 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::IncValue(const bool _bUpdateStatic /*= true*/)
{
	SetValue(GetValue() + 1, _bUpdateStatic);
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::DecValue
	Access:    	public 
	Parameter: 	const bool _bUpdateStatic 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::DecValue(const bool _bUpdateStatic /*= true*/)
{
	EXT_ASSERT(GetValue() > 0);
	SetValue(GetValue() - 1, _bUpdateStatic);
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::AddValue
	Access:    	public 
	Parameter: 	const int _Value
	Parameter: 	const bool _bUpdateStatic 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::AddValue(const int _Value, const bool _bUpdateStatic /*= true*/)
{
	SetValue(GetValue() + _Value, _bUpdateStatic);
}


/*------------------------------------------------------------------------------
	Method:   	CExtIntStatic::SubValue
	Access:    	public 
	Parameter: 	const int _Value
	Parameter: 	const bool _bUpdateStatic 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtIntStatic::SubValue(const int _Value, const bool _bUpdateStatic /*= true*/)
{
	EXT_ASSERT(GetValue() >= _Value);
	SetValue(GetValue() - _Value, _bUpdateStatic);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPercentageStatic::SetValue
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageStatic::SetValue(const int _Value)
{
	CExtIntStatic::m_nValue = _Value;
	DoUpdateDisplay();
}


/*------------------------------------------------------------------------------
	Method:   	CPercentageStatic::DoUpdateDisplay
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPercentageStatic::DoUpdateDisplay()
{
	CString strStatic;
	strStatic.Format(TEXT("%d%%"), m_nValue);
	CStatic::SetWindowText(strStatic);
}
/////////////////////////////////////////////////////////////////////////////


CExtStatic::~CExtStatic() 
{
	if (m_hBrush != null)
	{
		delete m_hBrush;
	}
	if (m_bFontCreated != false)
	{
		m_Font.DeleteObject();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnLButtonDown
	Access:    	public 
	Parameter: 	const uint _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnLButtonDown(uint _Flags, CPoint _Point)
{
	// Disable red dots!
	Default();
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetForeColor
	Access:    	public 
	Parameter: 	const COLORREF _ForeColour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetForeColor(const COLORREF _ForeColour)
{
	m_ForeColor = _ForeColour;
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetBackColor
	Access:    	public 
	Parameter: 	const COLORREF _BackColour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetBackColor(const COLORREF _BackColour)
{
	m_BackColor = _BackColour;
	if (m_hBrush != null)
	{
		delete m_hBrush;
	}
	m_hBrush = new CBrush(m_BackColor);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetColors
	Access:    	public 
	Parameter: 	const COLORREF _BackColour
	Parameter: 	const COLORREF _ForeColour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetColors(const COLORREF _BackColour, const COLORREF _ForeColour)
{
	SetBackColor(_BackColour);
	SetForeColor(_ForeColour);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetFontSize
	Access:    	public 
	Parameter: 	const int _iPoints
	Parameter: 	const int _nWeight 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetFontSize(const int _iPoints, const int _nWeight /*= FW_MEDIUM*/)
{
	EXT_ASSERT_POINTER(CWnd::m_hWnd,  HWND);
	if (m_bFontCreated != false)
	{
		m_Font.DeleteObject();
	}
    m_Font.CreateFont(_iPoints, 0, 0, 0, _nWeight, FALSE, FALSE, 0, 
					  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
					  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_bFontCreated = true;
	SetFont(&m_Font);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetValue
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetValue(const int _Value)
{
	CString strText;
	strText.Format(TEXT("%d"), _Value);
	SetWindowText(strText);
}


#ifdef POCKETPC
/*------------------------------------------------------------------------------
	Method:   	CExtStatic::CtlColor
	Access:    	public 
	Parameter: 	CDC* _pDC
	Parameter: 	UINT _nCtlColor
	Returns:   	HBRUSH
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
HBRUSH CExtStatic::CtlColor(CDC* _pDC, UINT _nCtlColor)
{
	EXT_ASSERT_POINTER(_pDC, CDC);
	_pDC->SetTextColor(m_ForeColor);
	_pDC->SetBkColor(m_BackColor);
    return(HBRUSH)(m_hBrush->m_hObject);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const CString _szString
	Parameter: 	const RECT& _rect
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	const uint _ResourceId
	Parameter: 	const int _Align
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(const int _FontSize, const COLORREF _ForeColor, 
						  const COLORREF _BackColor, const CString _szString, 
						  const RECT& _rect, CWnd* _pParentWnd, 
						  const uint _ResourceId, const int _Align, const int _Weight)
{
	CStatic::Create(_szString, WS_CHILD | WS_VISIBLE | _Align, _rect, _pParentWnd, _ResourceId);
	OnInitialise(_FontSize, _ForeColor, _BackColor, _szString, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const int _StringId
	Parameter: 	const RECT& _rect
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	const uint _ResourceId
	Parameter: 	const int _Align
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(const int _FontSize, const COLORREF _ForeColor, 
						  const COLORREF _BackColor, const int _StringId, 
						  const RECT& _rect, CWnd* _pParentWnd, 
						  const uint _ResourceId, const int _Align, const int _Weight)
{
	CString strText;
	if (_StringId != 0)
	{
		strText.LoadString(_StringId);
	}
	else
	{
		strText = TEXT("");
	}
	OnCreate(_FontSize, _ForeColor, _BackColor, strText, _rect, _pParentWnd, _ResourceId, _Align, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	CFont* _pFont
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const int _StringId
	Parameter: 	const RECT& _rect
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	const uint _ResourceId
	Parameter: 	const int _Align
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(CFont* _Font, const COLORREF _ForeColor, 
						  const COLORREF _BackColor, const int _StringId, 
						  const RECT& _rect, CWnd* _pParentWnd, 
						  const uint _ResourceId, const int _Align)
{
	EXT_ASSERT_POINTER(_Font, CFont);
	SubclassDlgItem(_ResourceId, _pParentWnd);
	OnInitialise(_Font, _ForeColor, _BackColor, _StringId);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	const UINT _ResourceId
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const int _StringId
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(const UINT _ResourceId, CWnd* _pParentWnd, 
						  const int _FontSize, const COLORREF _ForeColor, 
						  const COLORREF _BackColor, const int _StringId, 
						  const int _Weight)
{
	SubclassDlgItem(_ResourceId, _pParentWnd);
	OnInitialise(_FontSize, _ForeColor, _BackColor, _StringId, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnInitialise
	Access:    	public 
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const int _StringId
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnInitialise(const int _FontSize, const COLORREF _ForeColor, 
							  const COLORREF _BackColor, const int _StringId, 
							  const int _Weight)
{
	CString strText;
	if (_StringId != 0)
	{
		strText.LoadString(_StringId);
	}
	else
	{
		strText = TEXT("");
	}
	OnInitialise(_FontSize, _ForeColor, _BackColor, strText, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	const UINT _ResourceId
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const CString _szString
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(const UINT _ResourceId, CWnd* _pParentWnd, 
						  const int _FontSize, const COLORREF _ForeColor, 
						  const COLORREF _BackColor, const CString _szString, 
						  const int _Weight)
{
	SubclassDlgItem(_ResourceId, _pParentWnd);
	OnInitialise(_FontSize, _ForeColor, _BackColor, _szString, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnCreate
	Access:    	public 
	Parameter: 	const UINT _ResourceId
	Parameter: 	CWnd* _pParentWnd
	Parameter: 	CFont* _pFont
	Parameter: 	const CString _szString
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnCreate(const UINT _ResourceId, CWnd* _pParentWnd, CFont* _Font, 
						  const CString _szString, const COLORREF _ForeColor, 
						  const COLORREF _BackColor)
{
	EXT_ASSERT_POINTER(_Font, CFont);
	SubclassDlgItem(_ResourceId, _pParentWnd);
	OnInitialise(_Font, _ForeColor, _BackColor, _szString);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnInitialise
	Access:    	public 
	Parameter: 	const int _FontSize
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const CString _szString
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnInitialise(const int _FontSize, const COLORREF _ForeColor, 
							  const COLORREF _BackColor, const CString _szString, 
							  const int _Weight)
{
	EXT_ASSERT_POINTER(CWnd::m_hWnd, HWND);		// Not called SubclassDlgItem first!!!!!
	OnInitialiseColors(_ForeColor, _BackColor);
	SetFontSize(_FontSize, _Weight);
	SetWindowText(_szString);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnInitialiseColors
	Access:    	public 
	Parameter: 	const COLORREF _ForeColor 
	Parameter: 	const COLORREF _BackColor 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnInitialiseColors(const COLORREF _ForeColor /*= _BLACK*/, 
									const COLORREF _BackColor /*= WHITE*/)
{
	SetForeColor(_ForeColor);
	SetBackColor(_BackColor);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetWindowText
	Access:    	public 
	Parameter: 	CString _String
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetWindowText(CString _String)
{
	if (_String != TEXT(""))
	{
		CStatic::SetWindowText(_String);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::SetWindowTextNull
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::SetWindowTextNull()
{
	CStatic::SetWindowText(TEXT(""));
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnInitialise
	Access:    	public 
	Parameter: 	CFont * _Font
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const int _StringId
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnInitialise(CFont* _Font, const COLORREF _ForeColor, const COLORREF _BackColor, const int _StringId)
{
	EXT_ASSERT_POINTER(_Font, CFont);
	CString strText;
	if (_StringId != 0)
	{
		strText.LoadString(_StringId);
	}
	else
	{
		strText = TEXT("");
	}
	OnInitialise(_Font, _ForeColor, _BackColor, strText);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::OnInitialise
	Access:    	public 
	Parameter: 	CFont * _Font
	Parameter: 	const COLORREF _ForeColor
	Parameter: 	const COLORREF _BackColor
	Parameter: 	const CString _String
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::OnInitialise(CFont* _Font, const COLORREF _ForeColor, const COLORREF _BackColor, const CString _String)
{
	EXT_ASSERT_POINTER(_Font, CFont);
	EXT_ASSERT_POINTER(CWnd::m_hWnd, HWND);		// Not called SubclassDlgItem first!!!!!
	SetColors(_BackColor, _ForeColor);
	SetFont(_Font);
	SetWindowText(_String);
}


/*------------------------------------------------------------------------------
	Method:   	CExtStatic::Update
	Access:    	public 
	Parameter: 	CString _Str
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtStatic::Update(CString _Str)
{
	CStatic::SetWindowText(_Str);
	RedrawWindow();
}


 //--------------------------------------------------------------------------
void CExtStatic::Update(const int iStringId)
{
	CString strText;
	strText.LoadString(iStringId);
	Update(strText);
}
