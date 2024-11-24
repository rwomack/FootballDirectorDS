
 /*

	Program:- CCup.cpp

    The cup class routines

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


#include "stdafx.h"

#include "Stadium.h"
#include "CharityShield.h"
#include "MatchInfo.h"
#include "CupDisplayEvent.h"
#include "UI.h"
#include "SortedStringList.h"
#include "FootballDirector.h"


    const int				AUTOREGIONSIZE = 24;
	const ushort			NOCOUNTRY = 0xffff;


/*------------------------------------------------------------------------------
	Method:   	CCup::CCup
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CCup::CCup()
	: m_CountryID(NOCOUNTRY)
	, m_CharityShieldID(0)
    , m_SpecialActionFlag(0)
	, m_RunnersUpManagerRatingPoints(0)
	, m_WinnersManagerRatingPoints(0)
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	, m_PercentagePointsForPlay0ffs(0)
#endif
#ifdef ACTIVEFEDERATIONCUPS
    , m_ExcludeFederationQualifiersUntilRound(0)
#endif
{
}


/*------------------------------------------------------------------------------
Method:   	CCup::~CCup
Access:    	public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CCup::~CCup()
{
	DeleteAll();
}


/*------------------------------------------------------------------------------
Method:   	CCup::DeleteAll
Access:    	public 
Parameter: 	void
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCup::DeleteAll()
{
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoFindCompetitionTitleForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCup::DoFindCompetitionTitleForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr;
	CCupRound* pRound = DoFindRoundFromFixture(_Fixture);
	if (pRound != NULL)
	{
		TitleStr =  CBaseCompetition::GetName() + _T(" ") + pRound->GetName();
	}
	return TitleStr;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoFindRoundDate
	Access:    	public 
	Parameter: 	const ushort CurrRound
	Parameter: 	bool ReplayFlag
	Returns:   	CCalendar
	Qualifier: 	
	Purpose:	Find the date of the round matches or replays
------------------------------------------------------------------------------*/
CCalendar CCup::DoFindRoundDate(CCupRound& _CupRound, bool _ReplayFlag)
{
	CCalendar TheDate(WorldData().GetCurrentDate());
	switch(_ReplayFlag)
	{
	case false:
		while(TheDate.GetMatchDay() < _CupRound.GetMatchDate())
		{
			TheDate++;
		};
		while(TheDate.GetMatchDay() > _CupRound.GetMatchDate())
		{
			TheDate--;
		};
		break;
	case true:
		while(TheDate.GetMatchDay() < _CupRound.GetReplayDate())
		{
			TheDate++;
		};
		while(TheDate.GetMatchDay() > _CupRound.GetReplayDate())
		{
			TheDate--;
		};
		break;
	}
	return TheDate;
}


/*------------------------------------------------------------------------------
Method:   	CCup::DoInitialiseBaseFromResource
Access:    	public 
Parameter: 	CBinaryResource& _Resource
Parameter: 	const byte RunnersUpManagerRatingPoints
Parameter: 	const byte WinnersManagerRatingPoints
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCup::DoInitialiseBaseFromResource(CBinaryReader& _Resource, const byte _RunnersUpManagerRatingPoints, const byte _WinnersManagerRatingPoints)
{
	CBaseCompetition::DoInitialiseFromResource(_Resource);
	m_RunnersUpManagerRatingPoints = _RunnersUpManagerRatingPoints;
	m_WinnersManagerRatingPoints = _WinnersManagerRatingPoints;
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	m_PercentagePointsForPlay0ffs = 0;
#endif
	m_NeutralVenueStadiumID = _Resource.ReadUInt16();
	m_SpecialActionFlag = _Resource.ReadByte();
	m_CharityShieldID = _Resource.ReadUInt16();
#ifdef ACTIVEFEDERATIONCUPS
	m_ExcludeFederationQualifiersUntilRound = _Resource.ReadByte();
#endif
	m_TheRounds.DoInitialiseFromResource(_Resource);
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Parameter: 	ushort Id
	Parameter: 	const byte RunnersUpManagerRatingPoints
	Parameter: 	const byte WinnersManagerRatingPoints
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoInitialiseFromResource(CBinaryReader& _Resource, ushort _ID, const byte _RunnersUpManagerRatingPoints, const byte _WinnersManagerRatingPoints)
{
	m_CountryID = _Resource.ReadUInt16();
	if (m_CountryID == NOCOUNTRY)
	{
		m_CountryID = 55;	// Temporary for federation cups until no need for dummy play3rs
	}
	short nHowManyClubsToAdd = WorldData().GetClubList().DoAddCupClubs(_Resource, *GetCountry());
	if (nHowManyClubsToAdd > 0)
	{
#ifdef DEBUG_LOAD_CUP_CLUBS
		DEBUG_OUTPUT(("%s - Adding Non league clubs %d"), (GetName(), nHowManyClubsToAdd));
#endif
		for (int LoopCount = (WorldData().GetTotalClubs() - nHowManyClubsToAdd); LoopCount < WorldData().GetTotalClubs(); LoopCount++)
		{
			m_NonLeagueClubs.Add(LoopCount);
#ifdef DEBUG_LOAD_CUP_CLUBS
			DEBUG_OUTPUT(("%d - %s"), (LoopCount, WorldData().GetClub(LoopCount).GetName()));
#endif
		}
	}

	// Last year's cup winners etc for history and federation cup qualifiers
#ifdef TROPHYHISTORY
	ushort Champions = _Resource.ReadUInt16();
    ushort RunnersUp = _Resource.ReadUInt16();
    ushort Semi1Loser = _Resource.ReadUInt16();
    ushort Semi2Loser = _Resource.ReadUInt16();
	DoAddCupHistory(Champions, RunnersUp, Semi1Loser, Semi2Loser, _ID);
#endif
	DoInitialiseBaseFromResource(_Resource, _RunnersUpManagerRatingPoints, _WinnersManagerRatingPoints);
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	m_PercentagePointsForPlay0ffs = 0;
#endif
/*    for (uint ListPos = 0; ListPos < GetRounds().GetSize(); ListPos++)
	{
		afxDump << "Round " << ListPos << ": Round date " << GetRounds()[ListPos].GetMatchDate() << " : round type " << GetRounds()[ListPos].GetRoundType() << "\n";
	}
	afxDump << "\n";*/
}


/*------------------------------------------------------------------------------
	Method:   	CCup::LeagueClubsListSize
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCup::LeagueClubsListSize()
{
	return m_LeagueClubsList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoFindRoundFromFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::DoFindRoundFromFixture(CFixture* _Fixture)
{
	return  m_TheRounds.DoFindCupRoundFromFixture(_Fixture);
}


/*------------------------------------------------------------------------------
Method:   	CCup::GetActiveCountry
Access:    	public 
Returns:   	CActiveCountry*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CActiveCountry& CCup::GetActiveCountry()
{
	return GetCountry()->GetActiveCountry();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetRound
	Access:    	public 
	Parameter: 	int _CurrentRound
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CCup::GetRound(int _CurrentRound)
{
	EXT_ASSERT(_CurrentRound < m_TheRounds.GetSize());
	return m_TheRounds[_CurrentRound];
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetRounds
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRoundList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRoundList& CCup::GetRounds()
{
	return m_TheRounds;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetLeagueClubsList
	Access:    	public 
	Parameter: 	void
	Returns:   	CNumericArray<ushort>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<ushort>& CCup::GetLeagueClubsList()
{
	return m_LeagueClubsList;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetSpecialActionFlag
	Access:    	public 
	Returns:   	eSpecialCupTypes
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eSpecialCupTypes CCup::GetSpecialActionFlag()
{
	return (eSpecialCupTypes)m_SpecialActionFlag;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetRunnersUpManagerRatingPoints
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetRunnersUpManagerRatingPoints()
{
	return m_RunnersUpManagerRatingPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetWinnersManagerRatingPoints
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetWinnersManagerRatingPoints()
{
	return m_WinnersManagerRatingPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoAddCupHistory
	Access:    	public 
	Parameter: 	ushort _WinningClubID
	Parameter: 	ushort _RunnersUpClubID
	Parameter: 	ushort _SemiLoserClub1ID
	Parameter: 	ushort _SemiLoserClub2ID
	Parameter: 	ushort Id
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoAddCupHistory(ushort _WinningClubID, ushort _RunnersUpClubID, ushort _SemiLoserClub1ID, ushort _SemiLoserClub2ID, ushort Id)
{
	// Federation Cup qualifiers
	switch(Id)
	{
		case 0:
			GetActiveCountry().GetCupWinnersInfo().SetWinnersAndRunnersUp(_WinningClubID, _RunnersUpClubID);
			GetActiveCountry().GetCupWinnersInfo().SetSemiLosers(_SemiLoserClub1ID, _SemiLoserClub2ID);
			break;
		case 1:
			GetActiveCountry().GetCup1WinnersInfo().SetWinnersAndRunnersUp(_WinningClubID, _RunnersUpClubID);
			GetActiveCountry().GetCup1WinnersInfo().SetSemiLosers(_SemiLoserClub1ID, _SemiLoserClub2ID);
			break;
	}
	CBaseCompetition::DoAddChampions(_WinningClubID);
	CBaseCompetition::DoAddRunnersUp(_RunnersUpClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNeutralVenueID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCup::GetNeutralVenueStadiumID()
{
	return m_NeutralVenueStadiumID;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CupAndRoundTitle
	Access:    	public 
	Parameter: 	CCupRound * _CupRound
	Returns:   	CExtString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString CCup::CupAndRoundTitle(CCupRound& _CupRound)
{
	CExtString str;
	str.Format(_T("%s %s"), CBaseCompetition::GetName(), _CupRound.GetTitle());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CupAndRoundName
	Access:    	public 
	Parameter: 	CCupRound * _CupRound
	Returns:   	CExtString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString CCup::CupAndRoundName(CCupRound& _CupRound)
{
	CExtString str;
	str.Format(_T("%s %s"), CBaseCompetition::GetName(), _CupRound.GetName());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CupAndReplayTitle
	Access:    	public 
	Parameter: 	CCupRound & _CupRound
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString CCup::CupAndReplayTitle(CCupRound& _CupRound)
{
	CExtString str;
	str.Format(_T("%s %s"), CBaseCompetition::GetName(), _CupRound.GetReplayTitle());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNonLeagueClub
	Access:    	public 
	Parameter: 	ushort Id
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCup::GetNonLeagueClub(ushort _ID)
{
	return m_NonLeagueClubs[_ID];
}


/*------------------------------------------------------------------------------
	Method:   	CCup::IsDrawThisWeek
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:	Is it necessary to draw the next round today?
------------------------------------------------------------------------------*/
CCupRound* CCup::IsDrawThisWeek()
{
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetDrawDate() == WorldData().GetCurrentDate().GetMatchDay())
		{
			// Ignore if a 2 leg round
			if (m_TheRounds[ListPos].IsSecondLeg() == true)
			{
				return null;
			}
			return &m_TheRounds[ListPos];
		}
	}
	return null;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::pLastRound
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::GetPreviousRound(CCupRound& _CupRound)
{
	int RoundNumber = GetRounds().DoFindRoundNumber(&_CupRound);
	if (RoundNumber > 0)
	{
		// Pointer to previous round
        return &m_TheRounds[RoundNumber - 1];
	}
	else
	{
		// No previous round
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoFindFinalRound
	Access:    	public 
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::DoFindFinalRound()
{
	return &m_TheRounds[m_TheRounds.GetUpperBound()];
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetFinalRound
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::GetFinalRound()
{
	return &m_TheRounds[m_TheRounds.GetUpperBound()];
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetCountryID
	Access:    	public const 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CCup::GetCountryID() const
{
	return m_CountryID;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::WinnerNameStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCup::WinnerNameStr()
{
	return WorldData().GetClub(CBaseCompetition::GetWinningClubID()).GetUpperCaseName();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::RunnerUpNameStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCup::RunnerUpNameStr()
{
	return WorldData().GetClub(CBaseCompetition::GetRunnersUpClubID()).GetUpperCaseName();
}



/*------------------------------------------------------------------------------
	Method:   	CCup::GetCupTiedPlayerList
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupTiedPlayerList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerIDList& CCup::GetCupTiedPlayerList()
{
	return m_CupTiedPlayerList;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoAnnounceFinal
	Access:    	public 
	Parameter: 	const ushort aRoundNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoAnnounceFinal(CCupRound& _CupRound)
{
	CExtString Message;
	Message.Format(IDS_CUPFINALCLUBS, CupAndRoundName(_CupRound),
									  WorldData().GetString(_CupRound.GetHomeTeam(0)),
									  WorldData().GetString(_CupRound.GetAwayTeam(0)),
									  _CupRound.LongMatchDateStr());
	if (m_CountryID != NOCOUNTRY)
	{
		// Is not a federation cup so add to the country news
		GetActiveCountry().AddToNewsLists(Message);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoAnnounceDraw
	Access:    	public 
	Parameter: 	CCupRound & _CupRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoAnnounceDraw(CCupRound& _CupRound)
{
	CExtString Message;
	Message.Format(IDS_CUPDRAWBEENANNOUNCED, CupAndRoundName(_CupRound));
	if (m_CountryID != NOCOUNTRY)
	{
		// Is not a federation cup so add to the country news
		GetActiveCountry().AddToNewsLists(Message);
	}
#ifdef NEWSPAPERON
	Message.FormatMessage(IDS_CUPDRAWANNOUNCEDNEWSPAPER, CupAndRoundName(_CupRound));
    m_NewsEvents.SetDrawMadeThisWeek(Message);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoDraws
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:	Draw requires processing this turn
------------------------------------------------------------------------------*/
void CCup::DoDraws()
{
//  	DEBUG_OUTPUT(("%s, Non league clubs %d"), 9GetName(), TotalNonLeagueClubs()));
	CCupRound* pCurrentCupRound = IsDrawThisWeek();
    if (pCurrentCupRound == null)
	{
		return;
	}

	CCupRound& theRound = *pCurrentCupRound;
	if (theRound.GetNumInRound() > 2)
	{
		DoAnnounceDraw(theRound);
	}

	CNumericArray<ushort> DrawClubs, Losers;
	int RoundNumber = m_TheRounds.DoFindRoundNumber(pCurrentCupRound);
	// If not the first round
	if (RoundNumber > 0)
	{
#ifdef CUP_GROUP3ONCE2REGIONS
		if (theRound.GetNumberOfRegions() == 2 && pLastRound(theRound)->ReplayListSize() > 0)
		{
			ushort n;
			int ListPos, DrawPos;
			Randomize();		// Set random seed
			pLastRound(theRound)->GetDrawClubs(DrawClubs, Losers, this, pLastRound(theRound));

			// 1st round byes are stored in replay fixtures
			// Add the bye teams
			ListPos = 8;
			for (n = 0; n < 4; n ++)
			{
				DrawClubs.SetAt(ListPos++, pLastRound(theRound)->GetHomeReplayTeam(n));
				DrawClubs.SetAt(ListPos++, pLastRound(theRound)->GetAwayReplayTeam(n));
			}
			for (n = 4; n < 8; n ++)
			{
				DrawClubs.Add(pLastRound(theRound)->GetHomeReplayTeam(n));
				DrawClubs.Add(pLastRound(theRound)->GetAwayReplayTeam(n));
			}
//			afxDump << "Cup teams for draw " << DrawClubs.GetSize() << " Should be " << theRound.GetNumInRound() << "\n";
			EXT_ASSERT(DrawClubs.CheckForDuplicates() == false);
			EXT_ASSERT(DrawClubs.GetSize() == theRound.GetNumInRound());

			// Shuffle the clubs
			DrawPos  = theRound.GetNumInRound() / theRound.GetNumberOfRegions();
			for (n = 0; n < theRound.GetNumInRound(); n += DrawPos)
			{
				DrawClubs.ShuffleArray(n, n + DrawPos - 1);
			}
		     // Display Draw Results.
			theRound.DisplayDraw(DrawClubs, this, false);
			theRound.CreateMatchList(this, DrawClubs);
			return;
		}
#endif

#ifdef ACTIVEFEDERATIONCUPS
 		if (m_ExcludeFederationQualifiersUntilRound == RoundNumber)
 		{
 			// Add clubs in federation cups to entrants of this cup round
			DEBUG_OUTPUT(("NumberOfFederationClubsInRound %d m_LeagueClubsList %d"), (m_FederationQualifiersList.GetSize(), m_LeagueClubsList.GetSize()));
			EXT_ASSERT(m_FederationQualifiersList.GetSize() > 0);	// Ensure there are some
 			m_LeagueClubsList.Append(m_FederationQualifiersList);
 			m_FederationQualifiersList.RemoveAll();
			DEBUG_OUTPUT(("NumberOfFederationClubsInRound %d m_LeagueClubsList %d"), (m_FederationQualifiersList.GetSize(), m_LeagueClubsList.GetSize()));
			g_DebugStr.OutputDebug();
 		}
#endif
#ifdef CUPGROUPS
		// If last round was end of group matches
		switch(GetPreviousRound(theRound)->GetRoundType())
		{
#ifdef CUP_GROUP3ONCE2REGIONS
		case GROUP3ONCE2REGIONS:
				// Last round was a 2 region group
                 GetWinners2RegionsGroup3Once(RoundNumber, DrawClubs);
                 DrawClubs.ShuffleArray(0, theRound.GetNumInRound() / 2 - 1);
                 DrawClubs.ShuffleArray(theRound.GetNumInRound() / 2, theRound.GetNumInRound() - 1);
                 theRound.DoCreateMatchListAndRelatedRounds(*this, DrawClubs);
				return;
#endif
			case GROUPMATCH:
				// Last round was a group
#ifdef DUTCHCUP
				if (m_SpecialActionFlag == DUTCHCUPSPECIALEVENT)
				{
					DrawClubs.Append(&m_LeagueClubsList);
					m_LeagueClubsList.RemoveAll();
					GetGroupMatchWinners(DrawClubs, 32 - DrawClubs.GetSize());
				}
				else
#endif
				{
					GetGroupMatchWinners(DrawClubs, theRound.GetNumInRound());
				}
				// May have special case draw for group winners and runners up etc
				DoTheDraw(DrawClubs, theRound.GetRoundType());
                theRound.DoDisplayDraw(DrawClubs, *this, false);
                theRound.DoCreateMatchListAndRelatedRounds(*this, DrawClubs);
				return;
		}
#endif
	}
	else
	{
		// First round, remove any clubs excluded
#ifdef ACTIVEFEDERATIONCUPS
		if (m_ExcludeFederationQualifiersUntilRound > 0)
		{
			GetFederation().DoListCountryEntrants(GetActiveCountry(), &m_FederationQualifiersList);
			m_LeagueClubsList.RemoveIfFoundInArray2(&m_FederationQualifiersList);
		}
#endif
       if (theRound.Is2Regions() == true)
		{
#ifdef AUTOWINDSCREENSHIELD
			if (GetPreviousRound(theRound) == null)
			{
				// AutoWindshield cup first round is in 2 regions
				CNumericArray<ushort> NorthernClubs;
				CNumericArray<ushort> SouthernClubs;
				CNumericArray<ushort> TempList;
				// Organise these in the 'Northermost' order, then the n/s split wilk be ok
				static const int RegionList[] =
				{
					1, 2,											//NORTHEAST,
					3, 4, 5, 28,									//YORKSHIRE
					11,												//MANCHESTER,
					8,												//MERSEYSIDE,
					7, 9, 10, 33, 									//NORTHWEST,
					6, 12, 14, 13, 15, 22, 27, 35, 37, 				//MIDLANDS,
					17, 18, 19, 21, 30, 32, 36,						//LONDON
					16, 20, 23, 24, 25, 31, 34, 					//SOUTH,
					29,												//EASTANGLIA,
					26,												//WALES
					NOTFOUND
				};
				int Region = 0;
				while(RegionList[Region] != NOTFOUND)
				{
					WorldData().GetCountry(m_CountryID).GetActiveCountry().GetDivision(2).GetClubsInRegion(RegionList[Region], TempList);
					WorldData().GetCountry(m_CountryID).GetActiveCountry().GetDivision(3).GetClubsInRegion(RegionList[Region++], TempList);

					// Add them to the correct region
					for (int LoopCount = 0; LoopCount < TempList.GetSize(); LoopCount++)
					{
						if (NorthernClubs.GetSize() < AUTOREGIONSIZE)
						{
							NorthernClubs.Add(TempList.GetAt(LoopCount));
//							DEBUG_OUTPUT(("Region %d, Northern %d %s %d"), (Region, NorthernClubs.GetSize(), WorldData().GetClub(NorthernClubs.GetAtUpperBound()).GetName(), WorldData().GetClub(NorthernClubs.GetAtUpperBound()).GetRegionID()));
						}
						else
						{
							SouthernClubs.Add(TempList.GetAt(LoopCount));
// 							DEBUG_OUTPUT(("Region %d, Southern %d %s"), (Region, SouthernClubs.GetSize(), WorldData().GetClub(SouthernClubs.GetAtUpperBound()).GetName()));
						}
					}
					TempList.RemoveAll();
				}
				EXT_ASSERT(SouthernClubs.GetSize() == AUTOREGIONSIZE);

				NorthernClubs.DoShuffleArray();
				for (int LoopCount = 0; LoopCount < AUTOREGIONSIZE; LoopCount ++)
				{
					TempList.Add(NorthernClubs.GetAt(LoopCount));
				}

				SouthernClubs.DoShuffleArray();
				for (int LoopCount = 0; LoopCount < AUTOREGIONSIZE; LoopCount ++)
				{
					TempList.Add(SouthernClubs.GetAt(LoopCount));
				}
				m_LeagueClubsList.RemoveAll();				// Not required
#ifdef ACTIVEFEDERATIONCUPS
				m_FederationQualifiersList.RemoveAll();	// Not required
#endif
				m_TheRounds[0].SetRoundSize(48);				// Include the bye clubs
				m_TheRounds[0].DoCreateMatchList(*this, TempList);
				m_TheRounds[0].SetRoundSize(32);				// Exclude the bye clubs
				// 8 team from each region have a bye into the next round
				// Store them in the replay match list
				for (int LoopCount = 0 ; LoopCount < 4; LoopCount++)
				{
					m_TheRounds[0].MoveMatchToReplay(8);
				}
				for (int LoopCount = 0; LoopCount < 4; LoopCount++)
				{
					m_TheRounds[0].MoveLastMatchToReplay();
				}
				EXT_ASSERT(m_TheRounds[0].MatchListSize() == 16);
				m_TheRounds[0].DoDisplayDraw(TempList, *this, false);
				return;
			}
#endif
		}

		switch(m_SpecialActionFlag)
		{
			case ENGLISHLEAGUECUPSPECIALEVENT:
				{
					// Default values assume no Federation cup entrants
					byte Round1Size = 56;
					byte Round2Size = (92 - Round1Size) + Round1Size / 2;

#ifdef ACTIVEFEDERATIONCUPS
					// If using Federation cups
					ushort NumberFedCupEntrants = m_FederationQualifiersList.GetSize();
					EXT_ASSERT(NumberFedCupEntrants > 4 && NumberFedCupEntrants < 13);
					if (NumberFedCupEntrants > 0)
					{
						// The clubs in Federation cups do not enter the league cup until round 3
						Round1Size = 66 + 2 * (NumberFedCupEntrants - 5);
						Round2Size = 92 - NumberFedCupEntrants - Round1Size + (Round1Size / 2);
					}
#endif
					// Set the actual round sizes
					m_TheRounds[0].SetRoundSize(Round1Size);
					m_TheRounds[1].SetRoundSize(Round2Size);
				}
				break;
#ifdef DUTCHCUP
			case DUTCHCUPSPECIALEVENT:
				// Round 1 is a group, 2 from each group of 4 qualify for round 4
				m_TheRounds[0].SetNumberNonLeagueClubsToAdd(52 - m_LeagueClubsList.GetSize());
				break;
#endif
#ifdef SPANISHCUP
			case SPANISHCOPADELRAYSPECIALEVENT:
				SetUpSpanishCopaDelRay();
				break;
#endif
		}
	}
	// Do the draw
    theRound.DoDraw(*this, GetPreviousRound(theRound), DrawClubs);
    DoCreateRelatedRounds(theRound, DrawClubs);          // 2nd legs and groups
	if (theRound.GetNumInRound() == 2)
	{
		DoAnnounceFinal(theRound);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoTheDraw
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aDrawClubIDList
	Parameter: 	const byte RoundType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoTheDraw(CNumericArray<ushort>& _DrawClubIDList, eCupRoundTypes _RoundType)
{
	EXT_ASSERT(_DrawClubIDList.GetSize() > 0);
	switch(_RoundType)
	{
		case EXTRATIME1432:
			_DrawClubIDList.DoSwap(1, 3);		// 1 v 4, 3 v 2
			break;

		case EXTRATIME1423:
		case LEG1AWAYDOUBLE1423:
			_DrawClubIDList.DoSwap(1, 3);		// 1 v 4, 3 v 2
			_DrawClubIDList.DoSwap(2, 3);		// 1 v 4, 2 v 3
			break;

		case LEG1AWAYDOUBLE1324:
			_DrawClubIDList.DoSwap(2, 3);		// 1 v 3, 2 v 4
			break;

#ifdef CUPGROUPS
		case GROUP415372648:
			_DrawClubIDList.DoSwap(1, 4);
			_DrawClubIDList.DoSwap(3, 6);
			break;
#endif
		default :
           _DrawClubIDList.DoShuffleArray();
		   break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CreateRelatedRounds
	Access:    	public 
	Parameter: 	CCupRound& _CupRound
	Parameter: 	CNumericArray<ushort>& _DrawClubIDList
	Returns:   	void
	Qualifier: 	
	Purpose:	Create 2nd leg or group matches for the specified round
------------------------------------------------------------------------------*/
void CCup::DoCreateRelatedRounds(CCupRound& _CupRound, CNumericArray<ushort>& _DrawClubIDList)
{
	int RoundNumber = GetRounds().DoFindRoundNumber(&_CupRound);
    // If a 2 leg round then reverse fixtures for 2nd leg
	switch(_CupRound.GetRoundType())
	{
		case LEG1:
		case LEG1AWAYDOUBLE:
		case LEG1AWAYDOUBLE1423:
		case LEG1AWAYDOUBLE1324:
		case LEG12REGIONS:
		case LEG1AWAYDOUBLE2REGIONS:
			EXT_ASSERT(RoundNumber <= m_TheRounds.GetSize() - 2);	// Must be a leg 2 round
			m_TheRounds[RoundNumber + 1].DoCopyDrawLeg2(_CupRound, GetSubsSelect(), GetSubsUse());
			break;
#ifdef CUPGROUPS
		case GROUPMATCH:
		case GROUP415372648:
			CreateGroupMatches(_CupRound, _DrawClubIDList);
			break;
#endif
	}
}


/*------------------------------------------------------------------------------
Method:   	CCup::DoFindRoundNumber
Access:    	public 
Parameter: 	CCupRound& _CupRound
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int CCup::DoFindRoundNumber(CCupRound& _CupRound)
{
	int iRound = NOTFOUND;
	for (int ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
		if (&m_TheRounds[ListPos] == &_CupRound)
		{
			iRound = ListPos;
			break;
		}
	}
	EXT_ASSERT(iRound != NOTFOUND);		// Invalid pointer
	return iRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::pCountry
	Access:    	public 
	Parameter: 	void
	Returns:   	CCountry*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry* CCup::GetCountry()
{
	if (m_CountryID != NOCOUNTRY)
	{
		return &WorldData().GetCountry(m_CountryID);
	}
	else
	{
		// Federation competition
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CountryRanking
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetCountryRanking()
{
	if (m_CountryID != NOCOUNTRY)
	{
		return GetCountry()->GetRanking();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::TotalNonLeagueClubs
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CCup::TotalNonLeagueClubs()
{
	return m_NonLeagueClubs.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::TotalRounds
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CCup::TotalRounds()
{
	int RoundSize = m_TheRounds.GetSize();
	if (m_TheRounds[RoundSize - 1].GetRoundType() == ENDOFCUP)
	{
		RoundSize --;
	}
	return RoundSize + m_TheRounds.TotalRoundsWithReplaysExisting();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::AfterCupMatchUpdate
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Parameter: 	CFixture& aFixture
	Parameter: 	CMatchInfo* MatchDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::AfterCupMatchUpdate(CCalendar* CurrentDate, CFixture& aFixture, CMatchInfo* MatchDetails)
{
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetReplayDate() == CurrentDate->GetMatchDay())
		{
			AfterMatchUpdate(m_TheRounds[ListPos].GetRoundType(), aFixture, MatchDetails);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::AfterMatchUpdate
	Access:    	public 
	Parameter: 	const eCupRoundTypes a_eRoundType
	Parameter: 	CFixture& aFixture
	Parameter: 	CMatchInfo* MatchDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::AfterMatchUpdate(const eCupRoundTypes _eRoundType, CFixture& _Fixture, CMatchInfo* _MatchDetails)
{
	ushort strVenueID = WorldData().GetText().DoFindStringID(_MatchDetails->GetStadiumName());
    CClub* theClub = &_Fixture.GetClub(HOME);
    if (theClub->IsNonLeagueClub() == false)
	{
		switch(_eRoundType)
		{
			case NEUTRAL:
			case NEUTRAL2REGIONS:
			case NEUTRALEXTRATIME:
				// Don't check records if a neutral venue match
				break;

			default :
                theClub->CheckHomeRecords(_Fixture, CBaseCompetition::m_TitleID, strVenueID);
				if (theClub->GetManagerID() != NOMANAGER)
				{
					if (WorldData().GetManager(theClub->GetManagerID()).PlayedCupMatch(_Fixture) == true)
					{
						theClub->DoSackManager();
					}
				}
		}
	}

    theClub = &_Fixture.GetClub(HOME);
    if (theClub->IsNonLeagueClub() == false)
	{
		switch(_eRoundType)
		{
			case NEUTRAL:
			case NEUTRAL2REGIONS:
			case NEUTRALEXTRATIME:
				// Don't check records if a neutral venue match
				break;

			default :
                theClub->CheckAwayRecords(_Fixture, CBaseCompetition::m_TitleID, strVenueID);
				if (theClub->GetManagerID() != NOMANAGER)
				{
					if (WorldData().GetManager(theClub->GetManagerID()).PlayedCupMatch(_Fixture) == true)
					{
						theClub->DoSackManager();
					}
				}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::PlayReplays
	Access:    	public 
	Parameter: 	const eMatchCompetitionType MatchType
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::DoPlayReplays(const eMatchCompetitionType _MatchType, const bool _ShowSummary)
{
	bool PlayedFlag = false;
	CMatchInfo MatchDetails;

    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetReplayDate() == WorldData().GetCurrentDate().GetMatchDay())
		{
			WorldData().GetMatchHandler().SetCompetitionInfo(null, &m_TheRounds[ListPos], this);
            for (int MatchNum = 0; MatchNum < m_TheRounds[ListPos].GetReplayListSize(); MatchNum ++)
			{
				m_TheRounds[ListPos].GetReplayList()[MatchNum].SetMatchType(_MatchType);
				m_TheRounds[ListPos].GetReplayList()[MatchNum].GetMatchVars().SetHomeAwayClubs(m_TheRounds[ListPos].GetReplayList()[MatchNum]);

				PlayedFlag = true;
				if (DisplayMatchIntro(m_TheRounds[ListPos].GetReplayList()[MatchNum], _ShowSummary) == true)
				{
                	// Add match to matches view list
					m_TheRounds[ListPos].GetReplayDetails(&m_TheRounds[ListPos].GetReplayList()[MatchNum], MatchDetails);
					WorldData().GetMatchPlay().DoMatch(&m_TheRounds[ListPos].GetReplayList()[MatchNum], this, &m_TheRounds[ListPos], MatchDetails.GetStadiumName());
				}
				else
				{
					// Calculate the match result
#ifdef	QUICKRESULTS
					if (GetCountryRanking() == 2)
					{
						do
						{
							m_TheRounds[ListPos].GetReplayList()[MatchNum].QuickResultGenerator();
						}
						while(DoCheckIfTied(m_TheRounds[ListPos], null, &m_TheRounds[ListPos].GetReplayList()[MatchNum]) == true);
					}
					else
#endif
					{
						m_TheRounds[ListPos].GetReplayList()[MatchNum].DoPlayMatch(true);
						m_TheRounds[ListPos].GetReplayList()[MatchNum].DoShowSummary(_ShowSummary, this);
					}
				}
				if (GetCountryRanking() < 2)
				{
					AfterMatchUpdate(m_TheRounds[ListPos].GetRoundType(), m_TheRounds[ListPos].GetReplayList()[MatchNum], &MatchDetails);
				}
			}
            break;
        }
	}
	return PlayedFlag;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::PlayedRound
	Access:    	public 
	Parameter: 	const ushort WeekNumber
	Parameter: 	int RoundNum
	Returns:   	bool
	Qualifier: 	
	Purpose:	Has the specified round been played? if round -1 is requested 
				then checks if any round has been played
------------------------------------------------------------------------------*/
bool CCup::HasPlayedRound(const ushort _WeekNumber, int _RoundNum)
{
	bool bRet = false;
	if (_RoundNum == -1)
	{
		// Check all rounds
		for (uint LoopCount = 0; LoopCount < m_TheRounds.GetSize(); LoopCount++)
		{
			if (m_TheRounds[LoopCount].GetMatchDate() <= _WeekNumber)
			{
	            bRet = true;
				break;
			}
		}
	}
	else
	{
		// Check specific round
		if (m_TheRounds[_RoundNum].GetMatchDate() <= _WeekNumber)
		{
            bRet = true;
		}
	}
	return bRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::TodaysTitleStr
	Access:    	public 
	Parameter: 	const ushort DayNumber
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCup::TodaysTitleStr(const ushort DayNumber)
{
    CExtString sRet;
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetMatchDate() == DayNumber && m_TheRounds[ListPos].GetRoundType() != ENDOFCUP)
		{
            sRet = CupAndRoundTitle(m_TheRounds[ListPos]);
            break;
		}
        if (m_TheRounds[ListPos].GetReplayDate() == DayNumber)
		{
            sRet = CupAndReplayTitle(m_TheRounds[ListPos]);
            break;
		}
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::BuildCupFixtureList
	Access:    	public 
	Parameter: 	const ushort MatchNumber
	Parameter: 	CSoccerResultList& FixtList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoBuildCupFixtureList(const ushort MatchNumber, CSoccerResultList& FixtList)
{
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetMatchDate() == MatchNumber && m_TheRounds[ListPos].GetRoundType() != ENDOFCUP)
		{
			m_TheRounds[ListPos].BuildMatchDisplayList(FixtList);
            break;
        }
        if (m_TheRounds[ListPos].GetReplayDate() == MatchNumber)
		{
			m_TheRounds[ListPos].BuildReplayDisplayList(FixtList);
            break;
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoBuildEventsStrList
	Access:    	public 
	Parameter: 	CCupDisplayEventList& EventList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoBuildEventsStrList(CCupDisplayEventList& EventList)
{
	EventList.DeleteAll();
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
		m_TheRounds[ListPos].DoBuildEventsStrList(EventList);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::IsClubMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime& CurrentDate
	Parameter: 	const ushort Club
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::IsClubMatchToday(const CCalendar& _CurrentDate, const ushort _ClubID, CMatchInfo& _Match)
{
	bool bRet = false;
    for (int ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
		bRet = m_TheRounds[ListPos].IsClubMatchToday(_CurrentDate, *this, _ClubID, _Match);
        if (bRet == true)
		{
			CString szTemp;
// 			szTemp.Format(_T("%s %s"), CBaseCompetition::GetName(), _Match.GetCompetitionTitle());
// 			_Match.SetCompetitionTitle(szTemp);	// Cup title + round title
			_Match.SetCompetitionTitle(CBaseCompetition::GetName());	// Cup title + round title
			_Match.SetCup(this);
			break;
		}
    }
    return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::RoundToday
	Access:    	public 
	Parameter: 	const CCalendar & _CurrentDate
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::RoundToday(const CCalendar& _CurrentDate)
{
	CCupRound* pRound = NULL;
	for (int ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
		if (m_TheRounds[ListPos].RoundToday(_CurrentDate) != NULL)
		{
			pRound = &m_TheRounds[ListPos];
			break;
		}
	}
	return pRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::RoundPlayedThisWeek
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Returns:   	CFixtureList*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList* CCup::RoundPlayedThisWeek(CCalendar* CurrentDate)
{
    CFixtureList* pFixture = null;
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetMatchDate() == CurrentDate->GetMatchDay() && m_TheRounds[ListPos].GetRoundType() != ENDOFCUP)
		{
            pFixture = &m_TheRounds[ListPos].GetMatchList();
			break;
		}
    }
	return pFixture;
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::ReplayPlayedThisWeek
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Returns:   	CFixtureList*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList* CCup::ReplayPlayedThisWeek(CCalendar* CurrentDate)
{
    CFixtureList* pFixture = null;
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetReplayDate() == CurrentDate->GetMatchDay())
		{
			// Ensure there were some replays
			if (m_TheRounds[ListPos].GetReplayListSize() > 0)
			{
				pFixture = &m_TheRounds[ListPos].GetReplayList();
				break;
			}
		}
    }
	return pFixture;
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::LastRoundPlayed
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Returns:   	int
	Qualifier: 	
	Purpose:	What was the last round played
------------------------------------------------------------------------------*/
int  CCup::LastRoundPlayed(CCalendar* CurrentDate)
{
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if ((m_TheRounds[ListPos].GetMatchDate() == CurrentDate->GetMatchDay() || 
            m_TheRounds[ListPos].GetReplayDate() == CurrentDate->GetMatchDay()) && 
			m_TheRounds[ListPos].GetRoundType() != ENDOFCUP)
		{
				return ListPos;
        }
    }
    return -1;
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::GetLastRoundPlayedPtr
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::GetLastRoundPlayedPtr(CCalendar* CurrentDate)
{
    int RoundNum = LastRoundPlayed(CurrentDate);
	if (RoundNum > -1)
	{
        return &m_TheRounds[RoundNum];
	}
	else
	{
		return null;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::GetRoundsPlayed
	Access:    	public 
	Parameter: 	CMatchTime* CurrentDate
	Returns:   	int
	Qualifier: 	
	Purpose:	How many rounds have been played?
------------------------------------------------------------------------------*/
int  CCup::GetRoundsPlayed(CCalendar* CurrentDate)
{
    int Count = 0;
    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetMatchDate() < CurrentDate->GetMatchDay())
		{
            Count ++;
        }
    }
	return Count;
 }

/*------------------------------------------------------------------------------
	Method:   	CCup::HaveAllRoundsBeenPlayed
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::HaveAllRoundsBeenPlayed()
{
    return m_TheRounds[m_TheRounds.GetUpperBound() - 1].HaveAllRoundMatchesBeenPlayed();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::PlayMatches
	Access:    	public 
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::DoPlayMatches(const bool _bShowSummary)
{
	bool PlayedFlag = false;
#ifdef NEWSPAPERON
	m_NewsEvents.DoResetVariables();           // Clearout last turn's newspaper stuff
#endif

    for (uint ListPos = 0; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (m_TheRounds[ListPos].GetMatchDate() == WorldData().GetCurrentDate().GetMatchDay() && m_TheRounds[ListPos].GetRoundType() != ENDOFCUP)
		{
			PlayedFlag = true;
// 			DEBUG_OUTPUT(("%s, Round date  %d: Draw date %d: Round type %d"), (CupAndRoundTitle(m_TheRounds[ListPos]), m_TheRounds[ListPos].GetMatchDate(), m_TheRounds[ListPos].GetDrawDate(), m_TheRounds[ListPos].GetRoundType()));
			DoPlayGames(m_TheRounds[ListPos], _bShowSummary);
            break;
        }
	}							// Next round
	return PlayedFlag;
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::PlayNormalGames
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Parameter: 	ushort ListPos
	Parameter: 	const bool bShowSummary
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoPlayNormalGames(CCupRound& _CupRound, const bool _bShowSummary)
{
	CFixtureList ViewMatches;
    CCupRound* pLastCupRound = null;
	CMatchInfo MatchDetails;

	for (int MatchNum = 0; MatchNum < _CupRound.GetMatchList().GetSize(); MatchNum ++)
	{
		_CupRound.GetMatchList()[MatchNum].GetMatchVars().SetHomeAwayClubs(_CupRound.GetMatchList()[MatchNum]);
		_CupRound.GetMatchDetails(*this, &_CupRound.GetMatchList()[MatchNum], MatchDetails);

		// Play match
		if (DisplayMatchIntro(_CupRound.GetMatchList()[MatchNum], _bShowSummary) == true)
		{
        	// Add match to matches view list
			ViewMatches.Add(&_CupRound.GetMatchList()[MatchNum]);
		}
		else
		{
			// Calculate the match result
#ifdef	QUICKRESULTS
			if (GetCountryRanking() == 2)
			{
				do
				{
					_CupRound.GetMatchList()[MatchNum]->QuickResultGenerator();
				}
				while(DoCheckIfTied(pCupRound, null, _CupRound.GetMatchList()[MatchNum]) == true);
			}
			else
#endif
			{
				_CupRound.GetMatchList()[MatchNum].DoPlayMatch();
				_CupRound.GetMatchList()[MatchNum].DoShowSummary(_bShowSummary, this);
				AfterMatchUpdate(_CupRound.GetRoundType(), _CupRound.GetMatchList()[MatchNum], &MatchDetails);
			}
		}
	}

	// View any requested user matches
	for (int ListPos = 0; ListPos < ViewMatches.GetSize(); ListPos++)
	{
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
		_CupRound.GetMatchDetails(*this, &ViewMatches[ListPos], MatchDetails);
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
		WorldData().GetMatchPlay().DoMatch(&ViewMatches[ListPos], this, &_CupRound, MatchDetails.GetStadiumName());
		AfterMatchUpdate(_CupRound.GetRoundType(), ViewMatches[ListPos], &MatchDetails);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::PlayExtraTimeNormalGames
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Parameter: 	const bool bShowSummary
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoPlayExtraTimeNormalGames(CCupRound& _CupRound, const bool _bShowSummary)
{
	CFixtureList ViewMatches;
	CMatchInfo MatchDetails;
	for (int MatchNum = 0; MatchNum < _CupRound.GetMatchList().GetSize(); MatchNum ++)
	{
        _CupRound.GetMatchList()[MatchNum].GetMatchVars().SetHomeAwayClubs(_CupRound.GetMatchList()[MatchNum]);
		_CupRound.GetMatchDetails(*this, &_CupRound.GetMatchList()[MatchNum], MatchDetails);

		// Play match
		if (DisplayMatchIntro(_CupRound.GetMatchList()[MatchNum], _bShowSummary) == true)
		{
        	// Add match to matches view list
			ViewMatches.Add(&_CupRound.GetMatchList()[MatchNum]);
		}
		else
		{
			// Calculate the match result
#ifdef	QUICKRESULTS
			if (GetCountryRanking() == 2)
			{
				do
				{
					_CupRound.GetMatchList()[MatchNum]->QuickResultGenerator();
				}
				while (_CupRound.GetMatchList()[MatchNum]->IsMatchDrawn() == true);
			}
			else
#endif
			{
				_CupRound.GetMatchList()[MatchNum].DoPlayMatch(true);
				_CupRound.GetMatchList()[MatchNum].DoShowSummary(_bShowSummary, this);
				AfterMatchUpdate(_CupRound.GetRoundType(), _CupRound.GetMatchList()[MatchNum], &MatchDetails);
			}
		}
	}

	// View any requested user matches
	for (int ListPos = 0; ListPos < ViewMatches.GetSize(); ListPos++)
	{
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
		_CupRound.GetMatchDetails(*this, &ViewMatches[ListPos], MatchDetails);
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
		WorldData().GetMatchPlay().DoMatch(&ViewMatches[ListPos], this, &_CupRound, MatchDetails.GetStadiumName());
		AfterMatchUpdate(_CupRound.GetRoundType(), ViewMatches[ListPos], &MatchDetails);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DisplayMatchIntro
	Access:    	public 
	Parameter: 	CFixture& aFixture
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::DisplayMatchIntro(CFixture& _Fixture, const bool _bShowSummary)
{
	if (_Fixture.IsUserMatch() == true)
	{
		WorldData().GetMatchHandler().GetMatchPlay().DoInitialiseIntro(&_Fixture, null, this, &m_TheRounds[GetNextRoundNumber(WorldData().GetCurrentDate())]);
		return theApp.DisplayMatchIntro(_bShowSummary);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::Play2LegGames
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Parameter: 	const bool bShowSummary
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoPlay2LegGames(CCupRound& _CupRound, const bool _bShowSummary)
{
    CCupRound* pLastCupRound = null;
	if (m_TheRounds.DoFindRoundNumber(&_CupRound) > 0)
	{
		pLastCupRound = &m_TheRounds[m_TheRounds.DoFindRoundNumber(&_CupRound) - 1];
	}

	// Play all matches in the round
	CFixtureList ViewMatches;
	CMatchInfo MatchDetails;
	CNumericArray<byte> MatchNumbers;
	CFixture* pLastFixture = null;
	for (int MatchNum = 0; MatchNum < _CupRound.GetMatchList().GetSize(); MatchNum ++)
	{
        _CupRound.GetMatchList()[MatchNum];             // First free structure
		_CupRound.GetMatchList()[MatchNum].GetMatchVars().SetHomeAwayClubs(_CupRound.GetMatchList()[MatchNum]);
		_CupRound.GetMatchDetails(*this, &_CupRound.GetMatchList()[MatchNum], MatchDetails);

		// Play match
		if (DisplayMatchIntro(_CupRound.GetMatchList()[MatchNum], _bShowSummary) == true)
		{
        	// Add match to matches view list
			ViewMatches.Add(&_CupRound.GetMatchList()[MatchNum]);
			MatchNumbers.Add(MatchNum);
		}
		else
		{
			// Calculate the match result
            pLastFixture = &pLastCupRound->GetMatchList()[MatchNum];             // First leg CFixture
#ifdef	QUICKRESULTS
			if (GetCountryRanking() == 2)
			{
				do
				{
					_CupRound.GetMatchList()[MatchNum]->QuickResultGenerator();
				}
				while(DoCheckIfTied(pCupRound, pLastFixture, _CupRound.GetMatchList()[MatchNum]) == true);
			}
			else
#endif
			{
				// Calculate the match result
				_CupRound.GetMatchList()[MatchNum].DoPlayMatch();
				// ********** Fudge alert, ensure ET and penalty shoot out happens, *****************************************
				if (DoCheckIfTied(_CupRound, pLastFixture, _CupRound.GetMatchList()[MatchNum]) == true)
				{
					_CupRound.GetMatchList()[MatchNum].DoExtraTime();
					if (DoCheckIfTied(_CupRound, pLastFixture, _CupRound.GetMatchList()[MatchNum]) == true)
					{
						_CupRound.GetMatchList()[MatchNum].DoPenaltyShootOut();
					}
				}
				_CupRound.GetMatchList()[MatchNum].DoShowSummary(_bShowSummary, this);
				AfterMatchUpdate(_CupRound.GetRoundType(), _CupRound.GetMatchList()[MatchNum], &MatchDetails);
			}
		}
	}

	// View any requested user matches
	for (int ListPos = 0; ListPos < ViewMatches.GetSize(); ListPos++)
	{
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
		_CupRound.GetMatchDetails(*this, &ViewMatches[ListPos], MatchDetails);
		ViewMatches[ListPos].GetMatchVars().SetHomeAwayClubs(ViewMatches[ListPos]);
        pLastFixture = &pLastCupRound->GetMatchList()[MatchNumbers[ListPos]];             // First leg CFixture
		WorldData().GetMatchPlay().DoMatch(&ViewMatches[ListPos], this, &_CupRound, pLastFixture, MatchDetails.GetStadiumName());
		AfterMatchUpdate(_CupRound.GetRoundType(), ViewMatches[ListPos], &MatchDetails);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoPlayGames
	Access:    	public 
	Parameter: 	CCupRound & _CupRound
	Parameter: 	const bool _bShowSummary
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoPlayGames(CCupRound& _CupRound, const bool _bShowSummary)
{
	EXT_ASSERT(_CupRound.GetMatchList().GetSize() > 0);
	WorldData().GetMatchHandler().SetCompetitionInfo(null, &_CupRound, this);

	CCupRound* pLastLegRound = null;
	switch(_CupRound.GetRoundType())
	{
		case NORMALMATCH:
		case REVERSEDNORMAL:
		case LEG1:
		case LEG1AWAYDOUBLE:
		case LEG1AWAYDOUBLE1423:
		case LEG1AWAYDOUBLE1324:
		case NEUTRAL:
		case NORMALMATCH2REGIONS:
		case REVERSEDNORMAL2REGIONS:
		case LEG12REGIONS:
		case LEG1AWAYDOUBLE2REGIONS:
		case NEUTRAL2REGIONS:
#ifdef CUPGROUPS
		case GROUPMATCH:
		case GROUP415372648:
#endif
#ifdef CUP_GROUP3ONCE2REGIONS
		case GROUP3ONCE2REGIONS:
#endif
			DoPlayNormalGames(_CupRound, _bShowSummary);
			break;

		case EXTRATIME1423:
		case EXTRATIME1432:
		case EXTRATIME2REGIONS:
		case REVERSEDEXTRATIME2REGIONS:
		case EXTRATIME:
		case REVERSEDEXTRATIME:
		case NEUTRALEXTRATIME:
			DoPlayExtraTimeNormalGames(_CupRound, _bShowSummary);
			break;

		case LEG2:
		case LEG2EXTRATIME:
		case LEG2AWAYDOUBLE:
		case LEG2EXTRATIMEAWAYDOUBLE:
		case LEG22REGIONS:
		case LEG2EXTRATIME2REGIONS:
		case LEG2AWAYDOUBLE2REGIONS:
		case LEG2EXTRATIMEAWAYDOUBLE2REGIONS:
			DoPlay2LegGames(_CupRound, _bShowSummary);
			pLastLegRound = &m_TheRounds[m_TheRounds.DoFindRoundNumber(&_CupRound) - 1];
			break;
	}
	_CupRound.DoBuildReplayList(pLastLegRound, this);
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNeutralVenueStadiumCapacity
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CCup::GetNeutralVenueStadiumCapacity()
{
	EXT_ASSERT(WorldData().GetStadium(m_NeutralVenueStadiumID)->GetCapacity() > 0);
	return WorldData().GetStadium(m_NeutralVenueStadiumID)->GetCapacity();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::CheckIfTied
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Parameter: 	CFixture* pLastFixture
	Parameter: 	CFixture& aFixture
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCup::DoCheckIfTied(CCupRound& _CupRound, CFixture* _LastFixture, CFixture& _Fixture)
{
	byte HomeGoalsToScore, AwayGoalsToScore;
	switch(_CupRound.GetRoundType())
	{
		case LEG1:
		case LEG1AWAYDOUBLE:
		case LEG12REGIONS:
		case LEG1AWAYDOUBLE2REGIONS:
		case LEG1AWAYDOUBLE1423:
		case LEG1AWAYDOUBLE1324:
		case NORMALMATCH:
#ifdef CUPGROUPS
		case GROUPMATCH:
		case GROUP415372648:
#endif
#ifdef CUP_GROUP3ONCE2REGIONS
		case GROUP3ONCE2REGIONS:
#endif
		case NEUTRAL:
		case NORMALMATCH2REGIONS:
		case REVERSEDNORMAL2REGIONS:
		case NEUTRAL2REGIONS:
			return false;
	}

	if (_LastFixture == null)
	{
		// Single leg match
		if (_Fixture.GetNumberGoals(HOME) == _Fixture.GetNumberGoals(AWAY))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// Get the aggregate score over the 2 legs
	HomeGoalsToScore = _LastFixture->GetNumberGoals(AWAY) + _Fixture.GetNumberGoals(HOME);
	AwayGoalsToScore = _LastFixture->GetNumberGoals(HOME) + _Fixture.GetNumberGoals(AWAY);

	// If a draw over 2 legs
	if (HomeGoalsToScore == AwayGoalsToScore && _LastFixture != null)
	{
		// Check if away goals count double
		if (_CupRound.DoAwayGoalsCountDouble() == true)
		{
			// Yes so add them on
			HomeGoalsToScore = _LastFixture->GetNumberGoals(AWAY) * 2 + _Fixture.GetNumberGoals(HOME);
			AwayGoalsToScore = _LastFixture->GetNumberGoals(HOME) + _Fixture.GetNumberGoals(AWAY) * 2;
		}
	}
	if (HomeGoalsToScore == AwayGoalsToScore)
	{
		return true;
	}
	else
	{
		return  false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNextRoundNumber
	Access:    	public 
	Parameter: 	CMatchTime& aCurrentDate
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCup::GetNextRoundNumber(CCalendar& _CurrentDate)
{
    for (int ListPos = m_TheRounds.GetSize() - 1; ListPos > -1; ListPos --)
	{
		// If this round has been drawn
        if (m_TheRounds[ListPos].GetDrawDate() != IGNOREDATE && m_TheRounds[ListPos].GetDrawDate() <= _CurrentDate.GetMatchDay())
		{
			// If the draw has been made
			if (m_TheRounds[ListPos].GetMatchList().GetSize() > 0)
			{
				return ListPos;
			}
		}
	}
	return wNOTFOUND;		// No draw been made yet
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetFirstLeg
	Access:    	public 
	Parameter: 	CCupRound& aCupRound
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCup::GetFirstLeg(CCupRound& _CupRound)
{
	CCupRound* theRound = null;
    for (int ListPos = 1; ListPos < m_TheRounds.GetSize(); ListPos++)
	{
        if (&m_TheRounds[ListPos] == &_CupRound)
		{
            theRound = &m_TheRounds[ListPos - 1];
			break;
		}
	}
	EXT_ASSERT(theRound != null);		// 1st leg not found
	return theRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::SeasonStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCup::SeasonStr()
{
	return GetActiveCountry().GetSeasonStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoEndOfSeason
	Access:    	public 
	Parameter: 	bool AddTrophy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoEndOfSeason(bool _AddTrophy /*= true*/)
{
	EXT_ASSERT(SeasonStr().IsEmpty() != TRUE);
	ushort LastRound = m_TheRounds.GetUpperBound();
	EXT_ASSERT(m_TheRounds[LastRound].GetRoundType() != ENDOFCUP);
	EXT_ASSERT(m_TheRounds[LastRound].GetMatchList().GetSize() > 0);

	// Get the club numbers of the finalists
    CNumericArray<ushort> DrawClubs, Losers;
#ifdef CUPGROUPS
	if (m_TheRounds[LastRound].GetRoundType() == GROUPMATCH)
	{
		// Play off mini league probably
		m_TheRounds[LastRound] = m_TheRounds.DoFindFirstGroupMatch();
		GetGroupMatchWinners(DrawClubs, m_TheRounds[LastRound].GetNumInRound() / m_TheRounds[LastRound].GetNumberOfClubsInGroup());
	}
	else
#endif
	{
		// Knockout final
		m_TheRounds[LastRound].DoFindChampionsAndRunnersUp(DrawClubs, Losers, false, *this);
	}
	CBaseCompetition::DoAddChampions(DrawClubs[0]);
	CBaseCompetition::DoAddRunnersUp(DrawClubs[1]);

	// If the winner qualifies for a Charity Shield then add it to its draw list
	CCharityShield* pCharityShield = WorldData().GetCharityShieldList().DoFind(m_CharityShieldID);
	if (pCharityShield != null)
	{
		pCharityShield->DoAddClub(DrawClubs[0]);
	}

	// Add to history
	if (_AddTrophy == true)
	{
		if (DrawClubs[0] != NOCLUB)
		{
		    WorldData().GetClub(DrawClubs[0]).WonCup(this);
		}

		if (DrawClubs.GetSize() > 1 && DrawClubs[1] != NOCLUB)
		{
	        WorldData().GetClub(DrawClubs[1]).RunnersUp(this);
		}
	}

	m_CupTiedPlayerList.RemoveList();
	m_TheRounds.DoEndOfSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::NewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoNewSeason()
{
    m_LeagueClubsList = GetActiveCountry().GetClubsStartingOrder();
	EXT_ASSERT(m_LeagueClubsList.CheckForDuplicates() == false);
#ifdef ACTIVEFEDERATIONCUPS
	if (m_ExcludeFederationQualifiersUntilRound > 0)
	{
		m_LeagueClubsList.RemoveIfFoundInArray2(&m_FederationQualifiersList);
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCup::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::Read(CDataFile* _pDataFile)
{
// 	DEBUG_OUTPUT(("Reading Cup %s"), (GetName()));
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CBaseCompetition::Read(_pDataFile);
    _pDataFile->Read(&m_SpecialActionFlag, sizeof m_SpecialActionFlag);
    _pDataFile->Read(&m_CountryID, sizeof m_CountryID);
	m_NonLeagueClubs.Read(_pDataFile);
	EXT_ASSERT(m_NonLeagueClubs.GetSize() < 40);
    m_TheRounds.Read(_pDataFile);
    _pDataFile->Read(&m_NeutralVenueStadiumID, sizeof m_NeutralVenueStadiumID);
    m_LeagueClubsList.Read(_pDataFile);
#ifdef ACTIVEFEDERATIONCUPS
    m_FederationQualifiersList.Read(_pDataFile);
    _pDataFile->Read(&m_ExcludeFederationQualifiersUntilRound, sizeof m_ExcludeFederationQualifiersUntilRound);
#endif
    _pDataFile->Read(&m_CharityShieldID, sizeof m_CharityShieldID);
	m_CupTiedPlayerList.Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CCup::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::Write(CDataFile* _pDataFile)
{
// 	DEBUG_OUTPUT(("Writing Cup %s"), (GetName()));
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CBaseCompetition::Write(_pDataFile);
    _pDataFile->Write(&m_SpecialActionFlag, sizeof m_SpecialActionFlag);
    _pDataFile->Write(&m_CountryID, sizeof m_CountryID);
    m_NonLeagueClubs.Write(_pDataFile);
	m_TheRounds.Write(_pDataFile);
    _pDataFile->Write(&m_NeutralVenueStadiumID, sizeof m_NeutralVenueStadiumID);
    m_LeagueClubsList.Write(_pDataFile);
#ifdef ACTIVEFEDERATIONCUPS
    m_FederationQualifiersList.Write(_pDataFile);
    _pDataFile->Write(&m_ExcludeFederationQualifiersUntilRound, sizeof m_ExcludeFederationQualifiersUntilRound);
#endif
    _pDataFile->Write(&m_CharityShieldID, sizeof m_CharityShieldID);
	m_CupTiedPlayerList.Write(_pDataFile);
 }


/*------------------------------------------------------------------------------
	Method:   	CCup::DoPlayMissedMatches
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoPlayMissedMatches()
{
	CCalendar& theMatchTime = WorldData().GetCurrentDate(); 
	byte CurrentWeekNumber = theMatchTime.GetMatchDay();
	byte SeasonStart = GetActiveCountry().GetSeasonStart();
	while(theMatchTime.GetMatchDay() != SeasonStart)
	{
		theMatchTime.PreviousMatchDay();			// back a match
	}
	DoNewSeason();
	// play all matches to present date
	while(CurrentWeekNumber != theMatchTime.GetMatchDay())
	{
		GetActiveCountry().DoDailyClearDown();
		DoPlayMatches(false);
		DoDraws();
		theMatchTime.NextMatchDay();			// Update the date
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::SetNeutralVenueStadiumID
	Access:    	public 
	Parameter: 	const ushort _NeutralStadiumID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::SetNeutralVenueStadiumID(const ushort _NeutralStadiumID)
{
	m_NeutralVenueStadiumID = _NeutralStadiumID;
}


#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
/*------------------------------------------------------------------------------
Method:   	CCup::GetPercentagePointsForPlay0ffs
Access:    	public 
Parameter: 	void
Returns:   	byte
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetPercentagePointsForPlay0ffs()
{
	return m_PercentagePointsForPlay0ffs;
}


/*------------------------------------------------------------------------------
Method:   	CCup::SetPercentagePointsForPlay0ffs
Access:    	public 
Parameter: 	const byte x
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCup::SetPercentagePointsForPlay0ffs(const byte x)
{
	m_PercentagePointsForPlay0ffs = x;
}
#endif


#ifdef CUPGROUPS
/*------------------------------------------------------------------------------
Method:   	CCup::AddClubsToGroup3
Access:    	public 
Parameter: 	const ushort RoundNum
Parameter: 	CNumericArray<ushort>& aTheClubs
Returns:   	void
Qualifier: 	
Purpose:	Add the list of clubs to the round club list
------------------------------------------------------------------------------*/
void CCup::AddClubsToGroup3(const ushort RoundNum, CNumericArray<ushort>& aTheClubs)
{
// 	EXT_ASSERT(RoundNum > -1 && RoundNum < m_TheRounds.GetSize());
// 	m_TheRounds[RoundNum].DoCopyTo3Rounds(*this, aTheClubs, m_TheRounds[RoundNum + 1], m_TheRounds[RoundNum + 2]);
}


/*------------------------------------------------------------------------------
	Method:   	CCup::DoBuildGroupTables
	Access:    	public 
	Parameter: 	CLeagueTableList & _TheTable
	Parameter: 	int _RoundNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::DoBuildGroupTables(CLeagueTableList& _TheTable, int _NumRounds /*= 0*/)
{
	CLeagueTableList PointsTable;
	CNumericArray<ushort> ClubIDList;
	CCupRound& pFirstCupRound = m_TheRounds.DoFindFirstGroupMatch();
	CCupRound* pSecondCupRound = m_TheRounds.GetNextRound(&pFirstCupRound);
	EXT_ASSERT(pSecondCupRound != null);
	int NumGroups = pFirstCupRound.GetNumberOfGroups();
	EXT_ASSERT(NumGroups > 0);
	for (int GroupNumber = 0; GroupNumber < NumGroups; GroupNumber++)
	{
		PointsTable.DoCreate(pFirstCupRound.GetNumberOfClubsInGroup());
		pFirstCupRound.GetGroupClubNumbers(GroupNumber, &ClubIDList, pSecondCupRound);
		PointsTable.SetClubIDs(&ClubIDList);				// Put the club numbers into the tables
		m_TheRounds.DoBuildGroupTables(*this, GroupNumber, PointsTable, _NumRounds);
		PointsTable.DoSort();								// DoSort the teams into order
		_TheTable.DoAdd(PointsTable);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNumberOfClubsInGroup
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetNumberOfClubsInGroup()
{
	return m_TheRounds.DoFindFirstGroupMatch().GetNumberOfClubsInGroup();
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetNumberOfGroups
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCup::GetNumberOfGroups()
{
	return m_TheRounds.DoFindFirstGroupMatch().GetNumberOfGroups();
}

/*------------------------------------------------------------------------------
	Method:   	CCup::GetNumberGroupMatchesPlayed
	Access:    	public 
	Parameter: 	const ushort RoundNum
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCup::GetNumberGroupMatchesPlayed(const ushort RoundNum)
{
	EXT_ASSERT(RoundNum > -1 && RoundNum < m_TheRounds.GetSize());
	EXT_ASSERT((m_TheRounds[RoundNum].IsGroupMatch() == true));
	int iRet = 0;
	for (int LoopCount = RoundNum; LoopCount > -1; LoopCount--)
	{
		if (m_TheRounds[RoundNum].IsGroupMatch() == false)
		{
			break;
		}
		else
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::GetGroupMatchWinners
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aTheClubs
	Parameter: 	int NumInNextRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCup::GetGroupMatchWinners(CNumericArray<ushort>& _TheClubID, const int _NumInNextRound)
{
	EXT_ASSERT(_NumInNextRound > 0);
	CCupRound* pFirstCupRound = &m_TheRounds.DoFindFirstGroupMatch();
	EXT_ASSERT(pFirstCupRound != null);
	CNumericArray<ushort> ClubIDList;
	ClubIDList.SetSize(pFirstCupRound->GetNumberOfClubsInGroup());

	int NumGroups = pFirstCupRound->GetNumInRound() / pFirstCupRound->GetNumberOfClubsInGroup();
	EXT_ASSERT(NumGroups > 0);
	int NumWinners;
	int NumRunnersUp;
	if (_NumInNextRound != 0)
	{
		NumWinners	= _NumInNextRound / NumGroups;
		NumRunnersUp = _NumInNextRound - (NumWinners * NumGroups);		// If not equal number of winners from each group}
	}
	else
	{
		NumWinners	= pFirstCupRound->GetNumberOfClubsInGroup();
		NumRunnersUp = 0;
	}
	EXT_ASSERT(NumWinners > 0);

	CCupRound* pSecondCupRound = m_TheRounds.GetNextRound(pFirstCupRound);
	EXT_ASSERT(pSecondCupRound != null);
	CLeagueTableList PointsTable;
	PointsTable.DoCreate(pFirstCupRound->GetNumberOfClubsInGroup());
	
	CLeagueTableList RunnersUpTable;
	for (int GroupNumber = 0; GroupNumber < NumGroups; GroupNumber++)
	{
		PointsTable.DoNewSeason();
		pFirstCupRound->GetGroupClubNumbers(GroupNumber, &ClubIDList, pSecondCupRound);
		PointsTable.SetClubIDs(&ClubIDList);			// Put the club numbers into the tables
		m_TheRounds.DoBuildGroupTables(*this, GroupNumber, PointsTable, 0);
		PointsTable.DoSort();								// DoSort the teams into order

		// Add Winners to The Clubs array
		for (int LoopCount = 0; LoopCount < NumWinners; LoopCount++)
		{
			_TheClubID.Add(PointsTable.GetAt(LoopCount)->GetClubID());
		}
		if (NumRunnersUp > 0)
		{
			RunnersUpTable.Add(PointsTable.GetAt(NumWinners));
			PointsTable.SetAt(NumWinners, new CLeagueTable());
		}

		// Send a message to the clubs that are knocked out
		/*	for (uint LoopCount = NumWinners + (NumRunnersUp > 0); LoopCount < PointsTable.GetSize(); LoopCount++)
		{
		pFirstCupRound->OutOfCupMessage(PointsTable.GetAt(LoopCount)->GetClubID(), CBaseCompetition::GetName(), false);
		}
		NumRunnersUp -= (NumRunnersUp > 0);*/
	}
	RunnersUpTable.DoSort();								// DoSort the teams into order
	for (int LoopCount = 0; LoopCount < NumRunnersUp; LoopCount++)
	{
		_TheClubID.Add(RunnersUpTable.GetAt(LoopCount)->GetClubID());
	}

	RunnersUpTable.RemoveList();
	PointsTable.RemoveList();
	EXT_ASSERT(_TheClubID.GetSize() == _NumInNextRound);
}


/*------------------------------------------------------------------------------
Method:   	CCup::GetWinners2RegionsGroup3Once
Access:    	public 
Parameter: 	const ushort NextRoundNum
Parameter: 	CNumericArray<ushort>& aTheClubs
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCup::GetWinners2RegionsGroup3Once(const ushort NextRoundNum, CNumericArray<ushort>& aTheClubs)
{
	EXT_ASSERT(NextRoundNum > 2);		// Must be 3 prevoius rounds
	int NumInRound = m_TheRounds[NextRoundNum - 3].GetMatchList().GetSize();
	CMatchInfo MatchDetails[3];
	StPoints PointsTable[3];
	StPoints TempPoints;
	bool swap = true;
	CFixture* pFixture;

	for (int LoopCount = 0; LoopCount < m_TheRounds[NextRoundNum - 3].GetMatchList().GetSize(); LoopCount++)
	{
// 		GetMatchDetails(NextRoundNum - 3, LoopCount, MatchDetails[0]);	// 1 - 2
// 		GetMatchDetails(NextRoundNum - 2, LoopCount, MatchDetails[1]);	// 2 - 3
// 		GetMatchDetails(NextRoundNum - 1, LoopCount, MatchDetails[2]);	// 3 - 1

		for (uint x = 0; x < 3; x++)
		{
			if (x == 2)
			{
				PointsTable[x].ClubNum = MatchDetails[x].GetHomeClubID();
				PointsTable[x - 2].ClubNum = MatchDetails[x].GetAwayClubID();
				pFixture = MatchDetails[LoopCount * 3 + x].GetFixture();

				PointsTable[x].GoalsFor += pFixture->GetNumberGoals(HOME);
				PointsTable[x].GoalsAgg += pFixture->GetNumberGoals(AWAY);
				PointsTable[x - 2].GoalsFor += pFixture->GetNumberGoals(AWAY);
				PointsTable[x - 2].GoalsAgg += pFixture->GetNumberGoals(HOME);

				if (pFixture->GetNumberGoals(HOME) > pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x].Points += 3;
				}
				else if (pFixture->GetNumberGoals(HOME) < pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x - 2].Points += 3;
				}
				else if (pFixture->GetNumberGoals(HOME) == pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x].Points ++;
					PointsTable[x - 2].Points ++;
				}
			}
			else
			{
				pFixture = MatchDetails[x].GetFixture();
				PointsTable[x].ClubNum = MatchDetails[x].GetHomeClubID();
				PointsTable[x + 1].ClubNum = MatchDetails[x].GetAwayClubID();
				PointsTable[x].GoalsFor += 	pFixture->GetNumberGoals(HOME);
				PointsTable[x].GoalsAgg += pFixture->GetNumberGoals(AWAY);
				PointsTable[x + 1].GoalsFor += pFixture->GetNumberGoals(AWAY);
				PointsTable[x + 1].GoalsAgg += pFixture->GetNumberGoals(HOME);

				if (pFixture->GetNumberGoals(HOME) > pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x].Points += 3;
				}
				else if (pFixture->GetNumberGoals(HOME) < pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x + 1].Points += 3;
				}
				else if (pFixture->GetNumberGoals(HOME) == pFixture->GetNumberGoals(AWAY))
				{
					PointsTable[x].Points ++;
					PointsTable[x + 1].Points ++;
				}
			}
		}

		// DoSort the teams into order
		while(swap == true)
		{
			swap = false;
			for (uint y = 0; y < 2; y++)
			{
				if (PointsTable[y].Points < PointsTable[y + 1].Points)
				{
					TempPoints = PointsTable[y];
					PointsTable[y] = PointsTable[y + 1];
					PointsTable[y + 1] = TempPoints;
					swap = true;
				}
				if ((swap == false) && (PointsTable[y].Points == PointsTable[y + 1].Points))
				{
					if ((PointsTable[y].GoalsFor - PointsTable[y].GoalsAgg) < (PointsTable[y + 1].GoalsFor - PointsTable[y + 1].GoalsAgg))
					{
						TempPoints = PointsTable[y];
						PointsTable[y] = PointsTable[y + 1];
						PointsTable[y + 1] = TempPoints;
						swap = true;
					}
				}
			}
		}

		// Add Winners to The Clubs array
		if (PointsTable[1].Points != PointsTable[2].Points)
		{
			aTheClubs.Add(PointsTable[0].ClubNum);
			aTheClubs.Add(PointsTable[1].ClubNum);
		}
		else if (PointsTable[1].Points == PointsTable[2].Points)
		{
			if (RandomNumber.IntLessThan(10) < 10)
			{
				aTheClubs.Add(PointsTable[0].ClubNum);
			}
			else
			{
				aTheClubs.Add(PointsTable[1].ClubNum);
			}
		}
		else if ((PointsTable[0].Points == PointsTable[2].Points) && (PointsTable[1].Points == PointsTable[2].Points))
		{
			int num = RandomNumber.IntLessThan(30);
			if ((num < 30) && (num > 20))
			{
				aTheClubs.Add(PointsTable[0].ClubNum);
				aTheClubs.Add(PointsTable[1].ClubNum);
			}
			else if ((num < 20) && (num > 10))
			{
				aTheClubs.Add(PointsTable[0].ClubNum);
				aTheClubs.Add(PointsTable[2].ClubNum);
			}
			else if (num < 10)
			{
				aTheClubs.Add(PointsTable[1].ClubNum);
				aTheClubs.Add(PointsTable[2].ClubNum);
			}
		}
	}
}


/*------------------------------------------------------------------------------
Method:   	CCup::CreateGroupMatches
Access:    	public 
Parameter: 	CCupRound& pCupRound
Parameter: 	CNumericArray<ushort>& aDrawClubs
Returns:   	void
Qualifier: 	
Purpose:	Transfer the draw to the fixtures for group match rounds
------------------------------------------------------------------------------*/
void CCup::CreateGroupMatches(CCupRound& pCupRound, CNumericArray<ushort>& aDrawClubs)
{
	//	EXT_ASSERT(pCupRound != null);
	switch(pCupRound.GetNumberOfClubsInGroup())
	{
	case 4:
		DoCreateGroup4MatchList(pCupRound);
		if (pCupRound.GetNumberOfGroupMatches() == 2)
		{
			ReverseGroup4MatchList(pCupRound);
		}
		break;
#ifdef CUPGROUPS5
	case 5:
		CreateGroup5MatchList(pCupRound, aDrawClubs);
		if (pCupRound.GetNumberOfGroupMatches() == 2)
		{
			ReverseGroup6MatchList(pCupRound);			// Same number of rounds as 6 so can use same method
		}
		break;
#endif
#ifdef CUPGROUPS6
	case 6:
		CreateGroup6MatchList(pCupRound);
		if (pCupRound.GetNumberOfGroupMatches() == 2)
		{
			ReverseGroup6MatchList(pCupRound);
		}
		break;
#endif
#ifdef CUPGROUPS8
	case 8:
		CreateGroup8MatchList(pCupRound);
		if (pCupRound.GetNumberOfGroupMatches() == 2)
		{
			ReverseGroupMatchList(pCupRound);
		}
		break;
#endif
	default :
		EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
Method:   	CCup::DoCreateGroup4MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Transfer the draw to the fixtures for groups of 4 teams match 
rounds(1 venue only)
------------------------------------------------------------------------------*/
void CCup::DoCreateGroup4MatchList(CCupRound& _CupRound)
{
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	CNumericArray<ushort> TheClubs;
	TheClubs.SetSize(4);
	ushort Capacity = 0;
	if (_CupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity();
	}

	// Setup the fixtures 1 group of 4 teams at a time
	CMatchInfo Match(_CupRound.VenueType(), GetSubsSelect(), GetSubsUse());
	//Match.SetStadiumSize(Capacity);
	int RoundNumber = DoFindRoundNumber(_CupRound);
	for (int LoopCount = 0; LoopCount < _CupRound.GetMatchList().GetSize(); LoopCount += 2)
	{
		TheClubs.SetAt(0, _CupRound.GetMatchList().GetAt(LoopCount)->GetHomeClubID());
		TheClubs.SetAt(1, _CupRound.GetMatchList().GetAt(LoopCount)->GetAwayClubID());
		TheClubs.SetAt(2, _CupRound.GetMatchList().GetAt(LoopCount + 1)->GetHomeClubID());
		TheClubs.SetAt(3, _CupRound.GetMatchList().GetAt(LoopCount + 1)->GetAwayClubID());

		Match.DoInitialiseClubs(TheClubs[1], TheClubs[2]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 1].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[3], TheClubs[0]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 1].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[0], TheClubs[2]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 2].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[3], TheClubs[1]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 2].GetMatchList().Add(new CFixture(Match));
	}
}


#ifdef CUPGROUPS5
/*------------------------------------------------------------------------------
Method:   	CCup::CreateGroup5MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Parameter: 	CNumericArray<ushort>& aTheClubs
Returns:   	void
Qualifier: 	
Purpose:	Transfer the draw to the fixtures for groups of 5 teams match 
rounds(1 venue only)
------------------------------------------------------------------------------*/
void CCup::CreateGroup5MatchList(CCupRound& pCupRound, CNumericArray<ushort>& aTheClubs)
{
	int RefereeNumber = 0;
	int ListPos = DoFindRoundNumber(&pCupRound);
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity(0);
	}

	// Setup the fixtures 1 group of 5 teams at a time
	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
	Match.SetStadiumSize(Capacity);
	for (uint n = 0; n < pCupRound.GetMatchList().GetSize(); n += 2)
	{
		Match.DoInitialiseClubs(aTheClubs[1 + (n / 2) * 5], aTheClubs[4 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 1].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[2 + (n / 2) * 5], aTheClubs[0 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 1].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[0 + (n / 2) * 5], aTheClubs[4 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 2].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[3 + (n / 2) * 5], aTheClubs[1 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 2].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[2 + (n / 2) * 5], aTheClubs[1 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 3].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[4 + (n / 2) * 5], aTheClubs[3 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 3].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[0 + (n / 2) * 5], aTheClubs[3 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 4].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(aTheClubs[2 + (n / 2) * 5], aTheClubs[4 + (n / 2) * 5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[ListPos + 4].GetMatchList().Add(new CFixture(Match));
	}
}
#endif


#ifdef CUPGROUPS6
/*------------------------------------------------------------------------------
Method:   	CCup::CreateGroup6MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Transfer the draw to the fixtures for groups of 6 teams match 
rounds(1 venue only)
------------------------------------------------------------------------------*/
void CCup::CreateGroup6MatchList(CCupRound& pCupRound)
{
	int RefereeNumber = 0, z;
	int ListPos = DoFindRoundNumber(pCupRound);
	CFixture* pFixture;
	CNumericArray<ushort> TheClubs;
	TheClubs.SetSize(6);
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity(0);
	}

	// Setup the fixtures 1 group of 6 teams at a time
	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
	Match.SetStadiumSize(Capacity);
	for (uint LoopCount = 0; LoopCount < pCupRound.GetMatchList().GetSize(); LoopCount += 3)
	{
		// Get the first round details
		TheClubs.RemoveAll();
		for (z = 0; z < 3; z++)
		{
			pFixture = pCupRound.GetMatchList().GetAt(LoopCount + z);
			TheClubs.Add(pFixture->GetHomeClubID());
			TheClubs.Add(pFixture->GetAwayClubID());
		}

		// Create the remaing rounds details
		m_TheRounds[ListPos + 1].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[1], TheClubs[4]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif

		m_TheRounds[ListPos + 1].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[2], TheClubs[0]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif

		m_TheRounds[ListPos + 1].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[3], TheClubs[5]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif

		m_TheRounds[ListPos + 2].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[0], TheClubs[4]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif

		m_TheRounds[ListPos + 2].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[5], TheClubs[2]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
		m_TheRounds[ListPos + 2].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[3], TheClubs[1]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);

		m_TheRounds[ListPos + 3].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[2], TheClubs[1]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
		m_TheRounds[ListPos + 3].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[4], TheClubs[3]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
		m_TheRounds[ListPos + 3].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[5], TheClubs[0]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);

		m_TheRounds[ListPos + 4].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[0], TheClubs[3]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
		m_TheRounds[ListPos + 4].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[1], TheClubs[5]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
		m_TheRounds[ListPos + 4].GetMatchList().Add(new CFixture(Match));
		Match.DoInitialiseClubs(TheClubs[2], TheClubs[4]
#ifdef	ACTIVEREFEREES
		, RefereeNumber
#endif
			);
	}
}
#endif


#ifdef CUPGROUPS8 
/*------------------------------------------------------------------------------
Method:   	CCup::CreateGroup8MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Transfer the draw to the fixtures for groups of 8 teams match 
rounds(1 venue only)
------------------------------------------------------------------------------*/
void CCup::CreateGroup8MatchList(CCupRound& pCupRound)
{
	ushort LoopCount;
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	int ListPos = DoFindRoundNumber(&pCupRound);
	ushort NumInRound = pCupRound.GetNumberOfClubsInGroup();
	Vector<CCupRound*> GroupRounds;
	for (LoopCount = 0; LoopCount < NumInRound - 1; LoopCount++)
	{
		GroupRounds.Add(m_TheRounds[ListPos++]);
	}
	CNumericArray<ushort> TheClubs;
	TheClubs.SetSize(8);
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity(0);
	}

	// Setup the fixtures 1 group of 8 teams at a time
	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
	Match.SetStadiumSize(Capacity);
	CNumericArray<byte>& pBaseFixtureList = WorldData().GetFixtureController().GetFixtureList(8);
	for (int LoopCount = 0; LoopCount < pCupRound.GetMatchList().GetSize(); LoopCount += NumInRound / 2)
	{
		Count = NumInRound;                      // 1st 8 already allocated to 1st round
		TheClubs.RemoveAll();
		// Get the first round details
		for (uint z = 0; z < NumInRound / 2; z++)
		{
			TheClubs.Add(pCupRound.GetMatchList().GetAt(LoopCount + z)->GetHomeClubID());
			TheClubs.Add(pCupRound.GetMatchList().GetAt(LoopCount + z)->GetAwayClubID());
		}

		// Create the remaing rounds details
		for (uint Round = 1; Round < GroupRounds.GetUpperBound(); Round++)
		{
			for (uint nMatch = 0; nMatch < NumInRound / 2; nMatch++)
			{
				Match.DoInitialiseClubs(TheClubs[pBaseFixtureList[Count++] - 1],
					TheClubs[pBaseFixtureList[Count++] - 1]);
#ifdef	ACTIVEREFEREES
				Match.SetReferee(RefereeNumber);
#endif
				GroupRounds.elementAt(Round).GetMatchList().Add(new CFixture(Match));
			}
		}
	}
}
#endif

/*------------------------------------------------------------------------------
Method:   	CCup::ReverseGroup4MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Reverse the draw to the fixtures for groups of 4 teams 
match rounds
------------------------------------------------------------------------------*/
void CCup::ReverseGroup4MatchList(CCupRound& pCupRound)
{
	int RoundNumber = DoFindRoundNumber(pCupRound);
	CNumericArray<ushort> TheClubs;
	TheClubs.SetSize(4);
	int RefereeNumber = 0;
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity();
	}

	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
//	Match.SetStadiumSize(Capacity);
	for (int LoopCount = 0; LoopCount < pCupRound.GetMatchList().GetSize(); LoopCount += 2)
	{
		TheClubs.SetAt(0, pCupRound.GetMatchList().GetAt(LoopCount)->GetHomeClubID());
		TheClubs.SetAt(1, pCupRound.GetMatchList().GetAt(LoopCount)->GetAwayClubID());
		TheClubs.SetAt(2, pCupRound.GetMatchList().GetAt(LoopCount + 1)->GetHomeClubID());
		TheClubs.SetAt(3, pCupRound.GetMatchList().GetAt(LoopCount + 1)->GetAwayClubID());

		Match.DoInitialiseClubs(TheClubs[1], TheClubs[0]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 3].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[3], TheClubs[2]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 3].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[2], TheClubs[1]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 4].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[0], TheClubs[3]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 4].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[2], TheClubs[0]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_TheRounds[RoundNumber + 5].GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(TheClubs[1], TheClubs[3]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber)
#endif
			m_TheRounds[RoundNumber + 5].GetMatchList().Add(new CFixture(Match));
	}
}


#ifdef CUPGROUPS6
/*------------------------------------------------------------------------------
Method:   	CCup::ReverseGroup6MatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Reverse the draw to the fixtures for groups of 6 teams 
match rounds
------------------------------------------------------------------------------*/
void CCup::ReverseGroup6MatchList(CCupRound& pCupRound)
{
	// 	EXT_ASSERT(pCupRound != null);
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity(0);
	}
	int ListPos = DoFindRoundNumber(&pCupRound);
	CFixture* pFixture;
	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
	Match.SetStadiumSize(Capacity);
	for (uint Round = 0; Round < 5; Round++)
	{
		for (uint n = 0; n < pRound.getMatchList().GetSize(); n ++)
		{
			pFixture = m_TheRounds[ListPos].GetMatchList()[n];
			Match.DoInitialiseClubs(pFixture->GetAwayClubID(), pFixture->GetHomeClubID());
#ifdef	ACTIVEREFEREES
			Match.SetReferee(RefereeNumber);
#endif
			m_TheRounds[ListPos + 5].GetMatchList().Add(new CFixture(Match));
		}
		ListPos++;
	}
}
#endif

/*------------------------------------------------------------------------------
Method:   	CCup::ReverseGroupMatchList
Access:    	public 
Parameter: 	CCupRound& pCupRound
Returns:   	void
Qualifier: 	
Purpose:	Reverse the draw to the fixtures for groups
------------------------------------------------------------------------------*/
void CCup::ReverseGroupMatchList(CCupRound& pCupRound)
{
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	ushort Capacity = 0;
	if (pCupRound.VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Capacity = GetNeutralVenueStadiumCapacity();
	}
	int RoundNumber = DoFindRoundNumber(pCupRound);
	int NumInGroup = pCupRound.GetNumberOfClubsInGroup();
	//CFixture* pFixture;

	CMatchInfo Match(pCupRound.VenueType(), GetSubsSelect(), GetSubsUse());
//	Match.SetStadiumSize(Capacity);
	for (int Round = 0; Round < NumInGroup - 1; Round++)
	{
//		for (uint LoopCount = 0; LoopCount < pRound->getMatchList().GetSize(); LoopCount ++)
		EXT_ASSERT(false);
		{
// 			pFixture = m_TheRounds[RoundNumber].GetMatchList().GetAt(LoopCount);
// 			Match.DoInitialiseClubs(pFixture->GetAwayClubID(), pFixture->GetHomeClubID());
// #ifdef	ACTIVEREFEREES
// 			Match.SetReferee(RefereeNumber);
// #endif
// 			m_TheRounds[RoundNumber + NumInGroup - 1].GetMatchList().Add(new CFixture(Match));
		}
		RoundNumber++;
	}
}
#endif


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
Method:   	CCup::GetFederationQualifiersList
Access:    	public 
Parameter: 	void
Returns:   	CNumericArray<ushort>&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CNumericArray<ushort>& CCup::GetFederationQualifiersList()
{
	return m_FederationQualifiersList;
}


/*------------------------------------------------------------------------------
	Method:   	CCup::pFederation
	Access:    	public 
	Parameter: 	void
	Returns:   	CFederation*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CCup::GetFederation()
{
	return GetCountry()->GetFederation();
}
#endif


#ifdef NEWSPAPERON
/*------------------------------------------------------------------------------
Method:   	CCup::NewsPaperWinnersHeadline
Access:    	public 
Parameter: 	void
Returns:   	CString
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CString CCup::NewsPaperWinnersHeadline()
{
	CString str;
	//	if (HaveAllRoundsBeenPlayed() == true)
	//{
	//		str.FormatMessage(IDS_NEWSPAPERWINNERSHEADLINE, WinnerNameStr(), RunnerUpNameStr(), CBaseCompetition::GetName());
	//	}
	return str;
}
#endif


#ifdef SPANISHCUP
/*------------------------------------------------------------------------------
Method:   	CCup::SetUpSpanishCopaDelRay
Access:    	public 
Parameter: 	void
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CCup::SetUpSpanishCopaDelRay()
{
#ifdef ACTIVEFEDERATIONCUPS
	switch(m_FederationQualifiersList.GetSize())
	{
	case 8:
		m_TheRounds[0].SetNumberNonLeagueClubsToAdd(18);
		m_TheRounds[0].SetRoundSize(40);
		m_TheRounds[2].SetRoundSize(32);
		m_TheRounds[4].SetRoundSize(16);
		break;
	case 9:
		m_TheRounds[0].SetNumberNonLeagueClubsToAdd(12);
		m_TheRounds[0].SetRoundSize(34);
		m_TheRounds[2].SetRoundSize(28);
		m_TheRounds[4].SetRoundSize(14);
		break;
	case 10:
		m_TheRounds[0]->SetNumberNonLeagueClubsToAdd(6);
		m_TheRounds[0].SetRoundSize(28);
		m_TheRounds[2].SetRoundSize(24);
		m_TheRounds[4].SetRoundSize(12);
		break;
	case 11:
		m_TheRounds[0]->SetNumberNonLeagueClubsToAdd(0);
		m_TheRounds[0].SetRoundSize(22);
		m_TheRounds[2].SetRoundSize(20);
		m_TheRounds[4].SetRoundSize(10);
		break;
	}
#endif
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCupList::CCupList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupList::CCupList()
{
	CBinaryReader theResource(0, IDR_CUP, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);

	m_pList = new CCup[CExtArray::GetSize()];
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].DoInitialiseFromResource(theResource, ListPos, 1, 2);
		m_pList[ListPos].GetCountry()->GetActiveCountry().AddCup(ListPos);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::~CCupList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupList::~CCupList()
{
	delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::DoDraws
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::DoDraws()
{
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].DoDraws();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::DoFindCompetitionTitleForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupList::DoFindCompetitionTitleForFixture(CFixture* _Fixture)
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
	Method:   	CCupList::IsClubMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime& CurrentDate
	Parameter: 	const ushort Club
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupList::IsClubMatchToday(const CCalendar& _CurrentDate, const ushort _ClubID, CMatchInfo& _Match)
{
    bool MatchPlayed = false;
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        MatchPlayed = m_pList[ListPos].IsClubMatchToday(_CurrentDate, _ClubID, _Match);
		if (MatchPlayed == true)
		{
			break;
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::PlayMatches
	Access:    	public 
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupList::DoPlayMatches(const bool _ShowSummary)
{
    bool MatchPlayed = false;
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        if (m_pList[ListPos].DoPlayMatches(_ShowSummary) == true)
		{
			MatchPlayed = true;
		}
        if (m_pList[ListPos].DoPlayReplays(REPLAYMATCH, _ShowSummary) == true)
		{
			MatchPlayed = true;
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].Read(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupList::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::DoEndOfSeason()
{
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].DoEndOfSeason();
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupList::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::DoNewSeason()
{
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].DoNewSeason();
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupList::IsPtrInList
	Access:    	public 
	Parameter: 	CCup* pCup
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupList::IsPtrInList(CCup* pCup)
{
	bool bRet = false;
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        if (GetAt(ListPos) == pCup)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}


int CCupList::Find(CCup* pCup)
{
	int iRet = -1;
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        if (GetAt(ListPos) == pCup)
		{
			iRet = ListPos;
			break;
		}
	}
	EXT_ASSERT(iRet != -1);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::PlayMissedMatches
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:	Play all the matches prior to the start of the game from the 
				start of the season upto end of the calendar() 
				Only to be called for countries where the season starts after 
				the end of season within calendar()
------------------------------------------------------------------------------*/
void CCupList::DoPlayMissedMatches()
{
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
        m_pList[ListPos].DoPlayMissedMatches();
	}
}



/*------------------------------------------------------------------------------
	Method:   	CCupList::DoBuildNameList
	Access:    	public 
	Parameter: 	CSortedStringList & _theList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupList::DoBuildNameList(CSortedStringList& _theList)
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		_theList.Add(m_pList[LoopCount].GetName(), LoopCount);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CCupList::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < CExtArray::GetSize());
	return static_cast<CCup*>(&m_pList[_ElementNumber]);
}
