
/*

	Screen59.cpp : Youth profile implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen59.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::CScreen59Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen59Dlg::CScreen59Dlg(CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen59Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::~CScreen59Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen59Dlg::~CScreen59Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen59Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen59 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen59Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::GetMemDC()->DrawSeperator(60);
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen59Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN59, IDS_SCREEN59);

	GetBasicDisplay()->DoAddTextRegions(eNUMTEXTREGION);
	CBitmapDialog::GetTextRegion(ePLAYERNAME)->DoInitialise(m_pCurrentYouth->GetName(), COL_ALIGNR, PosnColour[m_pCurrentYouth->GetSkills().GetPosition()]);
	CBitmapDialog::GetTextRegion(ePLAYERPOSITION)->DoInitialise(m_pCurrentYouth->GetSkills().PositionStr(), COL_ALIGNL, PosnColour[m_pCurrentYouth->GetSkills().GetPosition()]);

	CBitmapDialog::GetTextRegion(ePLAYERAGE)->DoInitialise(IDS_AGE, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(ePLAYERAGEVALUE)->DoInitialiseNum(m_pCurrentYouth->GetAge(), COL_ALIGNL, BLUE);

	CBitmapDialog::GetTextRegion(eHANDLING)->DoInitialise(IDS_HANDLING, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eTACKLING)->DoInitialise(IDS_TACKLING, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(ePASSING)->DoInitialise(IDS_PASSING, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eSHOOTING)->DoInitialise(IDS_SHOOTING, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eBSTFOOT)->DoInitialise(IDS_BESTFOOT, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(ePACE)->DoInitialise(IDS_PACE, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eTEMPER)->DoInitialise(IDS_TEMPER, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eFITNESS)->DoInitialise(IDS_FITNESS, COL_ALIGNR);
	CBitmapDialog::GetTextRegion(eMORALE)->DoInitialise(IDS_MORALE, COL_ALIGNR);

	CBitmapDialog::GetTextRegion(eHANDLINGVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetHandling(), COL_ALIGNL, BLUE);
 	CBitmapDialog::GetTextRegion(eTACKLINGVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetTackling(), COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(ePASSINGVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetPassing(), COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(eSHOOTINGVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetShooting(), COL_ALIGNL, BLUE);

	CString strPrint;
	if (m_pCurrentYouth->GetSkills().GetBestFoot() == 0)
	{
		strPrint.LoadString(IDS_LEFT);
	}
	else
	{
		strPrint.LoadString(IDS_RIGHT);
	}
	CBitmapDialog::GetTextRegion(eBESTFOOTVALUE)->DoInitialise(strPrint, COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(ePACEVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetPace(), COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(eTEMPERVALUE)->DoInitialise(m_pCurrentYouth->TemperamentStr(), COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(eFITNESSVALUE)->DoInitialiseNum(m_pCurrentYouth->GetSkills().GetFitness(), COL_ALIGNL, BLUE);
	CBitmapDialog::GetTextRegion(eMORALEVALUE)->DoInitialiseNum(m_pCurrentYouth->GetMorale(), COL_ALIGNL, BLUE);

	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::DoModal
	Access:    	public 
	Parameter: 	CYouth * _Youth
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CScreen59Dlg::DoModal(CYouth* _Youth)
{
	SetCurrentYouth(_Youth);
	return CDialog::DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen59Dlg::SetCurrentYouth
	Access:    	public 
	Parameter: 	CYouth * _Youth
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen59Dlg::SetCurrentYouth(CYouth* _Youth)			
{ 
	EXT_ASSERT_POINTER(_Youth, CYouth);
	m_pCurrentYouth = _Youth; 
	WorldData().SetCurrentYouth(_Youth);
}


/*------------------------------------------------------------------------------
Method:   	CScreen59Dlg::OnLButtonUp
Access:    	protected 
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen59Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CScreen59Dlg::OnLButtonDown
Access:    	protected 
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen59Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CScreen59Dlg::OnSize
Access:    	public 
Parameter: 	UINT nType
Parameter: 	int cx
Parameter: 	int cy
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen59Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);
	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);
		int CurrY = TOPY;
		CBitmapDialog::GetTextRegion(ePLAYERNAME)->MoveWindow(CRect(4, CurrY, GetCentreX() - 4, CurrY + 16));
		CBitmapDialog::GetTextRegion(ePLAYERPOSITION)->MoveWindow(CRect(GetCentreX() + 4, CurrY, theRect.right - 4, CurrY + 16));

		CurrY += GAPY + 2;
		int CurrX = GetCentreX() - 36;
		CBitmapDialog::GetTextRegion(ePLAYERAGE)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CBitmapDialog::GetTextRegion(ePLAYERAGEVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));

		CurrY += GAPY;
		int SaveCurrY = CurrY;
		CBitmapDialog::GetTextRegion(eHANDLING)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eTACKLING)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(ePASSING)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eSHOOTING)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eBSTFOOT)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));

		CurrY = SaveCurrY;
		CBitmapDialog::GetTextRegion(eHANDLINGVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eTACKLINGVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(ePASSINGVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eSHOOTINGVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eBESTFOOTVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 60, CurrY + 16));

		CurrY = SaveCurrY;
		CurrX = GetCentreX() + 60;
		CBitmapDialog::GetTextRegion(ePACE)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eTEMPER)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eFITNESS)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eMORALE)->MoveWindow(CRect(CurrX - 70, CurrY, CurrX, CurrY + 16));

		CurrY = SaveCurrY;
		CBitmapDialog::GetTextRegion(ePACEVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eTEMPERVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, theRect.right - 2, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eFITNESSVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
		CurrY += GAPY;
		CBitmapDialog::GetTextRegion(eMORALEVALUE)->MoveWindow(CRect(CurrX + 4, CurrY, CurrX + 24, CurrY + 16));
	}
	Sleep(1);
}
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CNotOwnedScreen59, CScreen59Dlg)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION_HIRE, OnButton1)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen59::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNotOwnedScreen59::OnInitDialog()
{
	CScreen59Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_SCREEN60NOTOWNEDMENU);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen59::OnButton1
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNotOwnedScreen59::OnButton1()
{
	WorldData().OnHireYouth();
	CDialog::OnOK ();
}
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(COwnedScreen59, CScreen59Dlg)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()

	ON_COMMAND(ID_ACTION_OFFERCONTRACT, OnButton1)
	ON_COMMAND(ID_ACTION_SACK, OnButton2)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen59::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COwnedScreen59::OnInitDialog()
{
	CScreen59Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_SCREEN59OWNEDMENU);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen59::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen59::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen59::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen59::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen59::OnButton1
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:	Promote youth to first team squad
------------------------------------------------------------------------------*/
void COwnedScreen59::OnButton1()
{
	theApp.OnPromoteYouth(TEXT(""));
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen59::OnButton2
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen59::OnButton2()
{
	if (WorldData().OnSackYouth() == true)
	{
		CDialog::OnOK ();
	}
}
