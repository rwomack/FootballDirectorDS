
 /*

    Class:- TransferListManager

	Football Director

	(c) Sports Director Ltd 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#include <nitro/itcm_begin.h>
#endif


#include "stdafx.h"
#include <fdmemory.h>
#include <TransferListManager.h>
#include <lemon/math/lmath.h>
#include <fdpackedbitarray.h>


//TODO: Move into CPlayer, store flag for ISONLOAN / ISINJURED also
//      Remove unneeded flags from transferlist manager
#define USE_BITS

#ifdef USE_BITS

//NOTE: Already in ITCM
//#ifdef L_PLATFORM_NDS
//#include <nitro/dtcm_begin.h>
//#endif
static FDPackedBitArray<char, 6, MAXNUMPLAYERS, 0> s_playerTransferListFlags;
//#ifdef L_PLATFORM_NDS
//#include <nitro/dtcm_end.h>
//#endif

static char GetPlayerFlags( int playerID )
{
  return s_playerTransferListFlags.Get( playerID );
}

static void SetPlayerFlags( int playerID, char flags )
{
  s_playerTransferListFlags.Set( playerID, flags );
}

static void SetPlayerFlag( int playerID, char flag )
{
  char curFlag = s_playerTransferListFlags.Get( playerID );
  s_playerTransferListFlags.Set( playerID, curFlag | flag );
}

static void ClearPlayerFlag( int playerID, char flag )
{
  char curFlag = s_playerTransferListFlags.Get( playerID );
  s_playerTransferListFlags.Set( playerID, curFlag & ~flag );
}

static void ClearAllPlayerFlag()
{
  s_playerTransferListFlags.ClearAll();
}

#endif

/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::CTransferListManager
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferListManager::CTransferListManager()
{
#ifdef USE_BITS
  ClearAllPlayerFlag();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::~CTransferListManager
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferListManager::~CTransferListManager()
{
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::Reset
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::Reset()
{
	m_records.RemoveAll();
#ifdef USE_BITS
  ClearAllPlayerFlag();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::FindRecord
	Access:    	private
	Parameter: 	const ushort usPlayerID
	Returns:   	CTransferListManager::StTransferListRecord *
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferListManager::StTransferListRecord* CTransferListManager::FindRecord(const ushort usPlayerID)
{
#ifdef USE_BITS
  if( GetPlayerFlags(usPlayerID) & HASRECORD )
#endif
  {
    // Has an entry - now find it
    for (int i = 0; i < m_records.GetSize(); i++)
    {
      if (m_records[i].m_usPlayerID == usPlayerID)
      {
        return &m_records[i];
      }
    }
  }

	return NULL;
}

/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::FindRecord
	Access:    	private
	Parameter: 	const ushort usPlayerID
	Returns:   	const CTransferListManager::StTransferListRecord*
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
const CTransferListManager::StTransferListRecord* CTransferListManager::FindRecord(const ushort usPlayerID) const
{
#ifdef USE_BITS
  if( GetPlayerFlags(usPlayerID) & HASRECORD )
#endif
  {
    for (int i = 0; i < m_records.GetSize(); i++)
    {
      if (m_records[i].m_usPlayerID == usPlayerID)
      {
        return &m_records[i];
      }
    }
  }

	return NULL;
}

/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoResetVariables(const ushort usPlayerID)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		m_records.RemoveElement(*pRecord);
#ifdef USE_BITS
    SetPlayerFlags( usPlayerID, 0 );
#endif
		return;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::DoJoinedNewClub
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoJoinedNewClub(const ushort usPlayerID)
{
	DoResetVariables(usPlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsBidAllowed
	Access:    	public
	Parameter: 	CClubTransferOffer & _TheOffer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsBidAllowed(const ushort usPlayerID, CClubTransferOffer& _TheOffer)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);

	if ((pRecord == NULL) || (IsRejectAllOffers(usPlayerID) ||
		(IsRejectOffersBelow(usPlayerID) && pRecord->m_RejectBelowPrice > _TheOffer.GetAmountOffered())))
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::GetValuation
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	CCountry & _Country
	Parameter: 	uint _Ret
	Returns:   	uint
	Qualifier: 	const
	Purpose:	Get a fair but random transfer valuation of player
------------------------------------------------------------------------------*/
uint CTransferListManager::GetValuation(const ushort usPlayerID, CCountry& _Country, uint _Ret) const
{
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return _Ret;

    if (IsTransferListed(usPlayerID))
	{
		// If the player is on the transfer list then offer around that amount
		if (GetAskingPrice(usPlayerID))
		{
			// only return a random value based on asking price after a week has gone by
			CCalendar calender = WorldData().GetCurrentDate();
			if( calender.GetWeek() > pRecord->m_lastPriceUpdate  || pRecord->m_stablePrice == 0 )
			{
        pRecord->m_stablePrice = (uint)_Country.GetCurrency().RoundedPounds(GetAskingPrice(usPlayerID) * .9f + RandomNumber.IntLessThan(static_cast<uint>(GetAskingPrice(usPlayerID) * .2f)));
				pRecord->m_lastPriceUpdate = calender.GetWeek();
			}

			return pRecord->m_stablePrice;
		}
		else
		{
			return 0;
		}
	}

	return _Ret;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::GetAskingPriceStr
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	CCountry & rCountry
	Returns:   	CString - the current wage of the player in the currency of the
				country currently employed in
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CTransferListManager::GetAskingPriceStr(const ushort usPlayerID, CCountry& rCountry)
{
	EXT_ASSERT(IsTransferListed(usPlayerID) == true);						// Ensure not listed
	return rCountry.GetCurrency().GetForeignStr(GetAskingPrice(usPlayerID));
 }


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsTransferListed
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsTransferListed(const ushort usPlayerID) const
{
#ifdef USE_BITS
  if( GetPlayerFlags( usPlayerID ) & ONTRANSFERLIST)
		return true;
#else
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return false;

	if (pRecord->m_Flags & ONTRANSFERLIST)
		return true;
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsLoanListed
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsLoanListed(const ushort usPlayerID) const
{
#ifdef USE_BITS
  if( GetPlayerFlags( usPlayerID ) & ONLOANLIST)
		return true;
#else
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return false;

  if (pRecord->m_Flags & ONLOANLIST)
		return true;
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsRejectOffersBelow
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsRejectOffersBelow(const ushort usPlayerID)
{
#ifdef USE_BITS
  if( GetPlayerFlags( usPlayerID ) & REJECTOFFERSBELOW)
		return true;
#else
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return false;

  if (pRecord->m_Flags & REJECTOFFERSBELOW)
		return true;
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsOutOfContract
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsOutOfContract(const ushort usPlayerID)
{
#ifdef USE_BITS
  if( GetPlayerFlags( usPlayerID ) & OUTOFCONTRACT)
		return true;
#else
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return false;

  if (pRecord->m_Flags & OUTOFCONTRACT)
		return true;
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IsRejectAllOffers
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::IsRejectAllOffers(const ushort usPlayerID)
{
#ifdef USE_BITS
  if( GetPlayerFlags( usPlayerID ) & REJECTALLOFFERS)
		return true;
#else
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
		return false;

  if (pRecord->m_Flags & REJECTALLOFFERS)
		return true;
#endif
	return false;
}

/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetTransferListed
	Access:    	public
	Parameter: 	const bool _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoSetPlayerFlags(const ushort usPlayerID, int flag, const bool _Value)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord == NULL)
	{
		if (_Value == true)
		{
			// Add a new record.
			StTransferListRecord cNewRecord;
			cNewRecord.m_usPlayerID = usPlayerID;
      cNewRecord.m_Flags |= flag;
#ifdef USE_BITS
      // set active record flag and this flag
      SetPlayerFlag( usPlayerID, flag | HASRECORD);
#endif
			m_records.Add(cNewRecord);
		}

		return;
	}

	if (_Value == true)
	{
    pRecord->m_Flags |= flag;
#ifdef USE_BITS
    // set single flag
    SetPlayerFlag( usPlayerID, flag );
#endif
	}
	else
	{
    pRecord->m_Flags &= ~flag;

		// If there are no other flags set, remove the record.
		if (pRecord->m_Flags == 0)
    {
			m_records.RemoveElement(*pRecord);
#ifdef USE_BITS
      // clear all flags
      SetPlayerFlags( usPlayerID, 0 );
#endif
    } else
    {
#ifdef USE_BITS
      // clear single flag
      ClearPlayerFlag( usPlayerID, flag );
#endif
    }
	}
}

/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetTransferListed
	Access:    	public
	Parameter: 	const bool _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetTransferListed(const ushort usPlayerID, const bool _Value)
{
  DoSetPlayerFlags( usPlayerID, ONTRANSFERLIST, _Value );
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetLoanListed
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	const bool x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetLoanListed(const ushort usPlayerID, const bool x)
{
  DoSetPlayerFlags( usPlayerID, ONLOANLIST, x );
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetRejectOffersBelow
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	bool x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetRejectOffersBelow(const ushort usPlayerID, bool x)
{
  DoSetPlayerFlags( usPlayerID, REJECTOFFERSBELOW, x );
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetRejectAllOffers
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	bool x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetRejectAllOffers(const ushort usPlayerID, bool x)
{
  DoSetPlayerFlags( usPlayerID, REJECTALLOFFERS, x );
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetOutOfContract
	Access:    	public
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetOutOfContract(const ushort usPlayerID, bool _Value)
{
  DoSetPlayerFlags( usPlayerID, OUTOFCONTRACT, _Value );
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::DoTransferList
	Access:    	public
	Parameter: 	const uint _AskingPrice
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoTransferList(const ushort usPlayerID, const uint _AskingPrice)
{
	EXT_ASSERT(IsTransferListed(usPlayerID) == false);
	SetTransferListed(usPlayerID, true);
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	EXT_ASSERT(pRecord != NULL);

	pRecord->m_AskingPrice = _AskingPrice;
    pRecord->m_WeeksAvailable = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::IncWeeksAvailable
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::IncWeeksAvailable(const ushort usPlayerID)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
		pRecord->m_WeeksAvailable++;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::GetAskingPrice
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CTransferListManager::GetAskingPrice(const ushort usPlayerID) const
{
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
		return pRecord->m_AskingPrice;

	CPlayer *pPlayer = WorldData().GetPlayer(usPlayerID);
	CCountry &cCountry = pPlayer->GetClub().GetActiveCountry().GetCountry();
	return GetValuation(usPlayerID, cCountry, pPlayer->DoCalculateAdjustedFee(cCountry));
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetAskingPrice
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetAskingPrice(const ushort usPlayerID, const uint _Value)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		pRecord->m_AskingPrice = _Value;
	}
	else
	{
		EXT_ASSERT(false && "Couldn't find transfer list record");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::GetRejectBelowPrice
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CTransferListManager::GetRejectBelowPrice(const ushort usPlayerID) const
{
	const StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		return pRecord->m_RejectBelowPrice;
	}

	EXT_ASSERT(false && "Couldn't find transfer list record");
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::SetRejectBelowPrice
	Access:    	public
	Parameter: 	const ushort usPlayerID
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::SetRejectBelowPrice(const ushort usPlayerID, const uint _Value)
{
	StTransferListRecord *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		pRecord->m_RejectBelowPrice = _Value;
	}
	else
	{
		EXT_ASSERT(false && "Couldn't find transfer list record");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::WeeklyTasks
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferListManager::WeeklyTasks(const ushort usPlayerID)
{
	if (IsTransferListed(usPlayerID) == true)
	{
		StTransferListRecord *pRecord = FindRecord(usPlayerID);
		pRecord->m_WeeksAvailable++;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::DoTakeOffTransferList
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoTakeOffTransferList(const ushort usPlayerID)
{
	DoTransferRequestCanceled(usPlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::DoTransferRequestCanceled
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::DoTransferRequestCanceled(const ushort usPlayerID)
{
	SetTransferListed(usPlayerID, false);
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::Write(CDataFile* _pDataFile)
{
	ushort usNumRecords = m_records.GetSize();
	_pDataFile->Write(&usNumRecords, sizeof(usNumRecords));

	for (int i = 0; i < usNumRecords; i++)
	{
		StTransferListRecord &cRecord = m_records[i];
		_pDataFile->Write(&cRecord.m_usPlayerID, sizeof(cRecord.m_usPlayerID));
		_pDataFile->Write(&cRecord.m_AskingPrice, sizeof(cRecord.m_AskingPrice));
		_pDataFile->Write(&cRecord.m_RejectBelowPrice, sizeof(cRecord.m_RejectBelowPrice));
		_pDataFile->Write(&cRecord.m_Flags, sizeof(cRecord.m_Flags));
		_pDataFile->Write(&cRecord.m_WeeksAvailable, sizeof(cRecord.m_WeeksAvailable));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferListManager::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferListManager::Read(CDataFile* _pDataFile)
{
	ushort usNumRecords;
	_pDataFile->Read(&usNumRecords, sizeof(usNumRecords));

#ifdef USE_BITS
  ClearAllPlayerFlag();
#endif

	m_records.SetSize(usNumRecords, StTransferListRecord());

	for (int i = 0; i < usNumRecords; i++)
	{
		StTransferListRecord &cRecord = m_records[i];
		_pDataFile->Read(&cRecord.m_usPlayerID, sizeof(cRecord.m_usPlayerID));
		_pDataFile->Read(&cRecord.m_AskingPrice, sizeof(cRecord.m_AskingPrice));
		_pDataFile->Read(&cRecord.m_RejectBelowPrice, sizeof(cRecord.m_RejectBelowPrice));
		_pDataFile->Read(&cRecord.m_Flags, sizeof(cRecord.m_Flags));
		_pDataFile->Read(&cRecord.m_WeeksAvailable, sizeof(cRecord.m_WeeksAvailable));

#ifdef USE_BITS
    SetPlayerFlags( cRecord.m_usPlayerID, cRecord.m_Flags | HASRECORD );
#endif
	}
}
