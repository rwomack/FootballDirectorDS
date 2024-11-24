
/*

	Screen03.h : header file

	Status Screen

	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------

*/


#if !defined(AFX_SCREEN03VIEW_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_)
#define AFX_SCREEN03VIEW_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_


#ifdef WIN_STANDARD
enum 
{
	ACTION_MENU_HEIGHT0	= 0,		// Not checked
	ACTION_MENU_HEIGHT1 = 65 + 30,	// Not currently used, not checked
	ACTION_MENU_HEIGHT2 = 83 + 40,	
	ACTION_MENU_HEIGHT3 = 101 + 46,	// Not currently used, not checked
	ACTION_MENU_HEIGHT4 = 119 + 52,	
	ACTION_MENU_HEIGHT5 = 137 + 60,	
	ACTION_MENU_HEIGHT6 = 155 + 68,	
	ACTION_MENU_HEIGHT7 = 173 + 74,
	ACTION_MENU_HEIGHT8 = 191 + 80,
};
#else
enum 
{
	ACTION_MENU_HEIGHT0	= 27,
	ACTION_MENU_HEIGHT1 = 65,	// Not currently used, not checked
	ACTION_MENU_HEIGHT2 = 83,	
	ACTION_MENU_HEIGHT3 = 101,	// Not currently used, not checked
	ACTION_MENU_HEIGHT4 = 119,	
	ACTION_MENU_HEIGHT5 = 137,	
	ACTION_MENU_HEIGHT6 = 155,	
	ACTION_MENU_HEIGHT7 = 173,
	ACTION_MENU_HEIGHT8 = 191,

	ACTION_MENU_ITEMHEIGHT = 18
};
#endif

#define ProfitOut(Amount, textID) \
	theList[NumFound].DoAddIncome(textID, Amount); \
	NumFound ++; \
	Income += Amount;

#define DebitOut(Amount, textID) \
	theList[NumFound].DoAddExpenditure(textID, Amount); \
	NumFound ++; \
	Expenditure += Amount;

class CAccountsGridItem
{
public:
	bool m_IsIncome;
	CString m_Description;
	CExtString m_IncomeStr;
	CExtString m_ExpenditureStr;

	void	DoAddIncome(int _StringID, int _Amount)
	{
		m_IsIncome = true;
		m_Description.LoadString(_StringID);
		m_IncomeStr.FormatLargeNumber(_Amount);
	}

	void	DoAddExpenditure(int _StringID, int _Amount)
	{
		m_IsIncome = false;
		m_Description.LoadString(_StringID);
		m_ExpenditureStr.FormatLargeNumber(_Amount);
	}
};

class CScreen03View : public CBitmapFormView
{
	DECLARE_DYNCREATE(CScreen03View)
public:
	CScreen03View();
	virtual ~CScreen03View();

	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS
	};

	enum eStatusView
	{
		eCLUB,
		eDIVISION,
		eACCOUNTS,
		eMATCH,
		eNEWS,

		eMAXVIEWS
	};

	enum eDIALOG22
	{
		eBORROWDOWN,
		eBORROWUP,

		eREPAYDOWN,
		eREPAYUP, 

	};

	enum
	{
		GRID_WIDTH08 = 230,

		// Screen19
		eDATEDOWN19 = 2,
		eDATEUP19,

		// Screen20
		TOPY20 = DATEDIVISIONSELECTOR_TOPY + 16,
		GRID_WIDTH20 = 234,
		eROUNDDOWN20 = 2,
		eROUNDUP20,

		// Screen22
		eBORROW = 0,
		eREPAY,
		eMAXSELECTOR22,
		eDOWNX22 = 132,
		BUTTON1Y22 = TOPY,
		BUTTON4Y22 = TOPY + 48,

		// Screen27 Special Players
		PICGAP = 36,
		CAPTAINY = 42,
		PENALTYTAKERY = CAPTAINY + PICGAP,
		CORNERTAKERY =  PENALTYTAKERY + PICGAP,
		FREEKICKTAKERY = CORNERTAKERY + PICGAP,
		NAMEX = 70,
		PICX = NAMEX - 60,

		// Screen27 Formation
		iPitchXGap = 19,
		iPitchYGap = 11,

		// Screen33
		MAXLISTSIZE33	= 15,
		TOPY33 = DIVISIONSELECTOR_TOPY + 12,

		// Screen36
		ID_SHOWPLAYERPOPUPMENU = 40000,
		NUM_PERCENT_SELECTORS36 = 4,

		GAPY36 = 13,
		HEADERSTATICY36 = 148,
		BUTTONGAPY36 = 26,

		BUTTON1Y36 = TOPY + 4,
		BUTTON2Y36 = BUTTON1Y36 + BUTTONGAPY36,
		BUTTON3Y36 = BUTTON1Y36 + BUTTONGAPY36 * 2,
		BUTTON4Y36 = BUTTON1Y36 + BUTTONGAPY36 * 3,
		BUTTON5Y36 = BUTTON1Y36 + BUTTONGAPY36 * 4 + 4,

		GRID_WIDTH48 = 230,
};

	virtual void			OnDraw(CDC* _pDC);  // overridden to draw this view
	void					OnIncrementProgress();
	void					OnSize(UINT nType, int cx, int cy);

protected:
	eView					m_CurrentView;
	bool					m_HasDivisionSelector;
	bool					m_HasCupSelector;
	bool					m_HasGrid;

	CCupSelector			m_CupSelector;
	CDivisionSelector		m_DivisionSelector;

	CPNGDrawInfo			m_pngLargeKits;
	CPNGDrawInfo			m_pngSmallKits;
	CPNGDrawInfo			m_pngPlayerStatus;
	CPNGDrawInfo			m_pngRoles;
	CPNGDrawInfo			m_pngNumbers;
	CVOImage				m_Pitch;
	CVOImage				m_pngProgressBar;

	// Screen03
	bool					m_NextMatch;
	bool					m_LastMatch;
	int						m_CurrentSelection;
	CTextRegion*			m_NewsText;
	CClubBalanceTextRegion	m_ClubBalanceText;
	// Screen05
	CScreen05Base			m_Screen;
	// Screen08
	eSortOrder				m_SortOrder;
	int						m_WageMultiplier;
	int						m_WhichEvents;
	uint					m_dTotalEmpWages;
	uint					m_dTotalYouthWages;
	uint					m_dTotalPlayerWages;
	ushort					m_iPlayInfoNum;
	// Screen11
	CMessageDisplay			m_MessageDisplay;
	CStringArray			m_strlistNews;
	// Screen18
	CClubFixtureGrid		m_ClubFixtureGrid;
	// Screen19
	CTextRegion				m_DateText;
	CCalendar				m_TheDate;
	CSoccerResultList		m_ResultList;
	// Screen20
	int						m_CurrentSelector;
	int						m_iCurrentRound;
	int						m_RoundY;
	CString					m_DateStr;
	bool					m_bShowReplays;
	// Screen22
	CCurrentTitledTextSelector m_CurrentTitledTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR22];
	CInvisibleGrid			m_InvisibleGrid;
	// Screen26
	CTextRegion				m_Header;
	int						m_NumOnScreen;
	eFINANCIALPERIOD		m_eWhichPeriod;
	// Screen27 Tactics
	CScreen27TacticsData	m_TacticsData;
	// Screen27 Special Players
	CPlayerSelector			m_Captain;
	CPlayerSelector			m_PenaltyTaker;
	CPlayerSelector			m_CornerTaker;
	CPlayerSelector			m_FreeKickTaker;
	// Screen27 Formation
	CExtRect				m_PitchRect;
	CTextSelector			m_FormationSelector27;
	ushort					m_iPlayerSelected;
	// Screen28
	CProgressBar			m_ProgressBar;
	// Screen 33
	CLeadingDisciplinaryList m_PointScorers;
	CLeadingGoalScorerList	m_GoalScorers;
	int						m_ViewMode;
	// Screen36
	CTitledTextSelector		m_FormationSelector;
	CCurrentPercentSelector	m_CurrentPercentSelector;
	CPercentageTitledSelector m_PercentSelector[NUM_PERCENT_SELECTORS36];
	// Screen39
	CTextSelector			m_TypeList;
	CEmployeeGrid			m_EmployeeGrid;
	// Screen41
	CYouthGrid				m_YouthGrid;
	// Screen48
	// Screen50
	CString					m_strPrint;
	CIndexedIntArray		m_FairPlayList;
	int						m_ClubNameLength;


	void					DoAddLRButtons();
	void					DoAdjustColumnWidthsScreen02();
	void					DoAdjustColumnWidthsScreen19();
	void					DoAdjustColumnWidthsScreen33();
	void					DoBuildGridScreen02();
	void					DoBuildGridScreen08(bool bPreservePosition = false);
	void					DoBuildGridScreen09();
	void					DoBuildGridScreen19();
	void					DoBuildGridScreen20();
	void					DoBuildGridScreen26();
	void					DoBuildGridScreen33();
	void					DoBuildGridScreen39();
	void					DoBuildGridScreen41();
	void					DoBuildGridScreen46();
	void					DoBuildGridScreen48();
	void					DoBuildGridScreen50();
	void					DoInitialiseTextRegions();
	void					DoBuildPlayerList();
	void					DoResetProgress();
	void					DoSaveTeamTraining();
	void					DoSort();
	void					DoUpdateData();
	void					DoUpdateLoanDetails();
	CCupRound&				GetCurrentCupRound();
	void					MoveGridScreen20();
	void					OnAction1();
	void					OnAction2();
	void					OnAction3();
	void					OnAction4();
	void					OnAction5();
	void					OnAction6();
	void					OnAction7();
	void					OnAction8();
	void					OnAction9();
	void 					OnActionClicked();
	void					OnClickAdjustPercentage(const UINT _Which, const bool _bInc = true);
	void 					OnExitView();
	void 					OnInitialiseView(const eView _NewView);
	void					OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnNextCupScreen20();
	void					OnNextCupRoundScreen20();
	void					OnNextDate();
	void 					OnNextMatch();
	void					OnPreviousCupScreen20();
	void					OnPreviousCupRoundScreen20();
	void 					OnScreen02();
public:
	void 					OnScreen03();
protected:
	void 					OnScreen05();
	void 					OnScreen08();
	void 					OnScreen09();
	void 					OnScreen11();
	void 					OnScreen18();
	void 					OnScreen19();
	void 					OnScreen20();
	void 					OnScreen22();
	void 					OnScreen26();
	void 					OnScreen27Tactics();
	void 					OnScreen27Formation();
	void 					OnScreen27SpecialPlayers();
	void 					OnScreen33();
	void 					OnScreen36();
	void 					OnScreen39();
	void 					OnScreen41();
	void 					OnScreen46();
	void 					OnScreen48();
	void 					OnScreen50();
	void					OnSelchangeCupListScreen20();
	void					OnSelchangeCupListScreen02();
	void					OnSelchangeDivisionList();
	void					OnSwitchTab();
	void 					OnViewAccounts();
	void					OnViewMatchReport();
	void 					OnViewLeagueTable();
	void 					OnViewNews();
	void 					OnViewSquad();
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					ReDraw();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN03VIEW_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_)
