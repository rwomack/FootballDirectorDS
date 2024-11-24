
/*
	File:- Federation.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CFederation::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ID = _Resource.ReadByte();
#ifdef ACTIVEFEDERATIONS
	m_Name.ReadFromResource(_Resource);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CFederation::GetID() const
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetCountryID
	Access:    	public 
	Parameter: 	ushort _Country
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CFederation::GetCountryID(ushort _Country)
{
	return m_Countries[_Country];
}


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
	Method:   	CFederation::DoAddCup
	Access:    	public 
	Parameter: 	byte _CupID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::DoAddCup(byte _CupID)
{
	m_CupList.Add(_CupID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::TotalCups
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CFederation::TotalCups()
{
	return m_CupList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetFederationCup
	Access:    	public 
	Parameter: 	ushort _CupId
	Returns:   	CFederationCup&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationCup& CFederation::GetFederationCup(ushort _CupID)
{
	return WorldData().GetFederationCup(m_CupList.GetAt(_CupID));
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::DoCountCountryEntrants
	Access:    	public 
	Parameter: 	CCountry * _Country
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CFederation::DoCountCountryEntrants(CActiveCountry& _Country)
{
	byte bRet = 0;
	Vector<ClubID> cCountryClubs;
	_Country.GetAllClubs(cCountryClubs);
    for (uint ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
	{
        bRet += WorldData().GetFederationCup(m_CupList.GetAt(ListPos)).DoCountCountryEntrants(cCountryClubs);
	}
	return 	bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::DoListCountryEntrants
	Access:    	public 
	Parameter: 	CCountry * _Country
	Parameter: 	CNumericArray<uint> * _TheList
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::DoListCountryEntrants(CActiveCountry& _Country, CNumericArray<ClubID>* _TheList)
{
	EXT_ASSERT(_TheList->GetSize() == 0);
	Vector<ClubID> cCountryClubs;
	_Country.GetAllClubs(cCountryClubs);
    for (uint ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
 	{
         WorldData().GetFederationCup(m_CupList.GetAt(ListPos)).DoListCountryEntrants(cCountryClubs, _TheList);
 	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::HasEnteredFederationCup
	Access:    	public 
	Parameter: 	ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFederation::HasEnteredFederationCup(ClubID _ClubID)
{
	bool bRet = false;
    for (uint ListPos = 0; ListPos < m_CupList.GetSize(); ListPos ++)
	{
		if (WorldData().GetFederationCup(m_CupList.GetAt(ListPos)).DoCheckIfQualified(_ClubID) == true)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CFederation::DoAddCountry
	Access:    	public 
	Parameter: 	ushort _CountryID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::DoAddCountry(ushort _CountryID)
{
	m_Countries.Add(_CountryID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetCountry
	Access:    	public 
	Parameter: 	const ushort _ListPos
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CFederation::GetCountry(const ushort _ListPos)
{
	return WorldData().GetCountry(m_Countries[_ListPos]);
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetRankedCountry
	Access:    	public 
	Parameter: 	const int _Ranking
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCountry& CFederation::GetRankedCountry(const int _Ranking)
{
	ushort ListPos = GetRankedCountryID(_Ranking);
	return WorldData().GetCountry(ListPos);
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::GetRankedCountryID
	Access:    	public 
	Parameter: 	const int Ranking
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CFederation::GetRankedCountryID(const int Ranking)
{
	ushort iRet = wNOTFOUND;
    for (uint ListPos = 0; ListPos < m_Countries.GetSize(); ListPos ++)
	{
		if (GetCountry(ListPos).GetFederationRanking() == Ranking)
		{
			iRet = m_Countries[ListPos];
		}
	}
	EXT_ASSERT(iRet != wNOTFOUND);
	return iRet;
}


 /*------------------------------------------------------------------------------
	Method:   	CFederation::GetRandomCountry
	Access:    	public 
	Parameter: 	CCountry * _Country1 - Null pointers may be passed for either or both
	Parameter: 	CCountry * _Country2
	Returns:   	CCountry&
	Qualifier: 	
	Purpose:	Select a random active country excluding the specified ones
------------------------------------------------------------------------------*/
CCountry& CFederation::GetRandomCountry(CCountry* _Country1, CCountry* _Country2)
{
    CCountry* pRandomCountry;
    do
	{
        pRandomCountry = &GetCountry(m_Countries.RandomPosition());
	}
	while(pRandomCountry == _Country1 || pRandomCountry == _Country2 || pRandomCountry->IsActive() == false);
	return *pRandomCountry;
}


/*------------------------------------------------------------------------------
	Method:   	CFederation::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Write(&m_ID, sizeof(m_ID));
#ifdef ACTIVEFEDERATIONS
    m_Name.Write(_pDataFile);
#endif
    m_Countries.Write(_pDataFile);
#ifdef ACTIVEFEDERATIONCUPS
    m_CupList.Write(_pDataFile);
#endif
 }


/*------------------------------------------------------------------------------
	Method:   	CFederation::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederation::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Read(&m_ID, sizeof(m_ID));
#ifdef ACTIVEFEDERATIONS
    m_Name.Read(_pDataFile);
#endif
    m_Countries.Read(_pDataFile);
#ifdef ACTIVEFEDERATIONCUPS
    m_CupList.Read(_pDataFile);
#endif
 }


/*------------------------------------------------------------------------------
	Method:   	CFederation::DoFindRandomClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CFederation::DoFindRandomClub()
{
	CCountry* pRandomCountry;
	int CountryID;
	do
	{
		CountryID = m_Countries.RandomPosition();
		pRandomCountry = &GetCountry(CountryID);
	}
	while(pRandomCountry->TotalDivisions() == 0 || pRandomCountry->GetRanking() == 2);

	return pRandomCountry->GetActiveCountry().DoFindRandomClub();
}


#ifdef ACTIVEFEDERATIONS
/*------------------------------------------------------------------------------
	Method:   	CFederation::GetName
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFederation::GetName()
{
	return m_Name;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CFederationList::CFederationList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationList::CFederationList()
{
	CBinaryReader theResource(0, IDR_FEDERATION, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pFederationList = FD_ENGINE_NEW CFederation[CExtArray::GetSize()];
	for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_pFederationList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationList::~CFederationList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederationList::~CFederationList()
{
	delete[] m_pFederationList;
}


/*------------------------------------------------------------------------------
	Method:   	CFederationList::DoCountryJoinFederation
	Access:    	public 
	Parameter: 	ushort _ID
	Parameter: 	ushort _FederationID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationList::DoCountryJoinFederation(ushort _ID,ushort _FederationID)
{
    m_pFederationList[_FederationID].DoAddCountry(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CFederationList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        m_pFederationList[ListPos].Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFederationList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFederationList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (uint ListPos = 0; ListPos < GetSize(); ListPos++)
	{
        m_pFederationList[ListPos].Read(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
Method:   	CFederationList::GetAt
Access:    	public 
Parameter: 	const ushort _ElementNumber
Returns:   	CFederation&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CFederation& CFederationList::GetAt(const ushort _ElementNumber)
{
	return static_cast<CFederation&> (m_pFederationList[_ElementNumber]);
}


#ifdef ACTIVEFEDERATIONS
/*------------------------------------------------------------------------------
	Method:   	CFederationList::Find
	Access:    	public 
	Parameter: 	CString _FedName
	Returns:   	CFederation&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFederation& CFederationList::Find(const CString& _FedName)
{
	EXT_ASSERT(_FedName.IsEmpty() != TRUE);
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        if (m_pFederationList[ListPos].GetName() == _FedName)
		{
            return m_pFederationList[ListPos];
        }
    }
	EXT_ASSERT(false);
	return m_pFederationList[0];
}
#endif
