
 /*

        Class:- Leading Disciplinary Points


 		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "LeadingGoalScorer.h"
#include "LeadingDisciplinaryPoints.h"


CPlayerClub::CPlayerClub(const ushort _PlayerID, const ushort _ClubID)
	: m_PlayerID(_PlayerID)
	, m_ClubID(_ClubID)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerClub::GetPlayerNum
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerClub::GetPlayerNum() const
{
	return m_PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerClub::GetClubNum
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerClub::GetClubNum() const
{
	return m_ClubID;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CLeadingDiscipinaryPoints::CLeadingDiscipinaryPoints
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	const ushort _ClubID
	Parameter: 	const byte _YellowCards
	Parameter: 	const byte _RedCards
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeadingDiscipinaryPoints::CLeadingDiscipinaryPoints(const ushort _PlayerID, const ushort _ClubID, const byte _YellowCards, const byte _RedCards) 
	: CPlayerClub(_PlayerID, _ClubID)
{
	m_YellowCards = _YellowCards;
	m_RedCards = _RedCards;
 }


/*------------------------------------------------------------------------------
	Method:   	CLeadingDiscipinaryPoints::GetYellow
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeadingDiscipinaryPoints::GetYellow() const
{
	return m_YellowCards;
}


/*------------------------------------------------------------------------------
	Method:   	CLeadingDiscipinaryPoints::GetRed
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeadingDiscipinaryPoints::GetRed() const
{
	return m_RedCards;
}


/*------------------------------------------------------------------------------
	Method:   	CLeadingDiscipinaryPoints::GetTotalScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeadingDiscipinaryPoints::GetTotalScored() const
{
	return m_YellowCards * BOOKINGPOINTS + m_RedCards * SENDINGOFFPOINTS;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CLeadingDisciplinaryList::Sort
	Access:    	public 
	Parameter: 	const uint _Left
	Parameter: 	const uint _Right
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeadingDisciplinaryList::Sort(const int _Left, const int _Right)
{
	EXT_ASSERT(_Right < GetSize() && _Left < _Right);
    int i, j, x;
    CLeadingDiscipinaryPoints* TempPtr;

    i = _Left;
    j = _Right;
    x = static_cast<CLeadingDiscipinaryPoints*> (GetAt((_Left + _Right) / 2))->GetTotalScored();

	do
	{
        while((static_cast<CLeadingDiscipinaryPoints*>(GetAt(i))->GetTotalScored() > x) && i < _Right) i++;
        while((static_cast<CLeadingDiscipinaryPoints*>(GetAt(j))->GetTotalScored() < x) && j > _Left) j--;
        if(i <= j)
		{
            TempPtr = static_cast<CLeadingDiscipinaryPoints*> (GetAt(i));
            SetAt(i, static_cast<CLeadingDiscipinaryPoints*> (GetAt(j)));
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
	Method:   	CLeadingDisciplinaryList::CheckForList
	Access:    	public 
	Parameter: 	CDivision& _Division
	Returns:   	bool
	Qualifier: 	
	Purpose:	Are there any players available for the list?
------------------------------------------------------------------------------*/
bool CLeadingDisciplinaryList::CheckForList(CDivision& _Division)
{
	bool iRet = false;
	RemoveAll();   // Clear the list from previous division
    for (uint currclub = 0; currclub < _Division.GetNumberOfClubsInDivision(); currclub++)
	{
         _Division.GetClub(currclub).GetFirstTeamSquad().AddLeadingDiscipinaryPoints(_Division.GetFirstClubNumberInDivision() + currclub, 1, this);
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
	Method:   	CLeadingDisciplinaryList::CreateList
	Access:    	public 
	Parameter: 	CDivision& _Division
	Parameter: 	const ushort _ListSize
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeadingDisciplinaryList::CreateList(CDivision& _Division, const ushort _ListSize)
{
	ushort MinPoints = 1;
	if (_Division.GetFirstClubInDivision().GetTableData().GetNumberGamesPlayed() > 10)
	{
		MinPoints = 6;
	}
    for (uint currclub = 0; currclub < _Division.GetNumberOfClubsInDivision(); currclub++)
	{
		_Division.GetClub(currclub).GetFirstTeamSquad().AddLeadingDiscipinaryPoints(_Division.GetFirstClubNumberInDivision() + currclub, MinPoints, this);
	}

    int num;
	bool DeletedOne;
	// Don't have too big a list
	while  (GetSize() > _ListSize)
	{
		num = 0;
		while(num < GetSize() && GetSize() > _ListSize)
		{
			DeletedOne = true;		// Go round inner loop
			while(DeletedOne == true && num < GetSize())
			{
				DeletedOne = false;
                if (static_cast<CLeadingDiscipinaryPoints*> (GetAt(num))->GetTotalScored() <= MinPoints)
				{
					RemoveAt(num);
					DeletedOne = true;
				}
			}
			num++;
		}
		MinPoints++;
	}

	if (GetSize() > 1)
	{
		Sort(0, GetUpperBound());
	}
}
