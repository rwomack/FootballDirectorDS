// Screen07.cpp : Press Conference implementation file
//
#include "stdafx.h"

#include "Screen07.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen07 dialog



CScreen07::CScreen07(CWnd* pParent /*=NULL*/)
    : CBitmapDialog(CScreen07::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen07, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SCREEN07YES, OnOK)
	ON_BN_CLICKED(IDC_SCREEN07NO, OnOK)
	ON_BN_CLICKED(IDC_SCREEN07MAYBE, OnOK)
	ON_BN_CLICKED(IDC_SCREEN07NOCOMMENT, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen07 message handlers


 //---------------------------------------------------------------------------
void CScreen07::OnPaint()
{
	// Offscreen buffer
	CExtOffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	m_buttonYes.OnPaint(&memDC);
	m_buttonNo.OnPaint(&memDC);
	m_buttonMaybe.OnPaint(&memDC);
	m_buttonNoComment.OnPaint(&memDC);
	m_MessageDisplay.OnPaint(&memDC);
	CBitmapDialog::OnPaint(true, true);
}


//---------------------------------------------------------------------------------------
void CScreen07::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_buttonYes.IsPointInRect(point, 1);
	m_buttonNo.IsPointInRect(point, 1);
	m_buttonMaybe.IsPointInRect(point, 1);
	m_buttonNoComment.IsPointInRect(point, 1);
	Invalidate();
}


//---------------------------------------------------------------------------------------
void CScreen07::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_buttonYes.IsPointInRect(point, 2) == true)
	{
		OnOK();
	}
	if (m_buttonNo.IsPointInRect(point, 2) == true)
	{
		OnOK();
	}
	if (m_buttonMaybe.IsPointInRect(point, 2) == true)
	{
		OnOK();
	}
	if (m_buttonNoComment.IsPointInRect(point, 2) == true)
	{
		OnOK();
	}
	Invalidate();
}


 //---------------------------------------------------------------------------
BOOL CScreen07::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(TEXT("HelpScreen07"), IDR_PNGSCREEN07, 7);
	CBitmapDialog::DoDisableOKButton();

	m_buttonYes.DoSetDrawInfo(IDR_PNGYESHI, IDR_PNGYESLO, CRect(10, 202, 110, 223), this);
	m_buttonNo.DoSetDrawInfo(IDR_PNGNOHI, IDR_PNGNOLO, CRect(10, 228, 110, 249), this);
	m_buttonMaybe.DoSetDrawInfo(IDR_PNGMAYBEHI, IDR_PNGMAYBELO, CRect(130, 202, 230, 223), this);
	m_buttonNoComment.DoSetDrawInfo(IDR_PNGNOCOMMENTHI, IDR_PNGNOCOMMENTLO, CRect(130, 228, 110, 249), this);

    // Setup message display
    m_MessageDisplay.SetDrawInfo(this, 										// parent
								 0x1000,									// scrollbar ID
								 CRect(8, 74, 232, 200),					// rectangle
								 (HFONT)theApp.pBigFont()->GetSafeHandle(),		// font
								 BLACK);

	CString strTest;
	strTest = _T("Do you think you can improve the fortunes of the club in the near future and create a squad capable of challenging for trophies?");
	m_MessageDisplay.AddString(strTest);
	m_MessageDisplay.ShowLastLine();
	return TRUE;
}


 //---------------------------------------------------------------------------
void CScreen07::OnOK()
{
	CDialog::OnOK();
}
