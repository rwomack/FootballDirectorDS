#if !defined(AFX_Screen65a3_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
#define AFX_Screen65a3_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_

// Screen65LineUps.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen65LineUps dialog

class CScreen65LineUps : public CMatchBitmapPropertyPage
{
	DECLARE_DYNCREATE(CScreen65LineUps)

// Construction
public:
	CScreen65LineUps();
	~CScreen65LineUps();

// Dialog Data
	enum { IDD = IDD_SCREEN65a3 };

// Overrides
protected:
    BOOL					OnInitDialog();
	void					OnPaint();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);

// Implementation
protected:
	SYSTEMTIME				m_SystemTime;
	CPNGDrawInfo			m_bmpPlayerStatus;

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen65a3_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
