#if !defined(AFX_SCREEN01_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN01_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen01.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen01 dialog

class CScreen01 : public CBitmapDialog
{
// Construction
public:
	CScreen01(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SCREEN01 };


// Overrides
	protected:
	void					OnSelchangeList01(void); 

// Implementation
protected:
	int						m_UserCountry;

	void					OnOK(void);
	void					OnPaint(void);
	virtual BOOL			OnInitDialog(void);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN01_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
