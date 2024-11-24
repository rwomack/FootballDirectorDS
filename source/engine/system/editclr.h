
//////////////////
// Edit control that changes colors when it has focus.
//
 #ifndef _ColorEdit__H__
 #define _ColorEdit__H__
class CColorEdit : public CEdit 
{
public:
	COLORREF BK_COLOR;
	COLORREF FG_COLOR;
		
	CColorEdit() 
	{
		brushstate=FALSE;
	}
	~CColorEdit() { }
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* );
	afx_msg void OnDestroy(void);	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
  
	DECLARE_MESSAGE_MAP()

	CBrush		s_brush;
	BOOL		brushstate;
	BOOL		gotfocus;

	int			maxlength;
};
#endif