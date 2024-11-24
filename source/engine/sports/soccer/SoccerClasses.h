
/*

	SoccerClasses.h : main header file for the SoccerClasses application


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#ifndef __SOCCERCLASSES_H__
#define __SOCCERCLASSES_H__


// copy the required defines into stdafx.h of the project, do NOT uncomment them here
//#define ALLOWSTADIUMBUILDING
//#define ACTIVEFRIENDLIES
//#define USELONGCLUBNAME
//#define ACTIVESCENARIOS
//#define ACTIVEFEDERATIONS
//#define ACTIVEFEDERATIONCUPS
//#define ACTIVEINTERFEDERATIONCUPS
//#define ACTIVEREFEREES
//#define ACTIVESTADIUMPLANS
//#define ASSISTS
//#define ACCOUNTS_PIE_CHARTS
//#define BALL_PITCH_LOCATION
//#define CUPGROUPS
//#define CUP_GROUP3ONCE2REGIONS
//#define CUPGROUPS5
//#define CUPGROUPS6
//#define CUPGROUPS8
//#define CUSTOMFORMATIONS
//#define COUNTRYNAME
//#define DETAILEDMANAGERHISTORY
//#define FEDERATIONTRANSFERS
//#define FULLLEAGUETABLEHISTORY
//#define INTERNATIONAL
//#define INTERCONTINENALCUPS
//#define LATESTSCORES
//#define MANAGEREXPENDITURE
//#define MANAGERTABLES
//#define MOTIVATEPLAYERSINMATCH
//#define MATCHTRACKING
//#define NEWSPAPERON
//#define OFFERUSERMANAGERJOB
//#define PLAYERTRANSFERHISTORY
//#define QUICKRESULTS
//#define PLAYERSORTS
//#define SCENARIOS
//#define SHOWTERRITORY
//#define SPECIAL_CASE_CHARITY_SHIELDS
//#define SOCCERTACTICSARROWS
//#define STOREGAMESEQUENCERECORDS
//#define STOREMANOFMATCH
//#define STOREMATCHINJURIES
//#define STORE_MAN_OF_MATCH
//#define STOREMATCHPOSESSION
//#define STORESUBSTITUTIONSINFO
//#define SURGICALINJURIES
//#define TEAMOFWEEKONMENU
//#define TESTSTATS
//#define TROPHYHISTORY
//#define USECLUBYEARFOUNDED
//#define YOUTHTRAINING

// Debug flags
//#define FORCETRANSFERREQUESTS		// Screen21 (player requests transfer) happens more frequently

#ifdef ACTIVESTADIUMPLANS
	
class CTicketPrice
{
public:
    CTicketPrice() 
	{
		m_dFullPrice = 0;
		m_dConcessionPrice = 0;
	};
	
    CTicketPrice& operator =(const CTicketPrice &p);
	
    // IO Methods
    void					Write(FILE* File);
    void					Read(FILE* File);
	
    // Methods
protected:
  float          m_dFullPrice;
  float          m_dConcessionPrice;
};
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CWorld;
	class CCup;
	class CCupRound;
	class CDivision;
    class CEmployee;
	class CFederationCup;
	class CFederationCupList;
    class CFixture;
    class CFixtureList;
	class CPerson;
    class CPlayer;
    class CTransferNegotiation;
	class CMatchPlay;
	class CManager;
    class CCountry;
	class CSoccerMatchPlayer;
	class CSoccerMatchPlayerList;
	class CCompetitionList;
	class CCurrency;
	class CClub;
	class CPopularity;
	class CPlayerSquad;

	
	extern int				UserMessageBox(const CString& Message, uint Style = MB_OK + MB_ICONINFORMATION);
	extern int				UserMessageBox(uint Message, uint Style = MB_OK + MB_ICONINFORMATION);

	extern CWorld&			WorldData();
	extern void				IncrementProgress();

    extern bool				MangContFlag;

#include "SoccerDefines.h"
#include "StringID.h"
#include "ExtPoint.h"

#include "EventString.h"
#include "AvailableSubstitutes.h"
#include "Stadium.h"

#include "InjuryType.h"
#include "PlayerSquad.h"
#include "PlayerLoan.h"
#include "PlayerRating.h"
#include "ContractEndTime.h"	
#include "Person.h"
#include "SoccerPlayer.h"
#include "PlayerContract.h"
#include "SoccerPlayerCareer.h"
#include "SoccerPlayerCareerPath.h"
#include "ClubTransferOffer.h"
#include "PlayerDiscipline.h"
#include "BaseCompetition.h"
#include "Calendar.h"	
#include "Injury.h"
#include "Player.h"
#include "loanrepay.h"
#include "FinancialLoan.h"
#include "CupTiedPlayer.h"
#include "SoccerSquadTraining.h"
#ifdef PLAYERTRANSFERHISTORY
#include "TransferHistory.h"
#endif
#ifdef TROPHYHISTORY
#include "TrophyHistory.h"
#endif
#include "News.h"
// ######### Tactics Related #########
#include "TeamTactic.h"
#include "SoccerTactic.h"

#include "Employee.h"
#include "Youth.h"
#include "LeagueTableDetails.h"
#include "SeasonLeaguePosition.h"
#include "TableData.h"
#include "ClubFriendlies.h"
#include "RefusedDeals.h"
#include "RecordAttendance.h"
#include "ClubMatchScore.h"
#include "ClubHistory.h"
#include "ClubMatchDetails.h"
#include "ClubSeasonStats.h"
#include "Accounts.h"
#include "ManagerContractOffer.h"
#include "KitSystem.h"
#include "Club.h"
#include "ClubList.h"
#include "MatchEvent.h"
#include "PenaltyShootOut.h"
#include "MatchTeamSkills.h"
#include "MatchEventsStats.h"
#include "MatchCommentary.h"
#include "MatchActionItem.h"
#include "SoccerMatchPlayer.h"
#include "SoccerBallPitchLocation.h"
#include "MatchSubs.h"
#ifdef STORESUBSTITUTIONSINFO
#include "SubstitutionsInfo.h"
#endif
#include "SoccerPlayerPerformance.h"
#include "MatchZonalStats.h"
#include "MatchVarsData.h"
#include "MatchVars.h"
#include "MatchTactics.h"
#include "MatchPlay.h"
#include "Fixture.h"
#include "CupRound.h"
#include "TeamOfTheWeek.h"
#include "ManagerOfTheMonth.h"
#include "DivisionPlayOffInfo.h"
#ifdef ACTIVEFEDERATIONCUPS
#include "DivisionFederationCupData.h"
#endif
#include "Division.h"
#include "Cup.h"
#include "Playoff.h"
#include "CharityShield.h"
#ifdef ACTIVEFEDERATIONCUPS
#include "FederationCup.h"
#endif
#include "SoccerResult.h"
#include "ContractClause.h"
#include "OutOfContractPlayer.h"
#include "CupWinnersInfo.h"
#include "ActiveCountry.h"
#include "Country.h"
#include "Currency.h"
#include "Rating.h"
#include "ManagerHistory.h"
#include "Confidence.h"
#include "Manager.h"
#include "Chairman.h"
#include "HumanManager.h"
#include "Aggression.h"
#include "MatchHandler.h"
#include "FixtureController.h"
#include "SkillMoneyValues.h"
#include "DivisionStrength.h"
#include "Popular.h"
#include "Federation.h"
#include "TransferDeadlineSystem.h"
#include "WorldData.h"
	
#ifdef	ACTIVEREFEREES
//#include "SoccerReferee.h"
#endif
#ifdef	ACTIVESTADIUMPLANS
//#include "Stand.h"
//#include "StadiumPlan.h"
#endif

#endif

