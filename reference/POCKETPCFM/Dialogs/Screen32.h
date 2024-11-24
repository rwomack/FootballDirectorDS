#if !defined(AFX_SCREEN32_H__143D2B4E_F12A_4E9A_9853_25406C1231F3__INCLUDED_)
#define AFX_SCREEN32_H__143D2B4E_F12A_4E9A_9853_25406C1231F3__INCLUDED_

// Screen32.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen32 dialog

class CScreen32Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen32Dlg(CWnd* pParent = null);   // standard constructor
	~CScreen32Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,

		GRID_WIDTH = 230
	};

	enum eView
	{
		INTRO_VIEW,
		LINEUP_VIEW,
		REPORT_VIEW
	};

protected:
    CMessageDisplay			m_MessageDisplay;
	CGrid					m_Grid;
	bool					m_bViewMatch;
	eView					m_CurrentView;
	CPNGDrawInfo			m_pngLargeKits;

	void					DisplayMatchDetails();

	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	BOOL					OnInitDialog();
	void					OnMatchLineups();
	void					OnMatchIntro();
	void					OnOppositionReport();
	void					OnPaint();
	void					OnResultOnly();
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void					SetCurrentView(int _LineupShow, int _ReportShow, eView _NewView);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN32_H__143D2B4E_F12A_4E9A_9853_25406C1231F3__INCLUDED_)
