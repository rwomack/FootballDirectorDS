

 /*

	Person class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


 #ifndef __PERSON_H__
 #define __PERSON_H__


    class CContract;



class CPersonName
{
public:
	CPersonName();
	
    CPersonName& operator = (const CPersonName &p);

	// Methods
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
    CString&     			GetName();
	ushort					GetForenameID() const;
    CString					GetForename();   
    CString					GetSurname();
	ushort					GetSurnameID() const;                       
    CString					GetUpperCaseSurname();
    void					SetForename(const CString& astrNewName);
    void					SetSurname(const CString& astrNewName);
	void					SetSurname(const ushort _NewNameID)			;
	CPersonName&			DoCopyName(const CPersonName* _PersonName = null);
	void					DoGenerateRandomName(CCountry& _rCountry);
    void        			DoGenerateRandomSurName(CCountry& rCountry);
    void        			DoGenerateRandomForeName(CCountry& rCountry);

private:
    ushort					m_ForenameID;
    ushort					m_nSurnameID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	enum eContractOfferResponse
	{
		NONEINPROGRESS,
		ACCEPTED,
		NEGOTIATING,
		REFUSED,
		MORESIGNON,
		LESSSIGNON,
		MOREWAGE,
		LESSWAGE,
		LONGERCONTRACT,
		SHORTERCONTRACT,
		LISTED,
		CLUBREFUSED,
		RETIRING,
		RETIRED,

		MAXCONTRACTSTATUS
	};


	static const byte		MAXIMUMMORALE = 99;
	static const byte		STARTMORALE = 80;
	static const byte		MINIMUMMORALE = 0;
	static const ushort		MINWEEKSATCLUB = 6;

	
class CPerson : public CPersonName
{
    // Variables
private:
	CDate					m_DateOfBirth;
	CContractEndTime		m_Contract;
	uint					m_Wage;
	uint					m_nNationalityID:8;
    uint					m_Temperament:7;
    uint					m_Morale:7;
    uint					m_WeeksHere:10;
    uint					m_WeeksSinceSignedNewContract:10;
    uint					m_ContractRenewalStatus:8;

public:
	CPerson(const CString& ForeName, const ushort SurName, const byte Age, const byte Nation);
    CPerson();

    CPerson&				operator = (const CPerson &p);
	CPerson&				DoCopy(const CPerson *_Person = null);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	 // Data Access Methods
	byte					GetMorale() const;
	byte					GetTemperament() const;
	ushort					GetNationalityID() const;
	int						GetWeeklyWage() const;          
	int						GetWage() const;
	int						GetAge();                                   
	eContractOfferResponse	GetContractRenewalStatus() const;
	ushort					GetWeeksHere() const;
    CContractEndTime&		GetContractEndDate();
	void					SetContractRenewalStatus(const byte _Value);	
    void        			SetTemperament(const byte aValue);				
    void        			SetMorale(const byte aValue);					
	void					SetWeeksHere(const ushort _Value);
	void					SetNationalityID(const ushort _Value);
	void					SetWage(const uint _Value);					

	// Methods
	void					AddWeeksHere(const ushort _Weeks);
    CString     			AgeStr();
	void        			DoClearNewContract();
	void 					DoCreate(const byte _AgeInYears, const ushort _Years, CCountry& _Country);
	void 					DoCreatePlayer(const uint _Wage, CCountry& _Country);
	void 					DoCreateYouth(CCountry& _Country);
	void					DoGenerateDateOfBirth(const ushort _Years);
	void					DoGenerateTemperament();
	void					DoGiveNewContract(int _Seasons, const uint _Wage = 0, const bool _bExtendExisting = true);
	void					DoLowerMorale(const byte _Amount);
	void					DoUpdateContractOffered(CContract& _ContractOffered);
	void					DoRaiseMorale(const byte _Amount);
	double					GetMoraleEffect() const;
	void					DoUpdateContractDetails(CContract* _PlayerContract);
    CString     			ContractValueStr(CCountry& rCountry, int TextId = NOTFOUND);
    uint					ContractValue();
	CString					ContractStr();
	CString					ContractShortStr();
	void					DoWeeklyTasks();
    bool        			DoEndOfSeason();
	void					DoIncrementWeeksHere();						
	void					DoNewSeason();
	bool					IsRetired() const;
	void					DoInitialiseFromResource(CBinaryReader& _Resource, const bool _IsManager = false);
	void					OnInitialise(const CString& _ForeNameID, const ushort _SurNameID, const CDate _DateOfBirth, const byte _Nation);
    void        			LowerMorale(const byte x);
    CString     			MoraleStr();
    void        			RaiseMorale(const byte x);
	CString					SalaryStr(CCountry& rCountry);
    CString     			TemperamentStr();
    CString     			WageStr(CCountry& rCountry);
    void        			DoResetVariables();
    CString     			NationalityStr();
    void        			NewContractRequest();
	bool					WillNegotiateContract();
};
#endif
