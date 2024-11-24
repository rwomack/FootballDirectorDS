
/*

	MatchInfo.h: interface for the CMatchInfo class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __MATCHINFO_H__
#define __MATCHINFO_H__

#include <SoccerDefines.h>


class CMatchInfo : public CAvailableSubstitutes
{
	// Variables
private:
	CString					m_CompTitle;
	CCup*					m_pCup;
	CFixture*				m_pFixture;
	ClubID					m_HomeClubID;
	ClubID					m_AwayClubID;
	ushort					m_StadiumID;
	CCalendar				m_TheDate;
	eMatchCompetitionType	m_eCompetitionType;
#ifdef	ACTIVEREFEREES
	ushort					m_nReferee;
#endif

public:
	CMatchInfo();
	CMatchInfo(const eMatchCompetitionType _eCompetitionType, const byte _Select, const byte _Use);
	CMatchInfo(const ClubID _HomeClubID, const ClubID _AwayClubID);

	void					DoInitialiseFriendlyMatch(const ClubID _HomeClubID, const ClubID _AwayClubID);
	void					DoInitialiseClubs(const ClubID _HomeClubID, const ClubID _AwayClubID);
	void					DoInitialiseLeagueMatch(const ClubID _HomeClubID, const ClubID _AwayClubID, CDivision& _Division, CFixture* _Fixture);
	ClubID					GetAwayClubID() const;								
	CString					GetAwayName();
	CString&				GetCompetitionName(); 
	CString&				GetCompetitionTitle();								
	CCup*					GetCup();										
	CFixture*				GetFixture();									
	ClubID					GetHomeClubID() const;				
	CString					GetHomeName();
	CClub*					GetOpponentClub(CClub* _TheClub);
	uint					GetStadiumCapacity() const;							
	CString&				GetStadiumName();								
	CCalendar&				GetTheDate();								
	eMatchCompetitionType	GetCompetitionType() const;
	bool					IsHomeClub(CClub* _Club);
	void					SetCompetitionTitle(const CString& _Str);							
	void					SetCup(CCup* _Cup);								
	void					SetTheDate(CCalendar& x);						
	void					SetFixture(CFixture* _nFixtureID);						
	void					SetStadiumID(const ushort _ID);						
	void 					SetCompetitionType(eMatchCompetitionType _eCompetitionType);
#ifdef	ACTIVEREFEREES
	void					SetReferee(const ushort nReferee);				
#endif
};
#endif
