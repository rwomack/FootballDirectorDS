#include "stdafx.h"

#include "DropList.h"


//---------------------------------------------------------------------
CDropList::CDropList()
{
	DoResetVariables();
}



//---------------------------------------------------------------------
CDropList::CDropList(UInt16 x, UInt16 y, bool bPrompt)
{
	DoResetVariables();
	m_InputX = x;
	m_InputY = y;
	m_bPrompt = bPrompt;
}


//---------------------------------------------------------------------
CDropList::~CDropList()
{
	m_fontTahoma.DeleteObject();
	m_fontTahomaBold.DeleteObject();
}

//---------------------------------------------------------------------
void CDropList::DoResetVariables()
{
	m_nCurSel = 0;
	m_bMenuLoaded = false;
	m_bBoldText = true;
	m_bPopupActive = false;
	m_bCurSelHasChanged = false;

	m_fontTahomaBold.CreateFont(13, 0, 0, 0, \
						  FW_MEDIUM, \
						  FALSE, FALSE, 0, \
						  ANSI_CHARSET, \
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
						  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
						  _T("Tahoma Bold"));
	m_fontTahoma.CreateFont(13, 0, 0, 0, \
						  FW_MEDIUM, \
						  FALSE, FALSE, 0, \
						  ANSI_CHARSET, \
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
						  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
						  _T("Tahoma"));
}


//---------------------------------------------------------------------
void CDropList::DoLoadMenu(UINT nID)
{
	if (m_bMenuLoaded == true)
	{
		m_Menu.DestroyMenu();
	}
	VERIFY(m_Menu.LoadMenu(nID));
	m_bMenuLoaded = true;
}


//---------------------------------------------------------------------
CMenu* CDropList::GetpPopup()
{
	ASSERT(m_bMenuLoaded == true);
	CMenu* pPopup = m_Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	return pPopup;
}


//---------------------------------------------------------------------
void CDropList::GetBulletInput(UInt16 x, UInt16 y, bool bPrompt)
{
	CMenu* pPopup = GetpPopup();
	int count = pPopup->GetMenuItemCount();
	int nFound = 0;		// count not including seperators
	UINT nID, nFirstID, nLastID, nFoundID;
	for (int nLoopCount = 0; nLoopCount < count; nLoopCount++)
	{
		nID = pPopup->GetMenuItemID(nLoopCount);
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
			if (nFound == GetCurSel())
			{
				nFoundID = nID;
			}
			nFound++;
		}
	}
	pPopup->CheckMenuRadioItem(nFirstID, nLastID, nFoundID, MF_CHECKED | MF_BYCOMMAND);
	DoGetInput(x, y, bPrompt, nFirstID);
}


//---------------------------------------------------------------------
void CDropList::DoGetInput(UInt16 x, UInt16 y, bool bPrompt, const UINT nFirstID)
{
	CMenu* pPopup = GetpPopup();
	m_bCurSelHasChanged = false;
	ASSERT(m_pParentWnd != NULL);
	if (m_bPrompt == true)
	{
		m_bPopupActive = true;
		m_pParentWnd->InvalidateRect(m_rectDisplay);
	}
	UINT nNewID = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, x, y, m_pParentWnd);
	m_bPopupActive = false;
	if (nNewID > 0)
	{
		SetCurSel(nNewID - nFirstID);
	}
	m_pParentWnd->InvalidateRect(m_rectDisplay);
}


//---------------------------------------------------------------------
void CDropList::GetInput(UInt16 x, UInt16 y, bool bPrompt)
{
	CMenu* pPopup = GetpPopup();
	int count = pPopup->GetMenuItemCount();
	int nFound = 0;		// count not including seperators
	UINT nID, nFirstID;
	CString strDescription;
	for (int nLoopCount = 0; nLoopCount < count; nLoopCount++)
	{
		nID = pPopup->GetMenuItemID(nLoopCount);
		if (nID > 0)
		{
			if (nLoopCount == 0)
			{
				nFirstID = nID;
			}
			if (nFound != GetCurSel())
			{
				pPopup->CheckMenuItem(nLoopCount, MF_UNCHECKED | MF_BYPOSITION);
			}
			else
			{
				pPopup->CheckMenuItem(nLoopCount, MF_CHECKED | MF_BYPOSITION);
			}
			nFound++;
		}
	}
	DoGetInput(x, y, bPrompt, nFirstID);
}


//---------------------------------------------------------------------
CString CDropList::GetCurSelStr()
{
	CString strDescription;
	if (m_bPopupActive == true)
	{
		strDescription.LoadString(IDS_SHOW);
	}
	else
	{
		CMenu* pPopup = GetpPopup();
		int count = pPopup->GetMenuItemCount();
		int nFound = 0;		// count not including seperators
		UINT nID;
		for (int nLoopCount = 0; nLoopCount < count; nLoopCount++)
		{
			nID = pPopup->GetMenuItemID(nLoopCount);
			if (nID > 0)
			{
				if (nFound == GetCurSel())
				{
					pPopup->GetMenuString(nLoopCount, strDescription, MF_BYPOSITION);
					break;
				}
				nFound++;
			}
		}
	}

	return strDescription;
}


//---------------------------------------------------------------------
UInt16 CDropList::GetCount(void)
{
	return GetpPopup()->GetMenuItemCount();
}


//---------------------------------------------------------------------
void CDropList::SetDrawInfo(CRect rectDisplay, CWnd* pParentWnd, UInt16 nCursel, int nBitmapID)
{
	ASSERT(pParentWnd != NULL);
	m_rectDisplay = rectDisplay;
	m_nIDResource = nBitmapID;
	m_nCurSel = nCursel;
	m_pParentWnd = pParentWnd;
	HDC dc = ::GetDC(m_pParentWnd->GetSafeHwnd());
	m_DIBSection.SetBitmap(dc, m_nIDResource, _T("PNG_BITMAP"));
	::ReleaseDC(m_pParentWnd->GetSafeHwnd(),dc);
}


//---------------------------------------------------------------------
void CDropList::SetCurSel(const UInt16 nCurSel)
{
	m_bCurSelHasChanged = true;
	m_nCurSel = nCurSel;
}


// Draw the text and the button on the rh side
//---------------------------------------------------------------------
void CDropList::OnDrawRight(CExtPaintDC* dc, CString strDescription)
{
	if (m_pParentWnd != NULL)
	{
		int nOldBkMode = dc->SetBkMode(TRANSPARENT);
		CFont* pfontCurrent;
		if (m_bBoldText == true)
		{
			pfontCurrent = dc->SelectObject(&m_fontTahomaBold);
		}
		else
		{
			pfontCurrent = dc->SelectObject(&m_fontTahoma);
		}
		dc->ExtTextOut(m_rectDisplay.left, m_rectDisplay.top + 1, 0, m_rectDisplay, strDescription, strDescription.GetLength(), NULL);
		dc->SetBkMode(nOldBkMode);
		m_DIBSection.DrawTransparent(dc->operator HDC(), m_rectDisplay.left + dc->GetTextExtent(strDescription).cx + 2, m_rectDisplay.top + GFXYOFFSET, -1, -1, RGB(255, 0, 255));
		dc->SelectObject(pfontCurrent);
	}
}


// Draw the text and the button on the rh side
//---------------------------------------------------------------------
void CDropList::OnDrawLeft(CExtPaintDC* dc, CString strDescription)
{
	if (m_pParentWnd != NULL)
	{
		int nOldBkMode = dc->SetBkMode(TRANSPARENT);
		CFont* pfontCurrent;
		if (m_bBoldText == true)
		{
			pfontCurrent = dc->SelectObject(&m_fontTahomaBold);
		}
		else
		{
			pfontCurrent = dc->SelectObject(&m_fontTahoma);
		}
		int nSaveLeft = m_rectDisplay.left;
		m_rectDisplay.left = m_rectDisplay.right - (dc->GetTextExtent(strDescription).cx + 10);
		dc->ExtTextOut(m_rectDisplay.left, m_rectDisplay.top + 1, 0, m_rectDisplay, strDescription, strDescription.GetLength(), NULL);
		dc->SetBkMode(nOldBkMode);
		m_DIBSection.DrawTransparent(dc->operator HDC(), m_rectDisplay.right - 7, m_rectDisplay.top + GFXYOFFSET, -1, -1, RGB(255, 0, 255));
		dc->SelectObject(pfontCurrent);
		m_rectDisplay.left = nSaveLeft;
	}
}


//---------------------------------------------------------------------
bool CDropList::DoInputSel(const bool bRadioCheck)
{
	if (bRadioCheck == true)
	{
		GetBulletInput(m_InputX, m_InputY, m_bPrompt);
	}
	else
	{
		GetInput(m_InputX, m_InputY, m_bPrompt);
	}
	return HasCurSelChanged();
}
