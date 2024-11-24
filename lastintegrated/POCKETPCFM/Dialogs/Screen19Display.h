#if !defined(AFX_SCREEN19DISPLAY_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN19DISPLAY_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

---------------------------------------------------------------

	File:- Screen19Display.h


	Football Director 

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------

*/


class CScreen19Display
{
public:
    CScreen19Display();		// standard constructor
    ~CScreen19Display();	// standard destructor

	// Dialog Data
	static const int		NUMONSCREEN = 9;

	void					DoBuildList();
	void					OnLButtonUp(const ushort _Column);
	void					OnDraw(CRect& _rRect, COffScreenBuffer& _rMemDC, CGrid& _Grid);
	BOOL					OnInitDialog(UINT _ID, CWnd* _pParent, CBasicDisplay* _BasicDisplay);
	CDivisionListBox&		GetDivisionList();
	CCalendar&				GetTheDate();
	CSoccerResultList&		GetResultList();

protected:
	CBasicDisplay*			m_BasicDisplay;
	
	void					DisplayMatches(CRect& _rRect, COffScreenBuffer& _rMemDC, CGrid& _Grid);
};
#endif // !defined(AFX_SCREEN19DISPLAY_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
