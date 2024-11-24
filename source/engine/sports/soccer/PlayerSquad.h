

/*

	Class - C++ Classes

	PlayerSquad class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __PLAYERSQUAD_H__
#define __PLAYERSQUAD_H__

#include <defines.h>

static const byte		GOODFLAIR = 85;
static const uint		MAXIMUMGOALKEEPERS = 6;
static const uint		MAXIMUMDEFENDERS = 10;
static const uint		MAXIMUMMIDFIELDERS = 10;
static const uint		MAXIMUMATTACKERS = 10;
static const ushort		MAXPLAYERLOANSPERSEASON = 6;
static const uint		PLAYERSINTEAM = 11;
static const uint		PLAYERSINSQUAD = 30;                // Squad size for club
static const int		MINSQUADSIZE = 18;					// Cant sack or sell players if less than this in squad

// Player types
enum ePlayerPosition
{
	FIRSTPOSN,

	GOALKEEPER = FIRSTPOSN,
	DEFENDER,
	MIDFIELDER,
	ATTACKER,

	LASTPOSN = ATTACKER,
	MAXPLAYERPOSITIONS
};

#include <player.h>

class CContract;
class CInjuryType;
class CLeadingDisciplinaryList;
class CLeadingGoalScorerList;
class CPlayerSearch;
class CSortedStringList;
class CYouth;

class CPlayerSquad
{
	// Variables
private:
	ushort					m_listSquadID[PLAYERSINSQUAD];
	ushort					m_CaptainID;
	ushort					m_PenaltyTakerID;
	ushort					m_CornerTakerID;
	ushort					m_FreeKickTakerID;
	byte					m_PlayersInOnLoanThisSeason;
	byte					m_PlayersOutOnLoan;
	byte					m_PlayersInOnLoan;
	byte					m_AverageStrengths[MAXPLAYERPOSITIONS];

 public:
	CPlayerSquad();
	CPlayerSquad(CPlayerSquad& arPlayerSquad);

	CPlayerSquad&			operator = (CPlayerSquad& arPlayerSquad);

	ushort					GetCaptainSquadNum();
	ushort					GetPenaltyTakerSquadNum();
	ushort					GetCornerTakerSquadNum();
	ushort					GetFreeKickTakerSquadNum();
	ushort					GetCaptainID() const;
	ushort					GetPenaltyTakerID() const;
	ushort					GetFreeKickTakerID() const;
	ushort					GetCornerTakerID() const;
	ushort					GetSquad(const ushort _Index);
	CPlayer&				GetPlayer(const ushort _SquadNumber);
	CPlayer*				GetLeadingScorer(CPlayer* pIgnorePlayer = NULL, const bool bIgnoreUnavailablePlayers = false);
	CPlayer*				GetLeadingFouler(CPlayer* pIgnorePlayer = NULL, const bool bIgnoreUnavailablePlayers = false);
	CPlayer*				GetPenaltyTaker();
	uint					GetPlayersInOnLoan() const;
	uint					GetPlayersOutOnLoan() const;
	ushort					GetLoansThisSeason() const;
    void					SetSquad(const ushort aSquadNum, const ushort aPlayerID);
    void					SetCaptainSquadNum(const ushort aSquadNum);
    void					SetPenaltyTakerSquadNum(const ushort aSquadNum);
    void					SetCornerTakerSquadNum(const ushort aSquadNum);
    void					SetFreeKickTakerSquadNum(const ushort aSquadNum);
	void 					SetLoansThisSeason(const byte _LoanCount);

	void					AddLeadingDiscipinaryPoints(const ClubID _CurrentClubID, const int MinDiscPoints, CLeadingDisciplinaryList* ListPtr);
	void					AddLeadScorers(const ClubID _CurrentClubID, const byte _MinGoals, CLeadingGoalScorerList* _pList);
	bool					AllowInjuries();
	uint					AllowToHirePlayer(CPlayer* _Player, const bool _ShowMessage = false);
	int                     AverageSquadMorale();
	void					BeenPromoted();
	ushort					BestPlayingPosition(const bool SquadNumber, const ePlayerPosition ePosition);
	void					BuildFitPlayerNameList(CSortedStringList& _theList);
	void					DoAfterMatchUpdate(const ushort _PrimaryTactics, const byte _OurGoals, const byte _TheirGoals, CFixture& _Fixture, const eWhichTeam _WhichTeam, CClub& _Club);
	ushort					DoFindPlayerIDInSquad(const ushort _PlayerID);
	void					DoIncrementPlayersOutOnLoan();
	void					DoDecrementPlayersInOnLoan();
	void					DoDecrementPlayersOutOnLoan();
	void					DoCopy(CPlayerSquad& arPlayerSquad);
	void					DoFansReactionToSale(CPlayer& arPlayer, const uint aCost);
	CString					DoFansReactionToSigning(CPlayer& arPlayer);
	void					DoAddPlayerToSquad(const ushort anPlayerID);
    byte					DoCalculateAverageTeamPlayerSkill();
  	float          			DoCalculateAveragePositionAge(ePlayerPosition Posn);
  	float          			DoCalculateAverageTeamPlayerAge();
  	float          			DoCalculateAverageSquadPlayerAge();
	void					DoBuildTeamTempersArray(CNumericArray<byte>* _PlayerTempers);
    byte					DoCalculateAverageSkill(const ePlayerPosition Posn);
	void					DoCalculateAverageStrengths();
	int						DoCalculateWageBill();
	bool					DoRequirePlayerPosition(const ePlayerPosition _Position);
	bool					CanAffordToLosePlayer(CPlayer* _Player, const bool _LoanPlayer = false);
	void					ClearSpecialPlayerIfCurrent(const ushort a_nPlayerToRemove);
	ushort					DoCountAllPlayersPosn(const ePlayerPosition Posn);
    ushort					CountFootedPlayersPosn(const ePlayerPosition aPosn, const eBESTFOOT aeFoot);
    uint					CountPlayersSeekingMove();
    ushort					DoCountAvailablePlayers();
    ushort					DoCountAvailablePlayersPosn(const ePlayerPosition Posn);
	byte					DoCountAvailableNonLoanedPlayers();
    byte					DoCountSuspendedPlayers();
    byte					DoCountInjuredPlayers();
    ushort					DoCountListedPlayers();
	ushort					DoCountStarPlayers(const byte MinStarPlayerSkill);
	int						CountPlayersScoredAtLeast(const uint iMin);
	int						CountPlayersFouledAtLeast(const uint iMin);
	ushort					DoCountRecentlySignedPlayers(const ushort aWeeksLessThan);
	void					DoCreateSquad(CClub* _Club);
	void					DoDailyTasks(CClub* _Club, const bool _IsWeekEnd, const bool _DoTraining, const uint _TimeTillNextMatch);
    void                    DoDeleteAllPlayers(const ClubID a_nClubID);
	ushort					DoFindPlayerSquadNumber(const ushort a_PlayerID);
	void					DoSelectTeam(const byte _NumSubs, CClub& _Club);
	void					DumpSquad();
	void					DoEndOfSeason(CClub& _Club);
	void					DoEnsureEnoughPlayers(CClub& _Club);
    void                    DoFindBestPlayer(const ushort FirstPlayer, const ePlayerPosition Posn);
    ushort					DoFindFitPlayerID(const ushort FirstPlayer, const ushort LastPlayer);
	ushort					DoFindPlayerID(CPlayer* _Player);
    ushort					DoFindSquadNumber(const ushort aPlayerID);
    ushort					DoFindSquadNumber(CPlayer* pPlayer);
	ePlayerPosition			DoFindWeakestPosition() const;
	ushort					DoFindWorstPlayingOutfieldPlayer(const int _WorstValue);
	ushort					DoFindWorstPlayingOutfieldPlayerPosition(const int _WorstValue, ePlayerPosition _Position);
    ushort					FreeSquadNumber();
	void					DoGenerateNonLeagueSquad(const ushort _HowMany, CCountry& _Country);
	void					DoGenerateYoungPlayers(const ushort HowMany, CClub& _Club);
	void                    GetAllTransferListed(const ClubID _ClubID, CPlayerSearch& PlayerSearch);
    bool                    DoGivePlayerFreeTransferToClub(const ePlayerPosition Posn, CClub* a_pClub);
	void					DoGivePlayersInjury(const ushort _HowMany, const CInjuryType::eInjurySeverity _Severity);
	bool					DoMakeFitPlayer(const ePlayerPosition Posn);
	void					DoNonLeagueClubEndOfSeason();
	void					GotPlayerOnLoan();
	void					InitialiseSpecialPlayers();
    ushort					IsInTeam(const ushort ID);
    bool                    IsPlayerOkToLeave(CPlayer* pPlayer);
    bool                    IsPlayerInSquad(const ushort PlayerNum);
    bool                    IsPlayerInSquad(CPlayer* pPlayer);
    uint					MaximumPlayersInPosition(const ePlayerPosition Posn) const;
	byte					DoCalculateMaximumPlayerSkill(CClub* _Club);
	byte					DoCalculateMinimumPlayerSkill(CClub* _Club);
    uint					MinimumAvailablePlayersInPosition(const ePlayerPosition Posn) const;
	void 					DoNewContractRequests();
	void					DoNewSeason();
    ushort					NextInjuredPlayer(const ushort CurrentPlayer);
    ushort					NextPlayerNumber(const ushort CurrPlayer);
	byte					DoCountGoodFlairDefendersPlaying();
	byte					DoCountGoodFlairMidfieldersPlaying();
	byte					DoCountGoodFlairAttackersPlaying();
    void                    OffListAllPlayersInPosition(ePlayerPosition Posn);
    void                    OnOffListPlayers();
	ushort					PenaltyTakerSquadNumber();
	void					DoCheckIfRequestNewContracts(CClub& _Club);
    CPlayer*                DoFindRandomPlayer();
    CPlayer*                DoFindRandomAttackingPlayer();
    CPlayer*                DoFindRandomSquadPlayer();
    ushort					PreviousPlayerNumber(const ushort CurrPlayer);
	ushort					RandomPlayingSquadNumber(const ePlayerPosition Posn = MAXPLAYERPOSITIONS);
	void					PromoteYouth(CContract* Contract, CYouth* pYouth);
	ushort					RandomPlayingPosition(const bool SquadNumber, const ePlayerPosition ePosition);
	ushort					DoRandomPlayerIDByPosition(const ePlayerPosition aePosition);
	CString					RandomPlayingPositionSurName(const ePlayerPosition ePosition);
    CString                 RandomPlayingGoalkeeperSurName();
    CString                 RandomPlayingFoulerSurName();
    CString                 RandomPlayingVictimSurName();
	ushort					DoRankInSquad(CPlayer& _Player);
	ushort					DoRankInPosition(CPlayer& _Player);
	void					DoRemovePlayerIDFromNonUserControlledSquad(const ushort a_nPlayerID);
	void					DoRemoveFromSquad(ushort _PlayerToRemoveSquadNumber);
    void                    DoSackPlayersIfContractExpired(const bool HumanManager, CCountry* pCountry);
    void                    DoSelectSpecialPlayers();
	void 					DoSelectNonUserControlledTeam(const byte _NumSubs, CClub& _Club, const bool _GenerateNewIfShort = true);
    void                    DoSwapPlayers(const ushort playernum, const ushort playernum1);
	void					DoTakeAllPlayersOffTransferList();
    void                    DoUpdateTeamStrengths(const byte goalstren, const byte defstren, const byte midstren, const byte attstren);
	void					DoWeeklyUpdate(CClub& _Club);
	ushort					DoCountFitPlayers();
	CPlayer*				GetFitPlayer(const ushort _SquadNum);
	void					PostInitialise(CClub& _Club);
	void					ReduceSquadSize(const int HowMany);
	ushort					TotalSquadSize();
#ifdef ASSISTS
	ushort					CountPlayersAssistedAtLeast(const uint iMin);
	CPlayer*				GetLeadingAssister(CPlayer* pIgnorePlayer = NULL, const bool bIgnoreUnavailablePlayers = false);
#endif
};
#endif
