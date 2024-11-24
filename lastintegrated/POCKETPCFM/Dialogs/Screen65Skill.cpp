// Screen65Skill.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Math.h"
#include "Screen65PropertyPage.h"
#include "Screen65Skill.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen65Skill property page

IMPLEMENT_DYNCREATE(CScreen65Skill, CPropertyPage)

CScreen65Skill::CScreen65Skill() :
	CMatchBitmapPropertyPage(CScreen65Skill::IDD)
{
}

CScreen65Skill::~CScreen65Skill()
{
}


BEGIN_MESSAGE_MAP(CScreen65Skill, CBitmapPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65Skill message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen65Skill::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Skill::OnPaint()
{
	CFixture& Fixture = CMatchBitmapPropertyPage::m_pCurrentMatchPlay->GetFixture();
	CString str;
	CRect bar;

	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CBitmapPropertyPage::OnInitRedrawArea();
    CRect DataRect(6, 92, 240, 280);
	memDC.DrawBar(DataRect.top,
			 Fixture.GetPercentageOfPlay(HOME),
			 100 - Fixture.GetPercentageOfPlay(HOME),
			 ORANGE, BLUE);

	str.LoadString(IDS_MSTATPERCENT);
	DataRect.left = 120 - memDC.GetTextExtent(str).cx / 2;
	memDC.SetTextColor(WHITE);
	memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, str, str.GetLength(), null);
	str.Format(_T("%d"), Fixture.GetPercentageOfPlay(HOME));
	memDC.DisplayRightTitle(str, &DataRect, 40, DataRect.top, WHITE, true);
	str.Format(_T("%d"), Fixture.GetPercentageOfPlay(AWAY));
	memDC.DisplayRightTitle(str, &DataRect, 220, DataRect.top, WHITE, true);

	DataRect.top += iGapY - 8;
	memDC.SetBkMode(OPAQUE);
	DataRect.top += 20;
	DataRect.left = 128;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetGoalKeeperSkill(AWAY) / 5, PosnColour[0], 4, 8);
	DataRect.left = 112;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetGoalKeeperSkill(HOME) / 5, PosnColour[0], 4, 8);
	DataRect.top += iGapY;
	DataRect.left = 128;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetDefenderSkill(AWAY) / 5, PosnColour[1], 4, 8);
	DataRect.left = 112;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetDefenderSkill(HOME) / 5, PosnColour[1], 4, 8);
	DataRect.top += iGapY;
	DataRect.left = 128;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetMidfielderSkill(AWAY) / 5, PosnColour[2], 4, 8);
	DataRect.left = 112;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetMidfielderSkill(HOME) / 5, PosnColour[2], 4, 8);
	DataRect.top += iGapY;
	DataRect.left = 128;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetAttackerSkill(AWAY) / 5, PosnColour[3], 4, 8);
	DataRect.left = 112;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetAttackerSkill(HOME) / 5, PosnColour[3], 4, 8);
	memDC.SetBkMode(TRANSPARENT);
	CMatchBitmapPropertyPage::OnPaint((bool)(theApp.GetApplicationPreferences().GetMatchSpeed() > 0));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Skill::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65Skill::OnInitDialog()
{
	m_gStatic.OnCreate(IDC_STATIC65GKSKILL, this, 16, PosnColour[0], WHITE, IDS_MSTATGSKILL);
	m_dStatic.OnCreate(IDC_STATIC65DEFSKILL, this, 16, PosnColour[1], WHITE, IDS_MSTATDSKILL);
	m_mStatic.OnCreate(IDC_STATIC65MIDSKILL, this, 16, PosnColour[2],  WHITE,IDS_MSTATMSKILL);
	m_aStatic.OnCreate(IDC_STATIC65ATTSKILL, this, 16, PosnColour[3],  WHITE,IDS_MSTATASKILL);

	return CMatchBitmapPropertyPage::OnInitDialog(TEXT("Match Day"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Skill::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Skill::OnSize(UINT nType, int cx, int cy)
{
	CMatchBitmapPropertyPage::OnSize(nType, cx, cy);
	CMatchBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN65);

	Sleep(1);
}