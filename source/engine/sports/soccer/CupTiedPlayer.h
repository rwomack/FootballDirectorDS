
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

#include <SoccerDefines.h>


class CClubAndPlayerID
{
public:
	CClubAndPlayerID();
    CClubAndPlayerID(const ushort iPlayerID, const ClubID cClubID);
    ~CClubAndPlayerID();

	// IO Methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile *_pDataFile);

	void					DoResetVariables();
    ushort					GetPlayerID() const;
	CPlayer&				GetPlayer();
	void					SetPlayerID(const ushort _PlayerID);
    ClubID					GetClubID() const;
	void					SetClubID(const ClubID _ClubID);
	void					SetClubID(const ushort _ClubID);

private:
    ushort					m_PlayerID;
    ClubID					m_ClubID;
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
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

    // list functionality
    void					Add(const ushort iPlayerId, const ClubID cClubID);
	void					Add(CClubAndPlayerID* pNeg);
	const ushort			DoFindListPosition(const ushort iPlayerID); 
	void					DeleteAll();
    void					Delete(CClubAndPlayerID* pCupTiedPlayer);
    CClubAndPlayerID&		operator [](const ushort x);									
	const ushort			GetSize();
	bool					HasPlayed(const ushort _PlayerID);
	bool					HasPlayedThisClub(const ushort iPlayerId, const ClubID cClubID);
	bool					HasPlayedDifferentClub(const ushort iPlayerId, const ClubID cClubID);
	void					Remove(const ushort _PlayerID, const ClubID _ClubID);
	void					Remove(const ushort _PlayerID);
    void					RemoveList();

	// Variables
protected:
	Vector<CClubAndPlayerID*> m_List;
};
#endif
