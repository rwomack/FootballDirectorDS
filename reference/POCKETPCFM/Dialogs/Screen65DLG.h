


// CScreen65DLG dialog

class CScreen65Dlg : public CBitmapDialog
{
public:
	CScreen65Dlg(CMatchPlay* _MatchPlay, CWnd* pParent = NULL);   // standard constructor
	virtual ~CScreen65Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,

		LEAGUETABLEGRIDWIDTH = 72 + 18 + 18 + 18 + 18 + 26 + 26 + 26
	};

	enum eVIEWMODE
	{
		eCOMMENTARY,
		eSKILL,
		eLINEUPS,
		eEVENTS,
		eRADAR, RED,
		eLATESTSCORES,
		eLEAGUETABLE,

		eMAXVIEWMODE,
	};

	enum 
	{
		ID_CLOCK = 500,
		ID_KICKOFF,
// 		ID_MOVIE,
// 		ID_PENALTY,
// 		ID_DISPLAY,
// 		ID_NEXTEVENT,
// 		ID_SOUND
	};

protected:
	bool					m_SubstitutionRequested;
	eVIEWMODE				m_ViewMode;
	CMatchPlay*				m_MatchPlay;
	bool					m_bIsMatchPaused;

	CTextRegion				m_HomeScoreStatic;
	CTextRegion				m_AwayScoreStatic;
	CTextRegion				m_TimeStatic;

	// Commentary View
	CString					m_strLastComment;
	CMessageDisplay			m_CommentaryDisplay;

	// Skill View

	//  eLINEUPS
	CGrid					m_LineupGrid;

	//  eEVENTS,

	//  eRADAR,
	CVOImage				m_Pitch;
	CExtRect				m_PitchRect;
	CVOImage				m_Ball;

	//  eLATESTSCORES,
	CSoccerResultGrid		m_LatestScoresGrid;

	//  eLEAGUETABLE,
	CGrid					m_TableGrid;

	// Implementation
	void					DoAddCommentary(CString strComment, COLORREF colorText = BLUE);
	void					DoBuildLeagueTable();
	void					DoBuildLineupGrid();
	void					DoCommenceHalf();
	void					DoDrawBar(int _YPos, int _Value1, int _Value2, int _HeaderStrID, int _BarHeight, CRect& _DataRect);
	void					DoEndOfHalf();
	void					DoPlaySoundFX(const eSOUNDFX theFX);
	void					DoSizeCommentaryBox();
	void					DoUpdateCommentary();
	void					SwitchToMode(eVIEWMODE _NewViewMode);

	// Menu options
	void					OnDoSubstitutions();
	void					OnMatchRadar();
	void					OnViewCommentary();
	void					OnViewEvents();
	void					OnViewLatestScores();
	void 					OnSlowMatch();
	void					OnViewSkill();
	void					OnSkipToEndOfMatch();
	void 					OnSpeedMatch();
	void 					OnSubstitutions();
	void					OnViewLeagueTables();
	void					OnViewTeams();

	BOOL					OnInitDialog();
	void					OnLButtonUp(UINT _Flags, CPoint _Point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnTimer(UINT _EventID);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};
