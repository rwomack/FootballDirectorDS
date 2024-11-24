/*

	File:- TabbedAlphaListCtrl.h

	Extension to CListCtrl class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "ExtListCtrl.h"
#include "TabbedAlphaListCtrl.h"


BEGIN_MESSAGE_MAP(CTabbedAlphaListCtrl, CExtListCtrl)
END_MESSAGE_MAP()


 // Sets the focus of the list box to the nearest string to the passed in string
 // ---------------------------------------------------------------------------
int CTabbedAlphaListCtrl::SetCurSelNearest(CString& strToFind)
{
	CString strInput(strToFind);
	CString szListText;
	int nCurSel = -1;
	int nTextLength = strToFind.GetLength();
	if (nTextLength > 0)
	{
		if (strToFind.GetLength() > 1)
		{
			szListText = strToFind.Left(1);
			szListText.MakeUpper();
			strToFind = szListText + strToFind.Right(strToFind.GetLength() - 1);
		}
		else
		{
			strToFind.MakeUpper();
		}
		do 
		{
			for (int nLoopCount = 0; nLoopCount < CListCtrl::GetItemCount(); nLoopCount++)
			{
				szListText = CListCtrl::GetItemText(nLoopCount, 0);
				if (strToFind.Left(nTextLength) == szListText.Left(nTextLength))
				{				
					nCurSel = nLoopCount;
					break;
				}
			}
			// Check if match found
			if (nCurSel > -1)
			{
				break;
			}
			// Check 1 character less off rh end of lookup
			nTextLength--;
		} 
		while(nTextLength);
	}
	SendMessage(WM_SETREDRAW, FALSE, 0);
	// Ensure item is top of screen
	CListCtrl::EnsureVisible(nCurSel + (CListCtrl::GetCountPerPage() - 1), FALSE); 
	CListCtrl::EnsureVisible(nCurSel, FALSE); 
	SendMessage(WM_SETREDRAW, TRUE, 0);
	strToFind = strInput.Left(nTextLength);
	return nCurSel;
}


// ---------------------------------------------------------------------------
CString CTabbedAlphaListCtrl::GetText(const UInt16 nIndex)
{
	return CListCtrl::GetItemText(nIndex, 0);
}