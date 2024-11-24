#if !defined(DIVISIONSELECTOR_INCLUDED_)
#define DIVISIONSELECTOR_INCLUDED_

/*

	DivisionSelector.h: interface for the DivisionSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CDivisionSelector : public CTextSelector
{
public:
	CDivisionSelector(void);
	virtual ~CDivisionSelector(void);

	void					DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID);
	CDivision&				GetCurrentDivision();
	void					MoveWindow(CRect _Rect);
	void					SetLeftX(const int _LeftX);

protected:
	int						m_LeftX;
};
#endif