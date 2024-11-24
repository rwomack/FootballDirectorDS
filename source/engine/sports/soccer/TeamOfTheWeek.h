

/*

	File:- TeamOfTheWeek.h: interface for the CTeamOfTheWeek class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#if !defined(AFX_TEAMOFTHEWEEK_H__25F58B61_1641_11D2_B220_0080C883C4EB__INCLUDED_)
#define AFX_TEAMOFTHEWEEK_H__25F58B61_1641_11D2_B220_0080C883C4EB__INCLUDED_


class CPlayerOfTheWeek
{
   // Variables
 private:
	 CPlayerMatchDetails	m_Details;

 public:
	CPlayerOfTheWeek();
	~CPlayerOfTheWeek();


    // Other methods
	CClub&					GetClub();
	ClubID					GetClubID() const;
	CPlayer&				GetPlayer();
	ushort					GetPlayerID() const;
	CPlayerMatchDetails*	GetPlayerMatchDetails();
	bool					IsInUse();
	void					PreMatchClearDown();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTeamOfTheWeek 
{
public:
	CTeamOfTheWeek();
	~CTeamOfTheWeek();

     // IO Methods
#ifdef TEAMOFWEEKONMENU
    void                    Write(FILE* File);
    void                    Read(FILE* File);
#endif
	
	void					DoAfterMatchCheck(CPlayerMatchDetails& _PlayerMatchDetails);
	CPlayerOfTheWeek*		GetAt(const ushort _Index);
	bool					IsAvailable();
	void					PreMatchClearDown();

protected:
	CPlayerOfTheWeek		m_Players[PLAYERSINTEAM];
};

#endif // !defined(AFX_TEAMOFTHEWEEK_H__25F58B61_1641_11D2_B220_0080C883C4EB__INCLUDED_)
