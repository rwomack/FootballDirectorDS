#if !defined(AFX_SCREEN65A4_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
#define AFX_SCREEN65A4_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_

// Screen65Events.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen65Events dialog

class CScreen65Events : public CMatchBitmapPropertyPage
{
	DECLARE_DYNCREATE(CScreen65Events)

// Construction
public:
	CScreen65Events();
	~CScreen65Events();

// Dialog Data
	enum { IDD = IDD_SCREEN65a4 };

protected:
    BOOL            		OnInitDialog();
	void					OnPaint();
	void					DrawBar(int YPos, int Value1, int Value2, int iHeader, CRect* DataRect);
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN65A4_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
