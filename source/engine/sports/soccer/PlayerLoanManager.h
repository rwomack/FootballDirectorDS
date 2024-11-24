//////////////////////////////////////////////////////////////////////////
// Player loan manager. Stores loan-related data rather than storing it
// in the CPlayer object.
//////////////////////////////////////////////////////////////////////////

#ifndef PLAYERLOANMANAGER_H_INCLUDED
#define PLAYERLOANMANAGER_H_INCLUDED

class CPlayerLoanManager
{
public:
	CPlayerLoanManager();
	~CPlayerLoanManager();

	void Reset();

  void          DoWeeklyUpdate();

	void					DoGoingOnLoan(const ushort usPlayerID, const ClubID _ClubOnLoanFromID, const byte _WeeksOnLoan);
	void					DoInitialise(const ushort usPlayerID);
  //void          DoWeeklyUpdate(const ushort usPlayerID);
	ClubID					GetClubOnLoanFrom(const ushort usPlayerID) const;
	byte					GetWeeksOnLoan(const ushort usPlayerID) const;
	bool					IsOnLoan(const ushort usPlayerID) const;
	bool					IsLoanedOut(const ushort usPlayerID, const ClubID _ClubID) const;
	bool					IsLoanedUntilEndOfSeason(const ushort usPlayerID) const;
	void					SetWeeksOnLoan(const ushort usPlayerID, const byte _WeeksOnLoan);                  
	void					SetClubOnLoanFrom(const ushort usPlayerID, const ClubID _ClubOnLoanFromID);				

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

private:
	struct StPlayerLoanInfo
	{
		StPlayerLoanInfo() : m_usPlayerID(NOPLAYER), m_ClubOnLoanFromID(NOCLUB), m_WeeksOnLoan(0) {};
		explicit StPlayerLoanInfo(const uint uiZero) : m_usPlayerID(NOPLAYER), m_ClubOnLoanFromID(NOCLUB), m_WeeksOnLoan(0)
		{
			EXT_ASSERT(uiZero == 0);
		}

		bool operator==(const StPlayerLoanInfo &rhs) const
		{
			return (m_usPlayerID == rhs.m_usPlayerID) &&
				(m_ClubOnLoanFromID == rhs.m_ClubOnLoanFromID) && 
				(m_WeeksOnLoan == rhs.m_WeeksOnLoan);
		}

		bool operator!=(const StPlayerLoanInfo &rhs) const
		{
			return !(*this == rhs);
		}

		ushort		m_usPlayerID;
		ClubID		m_ClubOnLoanFromID;
		byte		m_WeeksOnLoan;
	};

	StPlayerLoanInfo *FindRecord(const ushort usPlayerID);
	const StPlayerLoanInfo *FindRecord(const ushort usPlayerID) const;

	Vector<StPlayerLoanInfo> m_records;
};

#endif // #fndef PLAYERLOANMANAGER_H_INCLUDED
