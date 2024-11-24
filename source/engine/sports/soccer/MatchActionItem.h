
/*

    File:- MatchActionItem.h

    Describes an event within a match

    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __MATCHACTIONITEM_H__
#define __MATCHACTIONITEM_H__

#include "SoccerMatchPlayEnums.h"
#include "SoccerMatchScore.h"

enum eEventGFX
{
	EVENTGFX_NULL,
	EVENTGFX_GOAL,
	EVENTGFX_TARGET,
	EVENTGFX_PENALTY,
	EVENTGFX_INJURY,
	EVENTGFX_YELLOW,
	EVENTGFX_RED,
	EVENTGFX_SUBS,
	EVENTGFX_WHISTLE
};


class CMatchActionItem
{
public:
	void					DoAddCurrentEvent(const int _CurrentEventOffset);
	void					DoBookPlayer(CPlayer& _Player, CClub & _Club);
	void					DoBringSubOnAfterInjury(CPlayer &_PlayerOff, CClub &_Club, CPlayer &_PlayerOn);
	void					DoCorner(CClub &_Club);
	void					DoClearCurrentEvent();
	void					DoClearInformationStr();
  void          DoFullTime(bool bExtraTime=false);
	void					DoFreekickGoal(CPlayer &_Player, CClub &_Club, eWhichTeam _Who);
	void					DoFreeKick(CClub& _Club);
	void					DoFreeKickOnTarget(const eWhichTeam _Who);
	void					DoGoalkeeperPenaltyEvent();
	void					DoGoalkeeperOffPenaltyEvent();
	void					DoIncrementTime();
	void					DoInjuredPlayerOK(CPlayer &_Player, CClub &_Club, bool _IsOutfieldPlayer, eWhichTeam _Who);
	void					DoKickOff(CClub &_Club);
	void					DoNoSubOnAfterInjury(CPlayer &_PlayerOn, CClub &_Club, const ushort _PlayersInTeamOnPitch);
	void					DoNormalGoalScored(CPlayer &_Player, CClub &_Club, eWhichTeam _Who);
	void					DoOutfieldPlayerPenaltyEvent();
	void					DoOutfieldPlayerOffPenaltyEvent();
	void 					DoPenaltySaved(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who = HOME);
	void 					DoPenaltyMissed(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who = HOME);
	void 					DoShotOffTarget(CPlayer &_Player, CClub &_Club);
	void					DoDisAllowedShot(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club);
	void					DoDisAllowedHeader(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club);
	void 					DoScoredPenaltyGoal(CPlayer &_Player, CClub &_Club, const eWhichTeam _Who = HOME);
	void					DoScoredGoalFromCorner(const eWhichTeam _Who, const bool _IsBottomCorner, CPlayer &_Player, CClub &_Club);
	void 					DoSendPlayerOff(CPlayer &_Player, CClub &_Club);
	void 					DoSubstituteUserInjuredPlayer(CPlayer &_Player, CClub &_Club);
	void 					DoSubstitutePlayer(CPlayer& _PlayerOff, CPlayer& _PlayerOn, CClub &_Club);
	void 					DoSubstituteUserPlayer(CPlayer& _PlayerOn, CPlayer& _PlayerOff, CClub &_Club);
	void					DoTakeInjuredPlayerOff(const eWhichTeam _Who, const ushort _SquadNumber, const bool _CarriedOff, CPlayer &_Player, CClub &_Club);
	void					DoOffside(const CString& _PlayerAndClubName);
	void					DoShotOnTarget(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club);
	void					DoGoalFX();
	void					DoWhistleFX();
	void					DoOwnGoal(const eWhichTeam _Who, CPlayer &_Player, CClub &_Club);
	void					DoSetupType();
	void					DoResetVariables();
  void          DoHalfTime(bool bExtraTime=false);
	CMatchCommentary&		GetCommentary();
	eMatchEvent				GetCurrentEvent() const;
	CString&				GetCommentaryStr();
	COLORREF				GetCommentaryTextColor() const;
	CString&				GetInformationStr();
	eEventGFX				GetGfxID() const;
	byte					GetTimeSinceLastEvent() const;
	uint					GetType() const;
	eSOUNDFX				GetSoundFXID() const;
	void					SetSoundFXID(const eSOUNDFX SoundFXID);
	void					SetCurrentEvent(eMatchEvent _CurrentEvent, eWhichTeam _Who);
	void					SetInformationStr(const CString& _strCommentary);
	void					SetGfxID(const eEventGFX _gfxID);
	void					SetType(const UINT Type);
	eWhichTeam				TeamInPosession();

protected:
	uint					m_CurrentEvent;
	CString					m_strInformation;
	CMatchCommentary		m_Commentary;
	eEventGFX				m_gfxID;
	uint					m_Type;
	byte					m_TimeSinceLastEvent;
	eSOUNDFX				m_SoundFXID;
};
#endif
