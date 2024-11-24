#if !defined(AFX_Screen17_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen17_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen17.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen17Dlg dialog


class CScreen17Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen17Dlg(CWnd* pParent = null);   // standard constructor
	~CScreen17Dlg();

// Dialog Data
    enum 
	{ 
		TOPY17 = 100,
		GRID_WIDTH = 200,
			
		IDD = IDD_SCREENDLGNOCONTROLS 
	};

	enum eVIEWMODE
	{
		TIMELINE_VIEW,
		RATINGS_VIEW,
		EVENTS_VIEW
	};

// Overrides
	int DoModal(CCup* _Cup, CFixture*_Fixture);

// Implementation
protected:
	CCup*					m_pCup;
	CString					m_szTitle;
	CFixture*				m_pFixture;
	eVIEWMODE				m_WhichEvents;
	CPNGDrawInfo			m_pngGoal;
	CPNGDrawInfo			m_pngYellowCard;
	CPNGDrawInfo			m_pngRedCard;
	CPNGDrawInfo			m_pngSmallKits;
	CPNGDrawInfo			m_pngPenaltyShootOut;

	void					DoBuildTimeLineGrid();
	void					DoDrawBlobs(CRect& _DataRect, const int _Num, const int _Num1, const COLORREF _Colour);
	void					DrawBar(int _Value1, int _Value2, int _Header, int _BarHeight, CRect& _Rect);
	void					DrawMatchEvents();
	void					DrawTeamSkill();
	void					OnEvents();
	virtual BOOL			OnInitDialog();
	void					OnPaint();
	void					OnRatings();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnTimeLine();
	void					SetViewMode(int _Grid, eVIEWMODE _View);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen17_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
