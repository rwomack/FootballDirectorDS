
/*
	File:- PlayerSearch.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "PlayerSearch.h"

#define MAX_SEARCH_SIZE 1000

/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::CPlayerSearch
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSearch::CPlayerSearch()
{
    SetDefaults();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::DoInitSearch
	Access:    	public 
	Parameter: 	byte _MinAge
	Parameter: 	byte _MaxAge
	Parameter: 	byte _MinSkill
	Parameter: 	byte _MaxSkill
	Parameter: 	uint _MinPrice
	Parameter: 	uint _MaxPrice
	Parameter: 	const byte _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::DoInitSearch(byte _MinAge, byte _MaxAge, byte _MinSkill, byte _MaxSkill, uint _MinPrice, uint _MaxPrice, const byte _Flags)
{
	EXT_ASSERT(_MinSkill >= MINPLAYSKILL && _MaxSkill <= MAXPLAYSKILL);
	m_ExchangeRate = 1;
	m_MinimumAge = _MinAge;
	m_MaximumAge = _MaxAge;
	m_MinimumSkill = _MinSkill;
	m_MaximumSkill = _MaxSkill;
	m_MinimumPrice = _MinPrice;
	m_MaximumPrice = _MaxPrice;
	m_Flags = _Flags;
	m_bListCreated = false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetDefaults
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetDefaults()
{
	m_FindPosn = MAXPLAYERPOSITIONS;
  m_MinimumAge = MINIMUPLAYERAGE;
  m_MaximumAge = MAXIMUMPLAYERAGE;
  m_MinimumSkill = MINPLAYSKILL;
  m_MaximumSkill = MAXPLAYSKILL + 1;
  m_MinimumPrice = 0;
  m_ExchangeRate = 1;
  m_MaximumPrice = MONEYSTEPS[NUMMONEYSTEPS - 1] * GetExchangeRate();
	m_Flags = INCLUDEONTRANSFERLIST;
  m_maxPlayers = MAX_SEARCH_SIZE;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetPlayerList
	Access:    	public 
	Returns:   	CNumericArray<ushort>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<ushort>&  CPlayerSearch::GetPlayerList()
{
	return m_PlayerList;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetSize
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSearch::GetSize()
{
	return m_PlayerList.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMinAge
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMinAge(byte _Value)
{
	EXT_ASSERT(_Value > 0);
	m_MinimumAge = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMaxAge
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMaxAge(byte _Value)
{
	EXT_ASSERT(_Value > 0);
	m_MaximumAge = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMinSkill
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMinSkill(byte _Value)
{
	EXT_ASSERT(_Value >= MINPLAYSKILL);
	m_MinimumSkill = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMaxSkill
	Access:    	public 
	Parameter: 	byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMaxSkill(byte _Value)
{
	EXT_ASSERT(_Value >= MINPLAYSKILL);
	m_MaximumSkill = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetPlayerClubNumberList
	Access:    	public 
	Returns:   	CNumericArray<ushort>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<ClubID>& CPlayerSearch::GetPlayerClubNumberList()
{
	return m_PlayerClubNumberList;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetPlayerValueList
	Access:    	public 
	Returns:   	CNumericArray<uint>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<uint>& CPlayerSearch::GetPlayerValueList()
{
	return m_PlayerValueList;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetFindPosn
	Access:    	public 
	Returns:   	ePlayerPosition
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ePlayerPosition CPlayerSearch::GetFindPosn()
{
	return m_FindPosn;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetListCreated
	Access:    	public 
	Parameter: 	const bool _ListCreated
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetListCreated(const bool _ListCreated)
{
	m_bListCreated = _ListCreated;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMinAge
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSearch::GetMinAge()
{
	return m_MinimumAge;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMaxAge
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSearch::GetMaxAge()
{
	return m_MaximumAge;
}



/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMinSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSearch::GetMinSkill()
{
	return m_MinimumSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMaxSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSearch::GetMaxSkill()
{
	return m_MaximumSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMinPrice
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSearch::GetMinPrice()
{
	return m_MinimumPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMaxPrice
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSearch::GetMaxPrice()
{
	return m_MaximumPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetFindPosition
	Access:    	public 
	Parameter: 	ePlayerPosition _Position
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetFindPosition(ePlayerPosition _Position)
{
	m_FindPosn = _Position;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMinPrice
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMinPrice(const uint _Value)
{
	m_MinimumPrice = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMaxPrice
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMaxPrice(const uint _Value)
{
	m_MaximumPrice = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetIncludeFlag
	Access:    	public 
	Parameter: 	const eSearchStatusMask _FlagToSet
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetIncludeFlag(const eSearchStatusMask _FlagToSet)
{
	m_Flags |= _FlagToSet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetIncludeFlags
	Access:    	public 
	Parameter: 	const byte _Flags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetIncludeFlags(const byte _Flags)
{
	m_Flags = _Flags;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::DoEmpty
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::DoEmpty()
{
	if (m_bListCreated == true)
	{
		m_PlayerList.RemoveAll();
		m_PlayerValueList.RemoveAll();
		m_PlayerClubNumberList.RemoveAll();
	}
	m_bListCreated = false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::IsEmpty
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSearch::IsEmpty()
{
	return !m_bListCreated;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::DoRemovePlayer
	Access:    	public 
	Parameter: 	const ushort _PlayerListIndex
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::DoRemovePlayer(const ushort _PlayerListIndex)
{
	EXT_ASSERT(_PlayerListIndex < GetPlayerClubNumberList().GetSize());
	GetPlayerList().RemoveAt(_PlayerListIndex);
	GetPlayerValueList().RemoveAt(_PlayerListIndex);
	GetPlayerClubNumberList().RemoveAt(_PlayerListIndex);
}


void CPlayerSearch::SetMaximumPlayers( int max )
{
  m_maxPlayers = max;
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::DoAddToList
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	const ushort _ClubID
	Parameter: 	const uint _PlayerValue
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::DoAddToList(const ushort _PlayerID, const ClubID _ClubID, const uint _PlayerValue)
{
  if( GetPlayerList().GetSize() >= m_maxPlayers )
  {
    return;
  }

	m_bListCreated = true;
	GetPlayerList().Add(_PlayerID);
	GetPlayerValueList().Add(_PlayerValue);
	GetPlayerClubNumberList().Add(_ClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetIncludeFlag
	Access:    	public 
	Parameter: 	const eSearchStatusMask _FlagToTest
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSearch::GetIncludeFlag(const eSearchStatusMask _FlagToTest)
{
	if (m_Flags & _FlagToTest)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetExchangeRate
	Access:    	public 
	Returns:   	float
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
float CPlayerSearch::GetExchangeRate()
{
	EXT_ASSERT(m_ExchangeRate > 0);
	return m_ExchangeRate;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMaxPriceStep
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMaxPriceStep(const int _Value)
{
	EXT_ASSERT(_Value >= 0 && _Value < NUMMONEYSTEPS);
	m_MaximumPrice = MONEYSTEPS[_Value] * GetExchangeRate();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetMinPriceStep
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetMinPriceStep(const int _Value)
{
	EXT_ASSERT(_Value >= 0 && _Value < NUMMONEYSTEPS);
	m_MinimumPrice = MONEYSTEPS[_Value] * GetExchangeRate();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::Read(CDataFile* _pDataFile)
{
  	_pDataFile->Read(&m_FindPosn, sizeof(m_FindPosn));
    _pDataFile->Read(&m_MinimumAge, sizeof(m_MinimumAge));
    _pDataFile->Read(&m_MaximumAge, sizeof(m_MaximumAge));
    _pDataFile->Read(&m_MinimumSkill, sizeof(m_MinimumSkill));
    _pDataFile->Read(&m_MaximumSkill, sizeof(m_MaximumSkill));
    _pDataFile->Read(&m_ExchangeRate, sizeof(m_ExchangeRate));
    _pDataFile->Read(&m_MinimumPrice, sizeof(m_MinimumPrice));
    _pDataFile->Read(&m_MaximumPrice, sizeof(m_MaximumPrice));
    _pDataFile->Read(&m_Flags, sizeof(m_Flags));
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::Write(CDataFile* _pDataFile)
{
  	_pDataFile->Write(&m_FindPosn, sizeof(m_FindPosn));
    _pDataFile->Write(&m_MinimumAge, sizeof(m_MinimumAge));
    _pDataFile->Write(&m_MaximumAge, sizeof(m_MaximumAge));
    _pDataFile->Write(&m_MinimumSkill, sizeof(m_MinimumSkill));
    _pDataFile->Write(&m_MaximumSkill, sizeof(m_MaximumSkill));
    _pDataFile->Write(&m_ExchangeRate, sizeof(m_ExchangeRate));
    _pDataFile->Write(&m_MinimumPrice, sizeof(m_MinimumPrice));
    _pDataFile->Write(&m_MaximumPrice, sizeof(m_MaximumPrice));
    _pDataFile->Write(&m_Flags, sizeof(m_Flags));
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::SetExchangeRate
	Access:    	public 
	Parameter: 	float _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSearch::SetExchangeRate(float _Value)
{
	EXT_ASSERT(_Value > 0);
	m_MinimumPrice /= m_ExchangeRate;
	m_MaximumPrice /= m_ExchangeRate;
	m_ExchangeRate = _Value;
	m_MinimumPrice *= m_ExchangeRate;
	m_MaximumPrice *= m_ExchangeRate;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMinPriceStep
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSearch::GetMinPriceStep()
{
	int iRet = 0;
	for (uint LoopCount = 1; LoopCount < NUMMONEYSTEPS; LoopCount++)
	{
		if (m_MinimumPrice == MONEYSTEPS[LoopCount])
		{
			iRet = LoopCount;
			break;
		}
		if (m_MinimumPrice < MONEYSTEPS[LoopCount])
		{
			iRet = LoopCount - 1;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::GetMaxPriceStep
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSearch::GetMaxPriceStep()
{
	int iRet = 0;
	for (uint LoopCount = 1; LoopCount < NUMMONEYSTEPS; LoopCount++)
	{
		if (m_MaximumPrice == MONEYSTEPS[LoopCount])
		{
			iRet = LoopCount;
			break;
		}
		if (m_MaximumPrice < MONEYSTEPS[LoopCount])
		{
			iRet = LoopCount - 1;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::MinPriceStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSearch::MinPriceStr()
{
	CString Str;
	Str.Format(TEXT("%.2f"), m_MinimumPrice);
	return Str;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSearch::MaxPriceStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSearch::MaxPriceStr()
{
	CString Str;
	Str.Format(TEXT("%.2f"), m_MaximumPrice);
	return Str;
}
