
/*

    File:- PenaltyShootOut.h


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __PENALTYSHOOTOUT_H__
#define __PENALTYSHOOTOUT_H__


class CPenaltyShootOut
{
public:
	void					AwayPenaltyCommentary(CMatchPlay& _MatchPlay, const ushort _Index, CClub& _Club);
	int						DoCalculateShot(int _GoalKeeperSkill, int _ShotSkill);
	void					DoCreateTakersList(CClub& _Club, CNumericArray<byte>& _listTakers);
	void					DoShootout(CMatchPlay& _MatchPlay);
  void          DoPenaltyCommentary(CMatchPlay& _MatchPlay, const byte _ResultType, CPlayer &_Player, CClub &_Club, const eWhichTeam _Who);
	void					HomePenaltyCommentary(CMatchPlay& _MatchPlay, const ushort _Index, CClub& _Club);
	int						GetPlayerEffectiveSkill(eWhichTeam _Who, int _PlayerSquadNum, ePlayerWhichSkill _WhichSkill);

  int GetAwayPenaltyResult( int _index );
  int GetHomePenaltyResult( int _index );

  int GetHomeTakerIndex( int _index );
  int GetAwayTakerIndex( int _index );

	// Variables
private:
	CNumericArray<byte>		m_PenaltyShootoutHomeResult;
	CNumericArray<byte>		m_PenaltyShootoutAwayResult;
	CNumericArray<byte>		m_PenaltyShootoutHomeSquadNumbers;
	CNumericArray<byte>		m_PenaltyShootoutAwaySquadNumbers;
};
#endif
