
/*
---------------------------------------------------------------

	RecordAttendance.cpp : implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "RecordAttendance.h"


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::CRecordAttendanceDetails
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRecordAttendanceDetails::CRecordAttendanceDetails()
{
	m_RecordAttendance = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::GetRecordAttendance
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CRecordAttendanceDetails::GetRecordAttendance()					
{ 
	return m_RecordAttendance; 
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::CheckIfRecord
	Access:    	public 
	Parameter: 	uint Attendance
	Parameter: 	const CStringID& ClubNameID
	Parameter: 	CDate Date
	Parameter: 	bool bCheckLowest
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CRecordAttendanceDetails::CheckIfRecord(uint _Attendance, const CStringID& _ClubNameID, CDate _Date, bool _bCheckLowest /*= false*/)
{
	bool bUpdate = false;
	if (_bCheckLowest == false)
	{
		if (_Attendance > GetRecordAttendance())
		{
			bUpdate = true;
		}
    }
	else
	{
		if (_Attendance < GetRecordAttendance() || m_RecordAttendance == 0)
		{
			bUpdate = true;
		}
    }

	if (bUpdate == true)
	{
		m_RecordAttendance = _Attendance;
		m_RecordAttendanceClubID = _ClubNameID;
		m_RecordAttendanceDate = _Date;
	}
	return bUpdate;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::InfoStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordAttendanceDetails::InfoStr()
{
	CString str(TEXT("-"));
	if (m_RecordAttendance > 0)
	{
		str.Format(TEXT("%s - %s(%s)"), GetRecordAttendanceStr(), m_RecordAttendanceClubID.GetStr(), m_RecordAttendanceDate.ShortNumDateStr());
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::DetailsStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordAttendanceDetails::DetailsStr()
{
	CString str(TEXT("-"));
	if (m_RecordAttendance > 0)
	{
		str = GetRecordAttendanceStr();
	}
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::GetRecordAttendanceStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CRecordAttendanceDetails::GetRecordAttendanceStr()
{
	CExtString strAttend;
	strAttend.FormatLargeNumber(m_RecordAttendance);
	return strAttend;
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRecordAttendanceDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Read(&m_RecordAttendance, sizeof m_RecordAttendance);
	_pDataFile->Read(&m_RecordAttendanceClubID, sizeof m_RecordAttendanceClubID);
	_pDataFile->Read(&m_RecordAttendanceDate, sizeof m_RecordAttendanceDate);
}


/*------------------------------------------------------------------------------
	Method:   	CRecordAttendanceDetails::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRecordAttendanceDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Write(&m_RecordAttendance, sizeof m_RecordAttendance);
	_pDataFile->Write(&m_RecordAttendanceClubID, sizeof m_RecordAttendanceClubID);
	_pDataFile->Write(&m_RecordAttendanceDate, sizeof m_RecordAttendanceDate);
}
