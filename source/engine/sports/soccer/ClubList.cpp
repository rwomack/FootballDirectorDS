
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
    	pNewClub = FD_BIN_NEW CClub;
        SetAt(ListPos, pNewClub);

		ClubID cNewId;
		cNewId.id = ListPos;
		pNewClub->SetID(cNewId);
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
      		pNewClub = FD_BIN_NEW CClub;
			SetAt(ListPos + iOldSize, pNewClub);
			ClubID cNewID(ListPos + iOldSize);
			pNewClub->SetID(cNewID);
			pNewClub->DoInitialiseCupClubFromResource(szName, _Resource, _Country);
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
CClub& CClubList::GetByID(const ClubID _ClubID)
{
	FDASSERT(GetAt(_ClubID.id).DoFindID() == _ClubID);
	return GetAt(_ClubID.id);
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
void CClubList::DoSwapClubDivisionPositions(ClubID _Club1ID, ClubID _Club2ID)
{
	CClub &c1 = GetByID(_Club1ID);
	CClub &c2 = GetByID(_Club2ID);

	byte a = c1.GetTableData().GetLeaguePosition();
	c1.GetTableData().SetLeaguePosition(c2.GetTableData().GetLeaguePosition());
	c2.GetTableData().SetLeaguePosition(a);
}


/*------------------------------------------------------------------------------
	Method:   	CClubList::DoPromoteAndRelegateClubs
	Access:    	public
	Parameter: 	ClubID cPromotedClub
	Parameter: 	ClubID cRelegatedClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClubList::DoPromoteAndRelegateClubs(ClubID cPromotedClub, ClubID cRelegatedClub)
{
	CClub &c1 = GetByID(cPromotedClub);
	CClub &c2 = GetByID(cRelegatedClub);

	// Change divisions
	c2.GetDivision().RemoveClub(cRelegatedClub);
	c1.GetDivision().RemoveClub(cPromotedClub);
    byte  a = c2.GetDivisionID();
    c2.SetDivisionID(c1.GetDivisionID());
    c1.SetDivisionID(a);
	c1.GetDivision().AddClub(cPromotedClub);
	c2.GetDivision().AddClub(cRelegatedClub);

	c1.GetActiveCountry().DoSwapClubIDsInStartingOrder(cPromotedClub, cRelegatedClub);

	// Change position in divisions
	DoSwapClubDivisionPositions(cPromotedClub, cRelegatedClub);
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
    GetAt(_IndVal).DoManagerJoinedClub(_ManNum);
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
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs()-1; ListPos ++)
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
    for (ListPos = 0; ListPos < WorldData().GetTotalClubs()-1; ListPos++)
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
    for (ushort n = 0; n < GetSize(); n++)
	{
        Elements[n].Posn = n;
		Elements[n].TheClub = GetAt(n).DoFindID();
		switch(iMode)
		{
			case 0:
				Elements[n].Points = GetAt(n).GetTableData().GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetGoalDifference ();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetNumberGoalsScored();
				break;
			case 1:
				Elements[n].Points = GetAt(n).GetTableData().GetHomeGames()->GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetHomeGames()->GetGoalDifference();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetHomeGames()->GetNumberGoalsScored();
				break;
			case 2:
				Elements[n].Points = GetAt(n).GetTableData().GetAwayGames()->GetPoints();
				Elements[n].GoalsDiff = GetAt(n).GetTableData().GetAwayGames()->GetGoalDifference ();
				Elements[n].GoalFor = GetAt(n).GetTableData().GetAwayGames()->GetNumberGoalsScored();
				break;
			default:
				FDASSERT(false && "Unknown view type");
				break;
		}
    }

	::qsort(Elements, GetSize(), sizeof(StLeagueTable), StLeagueTableSortFunc);

	// Updated the league positions
    for (ushort n = 0; n < GetSize(); n++)
	{
		GetAt(Elements[n].Posn).GetTableData().SetLeaguePosition(n);
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
void CClubList::CreateFriendlyMatch(const ushort MatchNumber, const ushort HomeClubIndex, const ushort AwayClubIndex)
{
	ushort FriendlyNumber = GetAt(HomeClubIndex).MatchNumberToFriendlyNumber(MatchNumber);
	EXT_ASSERT(FriendlyNumber != NOFRIENDLY);
	GetAt(HomeClubIndex).GetFriendlies().CreateFriendlyMatch(FriendlyNumber, GetAt(AwayClubIndex).DoFindID(), HOME);

	FriendlyNumber = GetAt(AwayClubIndex).MatchNumberToFriendlyNumber(MatchNumber);
	EXT_ASSERT(FriendlyNumber != NOFRIENDLY);
	GetAt(AwayClubIndex).GetFriendlies().CreateFriendlyMatch(FriendlyNumber, GetAt(HomeClubIndex).DoFindID(), AWAY);
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
