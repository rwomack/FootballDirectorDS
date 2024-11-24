#if !defined(AFX_SCREEN04_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN04_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

	Screen04.h : header file

	Manager Club selection

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------

*/



class CScreen04 : public CDivisionSelectorBitmapDialog
{
// Construction
public:
	CScreen04(const ushort nLastClubID = wNOTFOUND, CWnd* pParent = null);   // standard constructor
	virtual ~CScreen04() {};

// Dialog Data
	enum 
	{
		IDD = IDD_SCREENDLGNOCONTROLS,

		GRID_WIDTH = 180
	};

protected:
	ushort					m_LastClubID;
	CPNGDrawInfo			m_pngSmallKits;

	virtual BOOL			OnInitDialog();
	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnOK();
	void					OnPaint();
	void					OnSelchangeDivisionlist();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnViewClub();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN04_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
