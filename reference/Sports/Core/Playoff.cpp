

 /*

    Program:- Playoff.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CPlayOff::CPlayOff
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOff::CPlayOff()
	: m_ID(0)
	, m_NumberPromoted(0)
	, CCup()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ID = _Resource.ReadByte();
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	byte PercentagePointsForPlay0ffs = _Resource.ReadByte();
#endif
    m_NumberPromoted = _Resource.ReadByte();
	CCup::DoInitialiseFromResource(_Resource, 3, 0, 2);
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	CCup::SetPercentagePointsForPlay0ffs(PercentagePointsForPlay0ffs);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::GetNumberPromoted
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayOff::GetNumberPromoted()	const
{
	return m_NumberPromoted;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::pCup
	Access:    	public 
	Parameter: 	void
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CPlayOff::GetCup()
{
	return static_cast<CCup*>(this);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::GetID
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayOff::GetID()	const
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::DoBuildFixtureList
	Access:    	public 
	Parameter: 	const eDivisionMatchType eMatchType
	Parameter: 	CSoccerResultList& ResultList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::DoBuildFixtureList(const eDivisionMatchType _eMatchType, CSoccerResultList& _ResultList)
{
	CCup::GetRounds()[GetRoundNumber(_eMatchType)].BuildMatchDisplayList(_ResultList);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::GetRoundNumber
	Access:    	public 
	Parameter: 	const eDivisionMatchType eMatchType
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayOff::GetRoundNumber(const eDivisionMatchType _eMatchType)
{
	ushort RoundNumber = wNOTFOUND;
	switch (_eMatchType)
	{
		case DIVPLAYOFFMATCH1:
			RoundNumber = 0;
			break;
		case DIVPLAYOFFMATCH2:
			RoundNumber = 1;
			break;
		case DIVPLAYOFFMATCH3:
			RoundNumber = 2;
			break;
		case DIVPLAYOFFMATCH4:
			RoundNumber = 3;
			break;
		case DIVPLAYOFFMATCH5:
			RoundNumber = 4;
			break;
		case DIVPLAYOFFMATCH6:
			RoundNumber = 5;
			break;
		case DIVPLAYOFFMATCH7:
			RoundNumber = 6;
			break;
		case DIVPLAYOFFMATCH8:
			RoundNumber = 7;
			break;
		case DIVPLAYOFFMATCH9:
			RoundNumber = 8;
			break;
		case DIVPLAYOFFMATCH10:
			RoundNumber = 9;
			break;
		case DIVPLAYOFFMATCH11:
			RoundNumber = 10;
			break;
		case DIVPLAYOFFMATCH12:
			RoundNumber = 11;
			break;
		case DIVPLAYOFFMATCH13:
			RoundNumber = 12;
			break;
		case DIVPLAYOFFMATCH14:
			RoundNumber = 13;
			break;
		case DIVPLAYOFFMATCH15:
			RoundNumber = 14;
			break;
		case DIVPLAYOFFMATCH16:
			RoundNumber = 15;
			break;
		case DIVPLAYOFFMATCH17:
			RoundNumber = 16;
			break;
		case DIVPLAYOFFMATCH18:
			RoundNumber = 17;
			break;
		case DIVPLAYOFFMATCH19:
			RoundNumber = 18;
			break;
		case DIVPLAYOFFMATCH20:
			RoundNumber = 19;
			break;
		default:
			// Not a playoff
			EXT_ASSERT(false);
	}
	EXT_ASSERT(RoundNumber < CCup::GetRounds().GetSize());
	return RoundNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::DoDraw
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::DoDraw()
{
	CCupRound* pCurrentCupRound = CCup::IsDrawThisWeek();
	if (pCurrentCupRound == null)
	{
		return;
	}
    CCupRound* pLastCupRound = CCup::GetPreviousRound(*pCurrentCupRound);
	CNumericArray<ushort> DrawClubs;
	CNumericArray<ushort> Losers;
	if (CCup::DoFindRoundNumber(*pCurrentCupRound) == 0)
	{
        // Is the first round so find the qualifying clubs
		WorldData().GetDivisionList().DoAddPlayOffQualifiers(m_ID, DrawClubs);
	}
	else
	{
		// All clubs have already been entered into the competition
#ifdef CUPGROUPS
		switch(pLastCupRound->GetRoundType())
		{
			case GROUPMATCH:
			case GROUP3ONCE2REGIONS:
			case GROUP415372648:
				CCup::GetGroupMatchWinners(DrawClubs, pCurrentCupRound->GetNumInRound());
				break;
			default :
#endif
				pCurrentCupRound->GetDrawClubs(DrawClubs, Losers, static_cast<CCup&>(*this), pLastCupRound);
#ifdef CUPGROUPS
				break;
		}
#endif
	}

	// All clubs have found so create the relevant rounds
	DoTheDraw(DrawClubs, pCurrentCupRound->GetRoundType());

	// Announce the fixtures if a human manager is involved
	pCurrentCupRound->DoDisplayDraw(DrawClubs, *this, false);
	// Create the Fixtures & 2nd legs etc
	pCurrentCupRound->DoCreateMatchListAndRelatedRounds(*this, DrawClubs);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::Read(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_ID, sizeof m_ID);
    CCup::Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::Write(CDataFile* _pDataFile)
{
    _pDataFile->Write(&m_ID, sizeof m_ID);
    CCup::Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::DeleteAll
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::DeleteAll()
{
    CCup::DeleteAll();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::GetWinners
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aClubs
	Parameter: 	bool AwardTrophy
	Returns:   	void
	Qualifier: 	
	Purpose:	Builds list of entrants in finishing order and returns winners
				Calls EndOfSeason
------------------------------------------------------------------------------*/
void CPlayOff::GetWinners(CNumericArray<ushort>& _ClubIDList, bool _AwardTrophy)
{
    CNumericArray<ushort> Losers;
    CCupRound* pLastCupRound = static_cast<CCupRound*> (CCup::GetFinalRound());
    EXT_ASSERT(pLastCupRound != null);

	switch(pLastCupRound->GetRoundType())
	{
		case REVERSEDNORMAL:
		case REVERSEDEXTRATIME:
		case LEG1:
		case LEG1AWAYDOUBLE:
			EXT_ASSERT(false);
			break;

		case NORMALMATCH2REGIONS:
		case EXTRATIME2REGIONS:
		case REVERSEDNORMAL2REGIONS:
		case REVERSEDEXTRATIME2REGIONS:
		case LEG12REGIONS:
		case LEG22REGIONS:
		case LEG2EXTRATIME2REGIONS:
		case LEG1AWAYDOUBLE2REGIONS:
		case LEG2AWAYDOUBLE2REGIONS:
		case LEG2EXTRATIMEAWAYDOUBLE2REGIONS:
		case NEUTRAL2REGIONS:
		case ENDOFCUP:
		case LEG1AWAYDOUBLE1423:
		case LEG1AWAYDOUBLE1324:
		case MAXROUNDTYPES:
			EXT_ASSERT(false);
			break;

#ifdef CUPGROUPS
		case GROUP3ONCE2REGIONS:
			EXT_ASSERT(false);
			break;
		case GROUPMATCH:
			{
				EXT_ASSERT(false);
// 				int NumGroups, NumWinners, NumRunnersUp;
// 				CLeagueTableList PointsTable;
// 				ushort GroupNumber, LoopCount;
// 				CCupRound* pSecondCupRound;
// 				CNumericArray<ushort> ClubNumbers;
// 				CCupRound& FirstCupRound = CCup::GetRounds().DoFindFirstGroupMatch();
// 				pSecondCupRound = CCup::GetRounds().GetNextRound(&FirstCupRound);
// 				EXT_ASSERT(pSecondCupRound != null);
// 				ClubNumbers.SetSize(FirstCupRound.GetNumberInGroup());
// 				PointsTable.DoCreate(FirstCupRound.GetNumberInGroup());
// 				NumGroups = FirstCupRound.GetNumInRound() / FirstCupRound.GetNumberInGroup();
// 				EXT_ASSERT(NumGroups > 0);
// 				NumWinners = GetNumberPromoted() / NumGroups;
// 				NumRunnersUp = GetNumberPromoted() - (NumWinners * NumGroups);		// If not equal number of winners from each group}
// 
// 				for (GroupNumber = 0; GroupNumber < NumGroups; GroupNumber++)
// 				{
// 					PointsTable.DoNewSeason();
// 					FirstCupRound.GetGroupClubNumbers(GroupNumber, ClubNumbers, pSecondCupRound);
// 					PointsTable.SetClubNumbers(ClubNumbers);			// Put the club numbers into the tables
// 					CCup::GetRounds().BuildGroupTables(static_cast<CCup&>(*this), GroupNumber, &PointsTable, 0);
// 					PointsTable.Sort();								// Sort the teams into order
// 
// 					for (LoopCount = 0; LoopCount < NumWinners + (NumRunnersUp > 0); LoopCount++)
// 					{
// 						// Add to the promotion list
// 						EXT_ASSERT(PointsTable.GetAt(LoopCount)->GetClubNumber() != wNOTFOUND);
// 						_ClubIDList.Add(PointsTable.GetAt(LoopCount)->GetClubNumber());
// 						// Update club trophy history
// 						if (_AwardTrophy == true)
// 						{
// 							WorldData().GetClub(PointsTable.GetAt(LoopCount)->GetClubNumber()).WonPlayoff (this);
// 						}
// 					}
// 				}
// 				// If only 1 group and 1 promoted then award runners up trophy
// 				if (m_NumberPromoted == 1 && NumGroups == 1)
// 				{
// 					if (_AwardTrophy == true)
// 					{
// 						WorldData().GetClub(PointsTable.GetAt(1)->GetClubNumber()).RunnersUpPlayoff (this);
// 					}
// 					else
// 					{
// 						// Return runners up
// 						_ClubIDList.Add(PointsTable.GetAt(1)->GetClubNumber());
// 					}
// 				}
			}
			break;
#endif
		default :
		    pLastCupRound->DoAddLastRoundWinners(_ClubIDList, Losers, false, static_cast<CCup&>(*this));
			// Update club trophy history
			for (uint n = 0; n < GetNumberPromoted(); n++)
			{
				WorldData().GetClub(_ClubIDList[n]).WonPlayoff(this);
			}
			// Runners up awarded if only 1 promoted
			if (m_NumberPromoted == 1)
			{
				_ClubIDList.Add(Losers[0]);		// So return top 2 clubs
				WorldData().GetClub(Losers[0]).RunnersUpPlayoff (this);
			}
			break;
	}
	EXT_ASSERT(_ClubIDList.GetSize() >= GetNumberPromoted());
	DoEndOfSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOff::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOff::DoEndOfSeason()
{
	CCup::DoEndOfSeason(false);			// Don't award trophies
}
//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::CPlayOffList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOffList::CPlayOffList()
{
	CBinaryReader theResource(0, IDR_PLAYOFF, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	if (CExtArray::GetSize() > 0)
	{
		m_pList = new CPlayOff[CExtArray::GetSize()];
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			m_pList[ListPos].DoInitialiseFromResource(theResource);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::~CPlayOffList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOffList::~CPlayOffList()
{
	if (GetSize() > 0)
	{
		delete[] m_pList;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOffList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	if (GetSize() > 0)
	{
		CPlayOff* pPlayOff = m_pList;
		for (uint nCount = 0; nCount < GetSize(); nCount ++)
		{
			pPlayOff++->Read(_pDataFile);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOffList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	if (GetSize() > 0)
	{
		CPlayOff* pPlayOff = m_pList;
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			 pPlayOff++->Write(_pDataFile);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::IsClubMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime& CurrentDate
	Parameter: 	ushort Club
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayOffList::IsClubMatchToday(const CCalendar& _CurrentDate, const ushort _ClubID, CMatchInfo& _Match)
{
    bool MatchPlayed = false;
	if (GetSize() > 0)
	{
		CPlayOff* pPlayOff = m_pList;
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (pPlayOff++->IsClubMatchToday(_CurrentDate, _ClubID, _Match))
			{
				MatchPlayed = true;
				break;
			}
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::PlayMatches
	Access:    	public 
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayOffList::DoPlayMatches(const bool _ShowSummary)
{
    bool MatchPlayed = false;
	if (GetSize() > 0)
	{
		CPlayOff* pPlayOff = m_pList;
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (pPlayOff++->DoPlayMatches(_ShowSummary))
			{
				MatchPlayed = true;
			}
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::DoDraws
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayOffList::DoDraws()
{
	if (GetSize() > 0)
	{
		CPlayOff* pPlayOff = m_pList;
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			pPlayOff++->DoDraw();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::FindFromID
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	CPlayOff*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOff* CPlayOffList::FindFromId(const byte _Value)
{
	CPlayOff* pPlayOff = m_pList;
	if (GetSize() > 0)
	{
	    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (pPlayOff->GetID() == _Value)
			{
				break;
			}
			pPlayOff++;
		}
	}
	return pPlayOff;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayOffList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CPlayOff*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayOff* CPlayOffList::GetAt(const ushort _ElementNumber)
{
	return static_cast<CPlayOff*> (&m_pList[_ElementNumber]);
}
