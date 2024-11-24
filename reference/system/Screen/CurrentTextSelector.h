#if !defined(CURRENTTEXTSELECTOR_INCLUDED_)
#define CURRENTTEXTSELECTOR_INCLUDED_

/*

	CurrentTextSelector.h: interface for the CurrentTextSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


class CCurrentTitledTextSelector
{
public:
	CCurrentTitledTextSelector();

	int						GetCurSel();
	int						GetListSize();
	void					OnInitialise(CTitledTextSelector* _List, const int _HowMany);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();

protected:
	int						m_CurSel;
	CTitledTextSelector*	m_List;
	int						m_HowMany;
};
#endif