

/*

	Club Match Details class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __CLUBMATCHDETAILS_H__
#define __CLUBMATCHDETAILS_H__

class CClubMatchDetails
{
private:
	int					m_Scored:4;
	int					m_Conceded:4;
    int					m_Venue:2;
    int					m_Attendance:17;
    uint					m_GateReceipts;

public:
	CClubMatchDetails();

	uint					DoCalculateSecurityCosts();
	byte				GetScored();
	byte				GetConceded();
	int					GetVenue() const;
	uint			GetAttendance() const;
	uint			GetGateReceipts() const;
	void					OnInitialise();
	void DoUpdate(CFixture& _Fixture, const eWhichTeam _Venue, const byte _AwayGatePercentage, const byte _FAGatePercentage);
	void					SetVenue(const int _Venue);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif