// Screen05Dlg.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"

#include "Screen05Base.h"
#include "Screen05Dlg.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen05Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::CScreen05Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen05Dlg::CScreen05Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen05Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::~CScreen05Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen05Dlg::~CScreen05Dlg() 
{
};


BEGIN_MESSAGE_MAP(CScreen05Dlg, CBitmapDialog)
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION1, OnViewDetails)
	ON_COMMAND(ID_ACTION3, OnSkillView)
	ON_COMMAND(ID_ACTION4, OnStatusView)
	ON_COMMAND(ID_ACTION5, OnContractView)
	ON_COMMAND(ID_ACTION6, OnStatsView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen05Dlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnSkillView
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnSkillView()
{
	m_Screen.OnSelchangeTab(eSKILL);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnStatusView
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnStatusView()
{
	m_Screen.OnSelchangeTab(eSTATUS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnContractView
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnContractView()
{
	m_Screen.OnSelchangeTab(eFINANCE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnStatsView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnStatsView()
{
	m_Screen.OnSelchangeTab(eSTATS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnViewDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnViewDetails()
{
	m_Screen.OnViewPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::OnPaint(true);		// Draw bitmap here or column headers get overwritten
	m_Screen.OnDraw(CBitmapDialog::GetBasicDisplay());
	GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen05Dlg::OnInitDialog()
{
	BOOL bRet = CBitmapDialog::OnInitDialog(IDS_HELPSCREEN05, IDS_SCREEN05);
	m_Screen.SetGrid(&CBitmapDialog::GetGrid());
	m_Screen.DoInitialise(m_hWnd, this, GetBasicDisplay(), &CBitmapDialog::GetGrid());
	CBitmapDialog::LoadMenu(IDR_SCREEN05NOTMANAGEDMENU);
	m_Screen.GetGrid().SetStyle(LC_NOHIGHLIGHT);
	SetIsInitialised(true);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Screen.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Screen.OnLButtonDown(m_hWnd, nFlags, point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Screen.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);
	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		m_Screen.OnSize(theWindow);

		Sleep(1);
	}
}