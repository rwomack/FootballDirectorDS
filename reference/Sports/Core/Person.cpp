
 /*

    Class:- Person


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


	const int				HIGHMORALE = 90;
	const int				LOWMORALE = 25;
	const double			HIGHMORALEMULTIPLIER = 1.05;
	const double			LOWMORALEMULTIPLIER = .98;

	const ushort			MINWEEKSSINCESIGNEDCONTRACT = 12;
    const ushort			FORENAMELEN = 1;
    const ushort			SURNAMELEN = 20;

	static CString			t_StrName;


/*------------------------------------------------------------------------------
	Method:   	CPersonName::CPersonName
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CPersonName::CPersonName()
	: m_ForenameID(0)
    , m_nSurnameID(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::operator=
	Access:    	public 
	Parameter: 	const CPersonName & p
	Returns:   	CPersonName&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPersonName& CPersonName::operator = (const CPersonName &p)
{
	return DoCopyName(&p);
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::DoCopyName
	Access:    	public 
	Parameter: 	const CPersonName * _PersonName
	Returns:   	CPersonName&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPersonName& CPersonName::DoCopyName(const CPersonName* _PersonName /*= null*/)
{
    m_ForenameID = _PersonName->m_ForenameID;
    m_nSurnameID =  _PersonName->m_nSurnameID;
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	CExtString tForename;
	tForename.ReadFromResource(_Resource, FORENAMELEN);
	m_ForenameID = WorldData().DoFindForenameID(tForename);
    m_nSurnameID = _Resource.ReadUInt16();
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::GetSurname
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPersonName::GetSurname()
{
	return WorldData().GetSurname(m_nSurnameID);
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::GetForename
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPersonName::GetForename()
{
	return WorldData().GetForename(m_ForenameID);
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::SetSurname
	Access:    	public 
	Parameter: 	const CString & NewName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::SetSurname(const CString& NewName)
{
	m_nSurnameID = WorldData().DoFindSurnameID(NewName);
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::SetForename
	Access:    	public 
	Parameter: 	const CString & NewName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::SetForename(const CString& NewName)
{
	m_ForenameID = WorldData().DoFindForenameID(NewName);
}



/*------------------------------------------------------------------------------
	Method:   	CPersonName::DoGenerateRandomName
	Access:    	public 
	Parameter: 	CCountry & _rCountry
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::DoGenerateRandomName(CCountry& _rCountry)
{
	if (_rCountry.GetRanking() == 2)
	{
		//@@aCountry = WorldData().pRandomCountryWithPlayers();
	}
	DoGenerateRandomForeName(_rCountry);
	DoGenerateRandomSurName(_rCountry);
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::DoGenerateRandomForeName
	Access:    	public 
	Parameter: 	CCountry & rCountry
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::DoGenerateRandomForeName(CCountry& rCountry)
{
    CPlayer* pPlayer = rCountry.GetActiveCountry().DoFindRandomPlayer();
    m_ForenameID = pPlayer->GetForenameID();
 }


/*------------------------------------------------------------------------------
	Method:   	CPersonName::DoGenerateRandomSurName
	Access:    	public 
	Parameter: 	CCountry & rCountry
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::DoGenerateRandomSurName(CCountry& rCountry)
{
    CPlayer* pPlayer;
	do
	{
		pPlayer = rCountry.GetActiveCountry().DoFindRandomPlayer();
		m_nSurnameID = pPlayer->GetSurnameID();
	}
	while(WorldData().GetSurname(m_nSurnameID).IsEmpty() == TRUE);
 }


/*------------------------------------------------------------------------------
	Method:   	CPersonName::GetName
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CPersonName::GetName()
{
    // If the forename has '@' as the first char, just copy the surname
	CString tForename = GetForename();
    if (tForename.IsEmpty() == FALSE && tForename != TEXT("@"))
	{
        t_StrName.Format(TEXT("%s.%s"),  tForename, GetSurname());
	}
	else
	{
        t_StrName = GetSurname();
	}
	return t_StrName;
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::GetForenameID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPersonName::GetForenameID() const                      
{ 
	return m_ForenameID; 
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::GetSurnameID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPersonName::GetSurnameID() const                       
{ 
	return m_nSurnameID; 
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::SetSurname
	Access:    	public 
	Parameter: 	const ushort _NewNameID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPersonName::SetSurname(const ushort _NewNameID)			
{ 
	m_nSurnameID = _NewNameID; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //---------------------------------------------------------------------------------------------------------------------------
CPerson::CPerson()
	: m_nNationalityID(0)
    , m_Wage(0)
    , m_Temperament(0)
    , m_Morale(0)
    , m_WeeksHere(0)
    , m_WeeksSinceSignedNewContract(0)
    , m_ContractRenewalStatus(NONEINPROGRESS)
{
    m_Contract = WorldData().GetCurrentDate().GetDate();
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::CPerson
	Access:    	public 
	Parameter: 	const CString & ForeName
	Parameter: 	const ushort SurName
	Parameter: 	const byte Age
	Parameter: 	const byte Nation
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPerson::CPerson(const CString& ForeName, const ushort SurName, const byte Age, const byte Nation)
	: m_Wage(0)
    , m_Temperament(0)
    , m_Morale(0)
    , m_WeeksHere(0)
    , m_WeeksSinceSignedNewContract(0)
    , m_ContractRenewalStatus(NONEINPROGRESS)
{
 	EXT_ASSERT(Age >= 16);
	OnInitialise(ForeName, SurName, Age, Nation);
    m_Contract = WorldData().GetCurrentDate().GetDate();
	m_Contract.IncreaseLength(1 + RandomNumber.IntLessThan(4));      // Seasons
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::operator=
	Access:    	public 
	Parameter: 	const CPerson & p
	Returns:   	CPerson&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPerson& CPerson::operator = (const CPerson& p)
{
	m_DateOfBirth = p.m_DateOfBirth;
	m_nNationalityID = p.m_nNationalityID;
    m_Wage = p.m_Wage;
    m_Contract = p.m_Contract;
    m_ContractRenewalStatus = p.m_ContractRenewalStatus;
    m_WeeksHere = p.m_WeeksHere;
	m_WeeksSinceSignedNewContract = p.m_WeeksSinceSignedNewContract;
    m_Temperament = p.m_Temperament;
    m_Morale = p.m_Morale;
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoCopy
	Access:    	public 
	Parameter: 	const CPerson * _Person
	Returns:   	CPerson&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPerson& CPerson::DoCopy(const CPerson *_Person /*= null*/)
{
    DoCopyName(_Person);
	m_DateOfBirth = _Person->m_DateOfBirth;
	m_nNationalityID = _Person->m_nNationalityID;
    m_Wage = _Person->m_Wage;
    m_Contract = _Person->m_Contract;
    m_ContractRenewalStatus = _Person->m_ContractRenewalStatus;
    m_WeeksHere = _Person->m_WeeksHere;
	m_WeeksSinceSignedNewContract= _Person->m_WeeksSinceSignedNewContract;
    m_Temperament = _Person->m_Temperament;
    m_Morale = _Person->m_Morale;
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoGenerateTemperament
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoGenerateTemperament()
{
	m_Temperament = RandomNumber.IntLessThan(9);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoRaiseMorale
	Access:    	public 
	Parameter: 	const byte _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoRaiseMorale(const byte _Amount)
{
	m_Morale = min(m_Morale + _Amount, MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoLowerMorale
	Access:    	public 
	Parameter: 	const byte _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoLowerMorale(const byte _Amount)
{
	m_Morale = max(m_Morale - _Amount, MINIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Parameter: 	const bool _IsManager
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoInitialiseFromResource(CBinaryReader& _Resource, const bool _IsManager /*= false*/)
{
	CPersonName::DoInitialiseFromResource(_Resource);
    m_DateOfBirth.ReadFromResource(_Resource);
//  	DEBUG_OUTPUT(("%s, Age : %d"), (GetName(), GetAge()));
	EXT_ASSERT(GetAge() < 70);

    m_nNationalityID = _Resource.ReadUInt16();

	if (_IsManager == false)
	{
//  	DEBUG_OUTPUT(("%s, Nationality : %d,  Age : %d"), (GetName(), m_nNationalityID, GetAge()));
		EXT_ASSERT(GetAge() >= MINIMUPLAYERAGE);
		ushort nContractLength = 1 + RandomNumber.IntLessThan(5);
		if (GetAge() > 34)
		{
			nContractLength = max(1, nContractLength - 3);
		}
		m_Contract.IncreaseLength(nContractLength);
		DoGenerateTemperament();
		m_WeeksHere = MINWEEKSATCLUB + 1;
		m_WeeksSinceSignedNewContract = MINWEEKSSINCESIGNEDCONTRACT;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoGenerateDateOfBirth
	Access:    	public 
	Parameter: 	const ushort _Years
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoGenerateDateOfBirth(const ushort _Years)
{
	m_DateOfBirth.DoGenerateDateInPast(WorldData().GetCurrentDate(), _Years);
}



/*------------------------------------------------------------------------------
	Method:   	CPerson::GetAge
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPerson::GetAge()
{
	return m_DateOfBirth.DoCalculateDifferenceInYears(WorldData().GetCurrentDate());
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetContractRenewalStatus
	Access:    	public 
	Returns:   	eContractOfferResponse
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eContractOfferResponse CPerson::GetContractRenewalStatus() const
{ 
	return (eContractOfferResponse)m_ContractRenewalStatus; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetMorale
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPerson::GetMorale() const
{ 
	return m_Morale; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetTemperament
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPerson::GetTemperament() const
{ 
	return m_Temperament; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetWeeklyWage
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPerson::GetWeeklyWage() const
{ 
	return m_Wage / 52; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetWeeksHere
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CPerson::GetWeeksHere() const
{ 
	return m_WeeksHere; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::WillNegotiateContract
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPerson::WillNegotiateContract()
{
	if (m_WeeksSinceSignedNewContract >= MINWEEKSSINCESIGNEDCONTRACT)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::OnInitialise
	Access:    	public 
	Parameter: 	const CString & _ForeNameID
	Parameter: 	const ushort _SurNameID
	Parameter: 	const CDate _DateOfBirth
	Parameter: 	const byte _Nation
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::OnInitialise(const CString& _ForeNameID, const ushort _SurNameID, const CDate _DateOfBirth, const byte _Nation)
{
	CPersonName::SetForename(_ForeNameID);
	CPersonName::SetSurname(_SurNameID);
    m_DateOfBirth = _DateOfBirth;
	EXT_ASSERT(GetAge() >= MINIMUPLAYERAGE);
    SetNationalityID(_Nation);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoCreateYouth
	Access:    	public 
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoCreateYouth(CCountry& _Country)
{
	static const byte nYouthRandomAge[] =
	{
		16, 17, 17, 16, 17,
		16, 17, 16, 16, 17, 16
	};
	DoCreate(nYouthRandomAge[RandomNumber.IntLessThan(10)], 0, _Country);
	EXT_ASSERT(GetAge() > 15 && GetAge() < 19);
	SetWage(YOUTHWEEKLYWAGE);
	m_Contract.IncreaseLength(18 - GetAge());
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoCreatePlayer
	Access:    	public 
	Parameter: 	const uint _Wage
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoCreatePlayer(const uint _Wage, CCountry& _Country)
{
	EXT_ASSERT(_Wage > 0);
	m_Wage = _Wage;
	DoCreate(19 + RandomNumber.IntLessThan(15), static_cast<ushort>(1 + RandomNumber.IntLessThan(2)), _Country);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoCreate
	Access:    	public 
	Parameter: 	const byte _AgeInYears
	Parameter: 	const ushort _Years
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoCreate(const byte _AgeInYears, const ushort _Years, CCountry& _Country)
{
	CPersonName::DoGenerateRandomName(_Country);
    DoResetVariables();
	//DEBUG_OUTPUT(("Creating person aged %d"),  (_AgeInYears));
	do 
	{
		DoGenerateDateOfBirth(_AgeInYears);
	} 
	while(GetAge() != _AgeInYears);
	//DEBUG_OUTPUT(("Created person aged %d"), (GetAge()));
	m_Contract.IncreaseLength(_Years);
	DoGenerateTemperament();
    SetNationalityID(_Country.GetID());
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::AgeStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPerson::AgeStr()
{
    CString str;
    str.Format(TEXT("%d"), static_cast<int>(GetAge()));
    return str;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::ContractStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPerson::ContractStr()
{
    return m_Contract.LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::ContractShortStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPerson::ContractShortStr()
{
    return m_Contract.ShortDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::IsRetired
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CPerson::IsRetired() const
{
 	if (GetContractRenewalStatus() == RETIRED)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoIncrementWeeksHere
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoIncrementWeeksHere()
{ 
	m_WeeksHere++; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoResetVariables()
{
    m_DateOfBirth = 0;
    m_nNationalityID = 0;
    m_Wage = 0;
    DoClearNewContract();
    m_WeeksHere = 0;
    m_Temperament = 0;
    m_Morale = STARTMORALE;
}


 //------------------------------------------------------------------------------------------------------
CString CPerson::MoraleStr()
{
	static int TextStr[] =
	{
		IDS_MORALETEXT1,
		IDS_MORALETEXT2,
		IDS_MORALETEXT3,
		//IDS_MORALETEXT4,
		//IDS_MORALETEXT5,
	};
	CString str;
	str.LoadString(TextStr[2]);
    return str;
 }


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoGiveNewContract
	Access:    	public 
	Parameter: 	int _Seasons
	Parameter: 	const uint _Wage
	Parameter: 	const bool _bExtendExisting
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoGiveNewContract(int _Seasons, const uint _Wage /*= 0*/, const bool _bExtendExisting /*= true*/)
{
	if (_bExtendExisting == false)
	{
		m_Contract.IncreaseLength(0);
	}
	m_Contract.IncreaseLength(_Seasons);
	if (_Wage > 0)
	{
		SetWage(_Wage);
	}
	DoClearNewContract();
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoClearNewContract
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoClearNewContract()
{
	m_ContractRenewalStatus = NONEINPROGRESS;
	m_WeeksSinceSignedNewContract = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoEndOfSeason
	Access:    	public 
	Returns:   	bool - true if needs removing
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPerson::DoEndOfSeason()
{
	if (m_Contract.HasExpired() == true)
	{
		return false;
	}
    return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoWeeklyTasks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoWeeklyTasks()
{
	DoIncrementWeeksHere();
	if (m_WeeksSinceSignedNewContract < 0X0ff)
	{
		m_WeeksSinceSignedNewContract++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::NewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Start of season clear down
------------------------------------------------------------------------------*/
void CPerson::DoNewSeason()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetNationalityID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPerson::GetNationalityID() const                          
{ 
	return m_nNationalityID; 
}


 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::NationalityStr()
{
	return WorldData().GetCountry(GetNationalityID()).NationalityStr();
 }


 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::TemperamentStr()
{
    EXT_ASSERT(m_Temperament >= 0 && m_Temperament < 10);

    CString str;
    static int TemperTextIds[] =
	{
        IDS_TEMPERTEXT1, IDS_TEMPERTEXT2,
        IDS_TEMPERTEXT3, IDS_TEMPERTEXT4,
        IDS_TEMPERTEXT5
    };
    str.LoadString(TemperTextIds[GetTemperament() >> 1]);
    return str;
 }


/*------------------------------------------------------------------------------
	Method:   	CPerson::DoUpdateContractOffered
	Access:    	public 
	Parameter: 	CContract & _ContractOffered
	Returns:   	void
	Qualifier: 	
	Purpose:	Has accepted a new contract
------------------------------------------------------------------------------*/
void CPerson::DoUpdateContractOffered(CContract& _ContractOffered)
{
	m_WeeksSinceSignedNewContract = 0;

	// not got a contract at the moment
    m_Contract.IncreaseLength(_ContractOffered.GetContractLength());
    m_Wage = _ContractOffered.GetWage();
	EXT_ASSERT(m_Wage > 0);
	m_ContractRenewalStatus = NONEINPROGRESS;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::ContractValue
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:	How much will it cost to release this person from his contract?
------------------------------------------------------------------------------*/
uint CPerson::ContractValue()
{
	return 	m_Contract.WeeksRemaining() * GetWage();
}


 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::ContractValueStr(CCountry& rCountry, int TextId)
{
    CString sRet, str;
	EXT_ASSERT(ContractValue() > 0);
	if (TextId == NOTFOUND)
	{
		sRet = rCountry.GetCurrency().GetForeignStr(ContractValue());
	}
	else
	{
		sRet.Format(TextId, GetName(), rCountry.GetCurrency().GetForeignStr(ContractValue()));
	}
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetWage
	Access:    	public 
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CPerson::GetWage() const                                  
{ 
	return m_Wage; 
}


 // Returns the current weekly wage of the person in the currency of the
 // country currently employed in
 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::WageStr(CCountry& rCountry)
{
    return rCountry.GetCurrency().GetForeignStr(GetWage());
}


 // Returns the current annual wage of the player in the currency of the
 // country currently employed in
 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::SalaryStr(CCountry& rCountry)
{
    return rCountry.GetCurrency().GetForeignStr(GetWage() * 52);
}


 //---------------------------------------------------------------------------------------
void CPerson::RaiseMorale(const byte x)
{
    m_Morale += x;
    m_Morale = min(max(m_Morale, MINIMUMMORALE), MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetMoraleEffect
	Access:    	public 
	Returns:   	double
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
double CPerson::GetMoraleEffect() const
{
	if (m_Morale > HIGHMORALE)
	{
		return HIGHMORALEMULTIPLIER;
	}
	if (m_Morale < LOWMORALE)
	{
		return LOWMORALEMULTIPLIER;
	}
	return 1;
}


 //---------------------------------------------------------------------------------------
void CPerson::LowerMorale(const byte x)
{
    m_Morale -= x;
    m_Morale = min(max(m_Morale, MINIMUMMORALE), MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetContractEndDate
	Access:    	public 
	Returns:   	CContractEndTime&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractEndTime& CPerson::GetContractEndDate()
{
	return m_Contract;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::AddWeeksHere
	Access:    	public 
	Parameter: 	const ushort _Weeks
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::AddWeeksHere(const ushort _Weeks)
{
	m_WeeksHere += _Weeks;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::SetWeeksHere
	Access:    	public 
	Parameter: 	const ushort _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::SetWeeksHere(const ushort _Value)
{
	m_WeeksHere = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::SetContractRenewalStatus
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::SetContractRenewalStatus(const byte _Value)
{ 
	EXT_ASSERT(_Value < MAXCONTRACTSTATUS);
	m_ContractRenewalStatus = _Value; 
}


 //---------------------------------------------------------------------------------------
void CPerson::SetTemperament(const byte aValue)				
{ 
	m_Temperament = aValue; 
}


 //---------------------------------------------------------------------------------------
void CPerson::SetMorale(const byte aValue)					
{ 
	m_Morale = aValue; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::SetNationalityID
	Access:    	public 
	Parameter: 	const ushort _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::SetNationalityID(const ushort _Value)
{
	m_nNationalityID = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::SetWage
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::SetWage(const uint _Value)
{ 
	m_Wage = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Read(this, sizeof this);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Write(this, sizeof this);
}