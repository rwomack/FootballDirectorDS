

/*
    Class - TransferNegotiation.h: interface for the CTransferNegotiation class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */



#if !defined(AFX_TRANSFERNEGOTIATION_H__4A04B602_4BCC_11D2_BD3F_0000B4908891__INCLUDED_)
#define AFX_TRANSFERNEGOTIATION_H__4A04B602_4BCC_11D2_BD3F_0000B4908891__INCLUDED_

#include <SoccerDefines.h>

class CTransferNegotiation
{
public:
    CTransferNegotiation() {};
    CTransferNegotiation(CClubTransferOffer& rTheBid, int DatesToNegotiation);
    ~CTransferNegotiation();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void 					DailyTasks(CClub* _Club);
	void					SetPlayerAgreed(CContract* pContract);
	void 					DoTransferPlayer();
	ushort 					GetPlayerID();
	ClubID 					GetSellClubID() const;
	byte					GetDatesToNegotiation() const;
	CClubTransferOffer&		GetOffer();

    // Variables
private:
	CClubTransferOffer		m_TheOffer;
	CContract				m_PlayerContract;
    ClubID					m_SellClubID;
    ushort					m_PlayerID;
	byte					m_DatesToNegotiation:7;
	byte					m_bPlayerAgreedTerms:1;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTransferNegotiationList
{
public:
    CTransferNegotiationList();
    ~CTransferNegotiationList();

    // IO Methods
	void    				Write(CDataFile* _pDataFile);
	void    				Read(CDataFile* _pDataFile);

    // Methods
    void                    AddOne(CClubTransferOffer& rTheBid, int DatesToNegotiation);
	void 					CheckIfPlayerInTransferNegotiations(ushort _PlayerNum, CClub* _BiddingClub, CClub* _UserClub);
	CTransferNegotiation*	DoFindNegotiationsForPlayer(CPlayer* pPlayer);
	void 					DailyTasks(CClub* _Club);
	bool					FindPlayer(CPlayer* _pPlayer);
	bool					FindPlayer(const ushort _PlayerNumber);
    void                    RemoveList();
	void					RemoveOne(const ushort _PlayerNum);
	void					DeleteAll();
	CTransferNegotiation*	GetAt(const ushort _ElementNumber);

	// Variables
private:
	Vector<CTransferNegotiation*> m_List;
};

#endif // !defined(AFX_TRANSFERNEGOTIATION_H__4A04B602_4BCC_11D2_BD3F_0000B4908891__INCLUDED_)
