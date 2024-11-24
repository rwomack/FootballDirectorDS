#include "stdafx.h"

#include "UI.h"


/*------------------------------------------------------------------------------
	Method:   	CContractEndDateSelector::CContractEndDateSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndDateSelector::CContractEndDateSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndDateSelector::~CContractEndDateSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndDateSelector::~CContractEndDateSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndDateSelector::DoAddDates
	Access:    	public 
	Parameter: 	const ushort _SeasonsToAddFirst
	Parameter: 	const ushort _nHowMany
	Parameter: 	const ushort _nCursel
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractEndDateSelector::DoAddDates(const ushort _SeasonsToAddFirst, const ushort _nHowMany, const ushort _nCursel)
{
	CTextSelector::DoRemoveAllData();
	m_NumberSeasonsFirst = _SeasonsToAddFirst;
	CContractEndTime TheDate(WorldData().GetCurrentDate());
	TheDate.DoEndOfSeason();
	uint LoopCount;
	for (LoopCount = 0; LoopCount < _SeasonsToAddFirst; LoopCount++)
	{
		TheDate.AddAYear();
	}
	for (LoopCount = 0; LoopCount < _nHowMany; LoopCount++)
	{
		CTextSelector::DoAddItem(TheDate.LongDateStr());
		TheDate.AddAYear();
	}
	CTextSelector::SetCurSel(_nCursel);
}


/*------------------------------------------------------------------------------
	Method:   	CContractEndDateSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CContractEndDateSelector::GetCurSel()
{
	// Seasons
	return CTitledTextSelector::GetCurSel() + m_NumberSeasonsFirst;
}