#if !defined(AFX_SCREEN97_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
#define AFX_SCREEN97_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_

// Screen97.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen97 dialog

class CScreen97 : public CBitmapDialog
{
// Construction
public:
	CScreen97(CWnd* pParent = null);   // standard constructor

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREEN97,

		BUTTON1X = 12,
		BUTTON1Y = 232,
		BUTTON1W = 87,
		BUTTON1H = 24,

		BUTTON2X = 141,
		BUTTON2Y = 232,
		BUTTON2W = CANCELBUTTONWIDTH,
		BUTTON2H = CANCELBUTTONHEIGHT,
	};

	enum eDIALOG97
	{
		eREGISTER,
		eCANCEL,

		eBUTTONS
	};

 // Implementation
protected:
#ifdef	POCKETPC
	CIntSIPEdit				m_editCode;
#else
	CIntEdit				m_editCode;
#endif
	CExtStatic				m_static01;
	CExtStatic				m_static02;
	CExtStatic				m_static03;
	CExtStatic				m_static04;
	CExtStatic				m_static05;
	CExtStatic				m_static06;
	CExtStatic				m_static07;
	CPNGButton			m_buttonRegister;
	CPNGButton			m_buttonCancel;

	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnOK();
	void					OnHelp();
	void					OnPaint();
	BOOL					OnInitDialog();
	void					DoRegister();
	void					DoCancel();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_SCREEN97_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
