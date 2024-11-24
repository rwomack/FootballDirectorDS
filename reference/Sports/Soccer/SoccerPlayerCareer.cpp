
/*

    PlayerCareer Class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */

#include "stdafx.h"


 //---------------------------------------------------------------------------------------------------------------------------
CPlayerCareer& CPlayerCareer::operator + (CPlayerCareer& p)
{
	m_Appearances += p.m_Appearances;
	m_Goals += p.m_Goals;
	m_Bookings += p.m_Bookings;
	m_SendingOffs += p.m_SendingOffs;
	m_Suspensions += p.m_Suspensions;
	return *this;
}


 //---------------------------------------------------------------------------------------------------------------------------
CPlayerCareer& CPlayerCareer::operator += (CPlayerCareer& p)
{
	m_Appearances += p.m_Appearances;
	m_Goals += p.m_Goals;
	m_Bookings += p.m_Bookings;
	m_SendingOffs += p.m_SendingOffs;
	m_Suspensions += p.m_Suspensions;
	return *this;
}


 //---------------------------------------------------------------------------------------------------------------------------
CPlayerCareer& CPlayerCareer::operator = (CPlayerCareer& p)
{
	m_Appearances = p.m_Appearances;
	m_Goals = p.m_Goals;
	m_Bookings = p.m_Bookings;
	m_SendingOffs = p.m_SendingOffs;
	m_Suspensions = p.m_Suspensions;
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareer::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerCareer::DoResetVariables()
{
   	m_Appearances = 0;
   	m_Goals = 0;
	m_Bookings = 0;
	m_SendingOffs = 0;
	m_Suspensions = 0;			// number of times been suspended
#ifdef ASSISTS
	m_Assists = 0;
#endif
}

	
/*------------------------------------------------------------------------------
	Method:   	CPlayerCareer::DoGenerateInitialData
	Access:    	public 
	Parameter: 	const ushort _CareerAppearances
	Parameter: 	const ePlayerPosition _Position
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerCareer::DoGenerateInitialData(const ushort _CareerAppearances, const ePlayerPosition _Position)
{
	m_Appearances = _CareerAppearances;
	if (m_Appearances > 4)
	{
		switch(_Position) 
		{
			case GOALKEEPER:
				// Number of goals conceded
				m_Goals = m_Appearances - m_Appearances / 4 + RandomNumber.IntLessThan(m_Appearances) / 4;
   				break;
			case ATTACKER:
				m_Goals = m_Appearances / 6 + RandomNumber.IntLessThan(m_Appearances / 3);
   				break;
			default:
				m_Goals = RandomNumber.IntLessThan(m_Appearances / 6);
				break;
		}
	}
}