

/*

		Class - C++ Classes

		ClubFriendlies class

		Football Director

		(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/


#ifndef __CLUBFRIENDLIES_H__
#define __CLUBFRIENDLIES_H__


	static const int		MAXFRIENDLIES = 4;
	static const ushort		NOFRIENDLY = NOCLUB;

	
class CClubFriendlies
{
private:
    ushort					m_FriendlyOpponentsClubID[MAXFRIENDLIES];
    byte					m_FriendlyVenue[MAXFRIENDLIES];

public:
	CClubFriendlies();

	ushort					GetOpponents(const ushort _Index);
	eWhichTeam				GetVenue(const ushort _Index);
	ushort					CountAvailableFriendlies();
    void                    CreateFriendlyMatch(const ushort FriendlyNum, const ushort Opponent, const eWhichTeam Venue);
	bool					IsFriendlyMatchToday(const ushort _MatchNumber, CMatchInfo& _Match, CClub* _ThisClub);
    CString                 FriendlyOpponentName(const ushort FriendlyNum);
    void                    OnNewSeason();
	bool                    CanPlayFriendly(const ushort MatchNumber, const ushort ClubNumber, CDivision* pDivision);
	CString GetStadiumNameStr(const ushort FriendlyNum, CClub* _ThisClub);
};
#endif
