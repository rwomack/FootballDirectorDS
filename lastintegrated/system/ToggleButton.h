#ifndef __TOGGLEBUTTON_H__
#define __TOGGLEBUTTON_H__


class CExtPaintDC;

class CToggleButton
{
public:
	CToggleButton()
	{
	}
	  
	~CToggleButton() {}

	void					DoInitialise(CRect rectArea, int nFirstResourceId, int nFirstStrId, int nSecondStrId, CWnd* wndParent);
	void					OnDraw(CExtPaintDC* pDC);
	UInt16					GetCurSel(void) const
	{
		return m_nCurSel;
	}
	void					SetCurSel(const UInt16 nCurSel);

protected:
	UInt16					m_nCurSel;
	CRect					m_rectRectangle;
	CExtStatic				m_static1;
	CExtStatic				m_static2;
};
#endif
