
/*
	TrophyHistory.h: interface for the CTrophyHistory class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#if !defined(AFX_TROPHYHISTORY_H__C447B821_7172_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_TROPHYHISTORY_H__C447B821_7172_11D1_A3F5_00001C303EC4__INCLUDED_


enum 
{
	DIVISIONWINNER,
    DIVISIONRUNNERUP,
    CUPWINNER,
    CUPRUNNERUP,
	MANAGEROFMONTH
};

class CTrophyHistory 
{
	// Variables
private:
	CExtString				m_Title;
	CExtString				m_Season;
	CExtString				m_SuppInfo;		// Manager will store club name
	byte					m_Type;
	byte					m_Gfx;

public:
	CTrophyHistory(CString Title, CString SuppInfo, CString Season, const byte Gfx, const byte Type);
	CTrophyHistory() {};
	virtual ~CTrophyHistory();

    // IO Methods
	void  Write(CDataFile* _pDataFile);
	void  Read(CDataFile* _pDataFile);

    // Methods
	const byte		GetType() const					
	{ 
		return m_Type; 
	}
	const byte		GetGfx() const						
	{ 
		return m_Gfx; 
	}
	const CString	GetTitle()							
	{ 
		return m_Title; 
	}
	const CString	GetSeason()						
	{ 
		return m_Season; 
	}
	const CString	GetSuppInfo()						
	{ 
		return m_SuppInfo; 
	}
	bool					IsDivision();
	bool					IsWinner();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTrophyHistoryList : public CPointerArray 
{
public:
    CTrophyHistoryList() {};             // Constructor
    ~CTrophyHistoryList();            // Destructor

    // IO Methods
	void  Write(CDataFile* _pDataFile);
	void  Read(CDataFile* _pDataFile);

    // Methods
	void					AddRunnersUp(CString Title, CString SuppInfo, CString Season, const byte Gfx, bool IsCup);
	void					AddChampions(CString Title, CString SuppInfo, CString Season, const byte Gfx, bool IsCup);
};
#endif // !defined(AFX_TROPHYHISTORY_H__C447B821_7172_11D1_A3F5_00001C303EC4__INCLUDED_)
