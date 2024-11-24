
/*
	File:- CupTiedPlayer.h

	C++ Classes

	Cup tied player class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __CUPTIEDPLAYER_H__
#define __CUPTIEDPLAYER_H__



class CClubAndPlayerID
{
public:
	CClubAndPlayerID();
    CClubAndPlayerID(const ushort iPlayerID, const ushort iClubID);
    virtual ~CClubAndPlayerID();

    ushort					GetPlayerID() const;
    void					SetPlayerID(const ushort x);
    ushort					GetClubID() const;
	void					SetClubID(const ushort _ClubID);

private:
    ushort					m_PlayerID;
    ushort					m_ClubID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CClubAndPlayerIDList
{
public:

	// constructor
    CClubAndPlayerIDList()					{};
    ~CClubAndPlayerIDList()					
	{ 
		RemoveList();  // do the tidyup
	}

    // IO Methods
	void  Read(CDataFile* _pDataFile);
	void Write(CDataFile* _pDataFile);

    // list functionality
    void					Add(const ushort iPlayerId, const ushort iClubID);
	void					Add(CClubAndPlayerID* pNeg);
	const ushort			DoFindListPosition(const ushort iPlayerID); 
	void					DeleteAll();
    void					Delete(CClubAndPlayerID* pCupTiedPlayer);
    CClubAndPlayerID&		operator [](const ushort x);									
	const ushort			GetSize();
	bool					HasPlayed(const ushort _PlayerID);
	bool					HasPlayedThisClub(const ushort iPlayerId, const ushort iClubID);
	bool					HasPlayedDifferentClub(const ushort iPlayerId, const ushort iClubID);
	void					Remove(const ushort iPlayerId, const ushort iClubID);
    void					RemoveList();

	// Variables
protected:
	Vector<CClubAndPlayerID*> m_List;
};
#endif
