
/*

	RecordAttendance.h


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/

#ifndef __RECORDATTENDANCE_H__
#define __RECORDATTENDANCE_H__


class CRecordAttendanceDetails
{
public:
	CRecordAttendanceDetails();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	bool					CheckIfRecord(uint _Attendance, const CStringID& _ClubNameID, CDate _Date, bool _bCheckLowest = false);
	CString					InfoStr();
	CString					DetailsStr();
	CString					GetRecordAttendanceStr();
	uint					GetRecordAttendance();					

private:
    uint					m_RecordAttendance;
	CStringID				m_RecordAttendanceClubID;
    CDate					m_RecordAttendanceDate;
};    
#endif