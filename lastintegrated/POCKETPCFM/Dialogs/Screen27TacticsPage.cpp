// Screen27.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "HumanManager.h"
#include "Screen27TacticsPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen27TacticsPage property page


IMPLEMENT_DYNCREATE(CScreen27TacticsPage, CPropertyPage)

 //---------------------------------------------------------------------------
CScreen27TacticsPage::CScreen27TacticsPage() :
    CBitmapPropertyPage(CScreen27TacticsPage::IDD)
{
}


 //---------------------------------------------------------------------------
CScreen27TacticsPage::~CScreen27TacticsPage()
{
}


BEGIN_MESSAGE_MAP(CScreen27TacticsPage, CBitmapPropertyPage)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK2701, OnButtonPlayOnTheBreak)
	ON_BN_CLICKED(IDC_CHECK2702, OnButtonOffsideTrap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27 message handlers


//---------------------------------------------------------------------------------------
void CScreen27TacticsPage::OnButtonPlayOnTheBreak()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
	ASSERT(pButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().SetPlayOnTheBreak(true);
	}
	else
	{
		WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().SetPlayOnTheBreak(false);
	}
}


//---------------------------------------------------------------------------------------
void CScreen27TacticsPage::OnButtonOffsideTrap()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
	ASSERT(pButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().SetOffsideTrap(true);
	}
	else
	{
		WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().SetOffsideTrap(false);
	}
}


 //---------------------------------------------------------------------------
void CScreen27TacticsPage::OnPaint()
{
	CBitmapPropertyPage::OnInitRedrawArea();
//	CBitmapPropertyPage::OnPaint();
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();

#include "Screen27TacticsOnPaint.cpp"
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsPage::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27TacticsPage::OnInitDialog()
{
	m_Static270101.OnCreate(IDC_STATIC270101, this, TITLEFONTSIZE, DARKGREEN, WHITE, IDS_STYLEOFPLAY, TITLEFONTWEIGHT);
	m_Static270102.OnCreate(IDC_STATIC270102, this, TITLEFONTSIZE, DARKGREEN, WHITE, IDS_MARKSTYLE, TITLEFONTWEIGHT);
	m_Static270103.OnCreate(IDC_STATIC270103, this, TITLEFONTSIZE, DARKGREEN, WHITE, IDS_PLAYONBREAK, TITLEFONTWEIGHT);
	m_Static270104.OnCreate(IDC_STATIC270104, this, TITLEFONTSIZE, DARKGREEN, WHITE, IDS_OFFSIDETRAP, TITLEFONTWEIGHT);
	m_Static270105.OnCreate(IDC_STATIC270105, this, TITLEFONTSIZE, DARKGREEN, WHITE, IDS_AGGRESSION, TITLEFONTWEIGHT);

	uint theButtons[] =
	{
		IDR_PNGSCREEN27LEFT, IDR_PNGSCREEN27RIGHT,	 
		IDR_PNGSCREEN27LEFT, IDR_PNGSCREEN27RIGHT, 
		IDR_PNGSCREEN27LEFT,  IDR_PNGSCREEN27RIGHT
	};
	GetBasicDisplay().DoAddButtons(&theButtons[0], this);

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
	pButton->SetCheck(WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().GetPlayOnTheBreak());

	pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
	pButton->SetCheck(WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().GetOffsideTrap());
    return CBitmapPropertyPage::OnInitDialog(TEXT("Match Tactics"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsPage::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapPropertyPage::GetBasicDisplay().OnLButtonDown(nFlags, point);
	CBitmapPropertyPage::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsPage::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsPage::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (CBitmapPropertyPage::GetBasicDisplay().OnLButtonUp(nFlags, point))
	{
		case eSTYLEOFPLAYDOWN:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoLowerDefenceStyle();
			break;
		case eSTYLEOFPLAYUP:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoRaiseDefenceStyle();
			break;
		case eMARKINGDOWN:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoLowerLongBallStyle();
			break;
		case eMARKINGUP:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoRaiseLongBallStyle();
			break;
		case eAGGRESSIONDOWN:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoLowerAggression();
			break;
		case eAGGRESSIONUP:
			WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().DoRaiseAggression();
			break;
	}
	CBitmapPropertyPage::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsPage::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsPage::OnSize(UINT nType, int cx, int cy)
{
	CBitmapPropertyPage::OnSize(nType, cx, cy);
	CBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN27TACTICS);

	CRect theWindow;
	GetClientRect(&theWindow);
	if (theWindow.bottom != 0)
	{
		m_Button1X = 120 - eUPDOWNWIDTH;
		m_Button2X = 240 - eUPDOWNWIDTH;
		GetBasicDisplay().DoMoveButton(eSTYLEOFPLAYDOWN, CRect(m_Button1X, BUTTON1Y, m_Button1X + BUTTON1W, BUTTON1Y + BUTTON1H));
		GetBasicDisplay().DoMoveButton(eSTYLEOFPLAYUP, CRect(m_Button2X, BUTTON2Y, m_Button2X + BUTTON2W, BUTTON2Y + BUTTON2H));
		GetBasicDisplay().DoMoveButton(eAGGRESSIONDOWN, CRect(m_Button1X, BUTTON5Y, m_Button1X + BUTTON5W, BUTTON5Y + BUTTON5H));
		GetBasicDisplay().DoMoveButton(eAGGRESSIONUP, CRect(m_Button2X, BUTTON6Y, m_Button2X + BUTTON6W, BUTTON6Y + BUTTON6H));
		GetBasicDisplay().DoMoveButton(eMARKINGDOWN, CRect(m_Button1X, BUTTON3Y, m_Button1X + BUTTON3W, BUTTON3Y + BUTTON3H));
		GetBasicDisplay().DoMoveButton(eMARKINGUP, CRect(m_Button2X, BUTTON4Y, m_Button2X + BUTTON4W, BUTTON4Y + BUTTON4H));
	}

	Sleep(1);
}
