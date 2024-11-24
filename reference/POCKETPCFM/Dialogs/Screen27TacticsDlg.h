#if !defined(AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
#define AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_

// Screen27TacticsDlg.h : header file
//


class CScreen27TacticsDlg : public CInMatchActionBitmapDialog
{

// Construction
public:
    CScreen27TacticsDlg(CWnd* pParent = null);
    ~CScreen27TacticsDlg();

	// Form Data
	enum 
	{ 
		IDD = IDD_SCREEN2701DLG
	};

	// Dialog Data
protected:
	CExtCheckBox			m_CounterAttackToggle;
	CScreen27TacticsData	m_TacticsData;

	void					OnButtonCounterAttack();
	void					OnButtonOffsideTrap();
	BOOL					OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnAction1Clicked();
	void 					OnAction2Clicked();
	void 					OnActionClicked();

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
