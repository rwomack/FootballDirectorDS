
/*

	TheApp.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Form.h"
#include "MainFrm.h"
#include "ExtEdit.h"
#include "MessageDisplay.h"
#include "FootballDirectorDoc.h"
#include "FootballDirector.h"
#include "MatchInfo.h"
#include "MessageDisplay.h"
#include "CurrentForm.h"
#include "PopupPlayerMenu.h"
#include "IndexedIntArray.h"
#include "ProgressBar.h"

#include "LeadingGoalScorer.h"
#include "LeadingDisciplinaryPoints.h"
#include "YouthGrid.h"
#include "EmployeeGrid.h"
#include "PlayerNameTextRegion.h"
#include "ClubFixtureGrid.h"
#include "Screen05Base.h"

#include "Screen03View.h"
#include "Screen87View.h" 
#include "Screen98.h"
#include "Screen99.h"

	char		g_Filename[256];
	CExtString	g_DebugStr;
	CRandomNumber RandomNumber;

extern AWRegistrationDataType regSavedPrefs;

#define REGISTRATION_FREE_GAMES 0
#define REGISTRATION_MAX_GAMES 10
#define REGISTRATION_DELAY_INCREMENT 25
#define REGISTRATION_FIRST_WAIT_LENGTH 800 // 8 seconds
#define REGISTRATION_MIN_WAIT_LENGTH 600 // 6 seconds
#define REGISTRATION_MAX_WAIT_LENGTH 2500 // 25 seconds



/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::CFootballDirectorApp
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFootballDirectorApp::CFootballDirectorApp()
	: m_bIsInitialised(false)
	, CWinApp()
{
}


/*------------------------------------------------------------------------------
	Method:   	WorldData
	Access:    	public
	Returns:   	CWorld&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CWorld& WorldData()
{
	return theApp.GetWorldData();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFootballDirectorApp object

CFootballDirectorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFootballDirectorApp initialization


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::InitInstance
	Access:    	public
	Returns:   	BOOL
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
BOOL CFootballDirectorApp::InitInstance()
{
	InitDebugMessages();
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CFootballDirectorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CScreen03View)
	);
	AddDocTemplate(pDocTemplate);
	m_ApplicationPrefs.DoInitialise(m_SystemPrefs, IDS_SYSTEMREGISTRYKEY);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	// Dispatch commands specified on the command line
	if(!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

#ifdef FMOD
	GetSoundManager().DoInitialise();
#endif
	m_WorldData = null;
	SetWhichCountry(0);
	OnInitialiseGame();

#ifdef TEST_DIALOGS
	DebugDialogs();
#endif
#ifndef NOTITLEMENU
	OnInitialiseDRM();
	CScreen99Dlg dlgMenu;
	int nResponse = dlgMenu.DoModal();
#else
	int nResponse = IDOK;
#endif
	GetBasicDisplay().SetBitmap(IDR_PNGHEADER);
	GetBasicDisplay().SetDisplayHeader(true);

	if (nResponse == IDOK)
	{
#ifndef NOTITLEMENU
		// Input Manager Name
		CScreen98 Screen98Dlg;
		Screen98Dlg.DoModal();
#endif
		// Select Club
		OnSelectUserClubToManage();
		m_bIsInitialised = true;
		m_WorldData->GetCountryList().DoEndOfWeek();
		GetActiveView()->OnScreen03();
	}
	else if (nResponse == IDABORT)
	{
		OnExitApp();
	}
	else
	{
		m_bIsInitialised = true;
		GetActiveView()->OnScreen03();
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnInitialiseDRM
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnInitialiseDRM()
{
	MemSet(&m_RegParams, sizeof(AWRegistrationParametersType), 0);

	m_RegParams.freeGames 		= REGISTRATION_FREE_GAMES;
	m_RegParams.maxTrialGames	= REGISTRATION_MAX_GAMES;
	m_RegParams.delayIncrement 	= REGISTRATION_DELAY_INCREMENT;
	m_RegParams.firstWaitLength = REGISTRATION_FIRST_WAIT_LENGTH;
	m_RegParams.minWaitLength 	= REGISTRATION_MIN_WAIT_LENGTH;
	m_RegParams.maxWaitLength 	= REGISTRATION_MAX_WAIT_LENGTH;
	m_RegParams.idListResource 	= DATA_REGISTRATION_APPID_LIST;
	m_RegParams.timeoutType		= AWREGISTRATIONTIMEOUT_GAMES;

	m_pRegSystem = new CRegistrationUI();
	m_pRegSystem->initialiseApplicationData(AfxGetInstanceHandle(), IDC_REGTEST, IDS_APP_TITLE, IDS_REGISTRYKEY, IDS_SYSTEMREGISTRYKEY);
	m_pRegSystem->setDefaultRegParams(&m_RegParams);
	m_pRegSystem->start();
	//###############################################################################################

	// Beta test code, only runs for 21 days
	if (IsRegistered() == false)
	{
		SYSTEMTIME CompileTime;
		const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
			"Sep", "Oct", "Nov", "Dec"};

		char temp [] = __DATE__;
		CompileTime.wYear = atoi(temp + 9) + 2000;
		*(temp + 6) = 0;
		CompileTime.wDay = atoi(temp + 4);
		*(temp + 3) = 0;
		for (int i = 0; i < 12; i++)
		{
			if (!strcmp(temp, months[i]))
			{
				CompileTime.wMonth = i + 1;
				break;
			}
		}
		SYSTEMTIME theTime;
		GetSystemTime(&theTime);
		CDate theDate;
		if (theDate.ConvertToJulian(theTime.wMonth, theTime.wDay, theTime.wYear) - theDate.ConvertToJulian(CompileTime.wMonth, CompileTime.wDay, CompileTime.wYear) > 21)
		{
			AfxMessageBox(IDS_VERSIONEXPIRED);
			OnExitApp();
		}
		//###############################################################################################
		if (m_pRegSystem->runTrialDelay() != 0)
		{
			OnExitApp();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::szAddPath
	Access:    	public
	Parameter: 	char * file
	Returns:   	char*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
char* CFootballDirectorApp::szAddPath(char* _pFile)
{
	char szTemp[80];
	wcstombs(szTemp, m_ApplicationPrefs.GetAppPath(), 80);
	sprintf(g_Filename, "%s\\%s", szTemp, _pFile);
	return( &g_Filename[0]);
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetSystemPreferences
	Access:    	public 
	Parameter: 	void
	Returns:   	CSystemPreferences&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSystemPreferences& CFootballDirectorApp::GetSystemPreferences(void) 						
{ 
	return m_SystemPrefs; 
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetApplicationPreferences
	Access:    	public 
	Parameter: 	void
	Returns:   	CApplicationPreferences&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CApplicationPreferences& CFootballDirectorApp::GetApplicationPreferences(void) 				
{ 
	return m_ApplicationPrefs; 
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetRegSystem
	Access:    	public 
	Returns:   	CRegistrationUI*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRegistrationUI* CFootballDirectorApp::GetRegSystem()									
{ 
	return m_pRegSystem; 
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetSaveDirectory
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFootballDirectorApp::GetSaveDirectory()
{
	return GetSystemPreferences().GetSaveDirectory();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetActiveView
	Access:    	public 
	Returns:   	CScreen03View*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen03View* CFootballDirectorApp::GetActiveView()
{
	return static_cast<CScreen03View*>(static_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveView());
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnIncrementProgress
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnIncrementProgress()
{
	GetActiveView()->OnIncrementProgress();	
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetWorldData
	Access:    	public
	Returns:   	CWorld&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CWorld& CFootballDirectorApp::GetWorldData()
{
	return *m_WorldData;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnHelp
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnHelp()
{
	CreateProcess(TEXT("peghelp.exe"), TEXT("file:FDHelp.htm#Main Menu"), null, null, FALSE, 0, null, null, null, null);
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnExitGame
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnExitGame()
{
	if (AfxMessageBox(IDS_WISHTOSAVE, MB_YESNO) == IDYES)
	{
		OnSaveGame();
	}
	OnExitApp();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnExitApp
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnExitApp()
{
	m_bIsInitialised = false;			// For OnPaint
	if (IsRegistered() == false)
	{
	}
	if (m_pRegSystem)
	{
		m_pRegSystem->stop();
		delete m_pRegSystem;
		m_pRegSystem = NULL;
	}
	PostQuitMessage(0);
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::HasOwnerName
	Access:    	public 
	Parameter: 	const bool bDisplayWarning
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFootballDirectorApp::HasOwnerName(const bool bDisplayWarning /*= false*/)
{
	if (strlen(GetSystemPreferences().GetUserName()) == 0)
	{
		if (bDisplayWarning == true)
		{
			AfxMessageBox(IDS_NOUSERNAME);
		}
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::IsRegistered
	Access:    	public 
	Parameter: 	const bool bDisplayWarning
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFootballDirectorApp::IsRegistered(const bool bDisplayWarning /* = false*/)
{
	if (!GetRegSystem()->isRegistered())
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::OnInitialiseGame
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::OnInitialiseGame()
{
//	ASSERT(0);
	//CScreen87View* pView = static_cast<CScreen87View*>(SwitchToView(LOADINGVIEW));
	CWaitCursor wait;
    m_WorldData = new CWorld();
    m_WorldData->DoInitialise();
//	ASSERT(0);
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::IsInitialised
	Access:    	public 
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const bool CFootballDirectorApp::IsInitialised()
{
	return m_bIsInitialised;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::UpdateTitleBar
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::UpdateTitleBar()
{
	m_pMainWnd->SetWindowText(TEXT("Football Director08"));
	GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetDate
	Access:    	public 
	Returns:   	CCalendar&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CFootballDirectorApp::GetDate()
{
	return GetWorldData().GetCurrentDate();
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::WhichCountry
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CFootballDirectorApp::WhichCountry()
{
	return m_WhichCountry;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::SetWhichCountry
	Access:    	public 
	Parameter: 	const int _CountryID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::SetWhichCountry(const int _CountryID)
{
	m_WhichCountry = _CountryID;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::GetBasicDisplay
	Access:    	public 
	Returns:   	CBasicDisplay&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay& CFootballDirectorApp::GetBasicDisplay()
{
	return m_Display;
}


/*------------------------------------------------------------------------------
	Method:   	CFootballDirectorApp::InitDebugMessages
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFootballDirectorApp::InitDebugMessages()
{
#ifdef _DEBUG

   afxTraceFlags = 0;

   // *** UNCOMMENT NEXT LINE if you want multi-app debugging ***
   // afxTraceFlags |= traceMultiApp;

   // *** UNCOMMENT NEXT LINE if you want main message pump trace (includes DDE) ***
   //afxTraceFlags |= traceAppMsg;

   // *** UNCOMMENT NEXT LINE if you want Windows message tracing ***
   afxTraceFlags |= traceWinMsg;

   // *** UNCOMMENT NEXT LINE if you want Windows command routing trace ***
  // afxTraceFlags |= traceCmdRouting;

   // *** UNCOMMENT NEXT LINE if you want special OLE callback trace ***
   // afxTraceFlags |= traceOle;

   // *** UNCOMMENT NEXT LINE if you want special database trace ***
   // afxTraceFlags |= traceDatabase;

   // *** UNCOMMENT NEXT LINE if you want special Internet client trace ***
   // afxTraceFlags |= traceInternet;

#endif /* DEBUG */
}
