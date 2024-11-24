
/*

	C++ Classes

	CupWinnersInfo class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#ifndef __CUPWINNERSINFO_H__
#define __CUPWINNERSINFO_H__



class 	CCupWinnersInfo
{
public:
	CCupWinnersInfo();

    // Methods
	void					SetSemiLosers(const ClubID _Club1ID, const ClubID _Club2ID);
	void					SetWinnersAndRunnersUp(const ClubID _nWinnersClubID, const ClubID _nRunnersUpClubID);

protected:
	ClubID					m_CupWinnersClubID;
	ClubID					m_CupRunnersUpID;
	ClubID					m_CupSemiLosers[2];
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif