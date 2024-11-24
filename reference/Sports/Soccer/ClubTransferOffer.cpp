
 /*

	Class:- Club Transfer Offer

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#include "stdafx.h"

const int MAXSELLONPERCENTAGE = 25;



	
/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::CClubTransferOffer
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CClubTransferOffer::CClubTransferOffer()
	: m_PlayerID(NOPLAYER)
	, m_BidClubID(NOCLUB)
	, m_AmountOffered(0)
	, m_nAmountRefused(0)
	, m_SellOnPercentOffered(0)
	, m_SellOnPercentRequired(0)
	, m_bDealCompleted(false)

{
}
 

/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::CClubTransferOffer
	Access:    	public 
	Parameter: 	CClubTransferOffer* _ClubTransferOffer
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubTransferOffer::CClubTransferOffer(CClubTransferOffer* _ClubTransferOffer)
	: m_PlayerID(_ClubTransferOffer->m_PlayerID) 
	, m_BidClubID(_ClubTransferOffer->m_BidClubID)
	, m_AmountOffered(_ClubTransferOffer->m_AmountOffered)
	, m_SellOnPercentOffered(_ClubTransferOffer->m_SellOnPercentOffered)
	, m_SellOnPercentRequired(_ClubTransferOffer->m_SellOnPercentRequired)
	, m_nAmountRefused(_ClubTransferOffer->m_nAmountRefused)
	, m_bDealCompleted(_ClubTransferOffer->m_bDealCompleted)
{
	EXT_ASSERT_POINTER(_ClubTransferOffer, CClubTransferOffer);
}

 
/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::CClubTransferOffer
	Access:    	public 
	Parameter: 	CPlayer* pPlayer
	Parameter: 	const uint dAmountOffered
	Parameter: 	CClub* pBiddingClub
	Parameter: 	byte SellOnPercentOffered 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubTransferOffer::CClubTransferOffer(CPlayer* _Player, const uint _AmountOffered, CClub* _BiddingClub /*= NULL*/, byte _SellOnPercentOffered /*= 0*/)
{
	DoInitialise(_Player, _AmountOffered, _BiddingClub, _SellOnPercentOffered);
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::DoInitialise
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	const uint _AmountOffered
	Parameter: 	CClub * _BiddingClub - Can be NULL
	Parameter: 	byte _SellOnPercentOffered
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::DoInitialise(CPlayer* _Player, const uint _AmountOffered, CClub* _BiddingClub /*= NULL*/, byte _SellOnPercentOffered /*= 0*/)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	ASSERT_NULL_OR_POINTER(_BiddingClub, CClub);
	m_SellOnPercentOffered = _SellOnPercentOffered;
	m_SellOnPercentRequired = 0;
	m_PlayerID = _Player->DoFindID();
	m_AmountOffered = _AmountOffered;
	if (_BiddingClub != NULL)
	{
		m_BidClubID = _BiddingClub->DoFindID();
	}
	m_nAmountRefused = 0;
	m_bDealCompleted = false;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetLoanPeriod
	Access:    	public 
	Parameter: 	const uint _Weeks
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetLoanPeriod(const uint _Weeks)
{
	m_AmountOffered = _Weeks;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetLoanPeriod
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CClubTransferOffer::GetLoanPeriod()
{
	return m_AmountOffered;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetAmountOffered
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubTransferOffer::GetAmountOffered() const
{
	return m_AmountOffered;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetSellOnPercentOffered
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CClubTransferOffer::GetSellOnPercentOffered() const
{
	return m_SellOnPercentOffered;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetSellOnPercentRequired
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CClubTransferOffer::GetSellOnPercentRequired() const
{
	return m_SellOnPercentRequired;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetSellOnPercentRequired
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetSellOnPercentRequired(const byte x)
{
	m_SellOnPercentRequired = x;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetAmountOffered
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetAmountOffered(const uint _Value)
{
	m_AmountOffered = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetDealCompleted
	Access:    	public 
	Parameter: 	const bool x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetDealCompleted(const bool x)
{
	m_bDealCompleted = x;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IsDealCompleted
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::IsDealCompleted() const
{
	return m_bDealCompleted;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetAmountRefused
	Access:    	public const 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CClubTransferOffer::GetAmountRefused() const
{
	return m_nAmountRefused;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetBidClub
	Access:    	public 
	Parameter: 	CClub* _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetBidClub(CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	m_BidClubID = _Club->DoFindID();
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetBidClub
	Access:    	public 
	Parameter: 	void
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CClubTransferOffer::GetBidClub()
{
	EXT_ASSERT(m_BidClubID != NOCLUB);
	return WorldData().GetClub(m_BidClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetBidClubID
	Access:    	public 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubTransferOffer::GetBidClubID()
{
	EXT_ASSERT(m_BidClubID != NOCLUB);
	return m_BidClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetPlayer
	Access:    	public 
	Parameter: 	void
	Returns:   	CPlayer&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer& CClubTransferOffer::GetPlayer()
{
	EXT_ASSERT(m_PlayerID != NOPLAYER);
	return *WorldData().GetPlayer(m_PlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::GetPlayerID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubTransferOffer::GetPlayerID()
{
	EXT_ASSERT(m_PlayerID != NOPLAYER);
	return m_PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetPlayer
	Access:    	public 
	Parameter: 	CPlayer* pPlayer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetPlayer(CPlayer* _Player)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	m_PlayerID = _Player->DoFindID();
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IsSellOnPercentAcceptable
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::IsSellOnPercentAcceptable()
{
	if (GetSellOnPercentOffered() < GetSellOnPercentRequired())
	{
		SetSellOnPercentOffered(GetSellOnPercentOffered() / 5);
		SetSellOnPercentRequired(GetSellOnPercentRequired() / 5);
		ushort SellOnChanceAccept[] =
		{
			75, 40, 15, 5
		};
		if (RandomNumber.IntLessThan(100) > SellOnChanceAccept[GetSellOnPercentRequired() - GetSellOnPercentOffered()])
		{
			return false;
		}
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IsFairOffer
	Access:    	public 
	Parameter: 	CCountry& rCountry
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::IsFairOffer(CCountry& rCountry)
{
	if (GetPlayer().IsFairPrice(GetAmountOffered(), rCountry) == false || GetAmountOffered() <= GetAmountRefused())
	{
		DoRejectBidTooLow();
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IsOfferForListedPlayerAcceptable
	Access:    	public 
	Parameter: 	CCountry& rCountry
	Parameter: 	CString& strClubName
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::IsOfferForListedPlayerAcceptable(CCountry& rCountry, CString& strClubName)
{

	if (GetAmountOffered() >= GetPlayer().GetTransferStatus().GetAskingPrice())
	{
		return true;
	}
	if (GetAmountOffered() > max(GetPlayer().DoCalculateMinimumTransferFee(rCountry), (GetPlayer().GetTransferStatus().GetAskingPrice() / 100) * (75 + RandomNumber.IntLessThan(20))))
	{
		return true;
	}
	m_szReasonString.Format(IDS_BIDTOOLOW, strClubName, GetPlayer().GetName());
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IsFairSellingPrice
	Access:    	public 
	Parameter: 	CCountry& _Country
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::IsFairSellingPrice(CCountry& _Country)
{
	double FairPrice = GetPlayer().GetSellingPrice(_Country);
	if (GetAmountOffered() < (FairPrice * .8) || \
		(IsBetween(static_cast<double>(GetAmountOffered()), FairPrice * .8,  FairPrice * .84) && RandomNumber.IntLessThan(100) > 5) || \
		(IsBetween(static_cast<double>(GetAmountOffered()), FairPrice * .85, FairPrice * .89) && RandomNumber.IntLessThan(100) > 10) || \
		(IsBetween(static_cast<double>(GetAmountOffered()), FairPrice * .90, FairPrice * .94) && RandomNumber.IntLessThan(100) > 30) || \
		(IsBetween(static_cast<double>(GetAmountOffered()), FairPrice * .95, FairPrice) && RandomNumber.IntLessThan(100) > 60))
	{
		if (GetBidClub().IsUserControlled() == true)
		{
			UserMessageBox(IDS_BIDNOTENOUGH);
		}
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::ShouldConsiderBidForNonListed
	Access:    	public 
	Parameter: 	const byte ChanceSellUnlistedPlayer
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubTransferOffer::ShouldConsiderBidForNonListed(const byte ChanceSellUnlistedPlayer)
{
	if (GetPlayer().GetTransferStatus().IsTransferListed() == false)
	{
		// Always consider very large bids
		if (GetAmountOffered() < GetPlayer().GetTransferStatus().GetAskingPrice() * 1.8)
		{
			// Should we consider the bid?
			if (RandomNumber.IntLessThan(100) > ChanceSellUnlistedPlayer)
			{
				return false;
			}
		}
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::DecrementSellOnPercentOffered
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::DecrementSellOnPercentOffered()
{
	if (m_SellOnPercentOffered > 0)
	{
		m_SellOnPercentOffered--;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::IncrementSellOnPercentOffered
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::IncrementSellOnPercentOffered()
{
	if (m_SellOnPercentOffered < MAXSELLONPERCENTAGE)
	{
		m_SellOnPercentOffered++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::SetSellOnPercentOffered
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::SetSellOnPercentOffered(const byte x)
{
	EXT_ASSERT(x < 100);
	m_SellOnPercentOffered = x;
}


/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::DoRejectBidTooLow
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubTransferOffer::DoRejectBidTooLow()
{
	m_szReasonString.LoadString(IDS_BIDNOTENOUGH);
	// If its higher than current highest refused
	m_nAmountRefused = max(m_AmountOffered, m_nAmountRefused);
}

/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::Write
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Saves the state of the object.
------------------------------------------------------------------------------*/
void CClubTransferOffer::Write(CDataFile* _pDataFile)
{
	// m_szString does not appear to require saving/loading.
	_pDataFile->Write(&m_PlayerID, sizeof(m_PlayerID));
	_pDataFile->Write(&m_BidClubID, sizeof(m_BidClubID));
	_pDataFile->Write(&m_AmountOffered, sizeof(m_AmountOffered));
	_pDataFile->Write(&m_nAmountRefused, sizeof(m_nAmountRefused));

	const ushort usSellOnPercentOffered = m_SellOnPercentOffered;
	const ushort usSellOnPercentRequired = m_SellOnPercentRequired;
	const ushort bDealCompleted = m_bDealCompleted;

	_pDataFile->Write(&usSellOnPercentOffered, sizeof(usSellOnPercentOffered));
	_pDataFile->Write(&usSellOnPercentRequired, sizeof(usSellOnPercentRequired));
	_pDataFile->Write(&bDealCompleted, sizeof(bDealCompleted));
}

/*------------------------------------------------------------------------------
	Method:   	CClubTransferOffer::Read
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Loads the state of the object.
------------------------------------------------------------------------------*/
void CClubTransferOffer::Read(CDataFile* _pDataFile)
{
	// m_szString does not appear to require saving/loading.
	_pDataFile->Read(&m_PlayerID, sizeof(m_PlayerID));
	_pDataFile->Read(&m_BidClubID, sizeof(m_BidClubID));
	_pDataFile->Read(&m_AmountOffered, sizeof(m_AmountOffered));
	_pDataFile->Read(&m_nAmountRefused, sizeof(m_nAmountRefused));

	ushort usSellOnPercentOffered = m_SellOnPercentOffered;
	ushort usSellOnPercentRequired = m_SellOnPercentRequired;
	ushort bDealCompleted = m_bDealCompleted;

	_pDataFile->Read(&usSellOnPercentOffered, sizeof(usSellOnPercentOffered));
	_pDataFile->Read(&usSellOnPercentRequired, sizeof(usSellOnPercentRequired));
	_pDataFile->Read(&bDealCompleted, sizeof(bDealCompleted));

	m_SellOnPercentOffered = usSellOnPercentOffered;
	m_SellOnPercentRequired = usSellOnPercentRequired;
	m_bDealCompleted = bDealCompleted;
}
