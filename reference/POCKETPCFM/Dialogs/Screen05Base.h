#pragma once

enum
{
	eSKILL,
	eSTATUS,
	eFINANCE,
	eSTATS
};


class CScreen05Base
{
public:
	CScreen05Base(void);
	virtual ~CScreen05Base(void);

	#define GAPX 21

	void					DoInitialise(HWND _hWnd, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CGrid* _Grid);
	void					DoInitialiseGrid();
	void					DoSwapPlayers();
	CGrid&					GetGrid();
	void					OnDraw(CBasicDisplay* _BasicDisplay);
	void					OnLButtonDown(HWND _hWnd, UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPlayerPopupMenu(UINT iColumn, CPoint point);
	void					OnSelchangeTab(int _CurTab);
	void					OnSize(CRect& theWindow);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					OnViewPlayer();
	void					SetGrid(CGrid* _Grid);

protected:
	CGrid*					m_Grid;
	CTextRegion				m_HeaderTextRegion;
	byte					m_bWhichEvents;
	CPNGDrawInfo			m_pngPlayerStatus;
	CClub*					m_pCurrentClub;
};
