
/*

	Screen36View.cpp : implementation file

	Team and individual training program selection

	Football Director

	(c) Rw Software 2004- 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "Screen36View.h"
#include "PopupPlayerMenu.h"
#include "SoccerSquadTraining.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen36View dialog


IMPLEMENT_DYNCREATE(CScreen36View, CFormView)

/*------------------------------------------------------------------------------
	Method:   	CScreen36View::CScreen36View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen36View::CScreen36View()
	: CBitmapFormView(CScreen36View::IDD)
{
	m_bTeamPercentLock[0] = false;
	m_bTeamPercentLock[1] = false;
	m_bTeamPercentLock[2] = false;
	m_bTeamPercentLock[3] = false;
}


BEGIN_MESSAGE_MAP(CScreen36View, CBitmapFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
//	ON_COMMAND_RANGE(IDC_CHECK3601, IDC_CHECK3604, OnClickLock)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen36View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnClickAdjustPercentage
	Access:    	protected 
	Parameter: 	const UINT nWhich
	Parameter: 	const bool bInc
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnClickAdjustPercentage(const UINT nWhich, const bool bInc)
{
 	int nLockCount = 0;
 	int nLockedPercentage = 0;
 	int nAmountToAdjust = 6;

 	int nMax = 100 - nLockedPercentage;
 	// get max/min amount that can be added/subbed from adjusted value
 	if (bInc == true)
 	{
 		if (m_PercentSelector[nWhich].GetCurValue() + nAmountToAdjust > nMax)
 		{
 			nAmountToAdjust = nMax - m_PercentSelector[nWhich].GetValue();
 		}
		m_PercentSelector[nWhich].OnIncreaseValue(nAmountToAdjust);
 	}
 	else
 	{
 		if (m_PercentSelector[nWhich].GetCurValue() - nAmountToAdjust < 0)
 		{
 			nAmountToAdjust = m_PercentSelector[nWhich].GetValue();
 		}
		m_PercentSelector[nWhich].OnDecreaseValue(nAmountToAdjust);
 	}
 
 	// do {nAmountToAdjust} passes to add/sub to adjusted control
 	bool bChanged;
 	while (nAmountToAdjust)
 	{
 		// try to add/remove a value from a different no locked control
 		bChanged = false;
 		for (int nCount = 0; nCount < NUM_PERCENT_SELECTORS; nCount++)
 		{
 			// if control not locked or adjusted one
 			if (nWhich != nCount)
 			{
				// if adding to adjusted the subtract the others
				if (bInc == true)
				{
					// if any left todo then sub from control
					if (nAmountToAdjust)
					{
						if (m_PercentSelector[nCount].GetCurValue() > 0)
						{
							m_PercentSelector[nCount].OnDecValue();
							nAmountToAdjust--;
							bChanged = true;
						}
					}
				}		// if less than adjusted one then increase
				else
				{
					// if any left todo then add to control
					if (nAmountToAdjust)
					{
						if (m_PercentSelector[nCount].GetCurValue() < 100)
						{
							m_PercentSelector[nCount].OnIncValue();
							nAmountToAdjust--;
							bChanged = true;
						}
					}
				}
 			}
 		}
 		if (bChanged == false)
 		{
 			nAmountToAdjust = 0;
 		}
 	}
 
 	// Save the values
 	DoSaveTeamTraining();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Team Training"), &theApp.GetBasicDisplay(), IDS_SCREEN36);
	m_PercentSelector[0].DoInitialise(BUTTON1Y, IDS_STRING3601T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_PercentSelector[1].DoInitialise(BUTTON2Y, IDS_STRING3604T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_PercentSelector[2].DoInitialise(BUTTON3Y, IDS_STRING3603T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_PercentSelector[3].DoInitialise(BUTTON4Y, IDS_STRING3602T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	CSquadTraining& rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();
	m_PercentSelector[0].SetCurValue(rTraining.Get5aSidePercent());
	m_PercentSelector[1].SetCurValue(rTraining.Tactics().GetPercentage());
	m_PercentSelector[2].SetCurValue(rTraining.SetPiece().GetPercentage());
	m_PercentSelector[3].SetCurValue(rTraining.GetRestingPercent());
	m_FormationSelector.SetCurValue(rTraining.Tactics().GetSelection());
	m_CurrentTextSelector.OnInitialise(&m_PercentSelector[0], NUM_PERCENT_SELECTORS + 1, NUM_PERCENT_SELECTORS);
	m_FormationSelector.DoInitialise(BUTTON5Y, IDS_TACTICFORMATION, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(m_FormationSelector);
	m_FormationSelector.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());

	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();
	m_PercentSelector[0].OnPaint();
	m_PercentSelector[1].OnPaint();
	m_PercentSelector[2].OnPaint();
	m_PercentSelector[3].OnPaint();
	m_FormationSelector.OnPaint();
	if (m_CurrentTextSelector.GetCurSel() < m_CurrentTextSelector.GetListSize())
	{
		m_CurrentTextSelector.OnPaint();
	}
	else
	{
		m_FormationSelector.DoDrawHighlight();
	}
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::DoSaveTeamTraining
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::DoSaveTeamTraining()
{
	CSquadTraining&  rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();
	rTraining.Set5aSidePercent(m_PercentSelector[0].GetValue());
	rTraining.Tactics().SetPercentage(m_PercentSelector[3].GetValue());
	rTraining.SetPiece().SetPercentage(m_PercentSelector[2].GetValue());
	rTraining.SetPiece().SetPercentage(m_PercentSelector[1].GetValue());
	rTraining.SetPiece().SetSelection(m_FormationSelector.GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_CurrentTextSelector.GetCurSel() == m_CurrentTextSelector.GetListSize())
	{
		m_FormationSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else
	{
		if (nChar == VK_LEFT)
		{
			m_PercentSelector[m_CurrentTextSelector.GetCurSel()].OnIncValue();
			OnClickAdjustPercentage(m_CurrentTextSelector.GetCurSel(), false);
		}
		else if (nChar == VK_RIGHT)
		{
			m_PercentSelector[m_CurrentTextSelector.GetCurSel()].OnDecValue();
			OnClickAdjustPercentage(m_CurrentTextSelector.GetCurSel());
		}
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);

	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnLButtonUp(UINT nFlags, CPoint point)
{
	int WhichButton = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	if (m_FormationSelector.OnLButtonUp(WhichButton) == true)
	{
		DoSaveTeamTraining();
	}
	else
	{
		for (int LoopCount = 0; LoopCount < NUM_PERCENT_SELECTORS; LoopCount++)
		{
			if (m_PercentSelector[LoopCount].IsUpButton(WhichButton) == true)
			{
				OnClickAdjustPercentage(LoopCount);
				break;
			}
			if (m_PercentSelector[LoopCount].IsDownButton(WhichButton) == true)
			{
				OnClickAdjustPercentage(LoopCount, false);
				break;
			}
		}
	}
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen36View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen36View::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);
	if (IsInitialised() == true)
	{
		m_PercentSelector[0].MoveWindow();
		m_PercentSelector[1].MoveWindow();
		m_PercentSelector[2].MoveWindow();
		m_PercentSelector[3].MoveWindow();

// 		CRect theRect;
// 		GetClientRect(&theRect);
// 		theRect.top = BUTTON5Y;
// 		theRect.bottom = theRect.top + 18;
// 		theRect.left = GetCentreX() - 50;
// 		theRect.right = theRect.left + 100;
		m_FormationSelector.MoveWindow();

		//GetBasicDisplay()->DoMoveButton(eTRAINING2DOWN, CRect(BUTTON11X, BUTTON11Y, BUTTON11X + eUPDOWNWIDTH, BUTTON11Y + eUPDOWNHEIGHT));
		//GetBasicDisplay()->DoMoveButton(eTRAINING2UP, CRect(BUTTON12X, BUTTON11Y, BUTTON12X + eUPDOWNWIDTH, BUTTON11Y + eUPDOWNHEIGHT));
	}

	Sleep(1);
}