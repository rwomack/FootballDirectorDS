/*

	CupSelectorBitmapFormView.h : header file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/




class CCupSelectorBitmapFormView : public CBitmapFormView
{
public:
	CCupSelectorBitmapFormView(UINT nIDTemplate);
	virtual ~CCupSelectorBitmapFormView(void);


	// Implementation
protected:
	CCupSelector			m_CupSelector;

	DECLARE_MESSAGE_MAP()
	void OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID);
	void					OnNextCup();
	void					OnPreviousCup();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _nFlags, CPoint _Point);
	void					OnPaint();
};
