#if !defined(AFX_SCREEN19DLG_H__3FA4A921_0ED9_4C9B_BEE7_6A8B15D7C499__INCLUDED_)
#define AFX_SCREEN19DLG_H__3FA4A921_0ED9_4C9B_BEE7_6A8B15D7C499__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// screen19dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen19Dlg dialog

class CScreen19Dlg : public CDialog
{
// Construction
public:
	CScreen19Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScreen19Dlg)
	enum { IDD = IDD_SCREEN19DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen19Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreen19Dlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN19DLG_H__3FA4A921_0ED9_4C9B_BEE7_6A8B15D7C499__INCLUDED_)
