#if !defined(AFX_Screen55_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen55_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen55.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CScreen13Dlg dialog


class CScreen55Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen55Dlg(CCup& _Cup, CWnd* pParent = null);   // standard constructor
	~CScreen55Dlg();

	// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};


protected:
	int						m_NumberOfClubsInGroup;
	int						m_NumberOfGroups;
	int						m_ClubNameLength;
	CTextSelector			m_GroupSelector;
	CLeagueTableList		m_GroupTables;

	void					OnCancelButton();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _Flags, CPoint _Point);
	void 					OnNextGroup();
	void					OnPaint();
	void 					OnPreviousGroup();
	void 					OnSelchangeGroupList();
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnViewSquad();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen55_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
