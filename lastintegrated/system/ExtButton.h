#ifndef __EXTBUTTON_H__
#define __EXTBUTTON_H__

/*

	File:- ExtButton.h

	Extension to CButton class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

// CButton overridden to allow foreground and background colours to be changed

class CExtColorButton : public CButton
{
public:
	CExtColorButton() : 
	  CButton()	
	  {
		  m_hBrush = NULL;
	  }
	~CExtColorButton() 
	{
		m_Font.DeleteObject();
	}

	void					SetColors(const COLORREF BackColor, const COLORREF ForeColor);
	void					SetNormalColors(const COLORREF BackColor, const COLORREF ForeColor);
	void					SetFocusColors(const COLORREF BackColor, const COLORREF ForeColor);
	void					SetForeColor(const COLORREF ForeColor);
	void					SetBackColor(const COLORREF BackColor);
	void					OnInit(const UINT nID, CWnd* pParent, CRect rectWindow, 
								   const COLORREF ForeColor = BLACK, const COLORREF BackColor = PALEGREY,
								   const COLORREF ForeColorFocus = WHITE, const COLORREF BackColorFocus = BLACK);

protected:
	COLORREF				m_ForeColor;
	COLORREF				m_BackColor;
	COLORREF				m_ForeColorNormal;
	COLORREF				m_BackColorNormal;
	COLORREF				m_ForeColorFocus;
	COLORREF				m_BackColorFocus;
	CBrush*					m_hBrush;
	CFont					m_Font;

	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnSetFocus(CWnd* pOldWnd);	
	void					OnKillFocus(CWnd* pNewWnd);
	HBRUSH					CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


// CButton overridden to disable red dots for tap & hold

class CExtButton : public CButton
{
public:
	CExtButton() : CButton() {};
	  
	~CExtButton() {}

		
protected:
	void					OnLButtonDown(UINT nFlags, CPoint point); 

    DECLARE_MESSAGE_MAP()
};
#endif
