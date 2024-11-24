#ifndef __ALPHABUTTONTAB_H__
#define __ALPHABUTTONTAB_H__


class CAlphaButtonTab
{
public:
	CAlphaButtonTab()
	{
	}
	  
	~CAlphaButtonTab() {}

	enum 
	{
		NUMBERTABS = 9
	};

	void					DoIncrementClickCount(void);
	void					DoInitialise(int nFirstTabId, int nTopY, CWnd* wndParent);
	void					DoResetClickCount(void);
	CString					GetTabCharStr(UInt16 nTabNum);
	UInt16					GetWhichTab(CString& szStr);
	int						GetX(int nTab);
	CString					OnClicked(UInt16 nTabNum);
	void					OnDraw(CDC* pDC);
	UInt16					GetItemCount(void)
	{
		return NUMBERTABS;
	}
	UInt16					GetCurSel(void)
	{
		return m_nCurSel;
	}
	void					SetCurSelListStart(CString& strFirstItem);
	void					SetCurSel(const UInt16 nCurSel);

protected:
	UInt16					m_nTopY;
	UInt16					m_nCurSel;
	UInt16					m_nClickCount;
	CExtStatic				m_staticAlphaButton[NUMBERTABS];
	CString					m_szStr[NUMBERTABS];
};
#endif
