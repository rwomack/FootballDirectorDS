

 /*

	File:- MatchEvent.h

    Match Event class

	Information about the match event, is stored within CFixture

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __MATCHEVENT_H__
#define __MATCHEVENT_H__

enum eMatchEvent
{
	// NOTE: If changes are made to this enumeration, make sure to update
	// CMatchActionItem::TeamInPosession() accordingly.
	NOEVENT = 0,
	HOMEGOAL, AWAYGOAL,                                 // 3
	HOMEBOOKING, AWAYBOOKING,                           // 5
	HOMESENDINGOFF, AWAYSENDINGOFF,                     // 7
	HOMEOWNGOAL, AWAYOWNGOAL,                           // 9
	HOMEPENALTYGOAL, AWAYPENALTYGOAL,                   // 11

	// Penalty shoot out
	HOMEPENALTYSHOOTOUTGOAL, AWAYPENALTYSHOOTOUTGOAL,	// 13

	HOMEOFFSIDEATTEMPT, AWAYOFFSIDEATTEMPT,             // 15
	HOMEFOULPLAYERATTEMPT, AWAYFOULPLAYERATTEMPT,       // 17
	HOMEINJUREPLAYERATTEMPT, AWAYINJUREPLAYERATTEMPT,   // 19
	HOMESHOTATTEMPT, AWAYSHOTATTEMPT,                   // 21
	HOMEMISSATTEMPT, AWAYMISSATTEMPT,                   // 23
	HOMEFREEKICKATTEMPT, AWAYFREEKICKATTEMPT,           // 25
	HOMECORNERATTEMPT, AWAYCORNERATTEMPT,               // 27
	HOMESUBATTEMPT, AWAYSUBATTEMPT,                     // 29
	// Sub events
	HOMEFREEKICKINDIRECT, AWAYFREEKICKINDIRECT,
	HOMEFREEKICKDEFLECTED, AWAYFREEKICKDEFLECTED,
	HOMEFREEKICKCLEARED, AWAYFREEKICKCLEARED,
	HOMEFREEKICKOFFTARGET, AWAYFREEKICKOFFTARGET,
	HOMEFREEKICKONTARGET, AWAYFREEKICKONTARGET,
	HOMEFREEKICKGOAL, AWAYFREEKICKGOAL,					// 41

	HOMEFREEKICKINDIRECTYELLOWGOALKEEPER, AWAYFREEKICKINDIRECTYELLOWGOALKEEPER,			// 43
	HOMEFREEKICKDEFLECTEDYELLOWGOALKEEPER, AWAYFREEKICKDEFLECTEDYELLOWGOALKEEPER,
	HOMEFREEKICKCLEAREDYELLOWGOALKEEPER, AWAYFREEKICKCLEAREDYELLOWGOALKEEPER,
	HOMEFREEKICKOFFTARGETYELLOWGOALKEEPER, AWAYFREEKICKOFFTARGETYELLOWGOALKEEPER,
	HOMEFREEKICKONTARGETYELLOWGOALKEEPER, AWAYFREEKICKONTARGETYELLOWGOALKEEPER,
	HOMEFREEKICKGOALYELLOWGOALKEEPER, AWAYFREEKICKGOALYELLOWGOALKEEPER,					// 53

	HOMEFREEKICKINDIRECTYELLOWOUTFIELDPLAYER, AWAYFREEKICKINDIRECTYELLOWOUTFIELDPLAYER,
	HOMEFREEKICKDEFLECTEDYELLOWOUTFIELDPLAYER, AWAYFREEKICKDEFLECTEDYELLOWOUTFIELDPLAYER,
	HOMEFREEKICKCLEAREDYELLOWOUTFIELDPLAYER, AWAYFREEKICKCLEAREDYELLOWOUTFIELDPLAYER,
	HOMEFREEKICKOFFTARGETYELLOWOUTFIELDPLAYER, AWAYFREEKICKOFFTARGETYELLOWOUTFIELDPLAYER,
	HOMEFREEKICKONTARGETYELLOWOUTFIELDPLAYER, AWAYFREEKICKONTARGETYELLOWOUTFIELDPLAYER,
	HOMEFREEKICKGOALYELLOWOUTFIELDPLAYER, AWAYFREEKICKGOALYELLOWOUTFIELDPLAYER,			// 65

	HOMEFREEKICKINDIRECTREDGOALKEEPER, AWAYFREEKICKINDIRECTREDGOALKEEPER,
	HOMEFREEKICKDEFLECTEDREDGOALKEEPER, AWAYFREEKICKDEFLECTEDREDGOALKEEPER,
	HOMEFREEKICKCLEAREDREDGOALKEEPER, AWAYFREEKICKCLEAREDREDGOALKEEPER,
	HOMEFREEKICKOFFTARGETREDGOALKEEPER, AWAYFREEKICKOFFTARGETREDGOALKEEPER,
	HOMEFREEKICKONTARGETREDGOALKEEPER, AWAYFREEKICKONTARGETREDGOALKEEPER,
	HOMEFREEKICKGOALREDGOALKEEPER, AWAYFREEKICKGOALREDGOALKEEPER,						// 77

	HOMEFREEKICKINDIRECTREDOUTFIELDPLAYER, AWAYFREEKICKINDIRECTREDOUTFIELDPLAYER,
	HOMEFREEKICKDEFLECTEDREDOUTFIELDPLAYER, AWAYFREEKICKDEFLECTEDREDOUTFIELDPLAYER,
	HOMEFREEKICKCLEAREDREDOUTFIELDPLAYER, AWAYFREEKICKCLEAREDREDOUTFIELDPLAYER,
	HOMEFREEKICKOFFTARGETREDOUTFIELDPLAYER, AWAYFREEKICKOFFTARGETREDOUTFIELDPLAYER,
	HOMEFREEKICKONTARGETREDOUTFIELDPLAYER, AWAYFREEKICKONTARGETREDOUTFIELDPLAYER,
	HOMEFREEKICKGOALREDOUTFIELDPLAYER, AWAYFREEKICKGOALREDOUTFIELDPLAYER,				// 89

	HOMETOPCORNERDEFLECTED, AWAYTOPCORNERDEFLECTED,
	HOMEBOTTOMCORNERDEFLECTED, AWAYBOTTOMCORNERDEFLECTED,								// 93
	HOMETOPCORNERCLEARED, AWAYTOPCORNERCLEARED,
	HOMEBOTTOMCORNERCLEARED, AWAYBOTTOMCORNERCLEARED,
	HOMETOPCORNEROFFTARGET, AWAYTOPCORNEROFFTARGET,
	HOMEBOTTOMCORNEROFFTARGET, AWAYBOTTOMCORNEROFFTARGET,								// 101
	HOMETOPCORNERONTARGET, AWAYTOPCORNERONTARGET,
	HOMEBOTTOMCORNERONTARGET, AWAYBOTTOMCORNERONTARGET,									// 105
	HOMETOPCORNERGOALSHOT, AWAYTOPCORNERGOALSHOT,
	HOMETOPCORNERGOALHEADER, AWAYTOPCORNERGOALHEADER,									// 109
	HOMEBOTTOMCORNERGOALSHOT, AWAYBOTTOMCORNERGOALSHOT,
	HOMEBOTTOMCORNERGOALHEADER, AWAYBOTTOMCORNERGOALHEADER,								// 111
	// Player walking off, sub walking on
	HOMEGOALKEEPERSUBSTITUTED, AWAYGOALKEEPERSUBSTITUTED,								// 113
	HOMEDEFENDERSUBSTITUTED, AWAYDEFENDERSUBSTITUTED,									// 115
	HOMEMIDFIELDERSUBSTITUTED, AWAYMIDFIELDERSUBSTITUTED,								// 117
	HOMEATTACKERSUBSTITUTED, AWAYATTACKERSUBSTITUTED,									// 119
	HOMEGOALSHOT, AWAYGOALSHOT,															// 121
	HOMEGOALHEADER, AWAYGOALHEADER,														// 123
	// Penalties
	HOMEPENALTYSAVED, AWAYPENALTYSAVED,
	HOMEPENALTYMISS, AWAYPENALTYMISS,
	HOMEPENALTYSAVEDYELLOWGOALKEEPER, AWAYPENALTYSAVEDYELLOWGOALKEEPER,
	HOMEPENALTYMISSYELLOWGOALKEEPER, AWAYPENALTYMISSYELLOWGOALKEEPER,
	HOMEPENALTYGOALYELLOWGOALKEEPER, AWAYPENALTYGOALYELLOWGOALKEEPER,
	HOMEPENALTYSAVEDREDGOALKEEPER, AWAYPENALTYSAVEDREDGOALKEEPER,						// 135
	HOMEPENALTYMISSREDGOALKEEPER, AWAYPENALTYMISSREDGOALKEEPER,
	HOMEPENALTYGOALREDGOALKEEPER, AWAYPENALTYGOALREDGOALKEEPER,
	HOMEPENALTYSAVEDYELLOWOUTFIELDPLAYER, AWAYPENALTYSAVEDYELLOWOUTFIELDPLAYER,
	HOMEPENALTYMISSYELLOWOUTFIELDPLAYER, AWAYPENALTYMISSYELLOWOUTFIELDPLAYER,
	HOMEPENALTYGOALYELLOWOUTFIELDPLAYER, AWAYPENALTYGOALYELLOWOUTFIELDPLAYER,
	HOMEPENALTYSAVEDREDOUTFIELDPLAYER, AWAYPENALTYSAVEDREDOUTFIELDPLAYER,
	HOMEPENALTYMISSREDOUTFIELDPLAYER, AWAYPENALTYMISSREDOUTFIELDPLAYER,
	HOMEPENALTYGOALREDOUTFIELDPLAYER, AWAYPENALTYGOALREDOUTFIELDPLAYER,					// 151

	HOMESHOTATTEMPTSHOT, AWAYSHOTATTEMPTSHOT,
	HOMESHOTATTEMPTHEADER, AWAYSHOTATTEMPTHEADER,
	HOMEMISSATTEMPTSHOT, AWAYMISSATTEMPTSHOT,
	HOMEMISSATTEMPTHEADER, AWAYMISSATTEMPTHEADER,
	HOMEDISALLOWEDSHOT, AWAYDISALLOWEDSHOT,
	HOMEDISALLOWEDHEADER, AWAYDISALLOWEDHEADER,											// 163
	// Penalty shoot out
	// NOTE: FDL-ELN: moved goal events to occur earlier in the enum, so the range of bits that need to be saved is smaller.
	HOMEPENALTYSHOOTOUTSAVED, AWAYPENALTYSHOOTOUTSAVED,
	HOMEPENALTYSHOOTOUTMISS, AWAYPENALTYSHOOTOUTMISS,									// 167
	// Injuries
	HOMEGOALKEEPERHOBBLE, HOMEGOALKEEPERCARRIEDOFF,										// 169
	HOMEOUTFIELDPLAYERHOBBLE, HOMEOUTFIELDPLAYERCARRIEDOFF,
	AWAYGOALKEEPERHOBBLE, AWAYGOALKEEPERCARRIEDOFF,
	AWAYOUTFIELDPLAYERHOBBLE, AWAYOUTFIELDPLAYERCARRIEDOFF,
	HOMEGOALKEEPERINJURYOK, HOMEOUTFIELDPLAYERINJURYOK,
	AWAYGOALKEEPERINJURYOK, AWAYOUTFIELDPLAYERINJURYOK,									// 179
	// Injured, take off and show sub coming on
	HOMEGOALKEEPERHOBBLESUBON, HOMEGOALKEEPERCARRIEDOFFSUBON,							// 181
	HOMEOUTFIELDPLAYERHOBBLESUBON, HOMEOUTFIELDPLAYERCARRIEDOFFSUBON,
	AWAYGOALKEEPERHOBBLESUBON, AWAYGOALKEEPERCARRIEDOFFSUBON,
	AWAYOUTFIELDPLAYERHOBBLESUBON, AWAYOUTFIELDPLAYERCARRIEDOFFSUBON,
	// Sub walking on
	HOMEGOALKEEPERSUBON, AWAYGOALKEEPERSUBON,
	HOMEDEFENDERSUBON, AWAYDEFENDERSUBON,
	HOMEMIDFIELDERSUBON, AWAYMIDFIELDERSUBON,
	HOMEATTACKERSUBON, AWAYATTACKERSUBON,
	// Injured, take off and don't show sub coming on(all subs used)
	HOMEGOALKEEPERHOBBLENOSUBON, HOMEGOALKEEPERCARRIEDOFFNOSUBON,
	HOMEOUTFIELDPLAYERHOBBLENOSUBON, HOMEOUTFIELDPLAYERCARRIEDOFFNOSUBON,
	AWAYGOALKEEPERHOBBLENOSUBON, AWAYGOALKEEPERCARRIEDOFFNOSUBON,
	AWAYOUTFIELDPLAYERHOBBLENOSUBON, AWAYOUTFIELDPLAYERCARRIEDOFFNOSUBON
};


class CMatchEvent
{
	DECLARE_FDMEMORY_BINALLOC(CMatchEvent);
    // Variables
private:
    byte					m_Time;
    byte					m_Type;
    CPersonName             m_PlayerName;

public:
	// Constructors
    CMatchEvent() {};
    CMatchEvent(const byte _Time, const byte _Type, const CPersonName _Name);

    // Methods
	void					DoResetVariables();
	eMatchEvent				GetEventType() const;
	CString					GetText();
	CString					GetEventDiscTimeStr();
	CString					GetEventGoalTimeStr();
	byte					GetTime() const;
	bool					IsBookingEvent();
	bool					IsDiscEvent();
	bool					IsGoalEvent();
	bool					IsHomeTeamEvent();
	bool					IsOwnGoal();
	bool					IsPenalty();
	bool					IsPenaltyShootOutEvent();
	bool					IsSendingOffEvent();

	bool					ShouldBeSaved() const;
	const CPersonName &		GetName() const;

	void SetTime(const byte bTime);
	void SetEventType(const byte bEventType);
	void SetName(const ushort usForename, const ushort usSurname);
};
///////////////////////////////////////////////////////////////////////////////////////////////


class CMatchEventList : public CPointerArray<CMatchEvent>
{
public:
	CMatchEventList() : m_bIsSummary(false), m_bSummaryPenaltyShootout(false), m_SummaryAwayGoals(0), m_SummaryHomeGoals(0), m_SummaryHomePenaltyShootoutGoals(0), m_SummaryAwayPenaltyShootoutGoals(0) {};          // Constructor
	~CMatchEventList();

	// IO Methods
	void					WriteTime(CDataFile* _pDataFile);
	void					WriteType(CDataFile* _pDataFile);
	void					WriteName(CDataFile* _pDataFile);
	void					ReadTime(CDataFile* _pDataFile);
	void					ReadType(CDataFile* _pDataFile);
	void					ReadName(CDataFile* _pDataFile);

    // Methods
	uint					AwayScore(const uint _Time) const;
    int                     AwayNumberPlayersBooked() const;
    int                     AwayNumberPlayersSentOff() const;
	CString					AwayPlayerScoredStr(const ushort _EventNumber) const;
	CString					AwayPlayerBookedStr(const ushort _EventNumber) const;
	CString					AwayPlayerSentOffStr(const ushort _EventNumber) const;
	int						DoCalculateGoalDifference() const;
	int						DoCountEvents(eMatchEvent _EventType) const;
	int						DoCountInGameEvents() const;
    CString		            GetLastEventStr() const;
	CString					GetEventStr(const ushort _EventNumber) const;
    CString		            GetLastEventTimeStr() const;
	bool					HasGoalBeenScored(const uint _Time) const;
    int                     HomeNumberPlayersBooked() const;
	uint					HomeScore(const uint _Time) const;
	uint					HalfTimeAwayGoals() const;
	uint					HalfTimeHomeGoals() const;
    int                     HomeNumberPlayersSentOff() const;
	CString					HomePlayerScoredStr(const ushort _EventNumber) const;
	CString					HomePlayerBookedStr(const ushort _EventNumber) const;
	CString					HomePlayerSentOffStr(const ushort _EventNumber) const;
    int                     NumberHomeOwnGoals() const;
    int                     NumberAwayOwnGoals() const;
    int                     NumberHomePenaltyGoals() const;
    int                     NumberAwayPenaltyGoals() const;
	int						NumberAwayPenaltyShootOutGoals() const;
	int						NumberHomePenaltyShootOutGoals() const;
	CMatchEvent*			GetFreeMatchEvent();
	eWhichTeam				PenaltyShootoutWinner() const;
	CString					PenaltyShootoutWinnerStr(const CString _HomeClubName, const CString _AwayClubName) const;
    void                    SortByType();
    void                    SortByTime();
	bool					WasPenaltyShootOut() const;
	//bool					IsAwayEvent(const ushort _EventNumber);
	//bool					IsHomeEvent(const ushort _EventNumber);

  void DoResetVariables();

	void RemovePenaltyShootOutEvents();

	// Converts the event list into a summary, which dispenses with the actual stored
	// events.
	void ConvertToSummary();
	bool IsSummary() const;

private:
	byte m_SummaryHomeGoals : 5;
	byte m_SummaryAwayGoals : 5;
	byte m_SummaryHomePenaltyShootoutGoals : 4;
	byte m_SummaryAwayPenaltyShootoutGoals : 4;
	bool m_bSummaryPenaltyShootout : 1;
	bool m_bIsSummary : 1;
};
#endif
