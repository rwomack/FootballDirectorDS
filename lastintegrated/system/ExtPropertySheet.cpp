/*

	ExtProperty.cpp : implementation file

	(c) RW Software 2003

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "ExtPropertySheet.h"

IMPLEMENT_DYNAMIC(CExtPropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CExtPropertyPage, CPropertyPage)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()




/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPropertyPage::OnKeyDown(const UINT _Char, const UINT _RepCnt, const UINT _Flags)
{
	if (_Char != VK_F23)
	{
		CPropertyPage::OnKeyDown(_Char, _RepCnt, _Flags);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPropertyPage::OnKeyUp(UINT _Char, UINT _RepCnt, UINT _Flags)
{
	if (_Char != VK_F23)
	{
		CPropertyPage::OnKeyUp(_Char, _RepCnt, _Flags);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPropertyPage::OnOK()
{
   CExtPropertySheet* pSheet = DYNAMIC_DOWNCAST(CExtPropertySheet, GetParent());

   if(pSheet != null)
   {
      pSheet->EndDialog(IDOK);
   }

   CPropertyPage::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::OnCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPropertyPage::OnCancel()
{
   CExtPropertySheet* pSheet = DYNAMIC_DOWNCAST(CExtPropertySheet, GetParent());

   if(pSheet != null)
   {
      pSheet->EndDialog(IDCANCEL);
   }

   CPropertyPage::OnCancel();
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::OnSetActive
	Access:    	virtual public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CExtPropertyPage::OnSetActive()
{
	BOOL bRet = CPropertyPage::OnSetActive();
#ifdef POCKETPC
	SHSipPreference(m_hWnd,SIP_UNCHANGED);
	SHSipPreference(m_hWnd,SIP_UP);
#endif
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertyPage::PreTranslateMessage
	Access:    	virtual public 
	Parameter: 	MSG* _pMsg
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CExtPropertyPage::PreTranslateMessage(MSG* _pMsg)
{
#ifdef POCKETPC
	// SHInputDialog must be first!
	SHInputDialog(_pMsg->hwnd, _pMsg->message, _pMsg->wParam);
#endif
	return CPropertyPage::PreTranslateMessage(_pMsg);
}
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CExtPropertySheet, CPropertySheet)


/*------------------------------------------------------------------------------
	Method:   	CExtPropertySheet::SetTabText
	Access:    	protected 
	Parameter: 	const int _TabID
	Parameter: 	const int _StringID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPropertySheet::SetTabText(const int _TabID, const int _StringID)
{
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	CString pszString;

	pszString.LoadString(_StringID);
	tcItem.pszText = pszString.GetBuffer(256);
	ASSERT(GetTabControl() != null);
	GetTabControl()->SetItem(_TabID, &tcItem);
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertySheet::ContinueModal
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CExtPropertySheet::ContinueModal()
{
   if (!CWnd::ContinueModal())
   {
	   return FALSE;
   }
   return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPropertySheet::PreTranslateMessage
	Access:    	virtual public 
	Parameter: 	MSG* _pMsg
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CExtPropertySheet::PreTranslateMessage(MSG* _pMsg)
{
#ifdef POCKETPC
	// SHInputDialog must be first!
	SHInputDialog(_pMsg->hwnd,_pMsg->message,_pMsg->wParam);
#endif
	return CPropertySheet::PreTranslateMessage(_pMsg);
}

