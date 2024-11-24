#if !defined(AFX_SCREEN36VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_)
#define AFX_SCREEN36VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_

// Screen36View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen36View dialog

class CScreen36View : public CBitmapFormView
{
// Construction
public:
	CScreen36View();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen36View)

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
		ID_SHOWPLAYERPOPUPMENU = 40000,

		NUM_PERCENT_SELECTORS = 4,

		GAPY = 13,
		HEADERSTATICY = 148,
		BUTTONGAPY = 26,

		BUTTON1Y = TOPY + 4,
		BUTTON2Y = BUTTON1Y + BUTTONGAPY,
		BUTTON3Y = BUTTON1Y + BUTTONGAPY * 2,
		BUTTON4Y = BUTTON1Y + BUTTONGAPY * 3,
		BUTTON5Y = BUTTON1Y + BUTTONGAPY * 4 + 4,
	};

// Implementation
protected:
	CTitledTextSelector		m_FormationSelector;
	CCurrentPercentSelector	m_CurrentTextSelector;
	CPercentageTitledSelector m_PercentSelector[NUM_PERCENT_SELECTORS];
	bool					m_bTeamPercentLock[NUM_PERCENT_SELECTORS];

	void					DoSaveTeamTraining();
	bool*					GetpLock();
	void					OnClickLock(UINT nButtonID);
	void					OnClickAdjustPercentage(const UINT nWhich, const bool bInc = true);
	virtual void			OnDraw(CDC* pDC);
	virtual void			OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnSwitchTab();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN36VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_)
