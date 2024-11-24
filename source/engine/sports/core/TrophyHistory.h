
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
	ClubID					m_club;
	byte					m_Type;
	byte					m_Gfx;

public:
	CTrophyHistory(const CString& Title, ClubID cClub, const CString& Season, const byte Gfx, const byte Type);
	CTrophyHistory() {};
	~CTrophyHistory();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	byte					GetGfx() const;
	CExtString				GetSeason() const;
	CExtString				GetTitle() const;
	byte					GetType() const;
	CString&				GetSuppInfo() const;
	bool					IsDivision();
	bool					IsWinner();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTrophyHistoryList : public CPointerArray<CTrophyHistory>
{
public:
    CTrophyHistoryList() {};             // Constructor
    ~CTrophyHistoryList();            // Destructor

    // IO Methods
	void  Write(CDataFile* _pDataFile);
	void  Read(CDataFile* _pDataFile);

    // Methods
	void					AddRunnersUp(const CString& Title, ClubID cClub, const CString& Season, const byte Gfx, bool IsCup);
	void					AddChampions(const CString& Title, ClubID cClub, const CString& Season, const byte Gfx, bool IsCup);
};
#endif // !defined(AFX_TROPHYHISTORY_H__C447B821_7172_11D1_A3F5_00001C303EC4__INCLUDED_)
