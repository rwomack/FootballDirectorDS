
/*
  ---------------------------------------------------------------------------------------

	File:- ExtEdit.cpp

	Extension to CEdit class

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"
#include "ExtEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CSIPUnderLinedEdit, CSIPEdit)
    ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(CSIPLookupEdit, CSIPUnderLinedEdit)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::CSIPLookupEdit
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSIPLookupEdit::CSIPLookupEdit()
	: m_dwSIPFlags(0)
{

}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnGetDlgCode
	Access:    	protected 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CSIPLookupEdit::OnGetDlgCode()
{
	uint uiVal = CSIPUnderLinedEdit::OnGetDlgCode();

	return uiVal |= DLGC_WANTALLKEYS;
}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPLookupEdit::OnKeyUp(UINT _Char, UINT _RepCnt, UINT _Flags)
{
	if (_Char != VK_UP && _Char != VK_DOWN && _Char != VK_EXECUTE)
	{
		CEdit::OnKeyUp(_Char, _RepCnt, _Flags);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPLookupEdit::OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags)
{
	if (_Char != VK_UP && _Char != VK_DOWN && _Char != VK_EXECUTE)
	{
		CEdit::OnKeyDown(_Char, _RepCnt, _Flags);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnSetFocus
	Access:    	protected 
	Parameter: 	CWnd* _pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPLookupEdit::OnSetFocus(CWnd* _pOldWnd)
{
	SIPINFO info;
	memset(&info,0,sizeof(info));
	info.cbSize=sizeof(info);

	SHSipInfo(SPI_GETSIPINFO,0,&info,null);
	m_dwSIPFlags=info.fdwFlags;
	info.fdwFlags|=SIPF_DISABLECOMPLETION;

	SHSipInfo(SPI_SETSIPINFO,0,&info,null);
	if(info.fdwFlags & SIPF_ON)
	{
		SHSipPreference(m_hWnd,SIP_FORCEDOWN);
		SHSipPreference(m_hWnd,SIP_UP);
	}
	CSIPUnderLinedEdit::OnSetFocus(_pOldWnd);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnKillFocus
	Access:    	protected 
	Parameter: 	CWnd* _pNewWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPLookupEdit::OnKillFocus(CWnd* _pNewWnd)
{
	SIPINFO info;

	memset(&info,0,sizeof(info));
	info.cbSize=sizeof(info);

	SHSipInfo(SPI_GETSIPINFO,0,&info,null);
	info.fdwFlags=m_dwSIPFlags;


	SHSipInfo(SPI_SETSIPINFO,0,&info,null);
	CSIPUnderLinedEdit::OnKillFocus(_pNewWnd);
}


/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSIPEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::CSIPEdit
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CSIPEdit::CSIPEdit()
	:CEdit(),
m_bSurrenderScrollKeys(false)
{

}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::SetSurrenderScrollKeys
	Access:    	public 
	Parameter: 	const bool _bSurrenderScrollKeys
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPEdit::SetSurrenderScrollKeys(const bool _bSurrenderScrollKeys)
{
	m_bSurrenderScrollKeys = _bSurrenderScrollKeys;
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::SetAllowSIPup
	Access:    	public 
	Parameter: 	const bool _bAllowSIPup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPEdit::SetAllowSIPup(const bool _bAllowSIPup)
{
	m_bAllowSIPup = _bAllowSIPup;
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::SetNewWindowText
	Access:    	public 
	Parameter: 	CString _strLookup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPEdit::SetNewWindowText(CString _strLookup)
{
	SetWindowText(_strLookup);
	SetSel(_strLookup.GetLength(), _strLookup.GetLength());
}


//---------------------------------------------------------------------------------------
void CSIPEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef POCKETPC
	//SHSipPreference(CWnd::GetSafeHwnd(), SIP_UP);
#endif
	CEdit::OnLButtonUp(nFlags, point);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::OnSetFocus
	Access:    	public 
	Parameter: 	CWnd* _pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPEdit::OnSetFocus(CWnd* _pOldWnd)
{
#ifdef POCKETPC
	if (m_bAllowSIPup == true)
	{
		SHSipPreference(CWnd::GetSafeHwnd(), SIP_UP);
	}
#endif
	CEdit::OnSetFocus(_pOldWnd);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::SetFocus
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPEdit::SetFocus()
{
#ifdef POCKETPC
	if (m_bAllowSIPup == true)
	{
		SHSipPreference(this->operator HWND(), SIP_UP);
	}
#endif
	CEdit::SetFocus();
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::PreTranslateMessage
	Access:    	virtual public 
	Parameter: 	MSG* _pMsg
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSIPEdit::PreTranslateMessage(MSG* _pMsg)
{
	if(m_bSurrenderScrollKeys)
	{
		switch(_pMsg->message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				int nVirtKey = (int)_pMsg->wParam;

				if (nVirtKey == VK_UP || nVirtKey == VK_DOWN || nVirtKey == VK_NEXT || nVirtKey == VK_PRIOR || nVirtKey == VK_F23 || nVirtKey == VK_RETURN)
				{
					return GetParent()->SendMessage(_pMsg->message, _pMsg->wParam, _pMsg->lParam);
				}

				break;

			}
		default:
			{
				break;
			}
		}
	}
	return CEdit::PreTranslateMessage(_pMsg);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPEdit::OnGetDlgCode
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CSIPEdit::OnGetDlgCode()
{
	DWORD dwVal = CEdit::OnGetDlgCode();
	if (m_bSurrenderScrollKeys)
		dwVal |= DLGC_WANTALLKEYS | DLGC_WANTARROWS;  //grab the keys so we can toss it back at our parent

	return dwVal;
}


/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CExtEdit::SetValue
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtEdit::SetValue(const int _Value)
{
	CString str;
	str.Format(_T("%d"), _Value);
	SetWindowText(str);
}


/*------------------------------------------------------------------------------
	Method:   	CExtEdit::GetValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CExtEdit::GetValue()
{
#ifdef UNICODE
	wchar_t str[256];
#else
	char str[256];
#endif
	GetWindowText(str, 255);
#ifdef UNICODE
	return _wtoi(str);
#else
	return atoi(str);
#endif
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CString str;
	GetWindowText(str);
	dc.SetTextColor(RGB(0, 0, 0));
	HFONT hOldFont = (HFONT)::SelectObject(dc.operator HDC(), ::GetStockObject(SYSTEM_FONT));
	dc.ExtTextOut(0, 0, 0, CRect(0, 0, 200, 15), str, str.GetLength(), null);
	::SelectObject(dc.operator HDC(), hOldFont);
	DrawUnderLine(&dc);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::OnLButtonDown(UINT _Flags, CPoint _point)
{
   Default();
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnRecognizeGesture
	Access:    	protected 
	Parameter: 	NMHDR* _pNMHDR
	Parameter: 	LRESULT* _pResult
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::OnRecognizeGesture(NMHDR* _pNMHDR, LRESULT* _pResult)
{
   *_pResult = TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnEraseBkgnd
	Access:    	protected 
	Parameter: 	CDC* _pDC
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSIPUnderLinedEdit::OnEraseBkgnd(CDC* _pDC)
{
	CRect ClientRect;

	GetClientRect(&ClientRect);
	ClientRect.bottom++;	// Redraw bit deeper
	_pDC->ExtTextOut(0, 0, ETO_OPAQUE, ClientRect, null, 0, null);
	DrawUnderLine(_pDC);

	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::DrawUnderLine
	Access:    	protected 
	Parameter: 	CDC* _pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::DrawUnderLine(CDC* _pDC)
{
	CPen borderpen(PS_SOLID, 1, m_colorUnderLine);
	CPen* oldPen = _pDC->SelectObject(&borderpen);
	CRect rc;
	// Get the client area of the edit control
	GetClientRect(&rc);
	EXT_ASSERT(m_LineY < rc.bottom);
	_pDC->MoveTo(0, m_LineY);
	_pDC->LineTo(rc.right, m_LineY);		// Top
	_pDC->SelectObject(oldPen);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnSetFocus
	Access:    	protected 
	Parameter: 	CWnd* _pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::OnSetFocus(CWnd* _pOldWnd)
{
	CEdit::OnSetFocus(_pOldWnd);
	Invalidate(FALSE);
}


/*------------------------------------------------------------------------------
	Method:   	CSIPUnderLinedEdit::OnKillFocus
	Access:    	protected 
	Parameter: 	CWnd* _pNewWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSIPUnderLinedEdit::OnKillFocus(CWnd* _pNewWnd)
{
	CEdit::OnKillFocus(_pNewWnd);
	// Force a repaint
	Invalidate(FALSE);
}
