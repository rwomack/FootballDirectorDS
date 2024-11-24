
/*

	EmployeeGrid.cpp : implementation file


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/
#include "stdafx.h"

#include "UI.h"
#include "EmployeeGrid.h"


/*------------------------------------------------------------------------------
Method:   	CEmployeeGrid::DoBuildGrid
Access:    	public 
Parameter: 	CEmployeeList & _List
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CEmployeeGrid::DoBuildGrid(CEmployeeList& _List)
{
	RemoveAllData();
	DoAddColumn(110);
	DoAddColumn(20, COL_NUMERICR);
	DoAddColumn(80, COL_ALIGNC);
	DoAddRows(_List.GetSize());
	CEmployee* pEmployee; 
	for (int LoopCount = 0 ; LoopCount < _List.GetSize(); LoopCount++)
	{
		pEmployee = (CEmployee*)_List.GetAt(LoopCount);
		EXT_ASSERT_POINTER(pEmployee, CEmployee);
		SetRowPointer(LoopCount, pEmployee);
		SetItemTextColour(LoopCount, 0, PosnColour[pEmployee->GetType()]);
		SetItemText(LoopCount, 0, pEmployee->GetName());
		SetItemText(LoopCount, 1, pEmployee->GetSkillStr());
		SetItemText(LoopCount, 2, pEmployee->TypeTextStr());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeGrid::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _CWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeGrid::OnInitialUpdate(CWnd* _CWnd, CBasicDisplay* _BasicDisplay)
{
	CGrid::OnInitialUpdate(_CWnd, _BasicDisplay, LC_SHOWCOLUMNS);
	CGrid::DoLoadBitmapBars(_CWnd->m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeGrid::SetCurrentEmployee
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeGrid::SetCurrentEmployee()
{
	WorldData().SetCurrentEmployee((CEmployee*)GetCurrentRowPointer());
}
