

/*

	Class - C++ Classes

	Club Transfer Offer

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 
*/


#ifndef __CLUBTRANSFEROFFER_H__
#define __CLUBTRANSFEROFFER_H__



class CClubTransferOffer
{
public:
    CClubTransferOffer(CPlayer* pPlayer, const uint dAmountOffered, CClub* pBiddingClub = NULL, byte SellOnPercentOffered = 0);
    CClubTransferOffer(CClubTransferOffer* a_pClubTransferOffer);
    CClubTransferOffer();

    // Methods
	void					DecrementSellOnPercentOffered();
	void					DoInitialise(CPlayer* _Player, const uint _AmountOffered, CClub* _BiddingClub = NULL, byte _SellOnPercentOffered = 0);
	void					DoRejectBidTooLow();
	uint					GetAmountOffered() const;
	uint					GetAmountRefused() const;
	CClub&					GetBidClub();
	ushort					GetBidClubID();
	uint					GetLoanPeriod();
	CPlayer&				GetPlayer();
	ushort					GetPlayerID();
	byte					GetSellOnPercentOffered() const;
	byte					GetSellOnPercentRequired() const;
	void					IncrementSellOnPercentOffered();
	bool					IsDealCompleted() const;
	bool					IsFairOffer(CCountry& rCountry);
	bool					IsFairSellingPrice(CCountry& _Country);
	bool					IsOfferForListedPlayerAcceptable(CCountry& rCountry, CString& strClubName);
	bool					IsSellOnPercentAcceptable();
	void 					SetAmountOffered(const uint _Value);
	void 					SetBidClub(CClub* _Club);
	void					SetDealCompleted(const bool x);
	void					SetLoanPeriod(const uint _Weeks);
	void 					SetPlayer(CPlayer* _Player);
	void					SetSellOnPercentOffered(const byte x);
	void					SetSellOnPercentRequired(const byte x);
	bool					ShouldConsiderBidForNonListed(const byte ChanceSellUnlistedPlayer);

    // IO Methods
	void    Write(CDataFile* _pDataFile);
	void    Read(CDataFile* _pDataFile);

   // Variables
	CString					m_szReasonString;
 private:
 	ushort					m_PlayerID;
	ushort					m_BidClubID;
	uint					m_AmountOffered;						// For a loan offer is the number of weeks
	uint					m_nAmountRefused;
	ushort					m_SellOnPercentOffered:5;
	ushort					m_SellOnPercentRequired:5;
	ushort					m_bDealCompleted:1;
};
#endif