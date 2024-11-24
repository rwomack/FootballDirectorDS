
/*
    File:- FederationCups.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#include "stdafx.h"

#include "MatchInfo.h"
#include "SortedStringList.h"


#ifdef ACTIVEFEDERATIONCUPS

/*------------------------------------------------------------------------------
	Method:   	CFederationCup::CFederationCup
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCup::CFederationCup()
{
}


/*------------------------------------------------------------------------------
Method:   	CFederationCup::~CFederationCup
Access:    	public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CFederationCup::~CFederationCup()
{
}


// int CFederationCup::CountBerthClubs()
// {
// 	return m_BerthClubIDs.gets
// }


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoAddCupQualifiers
	Access:    	public 
	Parameter: 	const ushort _Club1ID
	Parameter: 	const ushort _Club2ID
	Parameter: 	const ushort _Club3ID
	Parameter: 	const ushort _Club4ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoAddCupQualifiers(const ushort _Club1ID, const ushort _Club2ID, const ushort _Club3ID, const ushort _Club4ID)
{
	m_BerthClubIDs.Add(_Club1ID);
	m_BerthClubIDs.Add(_Club2ID);
	m_BerthClubIDs.Add(_Club3ID);
	m_BerthClubIDs.Add(_Club4ID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoAddQualifyingClubID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoAddQualifyingClubID(const ushort _ClubID)
{
	m_BerthClubIDs.Add(_ClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoCountCountryEntrants
	Access:    	public 
	Parameter: 	const ushort _FirstClubID
	Parameter: 	const ushort _LastClubID
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CFederationCup::DoCountCountryEntrants(const ushort _FirstClubID, const ushort _LastClubID)
{
	byte bRet = 0;
	CCupRound& theRound = CCup::GetRound(0);
	for (int LoopCount = _FirstClubID; LoopCount < _LastClubID; LoopCount++) 
	{
		if (theRound.IsClubInRound(LoopCount) == true)
		{
			bRet++;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoInitialiseSeasonNeutralVenue
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoInitialiseSeasonNeutralVenue()
{
	CCup::SetNeutralVenueStadiumID(m_NeutralStadiumIDs[WorldData().GetCurrentDate().GetSeason() % 8]);
	//DEBUG_OUTPUT(("%d, %s Final venue %s"), (m_NeutralStadiumIDs[WorldData().GetCurrentDate().GetSeason() % 8], GetName(), WorldData().GetStadium(CCup::GetNeutralVenueStadiumID())->GetName()));
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoListCountryEntrants
	Access:    	public 
	Parameter: 	const ushort _FirstClubID
	Parameter: 	const ushort _LastClubID
	Parameter: 	CNumericArray<ushort> * _TheList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoListCountryEntrants(const ushort _FirstClubID, const ushort _LastClubID, CNumericArray<ushort>* _TheList)
{
	CCupRound& theRound = CCup::GetRound(0);
	for (int LoopCount = _FirstClubID; LoopCount < _LastClubID; LoopCount++) 
	{
		if (theRound.IsClubInRound(LoopCount) == true)
		{
			_TheList->Add(LoopCount);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Parameter: 	const byte _ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoInitialiseFromResource(CBinaryReader& _Resource, const byte _ID, ushort* _NeutralStadiumIDList)
{
	m_ID = _ID;
    m_FederationID = 0;
 	WorldData().GetFederation(m_FederationID).DoAddCup(m_ID);
	m_NumberFromGroupsInNextRound = _Resource.ReadByte();
	m_NumberFromGroupsInNextCup = _Resource.ReadByte();
 	CCup::DoInitialiseFromResource(_Resource, NOCLUB, 2, 3);
	for (int LoopCount = 0; LoopCount < 8; LoopCount++)
	{
		m_NeutralStadiumIDs.Add(*_NeutralStadiumIDList);
		_NeutralStadiumIDList++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::GetFederation
	Access:    	public 
	Parameter: 	void
	Returns:   	CFederation&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CFederationCup::GetFederation(void)
{
	return WorldData().GetFederation(m_FederationID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoNewSeason(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoDraw
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoDraw()
{
    CCupRound* TheRoundPtr = CCup::IsDrawThisWeek(); 
    if (TheRoundPtr == NULL) 
	{
         return;         // No draw to create
    }

	// Draw requires processing this turn
	CCup::DoAnnounceDraw(*TheRoundPtr);
 
	CNumericArray<ushort> DrawClubs, Losers;
	int DrawPos = 0;
    // Add the winners from the last round
 	CCupRound* LastRoundPtr = CCup::GetPreviousRound(*TheRoundPtr);
    if (LastRoundPtr != NULL) 
  	{
        if (LastRoundPtr->GetRoundType() == GROUPMATCH) 
 		{
			CCup::GetGroupMatchWinners(DrawClubs, m_NumberFromGroupsInNextRound + m_NumberFromGroupsInNextCup);
			if (m_NumberFromGroupsInNextCup > 0)
			{
				Losers.DoAddAndRemoveFromArray2(DrawClubs, m_NumberFromGroupsInNextCup);
				WorldData().GetFederation(m_FederationID).GetFederationCup(m_ID + 1).m_BerthClubIDs.Append(Losers);
			}
			DrawClubs.DoAddAndRemoveFromArray2(m_BerthClubIDs, m_BerthClubIDs.GetSize());
        }
        else 
 		{
			LastRoundPtr->DoAddLastRoundWinners(DrawClubs, Losers, false, *this);
	    }
 	}
	else
	{
		// Add the new clubs
		for (int LoopCount = 0; LoopCount < m_BerthClubIDs.GetSize(); LoopCount++) 
		{
			DrawClubs.Add(m_BerthClubIDs.GetAt(LoopCount));
			DrawPos++;
		}
		m_BerthClubIDs.RemoveAll();
		for (int LoopCount = 0; LoopCount < CCup::TotalNonLeagueClubs(); LoopCount++) 
		{
			DrawClubs.Add(CCup::GetNonLeagueClub(LoopCount));
			DrawPos++;
		}
	}
 	//do {
 		DrawClubs.DoShuffleArray();
// 	} while (m_AllowSameCountryGroups == false && );
     TheRoundPtr->DoDisplayDraw(DrawClubs, (CCup&)*this, true);
     TheRoundPtr->DoCreateMatchListAndRelatedRounds((CCup&)*this, DrawClubs);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::Read(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_ID, sizeof m_ID);
    _pDataFile->Read(&m_FederationID, sizeof m_FederationID);
    CCup::Read(_pDataFile);
	m_BerthClubIDs.Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::Write(CDataFile* _pDataFile)
{
    _pDataFile->Write(&m_ID, sizeof m_ID);
    _pDataFile->Write(&m_FederationID, sizeof m_FederationID);
    CCup::Write(_pDataFile);
	m_BerthClubIDs.Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoReplaceClub
	Access:    	public 
	Parameter: 	ushort _ClubIDToReplace
	Parameter: 	ushort _ClubIDReplaceWith
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoReplaceClub(ushort _ClubIDToReplace, ushort _ClubIDReplaceWith)
{
	EXT_ASSERT(_ClubIDToReplace != wNOTFOUND && _ClubIDReplaceWith != wNOTFOUND);
	EXT_ASSERT(_ClubIDToReplace != _ClubIDReplaceWith);
	if (_ClubIDToReplace == CBaseCompetition::GetWinningClubID()) 
	{
		CBaseCompetition::DoAddRunnersUp(_ClubIDReplaceWith);
	}
//	m_BerthClubIDs.DoReplace(_ClubIDToReplace, _ClubIDReplaceWith);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoCheckIfQualified
	Access:    	public 
	Parameter: 	ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFederationCup::DoCheckIfQualified(const ushort _ClubID)
{
	EXT_ASSERT(_ClubID != NOCLUB);
	if (m_BerthClubIDs.Find(_ClubID) != wNOTFOUND) 
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCup::DoEndOfSeason(void)
{
	CCup::DoEndOfSeason();
	m_BerthClubIDs.RemoveAll();		// Last season's entries
    ushort ClubID = CBaseCompetition::GetWinningClubID();
    if (ClubID != NOCLUB) 
	{
		WorldData().GetClub(ClubID).WonCup(static_cast<CCup*>(this));
	}

    ClubID = CBaseCompetition::GetRunnersUpClubID();
    if (ClubID != NOCLUB) 
	{
		WorldData().GetClub(ClubID).RunnersUp(static_cast<CCup*>(this));
	}
}
///////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::CFederationCupList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCupList::CFederationCupList()
{
	m_pList = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::~CFederationCupList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCupList::~CFederationCupList()
{
	if (m_pList != NULL)
	{
		delete[] m_pList;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoInitialiseFromResource
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::DoInitialiseFromResource()
{
	CBinaryReader theResource(0, IDR_FEDERATIONCUP, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);

	ushort NeutralStadiumID [] = 
	{
		110,			// 0, Portugal, SL Benfica, 110, Estádio do Sport, 65000,		 ** Champions League season 1
		113,			// 1, Spain, FC Barcelona, 113, Camp Nou, 98260
		116,			// 2, Italy, AC Milan, 116, Giuseppe Meazza, 83679
		119,			// 3, Scotland, Celtic, 119, Celtic Park, 60832
		132,			// 4, France, Paris Saint-Germain, 132, Parc des Princes, 47428, ** UEFA Cup season 1
		156,			// 5, England, National, 156, Wembley, 90000
		128,			// 6, Holland, Ajax, 128, Amsterdam Arena, 51638
		107,			// 7, Germany, Bayern Munchen, 107, Allianz Arena, 69000
		117,			// 8, Italy, Juventus, 117, Olimpico Di Torino , 25370
		132,			// 9, France, Paris Saint-Germain, 132, Parc des Princes, 47428
		112,			// 10, Spain, Real Madrid, 112, Estadio Santiago Bernabéu, 80000
		102,			// 11, Holland, Feyenoord, 102, Stadion Feijenoord, 51137
	};
	m_pList = new CFederationCup[CExtArray::GetSize()];
	for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_pList[ListPos].DoInitialiseFromResource(theResource, ListPos, &NeutralStadiumID[ListPos * 4]);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::GetAt
	Access:    	public 
	Parameter: 	int x
	Returns:   	CFederationCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCup* CFederationCupList::GetAt(const ushort _Index)
{ 
	EXT_ASSERT(_Index < CExtArray::GetSize());
	return static_cast<CFederationCup*>(&m_pList[_Index]); 
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoAddQualifier
	Access:    	public 
	Parameter: 	const ushort _CupID
	Parameter: 	const ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFederationCupList::DoAddQualifier(const ushort _CupID, const ushort _ClubID)
{
	bool bRet = false;
	if (DoCheckIfInAnyCup(_ClubID) == false)
	{
		GetAt(_CupID)->DoAddQualifyingClubID(_ClubID);
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::DoNewSeason(void)
{
 	// If is time for first draw for Champions League then add all qualifiers for all the cups
	if (CExtArray::GetSize() == 0 || WorldData().GetCurrentDate().GetMatchDay() != 1) 
	{
		return;
	}

	// Initialise the neutral venue for this season's final
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
		GetAt(LoopCount)->DoInitialiseSeasonNeutralVenue();
	}

	// Not needed i first season, the qualifying clubs are in the start game data
// 	if (WorldData().GetCurrentDate().GetSeason() > 0)
// 	{
// 		for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
// 		{
// 			//EXT_ASSERT(GetAt(LoopCount)->CountBerthClubs() > 0);
// 			// Check if last year's winners are in a higher competition
// 			for (int PriorCup = 0; PriorCup < LoopCount; PriorCup++) 
// 			{
// 				if (GetAt(PriorCup)->DoCheckIfQualified(GetAt(LoopCount)->GetWinnersClubID()) == true) 
// 				{
// 					// Yes so remove from this competition
// 					GetAt(LoopCount)->SetWinnersClubID(NOCLUB);
// 					break;
// 				}
// 			}
// 			// Add all the qualifying clubs
// 			GetAt(LoopCount)->DoNewSeason();
// 		}
// 	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoEndOfSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Build next season's entry lists
------------------------------------------------------------------------------*/
void CFederationCupList::DoEndOfSeason()
{
	if (CExtArray::GetSize() == 0 || WorldData().GetCurrentDate().GetMatchDay() != TOTALMATCHES - 2) 
	{
		return;
	}

	// Add the winners to the history and reset the rounds
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
		GetAt(LoopCount)->DoEndOfSeason();
	}

	// Won the Champion's Cup
    CFederationCup* pCup = GetAt(CHAMPIONSCUP);
	EXT_ASSERT_POINTER(pCup, CFederationCup);
	ushort ClubID = pCup->GetWinningClubID();
	EXT_ASSERT(ClubID != NOCLUB);
    if (WorldData().GetClub(ClubID).IsUserControlled() == true) 
	{
#ifdef SCENARIOS
		ScenarioData.WonEuropeCup1();
#endif
	}
#ifdef INTERCONTINENALCUPS
	// Qualified for Super Cup as well
    GetAt(SUPERCUP)->m_TheQualifiers.Add(ClubID);
    if (WorldData().GetClub(ClubID)->ClubIsUserControlled() == true) 
	{
#ifdef SCENARIOS
		ScenarioData.QualifiedForEuropeCup4();
#endif
	}
	// Qualified for Intercontinental Cup as well
    GetAt(INTERCONTINENTALCUP)->m_TheQualifiers.Add(ClubID);
	CountryList.AddChampionsQualifiers(ClubID, &cPtr->m_TheQualifiers);
    if (WorldData().GetClub(ClubID)->ClubIsUserControlled() == true) 
	{
#ifdef SCENARIOS
		ScenarioData.QualifiedForEuropeCup5();
#endif
	}
    GetAt(SUPERCUP)->m_TheQualifiers.Add(ClubID);
	CountryList.AddCupWinnersQualifiers(ClubID, &cPtr->m_TheQualifiers);
    if (WorldData().GetClub(ClubID)->ClubIsUserControlled() == true) 
	{
#ifdef SCENARIOS
		ScenarioData.WonEuropeCup4();
#endif
	}
#endif

    pCup = GetAt(UEFACUP);
	EXT_ASSERT_POINTER(pCup, CFederationCup);
    ClubID = pCup->GetWinningClubID();
	EXT_ASSERT(ClubID != NOCLUB);
    if (WorldData().GetClub(ClubID).IsUserControlled() == true) 
	{
#ifdef SCENARIOS
		ScenarioData.WonEuropeCup3();
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoCheckIfInAnyCup
	Access:    	public 
	Parameter: 	const ushort _ClubIDToFind
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFederationCupList::DoCheckIfInAnyCup(const ushort _ClubIDToFind)
{
	if (CExtArray::GetSize() == 0) 
	{
		return false;
	}
	bool bRet = FALSE;
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
		if (GetAt(LoopCount)->DoCheckIfQualified(_ClubIDToFind) == true) 
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::IsClubMatchToday
	Access:    	public 
	Parameter: 	CMatchTime * _CurrentDate
	Parameter: 	ushort _ClubID
	Parameter: 	CMatchInfo& _Match
	Returns:   	bool
	Qualifier: 	
	Purpose:	Check to see if the club has a European cup match today
------------------------------------------------------------------------------*/
bool CFederationCupList::IsClubMatchToday(const CCalendar& _CurrentDate, ushort _ClubID, CMatchInfo& _Match)
{
	EXT_ASSERT(_ClubID != NOCLUB);
	bool bRet = false;
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
        bRet = GetAt(LoopCount)->IsClubMatchToday(_CurrentDate, _ClubID, _Match);
		if (bRet == true) 
		{
			if (GetAt(LoopCount)->RoundToday(_CurrentDate)->IsNeutralVenue() == true)
			{
				_Match.SetCompetitionType(FEDERATIONCUPMATCHNEUTRALVENUE);
			}
			else
			{
				_Match.SetCompetitionType(FEDERATIONCUPMATCH);
			}
			break;
		}
	}
	return bRet;
}


 /*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoPlayMatches
	Access:    	public 
	Parameter: 	bool _ViewEm
	Returns:   	bool
	Qualifier: 	
	Purpose:	Play any necessary cup matches
------------------------------------------------------------------------------*/
bool CFederationCupList::DoPlayMatches(bool _ViewEm)
{
	bool PlayedSome = false;
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
        if (GetAt(LoopCount)->DoPlayMatches(_ViewEm) == true) 
		{
			PlayedSome = true;
		}
	}
	return PlayedSome;
}



/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoBuildNameList
	Access:    	public 
	Parameter: 	CSortedStringList & _theList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::DoBuildNameList(CSortedStringList& _theList)
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		_theList.Add(m_pList[LoopCount].GetName(), LoopCount);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoReplaceClub
	Access:    	public 
	Parameter: 	ushort _ClubIDToReplace
	Parameter: 	ushort _ClubIDReplaceWith
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::DoReplaceClub(ushort _ClubIDToReplace, ushort _ClubIDReplaceWith)
{
	EXT_ASSERT(_ClubIDToReplace > -1 && _ClubIDReplaceWith > -1);
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
        GetAt(ListPos)->DoReplaceClub(_ClubIDToReplace, _ClubIDReplaceWith);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoFindCompetitionTitleForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFederationCupList::DoFindCompetitionTitleForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr;
	for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
		TitleStr = GetAt(ListPos)->DoFindCompetitionTitleForFixture(_Fixture);
		if (TitleStr.IsEmpty() == false)
		{
			break;
		}
	}
	return TitleStr;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
        GetAt(ListPos)->Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
        GetAt(ListPos)->Read(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::FindCup
	Access:    	public 
	Parameter: 	CCup * _TheCup
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CFederationCupList::FindCup(CCup* _TheCup)
{
	uint iRet = NOTFOUND;
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++) 
	{
		if (_TheCup == GetAt(LoopCount)) 
		{
			iRet = LoopCount;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCupList::DoDraws
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationCupList::DoDraws(void)
{
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
        GetAt(ListPos)->DoDraw();
    }
}

#endif