// Screen99.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "SaveGameHeader.h"

#include "Screen99.h"
#include "Screen90.h"
#include "Screen91.h"

#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen99Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::CScreen99Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen99Dlg::CScreen99Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen99Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::~CScreen99Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen99Dlg::~CScreen99Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen99Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen99 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnKillFocus
	Access:    	public 
	Parameter: 	CWnd * _pNewWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnKillFocus(CWnd* _pNewWnd)
{
//	CBitmapDialog::DoTurnOffFullScreen();
}


/*------------------------------------------------------------------------------
	Method:   	CSIPLookupEdit::OnSetFocus
	Access:    	protected 
	Parameter: 	CWnd * _pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnSetFocus(CWnd* _pOldWnd)
{
//	CBitmapDialog::DoInitFullScreen();
	// Call SHInitDialog with flags for full screen.
// 	SHINITDLGINFO shidi;
// 	shidi.dwMask = SHIDIM_FLAGS;
// 	shidi.dwFlags = 0;
// 	shidi.hDlg = m_hWnd;
// 	::SHInitDialog(&shidi);

//	HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
//	::ShowWindow(cmdBar, SW_SHOW);

	// SHFullScreen fails if dialog box is not foreground.
// 	SetForegroundWindow();
// 	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR);
// 	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
// 	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
// 	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
// 	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
// 	SHFullScreen(m_hWnd, SHFS_HIDESTARTICON);
// 
// 	RECT rect;
// 	GetWindowRect(&rect);
// 	rect.top = 0;
// 	MoveWindow(&rect, TRUE);
//	SetWindowPos(_pOldWnd, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnPaint()
{
	GetBasicDisplay()->OnInitRedrawArea(this);
	GetBasicDisplay()->OnPaint(false);
	m_Title.Draw(CBitmapDialog::GetMemDC()->operator HDC(), 0, 0);
	GetBasicDisplay()->OnPaintControls();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_UP:
			{
				m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATENORMAL);
				eDIALOG NewHilight[] = 
				{
					eEXIT, eCONTINUE, eNEWGAME, eREGISTRATION, eLOADGAME, eOPTIONS, eCREDITS
				};
				m_CurrentButton = NewHilight[m_CurrentButton];
				if (m_CurrentButton == eREGISTRATION && m_Buttons[eREGISTRATION].GetStatus() == BUTTONSTATEHIDDEN)
				{
					m_CurrentButton = eNEWGAME;
				}
				if (m_CurrentButton == eCONTINUE && m_Buttons[eCONTINUE].GetStatus() == BUTTONSTATEHIDDEN)
				{
					m_CurrentButton = eEXIT;
				}
				m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATECURRENT);
				ReDraw();
			}
			break;
		case VK_DOWN:
			{
				m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATENORMAL);
				eDIALOG NewHilight[] = 
				{
					eNEWGAME, eREGISTRATION, eLOADGAME, eOPTIONS, eCREDITS, eEXIT, eCONTINUE
				};
				m_CurrentButton = NewHilight[m_CurrentButton];
				if (m_CurrentButton == eREGISTRATION && m_Buttons[eREGISTRATION].GetStatus() == BUTTONSTATEHIDDEN)
				{
					m_CurrentButton = eLOADGAME;
				}
				if (m_CurrentButton == eCONTINUE && m_Buttons[eCONTINUE].GetStatus() == BUTTONSTATEHIDDEN)
				{
					m_CurrentButton = eNEWGAME;
				}
				m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATECURRENT);
				ReDraw();
			}
			break;
		case VK_F23:
		case VK_RETURN:
			m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATEDOWN);
			ReDraw();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
		case VK_F23:
			m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATECURRENT);
			ReDraw();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen99Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN99, IDR_PNGTITLEBACK, true);
	HDC dc = ::GetDC(m_hWnd);
	m_Title.SetBitmap(dc, IDR_PNGTITLEOVERLAY, _T("PNG"));
	::ReleaseDC(m_hWnd, dc);
	GetBasicDisplay()->SetBitmap(IDR_PNGTITLEBACK);
	GetBasicDisplay()->ButtonList().SetSize(eBUTTONS);
	GetBasicDisplay()->ButtonList().SetAt(eCONTINUE, &m_Buttons[eCONTINUE]);
	GetBasicDisplay()->ButtonList().SetAt(eNEWGAME, &m_Buttons[eNEWGAME]);
	GetBasicDisplay()->ButtonList().SetAt(eREGISTRATION, &m_Buttons[eREGISTRATION]);
	GetBasicDisplay()->ButtonList().SetAt(eLOADGAME, &m_Buttons[eLOADGAME]);
	GetBasicDisplay()->ButtonList().SetAt(eOPTIONS, &m_Buttons[eOPTIONS]);
	GetBasicDisplay()->ButtonList().SetAt(eCREDITS, &m_Buttons[eCREDITS]);
	GetBasicDisplay()->ButtonList().SetAt(eEXIT, &m_Buttons[eEXIT]);
	m_Buttons[eCONTINUE].DoSetDrawInfo(IDR_PNGSCREEN99CONTINUE, this, 3);
	m_Buttons[eNEWGAME].DoSetDrawInfo(IDR_PNGSCREEN99NEWGAME, this, 3);
	m_Buttons[eLOADGAME].DoSetDrawInfo(IDR_PNGSCREEN99LOADGAME, this, 3);
	m_Buttons[eOPTIONS].DoSetDrawInfo(IDR_PNGSCREEN99OPTIONS, this, 3);
	m_Buttons[eREGISTRATION].DoSetDrawInfo(IDR_PNGSCREEN99REGISTER, this, 3);
	m_Buttons[eCREDITS].DoSetDrawInfo(IDR_PNGSCREEN99CREDITS, this, 3);
	m_Buttons[eEXIT].DoSetDrawInfo(IDR_PNGSCREEN99EXIT, this, 3);
	m_Buttons[eNEWGAME].SetStatus(BUTTONSTATECURRENT);
	m_CurrentButton = eNEWGAME;
	if (theApp.IsRegistered(false) == true)
	{
		m_Buttons[eREGISTRATION].SetStatus(BUTTONSTATEHIDDEN);
	}

	CSaveGameFile SaveGame;
	SaveGame.SetFileNameFromID(IDS_AUTOSAVEGAMEFILE);
	WIN32_FIND_DATA FileData;   // Data structure describes the file found
	if (FindFirstFile(theApp.GetSaveDirectory() + SaveGame.GetFileName(), &FileData) == INVALID_HANDLE_VALUE)
	{
		m_Buttons[eCONTINUE].SetStatus(BUTTONSTATEHIDDEN);
	}

	GetBasicDisplay()->SetDisplayHeader(false);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonDown(nFlags, point);
	if (ButtonPressed != NOTFOUND)
	{
		SetCurrentButton((eDIALOG)ButtonPressed);
	}
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnButtonPressed((eDIALOG)GetBasicDisplay()->OnLButtonUp(nFlags, point));
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnButtonPressed
	Access:    	protected 
	Parameter: 	eDIALOG _Button
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnButtonPressed(eDIALOG _Button)
{
	// New game
	SetCurrentButton(_Button);
	switch (_Button)
	{	
		case eNEWGAME:
			OnOK();
			break;
		case eCONTINUE:
			{
				CWaitCursor theCursor;
				CSaveGameFile SaveGame;
				SaveGame.SetFileNameFromID(IDS_AUTOSAVEGAMEFILE);
				if (WorldData().GetGameFromDisk(theApp.GetSaveDirectory() + SaveGame.GetFileName()) == true)
				{
					CDialog::EndDialog(IDIGNORE);
				}
			}
			break;
		case eLOADGAME:
			// Load game
			if (DoDialog(eLOADGAME) == IDIGNORE)
			{
				EndDialog(IDIGNORE);
			}
			break;
		case eCREDITS:
			// Credits
			DoDialog(eCREDITS);
			break;
		case eOPTIONS:
			// Options
			DoDialog(eOPTIONS);
			break;
		case eREGISTRATION:
			theApp.GetRegSystem()->showRegisterForm();
			if (theApp.IsRegistered() == true)
			{
				theApp.GetRegSystem()->showRegisteredForm();
				m_Buttons[eREGISTRATION].SetStatus(BUTTONSTATEHIDDEN);
			}
			break;
		case eEXIT:
			EndDialog(IDABORT);
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::SetCurrentButton
	Access:    	protected 
	Parameter: 	eDIALOG _Button
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::SetCurrentButton(eDIALOG _Button)
{
	m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATENORMAL);
	m_CurrentButton = _Button;
	if (m_Buttons[m_CurrentButton].GetStatus() != BUTTONSTATEDOWN)
	{
		m_Buttons[m_CurrentButton].SetStatus(BUTTONSTATECURRENT);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::DoDialog
	Access:    	protected 
	Parameter: 	eDIALOG eDlg
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CScreen99Dlg::DoDialog(eDIALOG eDlg)
{
	int iRet = IDOK;
	switch (eDlg)
	{
		case eOPTIONS:
		{
			CScreen91Dlg Screen91Dlg;
			Screen91Dlg.DoModal();
			break;
		}
		case eCREDITS:
			theApp.OnAppAbout();
			break;
		case eLOADGAME:
		{
			CScreen90Load Screen90LoadDlg;
			iRet = Screen90LoadDlg.DoModal();
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen99Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen99Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);

		int ButtonX = GetCentreX() - BUTTONW / 2;
		CRect theRect;
		GetClientRect(&theRect);
		int ActiveButtonCount = 0;
		for (int LoopCount = 0; LoopCount < eBUTTONS; LoopCount++)
		{
			if (m_Buttons[LoopCount].GetStatus() != BUTTONSTATEHIDDEN)
			{
				ActiveButtonCount++;
			}
		}

		int ButtonY = ((theRect.bottom - theRect.top) - (BUTTONH * ActiveButtonCount)) / 2 - BUTTONH;
		for (int LoopCount = 0; LoopCount < eBUTTONS; LoopCount++)
		{
			if (m_Buttons[LoopCount].GetStatus() != BUTTONSTATEHIDDEN)
			{
				m_Buttons[LoopCount].MoveWindow(ButtonX, ButtonY += BUTTONH);
			}
		}
	}
	Sleep(1);
}