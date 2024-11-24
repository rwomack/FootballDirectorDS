
/*
	File:- Division.cpp

	Division list class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#endif

#include "stdafx.h"


#ifndef TESTBED
#include "UI.h"
#endif

#ifdef NDS
#include "SortedStringList.h"
#include <TableData.h>
#endif

#include "MatchInfo.h"
#include "CurrentForm.h"
#include "Stadium.h"
#include "FootballDirector.h"
#include <IndexedIntArray.h>


const uint				MINTRANSFERLISTEDPLAYERS = 10;
const uint				RNDTRANSFERLISTATTEMPTS = 6;
const uint				MINTRANSFERLISTATTEMPTS = 5;


/*------------------------------------------------------------------------------
	Method:   	CBuySellPercentageChance::CBuySellPercentageChance
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CBuySellPercentageChance::CBuySellPercentageChance()
	: m_PercentageChanceBuy(0)
	, m_PercentageChanceSell(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CBuySellPercentageChance::GetPercentageChanceBuy
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CBuySellPercentageChance::GetPercentageChanceBuy() const
{
	return m_PercentageChanceBuy;
}


/*------------------------------------------------------------------------------
	Method:   	CBuySellPercentageChance::GetPercentageChanceSell
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CBuySellPercentageChance::GetPercentageChanceSell() const
{
	return m_PercentageChanceSell;
}


/*------------------------------------------------------------------------------
	Method:   	CBuySellPercentageChance::DoInitialiseFromResource
	Access:    	public
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CBuySellPercentageChance::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_PercentageChanceBuy = _Resource.ReadByte();
	m_PercentageChanceSell = _Resource.ReadByte();
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoInitialiseFromResource
	Access:    	public
	Parameter: 	CBinaryResource & _Resource
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	CBaseCompetition::DoInitialiseFromResource(_Resource);
    m_PointsForWin = _Resource.ReadByte();
    m_DivisionStrengthID = _Resource.ReadByte();
    m_NumberClubsPromoted = _Resource.ReadByte();
    m_NumberClubsRelegated = _Resource.ReadByte();
    m_CountryID = _Resource.ReadUInt16();
    m_CountrySellBuyChances.DoInitialiseFromResource(_Resource);
#ifdef FEDERATIONTRANSFERS
	m_FederationSellBuyChances.DoInitialiseFromResource(_Resource);
#endif
    m_CharityshieldID = _Resource.ReadUInt16();
#ifdef ACTIVEFEDERATIONCUP
	GetFederationCup1 ()->DoInitialiseFromFile(File);
    GetFederationCup2 ()->DoInitialiseFromFile(File);
#endif
	m_TopPlayOff.DoInitialiseFromResource(_Resource);
	m_BottomPlayOff.DoInitialiseFromResource(_Resource);
	if (m_TopPlayOff.IsInUse() == true)
	{
		GetCountry().GetActiveCountry().DoAddPlayOff(m_TopPlayOff.GetID());
	}
	if (m_BottomPlayOff.IsInUse() == true)
	{
		GetCountry().GetActiveCountry().DoAddPlayOff(m_BottomPlayOff.GetID());
	}
    m_PlayOffForTitle = false;
	if (_Resource.ReadByte() != 0)
	{
		m_PlayOffForTitle = true;
	}
	for (int nCounter = 0; nCounter < TOTALMATCHES; nCounter++)
	{
		m_MatchTypeCalendar[nCounter] = _Resource.ReadByte();
	}
    m_initializationData.m_FirstClubNumberInDiv = _Resource.ReadUInt16();
    m_initializationData.m_NumberOfClubs = _Resource.ReadByte();
	DoInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildFinishedOrderList
	Access:    	public
	Parameter: 	CNumericArray<ushort> & _ClubsStartingOrder
	Parameter: 	int _FirstOffset
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildFinishedOrderList(CNumericArray<ClubID>& _ClubsStartingOrder, int _FirstOffset)
{
	ushort Posn;
	CIndexedIntArray<ClubID> cPositions;
	for (int ListPos = 0; ListPos < GetNumberOfClubsInDivision(); ListPos++)
	{
		const ClubID cClubID = GetClubs()[ListPos];
		CClub &cClub = WorldData().GetClubByID(cClubID);

		Posn = cClub.GetTableData().GetLeaguePosition();
		cPositions.AddNew(cClubID, Posn);
	}
	cPositions.SortLow();
	for (int i = 0; i < cPositions.GetSize(); i++)
	{
		_ClubsStartingOrder.SetAt(i + _FirstOffset, cPositions.GetIndex(i));
	}
	DoClearTables();		// Reset league tables etc.
}

 //---------------------------------------------------------------------------------------
CDivision::~CDivision()
{
    m_MatchData.RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetDivisionStrengthID
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetDivisionStrengthID()
{
	return m_DivisionStrengthID;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberOfClubsInDivision
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetNumberOfClubsInDivision() const
{
	return m_Clubs.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetFirstClubNumberInDivision
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const Vector<ClubID> &CDivision::GetClubs() const
{
	return m_Clubs;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetClub
	Access:    	public
	Parameter: 	ushort _ClubInSlot
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CDivision::GetClub(ushort _ClubInSlot)
{
	return WorldData().GetClubByID(GetClubs()[_ClubInSlot]);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::AddClub
	Access:    	public
	Parameter: 	const ClubID cClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::AddClub(const ClubID cClubID)
{
	m_Clubs.Add(cClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::RemoveClub
	Access:    	public
	Parameter: 	const ClubID cClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::RemoveClub(const ClubID cClubID)
{
	m_Clubs.RemoveElement(cClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberClubsRelegated
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberClubsRelegated() const
{
	return m_NumberClubsRelegated;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberClubsAutomaticallyRelegated
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberClubsAutomaticallyRelegated()	const
{
	return m_NumberClubsRelegated;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberClubsBottomPlayOff
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberClubsBottomPlayOff()
{
	return m_BottomPlayOff.GetNumberClubs();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberMatchesPlayedThisSeason
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetNumberMatchesPlayedThisSeason()	const
{
	return m_NumberMatchesPlayedThisSeason;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberClubsPromoted
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberClubsPromoted() const
{
	return m_NumberClubsPromoted;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberOfClubsTopPlayOff
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberOfClubsTopPlayOff()
{
	return m_TopPlayOff.GetNumberClubs();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetHowLongTillNextLeagueMatch
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetHowLongTillNextLeagueMatch()
{
	return m_HowLongTillNextLeagueMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetWeeksToEndOfSeason
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetWeeksToEndOfSeason()
{
	return m_WeeksToEndOfSeason;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountMOMWinners
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::DoCountMOMWinners()
{
	ushort nCount = 0;
	for (uint LoopCount = 0; LoopCount < 12; LoopCount++)
	{
		if (m_MOMWinners[LoopCount].GetClubID().id != NOCLUB)
		{
			nCount++;
		}
	}
	return nCount;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCountrySellBuyChances
	Access:    	public
	Returns:   	CBuySellPercentageChance*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CBuySellPercentageChance* CDivision::GetCountrySellBuyChances()
{
	return &m_CountrySellBuyChances;
}


 //---------------------------------------------------------------------------------------
void CDivision::DoBuildPopularityClubPtrList(CNumericArray<ushort>& _TheList, const ushort _ThePopularity)
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (cClub.GetPopularityID() == _ThePopularity)
		{
			_TheList.Add(LoopCount);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoPromoteClubInTablePosition
	Access:    	public
	Parameter: 	const int _TablePosition
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CDivision::DoPromoteClubInTablePosition(const int _TablePosition)
{
	ClubID PromotedClubID = DoFindClubIDInTablePosition(_TablePosition);
	if (_TablePosition > 0)
	{
		// Not champions or runners up so need to acknowledge promotion and relegation
		WorldData().GetClubByID(PromotedClubID).BeenPromoted(false);
		if (_TablePosition == 1)
		{
			CBaseCompetition::SetRunnersUpClubID(PromotedClubID);
			WorldData().GetClubByID(PromotedClubID).RunnersUp(*this);
		}
	}
	else
	{
		CBaseCompetition::SetWinningClubID(PromotedClubID);
		WorldData().GetClubByID(PromotedClubID).WonTitle(false);
	}
	return PromotedClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberPromotedViaPlayOffs
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberPromotedViaPlayOffs()
{
	if (m_TopPlayOff.IsInUse() == true)
	{
		return GetTopPlayOff()->GetNumberPromoted();
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::MakeCountryRanking0Clubs
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::MakeCountryRanking0Clubs()
{
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        cClub.MakeCountryRanking0();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetTopPlayOff
	Access:    	public
	Returns:   	CPlayOff*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayOff* CDivision::GetTopPlayOff()
{
	if (m_TopPlayOff.IsInUse() == true)
	{
		return WorldData().GetPlayOffList().DoFindFromId(m_TopPlayOff.GetID());
	}
	else
	{
		return NULL;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetBottomPlayOff
	Access:    	public
	Returns:   	CPlayOff*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayOff* CDivision::GetBottomPlayOff()
{
	if (m_BottomPlayOff.IsInUse() == true)
	{
		return WorldData().GetPlayOffList().DoFindFromId(m_BottomPlayOff.GetID());
	}
	else
	{
		return NULL;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetAllTransferListed
	Access:    	public
	Parameter: 	CClub * _Club
	Parameter: 	CPlayerSearch & _PlayerSearch
	Returns:   	void
	Qualifier:
	Purpose:	Builds a list of all the transfer listed players excluding the
				players at the club in {_Club}
------------------------------------------------------------------------------*/
void CDivision::GetAllTransferListed(CClub* _Club, CPlayerSearch& _PlayerSearch)
{
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		ClubID cClubID = GetClubs()[LoopCount];
        CClub& theClub = WorldData().GetClubByID(cClubID);
        if (_Club != &theClub)
		{
            theClub.GetFirstTeamSquad().GetAllTransferListed(cClubID, _PlayerSearch);
        }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::PointsOfClubInPosition
	Access:    	public
	Parameter: 	int Position
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::PointsOfClubInPosition(int Position)
{
	EXT_ASSERT(Position < GetNumberOfClubsInDivision());
	return DoFindClubInTablePosition(Position).GetTableData().GetPoints();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::NumberOfPointsToEscapeRelegation
	Access:    	public
	Parameter: 	CClub * _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::NumberOfPointsToEscapeRelegation(CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
// 	int PointsToEscape = 7;
// 	if (GetNumberOfClubsInDivision() < 14)
// 	{
// 		PointsToEscape = 3;
// 	}
	return PointsOfClubInPosition(GetNumberOfClubsInDivision() - 1 - m_NumberClubsRelegated) - _Club->GetTableData().GetPoints();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoInitialise
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoInitialise()
{
  //SIMON: Set weeks to end of season to maximum value to prevent end of season processing applying during init
  m_WeeksToEndOfSeason = 255;

	// Add club IDs to our internal tracking array, based on the loaded data.
	m_Clubs.RemoveAll();

	m_Clubs.SetSize(m_initializationData.m_NumberOfClubs, ClubID());
	for (int i = 0; i < m_initializationData.m_NumberOfClubs; i++)
	{
		ClubID cClubID(m_initializationData.m_FirstClubNumberInDiv + i);
		m_Clubs.SetAt(i, cClubID);
	}
	int Count = 0;
	// How many normal games per season are played?
	for (int LoopCount = 0; LoopCount < TOTALMATCHES; LoopCount++)
	{
		Count += (static_cast<eDivisionMatchType>(GetMatchType(LoopCount)) < DIVNOMATCH);
	}
	m_MatchData.DoCreate(NumberMatchClubs() * Count);
    m_MatchesPlayedThisMonth = 0;
	m_NumberMatchesPlayedThisSeason = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::LeaguePositionStatus
	Access:    	public
	Parameter: 	const byte iLeaguePositon
	Returns:   	eLeaguePositionStatus
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eLeaguePositionStatus CDivision::LeaguePositionStatus(const byte iLeaguePositon)
{
	eLeaguePositionStatus Color = NormalColor;

	if ((GetNumberClubsRelegatedIncludingPlayOffs() > -1) && (iLeaguePositon >= (GetNumberOfClubsInDivision() - GetNumberClubsRelegatedIncludingPlayOffs())))
	{
		Color = RelegatedColor;
	}
	else if (GetNumberClubsBottomPlayOff () > 0 && (iLeaguePositon >= (GetNumberOfClubsInDivision() - (m_NumberClubsRelegated + GetNumberClubsBottomPlayOff ()))))
	{
		Color = PlayOffColor;
	}

	if (iLeaguePositon < m_NumberClubsPromoted)
	{
		Color = PromotedColor;
	}
	else if (GetNumberOfClubsTopPlayOff () > 0 && iLeaguePositon < (m_NumberClubsPromoted + GetNumberOfClubsTopPlayOff ()))
	{
		Color = PlayOffColor;
	}
	return Color;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberClubsRelegatedIncludingPlayOffs
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetNumberClubsRelegatedIncludingPlayOffs()
{
	return m_NumberClubsRelegated + GetCountry().GetActiveCountry().HowManyPromotedToDivisonFromPlayoffs(this);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetMatchType
	Access:    	public
	Parameter: 	const byte _MatchDayNumber
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetMatchType(const byte _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	return m_MatchTypeCalendar[_MatchDayNumber];
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::TodaysFixtureListStart
	Access:    	public
	Parameter: 	const byte _TheDay
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::TodaysFixtureListStart(const byte _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	int iRet = NOTFOUND;
	if (IsCompetitiveMatch(_MatchDayNumber) == true)
	{
		iRet = static_cast<int> (GetMatchType(_MatchDayNumber)) * NumberMatchClubs();
		// Check that are within range(may play each other more than twice)
		while (iRet > WorldData().GetFixtureController().GetFixtureList(GetNumberOfClubsInDivision()).GetUpperBound())
		{
			iRet -= WorldData().GetFixtureController().GetFixtureList(GetNumberOfClubsInDivision()).GetUpperBound() - 1;
		}
	}
	else
	{
		if (IsPlayOffMatchDay(_MatchDayNumber) || IsFriendlyMatchDay(_MatchDayNumber) == true)
		{
			iRet = -1;
		}
		switch(GetMatchType(_MatchDayNumber))
		{
  			case DIVNOMATCH:
			case DIVENDSEASON:
			case DIVNEWSEASON:
			case DIVCLOSESEASON:
			case DIVTRAININGONLY:
			case DIVPLAYOFFINIT:
			case DIVARRANGEFRIENDLIES:
				iRet = -1;
				break;

			default :
				EXT_ASSERT(false);
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsFriendlyMatchDay
	Access:    	public
	Parameter: 	const byte _TheDay
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsFriendlyMatchDay(const byte _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	bool bRet = false;
	switch(GetMatchType(_MatchDayNumber))
	{
		case DIVFRIENDLYMATCH1:
		case DIVFRIENDLYMATCH2:
		case DIVFRIENDLYMATCH3:
		case DIVFRIENDLYMATCH4:
		case DIVFRIENDLYMATCH5:
		case DIVFRIENDLYMATCH6:
		case DIVFRIENDLYMATCH7:
		case DIVFRIENDLYMATCH8:
			bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsPlayOffMatchDay
	Access:    	public
	Parameter: 	const byte _MatchDayNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsPlayOffMatchDay(const byte _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	bool bRet = false;
	switch(GetMatchType(_MatchDayNumber))
	{
		case DIVPLAYOFFMATCH1:
		case DIVPLAYOFFMATCH2:
		case DIVPLAYOFFMATCH3:
		case DIVPLAYOFFMATCH4:
		case DIVPLAYOFFMATCH5:
		case DIVPLAYOFFMATCH6:
		case DIVPLAYOFFMATCH7:
		case DIVPLAYOFFMATCH8:
		case DIVPLAYOFFMATCH9:
		case DIVPLAYOFFMATCH10:
		case DIVPLAYOFFMATCH11:
		case DIVPLAYOFFMATCH12:
		case DIVPLAYOFFMATCH13:
		case DIVPLAYOFFMATCH14:
		case DIVPLAYOFFMATCH15:
		case DIVPLAYOFFMATCH16:
		case DIVPLAYOFFMATCH17:
		case DIVPLAYOFFMATCH18:
		case DIVPLAYOFFMATCH19:
		case DIVPLAYOFFMATCH20:
			bRet = true;
			break;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildLeagueFixtureList
	Access:    	public
	Parameter: 	const byte _MatchDayNumber
	Parameter: 	CSoccerResultList & _ResultList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildLeagueFixtureList(const byte _MatchDayNumber, CSoccerResultList& _ResultList)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	CCalendar TheDate(WorldData().GetCurrentDate());
	DoFindStartOfSeason(&TheDate);
	TheDate.SetMatchDay(_MatchDayNumber);
	DoBuildLeagueFixtureList(TheDate, _ResultList);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsCompetitiveMatch
	Access:    	public
	Parameter: 	const byte _MatchDay
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsCompetitiveMatch(const byte _MatchDay)
{
	EXT_ASSERT(_MatchDay < TOTALMATCHES);
	bool bRet = true;
	if (IsFriendlyMatchDay(_MatchDay) == true)
	{
		bRet = false;
	}
	else
	{
		eDivisionMatchType eMatchType = static_cast<eDivisionMatchType>(GetMatchType(_MatchDay));
		switch (eMatchType)
		{
  			case DIVNOMATCH:
			case DIVENDSEASON:
			case DIVNEWSEASON:
			case DIVCLOSESEASON:
			case DIVTRAININGONLY:
			case DIVPLAYOFFINIT:
			case DIVARRANGEFRIENDLIES:
				// No matches today
				bRet = false;
				break;

			default:
				bRet = true;	// Normal league match today
				break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildPlayOffFixtureList
	Access:    	public
	Parameter: 	const byte _MatchDay
	Parameter: 	CSoccerResultList & _ResultList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildPlayOffFixtureList(const byte _MatchDay, CSoccerResultList& _ResultList)
{
	eDivisionMatchType eMatchType = static_cast<eDivisionMatchType>(GetMatchType(_MatchDay));
	if (m_TopPlayOff.IsInUse() == true)
	{
		GetTopPlayOff()->DoBuildFixtureList(eMatchType, _ResultList);
	}
	if (m_BottomPlayOff.IsInUse() == true)
	{
		GetBottomPlayOff()->DoBuildFixtureList(eMatchType, _ResultList);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildLeagueFixtureList
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Parameter: 	CSoccerResultList & _ResultList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildLeagueFixtureList(CCalendar& _TheDate, CSoccerResultList& _ResultList)
{
	EXT_ASSERT(_ResultList.GetSize() == 0);	// Ensure list is empty
	if (IsCompetitiveMatch(_TheDate.GetMatchDay()) == false)
	{
		return;
	}
	if (IsPlayOffMatchDay(_TheDate.GetMatchDay()) == true)
	{
		DoBuildPlayOffFixtureList(_TheDate.GetMatchDay(), _ResultList);
	}
	else
	{
		int FixtureListStart = TodaysFixtureListStart(_TheDate.GetMatchDay());
		WorldData().GetMatchHandler().SetCompetitionInfo(this);
		CNumericArray<byte>& FixtureList = WorldData().GetFixtureController().GetFixtureList(GetNumberOfClubsInDivision());
		CSoccerResult* theResult;
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision() - 1; LoopCount += 2)
		{
			int ad = (int)FixtureList[FixtureListStart++] - 1;
			int ae = (int)FixtureList[FixtureListStart++] - 1;
			theResult = FD_ENGINE_NEW CSoccerResult(GetClubs()[ad], GetClubs()[ae]);
			_ResultList.Add(theResult);

			// If match has been played then get result
			if (_TheDate.GetDate() < WorldData().GetCurrentDate().GetDate())
			{
				ushort matchnum = GetLeagueMatchResult(_TheDate.GetMatchDay(), theResult->GetHomeClubDetails().GetClubID(), theResult->GetAwayClubDetails().GetClubID());
				if (matchnum != wNOTFOUND)
				{
          // if match attendance is 0 we've likely reset
          if( m_MatchData[matchnum].GetAttendance() > 0 )
          {
            theResult->SetResult(&m_MatchData[matchnum]);
          }
				}
			}
		}
	}
    // Sort into home club alphabetical order
    _ResultList.SortByHomeClub();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::SeekClubFixtureForMatchNumber
	Access:    	public
	Parameter: 	const ushort _ClubID
	Parameter: 	const byte _MatchNumber
	Parameter: 	CSoccerResult & _SoccerResult
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::SeekClubFixtureForMatchNumber(const ClubID _ClubID, const byte _MatchNumber, CSoccerResult& _SoccerResult)
{
	CCalendar TheDate(WorldData().GetCurrentDate());

	DoFindStartOfSeason(&TheDate);
	TheDate.SetMatchDay(_MatchNumber);
	if (IsCompetitiveMatch(_MatchNumber) == false)
	{
		return;
	}

	if (IsPlayOffMatchDay(TheDate.GetMatchDay()) == true)
	{
	//	DoBuildPlayOffFixtureList(_MatchNumber, _SoccerResult);
	}
	else
	{
		int FixtureListStart = TodaysFixtureListStart(TheDate.GetMatchDay());
		WorldData().GetMatchHandler().SetCompetitionInfo(this, NULL);
		CNumericArray<byte>& FixtureList = WorldData().GetFixtureController().GetFixtureList(GetNumberOfClubsInDivision());
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision() - 1; LoopCount += 2)
		{
			int ad = (int)FixtureList[FixtureListStart++] - 1;
			int ae = (int)FixtureList[FixtureListStart++] - 1;

			ClubID adID = GetClubs()[ad];
			ClubID aeID = GetClubs()[ae];
			if (adID == _ClubID || aeID == _ClubID)
			{
				_SoccerResult.DoAddClubIDs(adID, aeID);

				// If match has been played then get result
				if (TheDate.GetDate() < WorldData().GetCurrentDate().GetDate())
				{
					ushort matchnum = GetLeagueMatchResult(TheDate.GetMatchDay(), _SoccerResult.GetHomeClubDetails().GetClubID(), _SoccerResult.GetAwayClubDetails().GetClubID());
					_SoccerResult.SetResult(&m_MatchData[matchnum]);
				}
				break;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindFirstMatch
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoFindFirstMatch(CCalendar& _TheDate)
{
    while (GetMatchType(_TheDate.GetMatchDay()) >= DIVENDSEASON)
	{
        _TheDate.NextMatchDay();
    };
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindStartOfSeason
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoFindStartOfSeason(CCalendar& _TheDate)
{
	while (GetMatchType(_TheDate.GetMatchDay()) != DIVNEWSEASON)
	{
		// Has gone to previous game year and skipped week 0
		if (_TheDate.PreviousMatchDay() == true)
		{
			if (GetMatchType(0) == DIVNEWSEASON)
			{
				break;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindEndOfSeason
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoFindEndOfSeason(CCalendar& _TheDate)
{
	while (GetMatchType(_TheDate.GetMatchDay()) != DIVENDSEASON)
	{
		_TheDate.NextMatchDay();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindStartOfNextSeason
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoFindStartOfNextSeason(CCalendar& _TheDate)
{
	while (GetMatchType(_TheDate.GetMatchDay()) != DIVNEWSEASON)
	{
		// Has gone to previous game year and skipped week 0
		if (_TheDate.NextMatchDay() == true)
		{
			if (GetMatchType(0) == DIVNEWSEASON)
			{
				break;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindStartOfSeason
	Access:    	public
	Parameter: 	CCalendar * TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoFindStartOfSeason(CCalendar* TheDate)
{
    if (m_MatchData.GetSize() > 0)
	{
		// Matches played so move back to start of season
        DoFindStartOfSeason(*TheDate);
	}
	else
	{
		// No matches played yet so move forward
        DoFindFirstMatch(*TheDate);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetClubsInRegion
	Access:    	public
	Parameter: 	int _RegionToFind
	Parameter: 	CNumericArray<ushort> & _TheClubs
	Returns:   	void
	Qualifier:
	Purpose:	Adds all the clubs in this region to the list
------------------------------------------------------------------------------*/
void CDivision::GetClubsInRegion(int _RegionToFind, CNumericArray<ClubID>& _TheClubs)
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        if (cClub.GetRegionID() == _RegionToFind)
		{
			_TheClubs.Add(cClub.DoFindID());
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::WeeksToEndOfSeason
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:	How many weeks to the end of the season?
------------------------------------------------------------------------------*/
int CDivision::WeeksToEndOfSeason()
{
	CCalendar TheDate(WorldData().GetCurrentDate());
	int Count = 0;
	while (GetMatchType(TheDate.GetMatchDay()) != DIVENDSEASON && GetMatchType(TheDate.GetMatchDay()) != DIVCLOSESEASON )
	{
		TheDate.NextMatchDay();
		Count++;
	}
	return Count >> 2;		// Assumes 2 dates per week
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::HowLongTillNextLeagueMatch
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::HowLongTillNextLeagueMatch()
{
	int CurrentMatchDay = GetCurrentMatchDayNumber();
	int HowLongLeague = 0;
	while (GetMatchType(CurrentMatchDay++) == DIVNOMATCH)
	{
		HowLongLeague ++;
		// Wrap round if end of year
		if (CurrentMatchDay == TOTALMATCHES)
		{
			CurrentMatchDay = 0;
		}
	};
	return HowLongLeague;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCurrentMatchDayNumber
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::GetCurrentMatchDayNumber()
{
	return WorldData().GetCurrentDate().GetMatchDay();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindPlayerClub
	Access:    	public
	Parameter: 	ushort _PlayerID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::DoFindPlayerClub(ushort _PlayerID)
{
    EXT_ASSERT(_PlayerID != NOPLAYER);
    ushort iRet = NOCLUB;
	for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        if (cClub.GetFirstTeamSquad().IsPlayerInSquad(_PlayerID) == true)
		{
			iRet = LoopCount;
            break;
		};
	}
    EXT_ASSERT(iRet != NOCLUB);
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountClubsScoredMore
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::DoCountClubsScoredMore(CClub& _Club)
{
	int iRet = 0;
	int iTotal = _Club.GetTableData().GetNumberGoalsScored();
	for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cTestClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (&_Club != &cTestClub && iTotal < cTestClub.GetTableData().GetNumberGoalsScored())
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountClubsScoredLess
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::DoCountClubsScoredLess(CClub& _Club)
{
	int iRet = 0;
	int iTotal = _Club.GetTableData().GetNumberGoalsScored();
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cTestClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (&_Club != &cTestClub && iTotal > cTestClub.GetTableData().GetNumberGoalsScored())
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountClubsConcededMore
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::DoCountClubsConcededMore(CClub& _Club)
{
	int iRet = 0;
	int iTotal = _Club.GetTableData().GetNumberGoalsConceded();
	for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cTestClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if ((&_Club != &cTestClub) && (iTotal < cTestClub.GetTableData().GetNumberGoalsConceded()))
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountClubsConcededLess
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::DoCountClubsConcededLess(CClub& _Club)
{
	int iRet = 0;
	int iTotal = _Club.GetTableData().GetNumberGoalsConceded();
	for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cTestClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (&_Club != &cTestClub && iTotal > cTestClub.GetTableData().GetNumberGoalsConceded())
		{
			iRet++;
		}
	}
	return iRet;
}



/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountTransferListedPlayers
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::DoCountTransferListedPlayers()
{
	ushort iRet = 0;
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        iRet += cClub.GetFirstTeamSquad().DoCountListedPlayers();
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetLeadingScorerGoals
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::GetLeadingScorerGoals()
{
	int iGoalsFor = 0;
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		CPlayer* pPlayer = cClub.GetFirstTeamSquad().GetLeadingScorer();
    	if (pPlayer != NULL)
    	{
	      	if (pPlayer->GetNumberGoalsThisSeason() > iGoalsFor)
      		{
        		iGoalsFor = pPlayer->GetNumberGoalsThisSeason();
      		}
    	}
	}
	return iGoalsFor;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoCountGoodScorersForClub
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::DoCountGoodScorersForClub(CClub& _Club)
{
	int iGoalsFor = GetLeadingScorerGoals();
	return _Club.GetFirstTeamSquad().CountPlayersScoredAtLeast(max(3, iGoalsFor - 4));
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::CountGoodFoulersForClub
	Access:    	public
	Parameter: 	CClub * pClub
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::CountGoodFoulersForClub(CClub* pClub)
{
	int iGoalsFor = GetLeadingFoulerPoints();
	return pClub->GetFirstTeamSquad().CountPlayersFouledAtLeast(max(10, iGoalsFor - 9));
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetLeadingFoulerPoints
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::GetLeadingFoulerPoints()
{
	int iGoalsFor = 0;

    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		CPlayer* pPlayer = cClub.GetFirstTeamSquad().GetLeadingFouler();
    	if (pPlayer != NULL)
    	{
      		if (pPlayer->GetDiscipline().GetDisciplinaryPoints() > iGoalsFor)
      		{
        		iGoalsFor = pPlayer->GetDiscipline().GetDisciplinaryPoints();
      		}
    	}
	}
	return iGoalsFor;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::NumberMatchClubs
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::NumberMatchClubs()
{
	int NumClubs = GetNumberOfClubsInDivision();
	// Check if odd number of clubs
	if ((GetNumberOfClubsInDivision() >> 1) * 2 != GetNumberOfClubsInDivision())
	{
		// Yes so reduce the daily fixture list size by 1
		NumClubs--;
	}
	return NumClubs;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::BuildSortedAlphaClubList
	Access:    	public
	Parameter: 	CSortedStringList & _ListSortedClubs
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CDivision::BuildSortedAlphaClubList(CSortedStringList& _ListSortedClubs)
{
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		ClubID cClubID = GetClubs()[LoopCount];
		CClub &cClub = WorldData().GetClubByID(cClubID);
		_ListSortedClubs.Add(cClub.GetName(), cClubID.id);
	}
	//_ListSortedClubs.SortAscending();
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindClubIDInTablePosition
	Access:    	public
	Parameter: 	ushort _Position
	Returns:   	ushort - the club number in nn position in Division
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CDivision::DoFindClubIDInTablePosition(ushort _Position)
{
	EXT_ASSERT(_Position > -1 && _Position < GetNumberOfClubsInDivision());
	ClubID iRet;
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        if (cClub.GetTableData().GetLeaguePosition() == _Position)
		{
			iRet = cClub.DoFindID();
            break;
		}
    }
#ifdef _DEBUGv3
	if (iRet == NOCLUB)
	{
		for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
		{
			CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
			g_DebugStr.Format(_T("%d | %s | %d"), cClub.GetTableData().GetLeaguePosition(), cClub.GetName(), cClub.GetTableData().GetPoints());
			g_DebugStr.OutputDebug();
		}
	}
#endif
	EXT_ASSERT(iRet.id != NOCLUB);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindClubInTablePosition
	Access:    	public
	Parameter: 	ushort _Position
	Returns:   	CClub& - the club number in nn position in Division
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CDivision::DoFindClubInTablePosition(ushort _Position)
{
	EXT_ASSERT(_Position < GetNumberOfClubsInDivision());
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
// 		DEBUG_OUTPUT(("Club Posn %d"), (cClub.GetTableData().GetLeaguePosition());
// 		g_DebugStr.OutputDebug();

        if (cClub.GetTableData().GetLeaguePosition() == _Position)
			return cClub;
    }

	EXT_ASSERT(false && "This shouldn't happen -- no club was found with the specified league position");
	return WorldData().GetClubByID(GetClubs()[0]);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FindClubNumberInCurrentFormPosition
	Access:    	public
	Parameter: 	const ushort _FormPosition
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CDivision::FindClubNumberInCurrentFormPosition(const ushort _FormPosition)
{
//  	g_DebugStr.Format(_T("Seeking Posn %d"), _FormPosition);
//  	g_DebugStr.OutputDebug();
	ushort LoopCount;
    for (LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        if (cClub.GetCurrentForm()->GetCurrentFormPosn() == _FormPosition)
		{
//  			g_DebugStr.Format(_T("Club %s, Posn %d\n"), cClub.GetName(), cClub.GetCurrentForm()->GetCurrentFormPosn());
//  			g_DebugStr.OutputDebug();
			return cClub;
		}
    }
	EXT_ASSERT(false && "Shouldn't happen -- could not find club with the specified form position");
	return WorldData().GetClubByID(GetClubs()[0]);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindClubIDBottomOfTable
	Access:    	public
	Returns:   	ushort - the club number in bottom position in Division
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ClubID CDivision::DoFindClubIDBottomOfTable()
{
	return DoFindClubIDInTablePosition(GetNumberOfClubsInDivision() - 1);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindRandomNonUserClub
	Access:    	public
	Parameter: 	CClub * _ClubNotToFind
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CDivision::DoFindRandomNonUserClub(CClub* _ClubNotToFind)
{
    CClub* RandomClub = NULL;
	do
	{
		RandomClub = &WorldData().GetClubByID(GetClubs()[RandomNumber.IntLessThan(GetNumberOfClubsInDivision())]);
	}
	while (RandomClub == _ClubNotToFind || RandomClub->IsUserControlled() == true);
	return RandomClub;
}


 // Return a pointer to a club in this division that may be interested in buying this player
 // -------------------------------------------------------------------------------------------------------------------------
CClub* CDivision::DoFindRandomNonUserBuyingClub(CClub* _ClubNotToFind, CPlayer* _Player, const uint _BidAmount)
{
    CClub* RandomClub = NULL;
	for (int LoopCount = 0; LoopCount < SELLATTEMPTS * 3; LoopCount++)
	{
		RandomClub = DoFindRandomNonUserClub(_ClubNotToFind);
		if (RandomClub->InterestedInPlayer(_Player, _BidAmount) == true)
		{
			break;
		}
		RandomClub = NULL;
	}
	return RandomClub;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoFindUserClub
	Access:    	public
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CDivision::DoFindUserClub()
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (cClub.IsUserControlled() == true)
		{
			return &cClub;
		}
	}
	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoEndOfWeek
	Access:    	public
	Parameter: 	bool _NoWarnings
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoEndOfWeekTasks(bool _NoWarnings)
{
	if (GetCountryRanking() < 2)
	{
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
    	{
#ifdef NDS
      		if ((LoopCount & 8) == 0)
      		{
        		ProcessingCallback( "DoEndOfWeekTasks - Club loop" );
      		}
#endif
			CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
			cClub.DoEndOfWeekTasks(_NoWarnings);
		}

		// List a few players to replace those either bought or taken off the list
		if (DoCountTransferListedPlayers() < MINTRANSFERLISTEDPLAYERS || RandomNumber.IntLessThan(2) == 1)
		{
			DoMidWeekTasks();
		}
	}
#ifdef NDS
  ProcessingCallback( "DoEndOfWeekTasks1" );

	// FDL - ELN - Check for matches that occurred longer than two weeks ago, and convert them to summaries.
	CCalendar cToday = WorldData().GetCurrentDate();
	int iNumberOfWeeksToKeep = 1;

	// If there is a user team in the division, keep 2 weeks of stats.
	if (DoFindUserClub() != NULL)
	{
		iNumberOfWeeksToKeep++;
	}

	CCalendar cCutoffDate = cToday;
	for (int i = 0; i < iNumberOfWeeksToKeep; i++)
	{
		cCutoffDate.SubAWeek();
	}

	for (int i = 0; i < m_MatchData.GetSize(); i++)
	{
		CFixture *pFixture = &m_MatchData[i];
		CDate cMatchDate = pFixture->GetMatchDate();

    // keep all user matches
    if ((pFixture->IsSummary() == false) && (pFixture->IsUserMatch() == false) && (cMatchDate.GetDate() < cCutoffDate.GetDate()))
		{
			pFixture->ConvertMatchEventsToSummary();
		}
	}
  	ProcessingCallback( "DoEndOfWeekTasks2" );
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoDailyTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoDailyTasks()
{
	if (GetCountryRanking() < 2)
	{
		m_HowLongTillNextLeagueMatch = HowLongTillNextLeagueMatch();
		m_WeeksToEndOfSeason = WeeksToEndOfSeason();
		bool bUserClub = false;
		if (DoFindUserClub() != NULL)
		{
			bUserClub = true;
		}
		CBaseCompetition::DoDailyTasks(GetCountry().GetActiveCountry(), bUserClub);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoMidWeekTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoMidWeekTasks()
{
	if (GetCountryRanking() < 2)
	{
		CClub* pClub;
		uint NumberTransferListAttempts = MINTRANSFERLISTATTEMPTS + RandomNumber.IntLessThan(RNDTRANSFERLISTATTEMPTS);
		// List a few players to replace those either bought or taken off the list
		for (uint Attempt = 0; Attempt < 100; Attempt++)
		{
			if (DoCountTransferListedPlayers() >= MINTRANSFERLISTEDPLAYERS)
			{
				break;
			}
			for (uint LoopCount = 0; LoopCount < NumberTransferListAttempts; LoopCount++)
			{
				pClub = DoFindRandomNonUserClub(NULL);
				if (pClub != NULL)
				{
					pClub->DoTransferListRandomPlayers(1);
				}
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoAddPlayOffQualifiers
	Access:    	public
	Parameter: 	int PlayOffID
	Parameter: 	CNumericArray<ushort> & _DrawClubs
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoAddPlayOffQualifiers(int PlayOffID, CNumericArray<ClubID>& _DrawClubs)
{
	ClubID _ClubID;

	if (m_TopPlayOff.GetID() == PlayOffID)
	{
		for (ushort LoopCount = m_NumberClubsPromoted; LoopCount < m_NumberClubsPromoted + GetNumberOfClubsTopPlayOff (); LoopCount++)
		{
			_ClubID = DoFindClubIDInTablePosition(LoopCount);
			_DrawClubs.Add(_ClubID);

			CClub &cClub = WorldData().GetClubByID(_ClubID);
			cClub.DoQualifiedTopPlayOff();
		}
	}
	if (m_BottomPlayOff.GetID() == PlayOffID)
	{
		ushort FirstToAdd = GetNumberOfClubsInDivision() - m_NumberClubsRelegated - GetNumberClubsBottomPlayOff();
		for (ushort LoopCount = 0; LoopCount < GetNumberClubsBottomPlayOff(); LoopCount++)
		{
			_ClubID = DoFindClubIDInTablePosition(FirstToAdd++);
			_DrawClubs.Add(_ClubID);

			CClub &cClub = WorldData().GetClubByID(_ClubID);
			cClub.DoQualifiedBottomPlayOff();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	CBaseCompetition::Read(_pDataFile);
 	_pDataFile->Read(&m_MatchesPlayedThisMonth, sizeof(m_MatchesPlayedThisMonth));                  // count of how many groups of matches played this month
  	_pDataFile->Read(&m_NumberMatchesPlayedThisSeason, sizeof(m_NumberMatchesPlayedThisSeason));
    m_MatchData.Read(_pDataFile);
	for (int LoopCount = 0; LoopCount < 12; LoopCount++)
	{
    m_MOMWinners[LoopCount].Read(_pDataFile);
	}
#ifdef TEAMOFWEEKONMENU
	m_TeamOfTheWeek.Read(_pDataFile);
#endif
 }


 /*------------------------------------------------------------------------------
 	Method:   	CDivision::Write
 	Access:    	public
 	Parameter: 	CDataFile * _pDataFile
 	Returns:   	void
 	Qualifier:
 	Purpose:
 ------------------------------------------------------------------------------*/
 void CDivision::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	CBaseCompetition::Write(_pDataFile);
  	_pDataFile->Write(&m_MatchesPlayedThisMonth, sizeof(m_MatchesPlayedThisMonth));                  // count of how many groups of matches played this month
  	_pDataFile->Write(&m_NumberMatchesPlayedThisSeason, sizeof(m_NumberMatchesPlayedThisSeason));
  	m_MatchData.Write(_pDataFile);
  	for (int LoopCount = 0; LoopCount < 12; LoopCount++)
	{
    	m_MOMWinners[LoopCount].Write(_pDataFile);
	}
#ifdef TEAMOFWEEKONMENU
	m_TeamOfTheWeek.Write(_pDataFile);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildManagerOfMonth
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Select Manager of the month, decided by points then goals scored, if tied
				then 50/50 chance of winning
				Will not process unless 3 matches have been played
------------------------------------------------------------------------------*/
 void CDivision::DoBuildManagerOfMonth()
{
	if (m_MatchesPlayedThisMonth > 1)
	{
		m_MatchesPlayedThisMonth = 0;
		CClub* pMomClub = NULL;
		ushort momGoals = 0;         // goals scored
		ushort momPoints = 0;         // points won
		CManager* pMomManager = NULL;

    	// Check all clubs in division
		for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
		{
			CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
      		if (cClub.GetManagerID() != NOMANAGER )
			{
				//  Get pointer to the manager
                CManager *CurrentManager = &WorldData().GetManager(cClub.GetManagerID());

        		// Is current manager the best so far?
     			if (CurrentManager->GetPointsSinceMOM () > momPoints || CurrentManager->GetPointsSinceMOM () == momPoints && \
          	  		CurrentManager->GoalsSinceMOM().GetScored() > momGoals)
				{
					pMomManager = CurrentManager;
					pMomClub = &cClub;
        	  		momPoints = CurrentManager->GetPointsSinceMOM ();
          			momGoals = CurrentManager->GoalsSinceMOM().GetScored();
    			}
          		CurrentManager->DoClearMOM();
    		}
    	}
        if ((pMomManager != NULL) && (pMomClub != NULL))
		{
			int FirstFreeMOMWinners = 0;
			while (m_MOMWinners[FirstFreeMOMWinners].IsEmpty() == false)
			{
				FirstFreeMOMWinners++;
			}
			m_MOMWinners[FirstFreeMOMWinners].DoCreate(pMomClub, WorldData().GetCurrentDate().LastMonthStr());
            //m_NewsEvents.SetManagerOfTheMonthNumber(pMomClub->GetManager());
			pMomClub->WonMOM(WorldData().GetCurrentDate().LastMonthStr(), CBaseCompetition::GetName());

			CExtString str;
          str.Format(IDS_MANOFMONTHANNOUNCE2, CBaseCompetition::GetName(),
					WorldData().GetCurrentDate().LastMonthStr(), pMomClub->GetManagerName(), CNewsItem::PreprocessClubName(pMomClub).CStr());
			GetCountry().GetActiveCountry().AddToNewsLists(str);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoInitialiseFixture
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Parameter: 	CMatchInfo _Match
	Returns:   	CFixture&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture& CDivision::DoInitialiseFixture(const ClubID _HomeClubID, const ClubID _AwayClubID, CMatchInfo _Match)
{
	CFixture& theFixture = m_MatchData[m_NumberMatchesPlayedThisSeason++];
	_Match.DoInitialiseLeagueMatch(_HomeClubID, _AwayClubID, *this, &theFixture);
	theFixture.OnInitialise(_Match);
	WorldData().GetMatchHandler().SetCompetitionInfo(this, NULL);
	return theFixture;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsCloseSeasonToday
	Access:    	public
	Parameter: 	const int _DayNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsCloseSeasonToday(const int _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	if (GetMatchType(_MatchDayNumber) == DIVCLOSESEASON)
	{
		return true;
	}
	return false;
}

/*------------------------------------------------------------------------------
  Method:     CDivision::IsEndSeasonToday
	Access:    	public
	Parameter: 	const int _DayNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsEndSeasonToday(const int _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
  if (GetMatchType(_MatchDayNumber) == DIVENDSEASON)
	{
		return true;
	}
	return false;
}

 // Returns a list of matches to be played between now and end of calendar month for specified club
 //---------------------------------------------------------------------------------------
void CDivision::ClubMatchListForCurrentMonth(const ClubID iClubID, CPointerArray<CMatchInfo> & MatchesThisMonth)
{
    CCalendar TheDate(WorldData().GetCurrentDate());
	int MatchType;
	ClubID HomeClubID, AwayClubID;
	CSoccerResultList cSoccerResultList;

	while (TheDate.GetMonth() == WorldData().GetCurrentDate().GetMonth())
	{
		MatchType = GetMatchType(TheDate.GetMatchDay());
		if (IsCompetitiveMatch(MatchType) == true)
		{
			DoBuildLeagueFixtureList(TheDate.GetMatchDay(), cSoccerResultList);
			for (int LoopCount = 0; LoopCount < cSoccerResultList.GetSize(); LoopCount ++)
			{
				HomeClubID = cSoccerResultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID();
				AwayClubID = cSoccerResultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID();

				// If this club is involved in the match
				if (HomeClubID == iClubID || AwayClubID == iClubID)
				{
					MatchesThisMonth.Add(FD_ENGINE_NEW CMatchInfo(HomeClubID, AwayClubID));
					break;
				}
			}
		}
		cSoccerResultList.RemoveList();
		TheDate++;
	};
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetNumberGamesPerSeason
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::GetNumberGamesPerSeason()
{
	int tot = 0;
	// How many games per season are played?
	for (int LoopCount = 0; LoopCount < TOTALMATCHES; LoopCount++)
	{
		tot += (IsCompetitiveMatch(LoopCount) == true);
	}
	EXT_ASSERT(tot > 0);
	return tot;
}


 // Check if there is any matches this week and play them
 //---------------------------------------------------------------------------------------
bool CDivision::DoPlayLeagueMatches(const bool _ShowSummary)
{
    //@@m_NewsEvents.MakeNull();           // Clearout last turn's newspaper stuff
	m_TeamOfTheWeek.PreMatchClearDown();

	if (GetMatchType(GetCurrentMatchDayNumber()) == DIVNOMATCH || IsPlayOffMatchDay(GetCurrentMatchDayNumber()) == true)
	{
		return false;
	}
	if (IsFriendlyMatchDay(GetCurrentMatchDayNumber()) == true)
	{
		return DoPlayFriendlies(_ShowSummary);
	}

	if (IsCompetitiveMatch(GetCurrentMatchDayNumber()) == true)
	{
#ifdef	QUICKRESULTS
		if (GetCountryRanking() == 2)
		{
			QuickResults(GetCurrentMatchDayNumber());
			return true;
		}
#endif
		CSoccerResultList cSoccerResultList;
		CFixtureList ViewMatches;
		CFixtureList TheMatches;
		CMatchInfo Match;

		m_MatchesPlayedThisMonth ++;
		DoBuildLeagueFixtureList(GetCurrentMatchDayNumber(), cSoccerResultList);
		int LoopCount;
		for (LoopCount = 0; LoopCount < cSoccerResultList.GetSize(); LoopCount ++)
		{
			// Play the match if not been postponed
            CFixture& Fixture = DoInitialiseFixture(cSoccerResultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID(),
										   cSoccerResultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID(), Match);
			WorldData().GetMatchHandler().GetMatchPlay().DoInitialiseIntro(&Fixture, this);
            if (Fixture.IsUserMatch() == true && theApp.DisplayMatchIntro(_ShowSummary) == true)
			{
				// Add match to matches view list
				ViewMatches.Add(&Fixture);
			}
			else
			{
				// Calculate the match result
				Fixture.DoPlayMatch();
				Fixture.DoShowSummary(_ShowSummary, NULL);
#ifdef NDS
        		if (Fixture.IsUserMatch())
	        	{
          			lLogFmt("Division Match Over\n");
          			theApp.OnHighlightMenuIcon(HIGHLIGHT_FIXTURES);
        		}
#endif
			}
			TheMatches.Add(&Fixture);
		}

		// Play any user viewed matches
		for (LoopCount = 0; LoopCount < ViewMatches.GetSize(); LoopCount++)
		{
			WorldData().GetMatchHandler().DoDivisionMatch(&ViewMatches[LoopCount], this);
//			DoAfterMatchUpdate(ViewMatches[LoopCount]);
		}
		for (LoopCount = 0; LoopCount < cSoccerResultList.GetSize(); LoopCount ++)
		{
			DoAfterMatchUpdate(*TheMatches.GetAt(LoopCount));
		}
		SortLeagueTable();
		cSoccerResultList.RemoveList();
		return true;			// Create 'Team of the week' for the newspaper
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoAfterMatchUpdate
	Access:    	public
	Parameter: 	CFixture & _Fixture
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoAfterMatchUpdate(CFixture& _Fixture)
{
    // Update league tables
	uint PointsForAWin = m_PointsForWin;
	bool HomeWinPenShootout = false;
	switch(m_PointsForWin)
	{
		case JLEAGUEPOINTS:
			PointsForAWin = 3;
			if (_Fixture.GetExtraTimeFlag() == true)
			{
				PointsForAWin = 2;
			}
			if (_Fixture.GetEventList()->WasPenaltyShootOut() == true)
			{
				PointsForAWin = 1;
				if (_Fixture.GetEventList()->PenaltyShootoutWinner() == HOME)
				{
					HomeWinPenShootout = true;
				}
			}
			break;

		case MLSPOINTS:
			PointsForAWin = 3;
			if (_Fixture.GetEventList()->WasPenaltyShootOut() == true)
			{
				PointsForAWin = 1;
				if (_Fixture.GetEventList()->PenaltyShootoutWinner() == HOME)
				{
					HomeWinPenShootout = true;
				}
			}
			break;
	}

    CClub& HomeClub = _Fixture.GetClub(HOME);
    CClub& AwayClub = _Fixture.GetClub(AWAY);
    HomeClub.DoHomeUpdateTables(_Fixture, PointsForAWin, HomeWinPenShootout);
    AwayClub.DoAwayUpdateTables(_Fixture, PointsForAWin, HomeWinPenShootout);

	ushort strVenueID = WorldData().GetText().DoFindStringID(HomeClub.GetStadium()->GetName());
    HomeClub.CheckHomeRecords(_Fixture, CBaseCompetition::m_TitleID, strVenueID);
    AwayClub.CheckAwayRecords(_Fixture, CBaseCompetition::m_TitleID, strVenueID);

    // See if worthy of newspaper headlines
    //@@int MatchDay = m_MatchData.FindID(pFixture);
    //@@m_NewsEvents.CheckIfNewsworthyResult(MatchDay, &m_MatchData);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetPointsForDraw
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetPointsForDraw()
{
	if (m_PointsForWin == JLEAGUEPOINTS)
	{
		return 0;
	}
	return 1;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoNewSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoNewSeason()
{
  lLogFmt("DoNewSeason\n");
  //lBreak();

    m_MatchesPlayedThisMonth = 0;
	m_NumberMatchesPlayedThisSeason = 0;
	m_MatchData.DoNewSeason();
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        cClub.DoNewSeason(LoopCount);
	}
	DoClearMOM();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoClearMOM
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoClearMOM()
{
	for (int LoopCount = 0; LoopCount < 12; LoopCount++)
	{
		m_MOMWinners[LoopCount].DoInitialise();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsTopDivision
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsTopDivision()
{
	return GetCountry().GetActiveCountry().IsTopDivision(this);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::IsStartOfSeason
	Access:    	public
	Parameter: 	CCalendar * _TheDate
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::IsStartOfSeason(CCalendar* _TheDate)
{
	if (GetCurrentMatchDayNumber() == 0)
	{
		// If first season
		if (PlayedAGame() == false && _TheDate->GetMatchDay() == 0)
		{
			// At start of game
			return true;
		}
		else
		{
			return false;
		}
	}
	CCalendar tDate;
	tDate.DoInitialise(*_TheDate);
	DoFindStartOfSeason(tDate);
	if (tDate.GetMatchDay() == _TheDate->GetMatchDay())
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::PlayedAGame
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::PlayedAGame()
{
	if (m_MatchData.GetSize() == 0)
	{
		return false;
	}

	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoEndOfSeason
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CDivision::DoEndOfSeason()
{
    bool HumanManagerFound = false;
	int count = 0;
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        count += (cClub.GetTableData().GetLeaguePosition() == 0);
		if (count == 2)
		{
			EXT_ASSERT(false);
		}
	}

	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		// Check if it's a human manager(if so will need to display EoS summary at end of country processing)
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);

        if (cClub.IsUserControlled() == true)
		{
			HumanManagerFound = true;
		}
        cClub.DoEndOfSeason();
	}

    // Wipe fixture data
    lLogFmt("DoEndOfSeason\n");
    m_MatchData.DoNewSeason();

	return HumanManagerFound;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoClearTables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoClearTables()
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		cClub.GetTableData().SetLeaguePosition(LoopCount);
		cClub.DoClearTable();
	}
	DoClearMOM();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetLeagResult
	Access:    	public
	Parameter: 	const byte _MatchDayNumber
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetLeagueMatchResult(const byte _MatchDayNumber, const ClubID _HomeClubID, const ClubID _AwayClubID)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	ushort iRet = wNOTFOUND;
	byte MatchNumber = GetMatchType(_MatchDayNumber);
    if (MatchNumber < DIVNOMATCH)
	{
    	iRet = MatchNumber * (NumberMatchClubs() / 2);
    	EXT_ASSERT(iRet < m_MatchData.GetSize());

		bool bFound = false;
    	for (uint LoopCount = 0; LoopCount < NumberMatchClubs(); LoopCount += 2)
		{
			// If this club is involved in the match
      		if ((m_MatchData[iRet].GetHomeClubID() == _HomeClubID) && (m_MatchData[iRet].GetAwayClubID() == _AwayClubID))
			{
				bFound = true;
        		break;
      		}
			iRet++;
    	}

		if (bFound == false)
		{
			iRet = wNOTFOUND;
		}
	}
  	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetFixture
	Access:    	public
	Parameter: 	const ushort _Num
	Returns:   	CFixture&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture& CDivision::GetFixture(const ushort _Num)
{
	EXT_ASSERT(_Num < m_MatchData.GetSize());
	return m_MatchData[_Num];
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::NextMatchDateThisSeason
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::NextMatchDateThisSeason(CCalendar& _TheDate)
{
	if (GetMatchType(_TheDate.GetMatchDay()) != DIVENDSEASON)
	{
		_TheDate.NextMatchDay();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::PrevMatchDateThisSeason
	Access:    	public
	Parameter: 	CCalendar & _TheDate
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::PrevMatchDateThisSeason(CCalendar& _TheDate)
{
	if (GetCurrentMatchDayNumber() == 0 && PlayedAGame() == false)
	{
		// If first season
		if (_TheDate.GetMatchDay() > 0)
		{
			// Not at start of game
			_TheDate.PreviousMatchDay();
		}
		return;
	}
	else if (GetMatchType(_TheDate.GetMatchDay()) != DIVNEWSEASON)
	{
		_TheDate.PreviousMatchDay();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::SortLeagueTable
	Access:    	public
	Parameter: 	int iMode
	Returns:   	void
	Qualifier:
	Purpose:	Sort the league table into order
------------------------------------------------------------------------------*/
void CDivision::SortLeagueTable(int iMode)
{
    CClubList LeagueTable;
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		LeagueTable.Add(&cClub);
	}
	LeagueTable.SortTableData(iMode);

	// Update the league position list
	CClub &cFirstClub = WorldData().GetClubByID(GetClubs()[0]);
	if (cFirstClub.GetTableData().GetNumberGamesPlayed() > 0)
    {
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
		{
			CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
			cClub.SetThisWeeksLeaguePosition();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Decreasing order sort for setting form position, with the exception
// of the tiebreaker of club name, which sorts in ascending order.
//////////////////////////////////////////////////////////////////////////
int StLeagueTableSortFunc(const void *v1, const void *v2)
{
	StLeagueTable *s1 = (StLeagueTable *)v1;
	StLeagueTable *s2 = (StLeagueTable *)v2;

	if (s1->Points > s2->Points)
		return -1;
	if (s1->Points < s2->Points)
		return 1;

	if (s1->GoalsDiff > s2->GoalsDiff)
		return -1;
	if (s1->GoalsDiff < s2->GoalsDiff)
		return 1;

	if (s1->GoalFor > s2->GoalFor)
		return -1;
	if (s1->GoalFor < s2->GoalFor)
		return 1;

	const int iStricmpValue = ::stricmp(WorldData().GetClubByID(s1->TheClub).GetName().CStr(), WorldData().GetClubByID(s2->TheClub).GetName().CStr());
	return iStricmpValue;
}

/*------------------------------------------------------------------------------
	Method:   	CDivision::SortCurrentForm
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Sort the league table into order
------------------------------------------------------------------------------*/
void CDivision::SortCurrentForm()
{
    int ClubNum = 0;
	StLeagueTable records[26];
    for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        records[ClubNum].Posn = LoopCount;
        records[ClubNum].Points = cClub.GetCurrentForm()->GetCurrentFormPts();
        records[ClubNum].GoalsDiff = cClub.GetCurrentForm()->GetCurrentFormScored() - cClub.GetCurrentForm()->GetCurrentFormConceded();
        records[ClubNum].GoalFor = cClub.GetCurrentForm()->GetCurrentFormScored();
		records[ClubNum].TheClub = GetClubs()[LoopCount];
		ClubNum++;
    }

	::qsort(records, ClubNum, sizeof(StLeagueTable), StLeagueTableSortFunc);

    // Update the Club data
    for (int num = 0; num < ClubNum; num++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[records[num].Posn]);
		cClub.GetCurrentForm()->SetCurrentFormPosn(num);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoPlayMissedMatches
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoPlayMissedMatches()
{
	CCalendar& theDate = WorldData().GetCurrentDate();
	byte CurrentMatchDayNumber = theDate.GetMatchDay();
	while (GetMatchType(theDate.GetMatchDay()) != DIVNEWSEASON)
	{
		theDate.PreviousMatchDay();			// back a match
	}

	// play all matches to present date
	while (CurrentMatchDayNumber != theDate.GetMatchDay())
	{
		GetCountry().GetActiveCountry().DoDailyClearDown();
		DoPlayLeagueMatches(false);
		if (theDate.FirstDateOfMonth() == true)
		{
			DoBuildManagerOfMonth();		// Check and process manager of the month awards if necc
		}
		theDate.NextMatchDay();			// Update the date
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
		{
			CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
			cClub.DoDailyTasks(theDate.IsWeekEnd());
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCountry
	Access:    	public
	Returns:   	CCountry&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CDivision::GetCountry()
{
	return WorldData().GetCountry(m_CountryID);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCountryRanking
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetCountryRanking()
{
	return GetCountry().GetRanking();
}


 //---------------------------------------------------------------------------------------
bool CDivision::HasPlayedLeagueMatch()
{
	if (IsPlayOffMatchDay(GetCurrentMatchDayNumber()) || IsFriendlyMatchDay(GetCurrentMatchDayNumber()) == true)
	{
		return false;
	}
	switch(GetMatchType(GetCurrentMatchDayNumber()))
	{
  		case DIVNOMATCH:
		case DIVENDSEASON:
		case DIVNEWSEASON:
		case DIVCLOSESEASON:
		case DIVTRAININGONLY:
			return false;
			break;		// No fixtures this turn

		case DIVARRANGEFRIENDLIES:
			return false;
			break;		// No fixtures this turn

		case DIVPLAYOFFINIT:
			return false;
			break;

		default :
			return true;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetDivisionStrength
	Access:    	public
	Returns:   	CDivisionStrength&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDivisionStrength& CDivision::GetDivisionStrength()
{
    return WorldData().GetDivisionStrengthList().GetAt(m_DivisionStrengthID);
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildPlayerSkillClubPtrList
	Access:    	public
	Parameter: 	CNumericArray<ushort> & _TheList
	Parameter: 	const byte _PlayerSkill
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildPlayerSkillClubPtrList(CNumericArray<ushort>& _TheList, const byte _PlayerSkill)
{
	EXT_ASSERT(_PlayerSkill >= MINPLAYSKILL && _PlayerSkill  < MAXPLAYSKILL);
    for (uint LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
        if (cClub.GetFirstTeamSquad().DoCalculateMinimumPlayerSkill(&cClub) <= _PlayerSkill &&
			cClub.GetFirstTeamSquad().DoCalculateMaximumPlayerSkill(&cClub) >= _PlayerSkill)
		{
			_TheList.Add(LoopCount);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::CreateTransferList
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::CreateTransferList()
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		WorldData().GetClubByID(GetClubs()[LoopCount]).DoTransferListRandomPlayers(RandomNumber.IntLessThan(2));
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CDivision::CreateClubsStartingOrder
	Access:    	public
	Parameter: 	CNumericArray<ushort> & _ClubsStartingOrder
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::CreateClubsStartingOrder(CNumericArray<ClubID>& _ClubsStartingOrder)
{
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		//g_DebugStr.Format(_T("%d - %s"), GetClubID(LoopCount), GetClub(LoopCount).GetName());
		//g_DebugStr.OutputDebug();
		_ClubsStartingOrder.Add(GetClubs()[LoopCount]);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetPointsForWin
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivision::GetPointsForWin()
{
	return m_PointsForWin;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCountryID
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetCountryID() const
{
	return m_CountryID;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetManagerOfMonth
	Access:    	public
	Parameter: 	const ushort _MonthNum
	Returns:   	CManagerOfTheMonth&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CManagerOfTheMonth&	CDivision::GetManagerOfMonth(const ushort _MonthNum)
{
	return m_MOMWinners[_MonthNum];
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetCharityShieldID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetCharityShieldID()
{
	return m_CharityshieldID;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetTeamOfTheWeek
	Access:    	public
	Parameter: 	void
	Returns:   	CTeamOfTheWeek&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTeamOfTheWeek& CDivision::GetTeamOfTheWeek()
{
	return m_TeamOfTheWeek;
}


#ifdef	QUICKRESULTS
//---------------------------------------------------------------------------------------
void CDivision::QuickResults(const byte DayNumber)
{
	CFixture& Fixture;
	CMatchInfo Match;
	m_MatchesPlayedThisMonth++;
	CSoccerResultList cSoccerResultList;
	DoBuildLeagueFixtureList(DayNumber, cSoccerResultList);
	for (int LoopCount = 0; LoopCount < cSoccerResultList.GetSize(); LoopCount ++)
	{
		// Play the match if not been postponed
		Fixture = DoInitialiseFixture(cSoccerResultList.GetAt(LoopCount)->GetHomeClubDetails().GetClubID(),
			cSoccerResultList.GetAt(LoopCount)->GetAwayClubDetails().GetClubID(), Match);
		Fixture.QuickResultGenerator();
	}
	cSoccerResultList.DeleteAll();
}
#endif


#ifdef ACTIVEINTERFEDERATIONCUPS
// Find a random club in Division providing it has not qualified for a
// Continental cup
// -------------------------------------------------------------------------------------------------------------------------
ushort CDivision::RandomClubNotInContinental(const ushort ClubToIgnore)
{
	ushort RandomClub = 0;
	do
	{
		RandomClub = RandomNumber.IntLessThan(GetNumberOfClubsInDivision()) + GetFirstClubNumberInDivision();
	}
	while (RandomClub == ClubToIgnore && EuropeanCupList.CheckIfInAnyCup(RandomClub) == true);
	return RandomClub;
}
#endif


#ifdef ASSISTS
/*------------------------------------------------------------------------------
	Method:   	CDivision::CountGoodAssistersForClub
	Access:    	public
	Parameter: 	CClub * pClub
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::CountGoodAssistersForClub(CClub* pClub)
{
	return pClub->GetFirstTeamSquad().CountPlayersAssistedAtLeast(max(3, GetLeadingAssisterAssists() - 4));
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::GetLeadingAssisterAssists
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::GetLeadingAssisterAssists()
{
	ushort iGoalsFor = 0;
	CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		pPlayer = cClub.GetFirstTeamSquad().GetLeadingAssister();
		if (pPlayer != NULL && pPlayer->GetSeasonAssist() > iGoalsFor)
		{
			iGoalsFor = pPlayer->GetSeasonAssist();
		}
	}
	return iGoalsFor;
}
#endif


#ifdef ACTIVEFRIENDLIES
/*------------------------------------------------------------------------------
Method:   	CDivision::DoPlayFriendlies
Access:    	public
Parameter: 	const bool bShowSummary
Returns:   	bool
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
bool CDivision::DoPlayFriendlies(const bool bShowSummary)
{
	bool bRet = false;
	CFixture theFixture;
	CMatchInfo Match;
	WorldData().GetMatchHandler().SetCompetitionInfo(this, NULL);
	for (int LoopCount = 0; LoopCount < GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub &cClub = WorldData().GetClubByID(GetClubs()[LoopCount]);
		if (cClub.GetFriendlies().IsFriendlyMatchToday(GetCurrentMatchDayNumber(), Match, &cClub) == true)
		{
			bRet = true;
			theFixture.OnInitialise(Match);
			theFixture.GetMatchVars().SetHomeAwayClubs(theFixture);

			WorldData().GetMatchHandler().GetMatchPlay().DoInitialiseIntro(&theFixture, NULL);
			if (theFixture.IsUserMatch() == true && theApp.DisplayMatchIntro(bShowSummary) == true)
			{
				// Add match to matches view list
				WorldData().GetMatchHandler().DoMatch(&theFixture);
			}
			else
			{
				// Calculate the match result
				theFixture.DoPlayMatch();
				theFixture.DoShowSummary(bShowSummary, NULL);
#ifdef NDS
        		if( theFixture.IsUserMatch() )
        		{
          			lLogFmt("Division Friendly Match Over\n");
          			// Friendlies are never displayed in the club fixture screen, don't highlight
          			//theApp.OnHighlightMenuIcon(HIGHLIGHT_FIXTURES);
        		}
#endif
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FriendlyNumberToday
	Access:    	public
	Parameter: 	int _DayNumber
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::WhichFriendlyNumberToday(int _MatchDayNumber)
{
	EXT_ASSERT(_MatchDayNumber < TOTALMATCHES);
	switch(GetMatchType(_MatchDayNumber))
	{
		case DIVFRIENDLYMATCH1:
			return 0;
		case DIVFRIENDLYMATCH2:
			return 1;
		case DIVFRIENDLYMATCH3:
			return 2;
		case DIVFRIENDLYMATCH4:
			return 3;
		default :
			return NOFRIENDLY;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FriendlyNumberToMatchNumber
	Access:    	public
	Parameter: 	const int _FriendlyNumber
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CDivision::FriendlyNumberToMatchNumber(const int _FriendlyNumber)
{
	ushort MatchTypeToFind = FriendlyMatchType(_FriendlyNumber);
	int iRet = -1;
	for (uint LoopCount = 0; LoopCount < TOTALMATCHES; LoopCount++)
	{
		if (GetMatchType(LoopCount) == MatchTypeToFind)
		{
			iRet = LoopCount;
			break;
		}
	}
	EXT_ASSERT(iRet != -1);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FriendlyMatchType
	Access:    	public
	Parameter: 	const int _DayNumber
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivision::FriendlyMatchType(const int _DayNumber)
{
	EXT_ASSERT(_DayNumber < MAXFRIENDLIES);
	ushort MatchType[] =
	{
		DIVFRIENDLYMATCH1,
		DIVFRIENDLYMATCH2,
		DIVFRIENDLYMATCH3,
		DIVFRIENDLYMATCH4
	};
	return MatchType[_DayNumber];
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FriendlyDateStr
	Access:    	public
	Parameter: 	const int _FriendlyNum
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CDivision::FriendlyDateStr(const int _FriendlyNum)
{
	return FriendlyDate(_FriendlyNum).DayMonthStr();
}


/*------------------------------------------------------------------------------
	Method:   	CDivision::FriendlyDate
	Access:    	public
	Parameter: 	int FriendlyNum
	Returns:   	CCalendar
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCalendar CDivision::FriendlyDate(int FriendlyNum)
{
	ushort FriendlyMatchTpe = FriendlyMatchType(FriendlyNum);
	CCalendar TheDate(WorldData().GetCurrentDate());
	DoFindStartOfSeason(TheDate);

  //NOTE: To prevent an infinite loop, if a friendly can't be found in the first season then break;
  ushort season = TheDate.GetSeason();

	// Assumes friendly match has not been played yet
	while (GetMatchType(TheDate.GetMatchDay()) != FriendlyMatchTpe)
	{
    EXT_ASSERT(TheDate.GetSeason() == season);
    /*if( TheDate.GetSeason() > season )
    {
      DoFindStartOfSeason(TheDate);
      return TheDate;
    }*/
		TheDate.NextMatchDay();	// forward a match
	}
	return TheDate;
}
#endif

#ifdef LATESTSCORES


/*------------------------------------------------------------------------------
	Method:   	CDivision::DoBuildCurrentFixtureList
	Access:    	public
	Parameter: 	CFixtureList& _LatestScores
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivision::DoBuildCurrentFixtureList(CFixtureList& _LatestScores)
{
	m_MatchData.BuildCurrentFixtureList(_LatestScores);
}
#endif


void CDivision::RemoveAllClubs()
{
	m_Clubs.RemoveAll();
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::CDivisionList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDivisionList::CDivisionList()
{
	m_pDivisionList = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoInitialise
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_DIVISION, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pDivisionList = FD_ENGINE_NEW CDivision[CExtArray::GetSize()];
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].DoInitialiseFromResource(theResource);
		m_pDivisionList[LoopCount].GetCountry().AddDivision(LoopCount);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::~CDivisionList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDivisionList::~CDivisionList()
{
	if (m_pDivisionList != NULL)
	{
		delete[] m_pDivisionList;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].Read(_pDataFile);
	}
 }


// Process this weeks league games
// -----------------------------------------------------------------------
bool CDivisionList::DoPlayLeagueMatches(bool _ShowSummary)
{
    int LeagueMatchPlayed = 0;              // Was a match played last week?

	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		LeagueMatchPlayed += m_pDivisionList[LoopCount].DoPlayLeagueMatches(_ShowSummary);
	}

	// Was a match played last week?
	if (LeagueMatchPlayed != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoAddPlayOffQualifiers
	Access:    	public
	Parameter: 	int _PlayOffID
	Parameter: 	CNumericArray<ushort> & _DrawClubs
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoAddPlayOffQualifiers(int _PlayOffID, CNumericArray<ClubID>& _DrawClubs)
{
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].DoAddPlayOffQualifiers(_PlayOffID, _DrawClubs);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::UpdateTables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::UpdateTables()
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		m_pDivisionList[LoopCount].SortCurrentForm();
		m_pDivisionList[LoopCount].SortLeagueTable();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoNewSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoNewSeason()
{
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].DoNewSeason();
    }
}


#ifndef TESTBED
/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoBuildNameList
	Access:    	public
	Parameter: 	CSortedStringList & _theList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoBuildNameList(CSortedStringList& _theList)
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		_theList.Add(m_pDivisionList[LoopCount].GetName(), LoopCount);
	}
}
#endif

 // Play all the matches prior to the start of the game from the start of
 // the season upto end of the m_MatchTypeCalendar
 // only to be called for countries where the season starts after the end of
 // season within m_MatchTypeCalendar
 //---------------------------------------------------------------------------------------
void CDivisionList::DoPlayMissedMatches()
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].DoPlayMissedMatches();
	}
	UpdateTables();
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoFindID
	Access:    	public
	Parameter: 	CDivision * _Division
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CDivisionList::DoFindID(CDivision* _Division)
{
	EXT_ASSERT(_Division != NULL);
	ushort iRet = wNOTFOUND;
	CDivision* t_pDivision = static_cast<CDivision*>(m_pDivisionList);
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        if (_Division == t_pDivision++)
		{
			iRet = LoopCount;
			break;
		}
	}
	EXT_ASSERT(iRet != wNOTFOUND);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoBuildManagerOfMonth
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoBuildManagerOfMonth()
{
	if (WorldData().GetCurrentDate().FirstDateOfMonth() == true)
	{
		for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
		{
           m_pDivisionList[LoopCount].DoBuildManagerOfMonth();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoDailyTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoDailyTasks()
{
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
		m_pDivisionList[LoopCount].DoDailyTasks();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::BuildMOS
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::BuildMOS()
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
         //pDivision++->BuildMOS ();
	}

 }


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::GetDivisionStrengthID
	Access:    	public
	Parameter: 	int _IndVal
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CDivisionList::GetDivisionStrengthID(int _IndVal)
{
    EXT_ASSERT(_IndVal > -1 && _IndVal < CExtArray::GetSize());
    return m_pDivisionList[_IndVal].GetDivisionStrengthID();
 }


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::RandomPtr
	Access:    	public
	Returns:   	CDivision*
	Qualifier:
	Purpose:	Select a random division
------------------------------------------------------------------------------*/
CDivision* CDivisionList::RandomPtr()
{
	// Check if more than 1 division
	if (CExtArray::GetSize() > 0)
	{
        return &m_pDivisionList[RandomNumber.IntLessThan(CExtArray::GetSize())];
	}
	else
	{
        return &m_pDivisionList[0];
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::DoEndOfWeek
	Access:    	public
	Parameter: 	bool _NoWarnings
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::DoEndOfWeek(bool _NoWarnings)
{
	for (int LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].DoEndOfWeekTasks(_NoWarnings);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::CreateTransferList
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionList::CreateTransferList()
{
	for (uint LoopCount = 0; LoopCount < CExtArray::GetSize(); LoopCount++)
	{
        m_pDivisionList[LoopCount].CreateTransferList();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionList::GetAt
	Access:    	public
	Parameter: 	ushort _ElementNumber
	Returns:   	CDivision&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CDivisionList::GetAt(ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < CExtArray::GetSize());
	return static_cast<CDivision&>(m_pDivisionList[_ElementNumber]);
}
