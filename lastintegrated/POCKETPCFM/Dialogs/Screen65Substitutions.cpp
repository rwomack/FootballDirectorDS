// Screen65Substitutions.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"
#include "Screen65Substitutions.h"
#include "PopupPlayerMenu.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen65Substitutions property page

IMPLEMENT_DYNCREATE(CScreen65Substitutions, CPropertyPage)


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::CScreen65Substitutions
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen65Substitutions::CScreen65Substitutions() :
	CMatchBitmapPropertyPage(CScreen65Substitutions::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::~CScreen65Substitutions
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen65Substitutions::~CScreen65Substitutions()
{
}


BEGIN_MESSAGE_MAP(CScreen65Substitutions, CBitmapPropertyPage)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65Substitutions message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::DisplayStatus
	Access:    	protected 
	Parameter: 	CPlayer& _Player
	Parameter: 	const ushort nNumber
	Parameter: 	CRect& DataRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Substitutions::DisplayStatus(CPlayer& _Player, const ushort nNumber, CRect& DataRect)
{
	CString strPrint;
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	if (m_pCurrentMatchPlay->GetSwapSquadPlayer1() != nNumber && m_pCurrentMatchPlay->GetFixture().GetMatchSubs().HasSubBeenUsed(GetClub().GetFirstTeamSquad().GetSquad(nNumber)) != true)
	{
		if (_Player.GetMatchStatus() != MAXPLAYERSTATUSES)
		{
			// Display the status icon if not available to play
			DataRect.left -= 14;
			m_bmpPlayerStatus.DoDrawTransparent(memDC.operator HDC(), DataRect.left, DataRect.top, _Player.GetMatchStatus());
		}
		else
		{
			strPrint.Format(TEXT("%d"), nNumber + 1);
			DataRect.left -= memDC.GetTextExtent(strPrint).cx;
			memDC.SetTextColor(BLACK);
			memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strPrint, strPrint.GetLength(), null);
		}
	}
	else
	{
		int WhichArrow =  2 - (int)(nNumber < PLAYERSINTEAM);
		if (m_pCurrentMatchPlay->GetFixture().GetMatchSubs().HasSubBeenUsed(GetClub().GetFirstTeamSquad().GetSquad(nNumber)) == true)
		{
			WhichArrow = 0;
		}
		DataRect.left -= 10;
		m_pngArrowLR.DoDrawTransparent(memDC.operator HDC(), DataRect.left, DataRect.top, WhichArrow);
	}

	if (m_pCurrentMatchPlay->GetSwapSquadPlayer1() == nNumber)
	{
		memDC.SetTextColor(BLACK);
	}
	else if (m_pCurrentMatchPlay->GetFixture().GetMatchSubs().HasSubBeenUsed(GetClub().GetFirstTeamSquad().GetSquad(nNumber)) == true)
	{
		memDC.SetTextColor(GREY);
	}
	else
	{
		memDC.SetTextColor(PosnColour[_Player.GetSkills().GetPosition()]);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Substitutions::OnPaint()
{
	// Offscreen buffer
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CBitmapPropertyPage::OnInitRedrawArea();

	CClub& CurrentClub = GetClub();
	GetBasicDisplay().SetFontNormal();
	CString strName;
    CRect DataRect(4, TOPY, 240, 282);
	uint LoopCount;
	for (LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		DataRect.left = 16;
		DisplayStatus(CurrentClub.GetPlayer(LoopCount), LoopCount, DataRect);

		DataRect.left = 24;
		strName = CurrentClub.GetPlayer(LoopCount).GetName();
		memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strName, strName.GetLength(), null);
		DataRect.top += GAPY;
    }

	DataRect.top = TOPY;
	for (LoopCount = PLAYERSINTEAM; LoopCount < PLAYERSINTEAM + m_pCurrentMatchPlay->GetFixture().GetSubsSelect(); LoopCount++)
	{
		DataRect.left = 124;
		DisplayStatus(CurrentClub.GetPlayer(LoopCount), LoopCount, DataRect);

		DataRect.left = 132;
		strName = CurrentClub.GetPlayer(LoopCount).GetName();
		memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strName, strName.GetLength(), null);
		DataRect.top += GAPY;
	}

	// Display how many further substitutions maybe made
	DataRect.top += GAPY * 5 - 4;
	DataRect.left = 128;
	memDC.FillSolidRect(124, DataRect.top, 114, GAPY, VPALEGREEN);

	strName.Format(IDS_SUBSLEFT, m_pCurrentMatchPlay->GetFixture().GetMatchSubs().DoCountSubstitutionsLeft(CMatchBitmapPropertyPage::GetVenue()));
	memDC.SetTextColor(BLACK);
	memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strName, strName.GetLength(), null);
	
	GetBasicDisplay().RestoreFont();
	CMatchBitmapPropertyPage::OnPaint((bool)(theApp.GetApplicationPreferences().GetMatchSpeed() > 0));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65Substitutions::OnInitDialog()
{
#ifdef POCKETPC
	HDC dc = ::GetDC(m_hWnd);
	m_pngArrowLR.DoInitialise(dc, IDR_PNGMATCHSUBS, 3);
	m_bmpPlayerStatus.DoInitialise(dc, IDR_PNGPLAYERSTATUS, MAXPLAYERSTATUSES);
	::ReleaseDC(m_hWnd, dc);
#endif
	return CMatchBitmapPropertyPage::OnInitDialog(TEXT("Match Day"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::DoSetSwap
	Access:    	protected 
	Parameter: 	const ushort _SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:	A player has been selected
------------------------------------------------------------------------------*/
void CScreen65Substitutions::DoSetSwap(const ushort _SquadNumber)
{
	eSubstitutionAction eAction = m_pCurrentMatchPlay->DoSwapPlayers(CMatchBitmapPropertyPage::GetVenue(), _SquadNumber);
	switch (eAction)
	{
		case eNOSUBSLEFT:
#ifdef FMOD
			theApp.GetSoundManager().DoPlaySoundFX(SOUND_BEEP);
#endif
			break;
		case eSWAP1SET:
		case eSWAPPEDONPITCH:
		case eSWAPPEDONBENCH:
		case eSUBSTITUTIONMADE:
			break;
		case eSUBSTITUTIONCANCELLED:
			break;
		default:
			ASSERT(false);
	};
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Substitutions::OnLButtonDown(UINT _nFlags, CPoint _point)
{
#ifdef POCKETPC
	SHRGINFO shrgi = {0};
	shrgi.cbSize = sizeof(SHRGINFO);
	shrgi.hwndClient = m_hWnd;
	shrgi.ptDown.x = _point.x;
	shrgi.ptDown.y = _point.y;
	shrgi.dwFlags = SHRG_RETURNCMD;

	if(GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi))
	{
		CPoint point(GetMessagePos());
		point.y -= 26;
		WorldData().OnViewPlayerInformation(&GetClub().GetPlayer(DoCalcPlayerClickedOn(point)));
		Default();
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Substitutions::OnLButtonUp(UINT _nFlags, CPoint _point)
{
	ushort iPlayerClickedOn = DoCalcPlayerClickedOn(_point);
	if (iPlayerClickedOn != NOPLAYER)
	{
		DoSetSwap(iPlayerClickedOn);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::DoCalcPlayerClickedOn
	Access:    	protected 
	Parameter: 	CPoint _point
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CScreen65Substitutions::DoCalcPlayerClickedOn(CPoint _point)
{
	ushort iPlayerClickedOn = NOPLAYER;
	int iColumn = (_point.y - 90) / GAPY;
	if (_point.y > 90)
	{
		if (_point.x > 119)
		{
			// clicked on substitute
			if (iColumn < m_pCurrentMatchPlay->GetFixture().GetSubsSelect())
			{
				iPlayerClickedOn = iColumn + PLAYERSINTEAM;
			}
		}
		else
		{
			// clicked on team
			if (iColumn < PLAYERSINTEAM)
			{
				iPlayerClickedOn = iColumn;
			}
		}
	}
	return iPlayerClickedOn;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Substitutions::GetClub
	Access:    	protected 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CScreen65Substitutions::GetClub()
{
	if (m_pCurrentMatchPlay->IsUserControlledClub(HOME) == true)
	{
		return m_pCurrentMatchPlay->GetFixture().GetClub(HOME);
	}
	return m_pCurrentMatchPlay->GetFixture().GetClub(AWAY);
}
