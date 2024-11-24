
 /*

	Manager History classes

  
	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#ifndef __MANAGERHISTORY_H__
#define __MANAGERHISTORY_H__

class CManagerHistory
{
private:
	class CManagerHistoryData
	{
	public:
		uint					m_Won:10;
		uint					m_Drawn:10;
		uint					m_Lost:10;
		uint					m_Promotion:4;
		uint					m_Relegation:4;
	};
	CManagerHistoryData		m_Data;
#ifdef DETAILEDMANAGERHISTORY
	CClubMatchScore<ushort>	m_Goals;
	CTrophyHistoryList		m_TrophyHistory;
#endif

public:
	CManagerHistory();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

   // Methods
	uint					DoCalculateRating();
	void					DoResetVariables();
	ushort					GetDrawn() const;
	uint					GetGamesPlayed() const;
	ushort					GetLost() const;
	ushort					GetPromotion() const;
	ushort					GetRelegation() const;
	ushort					GetWon() const;
	void					IncPromotion();
	void					IncRelegation();
	void					PlayedMatch(const byte _GoalsFor, const byte _GoalsAgainst);
	void					SetDrawn(const uint _Value);
	void					SetLost(const uint _Value);
	void					SetPromotion(const uint _Value);
	void					SetRelegation(const uint _Value);
	void					SetWon(const uint _Value);
#ifdef DETAILEDMANAGERHISTORY
	CTrophyHistoryList& GetTrophyHistory()
	{
		return m_TrophyHistory;
	}
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////


class CManagerClubHistory
{
private:
	CDate					m_ClubDateJoined;
	CDate					m_ClubDateLeft;
	uint					m_TransferIncome;
	uint					m_TransferExpenditure;
	ushort					m_ClubID; 

public:
    // Methods
	void					DoAddTransferIncome(const uint _Amount);
	void					DoAddTransferExpenditure(const uint _Amount);
	void					DoJoinClub(const ushort _ClubID);
	void					DoResetVariables();
	CClub&					GetClub();
	ushort					GetClubID() const							{ return m_ClubID ; }
	CDate*					GetClubDateJoined()							{ return &m_ClubDateJoined; }
	CDate*					GetClubDateLeft()							{ return &m_ClubDateLeft; }
	uint					GetTransferIncome() const					{ return m_TransferIncome; }
	uint					GetTransferExpenditure() const				{ return m_TransferExpenditure; }
	void					SetClubID(ushort _ClubID)						{ m_ClubID = _ClubID; }
    void					SetClubDateLeft(CDate date)						{ m_ClubDateLeft = date; }
	void					SetTransferIncome(const uint x)					{ m_TransferIncome = x; }
	void					SetTransferExpenditure(const uint x)			{ m_TransferExpenditure = x; }
};

#endif