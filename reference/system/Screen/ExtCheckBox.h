#pragma once


// CExtCheckBox

class CExtCheckBox
{
public:
	CExtCheckBox();
	virtual ~CExtCheckBox();

	void					DoToggle();
	void					GetState();
	void					OnInitialUpdate(CBasicDisplay* _Display, const bool _State, HWND _hWnd);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(const CPoint _Location);

protected:
	CBasicDisplay*			m_BasicDisplay;
	bool					m_CurrentState;
	CPNGDrawInfo			m_pngCheckBox;
	CPoint					m_Location;

};


