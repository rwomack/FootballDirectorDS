
/*

	Screen60.cpp : Employee profile implementation file
	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "PopupPlayerMenu.h"
#include "FootballDirector.h"
#include "Screen60.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen60 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen60Dlg::CScreen60Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	: CBitmapDialog(IDD, pParent)
	Purpose:
------------------------------------------------------------------------------*/
CScreen60Dlg::CScreen60Dlg(CEmployee* _Employee, CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen60Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_Employee, CEmployee);
	m_pCurrentEmployee = _Employee;
}


BEGIN_MESSAGE_MAP(CScreen60Dlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(COwnedScreen60, CScreen60Dlg)
	ON_COMMAND(ID_ACTION_OFFERCONTRACT, OnButton1)
	ON_COMMAND(ID_ACTION_SACK, OnButton2)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CNotOwnedScreen60, CScreen60Dlg)
	ON_COMMAND(ID_ACTION_HIRE, OnButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen60 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen60Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::GetMemDC()->DrawSeperator(64);
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen60Dlg::SetpCurrentEmployee
	Access:    	public 
	Parameter: 	CEmployee * pEmployee
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::SetpCurrentEmployee(CEmployee* pEmployee)		
{ 
	m_pCurrentEmployee = pEmployee; 
}


/*------------------------------------------------------------------------------
	Method:   	CScreen60Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen60Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN60, IDS_SCREEN60);

	WorldData().SetCurrentEmployee(m_pCurrentEmployee);

	GetBasicDisplay()->DoAddTextRegions(eNUMTEXTREGION);
	CBitmapDialog::GetTextRegion(eNAME)->DoInitialise(m_pCurrentEmployee->GetName(), COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eROLE)->DoInitialise(m_pCurrentEmployee->TypeTextStr(), COL_ALIGNL, PosnColour[m_pCurrentEmployee->GetType()]);
	
	CBitmapDialog::GetTextRegion(eAGE)->DoInitialise(IDS_AGE, COL_ALIGNR);
	CString strPrint;
	strPrint.Format(_T("%d"), m_pCurrentEmployee->GetAge());
	CBitmapDialog::GetTextRegion(eAGEVALUE)->DoInitialise(strPrint, COL_ALIGNL, DARKGREEN);

	CBitmapDialog::GetTextRegion(eNATIONALITY)->DoInitialise(IDS_NATIONALITY, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eWAGE)->DoInitialise(IDS_WEEKLYWAGE, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eCONTRACT)->DoInitialise(IDS_CONTRACT, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eRATING)->DoInitialise(IDS_SKILL, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eRATINGVALUE)->DoInitialise(m_pCurrentEmployee->GetSkillStr(), COL_ALIGNL, DARKGREEN);

	CBitmapDialog::GetTextRegion(eNATIONALITYVALUE)->DoInitialise(m_pCurrentEmployee->NationalityStr(), COL_ALIGNL, DARKGREEN);
	CBitmapDialog::GetTextRegion(eWAGEVALUE)->DoInitialise(WorldData().GetCurrentUserClub()->GetForeignStr(m_pCurrentEmployee->GetWage()), COL_ALIGNL, DARKGREEN);
	CBitmapDialog::GetTextRegion(eCONTRACTVALUE)->DoInitialise(m_pCurrentEmployee->ContractStr(), COL_ALIGNL, DARKGREEN);

	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen60Dlg::CheckIfWillNegotiate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::CheckIfWillNegotiate()
{
}


/*------------------------------------------------------------------------------
Method:   	CScreen60Dlg::OnLButtonUp
Access:    	protected 
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CScreen60Dlg::OnLButtonDown
Access:    	protected 
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CScreen60Dlg::OnSize
Access:    	protected 
Parameter: 	UINT nType
Parameter: 	int cx
Parameter: 	int cy
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen60Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);
		int CurrY = TOPY;
		GetBasicDisplay()->DoMoveTextRegion(eNAME, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eROLE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
		
		CurrY += GAPY + 10;
		GetBasicDisplay()->DoMoveTextRegion(eAGE, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eAGEVALUE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
		CurrY += GAPY;
		GetBasicDisplay()->DoMoveTextRegion(eNATIONALITY, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eNATIONALITYVALUE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
		CurrY += GAPY;
		GetBasicDisplay()->DoMoveTextRegion(eWAGE, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eWAGEVALUE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
		CurrY += GAPY;
		GetBasicDisplay()->DoMoveTextRegion(eCONTRACT, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eCONTRACTVALUE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
		CurrY += GAPY;
		GetBasicDisplay()->DoMoveTextRegion(eRATING, CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		GetBasicDisplay()->DoMoveTextRegion(eRATINGVALUE, CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));
	}
	Sleep(1);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen60::CNotOwnedScreen60
	Access:    	public 
	Parameter: 	CEmployee * _Employee
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	: CScreen60Dlg(_Employee, pParent)
	Purpose:
------------------------------------------------------------------------------*/
CNotOwnedScreen60::CNotOwnedScreen60(CEmployee* _Employee, CWnd* pParent /*=null*/)
	: CScreen60Dlg(_Employee, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen60::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNotOwnedScreen60::OnInitDialog()
{
	CScreen60Dlg::OnInitDialog();
	if (WorldData().GetCurrentUserClub()->NeedEmployee(m_pCurrentEmployee->GetType(), false) == true)
	{
		CBitmapDialog::LoadMenu(IDR_SCREEN60NOTOWNEDMENU);
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen60::OnButton1
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNotOwnedScreen60::OnButton1()
{
	theApp.OnHireEmployee();
	OnOK();	// In case has been hired
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
Method:   	COwnedScreen60::COwnedScreen60
Access:    	public 
Parameter: 	CEmployee * _Employee
Parameter: 	CWnd * pParent
Returns:   	
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
COwnedScreen60::COwnedScreen60(CEmployee* _Employee, CWnd* pParent /*=null*/)
: CScreen60Dlg(_Employee, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen60::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COwnedScreen60::OnInitDialog()
{
	CScreen60Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_SCREEN60OWNEDMENU);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen60::OnButton1
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen60::OnButton1()
{
	if (m_pCurrentEmployee->WillNegotiateContract() != true)
	{
		CString str;
		str.Format(IDS_NOTNEGOTIATECONTRACT, m_pCurrentEmployee->GetName());
		AfxMessageBox(str, MB_OK | MB_ICONHAND);
	}
	else
	{
		theApp.OnNegotiateEmployeeContract();
	}
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen60::OnButton2
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen60::OnButton2()
{
	if (WorldData().OnSackEmployee() == true)
	{
		CDialog::OnOK();
	}
}
