
 /*

        Class:- ClubList

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CClubList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_CLUB, TEXT("FDDATA"));
	ushort HowMany = theResource.ReadUInt16();
	WorldData().SetTotalClubs(HowMany);
    SetSize(HowMany);

	CClub* pNewClub;
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
		pNewClub = new CClub;
        SetAt(ListPos, pNewClub);
		pNewClub->SetID(ListPos);
		pNewClub->DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoAddCupClubs
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Parameter: 	CCountry& rCountry
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CClubList::DoAddCupClubs(CBinaryReader& _Resource, CCountry& _Country)
{
	byte nHowManyClubsToAdd = _Resource.ReadByte();
	if (nHowManyClubsToAdd > 0)
	{
		WorldData().SetTotalClubs(WorldData().GetTotalClubs() + nHowManyClubsToAdd);
		ushort iOldSize = GetSize();
		SetSize(iOldSize + nHowManyClubsToAdd);

		CExtString  szName;
		CClub* pNewClub;
		for (uint ListPos = 0; ListPos < nHowManyClubsToAdd; ListPos ++)
		{
			szName.ReadFromResource(_Resource);
			EXT_ASSERT(szName.IsEmpty() != true);
			pNewClub = new CClub;
			SetAt(ListPos + iOldSize, pNewClub);
			pNewClub->DoInitialiseFromResource(szName, _Resource, _Country);
		}
	}
	return nHowManyClubsToAdd;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::GetAt
	Access:    	public 
	Parameter: 	int _Index
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CClubList::GetAt(int _Index)
{
	return *(CPointerArray<CClub>::GetAt(_Index));
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::GetByID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CClubList::GetByID(const ushort _ClubID)
{
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		if (GetAt(LoopCount).DoFindID() == _ClubID)
		{
			return GetAt(LoopCount);
		}
	}

	// Shouldn't happen!
	EXT_ASSERT(false);
	return GetAt(0);
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoWeeklyClearDown
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoWeeklyClearDown()
{
    for (uint ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
		if (GetAt(ListPos).IsNonLeagueClub() == false)
		{
			GetAt(ListPos).DoWeeklyClearDown();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoSwapClubDivisionPositions
	Access:    	public 
	Parameter: 	ushort _Club1ID
	Parameter: 	ushort _Club2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoSwapClubDivisionPositions(ushort _Club1ID, ushort _Club2ID)
{
	byte a = GetAt(_Club1ID).GetTableData().GetLeaguePosition();
	GetAt(_Club1ID).GetTableData().SetLeaguePosition(GetAt(_Club2ID).GetTableData().GetLeaguePosition());
	GetAt(_Club2ID).GetTableData().SetLeaguePosition(a);
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::SwapClubIDs
	Access:    	public 
	Parameter: 	const ushort aClubID1
	Parameter: 	ushort aClubID2
	Returns:   	void
	Qualifier: 	
	Purpose:	Swap 2 clubs positions in the list
				Assumes they are both in the same country
------------------------------------------------------------------------------*/
void CClubList::DoSwapClubIDs(const ushort _Club1ID, ushort _Club2ID)
{
	EXT_ASSERT(_Club1ID != _Club2ID);
	EXT_ASSERT(_Club1ID  < GetSize() && _Club2ID < GetSize());

    CClub* Temp = &GetAt(_Club1ID);
    SetAt(_Club1ID, &GetAt(_Club2ID));
    SetAt(_Club2ID, Temp);

	WorldData().GetManagerList().DoSwapClubIDs(_Club1ID, _Club2ID);

	/*    CUser* HumanPtr = WorldData().pUser(CurrManager);
	EXT_ASSERT(HumanPtr != null);
    if (HumanPtr->GetManagerID() > NOMANAGER && HumanPtr->GetManagerID() < WorldData().UserList()->GetSize()) {
        WorldData().pManagerList()->MakeCurrentManager(CurrManager);        // Ensures set up correct
	} */

	// Change divisions
    byte  a = GetAt(_Club2ID).GetDivisionID();
    GetAt(_Club2ID).SetDivisionID(GetAt(_Club1ID).GetDivisionID());
    GetAt(_Club1ID).SetDivisionID(a);
	// Change position in divisions
    a = GetAt(_Club2ID).GetTableData().GetLeaguePosition();
    GetAt(_Club2ID).GetTableData().SetLeaguePosition(GetAt(_Club1ID).GetTableData().GetLeaguePosition());
    GetAt(_Club1ID).GetTableData().SetLeaguePosition(a);

	GetAt(_Club2ID).GetActiveCountry().DoSwapClubIDs(_Club1ID, _Club2ID);

	WorldData().GetUserList().DoCheckTransferNegotiationsForSwapClubs(_Club1ID, _Club2ID);
	WorldData().GetContractClauseList().DoCheckForSwapClubs(_Club1ID, _Club2ID);
	WorldData().GetPlayerList().DoCheckForSwapClubs(_Club1ID, _Club2ID);
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::Read(CDataFile* _pDataFile)
{
    for (uint ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
        GetAt(ListPos).Read(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::Write(CDataFile* _pDataFile)
{
    for (uint ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
        GetAt(ListPos).Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::SetManagerID
	Access:    	public 
	Parameter: 	uint _IndVal
	Parameter: 	ushort _ManNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::SetManagerID(int _IndVal, ushort _ManNum)
{
    EXT_ASSERT(_IndVal > NOTFOUND && _IndVal < GetSize());
    GetAt(_IndVal).SetManagerID(_ManNum);
 }


/*------------------------------------------------------------------------------
	Method:   	CClubList::PostInitialise
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::PostInitialise()
{
	ushort HowMany = WorldData().GetTotalClubs();
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
		if (GetAt(ListPos).IsNonLeagueClub() == false)
		{
			GetAt(ListPos).PostInitialise();
		}
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CClubList::FindPlayerClub
	Access:    	public 
	Parameter: 	const ushort a_nPlayerID
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CClubList::DoFindPlayerClub(const ushort a_nPlayerID)
{
    CClub* pClub;
	int ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
        pClub = &GetAt(ListPos);
		EXT_ASSERT_POINTER(pClub, CClub);
         if (pClub->GetFirstTeamSquad().IsPlayerInSquad(a_nPlayerID) == true)
		 {
			 return pClub;
		 }
	}
	EXT_ASSERT(ListPos > 0);
	return null;
 }


/*------------------------------------------------------------------------------
	Method:   	CClubList::FindPlayerClub
	Access:    	public 
	Parameter: 	CPlayer* pPlayer
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CClubList::DoFindPlayerClub(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
    CClub* pClub = null;
	uint ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
		if (GetAt(ListPos).GetFirstTeamSquad().IsPlayerInSquad(_Player) == true)
		{
			pClub = &GetAt(ListPos);
			break;
		}
	}
	EXT_ASSERT_POINTER(pClub, CClub);
	return pClub;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::FindPlayerClubNumber
	Access:    	public 
	Parameter: 	const ushort a_nPlayerID
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubList::DoFindPlayerClubNumber(const ushort _PlayerID)
{
    CClub* pClub;
	uint ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
        pClub = &GetAt(ListPos);
		EXT_ASSERT_POINTER(pClub, CClub);
         if (pClub->GetFirstTeamSquad().IsPlayerInSquad(_PlayerID) == true)
		 {
			 break;
		 }
	}
	EXT_ASSERT(ListPos < WorldData().GetTotalClubs());
	return ListPos;
 }


 /*------------------------------------------------------------------------------
	Method:   	CClubList::FindPlayerClubNumber
	Access:    	public 
	Parameter: 	CPlayer* pPlayer
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubList::DoFindPlayerClubNumber(CPlayer* pPlayer)
{
	uint ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
		if (GetAt(ListPos).GetFirstTeamSquad().IsPlayerInSquad(pPlayer) == true)
		{
			return ListPos;
		}
	}
	EXT_ASSERT(ListPos > 0);
	return null;
 }


 //---------------------------------------------------------------------------------------
CClub* CClubList::GetEmployeeClub(CEmployee* _Employee)
{
	EXT_ASSERT(_Employee != null);
	uint ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
		if (GetAt(ListPos).IsEmployeeAtClub(_Employee) == true)
		{
			return &GetAt(ListPos);
		}
	}
	EXT_ASSERT(ListPos > 0);
	return null;
 }


/*------------------------------------------------------------------------------
	Method:   	CClubList::GetYouthClub
	Access:    	public 
	Parameter: 	CYouth* pYouth
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CClubList::GetYouthClub(CYouth* _Youth)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	uint ListPos;
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos++)
	{
		if (GetAt(ListPos).IsYouthAtClub(_Youth) == true)
		{
			break;
		}
	}
	return GetAt(ListPos);
 }


/*------------------------------------------------------------------------------
	Method:   	CClubList::HowManyUnManagedClubs
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CClubList::HowManyUnManagedClubs()
{
	int iRet = 0;
    for (uint ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos ++)
	{
        if (GetAt(ListPos).IsNonLeagueClub() == false)
		{
            iRet += GetAt(ListPos).GetManagerID() == NOMANAGER;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::Find
	Access:    	public 
	Parameter: 	CClub* pClub
	Returns:   	ushort - the index(position in list) of the pointer
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubList::Find(CClub* _Club)
{
    EXT_ASSERT(_Club != null);
    ushort iRet = NOCLUB;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        if (&GetAt(ListPos) == _Club)
		{
            iRet = ListPos;
            break;
        }
    }
    EXT_ASSERT(iRet != NOCLUB);
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::Find
	Access:    	public 
	Parameter: 	CString ClubName
	Returns:   	ushort - the index(position in list) of the club name
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubList::Find(CString ClubName)
{
    EXT_ASSERT(ClubName.IsEmpty() != TRUE);
    ushort iRet = NOCLUB;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        if (GetAt(ListPos).GetName() == ClubName)
		{
            iRet = ListPos;
            break;
        }
    }
    EXT_ASSERT(iRet != NOCLUB);
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::SortTableData
	Access:    	public 
	Parameter: 	int iMode
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::SortTableData(int iMode)
{
	StLeagueTable Elements[26];
	StLeagueTable Element;
    signed int ra = 0;
	ushort n;
    for (n = 0; n < GetSize(); n++)
	{
        Elements[n].Posn = n;
		switch(iMode)
		{
			case 0:
				Elements[n].Points = GetAt(n).GetTableData().GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetGoalDifference ();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetNumberGoalsScored();
				break;
			case 1:
				Elements[n].Points = GetAt(n).GetTableData().GetHomeGames()->GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetHomeGames()->GetGoalDifference ();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetHomeGames()->GetNumberGoalsScored();
				break;
			case 2:
				Elements[n].Points = GetAt(n).GetTableData().GetAwayGames()->GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetAwayGames()->GetGoalDifference ();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetAwayGames()->GetNumberGoalsScored();
				break;
		}
    }

    // sort into order, points first
    // if points equal then best goal difference
    // if goal difference equal then highest goals scored
    for (int LoopCount = 1; LoopCount < GetSize(); LoopCount ++)
	{
        ra = LoopCount - 1;
		Element = Elements[LoopCount];

        do
		{
            if (Elements[ra].Points > Element.Points)
			{
                break;
			}

            if (Elements[ra].Points == Element.Points && \
				Elements[ra].GoalsDiff > Element.GoalsDiff)
			{
                break;
			}

            if (Elements[ra].Points == Element.Points && \
				Elements[ra].GoalsDiff == Element.GoalsDiff && \
				Elements[ra].GoalFor > Element.GoalFor)
			{
                break;
			}

            Elements[ra + 1] = Elements[ra];
			Swap(ra, ra + 1);
            ra --;
        }
		while(ra >= 0);

        Elements[ra + 1] = Element;
    }

	// Updated the league positions
    for (n = 0; n < GetSize(); n++)
	{
		GetAt(n).GetTableData().SetLeaguePosition(n);
	}
}


#ifdef ACTIVEFRIENDLIES
/*------------------------------------------------------------------------------
	Method:   	CClubList::CreateFriendlyMatch
	Access:    	public 
	Parameter: 	const ushort MatchNumber
	Parameter: 	const ushort HomeClub
	Parameter: 	const ushort AwayClub
	Returns:   	void
	Qualifier: 	
	Purpose:	Set up a friendly match on the requested date
				Assumes both clubs are available for a friendly on the date
------------------------------------------------------------------------------*/
void CClubList::CreateFriendlyMatch(const ushort MatchNumber, const ushort HomeClub, const ushort AwayClub)
{
	ushort FriendlyNumber = GetAt(HomeClub).MatchNumberToFriendlyNumber(MatchNumber);
	EXT_ASSERT(FriendlyNumber != NOFRIENDLY);
	GetAt(HomeClub).GetFriendlies().CreateFriendlyMatch(FriendlyNumber, AwayClub, HOME);

	FriendlyNumber = GetAt(AwayClub).MatchNumberToFriendlyNumber(MatchNumber);
	EXT_ASSERT(FriendlyNumber != NOFRIENDLY);
	GetAt(AwayClub).GetFriendlies().CreateFriendlyMatch(FriendlyNumber, HomeClub, AWAY);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CClubList::DailyTasks
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoDailyTasks()
{
	bool WeekEnd = WorldData().GetCurrentDate().IsWeekEnd();
    for (int ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos++)
	{
		if (GetAt(ListPos).IsNonLeagueClub() == false)
		{
			GetAt(ListPos).DoDailyTasks(WeekEnd);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::EnsureSomeUnManaged
	Access:    	public 
	Parameter: 	const ushort _NumberClubsToCheck
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::EnsureSomeUnManaged(const ushort _NumberClubsToCheck)
{
	uint HowMany, NumberToProcess, Count = 0;
    for (uint ListPos = 0; ListPos < _NumberClubsToCheck; ListPos ++)
	{
		if (GetAt(ListPos).IsNonLeagueClub() == false)
		{
			Count += (GetAt(ListPos).GetManagerID() == NOMANAGER);
		}
	}
	// If no clubs are unmanaged then make a few, ensure non are human managed
	if (Count == 0)
	{
		NumberToProcess = 3 + RandomNumber.IntLessThan(5);
		for (HowMany = 0; HowMany < NumberToProcess; HowMany++)
		{
			if (WorldData().GetClub(RandomNumber.IntLessThan(_NumberClubsToCheck)).IsNonLeagueClub() == false)
			{
				WorldData().GetClub(RandomNumber.IntLessThan(_NumberClubsToCheck)).DoSackManager();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoFindRandomClub
	Access:    	public 
	Parameter: 	void
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CClubList::DoFindRandomClub()
{
	CClub* pClub;
	int ListPos;
	if (GetUpperBound() > 0)
	{
		do
		{
			ListPos = RandomNumber.IntLessThan(GetUpperBound());
			pClub = CPointerArray<CClub>::GetAt(ListPos);
			EXT_ASSERT_POINTER(pClub, CClub);
		}
		while(pClub == null || pClub->IsNonLeagueClub() == true);
	}
	else
	{
		pClub = CPointerArray<CClub>::GetAt(0);
	}
	return pClub;
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::operator[]
	Access:    	public 
	Parameter: 	const ushort x
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CClubList::operator [](const ushort x)
{
	EXT_ASSERT(x < CPointerArray<CClub>::GetSize());
	return *(CPointerArray<CClub>::GetAt(x));
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoAddClubsSuitableForPlayer
	Access:    	public 
	Parameter: 	CPlayer& a_rPlayer
	Parameter: 	CClubList& a_rSuitableClubs
	Parameter: 	CClub* apClubToAvoid 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoAddClubsSuitableForPlayer(CPlayer& _Player, CClubList& _SuitableClubs, CClub* _ClubToAvoid /*= null*/)
{
    for (int ListPos = 0; ListPos < WorldData().GetTotalClubs(); ListPos++)
	{
  		if (GetAt(ListPos).IsNonLeagueClub() == false && GetAt(ListPos).IsUserControlled() == false && _ClubToAvoid != &GetAt(ListPos))
		{
			if (GetAt(ListPos).GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD && _Player.WillSignTransferMsg(GetAt(ListPos)) == TEXT(""))
			{
				//afxDump << "Suitable club " << GetAt(ListPos).GetName() << " cash " << GetAt(ListPos).GetCashBalance() << "\n";
				_SuitableClubs.Add(&GetAt(ListPos));
			}
		}
	}
}
