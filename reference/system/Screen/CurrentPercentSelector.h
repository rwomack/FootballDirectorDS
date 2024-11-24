#if !defined(CURRENTPERCENTSELECTOR_INCLUDED_)
#define CURRENTPERCENTSELECTOR_INCLUDED_

/*

	CurrentPercentSelector.h: interface for the CurrentPercentSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


class CPercentageTitledSelector;


class CCurrentPercentSelector
{
public:
	CCurrentPercentSelector();

	int						GetCurSel() const;
	int						GetListSize() const;
	void					OnInitialise(CPercentageTitledSelector* _List, const int _HowMany, const int _ListSize);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();

protected:
	int						m_CurSel;
	int						m_ListSize;
	CPercentageTitledSelector* m_List;
	int						m_HowMany;
};
#endif