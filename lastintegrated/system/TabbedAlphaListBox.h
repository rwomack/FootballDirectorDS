#ifndef __TABBEDALPHALISTBOX_H__
#define __TABBEDALPHALISTBOX_H__

/*

	File:- TabbedAlphaListBox.h

	Extension to CTabbedAlphaListBox class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


class CTabbedAlphaListBox : public CListBox
{
public:
	CTabbedAlphaListBox() : CListBox(),
		m_bSurrenderAllKeys(false) {};
	  
	~CTabbedAlphaListBox() {};

	void					AddString(CString szCurrentString, UInt16 nIndex);
	void					DoInitialiseList(void);
	int						GetCurrentIndex(void);
	int						SetCurSelNearest(CString& szStr);
	void					SetSurrenderAllKeys(const bool bSurrenderAllKeys)
	{
		m_bSurrenderAllKeys = bSurrenderAllKeys;
	}

protected:
	CArray<UInt16, UInt16>	m_nIndexList;
	bool					m_bSurrenderAllKeys;

	BOOL					PreTranslateMessage(MSG* pMsg);
	void					OnLButtonDown(UINT nFlags, CPoint point)
	{
		Default(); 
	}
	DECLARE_MESSAGE_MAP()
};
#endif
