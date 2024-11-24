
/*

	C++ Classes

	ActiveCountry class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#ifndef __ACTIVECOUNTRY_H__
#define __ACTIVECOUNTRY_H__

class CActiveCountry
{
public:
	CActiveCountry(CBinaryReader& _Resource, const byte Id);
	~CActiveCountry();

	// Methods
	void					AddCup(const ushort _CupID);
	void					AddDivision(const ushort _DivisionID);
	void					AddToAvailableEmployees(CEmployee* _Employee);
	void                    AddToNewsLists(CExtString Message, const eNewsType type);
	void                    AddToNewsLists(int Message, const eNewsType type);
	void					BuildCupList(CCompetitionList* _pCupList);
	void					BuildThisWeekCupList(CCalendar* _pCurrentDate, Vector<CCup*>* _pCupList);
	void					BuildPopularityClubPtrList(CNumericArray<ushort>& _TheList, ushort _ThePopularity);
	void					BuildPlayerSkillClubPtrList(CNumericArray<ushort>& _TheList, const byte _PlayerSkill);
	void					DoBuildSeasonStr();
	void					DoCreateClubsStartingOrder();
	void					DeleteAll();
	void					DoAddPlayOff(const ushort x);
	void					DoCreateTransferList();
	void					DoDailyClearDown();
	void					DoPromoteViaPlayOffs(const ushort _PromotedClubID, CDivision& _RelegatedDivision, const ushort _RelegatedClubPosition);
	void					DoEndOfWeek(bool _NoWarnings);
	bool					DoEndOfSeason(CCountry& _Country);
	ushort					DoFindCountryDivNum(CDivision* pDivision);
	ushort					DoFindEmployeeID(CEmployee* _pEmployee);
	ushort					DoFindDivisionTopPlayOffWinnerClubID(const ushort _DivNum);
	CClub*                  DoFindRandomClub();
	CDivision&				DoFindRandomDivision();
	CPlayer*                DoFindRandomPlayer();
	CDivision*				DoFindSuitableTransferDivision(const ushort nDivisionID);
	ushort					DoFindPlayerClub(const ushort a_PlayerID);
	ushort					DoFindYouthID(CYouth* pYouth);
	void					DoMidWeekTasks(const byte _MatchNumber);																											 \
	void					DoPlayMissedMatches();
	void					DoSwapClubIDs(const ushort _Club1ID, const ushort _Club2ID);
	void					GetAllTransferListed(CClub* _Club, CPlayerSearch& _PlayerSearch);
	CNewsList&              GetNewsList();
	byte					GetID();
	CCountry&				GetCountry();
	ushort					GetDivisionID(ushort _DivisionNumber);
	CFederation&			GetFederation();
	ushort					GetPlayerSkillRandomClub(const byte _PlayerSkill);
	ushort					GetPopularityRandomClub(const ushort _ThePopularity);
	byte					GetLoanRate();
	CString					GetSeason();
	byte					GetSeasonStart();
	byte					GetSeasonEnd();
	CEmployee*				GetAvailableEmployee(ushort x);
	CEmployeeList&			GetAvailableEmployeeList();
	CYouth*					GetAvailableYouth(ushort x);
	CYouthList&				GetAvailableYouthList();
	CNumericArray<ushort>&	GetClubsStartingOrder();
	CCup*					GetCup(ushort x);
	CCharityShield*			GetCharityShield(const byte CountryId);
	CDivision&				GetDivision(const ushort CurrDiv);
	CEmployee*				GetRandomAvailableEmployee(const byte _Type, const byte _MinimumSkill, const byte _MaximumSkill);
	ushort					GetLowestDivisionNumber();
	CCupWinnersInfo&		GetCupWinnersInfo();
	CString					GetSeasonStr();
	ushort					GetHighestDivisionNumber();
	ushort					HowManyPromotedToDivisonFromPlayoffs(CDivision* DivisionPtr);
	bool					IsClubIDInCountry(const ushort _ClubID);
	bool					IsOneYearSeason();
	bool					IsRanking0();
	bool					IsTopDivision(CDivision* pDivision);
	void					DoNewSeason();
	void					RemoveStaffYouths();
	void                    SackYouth(ushort Who);
	void                    SackYouth(CYouth* yPtr);
	void                    SackEmployee(CEmployee* ePtr);
	void					SetRanking();
	void					DoStaffAndYouthEndOfWeek(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength);
	ushort					DoCountTotalClubs();
	ushort					TotalDivisions();
	ushort					TotalPlayOffs();
	ushort					TotalAvailableEmployees();
	ushort					TotalAvailableYouths();
	void					TurnRanking0On();
	void					TurnRanking0Off();
#ifdef ACTIVEFEDERATIONCUPS
	ushort					FirstClubID();
	ushort					LastClubID();
	CCupWinnersInfo&		GetCup1WinnersInfo();
	void                    DumpFederationCupsQualifiersList();
	ushort					GetFederationCupQualifier(const byte _Which);
#endif

private:
	byte					m_ID;
	byte					m_NationalStadiumId;
	bool					m_OneYearSeason;
	byte					m_LoanRate;
	byte					m_NumberClubsFedCup1;
	byte					m_NumberClubsFedCup2;
	CNumericArray<ushort>   m_DivisionIDList;
	CNumericArray<ushort>	m_ClubsStartingOrder;
	CString		            m_Season;
	CNumericArray<ushort>   m_PlayOffList;
	CNumericArray<ushort>	m_CupList;
	CNewsList               m_NewsList;
	CCupWinnersInfo			m_winnersCup[2];
	CYouthList*				m_YouthSquad;
	CEmployeeList*			m_StaffList;
#ifdef	ACTIVEREFEREES
	CNumericArray<ushort>	m_Referees;
	CNumericArray<ushort>	m_DailyRefereeList;
#endif
};
#endif