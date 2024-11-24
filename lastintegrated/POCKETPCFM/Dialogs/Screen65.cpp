// Screen65.cpp : implementation file

#include "stdafx.h"

#include "UI.h"
#include "MessageDisplay.h"
#include "ExtPropertySheet.h"
#include "ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"
#include "Screen65Skill.h"
#include "Screen65LineUps.h"
#include "Screen65Events.h"
#include "Screen65Substitutions.h"
#include "Screen65Commentary.h"
#include "Screen27TacticsPage.h"
#include "Screen27SpecialPlayersPage.h"
#include "Screen27FormationPage.h"
#include "Screen65.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen65


/*------------------------------------------------------------------------------
	Method:   	CScreen65::CScreen65
	Access:    	public 
	Parameter: 	UINT nIDCaption
	Parameter: 	CWnd * pParentWnd
	Parameter: 	UINT iSelectPage
	Parameter: 	CMatchPlay * pMatchPlay
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen65::CScreen65(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, CMatchPlay* pMatchPlay)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, m_pCurrentMatchPlay(pMatchPlay)
{
	AddPage(&m_CommentaryPage);
	AddPage(&m_SkillPage);
	AddPage(&m_LineUpsPage);
	AddPage(&m_EventsPage);
	AddPage(&m_TacticsPage);
	AddPage(&m_SubstitutionsPage);
	AddPage(&m_SpecialPlayersPage);
	AddPage(&m_FormationPage);

	m_CommentaryPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
	m_SkillPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
	m_LineUpsPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
	m_EventsPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
	m_SubstitutionsPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
	m_SpecialPlayersPage.SetCurrentMatchPlay(m_pCurrentMatchPlay);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::~CScreen65
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen65::~CScreen65()
{
	if (m_pCurrentMatchPlay->IsMatchOver() == false)
	{
		KillTimer(ID_CLOCK);
	}
}


BEGIN_MESSAGE_MAP(CScreen65, CPropertySheet)
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen65::SetTabText
	Access:    	protected 
	Parameter: 	const int nTabID
	Parameter: 	const int nStringID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::SetTabText(const int nTabID, const int nStringID)
{
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	CString pszString;

	pszString.LoadString(nStringID);
	tcItem.pszText = pszString.GetBuffer(256);
	ASSERT(GetTabControl() != null);
	GetTabControl()->SetItem(nTabID, &tcItem);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65::OnInitDialog()
{
#ifdef POCKETPC
	SHDoneButton(SHDB_HIDE);
#endif

	SetTabText(eCOMMENTARY, IDS_TAB6501);
	SetTabText(eSKILL, IDS_TAB6502);
	SetTabText(eLINEUPS, IDS_TAB6503);
	SetTabText(eEVENTS, IDS_TAB6504);
	SetTabText(eSUBSTITUTIONS, IDS_TAB6506);
	SetTabText(eTACTICS, IDS_SCREEN27TACTICS);
	SetTabText(eSPECIALPLAYERS, IDS_SCREEN27SPECIALPLAYERS);
    SetTabText(eFORMATION, IDS_SCREEN27FORMATIONS);

	SetTimer(ID_KICKOFF, 500, null);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::DoPlaySoundFX
	Access:    	protected 
	Parameter: 	const eSOUNDFX theFX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::DoPlaySoundFX(const eSOUNDFX theFX)
{
#ifdef FMOD
	if (theApp.GetApplicationPreferences().getSoundFX().IsEnabled() == true && m_pCurrentMatchPlay->GetMatchActionInfo().GetSoundFXID() != SOUND_MAX)
	{
		theApp.GetSoundManager().DoPlaySoundFX(theFX);
		m_pCurrentMatchPlay->GetMatchActionInfo().SetSoundFXID(SOUND_MAX);

		// Lengthen the time to next event to allow sound to play
		KillTimer(ID_CLOCK);

		// Dont set timer if match over
		if (m_pCurrentMatchPlay->GetMatchActionStage() != MATCHENDED && theApp.GetApplicationPreferences().GetMatchSpeed() > 0)
		{
			SetTimer(ID_CLOCK, 2500 - (theApp.GetApplicationPreferences().GetMatchSpeed() > 4) * 1200, null);
		}
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::DoCommenceHalf
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::DoCommenceHalf()
{
	SetTimer(ID_CLOCK, 800 - (theApp.GetApplicationPreferences().GetMatchSpeed() * 80 + 80), null);
	m_pCurrentMatchPlay->DoCommenceHalf();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::DoEndOfHalf
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::DoEndOfHalf()
{
	static const int HalfEndTextID[] =
	{
		0,				// MATCHNOTSTARTED,
		0,				// MATCHINPROGRESS,
		IDS_HALFTIME,	// MATCHHALFTIME,
		IDS_FULLTIME,	// MATCHENDOFNORMALTIME,
		IDS_HALFTIME,	// MATCHEXTRATIMEHALFTIME,
		IDS_HALFTIME,	// MATCHENDOFEXTRATIME,
		IDS_FULLTIME,	// MATCHREQUIREPENALTYSHOOTOUT,
		IDS_FULLTIME	// MATCHENDED
	};
	ASSERT(HalfEndTextID[m_pCurrentMatchPlay->GetMatchActionStage()] != 0);
	CString strCommentary;
	strCommentary.LoadString(HalfEndTextID[m_pCurrentMatchPlay->GetMatchActionStage()]);
	m_CommentaryPage.DoAddCommentary(strCommentary);
	UserMessageBox(strCommentary);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::OnTimer
	Access:    	protected 
	Parameter: 	UINT nIDEvent
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::OnTimer(UINT nIDEvent)
{
	bool bIsPauseMatchPage[] =
	{
		false,	// eCOMMENTARY,
		false,	// eSKILL,
		false,	// eLINEUPS,
		false,	// eEVENTS,
		true,	// eTACTICS,
		true,	// eSUBSTITUTIONS,
		true,	// eSPECIALPLAYERS,
		true,	// eFORMATION,
	};
	UpdateTimeDisplay();
	switch(nIDEvent)
	{
		case ID_CLOCK:
			KillTimer(ID_CLOCK);
			m_CommentaryPage.UpdateCommentary();
			if (m_pCurrentMatchPlay->IsUserSubstitutionRequired() == true)
			{
				CString str;
				str.Format(IDS_PLAYERINJURED, m_pCurrentMatchPlay->GetPlayerName());
				UserMessageBox(str);
				m_pCurrentMatchPlay->SetUserSubstitutionRequired(false);
				static_cast<CTabCtrl*>(GetTabControl())->SetCurSel(eSUBSTITUTIONS);
				UpdateTimeDisplay();
				SetTimer(ID_CLOCK, 1800, null);
				break;
			}
			// Pause match if on a user action page
			if (bIsPauseMatchPage[static_cast<CTabCtrl*>(GetTabControl())->GetCurSel()] != true && theApp.GetApplicationPreferences().GetMatchSpeed() != 0)
			{
				m_pCurrentMatchPlay->DoMinute();

				switch (m_pCurrentMatchPlay->GetMatchActionStage())
				{
					case MATCHEXTRATIMEHALFTIME:
					case MATCHHALFTIME:
						DoEndOfHalf();
						DoCommenceHalf();
						break;
					case MATCHENDOFNORMALTIME:
						DoEndOfHalf();
						DoCommenceHalf();
						break;
					case MATCHNOTSTARTED:
					case MATCHINPROGRESS:
						SetTimer(ID_CLOCK, (800 - theApp.GetApplicationPreferences().GetMatchSpeed() * 100 + 80), null);
						break;
					case MATCHENDOFEXTRATIME:
						DoEndOfHalf();
						break;
					case MATCHREQUIREPENALTYSHOOTOUT:
						DoEndOfHalf();
						m_CommentaryPage.DoAddCommentary(m_pCurrentMatchPlay->PenaltyShootoutWinnerStr());
						UserMessageBox(m_pCurrentMatchPlay->PenaltyShootoutWinnerStr());
						m_pCurrentMatchPlay->SetMatchActionStage(MATCHENDED);
#ifdef POCKETPC
						SHDoneButton(SHDB_SHOW);	  // Show 'ok' on dialog
#endif
						break;
					case MATCHENDED:
						DoEndOfHalf();
#ifdef POCKETPC
						SHDoneButton(SHDB_SHOW);	  // Show 'ok' on dialog
#endif
						break;

					default:
						ASSERT(false);
						break;
				}
			}
			else
			{
				// Match is paused so just keep feeding the timer until it restarts
				SetTimer(ID_CLOCK, 80, null);
			}
			break;

		case ID_KICKOFF:
			KillTimer(ID_KICKOFF);
			UserMessageBox(IDS_KICKOFF);
			DoCommenceHalf();
			break;

		default :
			ASSERT(false);
	}
#ifdef FMOD
		DoPlaySoundFX(m_pCurrentMatchPlay->GetMatchActionInfo().GetSoundFXID());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65::UpdateTimeDisplay
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65::UpdateTimeDisplay()
{
	m_pCurrentPage = null;

	switch(static_cast<CTabCtrl*>(GetTabControl())->GetCurSel())
	{
		case eCOMMENTARY:
			m_pCurrentPage = &m_CommentaryPage;
			m_CommentaryPage.InvalidateRect(null, FALSE);
			break;
		case eSKILL:
			m_pCurrentPage = &m_SkillPage;
			m_SkillPage.InvalidateRect(null, FALSE);
			break;
		case eLINEUPS:
			m_pCurrentPage = &m_LineUpsPage;
			m_LineUpsPage.InvalidateRect(null, FALSE);
			break;
		case eEVENTS:
			m_pCurrentPage = &m_EventsPage;
			m_EventsPage.InvalidateRect(null, FALSE);
			break;
		case eTACTICS:
			m_pCurrentPage = &m_TacticsPage;
			break;
		case eSUBSTITUTIONS:
			m_pCurrentPage = &m_SubstitutionsPage;
			//m_SubstitutionsPage.InvalidateRect(null, FALSE);
			break;
		case eSPECIALPLAYERS:
			m_pCurrentPage = &m_SpecialPlayersPage;
			break;
		case eFORMATION:
			m_pCurrentPage = &m_FormationPage;
			break;
	}
	/*if (m_pCurrentPage != null && (static_cast<CTabCtrl*>(GetTabControl())->GetCurSel() < ePAUSEPAGE || m_pCurrentPage == m_SubstitutionsPage))
	{
		m_pCurrentPage->InvalidateRect(null, FALSE);
	}*/
}
