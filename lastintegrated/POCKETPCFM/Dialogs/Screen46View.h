#if !defined(AFX_SCREEN46_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN46_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen46View.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen46View dialog

class CScreen46View : public CGridDivisionSelectorBitmapFormView
{
// Construction
public:
    CScreen46View();
    ~CScreen46View();
	DECLARE_DYNCREATE(CScreen46View)

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};


// Overrides
protected:
	virtual void			OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSelchangeDivisionlist();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnLButtonUp(UINT _Flags, CPoint _Point);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN46_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
