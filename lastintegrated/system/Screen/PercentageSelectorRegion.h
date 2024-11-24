#if !defined(PERCENTAGESELECTORREGION_INCLUDED_)
#define PERCENTAGESELECTORREGION_INCLUDED_

/*

	PercentageSelectorRegion.h: interface for the PercentageSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/
 


class CPercentageSelector
{
public:
	CPercentageSelector(void);
	virtual ~CPercentageSelector(void);

	void					DoDrawHighlight();
	void					DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	int						GetCurValue();
	bool					IsDownButton(const int _ButtonID);
	bool					IsUpButton(const int _ButtonID);
	void					MoveWindow(CRect _Rect);
	void					MoveWindow(int _XPos, int _YPos);
	void					OnDecValue();
	void					OnDecreaseValue(const int _Amount);
	void					OnDisplayCurValue();
	void					OnIncValue();
	void					OnIncreaseValue(const int _Amount);
	bool					OnLButtonUp(int _ButtonPressed);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					SetCurValue(int _Value);

protected:
	int						m_ButtonDownID;
	int						m_ButtonUpID;
	int						m_ButtonDownKey;
	int						m_ButtonUpKey;
	int						m_CurValue;
	CTextRegion*			m_TextRegion;
};
/////////////////////////////////////////////////////////////////////////////

class CPercentageTitledSelector : public CPercentageSelector
{
public:
	CPercentageTitledSelector(void);
	virtual ~CPercentageTitledSelector(void);

	void					DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	void					MoveWindow();

protected:
	CTextRegion*			m_TitleTextRegion;
	int						m_Ypos;
};

#endif 