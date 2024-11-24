
/*

    Chairman class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


 #ifndef __CHAIRMAN_H__
 #define __CHAIRMAN_H__


class CChairman
{
public:
    CChairman();

	// Methods
	void					DoResetVariables();
	void					DoManagerSacked();
    void					DoNewSeason(CDivision& aDivision, CClub &cClub);
    void					EndSeason();
//    void					DoMonthlyReport(CDivision& aDivision, ushort iClubID);
	void					PreMatchReport();
	void					AfterMatchReport();

private:
	byte					m_SeasonTargetPosition;
	byte					m_TargetPointsForMonth;
	byte					m_PointsThisMonth;
};
#endif
