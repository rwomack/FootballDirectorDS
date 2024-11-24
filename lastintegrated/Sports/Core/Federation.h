
/*

	Federation class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


 #ifndef __FEDERATION_H__
 #define __FEDERATION_H__


class CFederation
{
public:
    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	void					DoAddCountry(ushort _CountryID);
	void					DoAddCup(byte _CupID);
	byte DoCountCountryEntrants(CActiveCountry& _Country);
	CClub*					DoFindRandomClub();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	void DoListCountryEntrants(CActiveCountry& _Country, CNumericArray<ushort>* _TheList);
	ushort					GetCountryID(ushort _Country);
	CCountry&				GetCountry(const ushort _ListPos);
	byte					GetID() const;
	CCountry&				GetRankedCountry(const int _Ranking);
	ushort					GetRankedCountryID(const int Ranking);
	CCountry&				GetRandomCountry(CCountry* _Country1, CCountry* _Country2);
#ifdef ACTIVEFEDERATIONCUPS
	CFederationCup&			GetFederationCup(ushort _CupID);
	bool					HasEnteredFederationCup(ushort _ClubID);
	int						TotalCups();
#endif
#ifdef ACTIVEFEDERATIONS
	CString					GetName();
#endif

    // Variables
private:
	byte					m_ID;
    CNumericArray<ushort>	m_Countries;
#ifdef ACTIVEFEDERATIONS
	CExtString				m_Name;
#endif
#ifdef ACTIVEFEDERATIONCUPS
    CNumericArray<byte>		m_CupList;
#endif
 };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CFederationList : public CExtArray
{
public:
    CFederationList();		// Constructor
	~CFederationList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoCountryJoinFederation(ushort _ID,ushort _FederationID);
	CFederation&			GetAt(const ushort _ElementNumber);
	CFederation&			Find(CString _FedName);

	// Variables
private:
	CFederation*			m_pFederationList;
};
#endif




















































