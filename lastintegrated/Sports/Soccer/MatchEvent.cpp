/*

	File:- MatchEvent.cpp

	Match Event class


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "MatchEvent.h"


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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::DoCountEvents
	Access:    	public 
	Parameter: 	eMatchEvent _EventType
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::DoCountEvents(eMatchEvent _EventType)
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
int CMatchEventList::HomeNumberPlayersBooked()
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
int CMatchEventList::AwayNumberPlayersBooked()
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
int CMatchEventList::HomeNumberPlayersSentOff()
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
int CMatchEventList::AwayNumberPlayersSentOff()
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
int CMatchEventList::NumberHomeOwnGoals()
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
int CMatchEventList::NumberAwayOwnGoals()
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
int CMatchEventList::NumberHomePenaltyGoals()
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
int CMatchEventList::NumberAwayPenaltyGoals()
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
int CMatchEventList::NumberHomePenaltyShootOutGoals()
{
	return DoCountEvents(HOMEPENALTYSHOOTOUTGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::PenaltyShootoutWinner
	Access:    	public 
	Parameter: 	void
	Returns:   	eWhichTeam - home or away
	Qualifier: 	
	Purpose:	Which side won the penalty shoot out
------------------------------------------------------------------------------*/
eWhichTeam CMatchEventList::PenaltyShootoutWinner()
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
CString CMatchEventList::PenaltyShootoutWinnerStr(const CString _HomeClubName, const CString _AwayClubName)
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
bool CMatchEventList::WasPenaltyShootOut()
{
	if (NumberHomePenaltyShootOutGoals() > 0 || NumberAwayPenaltyShootOutGoals() > 0)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::NumberAwayPenaltyShootOutGoals
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMatchEventList::NumberAwayPenaltyShootOutGoals()
{
	return DoCountEvents(AWAYPENALTYSHOOTOUTGOAL);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HomePlayerScoredStr
	Access:    	public 
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::HomePlayerScoredStr(const ushort _EventNumber)
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
CString CMatchEventList::AwayPlayerScoredStr(const ushort _EventNumber)
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
CString CMatchEventList::HomePlayerBookedStr(const ushort _EventNumber)
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
CString CMatchEventList::AwayPlayerBookedStr(const ushort _EventNumber)
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
CString CMatchEventList::HomePlayerSentOffStr(const ushort _EventNumber)
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


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::IsHomeEvent
	Access:    	public 
	Parameter: 	const ushort _EventNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayPlayerSentOffStr
	Access:    	public 
	Parameter: 	const ushort _EventNumber
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchEventList::AwayPlayerSentOffStr(const ushort _EventNumber)
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
uint CMatchEventList::HomeScore(const uint _Time)
{
    uint iRet = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= _Time && (pEvent->GetEventType() == HOMEGOAL 
                                              || pEvent->GetEventType() == HOMEOWNGOAL
                                              || pEvent->GetEventType() == HOMEPENALTYGOAL));
    }
    return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::AwayScore
	Access:    	public 
	Parameter: 	const uint _Time
	Returns:   	uint - the score of the away team at the requested time
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::AwayScore(const uint _Time)
{
    uint iRet = 0;
	CMatchEvent* pEvent;
    for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= _Time && (pEvent->GetEventType() == AWAYGOAL 
                                              || pEvent->GetEventType() == AWAYOWNGOAL
                                              || pEvent->GetEventType() == AWAYPENALTYGOAL));
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventList::HalfTimeHomeGoals
	Access:    	public 
	Parameter: 	void
	Returns:   	uint - the number of goals scored in the first half by the home team
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchEventList::HalfTimeHomeGoals()
{
	uint iRet = 0;
	CMatchEvent* pEvent;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= HALFTIME && (pEvent->GetEventType() == HOMEGOAL \
                                                  || pEvent->GetEventType() == HOMEOWNGOAL\
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
uint CMatchEventList::HalfTimeAwayGoals()
{
	uint iRet = 0;
	CMatchEvent* pEvent;
	for (int ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        pEvent = GetAt(ListPos);
        iRet += (pEvent->GetTime() <= HALFTIME && (pEvent->GetEventType() == AWAYGOAL \
                                                  || pEvent->GetEventType() == AWAYOWNGOAL\
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
bool CMatchEventList::HasGoalBeenScored(const uint _Time)
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
void CMatchEventList::Write(CDataFile* _pDataFile)
{
    byte HowMany = GetSize();
	EXT_ASSERT(HowMany < 150); // Unusual number of match events
    _pDataFile->Write(&HowMany, sizeof HowMany);
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		_pDataFile->Write(GetAt(ListPos), sizeof CMatchEvent);
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
void CMatchEventList::Read(CDataFile* _pDataFile)
{
	byte HowMany;
    _pDataFile->Read(&HowMany, sizeof HowMany);
	EXT_ASSERT(HowMany < 150);	// Unusual number of match events
	int LoopCount;
	RemoveList();
	SetSize(HowMany);
	for (LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		SetAt(LoopCount, new CMatchEvent());
		_pDataFile->Read(GetAt(LoopCount), sizeof CMatchEvent);
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
CString CMatchEventList::GetLastEventStr()
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
CString CMatchEventList::GetLastEventTimeStr()
{
    CMatchEvent* pEvent;
	if (GetSize() > 0)
	{
        pEvent = GetAt(GetUpperBound());
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
		pMatchEvent = new CMatchEvent();
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
int CMatchEventList::DoCalculateGoalDifference()
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
int CMatchEventList::DoCountInGameEvents()
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
