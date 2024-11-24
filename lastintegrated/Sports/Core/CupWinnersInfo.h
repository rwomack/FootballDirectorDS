
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
	void					DoSwapClubIDs(const ushort _Club1ID, const ushort _Club2ID);
	void					SetSemiLosers(const ushort _Club1ID, const ushort _Club2ID);
	void					SetWinnersAndRunnersUp(const ushort _nWinnersClubID, const ushort _nRunnersUpClubID);

protected:
	ushort					m_CupWinnersClubID;
	ushort					m_CupRunnersUpID;
	ushort					m_CupSemiLosers[2];
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif