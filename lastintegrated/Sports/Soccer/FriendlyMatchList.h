

/*
    Class - C++ Classes

    FriendlyMatchList.h: interface for the CFriendlyMatchList class.


	Football Director

	(c) RW Software 1994 - 2001


  ---------------------------------------------------------------
 */


#if !defined(AFX_FRIENDLYMATCHLIST_H__A4DC17E1_0F30_11D2_A3F5_00001C303EC4__INCLUDED_)
#define AFX_FRIENDLYMATCHLIST_H__A4DC17E1_0F30_11D2_A3F5_00001C303EC4__INCLUDED_


class CFriendlyMatchList  : public CFixtureList
{
public:
	CFriendlyMatchList(void);
    ~CFriendlyMatchList(void);

// IO Methods
	void					Write(FILE* File);
	void					Read(FILE* File);

// Methods
	void					AddOne(const uint16 HomeTeam, const uint16 AwayTeam, uint8 WeekNumber);
	void					DeleteClubFixtures(const uint16 ClubNumber);
	bool					PlayMatches(const bool bShowSummary);
	CExtString				ResultStr(const uint16 ClubID, uint8 WeekNumber);

// Variables
private:
	CNumericArray<uint8>		m_WeekNumbers;
};

#endif // !defined(AFX_FRIENDLYMATCHLIST_H__A4DC17E1_0F30_11D2_A3F5_00001C303EC4__INCLUDED_)
