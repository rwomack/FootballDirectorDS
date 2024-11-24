

/*

	ClubList class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __CLUBLIST_H__
#define __CLUBLIST_H__



class CClubList : public CPointerArray<CClub>
{
public:
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

    CClub&                  operator [](const ushort x);
	void                    CreateFriendlyMatch(const ushort MatchNumber, const ushort HomeClubIndex, const ushort AwayClubIndex);
	byte					DoAddCupClubs(CBinaryReader& _Resource, CCountry& _Country);
	CClub*                  DoFindPlayerClub(const ushort a_nPlayerID);
	CClub*                  DoFindPlayerClub(CPlayer* pPlayer);
	ushort					DoFindPlayerClubNumber(const ushort _PlayerID);
	ushort                  DoFindPlayerClubNumber(CPlayer* pPlayer);
	CClub*                  DoFindRandomClub();
	void					DoInitialise();
	void					DoPromoteAndRelegateClubs(ClubID cPromotedClub, ClubID cRelegatedClub);
	void					DoSwapClubDivisionPositions(ClubID _Club1ID, ClubID _Club2ID);
	void					DoWeeklyClearDown();
	void					DoDailyTasks();
	void					DoAddClubsSuitableForPlayer(CPlayer& _Player, CClubList& _SuitableClubs, CClub* _ClubToAvoid = null);
    void                    DumpNames();
    ushort                  Find(CClub* pClub);
    ushort                  Find(CString ClubName);
	CClub&					GetAt(int _Index);
	CClub&					GetByID(const ClubID _ClubID);
	CClub*					GetEmployeeClub(CEmployee* _Employee);
	CClub&					GetYouthClub(CYouth* _Youth);
    int                     HowManyUnManagedClubs();
	void                    PostInitialise();
    void                    ReadNames();
    void                    SortTableData(int iMode = 0);
	void					SetManagerID(int _IndVal, ushort _ManNum);
};
#endif
