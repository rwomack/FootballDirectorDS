

/*
    Class - ContractClause.h: interface for the CContractClause class.


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */

#if !defined(AFX_CONTRACTCLAUSE_H__7F5381E2_4D4F_11D2_BD3F_0000B4908891__INCLUDED_)
#define AFX_CONTRACTCLAUSE_H__7F5381E2_4D4F_11D2_BD3F_0000B4908891__INCLUDED_

#include <SoccerDefines.h>

class CContractClause 
{
public:
	CContractClause() {};
	CContractClause(const ushort PlayerID, byte SellOnClausePercentage, const ClubID ClubToID, const ClubID ClubFromID);
	~CContractClause();

    // Methods
	CClub&					GetClubFrom();
	CClub&					GetClubTo();
	ushort					GetPlayerID() const;
	byte					GetSellOnClausePercentage() const;

	void					DoSetVariables(const ushort PlayerID, byte _SellOnClausePercentage, const ClubID _ClubToID, const ClubID _ClubFromID);
	void					DoResetVariables();
	bool					IsEmpty();						

	// Variables
private:
    ushort					m_PlayerID;
    ClubID					m_ClubToID;
    ClubID					m_ClubFromID;
    byte					m_SellOnClausePercentage;
};



class CContractClauseList : public CPointerArray<CContractClause>
{
public:
    CContractClauseList() {};
    ~CContractClauseList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void 					DoAddOne(const ushort _PlayerID, byte _SellOnClausePercentage, const ClubID _ClubToID, const ClubID _ClubFromID);
	CContractClause* 		DoFindPlayerClubClause(const ushort _PlayerID, CClub* _Club);
	void 					RemoveAllForPlayer(const ushort _PlayerID);
	void 				RemoveOne(const ushort _PlayerID, CClub* _Club);
};
#endif // !defined(AFX_CONTRACTCLAUSE_H__7F5381E2_4D4F_11D2_BD3F_0000B4908891__INCLUDED_)
