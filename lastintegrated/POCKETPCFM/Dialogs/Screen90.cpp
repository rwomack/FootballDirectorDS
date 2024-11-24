 /*

    Class:- CScreen90 - Load or Save game


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "SaveGameHeader.h"

#include "Screen90.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen90 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::CScreen90Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen90Dlg::CScreen90Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen90Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen90Dlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_ACTION1, OnAction1Clicked)
	ON_COMMAND(ID_ACTION2, OnAction2Clicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen90 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen90Load::OnAction1Clicked
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Load::OnAction1Clicked()
{
	int CurrRow = CBitmapDialog::GetGrid().GetCurrentRow();
	if (CurrRow != NOTFOUND)
	{
		OnLoadOrSave(CurrRow);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Load::OnAction2Clicked
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Load::OnAction2Clicked()
{
	CDialog::EndDialog(IDCANCEL);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Save::OnAction1Clicked
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Save::OnAction1Clicked()
{
	int CurrRow = CBitmapDialog::GetGrid().GetCurrentRow();
	if (CurrRow != NOTFOUND)
	{
		OnLoadOrSave(CurrRow);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Save::OnAction2Clicked
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Save::OnAction2Clicked()
{
	CDialog::EndDialog(IDCANCEL);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen90Dlg::OnInitDialog()
{
	BOOL bRet = CBitmapDialog::OnInitDialog(IDS_HELPSCREEN99, IDS_SCREEN90);
	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	theGrid.SetRowHeight(32);
	theGrid.DoAddColumn(40, COL_BITMAP);		// Club home shirt
	theGrid.DoAddColumn(100, COL_NORMAL);	// Club name
	theGrid.DoAddColumn(80, COL_NORMAL);		// Game date
	theGrid.DoAddRows(NUMONSCREEN);

	m_SaveGame[0].SetFileNameFromID(IDS_SAVEGAMEFILE01);
	m_SaveGame[1].SetFileNameFromID(IDS_SAVEGAMEFILE02);
	m_SaveGame[2].SetFileNameFromID(IDS_SAVEGAMEFILE03);
	m_SaveGame[3].SetFileNameFromID(IDS_SAVEGAMEFILE04);
	HDC dc = ::GetDC(m_hWnd);
	m_pngLargeKits.DoInitialise(dc, IDR_PNGLARGEHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);
	DoBuildList();
	SetIsInitialised(true);
 	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::DoBuildList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::DoBuildList()
{
	CDataFile theDataFile;
	CString* pString;
	WIN32_FIND_DATA FileData;   // Data structure describes the file found
	CString FileName;
	FileName.LoadString(IDS_SAVEGAMEFILE01);
	HANDLE hSearch;
	CGrid& theGrid = CBitmapDialog::GetGrid();
	for (int LoopCount = 0; LoopCount < NUMONSCREEN; LoopCount++)
	{
		hSearch= FindFirstFile(theApp.GetSaveDirectory() + m_SaveGame[LoopCount].GetFileName(), &FileData);
		if (hSearch != INVALID_HANDLE_VALUE)
		{
			pString = new CString(FileData.cFileName);
			theDataFile.OpenForRead(*pString);
			delete pString;
			m_SaveGame[LoopCount].Read(&theDataFile);
			theGrid.SetItemBitmap(LoopCount, 0, &m_pngLargeKits, m_SaveGame[LoopCount].GetKitID());
			theGrid.SetItemText(LoopCount, 1, m_SaveGame[LoopCount].GetClubName());
			theGrid.SetItemText(LoopCount, 2, m_SaveGame[LoopCount].GetCalendar().ShortDateStr());
			theDataFile.Close();
		}
		else
		{
			theGrid.SetItemBitmap(LoopCount, 0, &m_pngLargeKits, NOTFOUND);
			theGrid.SetItemStrId(LoopCount, 1, IDS_EMPTY);
		}
		theGrid.ResetScrollBar();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Check if clicked on file name list
	int CurrRow = CBitmapDialog::GetGrid().GetCurrentRow(point);
	if (CurrRow != NOTFOUND)
	{
		OnLoadOrSave(CurrRow);
		DoBuildList();
		CBitmapDialog::DoDisableOKButton();
	}
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Load::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen90Load::OnInitDialog()
{
	BOOL bRet = CScreen90Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_SCREEN90LOADMENU);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Save::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen90Save::OnInitDialog()
{
	BOOL bRet = CScreen90Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_SCREEN90SAVEMENU);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Load::OnLoadOrSave
	Access:    	virtual protected 
	Parameter: 	const int _FileNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Load::OnLoadOrSave(const int _FileNum)
{
	CWaitCursor theCursor;
	// Load game
	CString msg = TEXT("Loading");
	theApp.GetMainWnd()->UpdateWindow();
	if (WorldData().GetGameFromDisk(theApp.GetSaveDirectory() + m_SaveGame[_FileNum].GetFileName()) == true)
	{
		msg = TEXT("Loaded OK");
		CDialog::EndDialog(IDIGNORE);
	}
	else
	{
		msg = TEXT("Loading Failed");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Save::OnLoadOrSave
	Access:    	virtual protected 
	Parameter: 	const int _FileNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Save::OnLoadOrSave(const int _FileNum)
{
	CBitmapDialog::DoDisableOKButton();
	CWaitCursor theCursor;
	if (WorldData().SaveGameFile(theApp.GetSaveDirectory() + m_SaveGame[_FileNum].GetFileName()) == true)
	{
		//if (WorldData().GetGameFromDisk(m_SaveGame[_FileNum].GetFileName()) == true)
		{
		//	_FileNum = TEXT("Loaded OK");
		}
		EndDialog(IDOK);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::OnOK()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen90Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen90Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);

		int RowsToDisplay = 4;
		CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, TOPY, GetCentreX() + GRID_WIDTH / 2, TOPY + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
	}

	Sleep(1);
}