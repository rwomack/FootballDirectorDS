
/*

	GridBitmapDialog.h

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CGridBitmapDialog : public CBitmapDialog
{
public:
	CGridBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	CGridBitmapDialog(void);
	virtual ~CGridBitmapDialog(void);

	// Implementation
protected:
	CGrid					m_Grid;

	DECLARE_MESSAGE_MAP()
	BOOL					OnInitDialog(const CString _strHelpTopic, int _StrID);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
};
