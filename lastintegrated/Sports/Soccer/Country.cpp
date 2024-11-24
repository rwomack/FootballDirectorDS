
 /*
    File:- Country.cpp

    Country list class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "Competition.h"


	enum
	{
		MINIMUMUEFA,
		MEDIUMUEFA,
		FULLUEFA,
		ROTW
	};


/*------------------------------------------------------------------------------
	Method:   	CCountry::NationalityStr
	Access:    	public 
	Returns:   	CExtString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString&	CCountry::NationalityStr()
{
	return m_strNationality;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetFederationRanking
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCountry::GetFederationRanking()
{
	return m_FederationRanking;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetRanking
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCountry::GetRanking()
{
	return m_Ranking;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::SetFederationRanking
	Access:    	public 
	Parameter: 	const byte _FederationRanking
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::SetFederationRanking(const byte _FederationRanking)
{
	m_FederationRanking = _FederationRanking;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::SetRanking
	Access:    	public 
	Parameter: 	const byte _Ranking
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::SetRanking(const byte _Ranking)
{
	m_Ranking = _Ranking;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ID = _Resource.ReadByte();
#ifdef COUNTRYNAME
	m_strName.Read(File);
#endif
	m_strNationality.ReadFromResource(_Resource);
#ifdef ACTIVEFEDERATIONS
	m_FederationID = _Resource.ReadByte();
	EXT_ASSERT(m_FederationID < WorldData().GetFederationList().GetSize());
	WorldData().DoCountryJoinFederation(m_ID, m_FederationID);
	//DEBUG_OUTPUT(("%d : %s : %d"), (m_ID, NationalityStr(), m_FederationID));
#endif
#ifdef USE_CURRENCIES
    m_CurrencyId = _Resource.ReadByte();
#endif

	byte Temp = _Resource.ReadByte();
	SetRanking(Temp);

    m_FinancialRatioPercentage = _Resource.ReadByte();
    Temp = _Resource.ReadByte();
	SetFederationRanking(Temp);

	byte NumDivisions = _Resource.ReadByte();
	byte PlayerDataLevel = ROTW;

	if (NumDivisions > 0)
	{
		EXT_ASSERT(m_FinancialRatioPercentage != 0);
		WorldData().GetCountryList().AddActiveCountry(_Resource, m_ID);
		PlayerDataLevel = MINIMUMUEFA;
	}

	switch(WorldData().GetDataLevel())
	{
		case 0:
			break;
		case 1:	 // Medium Europe and ROTW
			if (PlayerDataLevel == FULLUEFA)
			{
				SetRanking(2);
			}
			break;
		case 2:	//Minimum Europe and ROTW
			if (PlayerDataLevel == FULLUEFA || PlayerDataLevel == MEDIUMUEFA)
			{
				SetRanking(2);
			}
			break;
		case 3: // Europe only
			if (PlayerDataLevel == ROTW)
			{
				SetRanking(2);
			}
			break;
		case 4:	// Medium Europe
			if (PlayerDataLevel == ROTW || PlayerDataLevel == FULLUEFA)
			{
				SetRanking(2);
			}
			break;
		case 5:	//Minimum Europe
			if (PlayerDataLevel == ROTW || PlayerDataLevel == FULLUEFA || PlayerDataLevel == MEDIUMUEFA)
			{
				SetRanking(2);
			}
			break;
		default :
			EXT_ASSERT(false);
	}
	//afxDump << m_strName << " " << getRanking() << "\n";
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetActiveCountry
	Access:    	public 
	Parameter: 	void
	Returns:   	CActiveCountry*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CActiveCountry& CCountry::GetActiveCountry()
{
//	afxDump << "Nationality " << m_strNationality << "  Ranking " << getRanking() << "\n";
	EXT_ASSERT(GetRanking() < 2);
	return WorldData().GetCountryList().GetActiveCountry(m_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::AddDivision
	Access:    	public 
	Parameter: 	const ushort _DivisionNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::AddDivision(const ushort _DivisionNum)
{
	GetActiveCountry().AddDivision(_DivisionNum);
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::SetRanking
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::SetRanking()
{
	EXT_ASSERT(GetRanking() == 1);
	SetRanking(0);
	GetActiveCountry().SetRanking();
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::TotalClubs
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCountry::TotalClubs()
{
	return GetActiveCountry().DoCountTotalClubs();
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetFinancialRatioPercentage
	Access:    	public const 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCountry::GetFinancialRatioPercentage() const
{
	return m_FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetID
	Access:    	public const 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCountry::GetID() const
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::TotalDivisions
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CCountry::TotalDivisions()
{
	if (GetRanking() < 2)
	{
		return GetActiveCountry().TotalDivisions();
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_FederationRanking, sizeof(m_FederationRanking));
	_pDataFile->Write(&m_Ranking, sizeof(m_Ranking));
	_pDataFile->Write(&m_FinancialRatioPercentage, sizeof(m_FinancialRatioPercentage));
	_pDataFile->Write(&m_ID, sizeof(m_ID));

#ifdef COUNTRYNAME
	m_strName.Write(_pDataFile);
#endif
	m_strNationality.Write(_pDataFile);

#ifdef USE_CURRENCIES
	_pDataFile->Write(&m_CurrencyId, sizeof(m_CurrencyId));
#endif

#ifdef ACTIVEFEDERATIONS
	_pDataFile->Write(&m_FederationID, sizeof(m_FederationID));
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountry::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_FederationRanking, sizeof(m_FederationRanking) );
	_pDataFile->Read(&m_Ranking, sizeof(m_Ranking));
	_pDataFile->Read(&m_FinancialRatioPercentage, sizeof(m_FinancialRatioPercentage) );
	_pDataFile->Read(&m_ID, sizeof(m_ID));

#ifdef COUNTRYNAME
	m_strName.Read(_pDataFile);
#endif
	m_strNationality.Read(_pDataFile);

#ifdef USE_CURRENCIES
	_pDataFile->Read(&m_CurrencyId, sizeof(m_CurrencyId));
#endif

#ifdef ACTIVEFEDERATIONS
	_pDataFile->Read(&m_FederationID, sizeof(m_FederationID));
#endif
}


#ifdef ACTIVEFEDERATIONS
/*------------------------------------------------------------------------------
	Method:   	CCountry::GetFederation
	Access:    	public 
	Parameter: 	void
	Returns:   	CFederation*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CCountry::GetFederation()
{
	return WorldData().GetFederation(m_FederationID);
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetFederationID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCountry::GetFederationID() const
{
	return m_FederationID;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CCountry::GetCurrency
	Access:    	public 
	Returns:   	CCurrency&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCurrency& CCountry::GetCurrency()
{
#ifdef USE_CURRENCIES
	return WorldData().GetCurrencyList().GetAt(m_CurrencyId);
#else
	return WorldData().GetCurrency();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::IsActive
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCountry::IsActive()
{
	if (GetRanking() > 1)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CCountry::LoanInterestWeeklyMultiplier
	Access:    	public 
	Parameter: 	void
	Returns:   	double
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
double CCountry::GetLoanInterestWeeklyMultiplier()
{
	return .3512338 / 100;
}


#ifdef COUNTRYNAME
/*------------------------------------------------------------------------------
	Method:   	CCountry::GetName
	Access:    	public 
	Returns:   	CExtString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString& CCountry::GetName()
{
	return m_strName;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCountryList::CCountryList
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCountryList::CCountryList() 
	: m_pList(NULL)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_COUNTRY, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pList = new CCountry[CExtArray::GetSize()];

    // Create the countries
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
 	{
         m_pList[ListPos].DoInitialiseFromResource(theResource);
 	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::~CCountryList
	Access:    	public 
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountryList::~CCountryList()
{
	DeleteActiveCountryList();
	delete [] m_pList;
	m_pList = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DeleteActiveCountryList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DeleteActiveCountryList()
{
	CActiveCountry* pCountry;
	for (int LoopCount = 0; LoopCount < m_ActiveCountryList.GetSize(); LoopCount++)
	{
		pCountry = m_ActiveCountryList.GetAt(LoopCount);
		delete pCountry;
	}
	m_ActiveCountryList.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::PlayMissedMatches
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Play all the matches prior to the start of the game from the start 
				of the season upto end of the calendar()
				Only to be called for countries where the season starts after the 
				end of season within calendar()
------------------------------------------------------------------------------*/
void CCountryList::PlayMissedMatches()
{
	for (uint ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->DoPlayMissedMatches();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoEndOfSeason
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCountryList::DoEndOfSeason()
{
	bool EndOfSeasonDone = false;
	CCountry* pCountry;
	for (int ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		pCountry = GetFromID(m_ActiveCountryList.GetAt(ListPos)->GetID());
		EXT_ASSERT_POINTER(pCountry, CCountry);
		if (m_ActiveCountryList.GetAt(ListPos)->DoEndOfSeason(*pCountry) == true)
		{
			EndOfSeasonDone = true;
		}
	}
	return EndOfSeasonDone;
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoDailyClearDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DoDailyClearDown()
{
	for (uint ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->DoDailyClearDown();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DoNewSeason()
{
	for (int ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->DoNewSeason();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoEndOfWeek
	Access:    	public 
	Parameter: 	const byte _MatchNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DoEndOfWeek(const bool _bNoWarnings /*= true*/)
{
	for (int ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->DoEndOfWeek(_bNoWarnings);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::PostInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::PostInitialise()
{
    CCountry* pCountry = static_cast<CCountry*>(m_pList);
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (pCountry->IsActive() == true)
		{
			pCountry->GetActiveCountry().DoCreateTransferList();
			pCountry->GetActiveCountry().DoCreateClubsStartingOrder();
		}
		pCountry++;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCountryList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::Write(CDataFile* _pDataFile)
{
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		m_pList[LoopCount].Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::Read(CDataFile* _pDataFile)
{
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		m_pList[LoopCount].Read(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::BuildCupList
	Access:    	public 
	Parameter: 	CCompetitionList* _pCupList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::BuildCupList(CCompetitionList* _pCupList)
{
	for (uint ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->BuildCupList(_pCupList);
	}
}

/*------------------------------------------------------------------------------
	Method:   	CCountryList::BuildThisWeekCupList
	Access:    	public 
	Parameter: 	CMatchTime* _pCurrentDate
	Parameter: 	Vector<CCup*>* _pCupList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::BuildThisWeekCupList(CCalendar* _pCurrentDate, Vector<CCup*>* _pCupList)
{
	for (uint ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->BuildThisWeekCupList(_pCurrentDate, _pCupList);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoSelectRandomActive
	Access:    	public 
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:	Select a random active country
------------------------------------------------------------------------------*/
CCountry& CCountryList::DoSelectRandomActive()
{
    CCountry* pCountry;
	ushort CountryId;
	if (m_ActiveCountryList.GetSize() > 1)
	{
		do
		{
			CountryId = static_cast<ushort>(RandomNumber.IntLessThan(m_ActiveCountryList.GetSize() - 1));
			pCountry = GetFromID(m_ActiveCountryList.GetAt(CountryId)->GetID());
		}
		while(pCountry->GetRanking() > 0);
	}
	else
	{
		pCountry = GetFromID(m_ActiveCountryList.GetAt(0)->GetID());
	}
	return *pCountry;
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::DoMidWeekTasks
	Access:    	public 
	Parameter: 	const byte _MatchNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::DoMidWeekTasks(const byte _MatchNumber)
{
	for (uint ListPos = 0; ListPos < m_ActiveCountryList.GetSize(); ListPos++)
	{
		m_ActiveCountryList.GetAt(ListPos)->DoMidWeekTasks(_MatchNumber);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CCountryList::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < CExtArray::GetSize());
	return static_cast<CCountry&>(m_pList[_ElementNumber]);
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::GetFromID
	Access:    	public 
	Parameter: 	ushort _ID
	Returns:   	CCountry*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry* CCountryList::GetFromID(ushort _ID)
{
    CCountry* pCountry = static_cast<CCountry*>(m_pList);
	bool bFound = false;
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
		if (pCountry->GetID() == _ID)
		{
			bFound = true;
			break;
		}
		pCountry++;
	}
	EXT_ASSERT(bFound == true);
	return pCountry;
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::AddActiveCountry
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Parameter: 	const byte _ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCountryList::AddActiveCountry( CBinaryReader& _Resource, const byte _ID )
{
	m_ActiveCountryList.Add(new CActiveCountry(_Resource, _ID));
}


/*------------------------------------------------------------------------------
	Method:   	CCountryList::GetActiveCountry
	Access:    	public 
	Parameter: 	const byte _ID
	Returns:   	CActiveCountry*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CActiveCountry& CCountryList::GetActiveCountry(const byte _ID)
{
	for (uint nListPos = 0; nListPos < m_ActiveCountryList.GetSize(); nListPos++)
	{
		if (m_ActiveCountryList.GetAt(nListPos)->GetID() == _ID)
		{
			return *m_ActiveCountryList.GetAt(nListPos);
		}
	}
	// Never gets here
	EXT_ASSERT(false);
	return *m_ActiveCountryList.GetAt(0);
}
