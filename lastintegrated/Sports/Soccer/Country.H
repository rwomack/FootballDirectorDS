
/*

	C++ Classes

	Country class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#ifndef __COUNTRY_H__
#define __COUNTRY_H__


class CCountry
{
	// Federation Cup berth descriptor types
	enum
	{
		BERTH_LEAGUECHAMPIONS,
		BERTH_LEAGUERUNNERSUP,
		BERTH_DOMESTICCUP1WINNERS,
		BERTH_DOMESTICCUP2WINNERS,
		BERTH_HIGHESTDIVSIONPLACE,
		BERTH_LASTYEARSWINNER,
		BERTH_CHAMPIONSQUALIFYINGLOSERS,
		BERTH_PLAYOFFWINNERS
	};

public:

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					AddDivision(const ushort _DivisionNum);
	byte					GetID() const;
    byte					GetFinancialRatioPercentage() const;
	CExtString&				NationalityStr();
	void					SetRanking();
    byte					GetFederationRanking();
	byte					GetRanking();
	void					SetFederationRanking(const byte _FederationRanking);
	void					SetRanking(const byte _Ranking);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	bool					IsActive();
	double					GetLoanInterestWeeklyMultiplier();
	CActiveCountry&			GetActiveCountry();
	CCurrency&				GetCurrency();
    void                    SetDivn(int Pos, CDivision* pDivision);
    ushort					TotalClubs();
    ushort					TotalDivisions();
#ifdef COUNTRYNAME
	CExtString&				GetName();
#endif
#ifdef ACTIVEFEDERATIONS
	byte					GetFederationID() const;
	CFederation&			GetFederation();
#endif

	// Variables
private:
	byte					m_FederationRanking;
	byte					m_Ranking;
	byte					m_FinancialRatioPercentage;
	byte					m_ID;
#ifdef COUNTRYNAME
    CExtString	            m_strName;
#endif
	CExtString	            m_strNationality;
#ifdef USE_CURRENCIES
    byte					m_CurrencyId;
#endif
#ifdef ACTIVEFEDERATIONS
    byte					m_FederationID;
#endif
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CCountryList : public CExtArray
{
public:
	CCountryList();
    ~CCountryList();	// Destructor

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoInitialise();
	void					AddActiveCountry(CBinaryReader& _Resource, const byte _ID);
	void					BuildCupList(CCompetitionList* _pCupList);
	void					BuildThisWeekCupList(CCalendar* _pCurrentDate, Vector<CCup*>* _pCupList);
	void					DeleteActiveCountryList();
    void                    PostInitialise();
    void                    DoInitialiseFromResource(CBinaryReader& _Resource);
	void					DoDailyClearDown();
	bool					DoEndOfSeason();
	void					DoEndOfWeek(const bool _bNoWarnings = true);
	void					DoMidWeekTasks(const byte _MatchNumber);
	void					DoNewSeason();
	CCountry&				GetAt(const ushort _ElementNumber);
	CCountry*				GetFromID(ushort _ID);
    void                    PlayMissedMatches();
	CActiveCountry&			GetActiveCountry(const byte _ID);
	CCountry&				DoSelectRandomActive();

	// Variables
private:
	CCountry*				m_pList;
	Vector<CActiveCountry*> m_ActiveCountryList;
};

#endif
