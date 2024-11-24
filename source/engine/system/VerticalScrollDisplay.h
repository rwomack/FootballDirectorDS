
/*

	VerticalScrollDisplay.h: interface for the CVerticalScrollDisplay class.


	(c) Rw Software 1994 - 2007

  ---------------------------------------------------------------------------------------
*/



#if !defined(AFX_VERTICALSCROLLDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_VERTICALSCROLLDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_

class CVerticalScrollDisplay
{
public:
	CVerticalScrollDisplay() {};

	void					OnInitialise(const int _RowsOnScreen, const uint _RowHeight, int _ScrollBarID, CRect _ListArea);
	bool 					IsScrollBarVisible();
	bool					OnVScroll(const uint _SBCode, const uint _Pos, CScrollBar* _pScrollBar);
	void					ResetScrollBar(const uint _NumItems, CScrollBar* _pScrollBar);
	int 					GetFirstRowOffBottomScreen();
	int 					GetVerticalScroll();
	int 					GetNumRowsOnScreen();	
	int 					GetRowHeight();
	void					SetRowHeight(const uint _Value);

protected:
};
#endif