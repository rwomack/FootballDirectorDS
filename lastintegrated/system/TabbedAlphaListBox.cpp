
/*

	File:- TabbedAlphaListBox.cpp

	Extension to CTabbedAlphaListBox class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "TabbedAlphaListBox.h"



BEGIN_MESSAGE_MAP(CTabbedAlphaListBox, CListBox)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



//---------------------------------------------------------------------
BOOL CTabbedAlphaListBox::PreTranslateMessage(MSG* pMsg)
{
	if(m_bSurrenderAllKeys && pMsg->message == WM_KEYDOWN && pMsg->wParam != VK_UP && pMsg->wParam != VK_DOWN)
	{
		if (pMsg->wParam == VK_EXECUTE)
		{
			ASSERT(0);
		}
		return GetParent()->SendMessage(pMsg->message,pMsg->wParam, pMsg->lParam);
	}
	else
	{
		return CListBox::PreTranslateMessage(pMsg);
	}
}


 // ---------------------------------------------------------------------------
void CTabbedAlphaListBox::DoInitialiseList()
{
	m_nIndexList.RemoveAll();
	while(CListBox::GetCount())
	{
		CListBox::DeleteString(0);
	}
}


 // ---------------------------------------------------------------------------
void CTabbedAlphaListBox::AddString(CString szCurrentString, UInt16 nIndex)
{
	CListBox::AddString(szCurrentString);
	m_nIndexList.Add(nIndex);
}


 // Sets the focus of the list box to the nearest string to the passed in string
 // ---------------------------------------------------------------------------
int CTabbedAlphaListBox::SetCurSelNearest(CString& strToFind)
{
	CString strInput(strToFind);
	CString strListText;
	int nCurSel = -1;
	int low;
	int mid;
	int high;
	int nTextLength = strToFind.GetLength();
	if (nTextLength > 0)
	{
		if (strToFind.GetLength() > 1)
		{
			strListText = strToFind.Left(1);
			strListText.MakeUpper();
			strToFind = strListText + strToFind.Right(strToFind.GetLength() - 1);
		}
		else
		{
			strToFind.MakeUpper();
		}

		do
		{
			low = 0;
			high = CListBox::GetCount() - 1;

			while(low <= high)
			{
				mid = (low + high) / 2;
				CListBox::GetText(mid, strListText);
				strListText = strListText.Left(nTextLength);
				if(strListText < strToFind.Left(nTextLength))
				{
					low = mid + 1;
				}
				else if(strListText > strToFind.Left(nTextLength))
				{
					high = mid - 1;
				}
				else
				{
					nCurSel = mid; // found
					// Step back to find first matching
					while(1)
					{
						if (nCurSel > -1)
						{
							CListBox::GetText(nCurSel, strListText);
							strListText = strListText.Left(nTextLength);
						}
						if (strListText != strToFind.Left(nTextLength) || nCurSel < 0)
						{
							nCurSel++;
							break;
						}
						nCurSel--;
					}
					break;
				}
			}
			// Check if match found
			if (nCurSel >= 0)
			{
				break;
			}
			// Check 1 character less off rh end of lookup
			nTextLength--;
		}
		while(nTextLength);
	}

	strToFind = strInput.Left(nTextLength);
	if (nTextLength > 0)
	{
		// Only if match found
		SendMessage(WM_SETREDRAW, FALSE, 0);
		CListBox::SetCurSel(nCurSel);
		CListBox::SetTopIndex(nCurSel);
		SendMessage(WM_SETREDRAW, TRUE, 0);
	}
	return nCurSel;
}


 // ---------------------------------------------------------------------------
int CTabbedAlphaListBox::GetCurrentIndex(void)
{
	int nCurSel = GetCurSel();
	if (nCurSel < m_nIndexList.GetSize() && nCurSel > -1)
	{
		return m_nIndexList.GetAt(nCurSel);
	}
	else
	{
		return nCurSel;
	}
}
