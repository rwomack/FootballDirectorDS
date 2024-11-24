#if !defined(AFX_DIVISIONSELECTORBITMAPDIALOG_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_DIVISIONSELECTORBITMAPDIALOG_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

	DivisionSelectorBitmapDialog.h : header file

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/




class CDivisionSelectorBitmapDialog : public CBitmapDialog
{
public:
	CDivisionSelectorBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	CDivisionSelectorBitmapDialog(void);
	virtual ~CDivisionSelectorBitmapDialog(void);

	// Implementation
protected:
	CDivisionSelector		m_DivisionSelector;

	BOOL OnInitDialog(const int _HelpTopic, int _StrID);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _nFlags, CPoint _Point);
//	void					OnPaint();

	DECLARE_MESSAGE_MAP()
};
#endif