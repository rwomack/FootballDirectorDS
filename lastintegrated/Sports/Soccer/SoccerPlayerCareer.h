#ifndef __PCAREER_H__
#define __PCAREER_H__


/*

    PlayerCareer Class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



class CPlayer;

class 	CPlayerCareer
{
private:
    ushort					m_Appearances;
    ushort					m_Goals;
	byte					m_Bookings;
	byte					m_SendingOffs;
	byte					m_Suspensions;			// number of times been suspended
#ifdef ASSISTS
	ushort					m_Assists;
#endif
#ifdef STOREMANOFMATCH
	byte					m_ManOfMatchAwards;
#endif

public:
	CPlayerCareer()
		: m_Appearances(0)
    	, m_Goals(0)
		, m_Bookings(0)
		, m_SendingOffs(0)
		, m_Suspensions(0)
#ifdef ASSISTS
		, m_Assists(0)
#endif	
#ifdef STOREMANOFMATCH
		, m_ManOfMatchAwards(0)
#endif
	{}

	CPlayerCareer& operator + (CPlayerCareer& p);
	CPlayerCareer& operator += (CPlayerCareer& p);
	CPlayerCareer& operator = (CPlayerCareer& p);

    ushort			GetAppearances() const
	{
		return m_Appearances;
	}
    ushort			GetGoals() const
	{
		return m_Goals;
	}
	byte				GetBookings() const
	{
		return m_Bookings;
	}
	byte				GetSendingOffs() const
	{
		return m_SendingOffs;
	}
	byte				GetSuspensions() const
	{
		return m_Suspensions; 			// number of times been suspended
	}
	void					DoClearSeasonDisciplinaryPoints()
	{
		m_Bookings = 0;
		m_SendingOffs = 0;
	}
	void					DoIncrementGoals()
	{
		m_Goals++;
	}
	void					DoIncrementAppearances()
	{
		m_Appearances++; 
	}
	void					DoIncrementBookings()
	{
		m_Bookings++;
	}
	void					DoIncrementSendingOffs()
	{
		m_SendingOffs++; 
	}

#ifdef ASSISTS
	ushort					GetAssists() const
	{
		return m_Assists;
	}
	void					DoIncrementAssists()
	{
		m_Assists++;
	}
#endif
	void					DoResetVariables();
	void DoGenerateInitialData(const ushort _CareerAppearances, const ePlayerPosition _Position);

};
#endif