#if !defined(AFX_GRIDDIVISIONSELECTORBITMAPDIALOG_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_GRIDDIVISIONSELECTORBITMAPDIALOG_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

	GridDivisionSelectorBitmapDialog.h : header file

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CGridDivisionSelectorBitmapDialog : public CDivisionSelectorBitmapDialog
{
public:
	CGridDivisionSelectorBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	CGridDivisionSelectorBitmapDialog(void);
	virtual ~CGridDivisionSelectorBitmapDialog(void);

	// Implementation
	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);

protected:
	CGrid					m_Grid;

	virtual BOOL			OnInitDialog(const CString _strHelpTopic, int _StrID);
	void					OnPaint();
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);

	DECLARE_MESSAGE_MAP()
};
#endif