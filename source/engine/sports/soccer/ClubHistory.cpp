
 /*

        Class:- CClubHistory


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "ClubHistory.h"


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::CRecordMatchDetails
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordMatchDetails::CRecordMatchDetails()
{
	m_Score.DoInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::CheckIfRecordVictory
	Access:    	public
	Parameter: 	const byte Score
	Parameter: 	const byte Concede
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const ushort strVenueID
	Parameter: 	const ushort strCompetitionTitleID
	Parameter: 	CDate Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordMatchDetails::CheckIfRecordVictory(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date)
{
	if ((_Score - _Concede) > (m_Score.GetScored() - m_Score.GetConceded()))
	{
		SetDetails(_Score, _Concede, _ClubNameID, _strVenueID, _strCompetitionTitleID, _Date);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::CheckIfRecordDefeat
	Access:    	public
	Parameter: 	const byte Score
	Parameter: 	const byte Concede
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const ushort strVenueID
	Parameter: 	const ushort strCompetitionTitleID
	Parameter: 	CDate Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordMatchDetails::CheckIfRecordDefeat(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date)
{
	if ((_Concede - _Score) > (m_Score.GetConceded() - m_Score.GetScored()))
	{
		SetDetails(_Score, _Concede, _ClubNameID, _strVenueID, _strCompetitionTitleID, _Date);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::SetDetails
	Access:    	public
	Parameter: 	const byte Score
	Parameter: 	const byte Concede
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const ushort strVenueID
	Parameter: 	const ushort strCompetitionTitleID
	Parameter: 	CDate Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordMatchDetails::SetDetails(const byte _Score, const byte _Concede, const CStringID& _ClubNameID, const ushort _strVenueID, const ushort _strCompetitionTitleID, CDate _Date)
{
	m_Score.DoInitialise(_Score, _Concede);
	m_ClubNameID = _ClubNameID;
	m_strVenueID = _strVenueID;
	m_strCompetitionID = _strCompetitionTitleID;
	m_Date = _Date;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::DetailsStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordMatchDetails::DetailsStr()
{
	CString str(TEXT("-"));
	if (m_Score.GetScored() > 0 || m_Score.GetConceded() > 0)
	{
#ifdef NDS
		str.Format(TEXT("%d - %d  %s (%s)"), m_Score.GetScored(), m_Score.GetConceded(), m_ClubNameID.GetStr(), m_Date.ShortDateStr());
#else
		str.Format(TEXT("%d - %d  %s (%s)"), m_Score.GetScored(), m_Score.GetConceded(), m_ClubNameID.GetStr(), m_Date.ShortNumDateStr());
#endif
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordMatchDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_ClubNameID, sizeof(m_ClubNameID));
  	_pDataFile->Read(&m_Date, sizeof(m_Date));
  	_pDataFile->Read(&m_Score, sizeof(m_Score));
  	_pDataFile->Read(&m_strVenueID, sizeof(m_strVenueID));
  	_pDataFile->Read(&m_strCompetitionID, sizeof(m_strCompetitionID));
}


/*------------------------------------------------------------------------------
	Method:   	CRecordMatchDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordMatchDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_ClubNameID, sizeof(m_ClubNameID));
  	_pDataFile->Write(&m_Date, sizeof(m_Date));
  	_pDataFile->Write(&m_Score, sizeof(m_Score));
  	_pDataFile->Write(&m_strVenueID, sizeof(m_strVenueID));
  	_pDataFile->Write(&m_strCompetitionID, sizeof(m_strCompetitionID));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::CRecordPlayerDetails
	Access:    	public
	Returns:
	Qualifier: 	: m_dValue(0)
	Purpose:
------------------------------------------------------------------------------*/
CRecordPlayerDetails::CRecordPlayerDetails()
	: m_Value(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::CheckIfRecord
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const uint Cost
	Parameter: 	CDate Date
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CRecordPlayerDetails::CheckIfRecord(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date)
{
	bool bIsRecord = false;
    if (_Cost > m_Value)
	{
		if (m_Value > 0)
		{
			bIsRecord = true;
		}
        m_PlayerName = _Name;
        m_ClubNameID = _ClubNameID;
        m_Value = _Cost;
        m_Date = _Date;
    }
	return bIsRecord;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::DetailsStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordPlayerDetails::DetailsStr(CCountry& rCountry)
{
	CString str(TEXT("-"));
	if (m_Value > 0)
	{
    //str.Format(TEXT("%s %s - %s (%s)"), rCountry.GetCurrency().GetForeignStr(m_Value), m_PlayerName.GetName(), m_ClubNameID.GetStr(), m_Date.MediumDateStr());
    str.Format(TEXT("%s - %s (%s)"), rCountry.GetCurrency().GetForeignStr(m_Value), m_PlayerName.GetName(), m_Date.ShortDateStr());
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::ValueStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordPlayerDetails::ValueStr(CCountry& rCountry)
{
	CString str(TEXT("-"));
	if (m_Value > 0)
	{
		str.Format(TEXT("%s"), rCountry.GetCurrency().GetForeignStr(m_Value));
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPlayerDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_PlayerName, sizeof(m_PlayerName));
  	_pDataFile->Read(&m_ClubNameID, sizeof(m_ClubNameID));
  	_pDataFile->Read(&m_Date, sizeof(m_Date));
  	_pDataFile->Read(&m_Value, sizeof(m_Value));
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPlayerDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPlayerDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_PlayerName, sizeof(m_PlayerName));
  	_pDataFile->Write(&m_ClubNameID, sizeof(m_ClubNameID));
  	_pDataFile->Write(&m_Date, sizeof(m_Date));
  	_pDataFile->Write(&m_Value, sizeof(m_Value));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CRecordCareerDetails::CRecordCareerDetails
	Access:    	public
	Returns:
	Qualifier: 	: m_RecordValue(0)
	Purpose:
------------------------------------------------------------------------------*/
CRecordCareerDetails::CRecordCareerDetails()
	: m_RecordValue(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CRecordCareerDetails::InfoStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CRecordCareerDetails::InfoStr()
{
	CString str(TEXT("-"));
	if (m_RecordValue > 0)
	{
		str.Format(TEXT("%d, %s"), m_RecordValue, m_RecordName.GetName());
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordCareerDetails::CheckIfRecord
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	ushort Value
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CRecordCareerDetails::CheckIfRecord(CPersonName& _Name, const ushort _Value)
{
    if (_Value > m_RecordValue)
	{
        m_RecordName = _Name;
        m_RecordValue = _Value;
		return true;
    }
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordCareerDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordCareerDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_RecordValue, sizeof(m_RecordValue));
  	_pDataFile->Read(&m_RecordName, sizeof(m_RecordName));
}


/*------------------------------------------------------------------------------
	Method:   	CRecordCareerDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordCareerDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_RecordValue, sizeof(m_RecordValue));
  	_pDataFile->Write(&m_RecordName, sizeof(m_RecordName));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::CRecordDivisionDetails
	Access:    	public
	Returns:
	Qualifier: 	: m_Division(bNOTFOUND) , m_strSeasonID(wNOTFOUND)
	Purpose:
------------------------------------------------------------------------------*/
CRecordDivisionDetails::CRecordDivisionDetails()
	: m_Division(bNOTFOUND)
	, m_strSeasonID(wNOTFOUND)
{
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::CheckIfHighest
	Access:    	public
	Parameter: 	const byte a_Division
	Parameter: 	const ushort a_SeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordDivisionDetails::DoCheckIfHighest(const byte _Division, const ushort _SeasonID)
{
    if (m_Division > _Division || m_Division == bNOTFOUND)
	{
        DoUpdate(_Division, _SeasonID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::CheckIfLowest
	Access:    	public
	Parameter: 	const byte a_Division
	Parameter: 	const ushort a_SeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordDivisionDetails::DoCheckIfLowest(const byte _Division, const ushort _SeasonID)
{
    if (m_Division < _Division || m_Division == bNOTFOUND)
	{
        DoUpdate(_Division, _SeasonID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::Update
	Access:    	public
	Parameter: 	const byte a_Division
	Parameter: 	const ushort a_SeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordDivisionDetails::DoUpdate(const byte _Division, const ushort _SeasonID)
{
	m_Division = _Division;
    m_strSeasonID = _SeasonID;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::DetailsStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordDivisionDetails::DetailsStr(CCountry& rCountry)
{
	EXT_ASSERT(rCountry.IsActive() == true);
	CString str(TEXT("-"));
	if (m_strSeasonID != wNOTFOUND)
	{
		str.Format(TEXT("%s (%s)"), rCountry.GetActiveCountry().GetDivision(m_Division).GetName(), WorldData().GetText().GetString(m_strSeasonID));
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordDivisionDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_Division, sizeof(m_Division));
  	_pDataFile->Read(&m_strSeasonID, sizeof(m_strSeasonID));
}


/*------------------------------------------------------------------------------
	Method:   	CRecordDivisionDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordDivisionDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_Division, sizeof(m_Division));
  	_pDataFile->Write(&m_strSeasonID, sizeof(m_strSeasonID));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::CRecordPositionDetails
	Access:    	public
	Returns:
	Qualifier: 	: m_Position(null)
	Purpose:
------------------------------------------------------------------------------*/
CRecordPositionDetails::CRecordPositionDetails()
	: m_Position(null)
{
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::CheckIfHighest
	Access:    	public
	Parameter: 	const byte Position
	Parameter: 	const byte Division
	Parameter: 	const ushort strSeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPositionDetails::DoCheckIfHighest(const byte _Position, const byte _Division, const ushort _SeasonID)
{
    if (_Position >= m_Position)
	{
        m_Position = _Position;
		CRecordDivisionDetails::DoUpdate(_Division, _SeasonID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::CheckIfLowest
	Access:    	public
	Parameter: 	const byte Position
	Parameter: 	const byte Division
	Parameter: 	const ushort strSeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPositionDetails::DoCheckIfLowest(const byte _Points, const byte _Division, const ushort _SeasonID)
{
    if (m_Position <= _Points)
	{
        m_Position = _Points;
		CRecordDivisionDetails::DoUpdate(_Division, _SeasonID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::DetailsStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordPositionDetails::DetailsStr(CCountry& _Country)
{
	CString str(TEXT("-"));
	if (m_Position > 0)
	{
		str.Format(TEXT("%d - %s"), GetPosition(), CRecordDivisionDetails::DetailsStr(_Country));
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::GetPosition
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CRecordPositionDetails::GetPosition() const
{
	return m_Position;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPositionDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_Position, sizeof(m_Position));
	CRecordDivisionDetails::Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPositionDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPositionDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_Position, sizeof(m_Position));
	CRecordDivisionDetails::Write(_pDataFile);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::CRecordPointsDetails
	Access:    	public
	Returns:
	Qualifier: 	: m_Points(null)
	Purpose:
------------------------------------------------------------------------------*/
CRecordPointsDetails::CRecordPointsDetails()
	: m_Points(null)
{
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::CheckIfMost
	Access:    	public
	Parameter: 	const byte Points
	Parameter: 	const byte Division
	Parameter: 	const ushort a_nSeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPointsDetails::DoCheckIfMost(const byte _Points, const byte _Division, const ushort _SeasonID)
{
    if (_Points >= m_Points)
	{
        m_Points = _Points;
		CRecordDivisionDetails::DoUpdate(_Division, _SeasonID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::CheckIfLeast
	Access:    	public
	Parameter: 	const byte Points
	Parameter: 	const byte Division
	Parameter: 	const ushort a_nSeasonID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPointsDetails::DoCheckIfLeast(const byte _Points, const byte _Division, const ushort _SeasonID)
{
    if (m_Points <= _Points)
	{
        m_Points = _Points;
		CRecordDivisionDetails::DoUpdate(_Division, _SeasonID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::DetailsStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordPointsDetails::DetailsStr(CCountry& rCountry)
{
	CString str(TEXT("-"));
	if (m_Points > 0)
	{
		str.Format(TEXT("%d - %s"), GetPoints(), CRecordDivisionDetails::DetailsStr(rCountry));
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::GetPoints
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CRecordPointsDetails::GetPoints() const
{
	return m_Points;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPointsDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_Points, sizeof(m_Points));
	CRecordDivisionDetails::Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CRecordPointsDetails::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRecordPointsDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_Points, sizeof(m_Points));
	CRecordDivisionDetails::Write(_pDataFile);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CClubHistory
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier: 	: m_RecordReceipts(null)
	Purpose:
------------------------------------------------------------------------------*/
CClubHistory::CClubHistory()
	: m_RecordReceipts(null)
{
 }


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordSign
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const uint Cost
	Parameter: 	CDate Date
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClubHistory::CheckIfRecordSign(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date)
{
	return m_RecordSigning.CheckIfRecord(_Name, _ClubNameID, _Cost, _Date);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordSale
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	const uint Cost
	Parameter: 	CDate Date
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClubHistory::CheckIfRecordSale(CPersonName& _Name, const CStringID& _ClubNameID, const uint _Cost, CDate _Date)
{
	return m_RecordSale.CheckIfRecord(_Name, _ClubNameID, _Cost, _Date);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordAppear
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const ushort Value
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClubHistory::CheckIfRecordAppear(CPersonName& _Name, const ushort _Value)
{
	return m_RecordAppearance.CheckIfRecord(_Name, _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordGoals
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const ushort Value
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClubHistory::CheckIfRecordGoals(CPersonName& _Name, const ushort _Value)
{
	return m_RecordGoals.CheckIfRecord(_Name, _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordSeasonGoals
	Access:    	public
	Parameter: 	CPersonName& Name
	Parameter: 	const ushort Value
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CClubHistory::CheckIfRecordSeasonGoals(CPersonName& _Name, const ushort _Value)
{
	return m_RecordSeasonGoals.CheckIfRecord(_Name, _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordAttend
	Access:    	public
	Parameter: 	int Attendance
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	CDate Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClubHistory::CheckIfRecordAttend(int Attendance, const CStringID& ClubNameID, CDate Date)
{
	m_HighestAttendance.CheckIfRecord(Attendance, ClubNameID, Date);
	m_LowestAttendance.CheckIfRecord(Attendance, ClubNameID, Date, true);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::CheckIfRecordReceipt
	Access:    	public
	Parameter: 	const uint Receipt
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	CDate Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClubHistory::CheckIfRecordReceipt(const uint Receipt, const CStringID& ClubNameID, CDate Date)
{
    if (Receipt > m_RecordReceipts)
	{
        m_RecordReceipts = Receipt;
        m_RecordReceiptsClubID = ClubNameID;
        m_RecordReceiptsDate = Date;
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pRecordVictory
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordMatchDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordMatchDetails* CClubHistory::GetRecordVictory()
{
	return &m_RecordVictory;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pRecordDefeat
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordMatchDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordMatchDetails* CClubHistory::GetRecordDefeat()
{
	return &m_RecordDefeat;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pHighestPosition
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordPositionDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordPositionDetails* CClubHistory::GetHighestPosition()
{
	return &m_HighestPosition;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pLowestPosition
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordPositionDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordPositionDetails* CClubHistory::GetLowestPosition()
{
	return &m_LowestPosition;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pLowestDivision
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordDivisionDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordDivisionDetails* CClubHistory::GetLowestDivision()
{
	return &m_LowestDivision;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pHighestDivision
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordDivisionDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordDivisionDetails* CClubHistory::GetHighestDivision()
{
	return &m_HighestDivision;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pMostPoints
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordPointsDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordPointsDetails* CClubHistory::GetMostPoints()
{
	return &m_MostPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::pLeastPoints
	Access:    	public
	Parameter: 	void
	Returns:   	CRecordPointsDetails*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRecordPointsDetails* CClubHistory::GetLeastPoints()
{
	return &m_LeastPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordAttendanceStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClubHistory::RecordAttendanceStr()
{
	return m_HighestAttendance.DetailsStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordAttendanceInfoStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClubHistory::RecordAttendanceInfoStr()
{
	return m_HighestAttendance.InfoStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::LowestAttendanceInfoStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClubHistory::LowestAttendanceInfoStr()
{
	return m_LowestAttendance.InfoStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::LowestAttendanceStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClubHistory::LowestAttendanceStr()
{
	return m_LowestAttendance.DetailsStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordSaleStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordSaleStr(CCountry& rCountry)
{
	return m_RecordSale.ValueStr(rCountry);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordSaleInfoStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordSaleInfoStr(CCountry& rCountry)
{
	return m_RecordSale.DetailsStr(rCountry);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordBuyStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CClubHistory::RecordBuyStr(CCountry& _Country)
{
	return m_RecordSigning.ValueStr(_Country);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordBuyInfoStr
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordBuyInfoStr(CCountry& rCountry)
{
	return m_RecordSigning.DetailsStr(rCountry);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordAppearanceInfoStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordAppearanceInfoStr()
{
	return m_RecordAppearance.InfoStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordGoalsInfoStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordGoalsInfoStr()
{
	return m_RecordGoals.InfoStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::RecordSeasonGoalsInfoStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CClubHistory::RecordSeasonGoalsInfoStr()
{
	return m_RecordSeasonGoals.InfoStr();
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClubHistory::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_RecordReceiptsClubID, sizeof(m_RecordReceiptsClubID));
  	_pDataFile->Read(&m_RecordReceipts, sizeof(m_RecordReceipts));
	m_RecordSigning.Read(_pDataFile);
	m_RecordSale.Read(_pDataFile);
	m_RecordAppearance.Read(_pDataFile);
	m_RecordGoals.Read(_pDataFile);
	m_RecordSeasonGoals.Read(_pDataFile);
	m_HighestAttendance.Read(_pDataFile);
	m_LowestAttendance.Read(_pDataFile);
	m_LowestDivision.Read(_pDataFile);
	m_HighestDivision.Read(_pDataFile);
  	_pDataFile->Read(&m_RecordReceiptsDate, sizeof(m_RecordReceiptsDate));
	m_HighestPosition.Read(_pDataFile);
	m_LowestPosition.Read(_pDataFile);
	m_MostPoints.Read(_pDataFile);
	m_LeastPoints.Read(_pDataFile);
	m_RecordVictory.Read(_pDataFile);
	m_RecordDefeat.Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CClubHistory::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CClubHistory::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Write(&m_RecordReceiptsClubID, sizeof(m_RecordReceiptsClubID));
  	_pDataFile->Write(&m_RecordReceipts, sizeof(m_RecordReceipts));
	m_RecordSigning.Write(_pDataFile);
	m_RecordSale.Write(_pDataFile);
	m_RecordAppearance.Write(_pDataFile);
	m_RecordGoals.Write(_pDataFile);
	m_RecordSeasonGoals.Write(_pDataFile);
	m_HighestAttendance.Write(_pDataFile);
	m_LowestAttendance.Write(_pDataFile);
	m_LowestDivision.Write(_pDataFile);
	m_HighestDivision.Write(_pDataFile);
  	_pDataFile->Write(&m_RecordReceiptsDate, sizeof(m_RecordReceiptsDate));
	m_HighestPosition.Write(_pDataFile);
	m_LowestPosition.Write(_pDataFile);
	m_MostPoints.Write(_pDataFile);
	m_LeastPoints.Write(_pDataFile);
	m_RecordVictory.Write(_pDataFile);
	m_RecordDefeat.Write(_pDataFile);
}
