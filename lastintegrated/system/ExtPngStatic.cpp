/*

	ExtPNGStatic.cpp : implementation file

	(c) RW Software 2003

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "VOImage.h"
#include "ExtPNGStatic.h"


// CExtPNGStatic

IMPLEMENT_DYNAMIC(CExtPNGStatic, CStatic)

CExtPNGStatic::CExtPNGStatic()
{
}

CExtPNGStatic::~CExtPNGStatic()
{
}


BEGIN_MESSAGE_MAP(CExtPNGStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CExtPNGStatic message handlers


//---------------------------------------------------------------------------------------
void CExtPNGStatic::OnPaint()
{
	HDC dc;
	dc = ::GetDC(m_hWnd);
	m_picPicture.Draw(dc, 0, 0);
	::ReleaseDC(m_hWnd, dc);
}


//---------------------------------------------------------------------------------------
void CExtPNGStatic::OnCreate(const UINT nID, CWnd* pParent, DWORD dwResourceID, LPCTSTR pcszClass, HMODULE hModule)
{
	SubclassDlgItem(nID, pParent);
	HDC theDC = ::GetDC(m_hWnd);
	m_picPicture.SetBitmap(theDC, dwResourceID, pcszClass);
	::ReleaseDC(m_hWnd,theDC);
}
