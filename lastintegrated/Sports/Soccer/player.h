
/*

    Player class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#ifndef __PLAYER_H__
#define __PLAYER_H__



    class CClub;
	class CMessageDisplay;
	class CYouth;
	class CCountryList;
	class CPlayerSearch;
	class CInjury;

	enum ePlayerStatus
	{
		PLAYERSTATUS_INJURED,
		PLAYERSTATUS_SUSPENDED,
		PLAYERSTATUS_CUPTIED,
		PLAYERSTATUS_LOANEDOUT,
		PLAYERSTATUS_LOANEDIN,
		PLAYERSTATUS_TRANSFERLISTED,
		PLAYERSTATUS_SUSPENSIONNEAR,
		PLAYERSTATUS_LOANLISTED,
		PLAYERSTATUS_RETIRING,
		PLAYERSTATUS_CONTRACTEXPIRING,

		MAXPLAYERSTATUSES
	};

	const int PlayerPositionText[] =
	{
		IDS_GOALKEEPER,
		IDS_DEFENDER,
		IDS_MIDFIELDER,
		IDS_ATTACKER
	};
#ifdef STOREMANOFMATCH
class CPlayerManOfMatchAwards
{
public:
    CPlayerManOfMatchAwards()
	{
		DoResetVariables();
	}
    ~CPlayerManOfMatchAwards() {};

	// Methods
    void					AwardedManOfMatch();
	void					DoJoinedNewClub();
	void					DoResetVariables();
	void					NewSeason();
    byte					NumberManOfMatchAwardsThisSeason()
	{
		return m_ManOfMatchAwardsThisSeason;
	}

private:
	uint					m_ManOfMatchAwardsThisClub:8;
	uint					m_ManOfMatchAwardsCareer:8;
	uint					m_ManOfMatchAwardsThisSeason:4;
};
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CPlayer  : public CPerson
{
	// Variables
private:
    CSoccerPlayer			m_Skills;
	CPlayerTransferStatus	m_TransferStatus;
	CPlayerLoan				m_LoanInfo;

	CPlayerCareer			m_Career;
	CPlayerCareer			m_CupCareer;
	CPlayerCareer			m_ClubCareer;
	CPlayerCareer			m_ClubSeasonCareer;
	CPlayerCareer			m_CupSeasonCareer;
	CPlayerCareer			m_LeagueSeasonCareer;
	CPlayerCareer			m_LastSeasonCareer;

	CPlayerDiscipline		m_Discipline;

	byte					m_HowLongPlayedLastMatch;
	byte					m_NumberMatchesSinceLastAppeared;

//    byte					SuccessiveAppear;
//    byte					SuccessiveGoal;
//    byte					SinceGoal;

#ifdef PLAYERTRANSFERHISTORY
		CTransferHistoryList	m_PlayerTransferHistory;
#endif
#ifdef INTERNATIONAL
	CPlayerCareer		m_InternationalCareer;
#endif

public:
	CPlayer(CClub* _pClub, ePlayerPosition Position);
    CPlayer();
    ~CPlayer() {};

     // Data Access Methods
	CPlayerDiscipline&		GetDiscipline();
#ifdef ASSISTS
	byte					GetLeagueGoalsAssistedThisSeason() const	{ return static_cast <byte>(m_LeagueSeasonCareer.GetAssists()); }
	byte					GetCupGoalsAssistedThisSeason() const		{ return static_cast <byte>(m_CupSeasonCareer.GetAssists()); }
    byte					GetSeasonAssist() const						{ return m_LeagueSeasonCareer.GetAssists() + m_CupSeasonCareer.GetAssists(); }
#endif
    byte					GetNumberAppearancesThisSeason() const		{ return m_LeagueSeasonCareer.GetAppearances() + m_CupSeasonCareer.GetAppearances(); }
    byte					GetNumberGoalsThisSeason() const			{ return m_LeagueSeasonCareer.GetGoals() + m_CupSeasonCareer.GetGoals(); }
    ushort					GetClubSeasonGoal() const					{ return m_ClubSeasonCareer.GetGoals(); }
    ushort					GetClubSeasonAppear() const					{ return m_ClubSeasonCareer.GetAppearances(); }
    ushort					GetLeagueSeasonGoal() const					{ return m_LeagueSeasonCareer.GetGoals(); }
    ushort					GetLeagueSeasonAppear() const				{ return m_LeagueSeasonCareer.GetGoals(); }
    ushort					GetLastSeasonGoal() const					{ return m_LastSeasonCareer.GetGoals(); }
    ushort					GetLastSeasonAppear() const					{ return m_LastSeasonCareer.GetAppearances(); }
    byte					GetCupGoalsScoredThisSeason() const			{ return static_cast <byte>(m_CupSeasonCareer.GetGoals()); }
    ushort					GetCupAppearancesThisSeason() const			{ return m_CupSeasonCareer.GetAppearances(); }
    ushort					GetCareerAppearances() const				{ return m_Career.GetAppearances(); }
    ushort					GetCareerGoals() const						{ return m_Career.GetGoals(); }
    ushort					GetClubAppearances() const					{ return m_ClubCareer.GetAppearances(); }
    ushort					GetClubGoal() const							{ return m_ClubCareer.GetGoals(); }
	byte					GetNumberBookingsThisSeason() const			{ return m_LeagueSeasonCareer.GetBookings() + m_CupSeasonCareer.GetBookings(); }
	byte					GetNumberSentOffThisSeason() const			{ return m_LeagueSeasonCareer.GetSendingOffs() + m_CupSeasonCareer.GetSendingOffs(); }
	byte					GetDisciplinaryPointsThisSeason()			{ return GetNumberBookingsThisSeason() * 3 + GetNumberSentOffThisSeason() * 6; }

	void					SetWeeksOnLoan(const byte _WeeksOnLoan);
	void					DoLoanComplete();
	bool					IsCupTied();
	byte					NumberMatchesSinceLastAppeared();
    byte					GetWeeksAvailable();
	byte					GetWeeksOnLoan();
	ushort					GetClubIDOnLoanFrom() const;
	byte					GetHowLongPlayedLastMatch() const;
    void					SetSinceAppear(const byte x);
	void					SetHowLongPlayedLastMatch(const byte _HowLongPlayed);        

	// Methods
	uint					DoAdjustFeeForContractRemaining(const uint _Value);
	void					DoIncrementAppearances(const eMatchCompetitionType _eMatchVenue, CClub& _Club, const byte _HowLongPlayed);
#ifdef PLAYERTRANSFERHISTORY
    void                    AddTransferHistory(const CString &From, const uint HowMuch, const CString &When);
#endif
	void					DoAfterMatchInTeamUpdate(CClub& _Club, const byte _Performance);
	void					DoAfterMatchInSquadUpdate(CClub& _Club);
    bool                    AllowSack();
    CString					AskingPriceStr();
    void                    AssistedGoal(const eMatchCompetitionType MatchType);
    bool                    IsTransferListed();
	bool					IsLoanListed();
	void					DoAddPerformance(const byte _Value);
	void					DoBackFromInjury();
	bool                    BecomeManager();
    void                    BeenPromoted();
    void                    BeenRelegated();
    bool                    BeenSentOff();             // Returns true if player sent off(only called during match)
	void					DoBookPlayer(const eMatchCompetitionType _eMatchVenue);
	void					DoClubTransferListPlayer(CCountry& _Country);
	void					DoCheckClubPlayerRecords(CClub& _Club);
	void					DoCheckForSwapClubs(const ushort _Club1ID, const ushort _Club2ID);
	void					DoCheckIfRetire(CClub& _Club);
    bool                    CheckAskForTransfer();
	void					DoCheckIfRequestNewContract(CClub& _Club);
	void					DoClearSeasonDisciplinaryPoints();
    void                    DoConcedeGoal(const eMatchCompetitionType c_eMatchVenue);
	void					DoWeeklyTasks();
	uint					ContractValue();
	CClub&					GetClub();
    CString		            ClubName();
	CString					PlayerAndClubName();
    void                    DoCreate(ushort nClubID, int new_or_old);
	void					DoCreatePlayer(const ushort _Age, ePlayerPosition _Position, const byte _Skill, CCountry& _Country);
	void					DailyTasks(CClub* _pClub, const bool _IsWeekEnd, const bool _DoTraining);
	uint					DoCalculateAdjustedFee(CCountry& _Country);
	void					DoContractTerminated(CClub& _Club);
	void					DoInitialise();
    void                    DoTraining(CClub& _Club);
	CString					DoRespondTransferRequestDenied();
	void					DoInitialiseFromResource(CBinaryReader& _Resource, CClub* _pClub);
	void					DoRetired(const ushort _ClubId);
	void					DoRegenerate(const ushort _PlayerID, CCountry& _Country);
	byte					DoCalculateEffectiveSkill(const ePlayerWhichSkill _WhichSkill);
    bool                    DoEndOfSeason(CClub& _Club);
	void					DoGenerateNonLeaguePlayer(CCountry& _Country);
	void					DoGenerateYoungPlayer(CClub& _Club, ePlayerPosition _Position);
	CCountry&				GetBidCountry(CCountry& _Country, CCountryList& _CountryList);
    CString		            GetAskingPriceStr(CCountry& a_rCountry);
    CString		            SuggestedAskingPriceStr();
	uint					GetSuggestedAskingPrice(CCountry& _Country);
	ePlayerStatus			GetSkillViewStatus();
	ePlayerStatus			GetStatus(const ushort _ClubID);
	ePlayerStatus			GetMatchStatus();
	CPopularity&			GetPlayerClubPopularity();
	sbyte					GetPerformance();
	byte					GetRating();
	void					DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _Country);
	uint					GetSellingPrice();
	uint					GetSellingPrice(CCountry& a_rCountry);
	uint					GetValuation(CCountry& _Country);
	CString					GetCurrentValuationStr();
	byte					DoGenerateSellOnClauseForPlayer();
    void                    DoGiveNewContract();
	void					DoGoingOnLoan(CClub* _ClubFrom, byte _HowLong);
    int						HowLongUnableToPlay();
    CString					HowLongUnableToPlayStr();
	byte					HowLongHasBeenUnableToPlay();
	ushort					DoFindID();
    byte					InjuryMatchPerformanceEffect();
    bool                    InjuryCarriedOff();
    CString					InjuryDetailsStr();
	bool					IsAvailableForMatch();
	bool					IsFairPrice(const double a_dAmount, CCountry a_rCountry);
    bool                    IsInFirstTeam();
    bool                    IsAtCurrentUserControlledClub();
	bool					IsPlayingPosition(const ePlayerPosition _Position);
    bool                    IsPlaying();
	bool					IsInjured();
    bool                    IsOnLoan();
	const bool				IsWillingToPlayInDivision(const byte DivisionStrength);
    void                    DoJoinedNewClub(CContract& _PlayerContract);
    CString		            LastClubName();
	void					DoUpdateTransferStatus(bool bToList, const uint a_nFee);
	void					DoTransferList(const uint _AskingPrice, CCountry& _Country);
    void                    DoTakeOffTransferList();
	CClub*					DoFindSuitableNewClub(CClub* _ClubToAvoid);
	void					DoUpdateContractOffered(CContract& _ContractOffered);
    void                    LowerPlayerMorales(int x);
	bool					ShouldMoveAbroad();
    void                    DoMakeOld();
    void                    DoMakeYoung();
    void                    DoResetVariables();
    void                    DoMakeInvincible();
	CString					MatchAvailabilityStr();
    void                    DoMakeFit();
	uint					DoCalculateMaximumTransferFee(CCountry& _Country);
    const uint				DoCalculateMinimumTransferFee(CCountry& a_rCountry);
	uint DoCalculateMinimumWage(CCountry& _Country, CPopularity* _Popularity);
    void                    NewContractRequest(CClub& _Club);
    void                    DoNewSeason();
    void                    NotAppear();
    bool                    OnLoan();
    ushort                  PercentageGamesPlayedThisSeason();
    CString		            PointsText();
#ifndef TESTBED
	void					PreMatchAvailabilityReport(CMessageDisplay* _MessageDisplay);
#endif
	void                    PrintDetails(CClub* _pClub);
    bool                    QualifiesForBosman();
    void                    RaisePlayerMorales(int x);
	bool					RequiresSellOnClause();
    void                    DoScoredGoal(const eMatchCompetitionType _MatchType);
	CString					SeasonGoalText();
	void					DoSendPlayerOff(const eMatchCompetitionType _eMatchVenue);
	void					TransferFromYouth(CYouth* _Youth, CContract* _Contract);
	void					DoTransferRequestCanceled();
	bool					OnTransferRequestGranted(CCountry& _Country);
	void					OnTransferRequestRefused();
    bool                    DoTryBookPlayer();
    eContractOfferResponse	TryAcceptContract(CContract* ContractRequired, CContract* ContractOffered, CClub& _OfferingClub);
    CString		            TryAcceptTransferContract(CContract* ContractRequired, CClub* _pClub, CContract* ContractOffered);
    bool	                TryAcceptLoanTransferBid(CClub* pBiddingClub, CString* szResponse);
	bool					DoTryCreateRandomEventPlayer(CClub* _Club);
    bool                    DoTryRenewNonUserPlayerContract(CClub& _Club);
	bool 					DoTryInjurePlayer(CClub& _Club, const CInjuryType::eInjurySeverity _Severity = CInjuryType::MAXINJURYSEVERITIES);
	bool 					DoTryInjurePlayerInTraining(CClub& _Club, const CInjuryType::eInjurySeverity _Severity = CInjuryType::MAXINJURYSEVERITIES);
	bool 					DoTryListPlayer(uint _AskingPrice, CCountry& _Country);
    bool                    DoesPlayerRetireDoToInjury();
	bool					WillingToTransfer();
    bool                    WillingToNegotiateNewContract(CClub& _Club);
	bool					WillSignMsg(CClub& _Club, const bool _Messages);
	CString					WillSignTransferMsg(CClub& _Club, const bool _RenewContract = false);
	bool					ValidSearchListed(CPlayerSearch& _PlayerSearch);
	CInjury*				GetInjury();
	double					GetInjuryEffect();
	CString					GetContractResponseStr(eContractOfferResponse eResponse, CContract* ContractOffered);
	void					SetInjuryMatchPerformanceEffect(const byte _Value);
	bool					DoTryListPlayerAtEndOfContract(CClub& _Club);
	CSoccerPlayer&			GetSkills();
	CPlayerTransferStatus&	GetTransferStatus();
#ifdef SURGICALINJURIES
	CString					SurgeryResultStr();
#endif
#ifdef STOREMANOFMATCH
    void					AwardedManOfMatch()							
	{ 
		m_ManOfMatchAwards.AwardedManOfMatch(); 
	}
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPlayerList : public CPointerArray<CPlayer>
{
public:
    CPlayerList() {};			// Constructor
    ~CPlayerList();				// Destructor

	CPlayer&				operator [](const ushort nIndex);

    ushort					AddNew();
	ushort					AddNew(CClub* _pClub, ePlayerPosition _Position);
	byte					AverageSkill();
	void					DoBuildFromSquad(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
	void					DoBuildFromTeamPlayingOutField(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
    void					DoBuildFromTeamPlayingPosn(CPlayerSquad& aPlayerIDList, CPlayerList& OldList, ePlayerPosition Posn);
	void					DoInitialise(ushort _MaxClubs);
	void					DoCheckForSwapClubs(const ushort _Club1ID, const ushort _Club2ID);
	void					DoRemoveAllPosition(ePlayerPosition Posn);
	void					DoRemoveNotPosition(ePlayerPosition _Posn);
	void					DoSortByOverallSkill();
	void					DoSortByEffectiveSkill();
	CPlayer*				DoFindWorstPlayer(const int _WorstValue);
	ushort					DoFindID(CPlayer* _Player);
	void					DoRegenerateRetiredPlayers(CCountry& _Country);
    void					DoNewSeason();
	void					Read(CDataFile* _pDataFile);
	void					DoDelete(const ushort a_nPlayerID);
    void					DoRetired(const ushort a_nPlayerID, const ushort a_ClubID);
    ushort					TransferFromYouth(CYouth* pYouth, CContract* Contract);
	CString					RandomSurName();
	void					Write(CDataFile* _pDataFile);
};

#endif
