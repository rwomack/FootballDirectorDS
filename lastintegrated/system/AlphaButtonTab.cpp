#include "stdafx.h"

#include "TabbedAlphaListBox.h"
#include "AlphaButtonTab.h"



 // ---------------------------------------------------------------------------
void CAlphaButtonTab::DoInitialise(int nFirstTabId, int nTopY, CWnd* wndParent)
{
	ASSERT(nTopY < 320);
	m_nTopY = nTopY;
	int nStaticId = nFirstTabId;

	CRect StaticRect(GetX(0) + 1, nTopY + 1, GetX(1) - 1, nTopY + 18);
	for (int n = 0; n < GetItemCount(); n++)
	{
		m_szStr[n] = GetTabCharStr(n);
		m_szStr[n].MakeLower();
		StaticRect.left = GetX(n) + 1;
		StaticRect.right = GetX(n + 1);
		m_staticAlphaButton[n].OnCreate(14, BLACK, WHITE, m_szStr[n], StaticRect, wndParent, nStaticId++, SS_NOTIFY | SS_CENTER);
	}
	SetCurSel(0);
}


 // ---------------------------------------------------------------------------
void CAlphaButtonTab::OnDraw(CDC* pDC)
{
	CPoint nPoint(0, m_nTopY);
	pDC->MoveTo(nPoint);
	nPoint.x = 240;
	pDC->LineTo(nPoint);

	nPoint.x = 0;
	nPoint.y = m_nTopY + 18;
	pDC->MoveTo(nPoint);
	nPoint.x = 240;
	pDC->LineTo(nPoint);

	for (int n = 0; n < GetItemCount(); n++)
	{
		nPoint.x = GetX(n);
		nPoint.y = m_nTopY;
		pDC->MoveTo(nPoint);
		nPoint.y = m_nTopY + 18;
		pDC->LineTo(nPoint);
	}
}


 // ---------------------------------------------------------------------------
UInt16 CAlphaButtonTab::GetWhichTab(CString& szStr)
{
	UInt16 nRet = 0xffff;
	CString szStringToFind = szStr.Left(1);
	szStringToFind.MakeUpper();
	for (int n = 0; n < GetItemCount(); n++)
	{
		if (GetTabCharStr(n).FindOneOf(szStringToFind) != -1)
		{
			nRet = n;
			break;
		}
	}
	if (nRet == 0xffff)
	{
		// Not an alpha char so return tab 0
		nRet = 0;
	}
	return nRet;
}


 // ---------------------------------------------------------------------------
CString CAlphaButtonTab::GetTabCharStr(UInt16 nTabNum)
{
	CString szRet;
	switch(nTabNum)
	{
		case 0:
			szRet = _T("#ab");
			break;
		case 1:
			szRet = _T("cde");
			break;
		case 2:
			szRet = _T("fgh");
			break;
		case 3:
			szRet = _T("ijk");
			break;
		case 4:
			szRet = _T("lmn");
			break;
		case 5:
			szRet = _T("opq");
			break;
		case 6:
			szRet = _T("rst");
			break;
		case 7:
			szRet = _T("uvw");
			break;
		case 8:
			szRet = _T("xyz");
			break;
	}
	szRet.MakeUpper();
	return szRet;
}


 // ---------------------------------------------------------------------------
void CAlphaButtonTab::DoResetClickCount()
{
	m_nClickCount = 0;
	if (m_nCurSel == 0)
	{
		// Skip #
		m_nClickCount = 1;
	}
}


 // ---------------------------------------------------------------------------
void CAlphaButtonTab::DoIncrementClickCount()
{
	m_nClickCount++;

	CString szRet = GetTabCharStr(m_nCurSel);
	if (m_nClickCount == szRet.GetLength())
	{
		DoResetClickCount();
	}
}


 // ---------------------------------------------------------------------------
CString CAlphaButtonTab::OnClicked(UInt16 nTabNum)
{
	if (GetCurSel() != nTabNum)
	{
		SetCurSel(nTabNum);
	}
	else
	{
		DoIncrementClickCount();
	}
	CString szRet = GetTabCharStr(m_nCurSel);
	return szRet.GetAt(m_nClickCount);
}


 // ---------------------------------------------------------------------------
int CAlphaButtonTab::GetX(int nTab)
{
	static int m_nX [] =
	{
		0, 26, 53, 80, 107,
		133, 160, 187, 214, 240
	};
	return m_nX[nTab];
}


// ---------------------------------------------------------------------------
void CAlphaButtonTab::SetCurSelListStart(CString& strFirstItem)
{
	SetCurSel(GetWhichTab(strFirstItem));
}


 // ---------------------------------------------------------------------------
void CAlphaButtonTab::SetCurSel(const UInt16 nCurSel)
{
	m_nCurSel = nCurSel;
	DoResetClickCount();

	// Highlight current tab button
	m_staticAlphaButton[m_nCurSel].SetColors(RGB(0, 48, 152), WHITE);
	for (int n = 0; n < GetItemCount(); n++)
	{
		if (n != m_nCurSel)
		{
			m_staticAlphaButton[n].SetColors(WHITE, BLACK);
		}
		m_staticAlphaButton[n].RedrawWindow();
	}
}
