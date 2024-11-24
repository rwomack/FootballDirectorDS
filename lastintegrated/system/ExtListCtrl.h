#ifndef __EXTLISTCTRL_H__
#define __EXTLISTCTRL_H__

/*

	File:- ExtListCtrl.h

	Extension to CListCtrl class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


class CExtListCtrl : public CListCtrl
{
public:
	CExtListCtrl() : CListCtrl() {};
	  
	~CExtListCtrl() {}

		
protected:
	void					OnLButtonDown(UINT nFlags, CPoint point); 

    DECLARE_MESSAGE_MAP()
};
#endif
