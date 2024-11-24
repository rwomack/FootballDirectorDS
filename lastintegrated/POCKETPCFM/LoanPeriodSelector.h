#pragma once

class CLoanPeriodSelector
{
public:
	CLoanPeriodSelector(void);
	~CLoanPeriodSelector(void);

	int GetPeriod();
	BOOL					OnInitDialog(CBasicDisplay* _BasicDisplay, CWnd* _Wnd);
	void					OnLButtonUp(const int _ButtonPressed);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnMakeOffer();
	void					OnSize(CRect _Rect, const int _CentreX);

protected:
	CTextRegion				m_Static;
	CTextSelector			m_PeriodSelector;
};
