
 /*

    Class:- Person


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


	const int				HIGHMORALE = 90;
	const int				LOWMORALE = 25;
  	const float       		HIGHMORALEMULTIPLIER = 1.05f;
  	const float       		LOWMORALEMULTIPLIER = .98f;

	const ushort			MINWEEKSSINCESIGNEDCONTRACT = 12;
    const ushort			FORENAMELEN = 1;
    const ushort			SURNAMELEN = 20;

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
CString CPersonName::GetSurname() const
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
CString CPersonName::GetForename() const
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
CString CPersonName::GetName() const
{
    // If the forename has '@' as the first char, just copy the surname
	CString tForename = GetForename();
	CString tReturnValue;
    if (tForename.IsEmpty() == FALSE && tForename != TEXT("@"))
	{
        tReturnValue.Format(TEXT("%s.%s"),  tForename.CStr(), GetSurname().CStr());
	}
	else
	{
        tReturnValue = GetSurname();
	}
	return tReturnValue;
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

void CPersonName::SetForename(const ushort _NewNameID)
{
	m_ForenameID = _NewNameID;
}

/*------------------------------------------------------------------------------
	Method:   	CPersonName::Write
	Access:    	public
	Parameter: 	CDataFile *_pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Serializes the CPersonName.
------------------------------------------------------------------------------*/
void CPersonName::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_ForenameID, sizeof(m_ForenameID));
	_pDataFile->Write(&m_nSurnameID, sizeof(m_nSurnameID));
}


/*------------------------------------------------------------------------------
	Method:   	CPersonName::Read
	Access:    	public
	Parameter: 	CDataFile *_pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Deserializes the CPersonName.
------------------------------------------------------------------------------*/
void CPersonName::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_ForenameID, sizeof(m_ForenameID));
	_pDataFile->Read(&m_nSurnameID, sizeof(m_nSurnameID));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //---------------------------------------------------------------------------------------------------------------------------
CPerson::CPerson()
{
	m_bitfields.m_nNationalityID = 0;
	m_bitfields.m_Temperament = 0;
    m_bitfields.m_Morale = 0;
    m_bitfields.m_WeeksHere = 0;
    m_bitfields.m_WeeksSinceSignedNewContract = 0;
    m_bitfields.m_ContractRenewalStatus = NONEINPROGRESS;
	m_bitfields.m_Wage = 0;

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
{
	m_bitfields.m_nNationalityID = 0;
	m_bitfields.m_Temperament = 0;
    m_bitfields.m_Morale = 0;
    m_bitfields.m_WeeksHere = 0;
    m_bitfields.m_WeeksSinceSignedNewContract = 0;
    m_bitfields.m_ContractRenewalStatus = NONEINPROGRESS;
	m_bitfields.m_Wage = 0;
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
	// Call base class operator.
	CPersonName::operator=(p);

	m_bitfields = p.m_bitfields;
#ifdef STOREFULLDATEOFBIRTH
	m_DateOfBirth = p.m_DateOfBirth;
#endif
    m_Contract = p.m_Contract;
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
	m_bitfields = _Person->m_bitfields;
#ifdef STOREFULLDATEOFBIRTH
	m_DateOfBirth = _Person->m_DateOfBirth;
#endif
    m_Contract = _Person->m_Contract;
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
	// Temperament range is 0-9.
	m_bitfields.m_Temperament = RandomNumber.IntLessThan(10);
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
	m_bitfields.m_Morale = min(m_bitfields.m_Morale + _Amount, MAXIMUMMORALE);
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
	m_bitfields.m_Morale = max(m_bitfields.m_Morale - _Amount, MINIMUMMORALE);
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
#ifdef STOREFULLDATEOFBIRTH
    m_DateOfBirth.ReadFromResource(_Resource);
#else
	CDate cDOB;
	cDOB.ReadFromResource(_Resource);
	m_bitfields.m_YearOfBirth = cDOB.GetYear();
#endif
	EXT_ASSERT(GetAge() < 70);
    m_bitfields.m_nNationalityID = _Resource.ReadUInt16();
	if (_IsManager == false)
	{
		// Is a player
		EXT_ASSERT(GetAge() >= MINIMUPLAYERAGE);
		ushort nContractLength = RandomNumber.IntLessThan(4);
		if (GetAge() > 34)
		{
			nContractLength = RandomNumber.IntLessThan(2);
		}
		if (GetAge() + nContractLength < 41 && nContractLength > 0)
		{
			m_Contract.IncreaseLength(nContractLength);
		}
		DoGenerateTemperament();
		m_bitfields.m_WeeksHere = MINWEEKSATCLUB + 1;
		m_bitfields.m_WeeksSinceSignedNewContract = MINWEEKSSINCESIGNEDCONTRACT;
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
#ifdef STOREFULLDATEOFBIRTH
	m_DateOfBirth.DoGenerateDateInPast(WorldData().GetCurrentDate(), _Years);
#else
	m_bitfields.m_YearOfBirth = WorldData().GetCurrentDate().GetYear() - _Years;
#endif
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
#ifdef STOREFULLDATEOFBIRTH
	return m_DateOfBirth.DoCalculateDifferenceInYears(WorldData().GetCurrentDate());
#else
	return WorldData().GetCurrentDate().GetYear() - m_bitfields.m_YearOfBirth;
#endif
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
	return m_bitfields.m_ContractRenewalStatus;
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
	return m_bitfields.m_Morale;
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
	return m_bitfields.m_Temperament;
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
	return m_bitfields.m_Wage;
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
	return m_bitfields.m_WeeksHere;
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
	if (m_bitfields.m_WeeksSinceSignedNewContract >= MINWEEKSSINCESIGNEDCONTRACT)
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

#ifdef STOREFULLDATEOFBIRTH
	m_DateOfBirth = _DateOfBirth;
#else
	m_bitfields.m_YearOfBirth = _DateOfBirth.GetYear();
#endif
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
	EXT_ASSERT(_Wage < (1 << WAGEBITS));
	m_bitfields.m_Wage = _Wage;
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
	do
	{
		DoGenerateDateOfBirth(_AgeInYears);
	}
	while(GetAge() != _AgeInYears);
//	DEBUG_OUTPUT(("Creating person aged %d"),  (GetAge()));
	if (_Years > 0)
	{
		m_Contract.IncreaseLength(_Years);
	}
//	DEBUG_OUTPUT(("Created person contract length %d"), (m_Contract.SeasonsRemaining()));
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
  Method:     CPerson::ContractMediumStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPerson::ContractMediumStr()
{
    return m_Contract.MediumDateStr();
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
	Method:   	CPerson::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoResetVariables()
{
#ifdef STOREFULLDATEOFBIRTH
    m_DateOfBirth = 0;
#endif
    m_bitfields.m_nNationalityID = 0;
    m_bitfields.m_Wage = 0;
    DoClearNewContract();
    m_bitfields.m_WeeksHere = 0;
    m_bitfields.m_Temperament = 0;
    m_bitfields.m_Morale = STARTMORALE;
#ifndef STOREFULLDATEOFBIRTH
	m_bitfields.m_YearOfBirth = 0;
#endif
	m_Contract = WorldData().GetCurrentDate();
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
		m_Contract.SetEndDate(_Seasons);
	}
	else
	{
		m_Contract.IncreaseLength(_Seasons);
	}
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
	m_bitfields.m_ContractRenewalStatus = NONEINPROGRESS;
	m_bitfields.m_WeeksSinceSignedNewContract = 0;
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

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

/*------------------------------------------------------------------------------
	Method:   	CPerson::DoIncrementWeeksHere
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::DoIncrementWeeksHere()
{
	m_bitfields.m_WeeksHere++;
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
	if (m_bitfields.m_WeeksSinceSignedNewContract < 0X0ff)
	{
		m_bitfields.m_WeeksSinceSignedNewContract++;
	}
}


#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif

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
	return m_bitfields.m_nNationalityID;
}


 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::NationalityStr()
{
	return WorldData().GetCountry(GetNationalityID()).NationalityStr();
 }


 //---------------------------------------------------------------------------------------------------------------------------
CString CPerson::TemperamentStr()
{
    EXT_ASSERT(GetTemperament() >= 0 && GetTemperament() < 10);

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
	m_bitfields.m_WeeksSinceSignedNewContract = 0;
	m_Contract.SetEndDate(_ContractOffered.GetContractLength());
	EXT_ASSERT(_ContractOffered.GetWage() < (1 << WAGEBITS));
    m_bitfields.m_Wage = _ContractOffered.GetWage();
	EXT_ASSERT(m_bitfields.m_Wage > 0);
	m_bitfields.m_ContractRenewalStatus = NONEINPROGRESS;
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
	return m_bitfields.m_Wage;
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
    m_bitfields.m_Morale += x;
    m_bitfields.m_Morale = min(max(m_bitfields.m_Morale, MINIMUMMORALE), MAXIMUMMORALE);
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::GetMoraleEffect
	Access:    	public
  	Returns:    float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CPerson::GetMoraleEffect() const
{
	if (m_bitfields.m_Morale > HIGHMORALE)
	{
		return HIGHMORALEMULTIPLIER;
	}
	if (m_bitfields.m_Morale < LOWMORALE)
	{
		return LOWMORALEMULTIPLIER;
	}
	return 1;
}


 //---------------------------------------------------------------------------------------
void CPerson::LowerMorale(const byte x)
{
    m_bitfields.m_Morale -= x;
    m_bitfields.m_Morale = min(max(m_bitfields.m_Morale, MINIMUMMORALE), MAXIMUMMORALE);
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
	m_bitfields.m_WeeksHere += _Weeks;
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
	m_bitfields.m_WeeksHere = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPerson::SetContractRenewalStatus
	Access:    	public
	Parameter: 	const eContractOfferResponse _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPerson::SetContractRenewalStatus(const eContractOfferResponse _Value)
{
	EXT_ASSERT(_Value < MAXCONTRACTSTATUS);
	m_bitfields.m_ContractRenewalStatus = _Value;
}


 //---------------------------------------------------------------------------------------
void CPerson::SetTemperament(const byte aValue)
{
	EXT_ASSERT((aValue >= 0) && (aValue <= 9));
	m_bitfields.m_Temperament = aValue;
}


 //---------------------------------------------------------------------------------------
void CPerson::SetMorale(const byte aValue)
{
	m_bitfields.m_Morale = aValue;
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
	m_bitfields.m_nNationalityID = _Value;
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
	EXT_ASSERT(_Value < (1 << WAGEBITS));
	m_bitfields.m_Wage = _Value;
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
	CPersonName::Read(_pDataFile);
#ifdef STOREFULLDATEOFBIRTH
	_pDataFile->Read(&m_DateOfBirth, sizeof(m_DateOfBirth));
#endif
	_pDataFile->Read(&m_Contract, sizeof(m_Contract));
	_pDataFile->Read(&m_bitfields, sizeof(m_bitfields));
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
	CPersonName::Write(_pDataFile);
#ifdef STOREFULLDATEOFBIRTH
	_pDataFile->Write(&m_DateOfBirth, sizeof(m_DateOfBirth));
#endif
	_pDataFile->Write(&m_Contract, sizeof(m_Contract));
	_pDataFile->Write(&m_bitfields, sizeof(m_bitfields));
}
