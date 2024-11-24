

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
    ClubID					m_FriendlyOpponentsClubID[MAXFRIENDLIES];
    byte					m_FriendlyVenue[MAXFRIENDLIES];

public:
	CClubFriendlies();

	ClubID					GetOpponents(const ushort _Index);
	eWhichTeam				GetVenue(const ushort _Index);
	ushort					CountAvailableFriendlies();
    void                    CreateFriendlyMatch(const ushort FriendlyNum, const ClubID _OpponentClubID, const eWhichTeam Venue);
	bool					IsFriendlyMatchToday(const ushort _MatchNumber, CMatchInfo& _Match, CClub* _ThisClub);
    CString                 FriendlyOpponentName(const ushort FriendlyNum);
    void                    OnNewSeason();
	bool                    CanPlayFriendly(const ushort MatchNumber, const ClubID _ClubID, CDivision* pDivision);
	CString GetStadiumNameStr(const ushort FriendlyNum, CClub* _ThisClub);
};
#endif
