
/*
	File:- CupTiedPlayer.h

	C++ Classes

	Out of conract player class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __OUTOFCONTRACTPLAYER_H__
#define __OUTOFCONTRACTPLAYER_H__


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class COutOfContractPlayerList : public CClubAndPlayerIDList
{
public:
	void DoEndOfWeekTasks();
	void					DoBuildSuitableList(CPlayerSearch& PlayerSearch);
	void DoSignedForClub(const short _ListID, CContract& _PlayerContract, CClub* _Club);
};
#endif
