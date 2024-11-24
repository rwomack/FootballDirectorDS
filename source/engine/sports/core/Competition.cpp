
 /*

	Program:- CCompetition.cpp

    The competition class routines


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "Competition.h"


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::DoAddCup
	Access:    	public 
	Parameter: 	CCup * pCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCompetitionList::DoAddCup(CCup* pCup)
{
	m_CupList.Add(pCup);
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::DoAddCharityShield
	Access:    	public 
	Parameter: 	CCharityShield * _CharityShield
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCompetitionList::DoAddCharityShield(CCharityShield* _CharityShield)
{
	m_CharityShieldList.Add(_CharityShield);
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::GetNumRounds
	Access:    	public 
	Parameter: 	const ushort CupId
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCompetitionList::GetNumRounds(const ushort CupId)
{
	return GetCup(CupId)->GetRounds().GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::GetSize
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCompetitionList::GetSize()
{
	return static_cast<ushort>(m_CupList.GetSize() + m_CharityShieldList.GetSize());
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::GetTitle
	Access:    	public 
	Parameter: 	const ushort CupId
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCompetitionList::GetTitle(const ushort CupId)
{
	return GetCup(CupId)->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::pCupRound
	Access:    	public 
	Parameter: 	const ushort CupId
	Parameter: 	const ushort iRoundNum
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCompetitionList::GetCupRound(const ushort CupId, const ushort iRoundNum)
{
	return &(GetCup(CupId)->GetRounds()[iRoundNum]);
}


/*------------------------------------------------------------------------------
	Method:   	CCompetitionList::pCup
	Access:    	public 
	Parameter: 	const ushort CupId
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CCompetitionList::GetCup(const ushort CupId)
{
	EXT_ASSERT(CupId < GetSize());
	if (CupId < m_CupList.GetSize())
	{
		return static_cast<CCup*>(m_CupList.GetAt(CupId));
	}
	else
	{
		return static_cast<CCharityShield*>(m_CharityShieldList.GetAt(CupId - m_CupList.GetSize()));
	}
}
