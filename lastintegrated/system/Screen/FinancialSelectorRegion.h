#if !defined(FINANCIALSELECTORREGION_INCLUDED_)
#define FINANCIALSELECTORREGION_INCLUDED_

/*

	FinancialSelectorRegion.h: interface for the FinancialSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CFinancialSelector : public CTextRegion
{
public:
	CFinancialSelector(void);
	virtual ~CFinancialSelector(void);

	void					DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	int						GetCurValue();
	void					MoveWindow(CRect _Rect);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool					OnLButtonUp(int _ButtonPressed);

	CString					GetCurValueStr();
	void					MoveWindow(int _XPos, int _YPos);
	void					OnDecValue();
	void					OnIncValue();
	void					SetCurValue(int _Value);

protected:
	int						m_ButtonDownID;
	int						m_ButtonUpID;

	int						m_ButtonDownKey;
	int						m_ButtonUpKey;
	int						m_CurValue;
};
/////////////////////////////////////////////////////////////////////////////

class CFinancialTitledSelector : public CFinancialSelector
{
public:
	CFinancialTitledSelector(void);
	virtual ~CFinancialTitledSelector(void);

	void					DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	void					MoveWindow();

protected:
	CTextRegion*			m_TitleTextRegion;
	int						m_Ypos;
};

#endif 