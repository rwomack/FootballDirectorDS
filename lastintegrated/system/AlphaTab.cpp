#include "stdafx.h"

#include "TabbedAlphaListBox.h"
#include "AlphaTab.h"



 // ---------------------------------------------------------------------------
void CAlphaTab::DoInitialise()
{
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	CString szString;
	szString = _T("A-C");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(0, &tcItem);
	szString = _T("D-G");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(1, &tcItem);
	szString = _T("H-L");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(2, &tcItem);
	szString = _T("M-O");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(3, &tcItem);
	szString = _T("P-S");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(4, &tcItem);
	szString = _T("T-V");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(5, &tcItem);
	szString = _T("W-Z");
	tcItem.pszText = szString.GetBuffer(256);
	CTabCtrl::InsertItem(6, &tcItem);
	CTabCtrl::SetCurSel(0);
}


 // ---------------------------------------------------------------------------
UInt16 CAlphaTab::GetWhichTab(CString& szStr, CTabbedAlphaListBox& pListBox)
{
	UInt16 nRet = 0xffff;
	CString szStringToFind = szStr.Left(1);
	szStringToFind.MakeUpper();
	for (int n = 0; n < CTabCtrl::GetItemCount(); n++)
	{
		if (pListBox.GetAlphaStr(n).FindOneOf(szStringToFind) != -1)
		{
			nRet = n;
			break;
		}
	}
	ASSERT(nRet != 0xffff);
	return nRet;
}
