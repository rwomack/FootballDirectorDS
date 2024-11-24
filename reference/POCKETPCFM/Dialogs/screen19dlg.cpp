// screen19dlg.cpp : implementation file
//

#include "stdafx.h"

#include "Screen19Display.h"
#include "Screen19Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen19Dlg dialog


CScreen19Dlg::CScreen19Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreen19Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreen19Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScreen19Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen19Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen19Dlg, CDialog)
	//{{AFX_MSG_MAP(CScreen19Dlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen19Dlg message handlers
