/*

	File:- MatchEvent.cpp

	Match Event class


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "MatchEvent.h"

IMPLEMENT_FDMEMORY_BINALLOC(CMatchEvent);

/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::CMatchEvent
	Access:    	public
	Parameter: 	const byte _Time
	Parameter: 	const byte _Type
	Parameter: 	const CPersonName _Name
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchEvent::CMatchEvent(const byte _Time, const byte _Type, const CPersonName _Name)
	: m_Time(_Time)
    , m_Type(_Type)
    , m_PlayerName(_Name)
{
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEvent::DoResetVariables()
{
    m_Time = 0;
    m_Type = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::GetEventType
	Access:    	public
	Parameter: 	void
	Returns:   	eMatchEvent
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eMatchEvent CMatchEvent::GetEventType() const
{
	return (eMatchEvent)m_Type;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::GetTime
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEvent::GetTime() const
{
	return m_Time;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::GetText
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEvent::GetText()
{
	return m_PlayerName.GetName();
}

const CPersonName &CMatchEvent::GetName() const
{
	return m_PlayerName;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::GetEventGoalTimeStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEvent::GetEventGoalTimeStr()
{
    CString str1;
	if (IsOwnGoal() == true)
	{
    str1.Format(IDS_OWNGOALEVENTTIME, m_PlayerName.GetName(), GetTime());
	}
	else if (IsPenalty() == true)
	{
    str1.Format(IDS_PENALTYGOALEVENTTIME, m_PlayerName.GetName(), GetTime());
	}
	else
	{
    str1.Format(IDS_GOALEVENTTIME, m_PlayerName.GetName(), GetTime());
	}
	return str1;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::GetEventDiscTimeStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEvent::GetEventDiscTimeStr()
{

    CString str1;
  str1.Format(IDS_DISCEVENTTIME, m_PlayerName.GetName(), GetTime());
	return str1;

}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsBookingEvent
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsBookingEvent()
{
	switch(GetEventType())
	{
		case HOMEBOOKING:
		case AWAYBOOKING:
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsDiscEvent
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsDiscEvent()
{
	switch(GetEventType())
	{
		case HOMEBOOKING:
		case AWAYBOOKING:
        case HOMESENDINGOFF:
		case AWAYSENDINGOFF:
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsGoalEvent
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsGoalEvent()
{
	switch(GetEventType())
	{
		case HOMEGOAL:
		case AWAYGOAL:
		case HOMEPENALTYGOAL:
		case AWAYPENALTYGOAL:
        case HOMEOWNGOAL:
		case AWAYOWNGOAL:                           // 9
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsHomeTeamEvent
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsHomeTeamEvent()
{
	switch (GetEventType())
	{
		case HOMEBOOKING:
		case HOMEGOAL:
		case HOMESENDINGOFF:
		case HOMEOWNGOAL:
		case HOMEPENALTYGOAL:
		case HOMEPENALTYSHOOTOUTGOAL:
		case HOMEPENALTYSHOOTOUTMISS:
		case HOMEPENALTYSHOOTOUTSAVED:
			return true;

		case AWAYGOAL:
		case AWAYBOOKING:
		case AWAYSENDINGOFF:
		case AWAYOWNGOAL:
		case AWAYPENALTYGOAL:
		case AWAYPENALTYSHOOTOUTGOAL:
		case AWAYPENALTYSHOOTOUTMISS:
		case AWAYPENALTYSHOOTOUTSAVED:
			return false;

		default:
			EXT_ASSERT(false);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsOwnGoal
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsOwnGoal()
{
	switch(GetEventType())
	{
		case HOMEOWNGOAL:
		case AWAYOWNGOAL:
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsPenalty
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsPenalty()
{
	switch(GetEventType())
	{
		case HOMEPENALTYGOAL:
		case AWAYPENALTYGOAL:
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsPenaltyShootOutEvent
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsPenaltyShootOutEvent()
{
	switch(GetEventType())
	{
		case HOMEPENALTYSHOOTOUTGOAL:
		case AWAYPENALTYSHOOTOUTGOAL:
		case HOMEPENALTYSHOOTOUTSAVED:
		case AWAYPENALTYSHOOTOUTSAVED:
		case HOMEPENALTYSHOOTOUTMISS:
		case AWAYPENALTYSHOOTOUTMISS:
			return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEvent::IsSendingOffEvent
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEvent::IsSendingOffEvent()
{
	switch(GetEventType())
	{
        case HOMESENDINGOFF:
		case AWAYSENDINGOFF:
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Returns whether or not the event should be saved. In order to conserve
// saved game space, not all events are saved.
//////////////////////////////////////////////////////////////////////////
bool CMatchEvent::ShouldBeSaved() const
{
	switch (GetEventType())
	{
	case HOMEGOAL:
	case AWAYGOAL:
	case HOMEOWNGOAL:
	case AWAYOWNGOAL:
	case HOMEPENALTYGOAL:
	case AWAYPENALTYGOAL:
	case HOMEBOOKING:
	case AWAYBOOKING:
	case HOMESENDINGOFF:
	case AWAYSENDINGOFF:
	case HOMEFREEKICKGOAL:
	case AWAYFREEKICKGOAL:
	case HOMEFREEKICKGOALYELLOWGOALKEEPER:
	case AWAYFREEKICKGOALYELLOWGOALKEEPER:
	case HOMEFREEKICKGOALYELLOWOUTFIELDPLAYER:
	case AWAYFREEKICKGOALYELLOWOUTFIELDPLAYER:
	case HOMEFREEKICKGOALREDGOALKEEPER:
	case AWAYFREEKICKGOALREDGOALKEEPER:
	case HOMEFREEKICKGOALREDOUTFIELDPLAYER:
	case AWAYFREEKICKGOALREDOUTFIELDPLAYER:
	case HOMETOPCORNERGOALSHOT:
	case AWAYTOPCORNERGOALSHOT:
	case HOMETOPCORNERGOALHEADER:
	case AWAYTOPCORNERGOALHEADER:
	case HOMEBOTTOMCORNERGOALSHOT:
	case AWAYBOTTOMCORNERGOALSHOT:
	case HOMEBOTTOMCORNERGOALHEADER:
	case AWAYBOTTOMCORNERGOALHEADER:
	case HOMEGOALSHOT:
	case AWAYGOALSHOT:
	case HOMEGOALHEADER:
	case AWAYGOALHEADER:
	case HOMEPENALTYGOALYELLOWGOALKEEPER:
	case AWAYPENALTYGOALYELLOWGOALKEEPER:
	case HOMEPENALTYGOALREDGOALKEEPER:
	case AWAYPENALTYGOALREDGOALKEEPER:
	case HOMEPENALTYGOALYELLOWOUTFIELDPLAYER:
	case AWAYPENALTYGOALYELLOWOUTFIELDPLAYER:
	case HOMEPENALTYGOALREDOUTFIELDPLAYER:
	case AWAYPENALTYGOALREDOUTFIELDPLAYER:
	case HOMEPENALTYSHOOTOUTGOAL:
	case AWAYPENALTYSHOOTOUTGOAL:
		return true;
	default:
		return false;
	}
}

void CMatchEvent::SetTime(const byte bTime)
{
	m_Time = bTime;
}

void CMatchEvent::SetEventType(const byte bEventType)
{
	m_Type = bEventType;
}

void CMatchEvent::SetName(const ushort usForename, const ushort usSurname)
{
	m_PlayerName.SetForename(usForename);
	m_PlayerName.SetSurname(usSurname);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMatchEventList::~CMatchEventList()
{
	RemoveList();
}

/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::DoCountEvents
	Access:    	public
	Parameter: 	eMatchEvent _EventType
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::DoCountEvents(eMatchEvent _EventType) const
{
	int iRet = 0;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
		if (GetAt(ListPos)->GetEventType() == _EventType)
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomeNumberPlayersBooked
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::HomeNumberPlayersBooked() const
{
	return DoCountEvents(HOMEBOOKING);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayNumberPlayersBooked
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::AwayNumberPlayersBooked() const
{
	return DoCountEvents(AWAYBOOKING);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomeNumberPlayersSentOff
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::HomeNumberPlayersSentOff() const
{
	return DoCountEvents(HOMESENDINGOFF);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayNumberPlayersSentOff
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::AwayNumberPlayersSentOff() const
{
	return DoCountEvents(AWAYSENDINGOFF);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberHomeOwnGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberHomeOwnGoals() const
{
	return DoCountEvents(HOMEOWNGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberAwayOwnGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberAwayOwnGoals() const
{
	return DoCountEvents(AWAYOWNGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberHomePenaltyGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberHomePenaltyGoals() const
{
	return DoCountEvents(HOMEPENALTYGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberAwayPenaltyGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberAwayPenaltyGoals() const
{
	return DoCountEvents(AWAYPENALTYGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberHomePenaltyShootOutGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberHomePenaltyShootOutGoals() const
{
	if (m_bIsSummary == false)
		return DoCountEvents(HOMEPENALTYSHOOTOUTGOAL);
	return m_SummaryHomePenaltyShootoutGoals;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::PenaltyShootoutWinner
	Access:    	public
	Parameter: 	void
	Returns:   	eWhichTeam - home or away
	Qualifier:
	Purpose:	Which side won the penalty shoot out
------------------------------------------------------------------------------*/
eWhichTeam CMatchEventList::PenaltyShootoutWinner() const
{
	EXT_ASSERT(WasPenaltyShootOut() == true);
    if (NumberHomePenaltyShootOutGoals() > NumberAwayPenaltyShootOutGoals())
	{
        return HOME;
    }
    else
	{
        return AWAY;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::PenaltyShootoutWinnerStr
	Access:    	public
	Parameter: 	const CString _HomeClubName
	Parameter: 	const CString _AwayClubName
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::PenaltyShootoutWinnerStr(const CString _HomeClubName, const CString _AwayClubName) const
{
	EXT_ASSERT(WasPenaltyShootOut() == true);
	CString str;
	if (PenaltyShootoutWinner() == HOME)
	{
		// Home Won Penalty Shootout
    str.Format(IDS_PENALTYWINNER, _HomeClubName, NumberHomePenaltyShootOutGoals(), NumberAwayPenaltyShootOutGoals());
	}
	else
	{
		// Away Won Penalty Shootout
    str.Format(IDS_PENALTYWINNER, _AwayClubName, NumberAwayPenaltyShootOutGoals(), NumberHomePenaltyShootOutGoals());
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::WasPenaltyShootOut
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEventList::WasPenaltyShootOut() const
{
	if (m_bIsSummary == false)
	{
		if (NumberHomePenaltyShootOutGoals() > 0 || NumberAwayPenaltyShootOutGoals() > 0)
		{
			return true;
		}
		return false;
	}

	return m_bSummaryPenaltyShootout;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberAwayPenaltyShootOutGoals
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberAwayPenaltyShootOutGoals() const
{
	if (m_bIsSummary == false)
		return DoCountEvents(AWAYPENALTYSHOOTOUTGOAL);
	return m_SummaryAwayPenaltyShootoutGoals;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomePlayerScoredStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::HomePlayerScoredStr(const ushort _EventNumber) const
{
    CExtString sRet;
	CString strGoalType;
    ushort Count = 0;
	CMatchEvent* pEvent;

    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        switch(pEvent->GetEventType())
		{
            case HOMEOWNGOAL:
                if (Count == _EventNumber)
				{
          strGoalType.LoadString(IDS_OG);
                    sRet = pEvent->GetEventGoalTimeStr() + strGoalType;
                }
				Count++;
                break;
            case HOMEPENALTYGOAL:
                if (Count == _EventNumber)
				{
          strGoalType.LoadString(IDS_PEN);
                    sRet = pEvent->GetEventGoalTimeStr() + strGoalType;
                }
				Count++;
                break;
            case HOMEGOAL:
                if (Count == _EventNumber)
				{
                    sRet = pEvent->GetEventGoalTimeStr();
                }
				Count++;
                break;
        }
    }
    return sRet;
}

/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayPlayerScoredStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::AwayPlayerScoredStr(const ushort _EventNumber) const
{
    CExtString sRet;
	CExtString StrGoalType;
    ushort Count = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        switch(pEvent->GetEventType())
		{
            case AWAYOWNGOAL:
                if (Count == _EventNumber)
				{
          StrGoalType.LoadString(IDS_OG);
                    sRet = pEvent->GetEventGoalTimeStr() + StrGoalType;
                }
				Count++;
                break;
            case AWAYPENALTYGOAL:
                if (Count == _EventNumber)
				{
          StrGoalType.LoadString(IDS_PEN);
                    sRet = pEvent->GetEventGoalTimeStr() + StrGoalType;
                }
				Count++;
                break;
            case AWAYGOAL:
                if (Count == _EventNumber)
				{
                    sRet = pEvent->GetEventGoalTimeStr();
                }
				Count++;
                break;
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomePlayerBookedStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::HomePlayerBookedStr(const ushort _EventNumber) const
{
    CExtString sRet;
    ushort Count = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        if (pEvent->GetEventType() == HOMEBOOKING)
		{
            if (Count == _EventNumber)
			{
                sRet = pEvent->GetEventDiscTimeStr();
                break;
            }
            Count++;
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayPlayerBookedStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::AwayPlayerBookedStr(const ushort _EventNumber) const
{
    CExtString sRet;
    ushort Count = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        if (pEvent->GetEventType() == AWAYBOOKING)
		{
            if (Count == _EventNumber)
			{
                sRet = pEvent->GetEventDiscTimeStr();
                break;
            }
            Count++;
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomePlayerSentOffStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::HomePlayerSentOffStr(const ushort _EventNumber) const
{
    CExtString sRet;
    int Count = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        if (pEvent->GetEventType() == HOMESENDINGOFF)
		{
            if (Count == _EventNumber)
			{
                sRet = pEvent->GetEventDiscTimeStr();
                break;
            }
            Count++;
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::IsAwayEvent
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
/*
bool CMatchEventList::IsAwayEvent(const ushort _EventNumber)
{
	bool bRet = false;
	switch(GetAt(_EventNumber)->GetEventType())
	{
		case AWAYGOAL:
		case AWAYBOOKING:
		case AWAYSENDINGOFF:
		case AWAYOWNGOAL:
		case AWAYPENALTYGOAL:
		case AWAYOFFSIDEATTEMPT:
		case AWAYFOULPLAYERATTEMPT:
		case AWAYINJUREPLAYERATTEMPT:
		case AWAYSHOTATTEMPT:
		case AWAYMISSATTEMPT:
		case AWAYFREEKICKATTEMPT:
		case AWAYCORNERATTEMPT:
		case AWAYSUBATTEMPT:
			bRet = true;
			break;
	}
	return bRet;
}
*/


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::IsHomeEvent
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
/*
bool CMatchEventList::IsHomeEvent(const ushort _EventNumber)
{
	bool bRet = true;
	switch(GetAt(_EventNumber)->GetEventType())
	{
		case AWAYGOAL:
		case AWAYBOOKING:
		case AWAYSENDINGOFF:
		case AWAYOWNGOAL:
		case AWAYPENALTYGOAL:
		case AWAYOFFSIDEATTEMPT:
		case AWAYFOULPLAYERATTEMPT:
		case AWAYINJUREPLAYERATTEMPT:
		case AWAYSHOTATTEMPT:
		case AWAYMISSATTEMPT:
		case AWAYFREEKICKATTEMPT:
		case AWAYCORNERATTEMPT:
		case AWAYSUBATTEMPT:
			bRet = false;
			break;
	}
	return bRet;
}
*/


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayPlayerSentOffStr
	Access:    	public
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::AwayPlayerSentOffStr(const ushort _EventNumber) const
{
    CExtString sRet;
    int Count = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        if (pEvent->GetEventType() == AWAYSENDINGOFF)
		{
            if (Count == _EventNumber)
			{
                sRet = pEvent->GetEventDiscTimeStr();
                break;
            }
            Count++;
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomeScore
	Access:    	public
	Parameter: 	const uint _Time
	Returns:   	uint - the score of the home team at the requested time
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::HomeScore(const uint _Time) const
{
	if (m_bIsSummary == false)
	{
		uint iRet = 0;
		CMatchEvent* pEvent;
		for (int ListPos = 0; ListPos < GetSize(); ListPos++)
		{
			pEvent = GetAt(ListPos);
			iRet += (pEvent->GetTime() <= _Time && (pEvent->GetEventType() == HOMEGOAL
												|| pEvent->GetEventType() == AWAYOWNGOAL
												|| pEvent->GetEventType() == HOMEPENALTYGOAL));
		}
		return iRet;
	}

	return m_SummaryHomeGoals;
 }


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayScore
	Access:    	public
	Parameter: 	const uint _Time
	Returns:   	uint - the score of the away team at the requested time
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::AwayScore(const uint _Time) const
{
	if (m_bIsSummary == false)
	{
		uint iRet = 0;
		CMatchEvent* pEvent;
		for (int ListPos = 0; ListPos < GetSize(); ListPos++)
		{
			pEvent = GetAt(ListPos);
			iRet += (pEvent->GetTime() <= _Time && (pEvent->GetEventType() == AWAYGOAL
												|| pEvent->GetEventType() == HOMEOWNGOAL
												|| pEvent->GetEventType() == AWAYPENALTYGOAL));
		}
		return iRet;
	}

	return m_SummaryAwayGoals;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HalfTimeHomeGoals
	Access:    	public
	Parameter: 	void
	Returns:   	uint - the number of goals scored in the first half by the home team
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::HalfTimeHomeGoals() const
{
	uint iRet = 0;
	CMatchEvent* pEvent;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= HALFTIME && (pEvent->GetEventType() == HOMEGOAL
                                                  || pEvent->GetEventType() == AWAYOWNGOAL
                                                  || pEvent->GetEventType() == HOMEPENALTYGOAL));
	}
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HalfTimeAwayGoals
	Access:    	public
	Parameter: 	void
	Returns:   	uint - the number of goals scored in the first half by the home team
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::HalfTimeAwayGoals() const
{
	uint iRet = 0;
	CMatchEvent* pEvent;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= HALFTIME && (pEvent->GetEventType() == AWAYGOAL
                                                  || pEvent->GetEventType() == HOMEOWNGOAL
                                                  || pEvent->GetEventType() == AWAYPENALTYGOAL));
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HasGoalBeenScored
	Access:    	public
	Parameter: 	const uint _Time
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchEventList::HasGoalBeenScored(const uint _Time) const
{
	bool bRet = false;
	CMatchEvent* pEvent;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++ )
	{
        pEvent = GetAt(ListPos);
		if (pEvent->GetTime() == _Time)
		{
			switch(pEvent->GetEventType())
			{
				case HOMEOWNGOAL:
				case HOMEPENALTYGOAL:
				case HOMEGOAL:
				case AWAYOWNGOAL:
				case AWAYPENALTYGOAL:
				case AWAYGOAL:
					bRet = true;
					break;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::SortByType
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventList::SortByType()
{
	// Bubble Sort on Type
	CMatchEvent* TempEvent;
	bool swap = true;

	while(swap == true)
	{
		swap = false;
		for (int ListPos = 0; ListPos < GetSize() - 1; ListPos++)
		{
            if (GetAt(ListPos)->GetEventType() > GetAt(ListPos + 1)->GetEventType())
			{
                TempEvent = GetAt(ListPos);
                SetAt(ListPos, GetAt(ListPos + 1));
				SetAt(ListPos + 1, TempEvent);
				swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::SortByTime
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Bubble Sort on Time
------------------------------------------------------------------------------*/
void CMatchEventList::SortByTime()
{
	CMatchEvent* TempEvent;
	bool swap = true;

	while(swap == true)
	{
		swap = false;
		for (int ListPos = 0; ListPos < GetSize() - 1; ListPos++)
		{
            if (GetAt(ListPos)->GetTime() > GetAt(ListPos + 1)->GetTime())
			{
                TempEvent = GetAt(ListPos);
                SetAt(ListPos, GetAt(ListPos + 1));
				SetAt(ListPos + 1, TempEvent);
				swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::Write
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventList::WriteTime(CDataFile* _pDataFile)
{
	if (m_bIsSummary == false)
	{
		byte HowMany = 0;

		for (int i = 0; i < GetSize(); i++)
		{
			if (GetAt(i)->ShouldBeSaved() == true)
				HowMany++;
		}

		EXT_ASSERT(HowMany < 150); // Unusual number of match events
		byte bFlag = 0;
		_pDataFile->WriteBits(bFlag, 1);
		_pDataFile->WriteBits(HowMany, 8);

		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (GetAt(ListPos)->ShouldBeSaved() == false)
				continue;

			CMatchEvent *pEvent = GetAt(ListPos);
			_pDataFile->WriteBits(pEvent->GetTime(), 8);
		}
	}
	else
	{
		byte bFlag = 1;
		_pDataFile->WriteBits(bFlag, 1);
		_pDataFile->WriteBits(m_bSummaryPenaltyShootout, 1);
		_pDataFile->WriteBits(m_SummaryHomeGoals, 5);
		_pDataFile->WriteBits(m_SummaryAwayGoals, 5);
		_pDataFile->WriteBits(m_SummaryHomePenaltyShootoutGoals, 4);
		_pDataFile->WriteBits(m_SummaryAwayPenaltyShootoutGoals, 4);
	}
 }

void CMatchEventList::WriteType(CDataFile *_pDataFile)
{
	if (m_bIsSummary == false)
	{
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (GetAt(ListPos)->ShouldBeSaved() == false)
				continue;

			CMatchEvent *pEvent = GetAt(ListPos);
			EXT_ASSERT(pEvent->GetEventType() < 16);
			_pDataFile->WriteBits(pEvent->GetEventType(), 4);
		}
	}
}

void CMatchEventList::WriteName(CDataFile *_pDataFile)
{
	if (m_bIsSummary == false)
	{
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			if (GetAt(ListPos)->ShouldBeSaved() == false)
				continue;

			CMatchEvent *pEvent = GetAt(ListPos);
			_pDataFile->WriteBits(pEvent->GetName().GetForenameID(), 16);
			_pDataFile->WriteBits(pEvent->GetName().GetSurnameID(), 16);
		}
	}
}

/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::Read
	Access:    	public
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventList::ReadTime(CDataFile* _pDataFile)
{
	byte bIsSummary = _pDataFile->ReadBits(1);

	if (bIsSummary == 0)
	{
		m_bIsSummary = false;

		byte HowMany = _pDataFile->ReadBits(8);
		EXT_ASSERT(HowMany < 150);	// Unusual number of match events
		RemoveList();
		SetSize(HowMany);
		for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
		{
			byte bTime = _pDataFile->ReadBits(8);
			SetAt(LoopCount, FD_BIN_NEW CMatchEvent(bTime, 0, CPersonName()));
		}
	}
	else
	{
		m_bIsSummary = true;

		m_bSummaryPenaltyShootout = _pDataFile->ReadBits(1);
		m_SummaryHomeGoals = _pDataFile->ReadBits(5);
		m_SummaryAwayGoals = _pDataFile->ReadBits(5);
		m_SummaryHomePenaltyShootoutGoals = _pDataFile->ReadBits(4);
		m_SummaryAwayPenaltyShootoutGoals = _pDataFile->ReadBits(4);
	}
 }

void CMatchEventList::ReadType(CDataFile *_pDataFile)
{
	if (m_bIsSummary == false)
	{
		for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
		{
			byte bEventType = _pDataFile->ReadBits(4);
			GetAt(LoopCount)->SetEventType(bEventType);
		}
	}
}

void CMatchEventList::ReadName(CDataFile *_pDataFile)
{
	if (m_bIsSummary == false)
	{
		for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
		{
			ushort usForename = _pDataFile->ReadBits(16);
			ushort usSurname = _pDataFile->ReadBits(16);
			GetAt(LoopCount)->SetName(usForename, usSurname);
		}
	}
}

/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::GetLastEventStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::GetLastEventStr() const
{
	if (GetSize() > 0)
	{
        return GetAt(GetUpperBound())->GetText();
	}
	else
	{
		return _T("");
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::GetLastEventTimeStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::GetLastEventTimeStr() const
{
	if (GetSize() > 0)
	{
        CMatchEvent *pEvent = GetAt(GetUpperBound());
		return pEvent->GetEventDiscTimeStr();
	}
	else
	{
		return _T("");
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::GetFreeMatchEvent
	Access:    	public
	Parameter: 	void
	Returns:   	CMatchEvent*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchEvent* CMatchEventList::GetFreeMatchEvent()
{
	CMatchEvent* pMatchEvent;
	if (WorldData().GetFreeMatchEventsList().GetSize() > 0)
	{
		pMatchEvent = WorldData().GetFreeMatchEventsList().GetAt(GetUpperBound());
		WorldData().GetFreeMatchEventsList().RemoveAt(GetUpperBound());
	}
	else
	{
		pMatchEvent = FD_BIN_NEW CMatchEvent();
	}
	EXT_ASSERT(pMatchEvent != null);
	return pMatchEvent;
}


/*------------------------------------------------------------------------------
Method:   	CMatchEventList::DoCalculateGoalDifference
Access:    	public
Returns:   	int
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::DoCalculateGoalDifference() const
{
	int HomeGoals = HomeScore(200);
	int AwayGoals = AwayScore(200);
	if (HomeGoals == AwayGoals)
	{
		return 0;
	}

	if (HomeGoals > AwayGoals)
	{
		return HomeGoals - AwayGoals;
	}
	return AwayGoals - HomeGoals;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::DoCountInGameEvents
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::DoCountInGameEvents() const
{
	int Count = 0;
	for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->IsPenaltyShootOutEvent() == false)
		{
			Count++;
		}
	}
	return Count;
}

//////////////////////////////////////////////////////////////////////////
// Removes all penalty shoot out events
//////////////////////////////////////////////////////////////////////////
void CMatchEventList::RemovePenaltyShootOutEvents()
{
	for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->IsPenaltyShootOutEvent() == true)
		{
			// Remove
			RemoveAt(ListPos);
			ListPos--;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Converts the event list into a summary, which dispenses with the actual stored
// events.
//////////////////////////////////////////////////////////////////////////
void CMatchEventList::ConvertToSummary()
{
	if (m_bIsSummary == false)
	{
		m_SummaryHomeGoals = HomeScore(200);
		m_SummaryAwayGoals = AwayScore(200);
		m_bSummaryPenaltyShootout = WasPenaltyShootOut();
		m_SummaryHomePenaltyShootoutGoals = NumberHomePenaltyShootOutGoals();
		m_SummaryAwayPenaltyShootoutGoals = NumberAwayPenaltyShootOutGoals();
		m_bIsSummary = true;
		RemoveList();
	}
}

bool CMatchEventList::IsSummary() const
{
	return m_bIsSummary;
}

void CMatchEventList::DoResetVariables()
{
  m_bIsSummary = false;
  m_bSummaryPenaltyShootout = false;
  m_SummaryAwayGoals = 0;
  m_SummaryHomeGoals = 0;
  m_SummaryHomePenaltyShootoutGoals = 0;
  m_SummaryAwayPenaltyShootoutGoals = 0;
  RemoveList();
}

