#include "stdafx.h"
#include ".\TLManagerRecordset.h"
#include "TLManager.h"


CTLManagerRecordset::CTLManagerRecordset(CDatabase* pDB, bool bReadAllRecords)
	: CExtRecordset(pDB)
{
	DoInitialise();
	if (bReadAllRecords)
	{
		CExtRecordset::SetSqlString(TEXT("SELECT ") + GetDBFieldsString() + TEXT(" FROM Manager"), TEXT(""));
		CExtRecordset::DoExecute();
	}
}


CTLManagerRecordset::CTLManagerRecordset(void)
{
	DoInitialise();
}


void CTLManagerRecordset::DoInitialise()
{
	CExtRecordset::m_strDBFieldNames.Add(TEXT("ManagerID"));
	CExtRecordset::m_strDBFieldNames.Add(TEXT("Forename"));
	CExtRecordset::m_strDBFieldNames.Add(TEXT("Surname"));
	CExtRecordset::m_strDBFieldNames.Add(TEXT("DateOfBirth"));
	CExtRecordset::m_strDBFieldNames.Add(TEXT("Nationality"));
}


CTLManagerRecordset::~CTLManagerRecordset(void)
{
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		delete static_cast<CTLManager*>(CExtRecordset::m_RecordList.GetAt(nLoopCount));
	}
}


short CTLManagerRecordset::DoCreateManagerData(const short nManagerID, CNonDuplicateStringList& strSurnames)
{
	CString strManagerID;
	strManagerID.Format(TEXT("ManagerID = %d"), nManagerID);
	SetSqlString(TEXT(""), strManagerID);
	DoExecute();
	AddRecord(new CTLManager);
	static_cast<CTLManager*>(GetCurrentRecord())->DoInitialiseFromDB(*this, strSurnames);
	return GetCurrentRecordID();
}


void CTLManagerRecordset::DoWriteAll(FILE* pFile)
{
	uint16 nHowMany = CExtRecordset::m_RecordList.GetSize();
	fwrite(&nHowMany, sizeof nHowMany, 1, pFile);
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		static_cast<CTLManager*>(m_RecordList.GetAt(nLoopCount))->Write(pFile);
	}
	afxDump << " Managers : " << nHowMany << "\n";
}
