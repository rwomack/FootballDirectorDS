

/*

	Class - CScreen14

	Employee contract Negotiation Dialog Box class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PlayerNameTextRegion.h"
#include "Screen14.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen14 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::CScreen14Dlg
	Access:    	public 
	Parameter: 	CEmployee * a_pEmployee
	Parameter: 	CString a_strPrompt
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen14Dlg::CScreen14Dlg(CEmployee* _pEmployee, CString _strPrompt, CWnd* pParent /*=null*/)
    : m_pCurrentEmployee(_pEmployee)
	, CBitmapDialog(CScreen14Dlg::IDD, pParent)
{
	m_strPrompt = _strPrompt;
	m_NegotiationsEnded = false;
}


BEGIN_MESSAGE_MAP(CScreen14Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_MAKEOFFER, OnMakeOffer)
	ON_COMMAND(ID_ACTION_CANCEL, OnCancel)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen14 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_EmployeeResponse.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_EmployeeResponse.OnPaint(CBitmapDialog::GetMemDC());

	if (m_NegotiationsEnded == false)
	{
		m_CurrentTextSelector.OnPaint();
		m_TextSelector[eWAGE].OnPaint();
		m_TextSelector[eCONTRACT].OnPaint();
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen14Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN14, IDS_SCREEN14);
	m_TextSelector[eWAGE].DoInitialise(TOPY + 26, IDS_WEEKLYWAGE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eWAGE].SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eCONTRACT].DoInitialise(TOPY + 50, IDS_CONTRACTENDDATE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eCONTRACT].SetStyle(COL_ALIGNC | COL_DATE);
	m_TextSelector[eCONTRACT].DoAddDates();
	m_NameText.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentEmployee);
	m_pCurrentEmployee->DoGenerateRandomContract(m_ContractRequired, m_pCurrentEmployee->GetClub());
	m_TextSelector[eCONTRACT].SetCurSel(m_ContractRequired.GetContractLength());
	m_TextSelector[eWAGE].SetCurValue(m_ContractRequired.GetWage());

	// Employee's response message display
	m_EmployeeResponse.SetDrawInfo(this, 0x1000, CRect(5, 170, 236, 232));						
	m_EmployeeResponse.AddString(m_strPrompt);

	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);
	CBitmapDialog::LoadMenu(IDR_MAKEOFFERMENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnEndNegotiations
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnEndNegotiations()
{
	m_NegotiationsEnded = true;
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
		switch (nChar)
		{
			case VK_UP:
			case VK_DOWN:
				m_EmployeeResponse.OnKeyDown(nChar, nRepCnt, nFlags);
				break;

			case VK_F23:
			case VK_RETURN:
				OnOK();
				break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
		m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnCancel()
{
	m_pCurrentEmployee->SetContractRenewalStatus(CLUBREFUSED);
	OnEndNegotiations();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
		switch (ButtonPressed)
		{
			case eWAGEDOWN:
			case eWAGEUP:
				m_TextSelector[eWAGE].OnLButtonUp(ButtonPressed);
				break;
			case eCONTRACTDOWN:
			case eCONTRACTUP:
				m_TextSelector[eCONTRACT].OnLButtonUp(ButtonPressed);
				break;
		}
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen14Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen14Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_NameText.MoveWindow();
		m_TextSelector[eWAGE].MoveWindow();
		m_TextSelector[eCONTRACT].MoveWindow();

		CRect theWindow;
		GetClientRect(&theWindow);
		m_EmployeeResponse.MoveWindow(CRect(4, TOPY + 72, theWindow.right - 4, theWindow.bottom - 4));
	}

	Sleep(1);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen14::OnOfferButton
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen14::OnMakeOffer()
{
	CContract ContractOffered;
	ContractOffered.SetWage(m_TextSelector[eWAGE].GetCurValue());
	ContractOffered.SetContractLength(m_TextSelector[eCONTRACT].GetCurSel());

	eContractOfferResponse eResponse = m_pCurrentEmployee->TryAcceptContract(m_ContractRequired, m_pCurrentEmployee->GetClub(), ContractOffered);
	m_EmployeeResponse.DoAddBlankLine();
	if (eResponse == ACCEPTED)
	{
		m_EmployeeResponse.AddString(m_pCurrentEmployee->szContractResponse(eResponse, ContractOffered, m_pCurrentEmployee->GetClub()), BLUE);
		OnEndNegotiations();
	}
	else
	{
		m_EmployeeResponse.AddString(m_pCurrentEmployee->szContractResponse(eResponse, ContractOffered, m_pCurrentEmployee->GetClub()), RED);
	}
	m_EmployeeResponse.ShowLastLine();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen14::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COwnedScreen14::OnInitDialog()
{
	BOOL bRet = CScreen14Dlg::OnInitDialog();
	m_pCurrentEmployee->DoGenerateRandomContract(m_ContractRequired, m_pCurrentEmployee->GetClub());
	m_TextSelector[eWAGE].SetCurValue(m_ContractRequired.GetWage());

	m_TextSelector[eCONTRACT].DoAddDates(m_pCurrentEmployee->GetContractEndDate().SeasonsRemaining());
	return bRet;
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen14::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNotOwnedScreen14::OnInitDialog()
{
	BOOL bRet = CScreen14Dlg::OnInitDialog();
	m_pCurrentEmployee->DoGenerateRandomContract(m_ContractRequired, *WorldData().GetCurrentUserClub());
	m_TextSelector[eWAGE].SetCurValue(m_ContractRequired.GetWage());
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen14::OnOfferButton
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNotOwnedScreen14::OnMakeOffer()
{
	CContract ContractOffered;
	ContractOffered.SetWage(m_TextSelector[eWAGE].GetCurValue());
	ContractOffered.SetContractLength(m_TextSelector[eCONTRACT].GetCurSel() + 1);

	eContractOfferResponse eResponse = m_pCurrentEmployee->TryAcceptContract(m_ContractRequired, m_pCurrentEmployee->GetClub(), ContractOffered);

	if (eResponse == ACCEPTED)
	{
		m_EmployeeResponse.AddString(m_pCurrentEmployee->szContractResponse(eResponse, ContractOffered, m_pCurrentEmployee->GetClub()), BLUE);
		WorldData().GetCurrentUserClub()->DoEmployStaff(m_pCurrentEmployee);
		OnEndNegotiations();
	}
	else
	{
		m_EmployeeResponse.AddString(m_pCurrentEmployee->szContractResponse(eResponse, ContractOffered, m_pCurrentEmployee->GetClub()), RED);
	}
	ReDraw();
}
