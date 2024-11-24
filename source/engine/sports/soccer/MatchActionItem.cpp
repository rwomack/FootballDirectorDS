
/*

    File:- MatchActionItem.cpp

    Describes an event within a match

    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoSetupType
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoSetupType()
{
	static int DisplayCommentary[] =
	{
		EVENTGFX_GOAL,
		EVENTGFX_TARGET,
		EVENTGFX_PENALTY,
		EVENTGFX_INJURY,
		EVENTGFX_YELLOW,
		EVENTGFX_RED,
		EVENTGFX_SUBS,
		EVENTGFX_WHISTLE,
	};
	m_Type = DisplayCommentary[m_gfxID + 1];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetCommentaryTextColor
	Access:    	public const
	Parameter: 	void
	Returns:   	COLORREF
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CMatchActionItem::GetCommentaryTextColor() const
{
	return WHITE;	//colorCommentary[m_gfxID + 1];
}

/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoResetVariables()
{
	m_CurrentEvent = NOEVENT;
	m_Commentary.Empty();
	m_gfxID = EVENTGFX_NULL;
	m_Type = 0;
	m_SoundFXID = SOUND_MAX;
	m_TimeSinceLastEvent = 0;
	m_strInformation = CString();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoIncrementTime
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoIncrementTime()
{
	if (GetCurrentEvent() == NOEVENT)
	{
		m_TimeSinceLastEvent++;
	}
	else
	{
		m_TimeSinceLastEvent = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::SetCurrentEvent
	Access:    	public
	Parameter: 	const enumMatchEvent CurrentEvent
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::SetCurrentEvent(eMatchEvent _CurrentEvent, eWhichTeam _Who)
{
	m_CurrentEvent = _CurrentEvent + _Who;
	if (m_CurrentEvent != NOEVENT)
	{
		m_TimeSinceLastEvent = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::SetInformationStr
	Access:    	public
	Parameter: 	const CString strCommentary
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::SetInformationStr(const CString &_strCommentary)
{
	m_strInformation = _strCommentary;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::SetSoundFXID
	Access:    	public
	Parameter: 	const eSOUNDFX SoundFXID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::SetSoundFXID(const eSOUNDFX SoundFXID)
{
	m_SoundFXID = SoundFXID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::SetGfxID
	Access:    	public
	Parameter: 	const eEventGFX _gfxID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::SetGfxID(const eEventGFX _gfxID)
{
	m_gfxID = _gfxID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::SetType
	Access:    	public
	Parameter: 	const UINT Type
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::SetType(const UINT Type)
{
	m_Type = Type;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoClearInformationStr
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoClearInformationStr()
{
	m_strInformation.Empty();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoAddCurrentEvent
	Access:    	public
	Parameter: 	const int CurrentEvent
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoAddCurrentEvent(const int _CurrentEventOffset)
{
	m_CurrentEvent = (int)m_CurrentEvent + _CurrentEventOffset;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoHalfTime
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoHalfTime(bool bExtraTime)
{
  m_Commentary.DoHalfTime(bExtraTime);
	m_gfxID = EVENTGFX_WHISTLE;
	m_SoundFXID = SOUND_FULLWHST;
	m_CurrentEvent = NOEVENT;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetTimeSinceLastEvent
	Access:    	public const
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchActionItem::GetTimeSinceLastEvent() const
{
	return m_TimeSinceLastEvent;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoFullTime
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoFullTime(bool bExtraTime)
{
  m_Commentary.DoFullTime(bExtraTime);    // Displayed by match screen
	m_gfxID = EVENTGFX_WHISTLE;
	m_SoundFXID = SOUND_FULLWHST;
	m_CurrentEvent = NOEVENT;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetCommentary
	Access:    	public
	Parameter: 	void
	Returns:   	CMatchCommentary&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchCommentary& CMatchActionItem::GetCommentary()
{
	return m_Commentary;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetCommentaryStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchActionItem::GetCommentaryStr()
{
	return m_Commentary.GetText();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetCurrentEvent
	Access:    	public const
	Parameter: 	void
	Returns:   	enumMatchEvent
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eMatchEvent CMatchActionItem::GetCurrentEvent() const
{
	return static_cast<eMatchEvent>(m_CurrentEvent);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetInformationStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchActionItem::GetInformationStr()
{
	return m_strInformation;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetGfxID
	Access:    	public const
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eEventGFX CMatchActionItem::GetGfxID() const
{
	return m_gfxID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetType
	Access:    	public const
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchActionItem::GetType() const
{
	return m_Type;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::GetSoundFXID
	Access:    	public const
	Parameter: 	void
	Returns:   	eSOUNDFX
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eSOUNDFX CMatchActionItem::GetSoundFXID() const
{
	return m_SoundFXID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::TeamInPosession
	Access:    	public
	Parameter: 	void
	Returns:   	eWhichTeam
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CMatchActionItem::TeamInPosession()
{
	eWhichTeam TeamInPosession[] =
	{
		HOME,
		HOME, AWAY,                 // HOMEGOAL, AWAYGOAL,
		HOME, AWAY,                 // HOMEBOOKING, AWAYBOOKING,
		HOME, AWAY,                 // HOMESENDINGOFF, AWAYSENDINGOFF,
        HOME, AWAY,                 // HOMEOWNGOAL, AWAYOWNGOAL,
		HOME, AWAY,                 // HOMEPENALTYGOAL, AWAYPENALTYGOAL,
		HOME, AWAY,					// HOMEPENALTYSHOOTOUTGOAL, AWAYPENALTYSHOOTOUTGOAL,
	    HOME, AWAY,                 // HOMEOFFSIDEATTEMPT, AWAYOFFSIDEATTEMPT,
		HOME, AWAY,                 // HOMEFOULPLAYERATTEMPT, AWAYFOULPLAYERATTEMPT,
		HOME, AWAY,                 // HOMEINJUREPLAYERATTEMPT, AWAYINJUREPLAYERATTEMPT,
		HOME, AWAY,                 // HOMESHOTATTEMPT, AWAYSHOTATTEMPT,
		HOME, AWAY,                 // HOMEMISSATTEMPT, AWAYMISSATTEMPT,
		HOME, AWAY,                 // HOMEFREEKICKATTEMPT, AWAYFREEKICKATTEMPT,
		HOME, AWAY,                 // HOMECORNERATTEMPT, AWAYCORNERATTEMPT,
	};

	FDASSERT(GetCurrentEvent() < (sizeof(TeamInPosession) / sizeof(eWhichTeam)));
	return TeamInPosession[GetCurrentEvent()];
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoTakeInjuredPlayerOff
	Access:    	public
	Parameter: 	const eWhichTeam eWho
	Parameter: 	const ushort PlayerSquadNumber
	Parameter: 	const bool bCarriedOff
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoTakeInjuredPlayerOff(const eWhichTeam _Who, const ushort _SquadNumber, const bool _CarriedOff, CPlayer &_Player, CClub &_Club)
{
	m_Commentary.DoTakeInjuredPlayerOff(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));

	eMatchEvent WhichEvent;
	if (_CarriedOff == false)
	{
		if (_SquadNumber > 0)
		{
			if (_Who == HOME)
			{
				WhichEvent = HOMEOUTFIELDPLAYERHOBBLE;
			}
			else
			{
				WhichEvent = AWAYOUTFIELDPLAYERHOBBLE;
			}
		}
		else
		{
			if (_Who == HOME)
			{
				WhichEvent = HOMEGOALKEEPERHOBBLE;
			}
			else
			{
				WhichEvent = AWAYGOALKEEPERHOBBLE;
			}
		}
	}
	else
	{
		if (_SquadNumber > 0)
		{
			if (_Who == HOME)
			{
				WhichEvent = HOMEOUTFIELDPLAYERCARRIEDOFF;
			}
			else
			{
				WhichEvent = AWAYOUTFIELDPLAYERCARRIEDOFF;
			}
		}
		else
		{
			if (_Who == HOME)
			{
				WhichEvent = HOMEGOALKEEPERCARRIEDOFF;
			}
			else
			{
				WhichEvent = AWAYGOALKEEPERCARRIEDOFF;
			}
		}
	}
	SetCurrentEvent(WhichEvent, HOME);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoBringSubOnAfterInjury
	Access:    	public
	Parameter: 	CPlayer& _PlayerOff
	Parameter: 	CClub& _Club
	Parameter: 	CPlayer& _PlayerOn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoBringSubOnAfterInjury(CPlayer &_PlayerOff, CClub &_Club, CPlayer &_PlayerOn)
{
	DoAddCurrentEvent(12);			// show sub coming on
	m_Commentary.DoBringSubOnAfterInjury(CNewsItem::PreprocessPlayerName(&_PlayerOff), CNewsItem::PreprocessClubName(&_Club), CNewsItem::PreprocessPlayerName(&_PlayerOn));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoNoSubOnAfterInjury
	Access:    	public
	Parameter: 	CPlayer & _PlayerOn
	Parameter: 	CClub &_Club
	Parameter: 	const ushort nPlayersInTeamOnPitch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoNoSubOnAfterInjury(CPlayer &_PlayerOn, CClub &_Club, const ushort _PlayersInTeamOnPitch)
{
	DoAddCurrentEvent(28);				// don't show sub coming on
	m_Commentary.DoNoSubOnAfterInjury(CNewsItem::PreprocessPlayerName(&_PlayerOn), CNewsItem::PreprocessClubName(&_Club), _PlayersInTeamOnPitch);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoBookPlayer
	Access:    	public
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoBookPlayer(CPlayer &_Player, CClub &_Club)
{
	m_Commentary.DoBookPlayer(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	SetGfxID(EVENTGFX_YELLOW);
	m_SoundFXID = SOUND_BOO;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoOutfieldPlayerPenaltyEvent
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoOutfieldPlayerPenaltyEvent()
{
	switch(GetCurrentEvent())
	{
		case HOMEPENALTYGOAL:
			m_CurrentEvent = HOMEPENALTYGOALYELLOWOUTFIELDPLAYER;
			break;
		case AWAYPENALTYGOAL:
			m_CurrentEvent = AWAYPENALTYGOALYELLOWOUTFIELDPLAYER;
			break;
		case HOMEPENALTYSAVED:
			m_CurrentEvent = HOMEPENALTYSAVEDYELLOWOUTFIELDPLAYER;
			break;
		case AWAYPENALTYSAVED:
			m_CurrentEvent = AWAYPENALTYSAVEDYELLOWOUTFIELDPLAYER;
			break;
		case HOMEPENALTYMISS:
			m_CurrentEvent = HOMEPENALTYMISSYELLOWOUTFIELDPLAYER;
			break;
		case AWAYPENALTYMISS:
			m_CurrentEvent = AWAYPENALTYMISSYELLOWOUTFIELDPLAYER;
			break;
		default :
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoGoalkeeperPenaltyEvent
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoGoalkeeperPenaltyEvent()
{
	switch(GetCurrentEvent())
	{
		case HOMEPENALTYGOAL:
			m_CurrentEvent = HOMEPENALTYGOALYELLOWGOALKEEPER;
			break;
		case AWAYPENALTYGOAL:
			m_CurrentEvent = AWAYPENALTYGOALYELLOWGOALKEEPER;
			break;
		case HOMEPENALTYSAVED:
			m_CurrentEvent = HOMEPENALTYSAVEDYELLOWGOALKEEPER;
			break;
		case AWAYPENALTYSAVED:
			m_CurrentEvent = AWAYPENALTYSAVEDYELLOWGOALKEEPER;
			break;
		case HOMEPENALTYMISS:
			m_CurrentEvent = HOMEPENALTYMISSYELLOWGOALKEEPER;
			break;
		case AWAYPENALTYMISS:
			m_CurrentEvent = AWAYPENALTYMISSYELLOWGOALKEEPER;
			break;
		default :
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoSendPlayerOff
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoSendPlayerOff(CPlayer &_Player, CClub &_Club)
{
	m_Commentary.DoSendPlayerOff(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club), false);
	SetGfxID(EVENTGFX_RED);
	m_SoundFXID = SOUND_BOO;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoOutfieldPlayerOffPenaltyEvent
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoOutfieldPlayerOffPenaltyEvent()
{
	switch(GetCurrentEvent())
	{
		case HOMEPENALTYGOAL:
			m_CurrentEvent = HOMEPENALTYGOALREDOUTFIELDPLAYER;
			break;
		case AWAYPENALTYGOAL:
			m_CurrentEvent = AWAYPENALTYGOALREDOUTFIELDPLAYER;
			break;
		case HOMEPENALTYSAVED:
			m_CurrentEvent = HOMEPENALTYSAVEDREDOUTFIELDPLAYER;
			break;
		case AWAYPENALTYSAVED:
			m_CurrentEvent = AWAYPENALTYSAVEDREDOUTFIELDPLAYER;
			break;
		case HOMEPENALTYMISS:
			m_CurrentEvent = HOMEPENALTYMISSREDOUTFIELDPLAYER;
			break;
		case AWAYPENALTYMISS:
			m_CurrentEvent = AWAYPENALTYMISSREDOUTFIELDPLAYER;
			break;
		default :
		EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoGoalkeeperOffPenaltyEvent
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoGoalkeeperOffPenaltyEvent()
{
	switch(GetCurrentEvent())
	{
		case HOMEPENALTYGOAL:
			m_CurrentEvent = HOMEPENALTYGOALREDGOALKEEPER;
			break;
		case AWAYPENALTYGOAL:
			m_CurrentEvent = AWAYPENALTYGOALREDGOALKEEPER;
			break;
		case HOMEPENALTYSAVED:
			m_CurrentEvent = HOMEPENALTYSAVEDREDGOALKEEPER;
			break;
		case AWAYPENALTYSAVED:
			m_CurrentEvent = AWAYPENALTYSAVEDREDGOALKEEPER;
			break;
		case HOMEPENALTYMISS:
			m_CurrentEvent = HOMEPENALTYMISSREDGOALKEEPER;
			break;
		case AWAYPENALTYMISS:
			m_CurrentEvent = AWAYPENALTYMISSREDGOALKEEPER;
			break;
		default :
			EXT_ASSERT(false);
		}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoScoredGoalFromCorner
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const uint _TopOrBottomCorner
	Parameter: 	CPlayer & _Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoScoredGoalFromCorner(const eWhichTeam _Who, const bool _IsBottomCorner, CPlayer &_Player, CClub &_Club)
{
	if (_IsBottomCorner == true)
	{
		if (RandomNumber.IntLessThan(10) <= HEADERCHANCE)
		{
			SetCurrentEvent(HOMETOPCORNERGOALSHOT, _Who);
		}
		else
		{
			SetCurrentEvent(HOMETOPCORNERGOALHEADER, _Who);
		}
	}
	else
	{
		if (RandomNumber.IntLessThan(10) <= HEADERCHANCE)
		{
			SetCurrentEvent(HOMEBOTTOMCORNERGOALSHOT, _Who);
		}
		else
		{
			SetCurrentEvent(HOMEBOTTOMCORNERGOALHEADER, _Who);
		}
	}
	m_Commentary.DoScoredGoalFromCorner(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	DoGoalFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoShotOffTarget
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoShotOffTarget(CPlayer &_Player, CClub &_Club)
{
	if (RandomNumber.IntLessThan(10) < HEADERCHANCE)
	{
		m_Commentary.DoShotOffTarget(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	}
	else
	{
		DoAddCurrentEvent(2);
		m_Commentary.DoHeaderOffTarget(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoSubstituteUserInjuredPlayer
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoSubstituteUserInjuredPlayer(CPlayer &_Player, CClub &_Club)
{
	m_Commentary.DoSubstituteUserInjuredPlayer(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoInjuredPlayerOK
	Access:    	public
	Parameter: 	CPlayer & _Player
	Parameter: 	CClub & _Club
	Parameter: 	int _EventOffset
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoInjuredPlayerOK(CPlayer &_Player, CClub &_Club, bool _IsOutfieldPlayer, eWhichTeam _Who)
{
	SetCurrentEvent(HOMEGOALKEEPERINJURYOK, _Who);
	m_Commentary.DoInjuredPlayerOK(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoNormalGoalScored
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Parameter: 	int _EventOffset
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoNormalGoalScored(CPlayer &_Player, CClub &_Club, eWhichTeam _Who)
{
	if (RandomNumber.IntLessThan(10) <= HEADERCHANCE)
	{
		// Headed Goal
		SetCurrentEvent(HOMEGOALHEADER, _Who);
	}
	else
	{
		SetCurrentEvent(HOMEGOALSHOT, _Who);
	}
	m_Commentary.DoNormalGoalScored(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	DoGoalFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoFreekickGoal
	Access:    	public
	Parameter: 	CPlayer & _Player
	Parameter: 	CClub & _Club
	Parameter: 	eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoFreekickGoal(CPlayer &_Player, CClub &_Club, eWhichTeam _Who)
{
	SetCurrentEvent(HOMEFREEKICKGOAL, _Who);
	m_Commentary.DoFreekickGoal(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	DoGoalFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoSubstitutePlayer
	Access:    	public
	Parameter: 	CPlayer&_PlayerOff
	Parameter: 	CPlayer&_PlayerOn
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoSubstitutePlayer(CPlayer& _PlayerOff, CPlayer& _PlayerOn, CClub &_Club)
{
	EXT_ASSERT(&_PlayerOn != &_PlayerOff);
	m_Commentary.DoSubstitutePlayer(CNewsItem::PreprocessPlayerName(&_PlayerOff), CNewsItem::PreprocessPlayerName(&_PlayerOn), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoOwnGoal
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoOwnGoal(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club)
{
	SetCurrentEvent( eMatchEvent(HOMEOWNGOAL + _Who), _Who);
	m_Commentary.DoOwnGoal(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	DoGoalFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoClearCurrentEvent
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoClearCurrentEvent()
{
	m_CurrentEvent = NOEVENT;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoKickOff
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoKickOff(CClub &_Club)
{
	DoWhistleFX();
	m_Commentary.DoKickOff(CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoGoalFX
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoGoalFX()
{
	SetGfxID(EVENTGFX_GOAL);
  SetSoundFXID(SOUND_GOALCHEE);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoWhistleFX
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoWhistleFX()
{
	SetGfxID(EVENTGFX_WHISTLE);
	SetSoundFXID(SOUND_WHISTLE);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoOffside
	Access:    	public
	Parameter: 	CString _PlayerAndClubName
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoOffside(const CString &_PlayerAndClubName)
{
	m_Commentary.DoOffside(_PlayerAndClubName);
	DoWhistleFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoCorner
	Access:    	public
	Parameter: 	CClub _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoCorner(CClub &_Club)
{
	m_Commentary.DoCorner(CNewsItem::PreprocessClubName(&_Club));
	DoWhistleFX();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoShotOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoShotOnTarget(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club)
{
	if (RandomNumber.IntLessThan(CHANCEDISALLOWEDGOAL) == 1)
	{
        if (RandomNumber.IntLessThan(10) < HEADERCHANCE)
		{
			DoDisAllowedShot(_Who, _Player, _Club);
		}
		else
		{
			DoDisAllowedHeader(_Who, _Player, _Club);
		}
		DoWhistleFX();
	}
	else
	{
		m_Commentary.DoShotSaved(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
    }
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoSubstituteUserPlayer
	Access:    	public
	Parameter: 	CPlayer &_PlayerOn
	Parameter: 	CPlayer &_PlayerOff
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoSubstituteUserPlayer(CPlayer& _PlayerOn, CPlayer& _PlayerOff, CClub& _Club)
{
	EXT_ASSERT(&_PlayerOn != &_PlayerOff);
	// What highlight to show
	if (_PlayerOn.IsAvailableForMatch() == true && _PlayerOff.IsAvailableForMatch() == true)
 	{
		// Player coming off is not injured so walks off
		DoSubstitutePlayer(_PlayerOn, _PlayerOff, _Club);
	}
	else
	{
		// Player come off injured already
		DoBringSubOnAfterInjury(_PlayerOff, _Club, _PlayerOn);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoDisAllowedShot
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoDisAllowedShot( const eWhichTeam _Who, CPlayer &_Player, CClub &_Club )
{
	SetCurrentEvent(HOMEDISALLOWEDSHOT, _Who);
	m_Commentary.DoDisAllowedShot(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	SetGfxID(EVENTGFX_TARGET);
	SetSoundFXID(SOUND_CROWDLRG);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoDisAllowedHeader
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoDisAllowedHeader(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club)
{
	SetCurrentEvent(HOMEDISALLOWEDHEADER, _Who);
	m_Commentary.DoDisAllowedHeader(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
	SetGfxID(EVENTGFX_TARGET);
	SetSoundFXID(SOUND_CROWDLRG);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoScoredPenaltyGoal
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoScoredPenaltyGoal(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who /*= HOME*/)
{
	SetCurrentEvent( eMatchEvent(HOMEPENALTYGOAL), _Who);
	m_Commentary.DoScoredPenaltyGoal(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoPenaltySaved
	Access:    	public
	Parameter: 	CPlayer &_Player
	Parameter: 	CClub &_Club
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoPenaltySaved(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who /*= HOME*/)
{
	SetCurrentEvent( eMatchEvent(HOMEPENALTYSAVED), _Who);
	m_Commentary.DoPenaltySaved(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoPenaltyMissed
	Access:    	public
	Parameter: 	CPlayer& _Player
	Parameter: 	CClub& _Club
	Parameter: 	const eWhichTeam a_Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoPenaltyMissed(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who /*= HOME*/)
{
	SetCurrentEvent( eMatchEvent(HOMEPENALTYMISS), _Who);
	m_Commentary.DoPenaltyMissed(CNewsItem::PreprocessPlayerName(&_Player), CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoFreeKick
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoFreeKick(CClub &_Club)
{
	m_Commentary.DoFreeKick(CNewsItem::PreprocessClubName(&_Club));
}


/*------------------------------------------------------------------------------
	Method:   	CMatchActionItem::DoFreeKickOnTarget
	Access:    	public
	Parameter: 	const eWhichTeam _Who
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchActionItem::DoFreeKickOnTarget(const eWhichTeam _Who)
{
	SetGfxID(EVENTGFX_TARGET);
    SetCurrentEvent(HOMEFREEKICKONTARGET, _Who);
}