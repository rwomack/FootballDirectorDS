
/*

	MatchCommentary.h: interface for the CMatchCommentary class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __MATCHCOMMENTARY_H__
#define __MATCHCOMMENTARY_H__

class CMatchCommentary
{
public:
	CString&				GetText();
  void          DoFullTime(bool bExtraTime);
  void          DoHalfTime(bool bExtraTime);
  void          DoKickOff(CString _ClubName);
  void          DoTakeInjuredPlayerOff(CString _PlayerName, CString _ClubName);
  void          DoBringSubOnAfterInjury(CString _PlayerOnName, CString _ClubName, CString _PlayerOffName);
  void          DoNoSubOnAfterInjury(CString _PlayerOnName, CString _ClubName, const ushort _PlayersInTeamOnPitch);
  void          DoBookPlayer(CString _PlayerName, CString _ClubName);
  void          DoSendPlayerOff(CString _PlayerName, CString _ClubName, const bool _TwoYellows);
  void          DoSubstituteUserInjuredPlayer(CString _PlayerName, CString _ClubName);
  void          DoInjuredPlayerOK(CString _PlayerName, CString _ClubName);
  void          DoNormalGoalScored(CString _PlayerName, CString _ClubName);
  void          DoFreekickGoal(CString _PlayerName, CString _ClubName);
  void          DoSubstitutePlayer(CString _PlayerOffName, CString _PlayerOnName, CString _ClubName);
  void          DoOwnGoal(CString _PlayerName, CString _ClubName);
	void 					DoOffside(const CString &_PlayerAndClubName);
	void 					DoCorner(const CString &_ClubName);
  void          DoDisAllowedShot(CString _PlayerName, CString _ClubName);
  void          DoDisAllowedHeader(CString _PlayerName, CString _ClubName);
  void          DoShotOffTarget(CString _PlayerName, CString _ClubName);
  void          DoHeaderOffTarget(CString _PlayerName, CString _ClubName);
  void          DoShotSaved(CString _PlayerName, CString _ClubName);
  void          DoScoredGoalFromCorner(CString _PlayerName, CString _ClubName);
  void          DoScoredPenaltyGoal(CString _PlayerName, CString _ClubName);
  void          DoPenaltySaved(CString _PlayerName, CString _ClubName);
  void          DoPenaltyMissed(CString _PlayerName, CString _ClubName);
  void          DoFreeKick(CString _ClubName);
  void          DoUpdateText(CString _strText, const bool _NewLine = false);
	void					Empty();

protected:
	CExtString					m_strText;
};
#endif
