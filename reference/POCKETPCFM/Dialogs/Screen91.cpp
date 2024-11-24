// Screen91.cpp : Options screen implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen91.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen91 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::CScreen91Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	: CBitmapDialog(IDD, pParent)
	Purpose:
------------------------------------------------------------------------------*/
CScreen91Dlg::CScreen91Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen91Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::~CScreen91Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen91Dlg::~CScreen91Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen91Dlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen91 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_TextSelector[EFFECTS].OnPaint();
	m_TextSelector[MENU].OnPaint();
	m_TextSelector[INGAME].OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen91Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN91, IDS_SCREEN91);
	CBitmapDialog::LoadMenu(IDR_MENUCONTINUE);

	int CurrY = 66;
	m_TextSelector[EFFECTS].DoInitialise(CurrY, IDS_SOUNDFX, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	CurrY += 44;
	m_TextSelector[MENU].DoInitialise(CurrY, IDS_MUSIC, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	CurrY += 44;
	m_TextSelector[INGAME].DoInitialise(CurrY, IDS_MUSICINGAME, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);

	int nVolumeID[] =
	{
		IDS_VOLUME, IDS_VOLUME2,
		IDS_VOLUME3, IDS_VOLUME4,
		IDS_VOLUME5, IDS_VOLUME6,
		IDS_VOLUME7, IDS_VOLUME8,
	};
	m_TextSelector[EFFECTS].DoAddStrIDList(nVolumeID, 8);
	m_TextSelector[EFFECTS].SetCurSel(theApp.GetApplicationPreferences().GetSoundFX().GetVolume());
	m_TextSelector[MENU].DoAddStrIDList(nVolumeID, 8);
	m_TextSelector[MENU].SetCurSel(theApp.GetApplicationPreferences().GetMusic().GetVolume());
	m_TextSelector[INGAME].DoAddStrIDList(nVolumeID, 8);
	m_TextSelector[INGAME].SetCurSel(theApp.GetApplicationPreferences().GetMusicInGame().GetVolume());

	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], 3);
//	SetWindowText(TEXT("Football Director"));
//	theApp.GetMainWnd()->UpdateWindow();	
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnOK()
{
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_F23:
		case VK_RETURN:
			OnOK();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_TextSelector[EFFECTS].OnLButtonUp(ButtonPressed);
	theApp.GetApplicationPreferences().SetSoundEffectsVolume(m_TextSelector[EFFECTS].GetCurSel());
#ifdef FMOD
	theApp.GetSoundManager().SetSFXMasterVolume(m_TextSelector[EFFECTS].GetCurSel());
#endif
	m_TextSelector[MENU].OnLButtonUp(ButtonPressed);
	theApp.GetApplicationPreferences().SetMenuMusicVolume(m_TextSelector[MENU].GetCurSel());
#ifdef FMOD
	theApp.GetSoundManager().SetMasterVolume(m_TextSelector[MENU].GetCurSel());
#endif
	theApp.GetApplicationPreferences().SetMusicInGameVolume(m_TextSelector[INGAME].GetCurSel());
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91Dlg::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen91::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen91Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		
		m_TextSelector[EFFECTS]. MoveWindow(); //CRect(GetCentreX() + eUPDOWNWIDTH, CurrY, GetCentreX() + eUPDOWNWIDTH + 80, CurrY + eUPDOWNHEIGHT));
		m_TextSelector[MENU].MoveWindow(); //CRect(GetCentreX() + eUPDOWNWIDTH, CurrY, GetCentreX() + eUPDOWNWIDTH + 80, CurrY + eUPDOWNHEIGHT));
		m_TextSelector[INGAME].MoveWindow(); //CRect(GetCentreX() + eUPDOWNWIDTH, CurrY, GetCentreX() + eUPDOWNWIDTH + 80, CurrY + eUPDOWNHEIGHT));
	}
	Sleep(1);
}