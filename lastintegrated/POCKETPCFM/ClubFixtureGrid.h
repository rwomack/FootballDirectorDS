#if !defined(AFX_CLUBFIXTUREGRID_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_CLUBFIXTUREGRID_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

/*

	ClubFixtureGrid.h: interface for the CClubFixtureGrid class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


class CMatchInfo;

class CClubFixtureGrid : public CGrid
{
public:
	CClubFixtureGrid(void);
	virtual ~CClubFixtureGrid(void);

	void					DoShowMatchSummary(const ushort _Row);
	void					DoShowOpponentSquad(const ushort _Row);
	void					MoveWindow(CRect& _ClientRect);
	void					OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, CClub* _Club);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					SetMatchType(eClubResultsType _eMatchType);

protected:
	eClubResultsType		m_eMatchType;
	CClub*					m_pCurrentClub;
	CPNGDrawInfo			m_pngSmallKits;
};
#endif