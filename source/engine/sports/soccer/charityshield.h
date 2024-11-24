

 /*

	CharityShield.h: interface for the CCharityShield class.


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/


#if !defined(AFX_CharityShield_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_CharityShield_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_



const ushort NUMBER_CHARITY_SHIELD_CLUBS = 2;


class CCharityShield : public CCup
{
    // Variables
private:
	ushort					m_ID;
    ClubID					m_TheClubs[NUMBER_CHARITY_SHIELD_CLUBS];

public:

    // IO Methods
  void						Write(CDataFile* File);
  void						Read(CDataFile* File);

    // Methods
	bool					ClubQualified(ClubID _ClubID);
	void                    DeleteAll();
	void					DoAddClub(ClubID _ClubID);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	bool					DoPlayMatches(const bool _ShowSummary);
    void                    DoDraw();
	void					DoEndOfSeason();
	CCup*					GetCup();
	ushort					GetID() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
class CCharityShieldList : public CExtArray
{
	// Variables
private:
	CCharityShield*			m_pList;

public:
    CCharityShieldList();
    ~CCharityShieldList();

    // IO Methods
	void					Write(CDataFile* File);
	void					Read(CDataFile* File);

    // Methods
	CString					DoFindCompetitionTitleForFixture(CFixture* _Fixture);
	bool					IsClubMatchToday(const CCalendar& _CurrentDate, ClubID _ClubID, CMatchInfo& _Match);
	CCharityShield*			CountryCharityShield(ushort CountryId);
	void					DoDraws();
	CCharityShield*			DoFind(ushort _ShieldID);
	CString					DoFindNeutralVenueNameForFixture(CFixture* _Fixture);
	bool					DoPlayMatches(const bool _ShowSummary);
};
#endif // !defined(AFX_CharityShield_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_)
