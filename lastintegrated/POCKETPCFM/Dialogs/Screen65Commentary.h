#if !defined(AFX_SCREEN65A1_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
#define AFX_SCREEN65A1_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_

// Screen65Commentary.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen65Commentary dialog

class CScreen65Commentary : public CMatchBitmapPropertyPage
{
	DECLARE_DYNCREATE (CScreen65Commentary)

// Construction
public:
	CScreen65Commentary();
	~CScreen65Commentary();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREEN65a1, 
	
		BUTTON1X = 140 - eUPDOWNWIDTH,
		BUTTON1Y = 226,
		BUTTON1W = eUPDOWNWIDTH,
		BUTTON1H = eUPDOWNHEIGHT,
		
		BUTTON2X = 210,
		BUTTON2Y = 226,
		BUTTON2W = eUPDOWNWIDTH,
		BUTTON2H = eUPDOWNHEIGHT,
	};

	enum eDIALOG65C
	{
		eMATCHSPEEDDOWN,
		eMATCHSPEEDUP,
	};	

	void					UpdateCommentary();
	void					DoAddCommentary(CString strComment, COLORREF colorText = BLUE);

protected:
    CExtStatic				m_CompetitionStatic;
	CString					m_strLastComment;
    CMessageDisplay			m_MessageDisplay;

	void					OnPaint();
  	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    BOOL					OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);

 	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN65A1_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
