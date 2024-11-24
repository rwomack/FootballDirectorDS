// Stand.h: interface for the CStand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STAND_H__966F0F01_85C4_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_STAND_H__966F0F01_85C4_11D1_A3F5_00001C303EC4__INCLUDED_


const int				NUMBEROFSTANDS = 8;
enum 
{
	NOTHING,
		REFURBISH,
		NEWBUILDING,
};

class CStand 
{
public:
	CStand();
	CStand(FILE* File);
	virtual ~CStand();

    // IO Methods
    void					Write(FILE* File);
    void					Read(FILE* File);

    // Methods
	bool					DoEndOfWeek(void);
	int				GetCapacity(void)							{ return m_Capacity;  }
	//uint8				GetPriceBand(void)							{ return m_PriceBand; }
	uint8				GetSize(void)								{ return m_Size; }
	uint8				GetRoof(void)								{ return m_Roof; }
	uint8				GetSeat(void)								{ return m_Seat; }
	uint8				GetBox(void)								{ return m_Box; }
	uint8				GetWeeksToConstruct(void)					{ return m_WeeksToConstruct; }
	int				SetCostToConstruct(void)					{ return m_CostToConstruct; }
	void				SetCapacity(int x)							{ m_Capacity = x;  }
	//void				SetPriceBand(uint8 x)						{ m_PriceBand = x; }
	void				SetSize(uint8 x)							{ m_Size = x; }
	void				SetRoof(uint8 x)							{ m_Roof = x; }
	void				SetSeat(uint8 x)							{ m_Seat = x; }
	void				SetBox(uint8 x)								{ m_Box = x; }

	void				SetWeeksToConstruct(uint8 x)				{ m_WeeksToConstruct = x; }
	void				SetCostToConstruct(int x)					{ m_CostToConstruct = x; }
	uint8				GetNewSize(void)							{ return m_NewSize; }
	uint8				GetNewRoof(void)							{ return m_NewRoof; }
	uint8				GetNewSeat(void)							{ return m_NewSeat; }
	uint8				GetNewBox(void)							{ return m_NewBox; }
	void				SetNewSize(uint8 x)							{ m_NewSize = x; }
	void				SetNewRoof(uint8 x)							{ m_NewRoof = x; }
	void				SetNewSeat(uint8 x)							{ m_NewSeat = x; }
	void				SetNewBox(uint8 x)							{ m_NewBox = x; }

	void				SetBuildingStatus(int x)					{ m_BuildingStatus = x; }
	uint8				GetBuildingStatus(void)					{ return m_BuildingStatus; }


    // Variables
protected:
    int						m_Capacity;
	int						m_CostToConstruct;
	//uint8					m_PriceBand;
	uint8					m_Size;
	uint8					m_Roof;
	uint8					m_Seat;
	uint8					m_Box;
	uint8					m_WeeksToConstruct;
	uint8					m_BuildingStatus;
	uint8					m_NewSize;
	uint8					m_NewRoof;
	uint8					m_NewSeat;
	uint8					m_NewBox;
};


class CStandList : public CPtrArray {
public:
    CStandList();             // Constructor
    ~CStandList();            // Destructor

    // IO Methods
    void					Write(FILE* File);
    void					Read(FILE* File);

    // Methods
    void					Create(FILE* File);
	void					DeleteAll(void);
	bool					DoEndOfWeek(void);
    CStand*					GetAt(int x)                               { return static_cast<CStand*>(CPtrArray::GetAt(x)); }
	void					RemoveList(void);
	int						GetTotalCapacity(void);
	void					SetTotalCapacity(int x);						
	void					RaiseTotalCapacity(int x);
	void					LowerTotalCapacity(int x);
	int						GetCapacity(int WhichStand);
	uint8					BuildingStatus(int WhichStand);
	//int						GetPriceBandCapacity(int x);
	//uint8					GetPriceBand(int x);
	//void					SetPriceBand(int x, uint8 y);

    // Variables
protected:
    int						m_Capacity;
};

#endif // !defined(AFX_STAND_H__966F0F01_85C4_11D1_A3F5_00001C303EC4__INCLUDED_)
