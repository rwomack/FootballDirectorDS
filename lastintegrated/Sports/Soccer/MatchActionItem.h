
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
	void					DoBookPlayer(CString& _PlayerName, CString& _ClubName);
	void					DoBringSubOnAfterInjury(CString& _PlayerOffName, CString& _ClubName, CString& _PlayerOnName);
	void					DoCorner(CString& _ClubName);
	void					DoClearCurrentEvent();
	void					DoClearInformationStr();
	void					DoFullTime();
	void					DoFreekickGoal(CString& _PlayerName, CString& _ClubName, eWhichTeam _Who);
	void					DoFreeKick(CString& _ClubName);
	void					DoFreeKickOnTarget(const eWhichTeam _Who);
	void					DoGoalkeeperPenaltyEvent();
	void					DoGoalkeeperOffPenaltyEvent();
	void					DoIncrementTime();
	void					DoInjuredPlayerOK(CString& _PlayerName, CString& _ClubName, bool _IsOutfieldPlayer, eWhichTeam _Who);
	void					DoKickOff(CString& _ClubName);
	void					DoNoSubOnAfterInjury(CString& _PlayerOnName, CString& _ClubName, const ushort _PlayersInTeamOnPitch);
	void					DoNormalGoalScored(CString& _PlayerName, CString& _ClubName, eWhichTeam _Who);
	void					DoOutfieldPlayerPenaltyEvent();
	void					DoOutfieldPlayerOffPenaltyEvent();
	void 					DoPenaltySaved(CString& _PlayerName, CString& _ClubName, const eWhichTeam _Who = HOME);
	void 					DoPenaltyMissed(CString& _PlayerName, CString& _ClubName, const eWhichTeam _Who = HOME);
	void 					DoShotOffTarget(CString& _PlayerName, CString& _ClubName);
	void					DoDisAllowedShot(const eWhichTeam _Who, CString& _PlayerName, CString& _ClubName);
	void					DoDisAllowedHeader(const eWhichTeam _Who, CString& _PlayerName, CString& _ClubName);
	void 					DoScoredPenaltyGoal(CString& _PlayerName, CString& _ClubName);
	void					DoScoredGoalFromCorner(const eWhichTeam _Who, const bool _IsBottomCorner, CString& _PlayerName, CString& _ClubName);
	void 					DoSendPlayerOff(CString& _PlayerName, CString& _ClubName);
	void 					DoSubstituteUserInjuredPlayer(CString& _PlayerName, CString& _ClubName);
	void 					DoSubstitutePlayer(CPlayer& _PlayerOff, CPlayer& _PlayerOn, CString& _ClubName);
	void 					DoSubstituteUserPlayer(CPlayer& _PlayerOn, CPlayer& _PlayerOff, CString& _ClubName);
	void					DoTakeInjuredPlayerOff(const eWhichTeam _Who, const ushort _SquadNumber, const bool _CarriedOff, CString& _PlayerName, CString& _ClubName);
	void					DoOffside(CString _PlayerAndClubName);
	void					DoShotOnTarget(const eWhichTeam _Who, CString& _PlayerName, CString& _ClubName);
	void					DoGoalFX();
	void					DoWhistleFX();
	void					DoOwnGoal(const eWhichTeam _Who, CString& _PlayerName, CString& _ClubName);
	void					DoSetupType();
	void					DoResetVariables();
	void					DoHalfTime();
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
	void					SetInformationStr(const CString _strCommentary);
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