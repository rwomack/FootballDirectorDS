/*
 
	TransparentStatic.cpp : implementation file

	(c) RW Software 2003

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "TransparentStatic.h"


// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic()
{
}

CTransparentStatic::~CTransparentStatic()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CTransparentStatic message handlers


BOOL CTransparentStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTransparentStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Where to draw text
	CRect client_rect;
	GetClientRect(client_rect);

	// Get the caption
	CString szText;
	GetWindowText(szText);

	// Get the font
	CFont *pFont, *pOldFont;
	pFont = GetFont();
	pOldFont = dc.SelectObject(pFont);

	// Map "Static Styles" to "Text Styles"
	#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
	#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

	DWORD dwStyle = GetStyle(), dwText = 0;

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER | DT_SINGLELINE	);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);

	NMAP_STYLE(	SS_LEFTNOWORDWRAP |
				SS_CENTERIMAGE |
				0 |
				0 |
				0,	DT_WORDBREAK				);

	// Set transparent background
	dc.SetBkMode(TRANSPARENT);

	// Draw the text
	dc.SetTextColor(m_ForeColor);
	dc.DrawText(szText, client_rect, dwText);

	// Select old font
	dc.SelectObject(pOldFont);
}


 //--------------------------------------------------------------------------
void CTransparentStatic::OnCreate(const UINT nID, CWnd* pParent, const int iFontSize, const CString szString, const COLORREF ForeColor, const int nWeight)
{
	SubclassDlgItem(nID, pParent);
	OnInitialise(iFontSize, szString, ForeColor, nWeight);
}


 //--------------------------------------------------------------------------
void CTransparentStatic::OnCreate(const UINT nID, CWnd* pParent, const int iFontSize, const int nStringID, const COLORREF ForeColor, const int nWeight)
{
	SubclassDlgItem(nID, pParent);
	CString strStr;
	strStr.LoadString(nStringID);
	OnInitialise(iFontSize, strStr, ForeColor, nWeight);
}


 //--------------------------------------------------------------------------
void CTransparentStatic::OnInitialise(const int iFontSize, const CString szString, const COLORREF ForeColor, const int nWeight)
{
	ASSERT(CWnd::m_hWnd != NULL);		// Not called SubclassDlgItem first!!!!!
	SetFontSize(iFontSize, nWeight);
	SetWindowText(szString);
	m_ForeColor = ForeColor;
}


 //--------------------------------------------------------------------------
void CTransparentStatic::SetFontSize(const int iPoints, const int nWeight)
{
	ASSERT(CWnd::m_hWnd != NULL);
	if (m_bFontCreated != false)
	{
		m_Font.DeleteObject();
	}
    m_Font.CreateFont(iPoints, 0, 0, 0, \
					  nWeight, FALSE, FALSE, 0, \
					  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
					  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_bFontCreated = true;
	SetFont(&m_Font);
}
