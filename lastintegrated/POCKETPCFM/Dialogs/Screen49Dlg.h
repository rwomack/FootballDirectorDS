#if !defined(AFX_SCREEN49DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_)
#define AFX_SCREEN49DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_

// Screen49Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen49Dlg dialog


class CScreen49Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen49Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREEN49DLG,
	};

// Implementation
protected:

	virtual BOOL			OnInitDialog(void);
	void					OnPaint(void);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN49DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_)
