
/*

    Player class


	Football Director

	Copyright 1994-2009 Sports Director Ltd. All rights reserved. 

	---------------------------------------------------------------------------------------
*/


#ifndef __PLAYER_H__
#define __PLAYER_H__

#ifdef NDS
#include <person.h>
#include <SoccerPlayer.h>
#include <PlayerDiscipline.h>
#include <PlayerLoan.h>
#include <SoccerPlayerCareer.h>
#endif

    class CClub;
	class CMessageDisplay;
	class CYouth;
	class CCountryList;
	class CPlayerSearch;
	class CInjury;
	class CPerson;

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

	const tIDS PlayerPositionText[] =
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
#ifdef NDS
#else
	ushort					m_ID;
#endif
	CSoccerPlayer			m_Skills;

	CPlayerCareer<11, 12, 8, 7, 7, 12, 11>	m_Career;
	CPlayerCareer<11, 12, 8, 7, 7, 12, 11>	m_CupCareer;
	CPlayerCareer<11, 12, 8, 7, 7, 12, 11>	m_ClubCareer;
#ifdef INTERNATIONAL
	CPlayerCareer<11, 12, 8, 7, 7, 12, 11>	m_InternationalCareer;
#endif
	CPlayerCareer<7, 7, 6, 5, 5, 8, 6>	m_ClubSeasonCareer;
	CPlayerCareer<7, 7, 6, 5, 5, 8, 6>	m_CupSeasonCareer;
	CPlayerCareer<7, 7, 6, 5, 5, 8, 6>	m_LeagueSeasonCareer;
	CPlayerCareer<7, 7, 6, 5, 5, 8, 6>	m_LastSeasonCareer;

	CPlayerDiscipline		m_Discipline;

	struct
	{
		byte					m_HowLongPlayedLastMatch:7;
#ifdef NDS
    	bool          			m_bOnLoan : 1;
		bool					m_bDontSave : 1;
#else
		bool					m_HasTransferRecord:1;
		bool					m_HasLoanRecord:1;
#endif
		byte					m_NumberMatchesSinceLastAppeared : 7;
	} m_bitfields;

//    byte					SuccessiveAppear;
//    byte					SuccessiveGoal;
//    byte					SinceGoal;

#ifdef PLAYERTRANSFERHISTORY
		CTransferHistoryList	m_PlayerTransferHistory;
#endif

public:
	CPlayer(CClub* _pClub, ePlayerPosition Position);
    CPlayer();
    ~CPlayer() {};

	CPlayer&				operator = (const CPlayer &rhs);

	// IO Methods
	virtual void			Write(CDataFile* _pDataFile);
	virtual void			Read(CDataFile* _pDataFile);

#ifdef NDS
  	void      				Read1(CDataFile* _pDataFile);
  	void     				Read2(CDataFile* _pDataFile);
  	void      				Read3(CDataFile* _pDataFile);
  	void      				Read4(CDataFile* _pDataFile);
  	void      				Read5(CDataFile* _pDataFile);
  	void      				Read6(CDataFile* _pDataFile);

  	void      				Write1(CDataFile* _pDataFile);
  	void      				Write2(CDataFile* _pDataFile);
  	void      				Write3(CDataFile* _pDataFile);
  	void      				Write4(CDataFile* _pDataFile);
  	void      				Write5(CDataFile* _pDataFile);
  	void      				Write6(CDataFile* _pDataFile);
	void					SetDontSave();
	bool					IsDontSave() const;
#endif

     // Other Methods
	uint					ContractLengthInDays();
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
    byte					GetLeagueSeasonGoal() const					{ return m_LeagueSeasonCareer.GetGoals(); }
    ushort					GetLeagueSeasonAppear() const				{ return m_LeagueSeasonCareer.GetGoals(); }
    ushort					GetLastSeasonGoal() const					{ return m_LastSeasonCareer.GetGoals(); }
    ushort					GetLastSeasonAppear() const					{ return m_LastSeasonCareer.GetAppearances(); }
    byte					GetCupGoalsScoredThisSeason() const			{ return m_CupSeasonCareer.GetGoals(); }
    ushort					GetCupAppearancesThisSeason() const			{ return m_CupSeasonCareer.GetAppearances(); }
    ushort					GetCareerAppearances() const				{ return m_Career.GetAppearances(); }
    ushort					GetCareerGoals() const						{ return m_Career.GetGoals(); }
    ushort					GetClubAppearances() const					{ return m_ClubCareer.GetAppearances(); }
    ushort					GetClubGoal() const							{ return m_ClubCareer.GetGoals(); }
	byte					GetNumberBookingsThisSeason() const;
	byte					GetNumberSentOffThisSeason() const;
	byte					GetDisciplinaryPointsThisSeason();

	void					SetWeeksOnLoan(const byte _WeeksOnLoan);
	bool					IsCupTied();
	byte					NumberMatchesSinceLastAppeared();
    byte					GetWeeksAvailable();
	byte					GetWeeksOnLoan();
	ClubID					GetClubIDOnLoanFrom();
	byte					GetHowLongPlayedLastMatch() const;
	void					SetHowLongPlayedLastMatch(const byte _HowLongPlayed);

	// Methods
	uint					DoAdjustFeeForContractRemaining(const uint _Value);
#ifdef PLAYERTRANSFERHISTORY
    void                    AddTransferHistory(const CString &From, const uint HowMuch, const CString &When);
#endif
	void					DoAddPerformance(const byte _Value);
	void					DoAfterMatchInTeamUpdate(CClub& _Club, const byte _Performance);
	void					DoAfterMatchInSquadUpdate(CClub& _Club);
	void					DoBackFromInjury();
	void					DoIncrementAppearances(const eMatchCompetitionType _eMatchVenue, CClub& _Club, const byte _HowLongPlayed);
	void					DoLoanComplete();
	void					DoNegotiateContract(CClub* _Club, const CString& _strPrompt);
    bool                    AllowSack();
    CString					AskingPriceStr();
    void                    AssistedGoal(const eMatchCompetitionType MatchType);
    bool                    IsTransferListed();
	bool					IsLoanListed();
	bool                    BecomeManager();
    void                    BeenPromoted();
    void                    BeenRelegated();
    bool                    BeenSentOff();             // Returns true if player sent off(only called during match)
	void					DoBookPlayer(const eMatchCompetitionType _eMatchVenue);
	void					DoCheckClubPlayerRecords(CClub& _Club);
	void					DoCheckIfRetire(CClub& _Club);
    bool                    CheckAskForTransfer();
	void					DoCheckIfRequestNewContract(CClub& _Club);
	void					DoClearSeasonDisciplinaryPoints();
    void                    DoConcedeGoal(const eMatchCompetitionType c_eMatchVenue);
	void					DoWeeklyTasks();
	uint					ContractValue();
	CClub&					GetClub();
	CString 				ClubName();
	CString					PlayerAndClubName();
    void                    DoCreate(ushort nClubID, int new_or_old);
	void					DoCreatePlayer(const ushort _Age, ePlayerPosition _Position, const byte _Skill, CClub& _Club);
	void					DailyTasks(CClub* _pClub, const bool _IsWeekEnd, const bool _DoTraining);
	uint					DoCalculateAdjustedFee(CCountry& _Country);
	void					DoContractTerminated(CClub& _Club, const bool _bWasSacked);
	void					DoGenerateNonLeaguePlayer(CClub& _Club);
	void					DoGenerateRandomContract(CContract& _ContractOffered, CClub* _Club);
	byte					DoGenerateSellOnClauseForPlayer();
	uint					DoGenerateWage(CClub* _Club);
	void					DoGenerateYoungPlayer(CClub& _Club, ePlayerPosition _Position);
	void                    DoGiveNewContract();
	void					DoGoingOnLoan(CClub* _ClubFrom, byte _HowLong);
	void					DoInitialise();
	void					DoLeaveClub();
    void                    DoTraining(CClub& _Club);
	CString					DoRespondTransferRequestDenied();
	void					DoInitialiseFromResource(CBinaryReader& _Resource, CClub* _pClub);
	void					DoRetired(const ClubID _ClubID);
	void					DoRegenerate(CCountry& _Country);
	void					DoRegenerateForClub(CClub& _Club);
	byte					DoCalculateEffectiveSkill(const ePlayerWhichSkill _WhichSkill);
    bool                    DoEndOfSeason(CClub& _Club);
	CCountry&				GetBidCountry(CCountry& _Country, CCountryList& _CountryList);
    CString		            GetAskingPriceStr(CCountry& a_rCountry);
    CString		            SuggestedAskingPriceStr();
	uint					GetSuggestedAskingPrice(CCountry& _Country);
	ePlayerStatus			GetSkillViewStatus();
	ePlayerStatus			GetStatus(const ClubID _ClubID);
	ePlayerStatus			GetMatchStatus();
	CPopularity&			GetPlayerClubPopularity();
	sbyte					GetPerformance();
	byte					GetRating();
	int						GetBosmanValuation(const ClubID& _ClubID);
	uint					GetSellingPrice();
	uint					GetSellingPrice(CCountry& a_rCountry);
	uint					GetValuation(CCountry& _Country);
	CString					GetCurrentValuationStr();
	bool					HasTransferRecord();
    int						HowLongUnableToPlay();
    CString					HowLongUnableToPlayStr();
	byte					HowLongHasBeenUnableToPlay();
	ushort					DoFindID() const;
    byte					InjuryMatchPerformanceEffect();
    bool                    InjuryCarriedOff();
    CString					InjuryDetailsStr();
	bool					IsAvailableForMatch();
  	bool          			IsFairPrice(const float a_dAmount, CCountry a_rCountry);
    bool                    IsInFirstTeamOrSubs();
    bool                    IsAtCurrentUserControlledClub();
	bool					IsPlayingPosition(const ePlayerPosition _Position);
    bool                    IsPlaying();
	bool					IsInjured();
    bool                    IsOnLoan();
	bool					IsOnLoan(const ushort _ID);
	bool					IsAvailableOnBosmanFreeTransfer();
	bool					IsWillingToPlayInDivision(const byte _PotentialClubDivisionStrength, const byte _CurrentClubDivisionStrength);
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
    uint					DoCalculateMinimumTransferFee(CCountry& a_rCountry);
	uint 					DoCalculateMinimumWage(CCountry& _Country, CPopularity* _Popularity);
	void					DoNewContractRequest(CClub& _Club);
    void                    DoNewSeason();
    void                    NotAppear();
    bool                    OnLoan();
    ushort                  PercentageGamesPlayedThisSeason();
#ifndef TESTBED
#ifndef NDS
	void					PreMatchAvailabilityReportFDL(lArray<CString> * _pStringList);
#else
	void					PreMatchAvailabilityReport(CMessageDisplay* _MessageDisplay);
#endif
#endif
	void                    PrintDetails(CClub* _pClub);
	bool					QualifiesForBosmanFreeTransfer();
    void                    RaisePlayerMorales(int x);
	bool					RequiresSellOnClause();
    void                    DoScoredGoal(const eMatchCompetitionType _MatchType);
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
	bool 					DoTryListPlayer(uint _AskingPrice, CCountry& _Country, const bool _bSacked);
    bool                    DoesPlayerRetireDoToInjury();
	bool					WillingToTransfer();
    bool                    WillingToNegotiateNewContract(CClub& _Club);
	bool					WillSignMsg(CClub& _Club, const bool _Messages);
	CString					WillSignTransferMsg(CClub& _Club, const bool _RenewContract = false);
	bool					DoTryListPlayerAtEndOfContract(CClub& _Club, const bool bSacked);
	bool					ValidSearchListed(CPlayerSearch& _PlayerSearch, CCountry& _Country);
	CString					GetContractResponseStr(eContractOfferResponse eResponse, CContract* ContractOffered);
	CInjury*				GetInjury();
  	float          			GetInjuryEffect();
	CSoccerPlayer&			GetSkills();
	void					SetInjuryMatchPerformanceEffect(const byte _Value);
	void					SetAskingPrice(const uint _Value);
	void					SetHasLoanRecord(const bool _State);
	void					SetHasTransferRecord(const bool _State);
	void					SetID(const ushort _ID);
	void					SetRejectAllOffers(const bool _State);
	void					SetTransferListed(const bool _State);
	void					SetLoanListed(const bool _State);
#ifdef SHOWPLAYERIDS
	CString					GetName();
#endif
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
#ifdef NDS
class CPlayerList
{
public:
	CPlayerList() 
		: m_pPlayers(NULL)
		, m_uiMaxAllocated(0)
		, m_uiSize(0) 
		{};			// Constructor
    ~CPlayerList();				// Destructor

	void 					SetSize(const unsigned int uiSize);
	CPlayer*				GetNextFreePlayer();
	unsigned int 			GetSize() const;
	unsigned int 			Find(const CPlayer* _Element) const;
	CPlayer*				GetAt(const int iIndex);
	const CPlayer*			GetAt(const int iIndex) const;
	void 					Swap(const int iIndex1, const int iIndex2);
	void 					RemoveList();

	CPlayer&				operator [](const ushort nIndex);

    ushort					AddNew();
	ushort					AddNew(CClub* _pClub, ePlayerPosition _Position);
	void					DoInitialise(ushort _MaxClubs);
	ushort					DoFindID(CPlayer* _Player);
	void					DoRegenerateRetiredPlayers(CCountry& _Country);
    void					DoNewSeason();
	void					Read(CDataFile* _pDataFile);
	void					DoDelete(const ushort a_nPlayerID);
    void					DoRetired(const ushort a_nPlayerID, const ushort a_ClubID);
    ushort					TransferFromYouth(CYouth* pYouth, CContract* Contract);
	void					Write(CDataFile* _pDataFile);

private:
	static const int NONLEAGUEPLAYERMIN = 2236;
	static const int NONLEAGUEPLAYERMAX = 3868;

	CPlayer*				m_pPlayers;
	unsigned int 			m_uiMaxAllocated;
	unsigned int 			m_uiSize;
};

class CPlayerIDList : public Vector<ushort>
{
public:
	CPlayerIDList();

	void					DoBuildFromSquad(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
	void					DoBuildFromTeamPlayingOutField(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
    void					DoBuildFromTeamPlayingPosn(CPlayerSquad& aPlayerIDList, CPlayerList& OldList, ePlayerPosition Posn);
	void					DoRemoveAllPosition(ePlayerPosition Posn);
	void					DoRemoveNotPosition(ePlayerPosition _Posn);
	void					DoSortByOverallSkill();
	void					DoSortByEffectiveSkill();
	ushort					DoFindWorstPlayer(const int _WorstValue);
	byte					AverageSkill();
	CString					RandomSurName();
};
#else

class CPlayerList : public CPointerArray<CPlayer>
{
public:
    CPlayerList() {};			// Constructor
    ~CPlayerList();				// Destructor

	CPlayer&				operator [](const ushort nIndex);

	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

	ushort					AddNew();
	ushort					AddNew(CClub* _pClub, ePlayerPosition _Position);
	byte					AverageSkill();
	void					DoBuildFromSquad(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
	void					DoBuildFromTeamPlayingOutField(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList);
    void					DoBuildFromTeamPlayingPosn(CPlayerSquad& aPlayerIDList, CPlayerList& OldList, ePlayerPosition Posn);
	void					DoInitialise(ushort _MaxClubs);
	void					DoDelete(const ushort a_nPlayerID);
	CPlayer*				DoFindWorstPlayer();
	void					DoNewSeason();
	void					DoRegenerateRetiredPlayers(CCountry& _Country);
    void					DoRetired(const ushort a_nPlayerID, const ushort a_ClubID);
	void					DoRemoveAllPosition(ePlayerPosition Posn);
	void					DoRemoveNotPosition(ePlayerPosition _Posn);
	void					DoSortByOverallSkill();
	void					DoSortByEffectiveSkill();
	CString					RandomSurName();
	ushort					TransferFromYouth(CYouth* _Youth, CContract* _Contract);
};
#endif
#endif
