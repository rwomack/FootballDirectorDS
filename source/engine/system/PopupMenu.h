#if !defined(AFX_POPUP_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_)
#define AFX_POPUP_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_

// Popup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CPopupMenu
{
public:
	void					DoModal(int nResourceId, int nX, int nY, CWnd* pWnd, int nAlign = TPM_LEFTALIGN)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(nResourceId));
		CMenu* pPopup = menu.GetSubMenu(0);
    FDASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(nAlign, nX, nY, pWnd);
	}

	UINT 					DoGetSelection(int nResourceId, int nX, int nY, CWnd* pWnd, int nCurSel = -1, int nAlign = TPM_LEFTALIGN)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(nResourceId));
		CMenu* pPopup = menu.GetSubMenu(0);
    FDASSERT(pPopup != NULL);
		if (nCurSel > -1)
		{
			int nFound = 0;		// count not including seperators
			UINT nID, nFirstID, nLastID, nFoundID;
			int count = pPopup->GetMenuItemCount();
			for (int nLoopCount = 0; nLoopCount < count; nLoopCount++)
			{
				nID = pPopup->GetMenuItemID(nLoopCount);
				// Check its not a seperator
				if (nID > 0)
				{
					if (nLoopCount == 0)
					{
						nFirstID = nID;
					}
					else
					{
						nLastID = nID;
					}
					if (nFound == nCurSel)
					{
						nFoundID = nID;
					}
					nFound++;
				}
			}
			pPopup->CheckMenuRadioItem(nFirstID, nLastID, nFoundID, MF_CHECKED | MF_BYCOMMAND);
		}
		return pPopup->TrackPopupMenu(nAlign | TPM_RETURNCMD, nX, nY, pWnd);
	}
};
#endif
