#if !defined(AFX_Screen61_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen61_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen61.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CScreen61 dialogs

/////////////////////////////////////////////////////////////////////////////
class CScreen61Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen61Dlg(CPlayer* _Player, CWnd* _Parent = null);   // standard constructor
	~CScreen61Dlg();

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS, 

		TOPY61 = 78
	};

	// TextRegions
	enum
	{
		TEXT_GOALS,
		TEXT_APPEARANCES,
		TEXT_STATUS1,
		TEXT_STATUS2,
		TEXT_STATUS3,
		TEXT_STATUS4,

		MAX_TEXTREGIONS
	};

protected:
	CPlayer*				m_pCurrentPlayer;
	CPlayerNameTextRegion	m_PlayerName;
	int						m_ViewMode;
	CCommandBar				m_wndCommandBar;
	int						m_TextRegionList[MAX_TEXTREGIONS];
	CPNGDrawInfo			m_pngLargeKits;

	void					DoBuildCareerGrid();
	void					DoBuildPersonalGrid();
	void					DoBuildStatsGrid();
	void					DoShowAvailability();
	void					DoSizeStatsGrid();
	void					DoSizeCareerGrid();
	void					DoSizePersonalGrid();

	void					OnPlayerBuy();
	void					OnPlayerContract();
	void					OnPlayerFree();
	void					OnPlayerLoan();
	void					OnPlayerOutOfContract();
	void					OnPlayerSell();
	void					OnViewStats();
	void 					OnViewPersonal();
	void 					OnViewCareer();
	void 					OnViewAvailability();

	virtual BOOL			OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnPlayerMenu();
	void					OnSelchangeTab();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
class COwnedScreen61 : public CScreen61Dlg
{
public:
	COwnedScreen61(CPlayer* _Player, CWnd* _Parent = null);   // standard constructor

	BOOL					OnInitDialog();
};


/////////////////////////////////////////////////////////////////////////////
class CNotOwnedScreen61 : public CScreen61Dlg
{
public:
	CNotOwnedScreen61(CPlayer* _Player, CWnd* _Parent = null);   // standard constructor

	BOOL					OnInitDialog();
};


/////////////////////////////////////////////////////////////////////////////
class CNoActionScreen61 : public CScreen61Dlg
{
public:
	BOOL					OnInitDialog();
};
#endif // !defined(AFX_Screen61_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
