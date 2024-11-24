

/*

	File:- HumanManager.h

	Human Manager class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __HUMAN_H__
#define __HUMAN_H__


	class CMessageDisplay;
    class CTransferNegotiationList;

class CUser
{
    // Variables
private:
    CTacticList*			m_pTacticsList;
    CNewsList*				m_pNewsList;
    CTransferNegotiationList* m_pTransferNegotiationList;
	CChairman				m_Chairman;
    CPlayerSearch*			m_pPlayerSearch;
    ushort					m_ManagerID;         // His position in the manager list
	ushort					m_WeeksPlayed;
#ifdef TROPHYHISTORY
	CTrophyHistoryList		m_TrophyHistory;
#endif
#ifdef YOUTHTRAINING
    CSquadTraining*	m_pYouthSquadTrainingDetails;
#endif
#ifdef ACTIVESTADIUMPLANS
	CTicketPrice			m_PriceBand1;
	CTicketPrice			m_PriceBand2;
	CTicketPrice			m_PriceBand3;
	double					m_BoxPrice;
#endif

public:
	// Constructors
    CUser();
    CUser(ushort nClub);
    ~CUser();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Other methods
    CString					AgeStr();
	void					AddToNews(const CExtString &_Message);
	CString					AddTransferNegotiation(CClubTransferOffer& _rTheBid);
	void					AvoidedRelegation();
	void					BeenPromotedViaPlayOff();
	bool					CheckIfValidTeam();
	void 					DoCheckTransferNegotiationsForSwapClubs(const ushort _Club1ID, const ushort _Club2ID);
	void 					CheckIfPlayerInTransferNegotiations(const ushort _PlayerID, CClub* _BiddingClub);
	bool 					DoCheckIfGotNoClub();
	bool 					DoCloseSeason();
	void					Champions(CDivision& _Division, const CString &_Season);
	void					Champions(CCup* _pCup, const CString &_Season);
	void					CreateClasses();
	void					DailyClubTasks();
	void					DeleteAll();
	void					DoSelectClub();
#ifndef TESTBED
	void					DoPreMatchReport(CMessageDisplay* _pMessageDisplay);
#endif
	void					DoEndOfWeek();
	void					DoFinishedBottomOfDivision();
	void					DoMonthlyTasks();
	void					DoManageNewClub(CClub& _ClubToManage, const uint _Amount, const ushort _Years);
	bool					DoNewSeason();
	byte					GetBoardRating();
	CChairman&				GetChairman();
    CClub&					GetClub();
	ushort					GetClubID();
    CString					GetClubName();
	ushort					GetCountryID();
	int						GetGamesPlayedClub();
    CManager&				GetManager();
	ushort					GetManagerID() const;
    CNewsList&				GetNewsList();
	void					GotNoClub();
	CPlayerSearch&			GetPlayerSearch();
	byte					GetPopularityID();
	CString					GetSalaryStr();
	byte					GetSupportersConfidence();
    CTacticList&			GetTacticsList();
    CTransferNegotiationList* GetTransferNegotiationList();
	double					GetWage();
	const ushort			GetWeeksPlayed() const;
	CYouth*					GetYouth(const uint _YouthNum);
    CSquadTraining*			GetYouthSquadTrainingDetails();
	const byte				GetYouthsTrainingSkillsPercent(const uint _YouthNum);
	bool					HasRefusedFriendly(const ushort _ClubID);
	bool					IsActive();
	bool					IsGoodEnoughToManageClub(const ushort _PopularityID);
	bool					IsManagingAClub();
	bool					IsManagingAClubInCountry(CActiveCountry& _Country);
	bool					IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match);
	void					LeftClub();
	void					RefuseFriendly(const ushort _ClubID);
	void					RemoveTransferNegotiation(const ushort _PlayerID);
	void					SetManagerID(const ushort _Num);
	void					SetFullName(CString& str, CString& str1);
	void					WonTrophy(const CString &Text);
	void					DoTraining(const ushort aTactic, const byte aHours);
	void 					DoQualifiedBottomPlayOff();
	void 					DoQualifiedTopPlayOff();
	void 					BeenPromoted();
	void 					BeenRelegated();
	int						TotalYouths();
	void					WonCharityShield(const CString &title);
	// Team tactics related
	byte 					GetCurrentStyleOfPlay();
	byte 					GetCurrentAggression();
	byte 					GetCurrentMarkingStyle();
	bool					GetOffsideTrap();
	bool 					GetCounterAttack();
	void 					SetCurrentAggression(byte _Value);
	void 					SetCurrentMarkingStyle(byte _Value);
	void 					SetCurrentStyleOfPlay(byte _Value);
	void 					SetOffsideTrap(bool _Value);
	void 					SetCounterAttack(bool _Value);

#ifdef CUSTOMFORMATIONS
	void					ChangeTacticTitle(int Which, const CString &String);
#endif
#ifdef YOUTHTRAINING
	void					SetYouthTeamTrainingHours(const byte z);
	void					SetYouthsTrainingSkillsPercent(const int x, const byte z);
	void					SetYouthsTrainingPacePercent(const int x, const byte z);
	void					SetYouthsTrainingStrengthPercent(const int x, const byte z);
	byte					GetYouthTeamTacticsToTrainHours();
	byte					GetYouthsTrainingPacePercent(const int x);
	byte					GetYouthsTrainingStrengthPercent(const int x);
	byte					GetYouthTeamTrainingHours();
	CSquadTraining*			GetYouthSquadTrainingDetails();
#endif
#ifdef TROPHYHISTORY
	void					TrophyRunnersUp(const CString &title, const byte Id, const CString &Season, bool IsCup);
	void					TrophyWinners(const CString &title, const byte Id, const CString &Season, bool IsCup);
	void					RunnersUp(CDivision& _Division, const CString &Season);
	void					RunnersUp(CCup* pCup, const CString &Season);
	void					RunnersUpPlayoff(CPlayOff* pPlayOff, const CString &Season);
	void					WonPlayoff(CPlayOff* pPlayOff, const CString &Season);
	CTrophyHistoryList&		rTrophyHistory()
	{
		return m_TrophyHistory;
	}
#endif
#ifdef MANAGEREXPENDITURE
	byte					GetLifeStyle();
	CString					BalanceStr();
	CString					WeeklyExpenditureStr();
	CString					WeeklyProfitLossStr();
#endif
};
//////////////////////////////////////////////////////////////////////////////////////////


class CUserList : public CPointerArray<CUser>
{
public:
    // Constructor
    CUserList(uint _HowMany);
    ~CUserList();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	CUser&					GetAt(const ushort _Index);

	// Methods
	void					AddToNews(CActiveCountry& _Country, const CExtString &_Message);
	bool					AnyActiveUsers();
    bool                    CheckIfValidTeams();
	ushort					CountActiveUsers();
	void					DoCheckTransferNegotiationsForSwapClubs(const ushort _Club1ID, const ushort _Club2ID);
	void					CheckIfPlayerInTransferNegotiations(ushort _PlayerID, CClub* _pBiddingClub);
	bool					DoCheckIfGotNoClubs();
	bool					DoCloseSeason();
	void					DailyTasks();
    void                    DoEndOfWeek();
	int						FindFirstActive();
	CUser&					FindManager(const ushort _ManagerID);
	bool					IsUserManager(const ushort _ManagerID);
	void					DoMakeAllInActive();
	void					MakeInActive(ushort _ManagerID);
	bool					IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match);
	void					DoMonthlyTasks();
	bool					DoNewSeason();
    void                    RemoveAllManagers();
};
#endif
