
 /*

    Class:- TransferListManager

	Stores transfer-list related data, rather
	than storing it in players themselves.

	Football Director

	(c) Sports Director Ltd 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#ifndef TRANSFERLISTMANAGER_H_INCLUDED
#define TRANSFERLISTMANAGER_H_INCLUDED

class CTransferListManager
{
public:
	CTransferListManager();
	~CTransferListManager();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	void					DoResetVariables(const ushort usPlayerID);
	void 					Reset();
	void					DoTransferList(const ushort usPlayerID, const uint _AskingPrice);
    void					IncWeeksAvailable(const ushort usPlayerID);
	CString					GetAskingPriceStr(const ushort usPlayerID, CCountry& rCountry);
	uint					GetValuation(const ushort usPlayerID, CCountry& _Country, uint _Ret) const;
    bool                    IsTransferListed(const ushort usPlayerID) const;
	bool					IsLoanListed(const ushort usPlayerID) const;
	void					DoJoinedNewClub(const ushort usPlayerID);
	void					DoTransferRequestCanceled(const ushort usPlayerID);
	void					DoTakeOffTransferList(const ushort usPlayerID);
	bool					WeeklyTasks(const ushort usPlayerID);
	uint					GetAskingPrice(const ushort usPlayerID) const;
    void					SetAskingPrice(const ushort usPlayerID, const uint _Value);
	uint					GetRejectBelowPrice(const ushort usPlayerID) const;
    void					SetRejectBelowPrice(const ushort usPlayerID, const uint _Value);
	bool					IsOutOfContract(const ushort usPlayerID);
	bool					IsRejectOffersBelow(const ushort usPlayerID);
	bool					IsRejectAllOffers(const ushort usPlayerID);
	void					SetTransferListed(const ushort usPlayerID, const bool _Value);
	void					SetLoanListed(const ushort usPlayerID, const bool x);
	void					SetOutOfContract(const ushort usPlayerID, bool _Value);
	void					SetRejectOffersBelow(const ushort usPlayerID, bool x);
	void					SetRejectAllOffers(const ushort usPlayerID, bool x);
	bool					IsBidAllowed(const ushort usPlayerID, CClubTransferOffer& _TheOffer);

private:
	enum eTransferStatusMasks
	{
		ONTRANSFERLIST = 0x01,
		ONLOANLIST = 0x02,
		REJECTOFFERSBELOW = 0x04,
		REJECTALLOFFERS = 0x08,
    OUTOFCONTRACT = 0x10,
    HASRECORD = 0x20,                   // used for bitarray
	};

	struct StTransferListRecord
	{
		StTransferListRecord() : m_Flags(0), m_usPlayerID(NOPLAYER), m_stablePrice(0), m_lastPriceUpdate(0) { };
		explicit StTransferListRecord(const uint uiZero) : m_Flags(0), m_usPlayerID(NOPLAYER), m_stablePrice(0), m_lastPriceUpdate(0)
		{
			EXT_ASSERT(uiZero == 0);
		};

		bool operator==(const StTransferListRecord &rhs) const
		{
			return (m_usPlayerID == rhs.m_usPlayerID) &&
				(m_AskingPrice == rhs.m_AskingPrice) &&
				(m_RejectBelowPrice == rhs.m_RejectBelowPrice) &&
				(m_Flags == rhs.m_Flags) &&
				(m_WeeksAvailable == rhs.m_WeeksAvailable);
		}

		bool operator!=(const StTransferListRecord &rhs) const
		{
			return !(*this == rhs);
		}

		ushort					m_usPlayerID;
		mutable uint			m_stablePrice;
		uint					m_AskingPrice;
		uint					m_RejectBelowPrice;
		mutable ushort			m_lastPriceUpdate;
		byte					m_Flags;
		byte					m_WeeksAvailable;
	};

	StTransferListRecord*	FindRecord(const ushort usPlayerID);
	const StTransferListRecord* FindRecord(const ushort usPlayerID) const;
  void DoSetPlayerFlags(const ushort usPlayerID, int flag, const bool _Value);

	Vector<StTransferListRecord> m_records;
};

#endif // #ifndef TRANSFERLISTMANAGER_H_INCLUDED
