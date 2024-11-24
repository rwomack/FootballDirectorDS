#if !defined(AFX_SCREEN54_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_)
#define AFX_SCREEN54_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_

// Screen54.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen54 dialog

class CScreen54 : public CDivisionSelectorBitmapDialog
{
// Construction
public:
	CScreen54(CWnd* pParent = null);   // standard constructor
	~CScreen54();

// Dialog Data
	enum 
	{
		IDD = IDD_SCREENDLGNOCONTROLS,
		TOPY54 = 42,
		GRID_WIDTH = 180
	};

protected:
	ushort					m_nCurrentSelection;
	CTextSelector			m_DateList;
	CPNGDrawInfo			m_pngSmallKits;

	void					OnAway();
	void					OnButton1();
	void					OnHome();
	void 					OnPreviousDivision();
	void 					OnNextDivision();
	void 					OnPreviousDate();
	void 					OnNextDate();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnPaint();
	void					OnSelchangeDivisionlist();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnViewClub();

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN54_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_)
