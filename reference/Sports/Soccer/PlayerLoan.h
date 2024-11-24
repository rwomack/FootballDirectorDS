#ifndef __PLAYERLOAN_H__
#define __PLAYERLOAN_H__


/*

    Player Loan Information class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


	const int				MAXLOANPLAYERWEEKS = 52;


class CPlayerLoan
{
public:

	// Methods
	void					DoCheckForSwapClubs(const ushort _Club1ID, const ushort _Club2ID);
	void					DoGoingOnLoan(const ushort _ClubOnLoanFromID, const byte _WeeksOnLoan);
	void					DoInitialise();
	void					DoWeeklyUpdate();
	ushort					GetClubOnLoanFrom() const;
	byte					GetWeeksOnLoan() const;
	bool					IsOnLoan();
	bool					IsLoanedOut(const ushort _ClubID);
	bool					IsLoanedUntilEndOfSeason();
	void					SetWeeksOnLoan(const byte _WeeksOnLoan);                  
	void					SetClubOnLoanFrom(const ushort _ClubOnLoanFromID);				

	// Variables
protected:
	ushort					m_ClubOnLoanFromID;
	byte					m_WeeksOnLoan;
};
#endif