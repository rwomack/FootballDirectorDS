
/*

	ExtTabCtrl.cpp: implementation for the Extended TabCtrl class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/

#include "stdafx.h"

#include "ExtTabCtrl.h"


IMPLEMENT_DYNAMIC(CExtTabCtrl, CTabCtrl)


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::CExtTabCtrl
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtTabCtrl::CExtTabCtrl()
	: m_ParentWnd(NULL)
	, m_bFontCreated(false)

{
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::~CExtTabCtrl
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtTabCtrl::~CExtTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CExtTabCtrl, CTabCtrl)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::GetParentWnd
	Access:    	public 
	Returns:   	CWnd*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWnd* CExtTabCtrl::GetParentWnd()
{
	return m_ParentWnd;
}


int CExtTabCtrl::GetWidth()
{
	return m_Width;
}

/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	int _ResourceID
	Parameter: 	int _Width
	Parameter: 	int * _PageArray
	Parameter: 	int _NumPages
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width, int* _PageArray, int _NumPages)
{
	OnInitialUpdate(_ParentWnd, _ResourceID, _Width);
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	CString pstrString;
	for (int LoopCount = 0; LoopCount < _NumPages; LoopCount++)
	{
		pstrString.LoadString(*_PageArray++);
		tcItem.pszText = pstrString.GetBuffer(256);
		InsertItem(LoopCount, &tcItem);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	int _ResourceID
	Parameter: 	int _Width
	Parameter: 	CStringArray & _PageArray
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width, CStringArray& _PageArray)
{
	OnInitialUpdate(_ParentWnd, _ResourceID, _Width);
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	for (int LoopCount = 0; LoopCount < _PageArray.GetSize(); LoopCount++)
	{
		tcItem.pszText = _PageArray.GetAt(LoopCount).GetBuffer(256);
		InsertItem(LoopCount, &tcItem);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	int _ResourceID
	Parameter: 	int _Width
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width)
{
	EXT_ASSERT_POINTER(_ParentWnd, CWnd);
	m_ParentWnd = _ParentWnd;
	SubclassDlgItem(_ResourceID, m_ParentWnd);
	EXT_ASSERT(_Width > 0);
	m_Width = _Width;
	SetFontSize(14);
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int iCurSel = GetCurSel();
	switch (nChar)
	{
		case VK_1:
		case VK_NUMPAD1:
			if (iCurSel < GetItemCount() - 1)
			{
				iCurSel++;
			}
			else
			{
				iCurSel = 0;
			}
			break;

		case VK_3:
		case VK_NUMPAD3:
			if (iCurSel > 0)
			{
				iCurSel--;
			} 
			else
			{
				iCurSel = GetItemCount() - 1;
			}
			break;
	}
	SetCurSel(iCurSel);
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (m_ParentWnd != NULL)
	{
		CRect theWindow;
		m_ParentWnd->GetClientRect(&theWindow);
		int CentreX = theWindow.right / 2;
		CTabCtrl::MoveWindow(CentreX - m_Width / 2, theWindow.bottom - TABHEIGHT - 2, CentreX + m_Width / 2, theWindow.bottom - 2);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtTabCtrl::SetFontSize
	Access:    	public 
	Parameter: 	const int _iPoints
	Parameter: 	const int _nWeight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtTabCtrl::SetFontSize(const int _iPoints, const int _nWeight /*= FW_MEDIUM*/)
{
	EXT_ASSERT_POINTER(CWnd::m_hWnd,  HWND);
	if (m_bFontCreated != false)
	{
		m_Font.DeleteObject();
	}
	m_Font.CreateFont(_iPoints, 0, 0, 0, _nWeight, FALSE, FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_bFontCreated = true;
	SetFont(&m_Font);
}
