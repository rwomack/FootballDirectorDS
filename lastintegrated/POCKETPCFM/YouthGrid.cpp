#include "stdafx.h"

#include "UI.h"
#include "YouthGrid.h"


/*------------------------------------------------------------------------------
	Method:   	CYouthGrid::DoBuildGrid
	Access:    	public 
	Parameter: 	CYouthList & _List
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CYouthGrid::DoBuildGrid(CYouthList& _List)
{
	RemoveAllData();
	DoAddColumn(110);
	DoAddColumn(20, COL_NUMERICR);
	DoAddColumn(80, COL_ALIGNC);
	DoAddRows(_List.GetSize());
	CYouth* pYouth; 
	for (int LoopCount = 0 ; LoopCount < _List.GetSize(); LoopCount++)
	{
		pYouth = (CYouth*)_List.GetAt(LoopCount);
		EXT_ASSERT_POINTER(pYouth, CYouth);
		SetRowPointer(LoopCount, pYouth);
		SetItemTextColour(LoopCount, 0, PosnColour[pYouth->GetSkills().GetPosition()]);
		SetItemText(LoopCount, 0, pYouth->GetName());
		SetItemNum(LoopCount, 1, pYouth->GetSkills().GetOverallSkill());
		SetItemStrId(LoopCount, 2, PlayerPositionText[pYouth->GetSkills().GetPosition()]);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthGrid::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _CWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CYouthGrid::OnInitialUpdate(CWnd* _CWnd, CBasicDisplay* _BasicDisplay)
{
	CGrid::OnInitialUpdate(_CWnd, _BasicDisplay, LC_SHOWCOLUMNS);
	CGrid::DoLoadBitmapBars(_CWnd->m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
}


/*------------------------------------------------------------------------------
	Method:   	CYouthGrid::SetCurrentYouth
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CYouthGrid::SetCurrentYouth()
{
	WorldData().SetCurrentYouth((CYouth*)GetCurrentRowPointer());
}

