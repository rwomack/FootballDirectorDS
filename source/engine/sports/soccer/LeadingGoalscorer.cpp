
 /*

        Class:- Leading goalscorers


 		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "LeadingGoalScorer.h"


/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorer::CLeadingGoalScorer
	Access:    	public 
	Parameter: 	ushort PNum
	Parameter: 	ushort CNum
	Parameter: 	byte LScore
	Parameter: 	byte CScore
	Returns:   	
	Qualifier: 	: CPlayerClub(PNum, CNum)
	Purpose:
------------------------------------------------------------------------------*/
CLeadingGoalScorer::CLeadingGoalScorer(ushort PNum, ClubID CNum, byte LScore, byte CScore) :
	CPlayerClub(PNum, CNum)
{
	m_LeagueScored = LScore;
	m_CupScored = CScore;
	m_Scored = LScore + CScore;
 }

	
/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorer::GetLeagueScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CLeadingGoalScorer::GetLeagueScored() const						
{ 
	return m_LeagueScored; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorer::GetCupScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CLeadingGoalScorer::GetCupScored() const						
{ 
	return m_CupScored; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorer::GetTotalScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeadingGoalScorer::GetTotalScored() const						
{ 
	return m_Scored; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CLeadingGoalScorerList::~CLeadingGoalScorerList()
{
	RemoveList();
}

/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorerList::Sort
	Access:    	public 
	Parameter: 	const uint left
	Parameter: 	const uint right
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeadingGoalScorerList::Sort(const int _Left, const int _Right)
{
	EXT_ASSERT(_Right < GetSize() && _Left < _Right);
    int i, j, x;
    CLeadingGoalScorer* TempPtr;

    i = _Left;
    j = _Right;
    x = GetAt((_Left + _Right) / 2)->GetTotalScored();

	do
	{
        while((GetAt(i)->GetTotalScored() > x) && i < _Right) i++;
        while((GetAt(j)->GetTotalScored() < x) && j > _Left) j--;
        if(i <= j)
		{
            TempPtr = GetAt(i);
            SetAt(i, GetAt(j));
			SetAt(j, TempPtr);
            i++;
            j--;
		}
    }
	while(i <= j);

    if (_Left < j) 
	{
		Sort(_Left, j);
	}
    if (i < _Right) 
	{
		Sort(i, _Right);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorerList::CheckForList
	Access:    	public 
	Parameter: 	CDivision& _Division
	Returns:   	bool
	Qualifier: 	
	Purpose:	Are there any players available for the list?
------------------------------------------------------------------------------*/
bool CLeadingGoalScorerList::CheckForList(CDivision& _Division)
{
	bool iRet = false;
	RemoveAll();   // Clear the list from previous division
    for (int currclub = 0; currclub < _Division.GetNumberOfClubsInDivision(); currclub++)
	{
		ClubID cClubID = _Division.GetClubs()[currclub];
        WorldData().GetClubByID(cClubID).GetFirstTeamSquad().AddLeadScorers(cClubID, 1, this);
		// Exit as soon as we have an entry
		if (GetSize() > 0)
		{
			iRet = true;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CLeadingGoalScorerList::DoCreateList
	Access:    	public 
	Parameter: 	CDivision& _Division
	Parameter: 	const ushort iListSize
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeadingGoalScorerList::DoCreateList(CDivision& _Division, const ushort _ListSize)
{
	RemoveAll();   // Clear the list from previous division
    for (int currclub = 0; currclub < _Division.GetNumberOfClubsInDivision(); currclub++)
	{
		ClubID cClubID = _Division.GetClubs()[currclub];
		WorldData().GetClubByID(cClubID).GetFirstTeamSquad().AddLeadScorers(cClubID, 1, this);
	}

	ushort MinGoals = 2;
    int num;
	bool DeletedOne;
	// Don't have too big a list
	while  (GetSize() > _ListSize)
	{
		num = 0;
		while(num < GetSize())
		{
			DeletedOne = true;		// Go round inner loop
			while(DeletedOne == true && num < GetSize())
			{
				DeletedOne = false;
                if (GetAt(num)->GetTotalScored() <= MinGoals)
				{
					DeleteOne(num);
					DeletedOne = true;
				}
			}
			num++;
		}
		MinGoals++;
	}

	if (GetSize() > 1)
	{
		Sort(0, GetUpperBound());
	}
}
