/*

	File:- ExtListCtrl.cpp

	Extension to CListCtrl class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "ExtListCtrl.h"


BEGIN_MESSAGE_MAP(CExtListCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// Elminates PocketPC MFC tap & hold
// ---------------------------------------------------------------------------
void CExtListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
   Default(); 
}


