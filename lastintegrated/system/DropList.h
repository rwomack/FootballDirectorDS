#ifndef __DROPLLIST_H__
#define __DROPLLIST_H__

// DropList.h : header file
//

#include "VOImage.h"


/////////////////////////////////////////////////////////////////////////////
// CDropList

class CDropList
{
public:
	CDropList();
	CDropList(UInt16 x, UInt16 y, bool bPrompt = false);
	~CDropList();

	enum 
	{
		GFXYOFFSET = 6
	};

	void					DoGetInput(UInt16 x, UInt16 y, bool bPrompt, const UINT nFirstID);
	void					DoResetVariables(void);
	virtual bool			DoInputSel(const bool bRadioCheck = false);
	void					DoLoadMenu(UINT nID);
	CMenu*					GetpPopup(void);
	void					GetBulletInput(UInt16 x, UInt16 y, bool bPrompt = false);
	void					GetInput(UInt16 x, UInt16 y, bool bPrompt = false);
	virtual CString			GetCurSelStr(void);
	UInt16					GetCount(void);
	void					OnDrawLeft(CExtPaintDC* dc, CString strDescription);
	void					OnDrawRight(CExtPaintDC* dc, CString strDescription);
	void					SetDrawInfo(CRect rectDisplay, CWnd* pParentWnd, UInt16 nCursel = 0, int nBitmapID = IDR_PNG_DOWNARROW);
	void					SetCurSel(const UInt16 nCurSel);

	//---------------------------------------------------------------------
	UInt16					GetCurSel(void) const
	{
		return m_nCurSel;
	}

	//---------------------------------------------------------------------
	void					SetCurSelHasChanged(const bool x)
	{
		m_bCurSelHasChanged = x;
	}

	//---------------------------------------------------------------------
	void					SetBoldText(const bool x)
	{
		m_bBoldText = x;
	}

	//---------------------------------------------------------------------
	bool					GetBoldText(void) const
	{
		return m_bBoldText;
	}

	//---------------------------------------------------------------------
	bool					HasCurSelChanged(void) const
	{
		return m_bCurSelHasChanged;
	}

	//---------------------------------------------------------------------
	BOOL					PtInRect(POINT point) const
	{
		return m_rectDisplay.PtInRect(point);
	}

protected:
	CWnd*					m_pParentWnd;
    CVOImage				m_DIBSection;
	CFont					m_fontTahoma;
	CFont					m_fontTahomaBold;
	int						m_nIDResource;
	CMenu					m_Menu;
	CRect					m_rectDisplay;
	UInt16					m_nCurSel;
	bool					m_bPopupActive;
	bool					m_bMenuLoaded;
	bool					m_bCurSelHasChanged;
	bool					m_bBoldText;
	UInt16					m_InputX;
	UInt16					m_InputY;
	bool					m_bPrompt;
};
#endif
