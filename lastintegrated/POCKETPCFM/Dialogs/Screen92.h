#if !defined(AFX_SCREEN92_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN92_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_



/////////////////////////////////////////////////////////////////////////////
// CScreen92 dialog

class CScreen92Dlg : public CDivisionSelectorBitmapDialog
{
// Construction
public:
    CScreen92Dlg(CWnd* pParent = null);   // standard constructor

// Dialog Data
    enum
	{
		IDD = IDD_SCREENDLGNOCONTROLS,
		GRID_WIDTH = 238,
	};

// Overrides
protected:
	CPNGDrawInfo			m_pngSmallKits;

	void					OnSelchangeDivisionlist();

	virtual BOOL			OnInitDialog();
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnViewPlayerDetails();
	void					OnViewSquadDetails();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN92_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
