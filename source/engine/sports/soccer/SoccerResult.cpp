
/*
	File:- SoccerResult.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultDetails::CSoccerResultDetails
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultDetails::CSoccerResultDetails()
{
	m_ClubID.id = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultDetails::SetClubDetails
	Access:    	public 
	Parameter: 	ushort nClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResultDetails::SetClubDetails(ClubID _ClubID)
{
    m_ClubID = _ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultDetails::GetName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CSoccerResultDetails::GetName()
{
	return WorldData().GetClubByID(m_ClubID).GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultDetails::IsUserControlledClub
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CSoccerResultDetails::IsUserControlledClub()
{
	return WorldData().GetClubByID(m_ClubID).IsUserControlled();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultDetails::GetClubID
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CSoccerResultDetails::GetClubID() const							
{ 
	return m_ClubID; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::CSoccerResult
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResult::CSoccerResult()
	: m_pFixture(NULL)
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::CSoccerResult
	Access:    	public 
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResult::CSoccerResult(const ClubID _HomeClubID, const ClubID _AwayClubID)
	: m_pFixture(NULL)
{
	DoAddClubIDs(_HomeClubID, _AwayClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::DoAddClubIDs
	Access:    	public 
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResult::DoAddClubIDs(const ClubID _HomeClubID, const ClubID _AwayClubID)
{
	m_HomeClubDetails.SetClubDetails(_HomeClubID);
	m_AwayClubDetails.SetClubDetails(_AwayClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::GetAwayClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CSoccerResult::GetAwayClub()
{
	return WorldData().GetClubByID(m_AwayClubDetails.GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::GetHomeClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CSoccerResult::GetHomeClub()
{
	return WorldData().GetClubByID(m_HomeClubDetails.GetClubID());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::GetAttendance
	Access:    	public const 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerResult::GetAttendance() const
{
	return m_pFixture->GetAttendance();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::GetFixture
	Access:    	public 
	Parameter: 	void
	Returns:   	CFixture*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture*	CSoccerResult::GetFixture()
{
	return m_pFixture;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::SetResult
	Access:    	public 
	Parameter: 	CFixture* _pFixture
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResult::SetResult(CFixture* _Fixture)
{
    EXT_ASSERT(_Fixture != NULL);
    m_pFixture = _Fixture;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::GoalsStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CSoccerResult::GoalsStr()
{
	CString str(TEXT("-"));

	if (GetFixture() != NULL)
	{
		str.Format(TEXT("%d-%d"), GetFixture()->GetNumberGoals(HOME), GetFixture()->GetNumberGoals(AWAY));
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::pHomeClubDetails
	Access:    	public 
	Parameter: 	void
	Returns:   	CSoccerResultDetails*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultDetails& CSoccerResult::GetHomeClubDetails()
{
	return m_HomeClubDetails;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResult::pAwayClubDetails
	Access:    	public 
	Parameter: 	void
	Returns:   	CSoccerResultDetails*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultDetails& CSoccerResult::GetAwayClubDetails()
{
	return m_AwayClubDetails;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultList::CSoccerResultList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultList::CSoccerResultList()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultList::~CSoccerResultList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultList::~CSoccerResultList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultList::SortByHomeClub
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResultList::SortByHomeClub()
{
	CSoccerResult* TempFixt;
    bool swap = true;
	while(swap == true)
	{
		swap = false;
        for (int LoopCount = 0; LoopCount < GetSize() - 1; LoopCount++)
		{
            if (GetAt(LoopCount)->GetHomeClubDetails().GetName() > GetAt(LoopCount + 1)->GetHomeClubDetails().GetName())
			{
                TempFixt = GetAt(LoopCount);
                SetAt(LoopCount, GetAt(LoopCount + 1));
                SetAt(LoopCount + 1, TempFixt);
				swap = true;
			}
		}
	}
}
