#if !defined(PLAYERSELECTOR_INCLUDED_)
#define PLAYERSELECTOR_INCLUDED_

/*

	PlayerSelector.h: interface for the DivisionSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CPlayerSelector : public CTextSelector
{
public:
	CPlayerSelector(void);
	virtual ~CPlayerSelector(void);

	void					DoInitialise(int _CurSel, CPlayerSquad& _Squad, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID);
	CPlayer*				GetCurrentPlayer();

protected:
};
#endif