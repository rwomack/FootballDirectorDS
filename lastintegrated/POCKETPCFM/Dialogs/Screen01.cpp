// Screen01.cpp : implementation file
//
#include "stdafx.h"
#include "Screen01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen01 dialog


 //---------------------------------------------------------------------------
CScreen01::CScreen01(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CScreen01::IDD, pParent)
{}


BEGIN_MESSAGE_MAP(CScreen01, CBitmapDialog)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST01, OnSelchangeList01)
	ON_BN_CLICKED(IDC_BUTTONOK, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen01 message handlers


 //---------------------------------------------------------------------------
void CScreen01::OnPaint()
{
	CBitmapDialog::OnPaint(false, true);
}


 //---------------------------------------------------------------------------
void CScreen01::OnSelchangeList01()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST01);
	m_UserCountry = pList->GetCurSel();
	if (m_UserCountry > -1)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTONOK);
		ASSERT(pButton);
		pButton->ShowWindow(SW_SHOW);
		CBitmapDialog::DoEnableOKButton();
	}
	else
	{
		CBitmapDialog::DoDisableOKButton();
	}
}


 //---------------------------------------------------------------------------
BOOL CScreen01::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(TEXT("HelpScreen01"), IDR_PNGSCREEN01, 0);	// 0 screen number = no display title bar

	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTONOK);
	ASSERT(pButton);
	pButton->ShowWindow(SW_HIDE);
#ifdef POCKETPC
	SHDoneButton(this->GetSafeHwnd(), SHDB_HIDE);
#endif

	//m_SelectCountryStatic.OnCreate(IDC_STATIC0101, this, 18, BLACK, PALEGREEN, IDS_SELECTCOUNTRY, FW_SEMIBOLD);
 	//m_SelectCountryStatic.ModifyStyle(TRUE ? 0 : WS_BORDER, TRUE ? WS_BORDER : 0, SWP_DRAWFRAME);

  RedrawWindow();
	m_UserCountry = -1;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST01);
	pList->AddString(_T("England"));
	pList->AddString(_T("Germany"));
	pList->AddString(_T("France"));
	pList->AddString(_T("Spain"));
	pList->AddString(_T("Italy"));
	return TRUE;
}


 //---------------------------------------------------------------------------
void CScreen01::OnOK()
{
	theApp.SetWhichCountry(max(0, m_UserCountry));
	CDialog::OnOK();
}
