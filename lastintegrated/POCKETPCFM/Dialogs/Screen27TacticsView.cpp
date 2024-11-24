
/*

	Screen27TacticsView.cpp: implementation for the Screen27TacticsView class.

	Football Director

	(c) Rw Software 2001 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "Screen27TacticsView.h"
#include "FootballDirector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CScreen27TacticsView, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::CScreen27TacticsView
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen27TacticsView::CScreen27TacticsView()
	: CBitmapFormView(CScreen27TacticsView::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::~CScreen27TacticsView
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27TacticsView::~CScreen27TacticsView()
{
}


BEGIN_MESSAGE_MAP(CScreen27TacticsView, CBitmapFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_CHECK2701, OnButtonCounterAttack)
	//ON_BN_CLICKED(IDC_CHECK2702, OnButtonOffsideTrap)

	ON_COMMAND(ID_ACTION1, OnAction1Clicked)
	ON_COMMAND(ID_ACTION2, OnAction2Clicked)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreen27TacticsView message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnActionClicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN27TACTICVIEWMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnAction1Clicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnAction1Clicked()
{
// 	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
// 	EXT_ASSERT(pButton);
// 	if (pButton->GetCheck() == BST_CHECKED)
// 	{
// 		pButton->SetCheck(BST_UNCHECKED);
// 		WorldData().GetCurrentUser()->SetCounterAttack(false);
// 	}
// 	else
// 	{
// 		pButton->SetCheck(BST_CHECKED);
// 		WorldData().GetCurrentUser()->SetCounterAttack(true);
// 	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnAction2Clicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnAction2Clicked()
{
// 	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
// 	EXT_ASSERT(pButton);
// 	if (pButton->GetCheck() == BST_UNCHECKED)
// 	{
// 		pButton->SetCheck(BST_CHECKED);
// 		WorldData().GetCurrentUser()->SetOffsideTrap(true);
// 	}
// 	else
// 	{
// 		pButton->SetCheck(BST_UNCHECKED);
// 		WorldData().GetCurrentUser()->SetOffsideTrap(false);
// 	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnButtonCounterAttack
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnButtonCounterAttack()
{
// 	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
// 	EXT_ASSERT(pButton);
// 	if (pButton->GetCheck() == BST_CHECKED)
// 	{
// 		WorldData().GetCurrentUser()->SetCounterAttack(true);
// 	}
// 	else
// 	{
// 		WorldData().GetCurrentUser()->SetCounterAttack(false);
// 	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnButtonOffsideTrap
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnButtonOffsideTrap()
{
// 	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
// 	EXT_ASSERT(pButton);
// 	if (pButton->GetCheck() == BST_CHECKED)
// 	{
// 		WorldData().GetCurrentUser()->SetOffsideTrap(true);
// 	}
// 	else
// 	{
// 		WorldData().GetCurrentUser()->SetOffsideTrap(false);
// 	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnActivateView
	Access:    	public 
	Parameter: 	BOOL bActivate
	Parameter: 	CView * pActivateView
	Parameter: 	CView * pDeactiveView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate == TRUE)
	{
// 		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
// 		EXT_ASSERT_POINTER(pButton, CButton);
// 		pButton->SetCheck(WorldData().GetCurrentUser()->GetCounterAttack());
// 		pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
// 		EXT_ASSERT_POINTER(pButton, CButton);
// 		pButton->SetCheck(WorldData().GetCurrentUser()->GetOffsideTrap());
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnDraw
	Access:    	public 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnPaint()
{
	CBitmapFormView::OnInitRedrawArea();
	m_CurrentTextSelector.OnPaint();
	m_TextSelector[eSTYLEOFPLAY].OnPaint();
	m_TextSelector[eMARKING].OnPaint();
	m_TextSelector[eAGGRESSION].OnPaint();
	m_CounterAttackToggle.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Match Tactics"), &theApp.GetBasicDisplay(), IDS_SCREEN27TACTICS);
	m_CounterAttackToggle.Create(NULL, _T(""), WS_VISIBLE, CRect(8, 160, 24, 176), this, 5000, 0);
	m_CounterAttackToggle.OnInitialUpdate(&theApp.GetBasicDisplay(), true, m_hWnd);
#include "TacticsInit.cpp"
	CRect theRect;
	m_CounterAttackToggle.GetWindowRect(theRect);
	int ab = 0;
}

 
/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_TextSelector[eSTYLEOFPLAY].OnLButtonUp(ButtonPressed);
	m_TextSelector[eMARKING].OnLButtonUp(ButtonPressed);
	m_TextSelector[eAGGRESSION].OnLButtonUp(ButtonPressed);

	WorldData().GetCurrentUser()->SetCurrentStyleOfPlay(m_TextSelector[eSTYLEOFPLAY].GetCurSel());
	WorldData().GetCurrentUser()->SetCurrentMarkingStyle(m_TextSelector[eMARKING].GetCurSel());
	WorldData().GetCurrentUser()->SetCurrentAggression(m_TextSelector[eAGGRESSION].GetCurSel());
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
		case VK_SELECT:
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsView::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsView::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);
	if (CBitmapFormView::IsInitialised() == true)
	{
		m_TextSelector[eSTYLEOFPLAY].MoveWindow();
		m_TextSelector[eMARKING].MoveWindow();
		m_TextSelector[eAGGRESSION].MoveWindow();
		GetBasicDisplay()->GetTextRegion(m_Text1ID)->MoveWindow(CRect(26, 134, 144, 154));
		GetBasicDisplay()->GetTextRegion(m_Text2ID)->MoveWindow(CRect(26, 158, 144, 178));

		m_CounterAttackToggle.MoveWindow(CRect(8, 160, 24, 176));
		CRect theRect;
		m_CounterAttackToggle.GetWindowRect(theRect);
		int ab = 0;
	}
	Sleep(1);
}