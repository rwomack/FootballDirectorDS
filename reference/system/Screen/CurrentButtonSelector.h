#if !defined(CURRENTBUTTONSELECTOR_INCLUDED_)
#define CURRENTBUTTONSELECTOR_INCLUDED_

/*

	CurrentButtonSelector.h: interface for the CurrentButtonSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


const int MAX_BUTTONS = 10;	// Change this size if more buttons needed on a screen

class CCurrentButtonSelector
{
public:
	CCurrentButtonSelector();

	int						GetCurSel();
	void					OnInitialise(uint* _ResourceID, CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, const int _HowMany, const UINT _PrevButtonKey = VK_UP, const UINT _NextButtonKey = VK_DOWN);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					SetCurSel(int _CurSel);

protected:
	int						m_CurSel;
	int						m_List[MAX_BUTTONS];
	int						m_HowMany;
	UINT					m_PrevButtonKey;
	UINT					m_NextButtonKey;
	CBasicDisplay*			m_BasicDisplay;
};
#endif