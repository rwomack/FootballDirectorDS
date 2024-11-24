#if !defined(AFX_GRIDDIVISIONSELECTORBITMAPFORMVIEW_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_GRIDDIVISIONSELECTORBITMAPFORMVIEW_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

	GridDivisionSelectorBitmapFormview.h : header file

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CGridDivisionSelectorBitmapFormView : public CDivisionSelectorBitmapFormView
{
public:
	CGridDivisionSelectorBitmapFormView(UINT nIDTemplate);
	~CGridDivisionSelectorBitmapFormView(void);

	// Implementation
protected:
	CGrid					m_Grid;

	virtual void OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID);
	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void					OnPaint();
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);

	DECLARE_MESSAGE_MAP()
};
#endif