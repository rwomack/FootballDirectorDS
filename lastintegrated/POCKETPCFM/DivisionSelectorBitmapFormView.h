/*

	DivisionSelectorBitmapFormview.h : header file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/




class CDivisionSelectorBitmapFormView : public CBitmapFormView
{
public:
	CDivisionSelectorBitmapFormView(UINT nIDTemplate);
	virtual ~CDivisionSelectorBitmapFormView(void);


	// Implementation
protected:
	CDivisionSelector		m_DivisionSelector;

	void OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _nFlags, CPoint _Point);
	void					OnPaint();
	void 					OnNextDivision();
	void 					OnPreviousDivision();

	DECLARE_MESSAGE_MAP()
};
