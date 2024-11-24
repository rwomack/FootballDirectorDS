
/*

	File:- DivisionStrength.h

	Division Strength class


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------------------------------
*/


 #ifndef __DIVISIONSTRENGTH_H__
 #define __DIVISIONSTRENGTH_H__

class CDivisionStrength
{
    // Variables
private:
    byte					m_ManagerPointsWin;           // Points awarded to manager of club for a league win
    byte					m_ManagerPointsDraw;
    byte					m_ManagerPointsMom;
    byte					m_ManagerPointsGoodWinAway;   // extra if win away against club n places higher
    byte					m_ManagerPointsGoodWinHome;   // extra if win home against club n places higher
    byte					m_ManagerPointsTopWin;        // extra if win away against club n places higher*/
    byte					m_TicketAttendanceRatio[50];
	uint					m_SeasonShirtSponsorShipAmount;
	uint					m_SeasonKitSponsorShipAmount;
	uint					m_SeasonTVIncome;
#ifdef MANAGERSTRESS
	byte					m_ManagerInitialStress;
#endif

public:
    // Methods
	uint					DoAdjustAttendanceForTicketPrice(const uint _HowMany, const uint _TicketPrice);
  float         GetTicketAttendanceRatio(const uint _ticketprice);
    byte					GetManagerPointsWin() const;
    byte					GetManagerPointsDraw() const;
	uint					GetSeasonShirtSponsorShipAmount();
	uint					GetSeasonKitSponsorShipAmount();
	uint					GetSeasonTVIncome();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
#ifdef MANAGERSTRESS
	const byte				GetManagerInitialStress() const;
#endif
};
///////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
class CDivisionStrengthList : public CExtArray
{
public:
	// Constructor
    CDivisionStrengthList();
    ~CDivisionStrengthList();

    // Methods
	CDivisionStrength&		GetAt(ushort _ElementNumber);

    // Variables
private:
	CDivisionStrength*		m_pDivisionStrengthList;
};
#endif
