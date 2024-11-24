
/*

	Class - CPlayerContractDlg

	Youth and Player contract Negotiation Dialog Box base class

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
 */

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PlayerContractDlg.h"


BEGIN_MESSAGE_MAP(CPlayerContractDlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::CPlayerContractDlg
	Access:    	public 
	Parameter: 	CString strPrompt
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerContractDlg::CPlayerContractDlg(CString strPrompt, UINT nIDTemplate, CWnd* pParent /*= null*/)
	: CBitmapDialog(nIDTemplate, pParent)
{
	m_Response.AddString(strPrompt);
	m_NegotiationsEnded = false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnEndNegotiations
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnEndNegotiations()
{
	m_NegotiationsEnded = true;
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CPlayerContractDlg::OnInitDialog()
{
	m_TextSelector[eWAGE].DoInitialise(TOPY + 20, IDS_WEEKLYWAGE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eSIGNONFEE].DoInitialise(TOPY + 20 + 22, IDS_SIGNINGONFEE,this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eCONTRACT].DoInitialise(TOPY + 20 + 22 + 22, IDS_CONTRACTENDDATE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eWAGE].SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eSIGNONFEE].SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eCONTRACT].SetStyle(COL_ALIGNC | COL_DATE);
	m_TextSelector[eCONTRACT].DoAddDates();

	m_TextSelector[eCONTRACT].SetCurSel(m_ContractRequired.GetContractLength());
	m_TextSelector[eWAGE].SetCurValue(m_ContractRequired.GetWage());
	m_TextSelector[eSIGNONFEE].SetCurValue(m_ContractRequired.GetSignOnFee());
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);

	m_Response.SetDrawInfo(this, 0x1000, CRect(5, 170, 236, 232));						
	m_Response.AddString(m_Prompt, BLACK);
	
	CBitmapDialog::LoadMenu(IDR_MAKEOFFERMENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
		switch (nChar)
		{
			case VK_UP:
			case VK_DOWN:
				m_Response.OnKeyDown(nChar, nRepCnt, nFlags);
				break;

			case VK_F23:
			case VK_RETURN:
				OnOK();
				break;
		}
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
		CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnLButtonDown
	Access:    	virtual public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
		CBitmapDialog::ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnLButtonUp(UINT nFlags, CPoint point)
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
			case eSIGNONFEEDOWN:
			case eSIGNONFEEUP:
				m_TextSelector[eSIGNONFEE].OnLButtonUp(ButtonPressed);
				break;
			case eCONTRACTDOWN:
			case eCONTRACTUP:
				m_TextSelector[eCONTRACT].OnLButtonUp(ButtonPressed);
				break;
		}
		CBitmapDialog::ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnOfferContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnOfferContract()
{
	m_ContractOffered.SetWage(m_TextSelector[eWAGE].GetCurValue());
	m_ContractOffered.SetSignOnFee(m_TextSelector[eSIGNONFEE].GetCurValue());
	m_ContractOffered.SetContractLength(m_TextSelector[eCONTRACT].GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::OnPaint(true, false);
	m_Response.OnPaint(CBitmapDialog::GetMemDC());
	if (m_NegotiationsEnded == false)
	{
		m_CurrentTextSelector.OnPaint();
		m_TextSelector[eWAGE].OnPaint();
		m_TextSelector[eSIGNONFEE].OnPaint();
		m_TextSelector[eCONTRACT].OnPaint();
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);
	m_TextSelector[eWAGE].MoveWindow();
	m_TextSelector[eSIGNONFEE].MoveWindow();
	m_TextSelector[eCONTRACT].MoveWindow();

	CRect theWindow;
	GetClientRect(&theWindow);
	m_Response.MoveWindow(CRect(4, TOPY + 84, theWindow.right - 4, theWindow.bottom - 4));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerContractDlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerContractDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Response.OnVScroll(nSBCode, nPos, pScrollBar);
}
