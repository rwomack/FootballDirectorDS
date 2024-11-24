#ifndef __PLAYERLOAN_H__
#define __PLAYERLOAN_H__

#include <SoccerDefines.h>

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
	void					DoGoingOnLoan(const ClubID _ClubOnLoanFromID, const byte _WeeksOnLoan);
	void					DoInitialise();
	void					DoWeeklyUpdate();
	ClubID					GetClubOnLoanFrom() const;
	byte					GetWeeksOnLoan() const;
	bool					IsOnLoan();
	bool					IsLoanedOut(const ClubID _ClubID);
	bool					IsLoanedUntilEndOfSeason();
	void					SetWeeksOnLoan(const byte _WeeksOnLoan);                  
	void					SetClubOnLoanFrom(const ClubID _ClubOnLoanFromID);				

	// Variables
protected:
	ClubID					m_ClubOnLoanFromID;
	byte					m_WeeksOnLoan;
};
#endif