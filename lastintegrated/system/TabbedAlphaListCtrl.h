#ifndef __TABBEDALPHALISTCTRL_H__
#define __TABBEDALPHALISTCTRL_H__

/*

	File:- TabbedAlphaListCtrl.h

	Extension to CListCtrl class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


class CExtListCtrl;

class CTabbedAlphaListCtrl : public CExtListCtrl
{
public:
	CTabbedAlphaListCtrl() : CExtListCtrl() {};
	  
	~CTabbedAlphaListCtrl() {}

	int						SetCurSelNearest(CString& szStr);
	CString					GetText(const UInt16 nIndex);
		
protected:

    DECLARE_MESSAGE_MAP()
};
#endif
