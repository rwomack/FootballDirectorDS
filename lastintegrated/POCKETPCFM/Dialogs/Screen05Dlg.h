#if !defined(AFX_SCREEN05DLG_H__CA82EE7C_AA7C_4B88_8819_F44107B1EAE3__INCLUDED_)
#define AFX_SCREEN05DLG_H__CA82EE7C_AA7C_4B88_8819_F44107B1EAE3__INCLUDED_

// Screen05Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen05Dlg dialog


class CScreen05Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen05Dlg(CWnd* pParent = null);   // standard constructor
	virtual ~CScreen05Dlg(); 

	
// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS
	};

// Implementation
protected:
	CScreen05Base			m_Screen;

	void					DoInitialise();
	void					OnPaint();
	virtual BOOL			OnInitDialog();
	void					OnPlayerPopupMenu(UINT iColumn, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void 					OnViewDetails();
	void 					OnSkillView();
	void 					OnStatusView();
	void 					OnContractView();
	void 					OnStatsView();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN05DLG_H__CA82EE7C_AA7C_4B88_8819_F44107B1EAE3__INCLUDED_)
