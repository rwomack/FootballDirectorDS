
/*

    Player Sort List class

	Football Director

	(c) RW Software 1994 - 2005

  ---------------------------------------------------------------
*/


#ifndef __SORTPLAYER_H__
#define __SORTPLAYER_H__



class CSortPlayerList : public CPlayerList
{
public:
    CPlayerList() {};			// Constructor
    ~CPlayerList();				// Destructor

	void					SortByPosnSkill(void);
	void					SortByFitness(void);
	void					SortByDisciplinary(void);
	void					SortByInjuryLength(void);
	void					SortBySuspension(void);
	void					SortBySeasonAppearances(void);
	void					SortBySeasonGoals(void);
	void					SortByClubAppearances(void);
	void					SortByClubGoals(void);
	void					SortByCareerAppearances(void);
	void					SortByCareerGoals(void);
	void					SortByAge(void);
	void					SortByWage(void);
	void					SortByContract(void);
};

#endif
