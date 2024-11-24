
 /*

	File:- ActiveCountry.cpp

    ActiveCountry list class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "Competition.h"


const uint				MAXIMUMCOUNTRYNEWSITEMS = 50;


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::~CActiveCountry
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CActiveCountry::~CActiveCountry()
{
	if (IsRanking0() == true)
	{
		TurnRanking0Off();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::CActiveCountry
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Parameter: 	const byte _ID
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CActiveCountry::CActiveCountry(CBinaryReader& _Resource, const byte _ID)
{
	m_ID = _ID;
	m_NationalStadiumId = _Resource.ReadByte();
	m_OneYearSeason = false;
	if (_Resource.ReadByte() != 0)
	{
		m_OneYearSeason = true;
	}
	m_NumberClubsFedCup1 = _Resource.ReadByte();
	m_NumberClubsFedCup2 = _Resource.ReadByte();
	m_LoanRate = 6;
	TurnRanking0On();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::AddCup
	Access:    	public 
	Parameter: 	const ushort _CupID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::AddCup(const ushort _CupID)
{
	m_CupList.Add(_CupID);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::AddDivision
	Access:    	public 
	Parameter: 	const ushort _DivisionID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::AddDivision(const ushort _DivisionID)
{
	m_DivisionIDList.Add(_DivisionID);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DeleteAll
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DeleteAll()
{
	if (IsRanking0() == true)
	{
		TurnRanking0Off ();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoDailyClearDown
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:	Initialise data before commencing to process fixtures
------------------------------------------------------------------------------*/
void CActiveCountry::DoDailyClearDown()
{
#ifdef	ACTIVEREFEREES
	m_DailyRefereeList = m_Referees;
	m_DailyRefereeList.ShuffleArray();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoEndOfWeek
	Access:    	public 
	Parameter: 	bool _NoWarnings
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoEndOfWeek(bool _NoWarnings)
{
	for (int LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
	{
		GetDivision(LoopCount).DoEndOfWeekTasks(_NoWarnings);
	}
	m_NewsList.DoShrinkList(MAXIMUMCOUNTRYNEWSITEMS);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoEndOfSeason
	Access:    	public 
	Parameter: 	CCountry& _pCountry
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CActiveCountry::DoEndOfSeason(CCountry& _Country)
{
	if (TotalDivisions() == 0 || GetSeasonEnd() != WorldData().GetCurrentDate().GetMatchDay())
	{
		return false;
	}

	for (int ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
	{
		GetCup(ListPos)->DoEndOfSeason();
	}

	// all divisions end of season
	for (int nListPos = 0; nListPos < m_DivisionIDList.GetSize(); nListPos++)
	{
		GetDivision(nListPos).DoEndOfSeason();
	}

	if (IsRanking0() == true)
	{
		m_YouthSquad->DoEndOfSeason(null);
		m_StaffList->DoEndOfSeason(false);
	}

	ushort RelegatedClubID = NOCLUB;
	ushort PromotedClubID = NOCLUB;

	CNumericArray<ushort> PlayOffClubs;
	// Top Division League champions and runners up
	CPlayOff* pPlayOff = GetDivision(0).GetTopPlayOff();
	if (pPlayOff != null)
	{
		if (pPlayOff->GetSpecialActionFlag() != MLSPLAYOFFSPECIALEVENT)
		{
			// Play offs for league title
			pPlayOff->GetWinners(PlayOffClubs, false);
			EXT_ASSERT(PlayOffClubs.GetSize() > 1);			// Ensure at least champions and runners up
			for (int LoopCount = 0; LoopCount < PlayOffClubs.GetSize(); LoopCount++)
			{
				WorldData().GetClubList().DoSwapClubDivisionPositions(PlayOffClubs[LoopCount], GetDivision(0).DoFindClubNumberInTablePosition(LoopCount));
			}
		}
	}
	else
	{
		PromotedClubID = GetDivision(0).DoFindClubNumberInTablePosition(0);
		GetDivision(0).DoAddChampions(PromotedClubID);
		WorldData().GetClub(PromotedClubID).WonTitle(true);

		RelegatedClubID = GetDivision(0).DoFindClubNumberInTablePosition(1);
		GetDivision(0).DoAddRunnersUp(RelegatedClubID);
		WorldData().GetClub(RelegatedClubID).RunnersUp(GetDivision(0));
	}

	// Charity Shield
	CCharityShield* pCharityShield = WorldData().GetCharityShieldList().DoFind(GetDivision(0).GetCharityShieldID());
	if (pCharityShield != null)
	{
		pCharityShield->DoEndOfSeason();
		// The champions will already be in the charity shield if they won the domestic cup
		EXT_ASSERT(RelegatedClubID != NOCLUB && PromotedClubID != NOCLUB);
		if (pCharityShield->ClubQualified(PromotedClubID) == false)
		{
			pCharityShield->DoAddClub(PromotedClubID);
		}
		else
		{
			// Add the runner up if the champions won the cup as well
			pCharityShield->DoAddClub(RelegatedClubID);
		}
	}
	// Finished Bottom of bottom division
	if (m_DivisionIDList.GetSize() > 1)
	{
		WorldData().GetClub(GetDivision(m_DivisionIDList.GetUpperBound()).DoFindClubNumberBottomOfTable()).DoFinishedBottomOfDivision();
	}

	// Promotion and relegation for all divisions
	for (int DivisionToPromoteFrom = 1; DivisionToPromoteFrom < m_DivisionIDList.GetSize(); DivisionToPromoteFrom++)
	{
		for (int TablePosition = 0; TablePosition < GetDivision(DivisionToPromoteFrom - 1).GetNumberClubsAutomaticallyRelegated(); TablePosition++)
		{
			RelegatedClubID = GetDivision(DivisionToPromoteFrom - 1).DoFindClubNumberInTablePosition(GetDivision(DivisionToPromoteFrom - 1).GetNumberOfClubsInDivision() - TablePosition - 1);
			WorldData().GetClub(RelegatedClubID).BeenRelegated();
		
			PromotedClubID = GetDivision(DivisionToPromoteFrom).DoPromoteClubInTablePosition(TablePosition);
			WorldData().GetClubList().DoSwapClubIDs(RelegatedClubID, PromotedClubID);
		}

		// Playoffs if a top of division playoff
		pPlayOff = GetDivision(DivisionToPromoteFrom).GetTopPlayOff();
		if (pPlayOff != null)
		{
			if (pPlayOff->GetSpecialActionFlag() != MLSPLAYOFFSPECIALEVENT)
			{
				DEBUG_OUTPUT(("Playoffs %s"), (GetDivision(DivisionToPromoteFrom).GetName()));
				pPlayOff->GetWinners(PlayOffClubs, true);
				for (uint nClubPos = 0; nClubPos < pPlayOff->GetNumberPromoted(); nClubPos++)
				{
					DoPromoteViaPlayOffs(PlayOffClubs[nClubPos], GetDivision(DivisionToPromoteFrom - 1), (nClubPos + 1));
				}
				PlayOffClubs.RemoveAll();
			}
		}
	}

	// Build country - club index according to new division and position within
	ushort FirstOffset = GetDivision(0).GetFirstClubNumberInDivision();
	for (uint nList1Pos = 0; nList1Pos < m_DivisionIDList.GetSize(); nList1Pos++)
	{
		GetDivision(nList1Pos).DoBuildFinishedOrderList(m_ClubsStartingOrder, FirstOffset);
		FirstOffset  += GetDivision(nList1Pos).GetNumberOfClubsInDivision();
	}
	WorldData().GetPlayerList().DoRegenerateRetiredPlayers(_Country);

	// Build the federation cup qualifiers lists
	int NumToAdd = 4;		// Champions League
	if (IsClubIDInCountry(WorldData().GetFederationCup(0).GetWinningClubID()) == true)
	{
		WorldData().GetFederationCupList().DoAddQualifier(0, WorldData().GetFederationCup(0).GetWinningClubID());
		NumToAdd--;
	}
	int ClubIDListPosition = 0;
	while (NumToAdd > 0)
	{
		if (WorldData().GetFederationCupList().DoAddQualifier(0, m_ClubsStartingOrder[ClubIDListPosition]) == true)
		{
			DEBUG_OUTPUT(("Added to Champions League %s"), (WorldData().GetClub(m_ClubsStartingOrder[ClubIDListPosition]).GetName()));
			NumToAdd--;
		}
		ClubIDListPosition++;
	}

	NumToAdd = 4;
	DEBUG_OUTPUT(("UEFA Cup Winners %s"), (WorldData().GetClub(WorldData().GetFederationCup(1).GetWinningClubID()).GetName()));
	if (IsClubIDInCountry(WorldData().GetFederationCup(1).GetWinningClubID()) == true)
	{
		WorldData().GetFederationCupList().DoAddQualifier(1, WorldData().GetFederationCup(1).GetWinningClubID());
		NumToAdd--;
	}
	while (NumToAdd > 0)
	{
		if (WorldData().GetFederationCupList().DoAddQualifier(1, m_ClubsStartingOrder[ClubIDListPosition]) == true && 
			WorldData().GetFederationCup(0).GetWinningClubID() != m_ClubsStartingOrder[ClubIDListPosition])
		{
			DEBUG_OUTPUT(("Added to UEAFA Cup %s"), (WorldData().GetClub(m_ClubsStartingOrder[ClubIDListPosition]).GetName()));
			NumToAdd--;
		}
		ClubIDListPosition++;
	}

#ifdef	ACTIVEREFEREES
	for (ListPos = 0; ListPos < m_Referees.GetSize(); ListPos ++)
	{
		RefereePtr(ListPos)->DoEndOfSeason(this);
	}
#endif
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::NewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoNewSeason()
{
	if (GetSeasonStart() == WorldData().GetCurrentDate().GetMatchDay())
	{
		DoBuildSeasonStr();
		for (uint LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
		{
			GetDivision(LoopCount).DoNewSeason();
		}

		for (uint LoopCount1 = 0; LoopCount1 < m_CupList.GetSize(); LoopCount1++)
		{
			GetCup(LoopCount1)->DoNewSeason();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetCup1WinnersInfo
	Access:    	public 
	Returns:   	CCupWinnersInfo
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupWinnersInfo& CActiveCountry::GetCup1WinnersInfo()
{
	return m_winnersCup[1];
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetClubsStartingOrder
	Access:    	public 
	Returns:   	CNumericArray<ushort>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<ushort>&	CActiveCountry::GetClubsStartingOrder()
{
	return m_ClubsStartingOrder;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetCup
	Access:    	public 
	Parameter: 	ushort _CupNumber
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CActiveCountry::GetCup(ushort _CupNumber)
{
	return WorldData().GetCup(m_CupList[_CupNumber]);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CActiveCountry::GetID()
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetNewsList
	Access:    	public 
	Returns:   	CNewsList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNewsList& CActiveCountry::GetNewsList()
{
	return m_NewsList;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetCountry
	Access:    	public 
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CActiveCountry::GetCountry()
{
	return *WorldData().GetCountryList().GetFromID(m_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetFederation
	Access:    	public 
	Returns:   	CFederation&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CActiveCountry::GetFederation()
{
	return GetCountry().GetFederation();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetAllTransferListed
	Access:    	public 
	Parameter: 	CClub* _pClub - excluding the players at the club
	Parameter: 	CPlayerSearch& _PlayerSearch
	Returns:   	void
	Qualifier: 	
	Purpose:	Builds a list of all the transfer listed players
------------------------------------------------------------------------------*/
void CActiveCountry::GetAllTransferListed(CClub* _Club, CPlayerSearch& _PlayerSearch)
{
	for (int ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
	{
		GetDivision(ListPos).GetAllTransferListed(_Club, _PlayerSearch);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetLoanRate
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CActiveCountry::GetLoanRate()
{
	return m_LoanRate;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetSeason
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CActiveCountry::GetSeason()
{
	return m_Season;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetSeasonStart
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CActiveCountry::GetSeasonStart()
{
	CCalendar TheDate(WorldData().GetCurrentDate());
	GetDivision(0).DoFindStartOfSeason(TheDate);
	return TheDate.GetMatchDay();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetSeasonEnd
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CActiveCountry::GetSeasonEnd()
{
	CCalendar TheDate(WorldData().GetCurrentDate());
	GetDivision(0).DoFindEndOfSeason(TheDate);
	return TheDate.GetMatchDay();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetSeasonStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CActiveCountry::GetSeasonStr()
{
	return WorldData().GetCurrentDate().GetSeasonStr(m_OneYearSeason);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::IsClubIDInCountry
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CActiveCountry::IsClubIDInCountry(const ushort _ClubID)
{
	bool bRet = true;
	if (_ClubID < FirstClubID() || _ClubID > LastClubID())
	{
		bRet = false;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::IsOneYearSeason
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CActiveCountry::IsOneYearSeason()
{
	return m_OneYearSeason;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TotalAvailableEmployees
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::TotalAvailableEmployees()
{
	return m_StaffList->GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TotalAvailableYouths
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::TotalAvailableYouths()
{
	return m_YouthSquad->GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TotalDivisions
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::TotalDivisions()
{
	return m_DivisionIDList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TotalPlayOffs
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::TotalPlayOffs()
{
	return m_PlayOffList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::HowManyPromotedToDivisonFromPlayoffs
	Access:    	public 
	Parameter: 	CDivision* _pDiv
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::HowManyPromotedToDivisonFromPlayoffs(CDivision* _pDiv)
{
	EXT_ASSERT_POINTER(_pDiv, CDivision);
	ushort nDivisionId = DoFindCountryDivNum(_pDiv);
	if (nDivisionId < m_DivisionIDList.GetUpperBound())
	{
		return GetDivision(nDivisionId + 1).GetNumberPromotedViaPlayOffs();
	}
	else
	{
		// Bottom division;
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetCharityShield
	Access:    	public 
	Parameter: 	const byte _CountryId
	Returns:   	CCharityShield*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCharityShield* CActiveCountry::GetCharityShield(const byte _CountryId)
{
	return WorldData().GetCharityShieldList().CountryCharityShield(_CountryId);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoAddPlayOff
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoAddPlayOff(const ushort _ID)
{
	if (m_PlayOffList.Find(_ID) == wNOTFOUND)
	{
		m_PlayOffList.Add(_ID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::AddToAvailableEmployees
	Access:    	public 
	Parameter: 	CEmployee* _pEmployee
	Returns:   	void
	Qualifier: 	
	Purpose:	Add the employee to the unemployed list
------------------------------------------------------------------------------*/
void CActiveCountry::AddToAvailableEmployees(CEmployee* _Employee)
{
	EXT_ASSERT_POINTER(_Employee, CEmployee);
	m_StaffList->Add(_Employee);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoFindRandomDivision
	Access:    	public 
	Returns:   	CDivision&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CActiveCountry::DoFindRandomDivision()
{
	return GetDivision(m_DivisionIDList.RandomValue());
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::SackYouth
	Access:    	public 
	Parameter: 	ushort _YouthID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::SackYouth(ushort _YouthID)
{
	EXT_ASSERT(IsRanking0() == true);
	m_YouthSquad->RemoveAt(_YouthID);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::SackYouth
	Access:    	public 
	Parameter: 	CYouth* _pYouth
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::SackYouth(CYouth* _pYouth)
{
	SackYouth(m_YouthSquad->Find(_pYouth));
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::SackEmployee
	Access:    	public 
	Parameter: 	CEmployee* _pEmployee
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::SackEmployee(CEmployee* _pEmployee)
{
	m_StaffList->Remove(_pEmployee);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::StaffAndYouthEndOfWeek
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoStaffAndYouthEndOfWeek(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
	m_YouthSquad->DoWeeklyTasks(_Country, _Popularity, _ClubDivStrength);
	m_StaffList->DoWeeklyTasks(_Country, true);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::IsTopDivision
	Access:    	public 
	Parameter: 	CDivision* _pDivision
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CActiveCountry::IsTopDivision(CDivision* _pDivision)
{
	if (_pDivision == &GetDivision(0))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetHighestDivisionNumber
	Access:    	public 
	Returns:   	ushort - the division list Id of the top division
	Qualifier: 	
	Purpose:	
------------------------------------------------------------------------------*/
ushort CActiveCountry::GetHighestDivisionNumber()
{
	return m_DivisionIDList[0];
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetLowestDivisionNumber
	Access:    	public 
	Returns:   	ushort - the division list Id of the lowest division
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::GetLowestDivisionNumber()
{
	return m_DivisionIDList.GetAtUpperBound();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoFindRandomClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:	Select a club at random from within the country
------------------------------------------------------------------------------*/
CClub* CActiveCountry::DoFindRandomClub()
{
	CClub* pRandomClub = DoFindRandomDivision().DoFindRandomNonUserClub(null);
//	DEBUG_OUTPUT(("%s, %d, %d"), (pRandomClub->GetName(), pRandomClub->GetFirstTeamSquad().GetSquad(0), pRandomClub->GetFirstTeamSquad().TotalSquadSize()));
	EXT_ASSERT(pRandomClub->GetFirstTeamSquad().GetSquad(0) != NOPLAYER);    // No players in squad
	return pRandomClub;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoFindRandomPlayer
	Access:    	public 
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CActiveCountry::DoFindRandomPlayer()
{
	CClub* pRndClub = DoFindRandomClub();
	int Num1;
	do
	{
		Num1 = RandomNumber.IntLessThan(PLAYERSINSQUAD - 1);
	}
	while (pRndClub->GetFirstTeamSquad().GetSquad(Num1) == NOPLAYER);
	EXT_ASSERT_POINTER(WorldData().GetPlayer(pRndClub->GetFirstTeamSquad().GetSquad(Num1)), CPlayer);
	return WorldData().GetPlayer(pRndClub->GetFirstTeamSquad().GetSquad(Num1));
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoFindSuitableTransferDivision
	Access:    	public 
	Parameter: 	const ushort _DivisionID
	Returns:   	CDivision*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision*	CActiveCountry::DoFindSuitableTransferDivision(const ushort _DivisionID)
{
	ushort WhichDivision = wNOTFOUND;
	for (uint ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
	{
		if (_DivisionID == m_DivisionIDList[ListPos])
		{
			WhichDivision = ListPos;
			break;
		}
	}
	EXT_ASSERT(WhichDivision != NOTFOUND);
	CDivision* pDiv = &GetDivision(WhichDivision);
	switch (RandomNumber.IntLessThan(5))
	{
		case 0:
			if (WhichDivision < m_DivisionIDList.GetUpperBound())
			{
				// Division below
				pDiv = &GetDivision(WhichDivision + 1);
			}
			break;
		case 1:
		case 2:
			if (WhichDivision > 0)
			{
				// Division above
				pDiv = &GetDivision(WhichDivision - 1);
			}
			break;
		case 3:
		case 4:
		case 5:
			// Same division
			break;
	}
	return pDiv;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::RemoveStaffYouths
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::RemoveStaffYouths()
{
	m_YouthSquad->RemoveList();
	m_StaffList->RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TurnRanking0On
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::TurnRanking0On()
{
	m_YouthSquad = new CYouthList();
	m_StaffList = new CEmployeeList();
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::TurnRanking0Off
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::TurnRanking0Off()
{
	delete m_YouthSquad;
	delete m_StaffList;
	m_YouthSquad = null;
	m_StaffList = null;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::IsRanking0
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CActiveCountry::IsRanking0()
{
	if (m_YouthSquad == null)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoBuildSeasonStr
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoBuildSeasonStr()
{
	m_Season = WorldData().GetCurrentDate().GetSeasonStr(m_OneYearSeason);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoCreateClubsStartingOrder
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Called at the start of the game, Saves club IDs in last season's 
				finishing order after promotion and relegation.
------------------------------------------------------------------------------*/
void CActiveCountry::DoCreateClubsStartingOrder()
{
	if (TotalDivisions() > 0)
	{
		m_ClubsStartingOrder.RemoveAll();
		for (uint ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
		{
			GetDivision(ListPos).CreateClubsStartingOrder(m_ClubsStartingOrder);
		}
		// Add last season's real life Champions League qualifiers
// 		DEBUG_OUTPUT(("%s\t%s\t%s\t%s"), (WorldData().GetClub(11).GetName(), WorldData().GetClub(5).GetName(), WorldData().GetClub(9).GetName(), WorldData().GetClub(0).GetName()));
		GetFederation().GetFederationCup(0).DoAddCupQualifiers(11, 5, 9, 0);
		// Add last season's real life UEFA Cup qualifiers
// 		DEBUG_OUTPUT(("%s\t%s\t%s\t%s"), (WorldData().GetClub(17).GetName(), WorldData().GetClub(7).GetName(), WorldData().GetClub(4).GetName(), WorldData().GetClub(15).GetName()));
		GetFederation().GetFederationCup(1).DoAddCupQualifiers(17, 7, 4, 15);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetDivision
	Access:    	public 
	Parameter: 	const ushort _CurrDivNumber
	Returns:   	CDivision&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CActiveCountry::GetDivision(const ushort _CurrDivNumber)
{
	return WorldData().GetDivisionList().GetAt(m_DivisionIDList[_CurrDivNumber]);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::SetRanking
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::SetRanking()
{
	m_YouthSquad = new CYouthList();
	m_StaffList = new CEmployeeList();
	for (uint ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
	{
		GetDivision(ListPos).MakeCountryRanking0Clubs();
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoCountTotalClubs
Access:    	public 
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoCountTotalClubs()
{
	ushort iRet = 0;
	for (uint ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
	{
		iRet += GetDivision(ListPos).GetNumberOfClubsInDivision();
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoFindCountryDivNum
	Access:    	public 
	Parameter: 	CDivision* _pDivision
	Returns:   	ushort
	Qualifier: 	
	Purpose:	Find the division number within the country
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoFindCountryDivNum(CDivision* _pDivision)
{
	EXT_ASSERT_POINTER(_pDivision, CDivision);
	ushort iRet = NOCLUB;
	for (uint n = 0; n < m_DivisionIDList.GetSize(); n++)
	{
		if (_pDivision == &GetDivision(n))
		{
			iRet = n;
			break;
		}
	}
	EXT_ASSERT(iRet != NOCLUB);		// Not found a match
	return iRet;
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoFindPlayerClub
Access:    	public 
Parameter: 	const ushort _PlayerID
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoFindPlayerClub(const ushort _PlayerID)
{
	EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize());
	ushort iRet = NOCLUB;
	CDivision* pDiv;
	for (uint ListPos = 0; ListPos < m_DivisionIDList.GetSize(); ListPos ++)
	{
		pDiv = &GetDivision(ListPos);
		for (int LoopCount = 0; LoopCount < pDiv->GetNumberOfClubsInDivision(); LoopCount++)
		{
			if (pDiv->GetClub(LoopCount).GetFirstTeamSquad().IsPlayerInSquad(_PlayerID) == true)
			{
				iRet = LoopCount;
				break;
			};
		}
	}
	EXT_ASSERT(iRet != NOCLUB);
	return iRet;
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::GetAvailableYouth
Access:    	public 
Parameter: 	ushort _YouthNumber
Returns:   	CYouth*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CYouth* CActiveCountry::GetAvailableYouth(ushort _YouthNumber)
{
	EXT_ASSERT(IsRanking0() == true);
	return m_YouthSquad->GetAt(_YouthNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetAvailableYouthList
	Access:    	public 
	Returns:   	CYouthList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CYouthList& CActiveCountry::GetAvailableYouthList()
{
	EXT_ASSERT(IsRanking0() == true);
	return *m_YouthSquad;
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::GetAvailableEmployee
Access:    	public 
Parameter: 	ushort _EmployeeNumber
Returns:   	CEmployee*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CEmployee* CActiveCountry::GetAvailableEmployee(ushort _EmployeeNumber)
{
	EXT_ASSERT(IsRanking0() == true);
	return static_cast<CEmployee*>(m_StaffList->GetAt(_EmployeeNumber));
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetAvailableEmployeeList
	Access:    	public 
	Returns:   	CEmployeeList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeList& CActiveCountry::GetAvailableEmployeeList()
{
	EXT_ASSERT(IsRanking0() == true);
	return *m_StaffList;
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoFindYouthID
Access:    	public 
Parameter: 	CYouth* _pYouth
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoFindYouthID(CYouth* _pYouth)
{
	EXT_ASSERT(IsRanking0() == true);
	return m_YouthSquad->Find(_pYouth);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoFindEmployeeID
Access:    	public 
Parameter: 	const CEmployee* _pEmployee
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoFindEmployeeID(CEmployee* _pEmployee)
{
	return m_StaffList->Find(_pEmployee);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::GetRandomAvailableEmployee
Access:    	public 
Parameter: 	const byte _Type
Parameter: 	const byte _MinimumSkill
Parameter: 	const byte _MaximumSkill
Returns:   	CEmployee* - null if none suitable are found
Qualifier: 	
Purpose:	Find an employee of the specified type with skill in the 
required range
------------------------------------------------------------------------------*/
CEmployee* CActiveCountry::GetRandomAvailableEmployee(const byte _Type, 
													  const byte _MinimumSkill, 
													  const byte _MaximumSkill)
{
	EXT_ASSERT(IsRanking0() == true);
	return m_StaffList->RandomElement(_Type, _MinimumSkill, _MaximumSkill);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::GetCupWinnersInfo
Access:    	public 
Parameter: 	void
Returns:   	CCupWinnersInfo&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CCupWinnersInfo& CActiveCountry::GetCupWinnersInfo()
{
	return m_winnersCup[0];
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetDivisionID
	Access:    	public 
	Parameter: 	ushort _DivisionNumber
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::GetDivisionID(ushort _DivisionNumber)
{
	return m_DivisionIDList[_DivisionNumber];
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::DoPromoteViaPlayOffs
	Access:    	public 
	Parameter: 	const ushort _PromotedClubID
	Parameter: 	CDivision& _rRelegatedDivision
	Parameter: 	const ushort _RelegatedClubPosition
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoPromoteViaPlayOffs(const ushort _PromotedClubID, CDivision& _RelegatedDivision, const ushort _RelegatedClubPosition)
{
	ushort RelegatedClubID = _RelegatedDivision.DoFindClubNumberInTablePosition(_RelegatedDivision.GetNumberOfClubsInDivision() - _RelegatedDivision.GetNumberClubsAutomaticallyRelegated() - _RelegatedClubPosition);
	// Check to ensure playoff winners are in the lower division
	if (RelegatedClubID != _PromotedClubID)
	{
#ifdef _DEBUGv3
		afxDump << "Relegation Position " << _rRelegatedDivision.GetNumberOfClubsInDivision() - _rRelegatedDivision.GetNumberClubsAutomaticallyRelegated() - _RelegatedClubPosition << "\n";
		afxDump << "Promoting Playoffs " << _PromotedClubID << " " << WorldData().GetClub(_PromotedClubID).GetName() << "\n";
		afxDump << "Relegating Playoffs " << RelegatedClubID << " " << WorldData().GetClub(RelegatedClubID).GetName() << "\n\n";
#endif
		WorldData().GetClub(RelegatedClubID).BeenRelegated();
		WorldData().GetClub(_PromotedClubID).BeenPromotedViaPlayOff();
		WorldData().GetClubList().DoSwapClubIDs(RelegatedClubID, _PromotedClubID);
	}
	else
	{
		// Avoided relegation by winning playoff
		WorldData().GetClub(RelegatedClubID).AvoidedRelegation();
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoSwapClubIDs
Access:    	public 
Parameter: 	const ushort _Club1ID
Parameter: 	const ushort _Club2ID
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoSwapClubIDs(const ushort _Club1ID, const ushort _Club2ID)
{
	m_winnersCup[0].DoSwapClubIDs(_Club1ID, _Club2ID);
	m_winnersCup[1].DoSwapClubIDs(_Club1ID, _Club2ID);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoFindDivisionTopPlayOffWinnerClubID
Access:    	public 
Parameter: 	const ushort _DivNum
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::DoFindDivisionTopPlayOffWinnerClubID(const ushort _DivNum)
{
	CNumericArray<ushort> PlayOffClubs;
	GetDivision(_DivNum).GetTopPlayOff()->GetWinners(PlayOffClubs, false);
	return PlayOffClubs[0];
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::AddToNewsLists
Access:    	public 
Parameter: 	CExtString _Message
Parameter: 	const eNewsType _Type
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::AddToNewsLists(CExtString _Message, const eNewsType _Type)
{
//	m_NewsList.Add(new CNewsItem(_Message, _Type, WorldData().GetCurrentDate()));
	WorldData().GetUserList().AddToNews(*this, _Message);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::AddToNewsLists
Access:    	public 
Parameter: 	int _MessageID
Parameter: 	const eNewsType _Type
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::AddToNewsLists(int _MessageID, const eNewsType _Type)
{
	CExtString str;
	str.LoadString(_MessageID);
	AddToNewsLists(str, _Type);
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoMidWeekTasks
Access:    	public 
Parameter: 	const byte _MatchNumber
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoMidWeekTasks(const byte _MatchNumber)
{
	for (uint LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
	{
		GetDivision(LoopCount).DoMidWeekTasks();
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoCreateTransferList
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoCreateTransferList()
{
	for (uint LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
	{
		GetDivision(LoopCount).CreateTransferList();
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::BuildCupList
Access:    	public 
Parameter: 	CCompetitionList* _pCupList
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::BuildCupList(CCompetitionList* _pCupList)
{
	EXT_ASSERT_POINTER(_pCupList, CCompetitionList);
	for (uint ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
	{
		_pCupList->DoAddCup(GetCup(ListPos));
	}
	CCharityShield* pCharityShield = WorldData().GetCharityShieldList().DoFind(GetDivision(0).GetCharityShieldID ());
	if (pCharityShield != null)
	{
		_pCupList->DoAddCharityShield(pCharityShield);
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::BuildThisWeekCupList
Access:    	public 
Parameter: 	CMatchTime* _pCurrentDate
Parameter: 	Vector<CCup*>* _pCupList
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::BuildThisWeekCupList(CCalendar* _pCurrentDate, Vector<CCup*>* _pCupList)
{
	for (uint ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
	{
		if (GetCup(ListPos)->RoundPlayedThisWeek(_pCurrentDate) != null || GetCup(ListPos)->ReplayPlayedThisWeek(_pCurrentDate) != null)
		{
			_pCupList->Add(GetCup(ListPos));
		}
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::DoPlayMissedMatches
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::DoPlayMissedMatches()
{
	if (m_OneYearSeason == true)
	{
		ushort LoopCount;
		for (LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
		{
			GetDivision(LoopCount).DoPlayMissedMatches();
		}
		for (LoopCount = 0; LoopCount < m_CupList.GetSize(); LoopCount++)
		{
			GetCup(LoopCount)->DoPlayMissedMatches();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::BuildPopularityClubPtrList
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& _TheList
	Parameter: 	ushort _ThePopularity
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::BuildPopularityClubPtrList(CNumericArray<ushort>& _TheList, ushort _ThePopularity)
{
	for (uint LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
	{
		GetDivision(LoopCount).DoBuildPopularityClubPtrList(_TheList, _ThePopularity);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::BuildPlayerSkillClubPtrList
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& _TheList
	Parameter: 	const uint8 _PlayerSkill
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CActiveCountry::BuildPlayerSkillClubPtrList(CNumericArray<ushort>& _TheList, const byte _PlayerSkill)
{
	for (uint LoopCount = 0; LoopCount < m_DivisionIDList.GetSize(); LoopCount++)
	{
		GetDivision(LoopCount).DoBuildPlayerSkillClubPtrList(_TheList, _PlayerSkill);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CActiveCountry::GetPopularityRandomClub
	Access:    	public 
	Parameter: 	const ushort _ThePopularity
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::GetPopularityRandomClub(const ushort _ThePopularity)
{
	EXT_ASSERT(_ThePopularity < WorldData().GetPopularityList().GetSize());
	CNumericArray<ushort> TheList;
	BuildPopularityClubPtrList(TheList, _ThePopularity);
	if (TheList.GetSize() > 0)
	{
		return TheList[TheList.RandomPosition()];
	}
	else
	{
		return NOCLUB;
	}
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::GetPlayerSkillRandomClub
Access:    	public 
Parameter: 	const byte _PlayerSkill
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::GetPlayerSkillRandomClub(const byte _PlayerSkill)
{
	EXT_ASSERT(_PlayerSkill >= MINPLAYSKILL && _PlayerSkill  < MAXPLAYSKILL);
	CNumericArray<ushort> TheList;
	BuildPopularityClubPtrList(TheList, _PlayerSkill);
	if (TheList.GetSize() > 0)
	{
		return TheList[TheList.RandomPosition()];
	}
	else
	{
		return NOCLUB;
	}
}


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
Method:   	CActiveCountry::FirstClubID
Access:    	public 
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::FirstClubID()
{
	return GetDivision(0).GetFirstClubNumberInDivision();
}


/*------------------------------------------------------------------------------
Method:   	CActiveCountry::LastClubID
Access:    	public 
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ushort CActiveCountry::LastClubID()
{
	return GetDivision(0).GetFirstClubNumberInDivision() + DoCountTotalClubs() - 1;
}
#endif