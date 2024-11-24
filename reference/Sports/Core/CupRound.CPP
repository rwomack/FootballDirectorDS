
 /*

    Program:- CCupRound.cpp

    The cup round class routines


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "MatchInfo.h"
#include "CupDisplayEvent.h"



/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetMatchList
	Access:    	public 
	Parameter: 	void
	Returns:   	CFixtureList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList& CCupRound::GetMatchList()
{
	return m_MatchList;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetReplayList
	Access:    	public 
	Parameter: 	void
	Returns:   	CFixtureList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList& CCupRound::GetReplayList()
{
	return m_ReplayList;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetRoundType
	Access:    	public 
	Parameter: 	void
	Returns:   	eCupRoundTypes
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eCupRoundTypes CCupRound::GetRoundType()
{
	return static_cast<eCupRoundTypes>(m_Type);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetNumInRound
	Access:    	public const 
	Parameter: 	void
	Returns:   	const byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetNumInRound() const
{
	return m_NumberOfClubsInRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetNumberOfClubsInGroup
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetNumberOfClubsInGroup() const
{
	return m_NumberOfClubsInGroup;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetMatchDate
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetMatchDate() const
{
	return m_MatchDate;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetDrawDate
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetDrawDate()
{
	return m_DrawDate;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetReplayDate
	Access:    	public 
	Parameter: 	void
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetReplayDate()
{
	return m_ReplayDate;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetNumberOfGroups
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCupRound::GetNumberOfGroups()
{
	return 	m_NumberOfClubsInRound / m_NumberOfClubsInGroup;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetNumberNonLeagueClubsToAdd
	Access:    	public const 
	Parameter: 	void
	Returns:   	const byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::GetNumberNonLeagueClubsToAdd() const
{
	return m_NumberNonLeagueClubsToAdd;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::SetNumberNonLeagueClubsToAdd
	Access:    	public 
	Parameter: 	const byte nNumberNonLeagueClubsToAdd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::SetNumberNonLeagueClubsToAdd(const byte nNumberNonLeagueClubsToAdd)
{
	m_NumberNonLeagueClubsToAdd = nNumberNonLeagueClubsToAdd;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	CExtString strName;
	strName.ReadFromResource(_Resource);
	m_NameID.DoFindID(strName);
	strName.ReadFromResource(_Resource);
	m_TitleID.DoFindID(strName);
	strName.ReadFromResource(_Resource);
	m_ReplayNameID.DoFindID(strName);
	strName.ReadFromResource(_Resource);
	m_DrawNameID.DoFindID(strName);

	m_NumberOfClubsInRound = _Resource.ReadByte();
	EXT_ASSERT(m_NumberOfClubsInRound < 200);
	m_NumberOfClubsInGroup = _Resource.ReadByte();
#ifdef DEBUG_LOAD_CUP_ROUNDS
	DEBUG_OUTPUT(("%s (%d), m_NumberOfClubsInRound %d : m_NumberOfClubsInGroup %d"), (GetName(), m_NameID, m_NumberOfClubsInRound, m_NumberOfClubsInGroup));
	if (m_NumberOfClubsInGroup > m_NumberOfClubsInRound)
	{
		DEBUG_OUTPUT(("%s %d, m_NumberOfClubsInGroup %d"), (GetName(), m_NameID, m_NumberOfClubsInGroup));
	}
#else
	EXT_ASSERT(m_NumberOfClubsInGroup < m_NumberOfClubsInRound);
#endif
	m_NumberNonLeagueClubsToAdd = 0;
    m_Type = _Resource.ReadByte();
	m_Type = min(m_Type, static_cast<eCupRoundTypes>(MAXROUNDTYPES - 1));
    m_DrawDate = _Resource.ReadByte();
	if (m_DrawDate != IGNOREDATE)
	{
		m_DrawDate--;
	}
    m_MatchDate = _Resource.ReadByte() - 1;
	EXT_ASSERT(m_MatchDate < TOTALMATCHES);
	m_ReplayDate = _Resource.ReadByte();
	if (m_ReplayDate != IGNOREDATE)
	{
		m_ReplayDate--;
	}
	EXT_ASSERT(m_ReplayDate == IGNOREDATE || m_ReplayDate > m_MatchDate);
#ifdef	ACTIVEREFEREES
    m_MinimumRefereeRating = _Resource.ReadByte();
    m_MaximumRefereeRating = _Resource.ReadByte();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoCalculateAggregateGoals
	Access:    	public 
	Parameter: 	CCupRound* pLastCupRound
	Returns:   	void
	Qualifier: 	
	Purpose:	if was a 2 leg round the {GoalsToScore[]} contains aggregate result
------------------------------------------------------------------------------*/
void CCupRound::DoCalculateAggregateGoals(CCupRound* pLastCupRound)
{
 	EXT_ASSERT_POINTER(pLastCupRound, CCupRound);
    CFixture* pLeg1Fixture;
    CFixture* pLeg2Fixture;
    for (int MatchNum = 0; MatchNum < m_MatchList.GetSize(); MatchNum ++)
	{
        pLeg1Fixture = &pLastCupRound->GetMatchList()[MatchNum];
		EXT_ASSERT_POINTER(pLeg1Fixture, CFixture);
        pLeg2Fixture = &m_MatchList[MatchNum];
		EXT_ASSERT_POINTER(pLeg2Fixture, CFixture);
		if (DoAwayGoalsCountDouble() == false)
		{
			pLeg2Fixture->SetHomeGoalsToScore(pLeg1Fixture->GetNumberGoals(AWAY) + pLeg2Fixture->GetNumberGoals(HOME));
			pLeg2Fixture->SetAwayGoalsToScore(pLeg1Fixture->GetNumberGoals(HOME) + pLeg2Fixture->GetNumberGoals(AWAY));
		}
		else
		{
			pLeg2Fixture->SetHomeGoalsToScore(pLeg1Fixture->GetNumberGoals(AWAY) + pLeg2Fixture->GetNumberGoals(HOME));
			pLeg2Fixture->SetAwayGoalsToScore(pLeg1Fixture->GetNumberGoals(HOME) + pLeg2Fixture->GetNumberGoals(AWAY));
			// If a draw after 2 legs then away goals count double
			if (pLeg2Fixture->GetHomeGoalsToScore() == pLeg1Fixture->GetNumberGoals(AWAY))
			{
				pLeg2Fixture->SetHomeGoalsToScore(pLeg1Fixture->GetNumberGoals(AWAY) * 2 + pLeg2Fixture->GetNumberGoals(HOME));
				pLeg2Fixture->SetAwayGoalsToScore(pLeg1Fixture->GetNumberGoals(HOME) + pLeg2Fixture->GetNumberGoals(AWAY) * 2);
            }
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoAwayGoalsCountDouble
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::DoAwayGoalsCountDouble()
{
	bool bRet = true;
	switch(GetRoundType())
	{
		case LEG2:
		case LEG2EXTRATIME:
		case LEG22REGIONS:
		case LEG2EXTRATIME2REGIONS:
			bRet = false;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::~CCupRound
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound::~CCupRound()
{
    RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::RemoveList
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::RemoveList()
{
    m_MatchList.RemoveList();
    m_ReplayList.RemoveList();
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsNeutralVenue
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsNeutralVenue()
{
	bool bRet = false;
	switch(m_Type)
	{
		case NEUTRAL:
		case NEUTRAL2REGIONS:
		case NEUTRALEXTRATIME:
			bRet = true;
			break;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::Is2Regions
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::Is2Regions()
{
	bool bRet = false;
	switch(m_Type)
	{
		case GROUP3ONCE2REGIONS:
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
			bRet = true;
			break;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsSecondLeg
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsSecondLeg()
{
	bool bRet = false;
	switch(m_Type)
	{
		case LEG2:
		case LEG2EXTRATIME:
		case LEG2AWAYDOUBLE:
		case LEG2EXTRATIMEAWAYDOUBLE:
		case LEG22REGIONS:
		case LEG2EXTRATIME2REGIONS:
		case LEG2AWAYDOUBLE2REGIONS:
		case LEG2EXTRATIMEAWAYDOUBLE2REGIONS:
			bRet = true;
			break;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::VenueType
	Access:    	public 
	Parameter: 	void
	Returns:   	const eMatchCompetitionType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eMatchCompetitionType CCupRound::VenueType()
{
	if (IsNeutralVenue() == true)
	{
		return CUPMATCHNEUTRALVENUE;
	}
	return CUPMATCH;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsGroupMatch
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsGroupMatch()
{
	bool bRet = false;
#ifdef CUPGROUPS
	switch(m_Type)
	{
		case GROUPMATCH:
		case GROUP3ONCE2REGIONS:
		case GROUP415372648:
			bRet = true;
			break;
	}
#endif
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsExtraTimeMatch
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsExtraTimeMatch()
{
	bool bRet = false;
	switch(m_Type)
	{
		case EXTRATIME:
		case EXTRATIME2REGIONS:
		case REVERSEDEXTRATIME2REGIONS:
		case REVERSEDEXTRATIME:
		case NEUTRALEXTRATIME:
		case EXTRATIME1423:
		case EXTRATIME1432:
			bRet = true;
			break;
	}
	return bRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoAddLastRoundWinners
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Parameter: 	CNumericArray<ushort>& aLosers
	Parameter: 	bool IsCharityShield
	Parameter: 	CCup& pCup
	Returns:   	const ushort
	Qualifier: 	
	Purpose:	Place the winners of the last round matches into the draw
				returns the number of winners found
------------------------------------------------------------------------------*/
ushort CCupRound::DoAddLastRoundWinners(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup)
{
	EXT_ASSERT(m_MatchList.GetSize() > 0);
	for (int LoopCount = 0; LoopCount < m_MatchList.GetSize(); LoopCount++)
	{
        _DrawClubs.Add(GetRoundFixtureWinners(LoopCount, _Losers, _IsCharityShield, _Cup));
    }
	EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
	return m_MatchList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::FindChampionsAndRunnersUp
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Parameter: 	CNumericArray<ushort>& aLosers
	Parameter: 	bool IsCharityShield
	Parameter: 	CCup& pCup
	Returns:   	void
	Qualifier: 	
	Purpose:	Place the winners of the final into the draw(0)
				Place the runners up of the final into the draw(1)
------------------------------------------------------------------------------*/
void CCupRound::DoFindChampionsAndRunnersUp(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup)
{
	DoAddLastRoundWinners(_DrawClubs, _Losers, _IsCharityShield, _Cup);
	EXT_ASSERT(m_MatchList.GetSize() > 0);
	if (m_MatchList[0].GetHomeClubID() == static_cast<signed int>(_DrawClubs[0]))
	{
		_DrawClubs.Add(m_MatchList[0].GetAwayClubID());
	}
	else
	{
		_DrawClubs.Add(m_MatchList[0].GetHomeClubID());
	}
	EXT_ASSERT(_DrawClubs.GetSize() == 2);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::WhoWonOnAwayGoalsCountDoubleClubID
	Access:    	public 
	Parameter: 	CFixture* apFixture
	Parameter: 	CCup& pCup
	Returns:   	ushort - ClubID of club
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::WhoWonOnAwayGoalsCountDoubleClubID(CFixture* _Fixture, CCup& _Cup)
{
	CFixture* pLastFixture = _Cup.GetFirstLeg(*this)->GetMatchDetailsPtr(m_MatchList.Find(_Fixture));

	// Get the aggregate score over the 2 legs
	_Fixture->SetHomeGoalsToScore(pLastFixture->GetNumberGoals(AWAY) + _Fixture->GetNumberGoals(HOME));
	_Fixture->SetAwayGoalsToScore(pLastFixture->GetNumberGoals(HOME) + _Fixture->GetNumberGoals(AWAY));

	// If a draw over 2 legs
	if (_Fixture->GetHomeGoalsToScore() == _Fixture->GetAwayGoalsToScore() && pLastFixture != null)
	{
		// Check if away goals count double
		if (DoAwayGoalsCountDouble() == true)
		{
			// Yes so add them on
			_Fixture->SetHomeGoalsToScore(pLastFixture->GetNumberGoals(AWAY) * 2 + _Fixture->GetNumberGoals(HOME));
			_Fixture->SetAwayGoalsToScore(pLastFixture->GetNumberGoals(HOME) + _Fixture->GetNumberGoals(AWAY) * 2);
		}
	}
    ushort ClubID = NOCLUB;
	// If is 2 legs then result is in {GoalsToScore}
	int TotalHomeGoals, TotalAwayGoals;
    TotalHomeGoals = _Fixture->GetHomeGoalsToScore();
    TotalAwayGoals = _Fixture->GetAwayGoalsToScore();
	if (TotalHomeGoals == TotalAwayGoals)
	{
		TotalHomeGoals += _Fixture->GetNumberGoals(HOME);
        TotalAwayGoals += _Fixture->GetNumberGoals(AWAY);
		if (TotalHomeGoals != TotalAwayGoals)
		{
			if (TotalHomeGoals > TotalAwayGoals)
			{
				ClubID = _Fixture->GetHomeClubID();
			}
			if (TotalHomeGoals < TotalAwayGoals)
			{
				ClubID = _Fixture->GetAwayClubID();
			}
			EXT_ASSERT(ClubID != NOCLUB);
			return ClubID;
		}
	}
	return wNOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::WasWonOnAwayGoals
	Access:    	public 
	Parameter: 	CFixture* apFixture
	Parameter: 	CCup& apCup
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::WasWonOnAwayGoals(CFixture* _Fixture, CCup& _Cup)
{
	bool bRet = false;
	if (DoAwayGoalsCountDouble() == true)
	{
		if (WhoWonOnAwayGoalsCountDoubleClubID(_Fixture, _Cup) != wNOTFOUND)
		{
			bRet = true;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::WhoWonOnAwayGoalsCountDoublestr
	Access:    	public 
	Parameter: 	CFixture* apFixture
	Parameter: 	CCup& apCup
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupRound::WhoWonOnAwayGoalsCountDoublestr(CFixture* _Fixture, CCup& _Cup)
{
	CString sRet;
	sRet.Format(IDS_WONONAWAYGOALSDOUBLE, WorldData().GetClub(WhoWonOnAwayGoalsCountDoubleClubID(_Fixture, _Cup)).GetName());
	return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsFirstLeg
	Access:    	public 
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsFirstLeg()
{
	bool bRet = false;
	switch(m_Type)
	{
		case LEG1:
		case LEG1AWAYDOUBLE:
		case LEG1AWAYDOUBLE1423:
		case LEG1AWAYDOUBLE1324:
		case LEG12REGIONS:
		case LEG1AWAYDOUBLE2REGIONS:
			bRet = true;
			break;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetRoundFixtureWinners
	Access:    	public 
	Parameter: 	const ushort _MatchNum
	Parameter: 	CNumericArray<ushort> & _Losers
	Parameter: 	bool _IsCharityShield
	Parameter: 	CCup & _Cup
	Parameter: 	const uint _HowManyWinners
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::GetRoundFixtureWinners(const ushort _MatchNum, CNumericArray<ushort>& _Losers, bool _IsCharityShield, CCup& _Cup, const uint _HowManyWinners /*= 1*/)
{
	EXT_ASSERT(_MatchNum < m_MatchList.GetSize());
	UINT TotalHomeGoals, TotalAwayGoals;
    CFixture* pFinalFixtureForTie;
    ushort retClubID = wNOTFOUND, LoserClubID = wNOTFOUND;

	EXT_ASSERT(IsFirstLeg() == false);
	if (IsSecondLeg() == true)
	{
		// Get the aggregate score over the 2 legs
		pFinalFixtureForTie = &_Cup.GetFirstLeg(*this)->GetMatchList()[_MatchNum];             // First leg CFixture
		EXT_ASSERT_POINTER(pFinalFixtureForTie, CFixture);
		m_MatchList[_MatchNum].SetHomeGoalsToScore(pFinalFixtureForTie->GetNumberGoals(AWAY) + m_MatchList[_MatchNum].GetNumberGoals(HOME));
		m_MatchList[_MatchNum].SetAwayGoalsToScore(pFinalFixtureForTie->GetNumberGoals(HOME) + m_MatchList[_MatchNum].GetNumberGoals(AWAY));
		// If is 2 legs then result is in {GoalsToScore}
        TotalHomeGoals = m_MatchList[_MatchNum].GetHomeGoalsToScore();
        TotalAwayGoals = m_MatchList[_MatchNum].GetAwayGoalsToScore();
		// If is drawn  and away goals count double
		if (TotalHomeGoals == TotalAwayGoals && DoAwayGoalsCountDouble() == true)
		{
	        TotalHomeGoals += pFinalFixtureForTie->GetNumberGoals(AWAY);
            TotalAwayGoals += m_MatchList[_MatchNum].GetNumberGoals(AWAY);
		}
		// Things still tied so get penalty shoot out winners
		if (TotalHomeGoals == TotalAwayGoals)
		{
			// Give the penalty shoot out winners an extra goal
			// ********** Fudge alert this needs changing, assumes there was a shoot out else away club goes thru *****************
			// ********** Need to change PlayMatches to ensure penalty shoot out happens, *****************************************
			if (m_MatchList[_MatchNum].GetEventList()->WasPenaltyShootOut() == true && m_MatchList[_MatchNum].GetEventList()->PenaltyShootoutWinner() == HOME)
			{
				TotalHomeGoals++;
			}
			else
			{
				TotalAwayGoals++;
			}
		}

		if (TotalHomeGoals > TotalAwayGoals)
		{
            LoserClubID = m_MatchList[_MatchNum].GetAwayClubID();
            retClubID = m_MatchList[_MatchNum].GetHomeClubID();
		}
		if (TotalHomeGoals < TotalAwayGoals)
		{
            LoserClubID = m_MatchList[_MatchNum].GetHomeClubID();
            retClubID = m_MatchList[_MatchNum].GetAwayClubID();
		}
		EXT_ASSERT(retClubID != wNOTFOUND && LoserClubID != wNOTFOUND);
		EXT_ASSERT(retClubID != LoserClubID);
	}
	else if (IsExtraTimeMatch() == true)
	{
		LoserClubID = m_MatchList[_MatchNum].GetLosingClubID();
		retClubID = m_MatchList[_MatchNum].GetWinningClubID();
		EXT_ASSERT(retClubID != wNOTFOUND && LoserClubID != wNOTFOUND);
		EXT_ASSERT(retClubID != LoserClubID);
	}
	else
	{
		switch (m_Type)
		{
			case NORMALMATCH:
			case REVERSEDNORMAL:
			case NEUTRAL:
			case NORMALMATCH2REGIONS:
			case REVERSEDNORMAL2REGIONS:
			case NEUTRAL2REGIONS:
				if (m_MatchList[_MatchNum].GetNumberGoals(HOME) != m_MatchList[_MatchNum].GetNumberGoals(AWAY))
				{
					pFinalFixtureForTie = &m_MatchList[_MatchNum];
				} 
				else
				{
					// drawn so find replay data
					pFinalFixtureForTie = m_ReplayList.FindFixture(m_MatchList[_MatchNum].GetAwayClubID(), m_MatchList[_MatchNum].GetHomeClubID());
				}
				EXT_ASSERT_POINTER(pFinalFixtureForTie, CFixture);
				//DEBUG_OUTPUT(("%s %d - %d %s"), (pFinalFixtureForTie->GetClubName(HOME), pFinalFixtureForTie->GetNumberGoals(HOME), pFinalFixtureForTie->GetNumberGoals(AWAY), pFinalFixtureForTie->GetClubName(AWAY)));
				LoserClubID = pFinalFixtureForTie->GetLosingClubID();
				retClubID = pFinalFixtureForTie->GetWinningClubID();
				EXT_ASSERT(retClubID != wNOTFOUND && LoserClubID != wNOTFOUND);
				break;

	#ifdef CUPGROUPS
			case GROUPMATCH:
			case GROUP3ONCE2REGIONS:
			case GROUP415372648:
	#endif
			default :
				EXT_ASSERT(false);
				break;
		}
	}
    if (LoserClubID != wNOTFOUND)
	{
		OutOfCupMessage(LoserClubID, _Cup.GetName(), _IsCharityShield, WorldData().GetClub(retClubID));
		_Losers.Add(LoserClubID);
	}
    return retClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoBuildReplayList
	Access:    	public 
	Parameter: 	CCupRound* pPreviousRound
	Parameter: 	CCup* pCup
	Returns:   	void
	Qualifier: 	
	Purpose:	If matches have been played today, check for any neccesary replays
				If was a 2 leg round and was leg 2 then calculate aggregate score
------------------------------------------------------------------------------*/
void CCupRound::DoBuildReplayList(CCupRound* _PreviousRound, CCup* _Cup)
{
	EXT_ASSERT(m_MatchList.GetSize() > 0);

	// Return if nothing to process for this round type
	if (IsGroupMatch() == true || IsFirstLeg() == true)
	{
		return;
	}
	if (IsExtraTimeMatch() == true)
	{
		return;
	}

#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	CMatchInfo Match;
	Match.SetSubstitutes(_Cup->GetSubsSelect(), _Cup->GetSubsUse());
	// Check all matches in the round for replays
	for (int MatchNum = 0; MatchNum < m_MatchList.GetSize(); MatchNum ++)
	{
		Match.DoInitialiseClubs(m_MatchList[MatchNum].GetAwayClubID(), m_MatchList[MatchNum].GetHomeClubID());
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		if (IsSecondLeg() == true)
		{
			// Calculate the aggregate scores
			DoCalculateAggregateGoals(_PreviousRound);
		}
		else if (IsNeutralVenue() == true)
		{
			// setup replay
			if (m_MatchList[MatchNum].GetNumberGoals(HOME) == m_MatchList[MatchNum].GetNumberGoals(AWAY))
			{
				EXT_ASSERT(m_ReplayDate != IGNOREDATE);
				Match.SetStadiumID(_Cup->GetNeutralVenueStadiumID());
				GetReplayList().Add(new CFixture(Match));
			}
		}
		else
		{
			switch(m_Type)
			{
				case NORMALMATCH:
				case REVERSEDNORMAL:
				case NORMALMATCH2REGIONS:
				case REVERSEDNORMAL2REGIONS:
					// setup replay
					if (m_MatchList[MatchNum].GetNumberGoals(HOME) == m_MatchList[MatchNum].GetNumberGoals(AWAY))
					{
						GetReplayList().Add(new CFixture(Match));
					}
					break;

				default :
					EXT_ASSERT(false);		// Dodgy round type
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoBuildEventsStrList
	Access:    	public 
	Parameter: 	CCupDisplayEventList& EventList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoBuildEventsStrList(CCupDisplayEventList& EventList)
{
	if (m_Type != ENDOFCUP)
	{
		if (m_DrawDate != IGNOREDATE && IsFinal() == false)
		{
			EventList.AddNew(DRAWDATE, this);
		}
		if (m_MatchDate != IGNOREDATE)
		{
			EventList.AddNew(MATCHDATE, this);
		}
		if (m_ReplayDate != IGNOREDATE)
		{
			EventList.AddNew(REPLAYDATE, this);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::LongMatchDateStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupRound::LongMatchDateStr()
{
	return DoCreateDate(m_MatchDate).LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::LongDrawDateStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupRound::LongDrawDateStr()
{
	return DrawDate().LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DrawDate
	Access:    	public 
	Parameter: 	void
	Returns:   	CMatchTime
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar CCupRound::DrawDate()
{
	return DoCreateDate(m_DrawDate);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoCreateDate
	Access:    	public 
	Parameter: 	const byte aDayNumber
	Returns:   	CMatchTime
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCalendar CCupRound::DoCreateDate(const byte _DayNumber)
{
	EXT_ASSERT(_DayNumber != IGNOREDATE);
	CCalendar TheDate(WorldData().GetCurrentDate());
	TheDate.DoSetToMatchDay(_DayNumber);
	return TheDate;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::LongReplayDateStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupRound::LongReplayDateStr()
{
	return DoCreateDate(m_ReplayDate).LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::OutOfCupMessage
	Access:    	public 
	Parameter: 	const ushort aClubID
	Parameter: 	CString CupTitle
	Parameter: 	bool IsCharityShield
	Parameter: 	CClub& arWinningClub
	Returns:   	void
	Qualifier: 	
	Purpose:	If the club has been knocked out of the cup this round and has 
				a human message then send the club a message expressing the 
				board or fan's attitude
------------------------------------------------------------------------------*/
void CCupRound::OutOfCupMessage(const ushort _ClubID, CString _CupTitle, bool _IsCharityShield, CClub& _WinningClub)
{
    CClub& theClub = WorldData().GetClub(_ClubID);

    if (theClub.IsUserControlled() == true && _IsCharityShield == false)
	{
// 		int Msg;
// 		if (_WinningClub.GetDivisionStrengthID() < theClub.GetDivisionStrengthID())
// 		{
// 			Msg = IDS_CUPDEFEAT7;
// 			theClub.DoLowerMorale(10);
// 		}
// 		else if (_WinningClub.GetDivisionStrengthID() < theClub.GetDivisionStrengthID())
// 		{
// 			Msg = IDS_CUPDEFEAT8;
// 			theClub.DoLowerMorale(40);
// 		}
// 		else
// 		{
// 			switch (m_NumberOfClubsInRound)
// 			{
// 				case 2:
// 					Msg = IDS_CUPDEFEAT6;
// 					break;
// 				case 4:
// 					Msg = IDS_CUPDEFEAT4;
// 					break;
// 				case 8:
// 					Msg = IDS_CUPDEFEAT2;
// 					break;
// 				case 16:
// 					Msg = IDS_CUPDEFEAT1;
// 					break;
// 				default:
// 					Msg = IDS_CUPDEFEAT0;
// 					break;
// 			}
// 			theClub.DoLowerMorale(30);
// 		}
// 	    CExtString str;
// 		str.Format(Msg, _CupTitle);
// 		// Inform the user
//         theClub.AddToClubNews(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsFinal
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsFinal()
{
	return (m_NumberOfClubsInRound == 2);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsFixtureInRound
	Access:    	public 
	Parameter: 	CFixture* pFixture
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsFixtureInRound(CFixture* pFixture)
{
	if (m_MatchList.FindID(pFixture) !=  wNOTFOUND)
	{
		return true;
	}
	if (m_ReplayList.FindID(pFixture) != wNOTFOUND)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::IsClubInRound
	Access:    	public 
	Parameter: 	int CurrClub
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::IsClubInRound(const ushort _ClubID)
{
	if (m_MatchList.FindClubFixture(0, _ClubID) == null)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::HasDrawBeenMade
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::HasDrawBeenMade()
{
	bool bRet = false;
	if (m_MatchList.GetSize() > 0)
	{
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::HaveAllRoundMatchesBeenPlayed
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::HaveAllRoundMatchesBeenPlayed()
{
	bool bRet = false;
	if (m_MatchList.GetSize() > 0)
	{
        if (m_MatchList[0].GetMatchLength() > 0)
		{
			if (m_ReplayList.GetSize() > 0)
			{
                bRet = m_ReplayList[0].GetMatchLength() > 0;
			}
			else
			{
				bRet = true;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetClubMatchDetailsPtr
	Access:    	public 
	Parameter: 	const ushort ClubNumber
	Returns:   	CFixture*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CCupRound::GetClubMatchDetailsPtr(const ushort _ClubID)
{
	EXT_ASSERT(m_MatchList.GetSize() > 0);
	return m_MatchList.FindClubFixture(0, _ClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetRoundLosers
	Access:    	public 
	Parameter: 	const uint MatchNum
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CCupRound::GetRoundLosers(const int _MatchNum)
{
	EXT_ASSERT(IsFirstLeg() == false);
	EXT_ASSERT(_MatchNum < m_MatchList.GetSize());
	int ListPos;
	if (IsSecondLeg() == true)
	{
		// If is 2 legs then result is in {GoalsToScore}
		// if away goals count double then these are included
		if (m_MatchList[_MatchNum].GetHomeGoalsToScore() < m_MatchList[_MatchNum].GetAwayGoalsToScore())
		{
			return m_MatchList[_MatchNum].GetHomeClubID();
		}
		if (m_MatchList[_MatchNum].GetHomeGoalsToScore() > m_MatchList[_MatchNum].GetAwayGoalsToScore())
		{
			return m_MatchList[_MatchNum].GetAwayClubID();
		}
		EXT_ASSERT(false);
	}
	else
	{
		switch(m_Type)
		{
			case NORMALMATCH:
			case EXTRATIME:
			case REVERSEDNORMAL:
			case REVERSEDEXTRATIME:
			case NEUTRAL:
			case NORMALMATCH2REGIONS:
			case EXTRATIME2REGIONS:
			case REVERSEDNORMAL2REGIONS:
			case REVERSEDEXTRATIME2REGIONS:
			case NEUTRAL2REGIONS:
			case NEUTRALEXTRATIME:
				if (m_MatchList[_MatchNum].GetHomeGoalsToScore() < m_MatchList[_MatchNum].GetNumberGoals(AWAY))
				{
					return m_MatchList[_MatchNum].GetHomeClubID();
				}
				if (m_MatchList[_MatchNum].GetHomeGoalsToScore() < m_MatchList[_MatchNum].GetNumberGoals(AWAY))
				{
					return m_MatchList[_MatchNum].GetAwayClubID();
				}
				// drawn so find replay data
				for (ListPos = 0; ListPos < m_ReplayList.GetSize(); ListPos++)
				{
					if (m_ReplayList[ListPos].GetHomeClubID() == m_MatchList[_MatchNum].GetAwayClubID())
					{
						if (m_ReplayList[ListPos].GetHomeGoalsToScore() < m_ReplayList[ListPos].GetNumberGoals(AWAY))
						{
							return m_ReplayList[ListPos].GetHomeClubID();
						}
						if (m_ReplayList[ListPos].GetHomeGoalsToScore() > m_ReplayList[ListPos].GetNumberGoals(AWAY))
						{
							return m_ReplayList[ListPos].GetAwayClubID();
						}

					}
				}
				EXT_ASSERT(false);		// Not found replay
				break;

			default :
				EXT_ASSERT(false);
		}
	}
	return NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoGroup3Draw1
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	CCupRound* pLastCupRound
	Parameter: 	CCupRound* a_pNextRound1
	Parameter: 	CCupRound* a_pNextRound2
	Returns:   	void
	Qualifier: 	
	Purpose:	Do the draw for a group of 3 playing each other once round
------------------------------------------------------------------------------*/
void CCupRound::DoGroup3Draw1 (CCup& pCup, CCupRound* pLastCupRound, CCupRound* a_pNextRound1, CCupRound* a_pNextRound2)
{
	EXT_ASSERT(IsFirstLeg() == false);
    CNumericArray<ushort> DrawClubs, Losers;
	GetDrawClubs(DrawClubs, Losers, pCup, pLastCupRound);
	DoCopyTo3Rounds(pCup, DrawClubs, a_pNextRound1, a_pNextRound2);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::CopyTo3Rounds
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Parameter: 	CCupRound* a_pNextRound1
	Parameter: 	CCupRound* a_pNextRound2
	Returns:   	void
	Qualifier: 	
	Purpose:	Transfer the draw to the fixtures for 3 group match rounds
				(play each other once only)
------------------------------------------------------------------------------*/
void CCupRound::DoCopyTo3Rounds(CCup& _Cup, CNumericArray<ushort>& _DrawClubs, CCupRound* _NextRound1, CCupRound* _NextRound2)
{
	int RefereeNumber = 0;
	CMatchInfo Match(VenueType(), _Cup.GetSubsSelect(), _Cup.GetSubsUse());
	if (VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Match.SetStadiumID(_Cup.GetNeutralVenueStadiumID());
	}
    for (uint LoopCount = 0; LoopCount < m_NumberOfClubsInRound; LoopCount += 3)
	{
        Match.DoInitialiseClubs(_DrawClubs[LoopCount], _DrawClubs[LoopCount + 1]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_MatchList.Add(new CFixture(Match));

		Match.DoInitialiseClubs(_DrawClubs[LoopCount + 1], _DrawClubs[LoopCount + 2]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
        _NextRound1->GetMatchList().Add(new CFixture(Match));

		Match.DoInitialiseClubs(_DrawClubs[LoopCount + 2], _DrawClubs[LoopCount]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
        _NextRound2->GetMatchList().Add(new CFixture(Match));
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetDrawClubs
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Parameter: 	CNumericArray<ushort>& aLosers
	Parameter: 	CCup& pCup
	Parameter: 	CCupRound* pLastCupRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::GetDrawClubs(CNumericArray<ushort>& _DrawClubs, CNumericArray<ushort>& _Losers, CCup& _Cup, CCupRound* _LastCupRound)
{
	_Cup.CupAndRoundTitle(*this).OutputDebug();
	// If is the first round
	if (_LastCupRound == null)
	{
		if (GetNumberNonLeagueClubsToAdd() == 0 && _Cup.GetSpecialActionFlag() == NOSPECIALEVENT)
		{
			m_NumberNonLeagueClubsToAdd = _Cup.TotalNonLeagueClubs();
		}
#ifdef DEBUG_LOAD_CUP_CLUBS
		DEBUG_OUTPUT(("Non league clubs %d"), (_Cup.TotalNonLeagueClubs())0;
#endif
		EXT_ASSERT(GetNumberNonLeagueClubsToAdd() <= _Cup.TotalNonLeagueClubs());
		// Add the nonleagueclubs
		for (uint LoopCount = 0; LoopCount < GetNumberNonLeagueClubsToAdd(); LoopCount++)
		{
			EXT_ASSERT(_Cup.GetNonLeagueClub(LoopCount) < WorldData().GetTotalClubs());
			_DrawClubs.Add(_Cup.GetNonLeagueClub(LoopCount));
		}
		EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
	}
	else
	{
		// Add the winners of the last round
		EXT_ASSERT_POINTER(_LastCupRound, CCupRound);
		if (Is2Regions() == true && _LastCupRound->GetReplayListSize() > 0)
		{
			// This is the 2nd round of the Johnstones Paint Trophy
			// Bodge alert, the 1st round replay list contains the 16 clubs who were given a bye
			CFixtureList&  MatchList = _LastCupRound->GetMatchList();
			CFixtureList&  ReplayList = _LastCupRound->GetReplayList();
			for (int LoopCount = 0; LoopCount < MatchList.GetSize(); LoopCount++)
			{
				// Get the bye clubs 2 at a time
				if (LoopCount % 2 == 0)
				{
#ifdef DEBUG_JOHNSTONE_TROPHY
					DEBUG_OUTPUT(("Add %s, %s"),  (ReplayList[LoopCount / 2].GetClub(HOME).GetName(), ReplayList[LoopCount / 2].GetClub(AWAY).GetName()));
#endif
					_DrawClubs.Add(ReplayList[LoopCount / 2].GetHomeClubID());
					_DrawClubs.Add(ReplayList[LoopCount / 2].GetAwayClubID());
				}
				_DrawClubs.Add(MatchList[LoopCount].GetWinningClubID());
#ifdef DEBUG_JOHNSTONE_TROPHY
				DEBUG_OUTPUT(("Winner %s"),  (MatchList[LoopCount].GetWinningClub()->GetName()));
#endif
			}
		}
		else
		{
			_LastCupRound->DoAddLastRoundWinners(_DrawClubs, _Losers, false, _Cup);
		}
    }

    // Add the new clubs
#ifdef DEBUG_JOHNSTONE_TROPHY
	DEBUG_OUTPUT(("m_NumberOfClubsInRound %d,  _DrawClubs.GetSize() %d,  _Cup.GetLeagueClubsList().GetSize() %d"),  (m_NumberOfClubsInRound, _DrawClubs.GetSize(), _Cup.GetLeagueClubsList().GetSize()));
#endif
	EXT_ASSERT(m_NumberOfClubsInRound <= (_DrawClubs.GetSize() + _Cup.GetLeagueClubsList().GetSize()));
    if (_DrawClubs.GetSize() < m_NumberOfClubsInRound && _Cup.LeagueClubsListSize() > 0)
	{
#ifdef DEBUG_JOHNSTONE_TROPHY
		DEBUG_OUTPUT(("m_NumberOfClubsInRound %d,  _DrawClubs.GetSize() %d,  _Cup.GetLeagueClubsList().GetSize() %d"), (m_NumberOfClubsInRound, _DrawClubs.GetSize(), _Cup.GetLeagueClubsList().GetSize()));
		g_DebugStr.OutputDebug();
#endif
		EXT_ASSERT(m_NumberOfClubsInRound <= (_DrawClubs.GetSize() + _Cup.GetLeagueClubsList().GetSize()));
		_DrawClubs.DoAddAndRemoveFromArray2(_Cup.GetLeagueClubsList(), m_NumberOfClubsInRound - _DrawClubs.GetSize());
		EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
    }
	EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
	EXT_ASSERT(_DrawClubs.GetSize() == m_NumberOfClubsInRound);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoDraw
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	CCupRound* pLastCupRound
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoDraw(CCup& pCup, CCupRound* _LastCupRound, CNumericArray<ushort>& _DrawClubs)
{
	EXT_ASSERT(m_MatchList.GetSize() == 0 && m_ReplayList.GetSize() == 0);
	RandomNumber.Randomize();					// Set random seed

    CNumericArray<ushort> Losers;
	GetDrawClubs(_DrawClubs, Losers, pCup, _LastCupRound);
	EXT_ASSERT(_DrawClubs.GetSize() == m_NumberOfClubsInRound);
    // Shuffle the clubs
	if (Is2Regions() == false)
	{
		EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
        _DrawClubs.DoShuffleArray();
		EXT_ASSERT(_DrawClubs.CheckForDuplicates() == false);
	}
	else
	{
		// is seperate draws for more than one group
		int DrawPos = m_NumberOfClubsInRound / 2;
		for (uint LoopCount = 0; LoopCount < m_NumberOfClubsInRound; LoopCount += DrawPos)
		{
            _DrawClubs.DoShuffleArray(LoopCount, LoopCount + DrawPos - 2);
		}
	}
     // Display Draw Results.
    DoDisplayDraw(_DrawClubs, pCup, false);
    DoCreateMatchList(pCup, _DrawClubs);
}



/*------------------------------------------------------------------------------
	Method:   	CCupRound::DisplayDraw
	Access:    	public 
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Parameter: 	CCup& pCup
	Parameter: 	bool IsContinentalCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoDisplayDraw(CNumericArray<ushort>& _DrawClubs, CCup& _Cup, bool _IsContinentalCup)
{
	if (WorldData().IsFastTest() == true || IsGroupMatch() == true)
	{
		return;
	}

	CExtString AwayStr;
	CString str;
	int ClubIndex;
	CString CupAndRoundNameStr = _Cup.CupAndRoundName(*this);
	for (uint LoopCount = 0; LoopCount < _DrawClubs.GetSize(); LoopCount++)
	{
		if (WorldData().GetClub(_DrawClubs[LoopCount]).IsUserControlled() == true)
		{
			if (LoopCount & 1)
			{
				// User is away team
				ClubIndex = LoopCount - 1;
			}
			else
			{
				// Got a home tie
				ClubIndex = LoopCount;
			}
			if (_DrawClubs.GetSize() != 2)
			{
				// Display the fixture involving the human manager
				str = WorldData().GetClub(_DrawClubs[ClubIndex]).GetName() + _T(" - ") + WorldData().GetClub(_DrawClubs[ClubIndex + 1]).GetName();
				if (IsFirstLeg() == true)
				{
					AwayStr.Format(IDS_CUPOPPONENTS2LEGS, CupAndRoundNameStr, str,
								   LongMatchDateStr(), _Cup.GetRounds().GetNextRound(this)->LongMatchDateStr());
				}
				else
				{
					AwayStr.Format(IDS_CUPOPPONENTS, CupAndRoundNameStr, str, LongMatchDateStr());
				}
			}
			else
			{
				AwayStr.Format(IDS_CUPOPPONENTS, CupAndRoundNameStr, str, LongMatchDateStr());
			}
			WorldData().GetClub(_DrawClubs[ClubIndex]).AddToClubNews(AwayStr);
			WorldData().GetClub(_DrawClubs[ClubIndex + 1]).AddToClubNews(AwayStr);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::CreateMatchListAndRelatedRounds
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	CNumericArray<ushort>& aDrawClubs
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoCreateMatchListAndRelatedRounds(CCup& _Cup, CNumericArray<ushort>& _DrawClubs)
{
	DoCreateMatchList(_Cup, _DrawClubs);
	// Create 2nd legs etc
	_Cup.DoCreateRelatedRounds(*this, _DrawClubs);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoCreateMatchList
	Access:    	public 
	Parameter: 	CCup & _Cup
	Parameter: 	CNumericArray<ushort> & _DrawClubs
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoCreateMatchList(CCup& _Cup, CNumericArray<ushort>& _DrawClubs)
{
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	CMatchInfo Match(VenueType(), _Cup.GetSubsSelect(), _Cup.GetSubsUse());
	if (VenueType() == CUPMATCHNEUTRALVENUE)
	{
		Match.SetStadiumID(_Cup.GetNeutralVenueStadiumID());
	}

	_DrawClubs.CheckForDuplicates();
	for (uint  ListPos = 0; ListPos < m_NumberOfClubsInRound; ListPos += 2 )
	{
		Match.DoInitialiseClubs(_DrawClubs[ListPos], _DrawClubs[ListPos + 1]);
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
		m_MatchList.Add(new CFixture(Match));
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoCopyDrawLeg2
	Access:    	public 
	Parameter: 	CCupRound & _LastCupRound
	Parameter: 	const byte _SubsSelect
	Parameter: 	const byte _SubsUse
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoCopyDrawLeg2(CCupRound& _LastCupRound, const byte _SubsSelect, const byte _SubsUse)
{
#ifdef	ACTIVEREFEREES
	int RefereeNumber = 0;
#endif
	// Transfer the leg 1 draw to the fixtures with home and away reversed
 	CMatchInfo Match(VenueType(), _SubsSelect, _SubsUse);
    for (int ListPos = 0; ListPos < _LastCupRound.GetMatchList().GetSize(); ListPos ++)
	{
		Match.DoInitialiseClubs(_LastCupRound.GetAwayTeam(ListPos), _LastCupRound.GetHomeTeam(ListPos));
#ifdef	ACTIVEREFEREES
		Match.SetReferee(RefereeNumber);
#endif
        m_MatchList.Add(new CFixture(Match));
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::DoEndOfSeason()
{
	m_MatchList.RemoveList();
	m_ReplayList.RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::Write(CDataFile* _pDataFile)
{
    _pDataFile->Write(&m_TitleID, sizeof CStringID);
    _pDataFile->Write(&m_NameID, sizeof CStringID);
    _pDataFile->Write(&m_ReplayNameID, sizeof CStringID);
    _pDataFile->Write(&m_NumberOfClubsInRound, sizeof m_NumberOfClubsInRound);
    _pDataFile->Write(&m_Type, sizeof m_Type);
    _pDataFile->Write(&m_NumberNonLeagueClubsToAdd, sizeof m_NumberNonLeagueClubsToAdd);
    _pDataFile->Write(&m_MatchDate, sizeof m_MatchDate);
    _pDataFile->Write(&m_DrawDate, sizeof m_DrawDate);
    _pDataFile->Write(&m_ReplayDate, sizeof m_ReplayDate);
#ifdef	ACTIVEREFEREES
    _pDataFile->Write(&m_MinimumRefereeRating, sizeof m_MinimumRefereeRating);
    _pDataFile->Write(&m_MaximumRefereeRating, sizeof m_MaximumRefereeRating);
#endif
	//DEBUG_OUTPUT(("%s"), (GetName()));
    m_MatchList.Write(_pDataFile);
    m_ReplayList.Write(_pDataFile);
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::Read(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_TitleID, sizeof CStringID);
    _pDataFile->Read(&m_NameID, sizeof CStringID);
    _pDataFile->Read(&m_ReplayNameID, sizeof CStringID);
    _pDataFile->Read(&m_NumberOfClubsInRound, sizeof m_NumberOfClubsInRound);
    _pDataFile->Read(&m_Type, sizeof m_Type);
    _pDataFile->Read(&m_NumberNonLeagueClubsToAdd, sizeof m_NumberNonLeagueClubsToAdd);
    _pDataFile->Read(&m_MatchDate, sizeof m_MatchDate);
    _pDataFile->Read(&m_DrawDate, sizeof m_DrawDate);
    _pDataFile->Read(&m_ReplayDate, sizeof m_ReplayDate);
#ifdef	ACTIVEREFEREES
    _pDataFile->Read(&m_MinimumRefereeRating, sizeof m_MinimumRefereeRating);
    _pDataFile->Read(&m_MaximumRefereeRating, sizeof m_MaximumRefereeRating);
#endif
	//DEBUG_OUTPUT(("%s"), (GetName()));
    m_MatchList.Read(_pDataFile);
    m_ReplayList.Read(_pDataFile);
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetHomeTeam
	Access:    	public 
	Parameter: 	const uint MatchNum
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::GetHomeTeam(const ushort _MatchNum)
{
	EXT_ASSERT(_MatchNum > -1 && _MatchNum < m_MatchList.GetSize());
    return m_MatchList[_MatchNum].GetHomeClubID();
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetAwayTeam
	Access:    	public 
	Parameter: 	const uint MatchNum
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::GetAwayTeam(const ushort _MatchNum)
{
	EXT_ASSERT(_MatchNum > -1 && _MatchNum < m_MatchList.GetSize());
    return m_MatchList[_MatchNum].GetAwayClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetHomeReplayTeam
	Access:    	public 
	Parameter: 	const uint MatchNum
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::GetHomeReplayTeam(const int _MatchNum)
{
	EXT_ASSERT(_MatchNum > -1 && _MatchNum < m_ReplayList.GetSize());
    return m_ReplayList[_MatchNum].GetHomeClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetAwayReplayTeam
	Access:    	public 
	Parameter: 	const uint MatchNum
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRound::GetAwayReplayTeam(const int _MatchNum)
{
	EXT_ASSERT(_MatchNum > -1 && _MatchNum < m_ReplayList.GetSize());
    return m_ReplayList[_MatchNum].GetAwayClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::SetRoundSize
	Access:    	public 
	Parameter: 	const byte NumInRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::SetRoundSize(const byte _NumInRound)
{
	EXT_ASSERT(_NumInRound > 0);
	m_NumberOfClubsInRound = _NumInRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::DrawTitleStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupRound::DrawTitleStr()
{
	CString str1;
	str1.Format(IDS_CUPDRAWTITLE, GetDrawName());
	return str1;
}


CString& CCupRound::GetDrawName()
{
	return m_DrawNameID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetTitle
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CCupRound::GetTitle()
{
	return m_TitleID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetName
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CCupRound::GetName()
{
	return m_NameID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::ReplayTitleStr
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CCupRound::GetReplayTitle()
{
	return m_ReplayNameID.GetStr();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::NumberClubGoals
	Access:    	public 
	Parameter: 	const ushort aClubID
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCupRound::NumberClubGoals(const ushort _ClubID)
{
	byte bRet = 0;
	for (int LoopCount = 0; LoopCount < m_MatchList.GetSize(); LoopCount++)
	{
        if (m_MatchList[LoopCount].GetHomeClubID() == _ClubID)
		{
			bRet = m_MatchList[LoopCount].GetNumberGoals(HOME);
			break;
		}
	 	if (m_MatchList[LoopCount].GetAwayClubID() == _ClubID)
		{
			bRet = m_MatchList[LoopCount].GetNumberGoals(AWAY);
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::BuildReplayDisplayList
	Access:    	public 
	Parameter: 	CSoccerResultList& ResultList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::BuildReplayDisplayList(CSoccerResultList& ResultList)
{
	BuildDisplayList(m_ReplayList, ResultList);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::BuildMatchDisplayList
	Access:    	public 
	Parameter: 	CSoccerResultList& ResultList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::BuildMatchDisplayList(CSoccerResultList& _ResultList)
{
	BuildDisplayList(m_MatchList, _ResultList);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::BuildDisplayList
	Access:    	public 
	Parameter: 	CFixtureList& GamesList
	Parameter: 	CSoccerResultList& ResultList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::BuildDisplayList(CFixtureList& _GamesList, CSoccerResultList& _ResultList)
{
	EXT_ASSERT(_ResultList.GetSize() == 0);
    CSoccerResult* pSoccerResult;
	_ResultList.SetSize(_GamesList.GetSize());
	for (int LoopCount = 0; LoopCount < _GamesList.GetSize(); LoopCount++)
	{
        _ResultList.SetAt(LoopCount, new CSoccerResult());
        pSoccerResult = _ResultList.GetAt(LoopCount);
		pSoccerResult->GetHomeClubDetails().SetClubDetails(_GamesList[LoopCount].GetHomeClubID());
		pSoccerResult->GetAwayClubDetails().SetClubDetails(_GamesList[LoopCount].GetAwayClubID());
		if (_GamesList[LoopCount].GetMatchLength() > 0)
		{
            pSoccerResult->SetResult(&_GamesList[LoopCount]);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetMatchDetails
	Access:    	public 
	Parameter: 	CCup & _Cup
	Parameter: 	const ushort _MatchNum
	Parameter: 	CMatchInfo & _MatchDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::GetMatchDetails(CCup& _Cup, const ushort _MatchNum, CMatchInfo& _MatchDetails)
{
	EXT_ASSERT(_MatchNum < m_MatchList.GetSize ());
	GetMatchDetails (_Cup, m_MatchList.GetAt(_MatchNum), _MatchDetails);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetMatchDetails
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	CFixture* pFixture
	Parameter: 	CMatchInfo& MatchDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::GetMatchDetails(CCup& _Cup, CFixture* _Fixture, CMatchInfo& _MatchDetails)
{
    _MatchDetails.SetFixture(_Fixture);
	_Fixture->GetMatchDetails(_MatchDetails);
	if (IsNeutralVenue() == true)
	{
		_MatchDetails.SetCompetitionType(CUPMATCHNEUTRALVENUE);
		_MatchDetails.SetStadiumID(_Cup.GetNeutralVenueStadiumID());
	}
	else
	{
		_MatchDetails.SetCompetitionType(CUPMATCH);
		_MatchDetails.SetStadiumID(_Fixture->GetClub(HOME).GetStadiumID());
	}
	_MatchDetails.SetCompetitionTitle(m_TitleID.GetStr());
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetMatchDetailsPtr
	Access:    	public 
	Parameter: 	const ushort MatchNum
	Returns:   	CFixture*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CCupRound::GetMatchDetailsPtr(const ushort MatchNum)
{
	if (MatchNum < m_MatchList.GetSize())
	{
        return &m_MatchList[MatchNum];
	}
	else
	{
		return null;
	}
 }



/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetReplayDetails
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Parameter: 	CMatchInfo & _MatchDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::GetReplayDetails(CFixture* _Fixture, CMatchInfo& _MatchDetails)
{
    _MatchDetails.SetFixture(_Fixture);
	_MatchDetails.GetFixture()->GetMatchDetails(_MatchDetails);
	_MatchDetails.SetCompetitionTitle(m_ReplayNameID.GetStr());
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetReplayDetailsPtr
	Access:    	public 
	Parameter: 	const ushort MatchNum
	Returns:   	CFixture*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CCupRound::GetReplayDetailsPtr(const ushort MatchNum)
{
	EXT_ASSERT(MatchNum < m_ReplayList.GetSize());
    return &m_ReplayList[MatchNum];
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRound::ClubMatchToday
	Access:    	public 
	Parameter: 	const CMatchTime& CurrentDate
	Parameter: 	CCup& pCup
	Parameter: 	const ushort Club
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:	If the specified club has a game this turn then fill in the details
------------------------------------------------------------------------------*/
bool CCupRound::IsClubMatchToday(const CCalendar& _CurrentDate, CCup& _Cup, const ushort _ClubID, CMatchInfo& _Match)
{
	bool bRet = false;
    if (m_MatchDate == _CurrentDate.GetMatchDay())
	{
		for (int LoopCount = 0; LoopCount < m_MatchList.GetSize(); LoopCount++)
		{
            if (_ClubID == m_MatchList[LoopCount].GetHomeClubID() || _ClubID == m_MatchList[LoopCount].GetAwayClubID())
			{
				GetMatchDetails(_Cup, &m_MatchList[LoopCount], _Match);
				bRet = true;
				break;
			}
		}
	}
	// Now check if a replay
    if (m_ReplayDate == _CurrentDate.GetMatchDay())
	{
		for (int LoopCount = 0; LoopCount < m_ReplayList.GetSize(); LoopCount++)
		{
            if (_ClubID == m_ReplayList[LoopCount].GetHomeClubID() || _ClubID == m_ReplayList[LoopCount].GetAwayClubID())
			{
				GetReplayDetails(&m_ReplayList[LoopCount], _Match);
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


CCupRound* CCupRound::RoundToday(const CCalendar& _CurrentDate)
{
	bool bRet = false;
	if (m_MatchDate == _CurrentDate.GetMatchDay() || m_ReplayDate == _CurrentDate.GetMatchDay())
	{
		return this;
	}
	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::ToPlayAfterWeek
	Access:    	public 
	Parameter: 	const byte MatchNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCupRound::ToPlayAfterWeek(const byte MatchNumber)
{
	if ((m_DrawDate > MatchNumber && m_DrawDate != IGNOREDATE) || 
		(m_MatchDate > MatchNumber && m_MatchDate != IGNOREDATE) || 
		(m_ReplayDate > MatchNumber && m_ReplayDate != IGNOREDATE))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::MoveMatchToReplay
	Access:    	public 
	Parameter: 	const ushort MatchNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::MoveMatchToReplay(const ushort MatchNumber)
{
	m_ReplayList.Add(&m_MatchList[MatchNumber]);
	m_MatchList.RemoveAt(MatchNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::MoveLastMatchToReplay
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::MoveLastMatchToReplay()
{
	m_ReplayList.Add(&m_MatchList[m_MatchList.GetUpperBound()]);
	m_MatchList.RemoveAt(m_MatchList.GetUpperBound());
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::ReplayListSize
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCupRound::GetReplayListSize()
{
	return m_ReplayList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::MatchListSize
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCupRound::MatchListSize()
{
	return m_MatchList.GetSize();
}


#ifdef CUPGROUPS
/*------------------------------------------------------------------------------
	Method:   	CCupRound::GetGroupClubNumbers
	Access:    	public 
	Parameter: 	int GroupNumber
	Parameter: 	CNumericArray<ushort> * ClubNumbers
	Parameter: 	CCupRound * pSecondCupRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::GetGroupClubNumbers(const int _GroupNumber, CNumericArray<ushort>* _ClubIDList, CCupRound* _SecondCupRound)
{
	int FirstClubNumber = _GroupNumber * GetNumberOfClubsInGroup();
	int MatchNumber;
	_ClubIDList->RemoveAll();

	if (GetNumberOfClubsInGroup() == 5)
	{
		FirstClubNumber = _GroupNumber * 2;
		_ClubIDList->Add(m_MatchList.GetAt(_GroupNumber * 2)->GetHomeClubID());
		_ClubIDList->Add(m_MatchList.GetAt(_GroupNumber * 2)->GetAwayClubID());
		_ClubIDList->Add(m_MatchList.GetAt(_GroupNumber * 2 + 1)->GetHomeClubID());
		_ClubIDList->Add(m_MatchList.GetAt(_GroupNumber * 2 + 1)->GetAwayClubID());
		// 5th club had a bye in first round
		_ClubIDList->Add(_SecondCupRound->GetMatchList().GetAt(_GroupNumber * 2)->GetAwayClubID());
	}
	else
	{
		for (int LoopCount = 0; LoopCount < GetNumberOfClubsInGroup(); LoopCount += 2)
		{
			MatchNumber = (FirstClubNumber >> 1) + (LoopCount >> 1);
			if (MatchNumber < m_MatchList.GetSize())
			{
				_ClubIDList->Add(m_MatchList.GetAt(MatchNumber)->GetHomeClubID());
				_ClubIDList->Add(m_MatchList.GetAt(MatchNumber)->GetAwayClubID());
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRound::NumberMatchesinGroup
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCupRound::GetNumberOfGroupMatches(void)
{
	return m_NumberOfClubsInGroup >> 1;
}

#endif


#ifdef LATESTSCORES
/*------------------------------------------------------------------------------
	Method:   	CCupRound::BuildCurrentNonUserFixtureList
	Access:    	public 
	Parameter: 	CFixtureList* LatestScores
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRound::BuildCurrentNonUserFixtureList(CFixtureList& _LatestScores)
{
	if (m_MatchDate == WorldData().GetCurrentDate().GetMatchDay())
	{
		m_MatchList.BuildCurrentNonUserFixtureList(_LatestScores);
	}
	else
	{
		m_ReplayList.BuildCurrentNonUserFixtureList(_LatestScores);
	};
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::~CCupRoundList
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRoundList::~CCupRoundList()
{
	delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	// Read in the rounds
	ReadSizeFromResource(_Resource);
	m_pList = new CCupRound[CExtArray::GetSize()];
#ifdef DEBUG_LOAD_CUP_ROUNDS
	DEBUG_OUTPUT(("%d Rounds found"), (CExtArray::GetSize()));
#endif
	for (uint ToFind = 0; ToFind < CExtArray::GetSize(); ToFind ++)
	{
#ifdef DEBUG_LOAD_CUP_ROUNDS
		DEBUG_OUTPUT(("Round %d"), (ToFind));
#endif
        m_pList[ToFind].DoInitialiseFromResource(_Resource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::pLastRound
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CCupRoundList::GetLastRound()
{
	return operator[](CExtArray::GetUpperBound());
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    CExtArray::WriteSize(_pDataFile);
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_pList[LoopCount].Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CExtArray::ReadSize(_pDataFile);
	EXT_ASSERT(CExtArray::GetSize() < 32);
	delete[] m_pList;
	m_pList = new CCupRound[GetSize()];
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		m_pList[LoopCount].Read(_pDataFile);
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::operator[]
	Access:    	public 
	Parameter: 	ushort x
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CCupRoundList::operator [] (ushort x)
{
	EXT_ASSERT(x < GetSize());
	return *(static_cast<CCupRound*>(&m_pList[x]));
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::DoEndOfSeason()
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_pList[LoopCount].DoEndOfSeason();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::DoFindCupRoundFromFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCupRoundList::DoFindCupRoundFromFixture(CFixture* _Fixture)
{
    CCupRound* pCupRound = null;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (m_pList[LoopCount].IsFixtureInRound(_Fixture) == true)
		{
            pCupRound = &m_pList[LoopCount];
            break;
        }
    }
    return pCupRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::pNextRound
	Access:    	public 
	Parameter: 	CCupRound* apCupRound
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCupRoundList::GetNextRound(CCupRound* _CupRound)
{
    CCupRound* pFirstCupRound = null;
    for (uint LoopCount = 0; LoopCount < GetUpperBound(); LoopCount++)
	{
        if (&m_pList[LoopCount] == _CupRound)
		{
            pFirstCupRound = &m_pList[LoopCount + 1];
            break;
        }
    }
    EXT_ASSERT_POINTER(pFirstCupRound, CCupRound);
    return pFirstCupRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::DoFindRoundNumber
	Access:    	public 
	Parameter: 	CCupRound* apCupRound
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRoundList::DoFindRoundNumber(CCupRound* _CupRound)
{
	EXT_ASSERT_POINTER(_CupRound, CCupRound);
	ushort nRound = wNOTFOUND;
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (_CupRound == &(operator [](LoopCount)))
		{
			nRound = LoopCount;
			break;
		}
	}
    EXT_ASSERT(nRound != wNOTFOUND);
	return nRound;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::SetRoundSize
	Access:    	public 
	Parameter: 	ushort RoundNum
	Parameter: 	const byte NumInRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::SetRoundSize(ushort RoundNum, const byte NumInRound)
{
	EXT_ASSERT(RoundNum < GetSize());
	operator [](RoundNum).SetRoundSize(NumInRound);
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::TotalRoundsWithReplaysExisting
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCupRoundList::TotalRoundsWithReplaysExisting()
{
	ushort iRet = 0;
	// Check all rounds
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        iRet += (m_pList[LoopCount].GetReplayListSize() > 0);
	}
	return iRet;
}


#ifdef CUPGROUPS
/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::BuildGroupTables
	Access:    	public 
	Parameter: 	CCup& pCup
	Parameter: 	int GroupNumber
	Parameter: 	CLeagueTableList* PointsTable
	Parameter: 	int NumRounds
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupRoundList::DoBuildGroupTables(CCup& _Cup, int _GroupNumber, CLeagueTableList& _PointsTable, int _NumRounds /*= 0*/)
{
    CCupRound* pCupRound = &DoFindFirstGroupMatch();
    ushort RoundNumber = DoFindRoundNumber(pCupRound);
	int NumberMatchesinGroup = (pCupRound->GetNumberOfGroupMatches());
	int FirstMatch = _GroupNumber * NumberMatchesinGroup;
	if (_NumRounds == 0)
	{
		_NumRounds = NumberOfRoundsInGroupStage();
	}

	CMatchInfo MatchDetails;
	for (int LoopCount = RoundNumber; LoopCount < RoundNumber + _NumRounds; LoopCount++)
	{
		for (int MatchNumber = FirstMatch; MatchNumber < NumberMatchesinGroup + FirstMatch; MatchNumber++)
		{
			if (m_pList[LoopCount].HaveAllRoundMatchesBeenPlayed() == true)
			{
				m_pList[LoopCount].GetMatchDetails(_Cup, MatchNumber, MatchDetails);
				_PointsTable.DoUpdate(MatchDetails);
			}
		}
	}
#ifdef 	PERCENTAGEPOINTSFORPLAY0FFS
	_PointsTable->AddPercentagePointsFromDivision(_Cup.GetPercentagePointsForPlay0ffs());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::NumberOfRoundsInGroupStage
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:

	NB. Assumes only 1 set of group matches in cup
------------------------------------------------------------------------------*/
ushort CCupRoundList::NumberOfRoundsInGroupStage()
{
	ushort Count = 0;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        Count += (operator [](LoopCount).IsGroupMatch() == true);
    }
    return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CCupRoundList::DoFindFirstGroupMatch
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:	Find first group match

	NB. Assumes only 1 set of group matches in cup
------------------------------------------------------------------------------*/
CCupRound& CCupRoundList::DoFindFirstGroupMatch()
{
    CCupRound* pFirstCupRound = null;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (operator [](LoopCount).IsGroupMatch() == true)
		{
            pFirstCupRound = &operator [](LoopCount);
            break;
        }
    }
    return *pFirstCupRound;
}


 // Find first group match
 // NB. Assumes only 1 set of group matches in cup
 // -------------------------------------------------------------------------------------------------------------------------
ushort CCupRoundList::FindFirstGroupMatchNumber()
{
	ushort nRound = wNOTFOUND;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (operator [](LoopCount).IsGroupMatch() == true)
		{
			nRound = LoopCount;
            break;
        }
    }
    EXT_ASSERT(nRound != wNOTFOUND);
    return nRound;
}
#endif
