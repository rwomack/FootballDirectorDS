
/*
	File:- Competition.h

	C++ Classes

	Competition class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __COMPETITION_H__
#define __COMPETITION_H__



class CCompetitionList
{
public:
	CCompetitionList() {};

	void					DoAddCup(CCup* pCup);
	void					DoAddCharityShield(CCharityShield* _CharityShield);
	CCup*					GetCup(const ushort CupId);
	CCupRound*				GetCupRound(const ushort CupId, const ushort iRoundNum);
	int						GetNumRounds(const ushort CupId);
	ushort					GetSize();
	CString					GetTitle(const ushort CupId);

private:
	CPointerArray<CCup>	m_CupList;
	CPointerArray<CCharityShield>	m_CharityShieldList;
};
#endif
