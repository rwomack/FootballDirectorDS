
 /*

	Manager class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#ifndef __MANAGE_H__
#define __MANAGE_H__


	static const int		MAXMANAGERHISTORY = 2;


class CManager : public CPerson
{
public:
	// Constructors
    CManager(const uint _Wage, const byte _Contract, const ClubID _ClubID);
    CManager(CPlayer* _pPlayer, const uint _Wage, const byte _Contract, const ClubID _ClubID);
    CManager(CEmployee* _pEmployee, const uint _Wage, const byte _Contract, const ClubID _ClubID);
    CManager();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					AddPoints(const ushort _Points);
	void					BeenPromoted();
	void					BeenRelegated();
	bool					CheckIfGotContract();
	void					DoAddTransferIncome(const uint _Amount);
	void					DoAddTransferExpenditure(const uint _Amount);
	void					DoMakeActive(CPlayer* _Player, const uint _Wage, const byte _Contract, const ClubID _ClubID);
	void					DoRetire();
	void					DoClearMOM();
	void					DoClearTable();
	bool					DoAdjustRating(const short _PopularityDifference);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	bool					DoEndOfSeason();				// true if manager is retiring
	bool					DoEndOfWeekTasks();
	uint					DoCalculateRating();
	void					DoNewClub(CClub& _NewClub);            // Wage and contract are stored in club data
	void					DoNewSeason();
  	void          			DoRaiseRating(float howmmany);
	bool					DoPlayedLeagueMatch(CFixture& _Fixture);
	bool					DoRatingAdjustmentDrawnMatch(short _PopularityDifference);
	bool					DoRatingAdjustmentLostMatch(const short _PopularityDifference);
	void					DoRatingAdjustmentWonMatch(short _PopularityDifference);
	void					DoFinishedBottomOfDivision();
	void					DoFinishedTopOfDivision(bool _IsTopDivision);
	void					DoQualifiedTopPlayOff();
	bool					DoQualifiedBottomPlayOff();
	CClub*					GetCurrentClub();
	ClubID					GetCurrentClubID() const;
	CString&				GetClubName();
	CString					GetClubUpperCaseName();
	uint 					GetClubTransferIncome();
	uint					GetClubTransferExpenditure();
	uint 					GetCareerTransferIncome();
	uint 					GetCareerTransferExpenditure();
	sbyte					GetBoardConfidence();
	sbyte					GetSupportersConfidence();
	int						GetGamesPlayedClub() const;
	uint 					GetSeasonTransferExpenditure()	const;
	uint					GetSeasonTransferIncome() const;
	ClubID					GetLastClubID()	const;
	byte					GetPointsSinceMOM() const;
	ushort					GetSeasonPoints() const;
	ushort					GetTotalPoints() const;
	CRating&				GetRatingController();
	CGameMatchScore&		GoalsSinceMOM();
	bool					IsActive();
	bool					IsManagingClub();
	bool					IsManagingAClubInCountry(CActiveCountry& _Country);
	bool					IsUser() const;
	CString					LastClubName();
	CString					LastClubUpperCaseName();
	CClub*					GetLastClub();
	void					DoLeaveClub();
  	bool          			DoLowerRating(float howmmany);
	void					DoResetVariables();
	bool					IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match);
	void					OnInitialise(const uint _Wage, const byte _Contract, const ClubID _ClubID);
    bool					PlayedCupMatch(CFixture& aFixture);
	void					RenewClubContract();
	void					RunnerUpInCup(CCup* _Cup, const CString &_SeasonStr);
	CString					SalaryStr(CCountry& rCountry);
	void					SetClubTransferIncome(const uint x);
	void					SetClubTransferExpenditure(const uint x);
	void					SetIsUser(bool _IsUser);
	void					SetPointsSinceMOM(const byte _Points);
	void					SetSeasonPoints(const byte _Value);
	void					SetSeasonTransferIncome(const byte _Value);
	void					SetSeasonTransferExpenditure(const byte _Value);
	void					SetTotalPoints(const byte _Value);
	void					SetWage(const uint x);
    bool					TrySackManager();
	CString					WageStr(CCountry& rCountry);
	void					WonCup(CCup* _Cup, const CString &_SeasonStr);
	void					WonMOM(const CString &_Season);
	void					WonTitle(bool _IsTopDivision, const CString &_SeasonStr);

#ifdef MANAGERSTRESS
	sbyte					GetStressLevel() const
	{
		return m_StressLevel;
	}
	void					SubtractFromStressLevel(sbyte x)
	{
		m_StressLevel -= x;
	}
#endif
#ifdef MANAGEREXPENDITURE
	byte					GetLifeStyle();
	uint					GetBankBalance() const
	{
		return m_Data.m_BankBalance;
	}
	uint 					GetWeekExpenditure() const
	{
		return m_Data.m_WeekExpenditure;
	}
	CString					WeeklyExpenditureStr(CCountry& rCountry);
	CString					WeeklyProfitLossStr(CCountry& rCountry);
	CString					BalanceStr(CCountry& rCountry);
	uint					NextWeeksBalanceIfNoClub();
#endif
#ifdef ACTIVEFEDERATIONCUPS
	void					QualifiedForFederationCup();
#endif

	// Variables
private:
	class CManagerData
	{
	public:
		uint				m_SeasonTransferIncome;
		uint 				m_SeasonTransferExpenditure;
		uint				m_SeasonPoints:7;
		uint				m_TotalPoints:18;
		uint				m_PointsSinceMOM:6;
		uint				m_IsUser:1;
#ifdef MANAGERSTRESS
		sbyte				m_StressLevel;
#endif
#ifdef MANAGEREXPENDITURE
		uint 				m_BankBalance;
		uint				m_WeekExpenditure;
#endif
	};
	CManagerData			m_Data;
    CManagerClubHistory		m_ManHistory[MAXMANAGERHISTORY];
    CManagerHistory			m_ClubHistory;
    CManagerHistory			m_SeasonHistory;
    CManagerHistory			m_CareerHistory;
	CRating					m_ratingManager;
    CGameMatchScore			m_GoalsSinceMOM;
	CConfidence				m_BoardConfidence;
	CConfidence				m_SupportersConfidence;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
class CManagerList : public CPointerArray<CManager>
{
public:
	// Constructor
    CManagerList() {};
    ~CManagerList();

	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	ushort					DoAddNewManager(CPlayer* _Player, const uint _Wage, const byte _Contract, const ClubID _ClubID);
	void					DoBuildUnemployedList(CManagerList& _TheList, const ushort _Country, const int _MinRating, const int _MaxRating);
    void					DoEndOfWeekTasks();
	ushort					DoFindID(CManager* _Manager);
	void					DoInitialise();
	void					DoNewSeason();
	void					RestoreCurrentManager();
#ifdef MANAGERTABLES
	void					DoSwapListPositions(const ushort aElement1, const ushort aElement2);
    void					SortSeasonPoints(int left, int right);
	void					SortPointsSinceMOM(int left, int right);
	void					SortTotalPoints(int left, int right);
#endif
};
#endif
