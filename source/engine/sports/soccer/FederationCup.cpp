
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
void CFederationCup::DoAddCupQualifiers(const ClubID _Club1ID, const ClubID _Club2ID, const ClubID _Club3ID, const ClubID _Club4ID)
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
void CFederationCup::DoAddQualifyingClubID(const ClubID _ClubID)
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
byte CFederationCup::DoCountCountryEntrants(const Vector<ClubID> &cCountryClubs)
{
	byte bRet = 0;
	CCupRound& theRound = CCup::GetRound(0);
	for (int LoopCount = 0; LoopCount < cCountryClubs.GetSize(); LoopCount++)
	{
		if (theRound.IsClubInRound(cCountryClubs[LoopCount]) == true)
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
void CFederationCup::DoListCountryEntrants(const Vector<ClubID> &cCountryClubs, CNumericArray<ClubID>* _TheList)
{
	CCupRound& theRound = CCup::GetRound(0);
	for (int LoopCount = 0; LoopCount < cCountryClubs.GetSize(); LoopCount++)
	{
		if (theRound.IsClubInRound(cCountryClubs[LoopCount]) == true)
		{
			_TheList->Add(cCountryClubs[LoopCount]);
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
 
	CNumericArray<ClubID> DrawClubs, Losers;
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
	EXT_ASSERT(DrawClubs.GetSize() == TheRoundPtr->GetNumberOfClubsInRound());

 	//do {
 		DrawClubs.DoShuffleArray();
// 	} while (m_AllowSameCountryGroups == false && );
     TheRoundPtr->DoDisplayDraw(DrawClubs, *this, true);
     TheRoundPtr->DoCreateMatchListAndRelatedRounds(*this, DrawClubs);
	 EXT_ASSERT(TheRoundPtr->GetMatchList().GetSize() == TheRoundPtr->GetNumberOfClubsInRound() / 2);
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
    _pDataFile->Read(&m_ID, sizeof(m_ID));
    _pDataFile->Read(&m_FederationID, sizeof(m_FederationID));
    CCup::Read(_pDataFile);
	m_BerthClubIDs.Read(_pDataFile);
	_pDataFile->Read(&m_AllowSameCountryGroups, sizeof(m_AllowSameCountryGroups));
	_pDataFile->Read(&m_NumberFromGroupsInNextRound, sizeof(m_NumberFromGroupsInNextRound));
	_pDataFile->Read(&m_NumberFromGroupsInNextCup, sizeof(m_NumberFromGroupsInNextCup));
	m_NeutralStadiumIDs.Read(_pDataFile);
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
    _pDataFile->Write(&m_ID, sizeof(m_ID));
    _pDataFile->Write(&m_FederationID, sizeof(m_FederationID));
    CCup::Write(_pDataFile);
	m_BerthClubIDs.Write(_pDataFile);
	_pDataFile->Write(&m_AllowSameCountryGroups, sizeof(m_AllowSameCountryGroups));
	_pDataFile->Write(&m_NumberFromGroupsInNextRound, sizeof(m_NumberFromGroupsInNextRound));
	_pDataFile->Write(&m_NumberFromGroupsInNextCup, sizeof(m_NumberFromGroupsInNextCup));
	m_NeutralStadiumIDs.Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationCup::DoCheckIfQualified
	Access:    	public
	Parameter: 	ushort _ClubID
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CFederationCup::DoCheckIfQualified(const ClubID _ClubID)
{
	EXT_ASSERT(_ClubID.id != NOCLUB);
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
    ClubID _ClubID = CBaseCompetition::GetWinningClubID();
    if (_ClubID.id != NOCLUB)
	{
		WorldData().GetClubByID(_ClubID).WonCup(this);
	}

    _ClubID = CBaseCompetition::GetRunnersUpClubID();
    if (_ClubID.id != NOCLUB)
	{
		WorldData().GetClubByID(_ClubID).RunnersUp(this);
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
	m_pList = FD_ENGINE_NEW CFederationCup[CExtArray::GetSize()];
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
bool CFederationCupList::DoAddQualifier(const ushort _CupID, const ClubID _ClubID)
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
	if (CExtArray::GetSize() == 0 || WorldData().GetCurrentDate().GetMatchDay() != TOTALMATCHES - 7)
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
	ClubID _ClubID = pCup->GetWinningClubID();
	EXT_ASSERT(_ClubID.id != NOCLUB);
    if (WorldData().GetClubByID(_ClubID).IsUserControlled() == true)
	{
#ifdef SCENARIOS
		ScenarioData.WonEuropeCup1();
#endif
	}
#ifdef INTERCONTINENALCUPS
	// Qualified for Super Cup as well
    GetAt(SUPERCUP)->m_TheQualifiers.Add(_ClubID);
    if (WorldData().GetClubByID(_ClubID)->ClubIsUserControlled() == true)
	{
#ifdef SCENARIOS
		ScenarioData.QualifiedForEuropeCup4();
#endif
	}
	// Qualified for Intercontinental Cup as well
    GetAt(INTERCONTINENTALCUP)->m_TheQualifiers.Add(_ClubID);
	CountryList.AddChampionsQualifiers(_ClubID, &cPtr->m_TheQualifiers);
    if (WorldData().GetClubByID(_ClubID)->ClubIsUserControlled() == true)
	{
#ifdef SCENARIOS
		ScenarioData.QualifiedForEuropeCup5();
#endif
	}
    GetAt(SUPERCUP)->m_TheQualifiers.Add(_ClubID);
	CountryList.AddCupWinnersQualifiers(_ClubID, &cPtr->m_TheQualifiers);
    if (WorldData().GetClubByID(_ClubID)->ClubIsUserControlled() == true)
	{
#ifdef SCENARIOS
		ScenarioData.WonEuropeCup4();
#endif
	}
#endif

    pCup = GetAt(UEFACUP);
	EXT_ASSERT_POINTER(pCup, CFederationCup);
    _ClubID = pCup->GetWinningClubID();
	EXT_ASSERT(_ClubID.id != NOCLUB);
    if (WorldData().GetClubByID(_ClubID).IsUserControlled() == true)
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
bool CFederationCupList::DoCheckIfInAnyCup(const ClubID _ClubIDToFind)
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
bool CFederationCupList::IsClubMatchToday(const CCalendar& _CurrentDate, ClubID _ClubID, CMatchInfo& _Match)
{
	EXT_ASSERT(_ClubID.id != NOCLUB);
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
void CFederationCupList::DoBuildNameList(CStringArray &_theList)
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		_theList.Add(m_pList[LoopCount].GetName());
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
	Method:   	CFederationCupList::DoFindNeutralVenueNameForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFederationCupList::DoFindNeutralVenueNameForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr = _T("");
	for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
		TitleStr = m_pList->DoFindNeutralVenueNameForFixture(_Fixture);
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
