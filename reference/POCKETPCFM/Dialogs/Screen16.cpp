// Screen16.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "PlayerNameTextRegion.h"
#include "Screen16.h"


IMPLEMENT_DYNAMIC(CScreen16Dlg, CDialog)


 /////////////////////////////////////////////////////////////////////////////
// CScreen16Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::CScreen16Dlg
	Access:    	public 
	Parameter: 	CPlayer * pPlayer
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen16Dlg::CScreen16Dlg(CPlayer* pPlayer, CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen16Dlg::IDD, pParent)
{
	m_pCurrentPlayer = pPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::DoDataExchange
	Access:    	virtual protected 
	Parameter: 	CDataExchange * pDX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (pDX->m_bSaveAndValidate == 1)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1603);
		EXT_ASSERT_POINTER(pButton, CButton);
		if (pButton->GetCheck() == BST_CHECKED)
		{
			m_pCurrentPlayer->GetTransferStatus().SetAskingPrice(m_TextSelector[eFEE].GetCurValue());
		}
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1601);
		EXT_ASSERT_POINTER(pButton, CButton);
		if (m_pCurrentPlayer->GetTransferStatus().IsRejectAllOffers() == true)
		{
			pButton->SetCheck(BST_CHECKED);
		}
		else
		{
			pButton->SetCheck(BST_UNCHECKED);
		}

		pButton = (CButton*)GetDlgItem(IDC_CHECK1603);
		EXT_ASSERT_POINTER(pButton, CButton);
		if (m_pCurrentPlayer->GetTransferStatus().IsTransferListed() == true)
		{
			pButton->SetCheck(BST_CHECKED);
		}
		else
		{
			pButton->SetCheck(BST_UNCHECKED);
		}

		pButton = (CButton*)GetDlgItem(IDC_CHECK1604);
		EXT_ASSERT_POINTER(pButton, CButton);
		if (m_pCurrentPlayer->GetTransferStatus().IsLoanListed() == true)
		{
			pButton->SetCheck(BST_CHECKED);
		}
		else
		{
			pButton->SetCheck(BST_UNCHECKED);
		}
	}
}


BEGIN_MESSAGE_MAP(CScreen16Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_CHECK1601, OnButtonRejectAllOffers)
	//ON_BN_CLICKED(IDC_CHECK1602, OnButtonRejectOffersBelow)
	ON_BN_CLICKED(IDC_CHECK1603, OnButtonTransferList)
	ON_BN_CLICKED(IDC_CHECK1604, OnButtonLoanList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen16 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonRejectAllOffers
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonRejectAllOffers()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1601);
	EXT_ASSERT_POINTER(pButton, CButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		m_pCurrentPlayer->GetTransferStatus().SetRejectAllOffers(true);
	}
	else
	{
		m_pCurrentPlayer->GetTransferStatus().SetRejectAllOffers(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonRejectOffersBelow
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonRejectOffersBelow()
{
/*	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1602);
	EXT_ASSERT_POINTER(pButton, CButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		m_pCurrentPlayer->GetTransferStatus().SetRejectOffersBelow(true);
	}
	else
	{
		m_pCurrentPlayer->GetTransferStatus().SetRejectOffersBelow(false);
	}*/
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonTransferList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonTransferList()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1603);
	EXT_ASSERT_POINTER(pButton, CButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		m_pCurrentPlayer->DoUpdateTransferStatus(true, m_TextSelector[eFEE].GetCurValue());
	}
	else
	{
		m_pCurrentPlayer->DoUpdateTransferStatus(false, m_TextSelector[eFEE].GetCurValue());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonLoanList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonLoanList()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1604);
	EXT_ASSERT_POINTER(pButton, CButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		m_pCurrentPlayer->GetTransferStatus().SetLoanListed(true);
	}
	else
	{
		m_pCurrentPlayer->GetTransferStatus().SetLoanListed(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen16Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN16, IDS_SCREEN16);

	m_TextSelector[eFEE].DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eFEE].GetTextRegion()->SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eFEE].SetCurValue(m_pCurrentPlayer->GetSellingPrice());
	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);

	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonOk
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonOk()
{
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnButtonCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnButtonCancel()
{
	CDialog::OnCancel();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	//m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_F23:
		case VK_RETURN:
			OnOK();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
//	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	switch (ButtonPressed)
	{
		case eFEEDOWN:
		case eFEEUP:
			m_TextSelector[eFEE].OnLButtonUp(ButtonPressed);
			break;

		case eCANCEL:
			OnButtonCancel();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen16Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen16Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		m_PlayerName.MoveWindow();
		m_TextSelector[eFEE].MoveWindow(GetCentreX() - 50, TOPY + 40);

		CRect theWindow;
		GetClientRect(&theWindow);
		int ButtonX = theWindow.right - BUTTON5W - 10;
		int ButtonY = theWindow.bottom - BUTTON5H - 4;
		GetBasicDisplay()->DoMoveButton(eCANCEL, CRect(ButtonX, ButtonY, ButtonX + BUTTON5W, ButtonY + BUTTON5H));
	}
	Sleep(1);
}