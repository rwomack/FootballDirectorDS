
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
	void						DoAddCupQualifiers(const ushort _Club1ID, const ushort _Club2ID, const ushort _Club3ID, const ushort _Club4ID);
	void						DoAddQualifyingClubID(const ushort _ClubID);
	bool						DoCheckIfQualified(const ushort _ClubID);
	byte						DoCountCountryEntrants(const ushort _FirstClubID, const ushort _LastClubID);
	void						DoDraw();
	void						DoEndOfSeason(void);
	void						DoInitialiseSeasonNeutralVenue();
	void						DoInitialiseFromResource(CBinaryReader& _Resource, const byte _ID, ushort* _NeutralStadiumIDList);
	void						DoListCountryEntrants(const ushort _FirstClubID, const ushort _LastClubID, CNumericArray<ushort>* _TheList);
	void						DoNewSeason(void);
	void						DoReplaceClub(ushort _ClubIDToReplace, ushort _ClubIDReplaceWith);
	CFederation&				GetFederation(void);

	// Variables
protected:
	byte						m_ID;
	bool						m_AllowSameCountryGroups;
	ushort						m_FederationID;
	CNumericArray<ushort>		m_BerthClubIDs;
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
	bool						DoAddQualifier(const ushort _CupID, const ushort _ClubID);
	void						DoBuildNameList(CSortedStringList& _theList);
	bool						DoCheckIfInAnyCup(const ushort _ClubIDToFind);
    void						DoDraws(void);
	void						DoEndOfSeason();
	CString						DoFindCompetitionTitleForFixture(CFixture* _Fixture);
	void						DoInitialiseFromResource();
	void						DoNewSeason(void);
	bool						DoPlayMatches(bool _ViewEm);
	void						DoReplaceClub(ushort _ClubIDToReplace, ushort _ClubIDReplaceWith);
	uint						FindCup(CCup* _TheCup);
	CFederationCup*				GetAt(const ushort _Index);
	bool						IsClubMatchToday(const CCalendar& _CurrentDate, ushort _ClubID, CMatchInfo& _Match);

protected:
	CFederationCup*				m_pList;
};
#endif
