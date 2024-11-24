#if !defined(AFX_Screen09_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen09_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


// Screen09View.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen09View dialog

class CScreen09View : public CBitmapFormView
{
// Construction
public:
	CScreen09View();

	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS, 

	};

// Implementation
protected:
	ushort					m_iCurrentClub;
	CGrid					m_Grid;

	void					DoBuildGrid();
	ushort					GetCurrentClub() const;						
	void					OnClose();
	void					OnDraw(CDC* pDC);
	virtual void			OnInitialUpdate();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void 					SetCurrentClub(const ushort x);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen09_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
