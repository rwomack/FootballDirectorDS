
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
	 ushort					m_PlayerID;
	 ushort					m_ClubID;
	 sbyte					m_Rating;
	 byte					m_Skill;

 public:
	CPlayerOfTheWeek();
	virtual ~CPlayerOfTheWeek();

    // Other methods
	void					AfterMatchCheck(const ushort x, const ushort iClubID);
	void					PreMatchClearDown();
	const ushort			GetPlayerID() const
	{
		return m_PlayerID;
	}
	const ushort			GetClubID() const
	{
		return m_ClubID;
	}
	const sbyte				GetRating() const
	{
		return m_Rating;
	}
	bool					IsInUse();
	CPlayer& GetPlayer();
	CClub&  GetClub();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTeamOfTheWeek : public CPointerArray<CPlayerOfTheWeek>
{
public:
	CTeamOfTheWeek();
	virtual ~CTeamOfTheWeek();

     // IO Methods
#ifdef TEAMOFWEEKONMENU
    void                    Write(FILE* File);
    void                    Read(FILE* File);
#endif
	
	bool					IsAvailable();
	void					PreMatchClearDown();
};

#endif // !defined(AFX_TEAMOFTHEWEEK_H__25F58B61_1641_11D2_B220_0080C883C4EB__INCLUDED_)
