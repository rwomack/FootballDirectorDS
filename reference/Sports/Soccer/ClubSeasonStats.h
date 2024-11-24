

/*

	Class - C++ Classes

	Club class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __CLUBSEASONSTATS_H__
#define __CLUBSEASONSTATS_H__
	
class CClubSeasonStats
{
public:
	CClubSeasonStats();

    ushort					GetSeasonDisciplinaryPoints();
    ushort					GetSeasonBookings() const;					
    ushort					GetSeasonSentOff() const; 					
    uint					GetTotalAttend() const; 						
    uint					GetBestAttendance() const;
    ushort					GetHomeLeagueAndCupGamesPlayed() const;
    void					SetBestAttendance(const uint Num);

    uint					GetAverageAttendance();
	CExtString				GetAverageAttendanceStr();

    void                    DoAddTotalAttend(const uint x);
	void					DoAddSeasonBookings(const ushort _Booked);
    void                    DoAddSeasonSendingOffs(const ushort aSentOff);
	void					DoNewSeason();

protected:
    uint					m_TotalAttendanceThisSeason;    // Total Attendance for league games
    uint					m_BestAttendanceThisSeason;
    byte					m_NumberPlayersBookedThisSeason;
    byte					m_NumberPlayersSentOffThisSeason;
    byte					m_HomeLeagueAndCupGamesPlayed;
};
#endif