

/*

	C++ Classes

	Pocket Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/

#ifndef __CHIST_H__
#define __CHIST_H__



class CDate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRecordMatchDetails
{
private:
	CStringID				m_ClubNameID;
	CDate					m_Date;
	CClubMatchScore<byte>	m_Score;
	ushort					m_strVenueID;
	ushort					m_strCompetitionID;

public:
	CRecordMatchDetails();
	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					CheckIfRecordVictory(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date);
	void					CheckIfRecordDefeat(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date);
	CString					DetailsStr();
	void					SetDetails(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRecordCareerDetails
{
private:
	ushort					m_RecordValue;
	CPersonName				m_RecordName;

public:
	CRecordCareerDetails();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	bool					CheckIfRecord(CPersonName& _Name, const ushort _Value);
	CString					InfoStr();
	CString					ValueStr();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRecordPlayerDetails
{
private:
	CPersonName				m_PlayerName;
	CStringID				m_ClubNameID;
	CDate					m_Date;
	uint					m_Value;

public:
	CRecordPlayerDetails();
 	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	bool					CheckIfRecord(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date);
	CString					DetailsStr(CCountry& rCountry);
	CString					ValueStr(CCountry& rCountry);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRecordDivisionDetails
{	
public:
	CRecordDivisionDetails();
	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoCheckIfHighest(const byte _Division, const ushort _SeasonID);
	void					DoCheckIfLowest(const byte _Division, const ushort _SeasonID);
	virtual CString			DetailsStr(CCountry& rCountry);
	void					DoUpdate(const byte _Division, const ushort _SeasonID);

private:
	byte					m_Division;
	ushort					m_strSeasonID;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class	CRecordPointsDetails : public CRecordDivisionDetails
{
private:
	byte					m_Points;

public:
	CRecordPointsDetails();
	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoCheckIfMost(const byte _Points, const byte _Division, const ushort _SeasonID);
	void					DoCheckIfLeast(const byte _Points, const byte _Division, const ushort _SeasonID);
	CString					DetailsStr(CCountry& rCountry);
	byte					GetPoints() const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class	CRecordPositionDetails : public CRecordDivisionDetails
{
private:
	byte					m_Position;

public:
	CRecordPositionDetails();
	
	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoCheckIfHighest(const byte _Points, const byte _Division, const ushort _SeasonID);
	void					DoCheckIfLowest(const byte _Points, const byte _Division, const ushort _SeasonID);
	CString					DetailsStr(CCountry& _Country);
	byte					GetPosition() const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CClubHistory 
{
 private:
	CRecordPlayerDetails	m_RecordSigning;
    CRecordPlayerDetails	m_RecordSale;
    
	CRecordCareerDetails	m_RecordAppearance;
	CRecordCareerDetails	m_RecordGoals;
	CRecordCareerDetails	m_RecordSeasonGoals;
    
	CRecordAttendanceDetails m_HighestAttendance; 
	CRecordAttendanceDetails m_LowestAttendance; 

	CRecordDivisionDetails	m_LowestDivision;
	CRecordDivisionDetails	m_HighestDivision;
	
    uint					m_RecordReceipts;
	CStringID				m_RecordReceiptsClubID;
	CDate					m_RecordReceiptsDate;

	CRecordPositionDetails	m_HighestPosition;
	CRecordPositionDetails	m_LowestPosition;
	
	CRecordPointsDetails	m_MostPoints;
	CRecordPointsDetails	m_LeastPoints;
	
	CRecordMatchDetails		m_RecordVictory;
	CRecordMatchDetails		m_RecordDefeat;
    
 public:
    CClubHistory();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	bool					CheckIfRecordSign(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date);
	bool					CheckIfRecordSale(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date);
	bool					CheckIfRecordAppear(CPersonName& _Name, const ushort _Value);
	bool					CheckIfRecordGoals(CPersonName& _Name, const ushort _Value);
	bool					CheckIfRecordSeasonGoals(CPersonName& _Name, const ushort _Value);
	void					CheckIfRecordAttend(int Attendance, const CStringID& ClubNameID, CDate Date);
    void                    CheckIfRecordReceipt(const uint Receipt, const CStringID& ClubNameID, CDate Date);
	CString					RecordAttendanceStr();
	CString					LowestAttendanceStr();
	CString					RecordAttendanceInfoStr();
	CString					LowestAttendanceInfoStr();
	CString					RecordSaleStr(CCountry& rCountry);
	CString					RecordSaleInfoStr(CCountry& rCountry);
	CString					RecordBuyStr(CCountry& _Country);
	CString					RecordBuyInfoStr(CCountry& rCountry);
	CString					RecordAppearanceInfoStr();
	CString					RecordGoalsInfoStr();
	CString					RecordSeasonGoalsInfoStr();
	CRecordMatchDetails*	GetRecordVictory();
	CRecordMatchDetails*	GetRecordDefeat();
	CRecordPointsDetails*	GetMostPoints();
	CRecordPointsDetails*	GetLeastPoints();
	CRecordPositionDetails* GetHighestPosition();
	CRecordPositionDetails* GetLowestPosition();
	CRecordDivisionDetails* GetLowestDivision();
	CRecordDivisionDetails* GetHighestDivision();
};
#endif
