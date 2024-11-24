
 /*

    Stadium class


	Football Director

	(c) Rw Software 1994 - 2008
   
  ---------------------------------------------------------------
 */


 #ifndef __STADIUM_H__
 #define __STADIUM_H__

#ifdef	ACTIVESTADIUMPLANS
#include "Stand.h"
#endif



class CStadium
{
public:
	CStadium () {};
	~CStadium () {};

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	uint					GetCapacity();
	CExtString				GetCapacityStr();
	CString&				GetName();
	void					SetCapacity(const uint _NewCapacity);

#ifdef	ACTIVESTADIUMPLANS
	CStandList&				GetStandPtr();
	bool					DoEndOfWeek();
	uint					GetPlanID() const							
	{ 
		return m_Id; 
	}
	uint					StandCapacity(const ushort WhichStand);
	void					LowerCapacity(const uint x);
	void					RaiseCapacity(const uint x);
	int						StandsUnderRefurbishment();
	CStandList*				pStand();
#endif

    // Variables
private:
	CExtString				m_Name;
#ifdef	ACTIVESTADIUMPLANS
	CStandList				m_StandData;
#else
    uint					m_Capacity;
#endif
 };
///////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
class CStadiumList
{
public:
	CStadiumList() {};		// Constructor
    ~CStadiumList();		// Destructor

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoInitialise();
	ushort					GetSize()				{ return m_HowMany; }
	CStadium*				GetAt(const uint x)			{ EXT_ASSERT(x < m_HowMany); return &m_pList[x]; }

private:
	CStadium*				m_pList;
	ushort					m_HowMany;
};	
#endif
