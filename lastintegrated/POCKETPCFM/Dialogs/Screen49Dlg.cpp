// Screen49Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "Screen49Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen49Dlg dialog


CScreen49Dlg::CScreen49Dlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CScreen49Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen49Dlg, CBitmapDialog)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen49Dlg message handlers


 //---------------------------------------------------------------------------------------
BOOL CScreen49Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(TEXT("Main Menu"), IDR_PNGSCREEN49, 49);

	return TRUE;
}


 //---------------------------------------------------------------------------------------
void CScreen49Dlg::OnPaint()
{
	CBitmapDialog::OnPaint(true, true);
}
