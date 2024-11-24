<<<<<<< .mine
#ifndef __PLAYERTRANSFERSTATUS_H__
#define __PLAYERTRANSFERSTATUS_H__


/*

    Player Transfer Status class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#include <ClubTransferOffer.h>


class CPlayerTransferStatus
{
public:
    CPlayerTransferStatus()
		: m_Flags(0)
	{ 
		DoResetVariables(); 
	}

	enum eTransferStatusMasks 
	{
		ONTRANSFERLIST = 0x01,
		ONLOANLIST = 0x02,
		REJECTOFFERSBELOW = 0x04,
		REJECTALLOFFERS = 0x08,
		OUTOFCONTRACT = 0x10
	};

	// Methods
	void					DoResetVariables();
	void					DoTransferList(const uint _AskingPrice);
    void					IncWeeksAvailable();                        
	CString					GetAskingPriceStr(CCountry& rCountry);
	uint					GetValuation(CCountry& _Country, uint _Ret);
    bool                    IsTransferListed();
	bool					IsLoanListed();
	void					DoJoinedNewClub();
	void					DoTransferRequestCanceled();
	void					DoTakeOffTransferList();
	bool					WeeklyTasks();
	uint					GetAskingPrice() const;
    void					SetAskingPrice(const uint _Value);
	uint					GetRejectBelowPrice() const;
    void					SetRejectBelowPrice(const uint _Value);
	bool					IsOutOfContract();
	bool					IsRejectOffersBelow();
	bool					IsRejectAllOffers();
	void					SetTransferListed(const bool _Value);
	void					SetLoanListed(const bool x);
	void 					SetOutOfContract(bool _Value);
	void					SetRejectOffersBelow(bool x);
	void					SetRejectAllOffers(bool x);
	bool 					IsBidAllowed(CClubTransferOffer& _TheOffer);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

private: 
	uint					m_stablePrice;
	uint					m_AskingPrice;
	uint					m_RejectBelowPrice;
	ushort					m_lastPriceUpdate;
	byte					m_Flags;
    byte					m_WeeksAvailable;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif=======
>>>>>>> .r607
