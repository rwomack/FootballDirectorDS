#if !defined(CUPSELECTOR_INCLUDED_)
#define CUPSELECTOR_INCLUDED_

/*

	CupSelector.h: interface for the CupSelector class.


	(c) Sports Director Ltd 2007 - 2008 - 2008

  ---------------------------------------------------------------
*/



class CCupSelector : public CTextSelector
{
public:
	CCupSelector(void);
	virtual ~CCupSelector(void);

	void					DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID);
	CCup&					GetCurrentCup();
	void					MoveWindow(CRect _Rect);

protected:
};
#endif