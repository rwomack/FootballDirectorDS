
/*
	File:- FederationCups.h


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#ifndef __FEDERATIONCUP_H__
    #define __FEDERATIONCUP_H__


enum eFederationCup
{
	CHAMPIONSCUP,
	UEFACUP
};

class CFederationCup : public CCup 
{
public:
    // Constructor
    CFederationCup(void);
    ~CFederationCup(void);

    // IO Methods
	void						Read(CDataFile* _pDataFile);
	void						Write(CDataFile* _pDataFile);

	// Other methods
	void						DoAddCupQualifiers(const ClubID _Club1ID, const ClubID _Club2ID, const ClubID _Club3ID, const ClubID _Club4ID);
	void						DoAddQualifyingClubID(const ClubID _ClubID);
	bool						DoCheckIfQualified(const ClubID _ClubID);
	byte						DoCountCountryEntrants(const Vector<ClubID> &cCountryClubs);
	void						DoDraw();
	void						DoEndOfSeason(void);
	void						DoInitialiseSeasonNeutralVenue();
	void						DoInitialiseFromResource(CBinaryReader& _Resource, const byte _ID, ushort* _NeutralStadiumIDList);
	void						DoListCountryEntrants(const Vector<ClubID> &cCountryClubs, CNumericArray<ClubID>* _TheList);
	void						DoNewSeason(void);
	CFederation&				GetFederation(void);

	// Variables
protected:
	byte						m_ID;
	bool						m_AllowSameCountryGroups;
	ushort						m_FederationID;
	CNumericArray<ClubID>		m_BerthClubIDs;
	byte						m_NumberFromGroupsInNextRound;
	byte						m_NumberFromGroupsInNextCup;
	CNumericArray<ushort>		m_NeutralStadiumIDs;

};
///////////////////////////////////////////////////////////////////////////////////////


class CFederationCupList : public CExtArray 
{
public:
    // Constructor
	CFederationCupList ();
	~CFederationCupList ();

    // IO Methods
	void						Read(CDataFile* _pDataFile);
	void						Write(CDataFile* _pDataFile);

	// Other methods
	bool						DoAddQualifier(const ushort _CupID, const ClubID _ClubID);
	void						DoBuildNameList(CStringArray &_theList);
	bool						DoCheckIfInAnyCup(const ClubID _ClubIDToFind);
    void						DoDraws(void);
	void						DoEndOfSeason();
	CString						DoFindNeutralVenueNameForFixture(CFixture* _Fixture);
	CString						DoFindCompetitionTitleForFixture(CFixture* _Fixture);
	void						DoInitialiseFromResource();
	void						DoNewSeason(void);
	bool						DoPlayMatches(bool _ViewEm);
	uint						FindCup(CCup* _TheCup);
	CFederationCup*				GetAt(const ushort _Index);
	bool						IsClubMatchToday(const CCalendar& _CurrentDate, ClubID _ClubID, CMatchInfo& _Match);

protected:
	CFederationCup*				m_pList;
};
#endif
