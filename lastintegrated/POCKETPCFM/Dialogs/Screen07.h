#if !defined(AFX_Screen07_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen07_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen07.h : header file
//

#include "MessageDisplay.h"

/////////////////////////////////////////////////////////////////////////////
// CScreen07 dialog


class CScreen07 : public CBitmapDialog
{
// Construction
public:
    CScreen07 (CWnd* pParent = NULL);   // standard constructor
	~CScreen07 (void)			{ m_StaticFont.DeleteObject(); }

// Dialog Data
    enum { IDD = IDD_SCREEN07 };

protected:
    CMessageDisplay			m_MessageDisplay;
	CFDPNGButton			m_buttonYes;
	CFDPNGButton			m_buttonNo;
	CFDPNGButton			m_buttonMaybe;
	CFDPNGButton			m_buttonNoComment;

// Implementation
	void					OnOK(void);
	void					OnPaint(void);
	virtual BOOL			OnInitDialog(void);
	void					OnLButtonDown(UINT nFlags, CPoint point); 
	void					OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen07_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
