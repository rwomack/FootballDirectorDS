
/*

	Screen06.cpp

	Player Search

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"

#include "UI.h" 
#include "Dialog.h"
#include "Screen06.h"
#include "PlayerSearch.h"


IMPLEMENT_DYNAMIC(CScreen06, CDialog)


int MinAge[] = 
{
	MINIMUPLAYERAGE, 22, 31
};
int MaxAge[] = 
{
	21, 30, MAXIMUMPLAYERAGE
};
static const int MinSkill [] = 
{
	MINPLAYSKILL, 30, 60
};

static const int MaxSkill [] = 
{
	40, 70, MAXPLAYSKILL
};

static const int MinPrice[] = 
{
	0, 3, 5, 7
};

static const int MaxPrice[] = 
{
	3, 5, 7, NUMMONEYSTEPS - 1
};


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::GetValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompactPlayerSearch::GetValue()
{
	return m_Value;
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::GetAge
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompactPlayerSearch::GetAge()
{
	return m_Age;
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::GetSkill
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompactPlayerSearch::GetSkill()
{
	return m_Skill;
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::GetPosition
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompactPlayerSearch::GetPosition()
{
	return m_Position;
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::GetAvailability
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompactPlayerSearch::GetAvailability()
{
	return m_Availability;
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::SetFromPlayerSearch
	Access:    	public 
	Parameter: 	CPlayerSearch & _Search
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCompactPlayerSearch::SetFromPlayerSearch(CPlayerSearch& _Search)
{
	for (int LoopCount = 0; LoopCount < NUMMONEYSTEPS; LoopCount++)
	{
		if (_Search.GetMinPriceStep() == MONEYSTEPS[LoopCount])
		{
			m_Value = LoopCount;
			break;
		}
	}

	for (int LoopCount = 0; LoopCount < 3; LoopCount++)
	{
		if (MinAge[LoopCount] == _Search.GetMinAge())
		{
			m_Age = LoopCount;
			break;
		}
	}
	
	for (int LoopCount = 0; LoopCount < 3; LoopCount++)
	{
		if (MinSkill[LoopCount] == _Search.GetMinSkill())
		{
			m_Skill = LoopCount;
			break;
		}
	}
	
	m_Position = _Search.GetFindPosn();

	m_Availability = 2;
	if (_Search.GetIncludeFlag(CPlayerSearch::INCLUDEONTRANSFERLIST))
	{
		m_Availability = 0;
	}
	else if (_Search.GetIncludeFlag(CPlayerSearch::INCLUDEONLOANLIST))
	{
		m_Availability = 1;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCompactPlayerSearch::SetToPlayerSearch
	Access:    	public 
	Parameter: 	CPlayerSearch & _Search
	Returns:   	void
	Qualifier: 	
	Purpose:	Convert class data to CPlayerSearch class data
------------------------------------------------------------------------------*/
void CCompactPlayerSearch::SetToPlayerSearch(CPlayerSearch& _Search)
{
	_Search.SetMinPriceStep(MinPrice[m_Value]);
	_Search.SetMaxPriceStep(MaxPrice[m_Value]);
	
	_Search.SetMinAge(MinAge[m_Age]);
	_Search.SetMaxAge(MaxAge[m_Age]);
	
	_Search.SetMinSkill(MinSkill[m_Skill]);
	_Search.SetMaxSkill(MaxSkill[m_Skill]);
	
	_Search.SetFindPosition((ePlayerPosition)m_Position);

	byte Flags = CPlayerSearch::INCLUDESUSPENDED | CPlayerSearch::INCLUDEINJURED;
	switch(m_Availability)
	{
		case 0:
			Flags |= CPlayerSearch::INCLUDEONTRANSFERLIST | CPlayerSearch::INCLUDEOUTOFCONTRACT;
			break;
		case 1:
			Flags |= CPlayerSearch::INCLUDEONLOANLIST;
			break;
		case 2:
			Flags |= CPlayerSearch::INCLUDENONLISTED | CPlayerSearch::INCLUDEOUTOFCONTRACT;
			break;

		default:
			EXT_ASSERT(0);
	}
	_Search.SetIncludeFlags(Flags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::CScreen06
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen06::CScreen06(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CScreen06::IDD, pParent)
{

}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::~CScreen06
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen06::~CScreen06()
{
	m_CompactSearch.SetToPlayerSearch(WorldData().GetCurrentUser()->GetPlayerSearch());
}


BEGIN_MESSAGE_MAP(CScreen06, CBitmapDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen06 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen06::OnInitDialog()
{
	CBitmapDialog::DoEnableOKButton();
	CBitmapDialog::LoadMenu(IDR_MENUCONTINUE);

	int CurrY = TOPY + 12;
	int TitleID[] =
	{
		IDS_SEARCHVALUE, IDS_SEARCHAGE, IDS_SEARCHSKILL, IDS_SEARCHPOSITION, IDS_SEARCHAVAILABILITY
	};
	for (int Loopcount = 0; Loopcount < eMAXSELECTOR; Loopcount++)
	{
		m_TextSelector[Loopcount].DoInitialise(CurrY, TitleID[Loopcount], this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
		CurrY += eUPDOWNHEIGHT + 6;
	}

	m_CompactSearch.SetFromPlayerSearch(WorldData().GetCurrentUser()->GetPlayerSearch());

	// Populate Value List box
	int strValueID[] = 
	{
		IDS_SEARCHVALUE1, IDS_SEARCHVALUE2, IDS_SEARCHVALUE3, IDS_SEARCHVALUE4
	};
	m_TextSelector[eVALUE].DoAddStrIDList(&strValueID[0], 4, m_CompactSearch.GetValue());

	// Populate Age List box
	int strAgeID[] = 
	{
		IDS_SEARCHAGE1, IDS_SEARCHAGE2, IDS_SEARCHAGE3
	};
	m_TextSelector[eAGE].DoAddStrIDList(&strAgeID[0], 3, m_CompactSearch.GetAge());

	// Populate Skill List box
	int strSkillID[] = 
	{
		IDS_SEARCHSKILL1, IDS_SEARCHSKILL2, IDS_SEARCHSKILL3
	};
	m_TextSelector[eSKILL].DoAddStrIDList(&strSkillID[0], 3, m_CompactSearch.GetAge());

	// Populate Position List box
	int strPosnID[] = 
	{
		IDS_SEARCHPOSN1, IDS_SEARCHPOSN2, IDS_SEARCHPOSN3, IDS_SEARCHPOSN4, IDS_SEARCHPOSN5
	};
	m_TextSelector[ePOSITION].DoAddStrIDList(&strPosnID[0], 5, 0);

	// Populate Availability List box
	int strAvailID[] = 
	{
		IDS_SEARCHAVAIL1, IDS_SEARCHAVAIL2, IDS_SEARCHAVAIL3
	};
	m_TextSelector[eAVAILABILITY].DoAddStrIDList(&strAvailID[0], 3, 0);
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);
	SetIsInitialised(true);
	return CBitmapDialog::OnInitDialog(IDS_HELPSCREEN06, IDS_SCREEN06);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	for (int Loopcount = 0; Loopcount < eMAXSELECTOR; Loopcount++)
	{
		m_TextSelector[Loopcount].OnLButtonUp(ButtonPressed);
	}
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
		case VK_SELECT:
			OnOK();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_CurrentTextSelector.OnPaint();
	CBitmapDialog::OnPaint(false, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen06::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen06::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);

		for (int Loopcount = 0; Loopcount < eMAXSELECTOR; Loopcount++)
		{
			m_TextSelector[Loopcount].MoveWindow();
		}
		Sleep(1);
	}
}