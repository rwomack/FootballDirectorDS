
/*

	MatchInfo.h: interface for the CMatchInfo class.

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __MATCHINFO_H__
#define __MATCHINFO_H__




class CMatchInfo : public CAvailableSubstitutes
{
	// Variables
private:
	CString					m_CompTitle;
	CCup*					m_pCup;
	CFixture*				m_pFixture;
	ushort					m_HomeClubID;
	ushort					m_AwayClubID;
	ushort					m_StadiumID;
	CCalendar				m_TheDate;
	eMatchCompetitionType	m_eCompetitionType;
#ifdef	ACTIVEREFEREES
	ushort					m_nReferee;
#endif

public:
	CMatchInfo();
	CMatchInfo(const eMatchCompetitionType _eCompetitionType, const byte _Select, const byte _Use);
	CMatchInfo(const ushort _HomeClubID, const ushort _AwayClubID);

	void					DoInitialiseFriendlyMatch(const ushort _HomeClubID, const ushort _AwayClubID);
	void					DoInitialiseClubs(const ushort _HomeClubID, const ushort _AwayClubID);
	void					DoInitialiseLeagueMatch(const ushort _HomeClubID, const ushort _AwayClubID, CDivision& _Division, CFixture* _Fixture);
	ushort					GetAwayClubID() const;								
	CString					GetAwayName();
	CString&				GetCompetitionName(); 
	CString&				GetCompetitionTitle();								
	CCup*					GetCup();										
	CFixture*				GetFixture();									
	ushort					GetHomeClubID() const;				
	CString					GetHomeName();
	CClub*					GetOpponentClub(CClub* _TheClub);
	uint					GetStadiumCapacity() const;							
	CString&				GetStadiumName();								
	CCalendar&				GetTheDate();								
	eMatchCompetitionType	GetCompetitionType() const;
	bool					IsHomeClub(CClub* _Club);
	void					SetCompetitionTitle(CString _Str);							
	void					SetCup(CCup* _Cup);								
	void					SetTheDate(CCalendar& x);						
	void					SetFixture(CFixture* _nFixtureID);						
	void					SetStadiumID(const ushort _ID);						
	void SetCompetitionType(eMatchCompetitionType _eCompetitionType);
#ifdef	ACTIVEREFEREES
	void					SetReferee(const ushort nReferee);				
#endif
};
#endif
