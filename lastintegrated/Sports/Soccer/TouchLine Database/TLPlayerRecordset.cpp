#include "stdafx.h"
#include "TLPlayer.h"


CTLPlayerRecordset::CTLPlayerRecordset(CDatabase* pDB, bool bReadAllRecords)
: CExtRecordset(pDB)
{
	DoInitialise();
	if (bReadAllRecords)
	{
		afxDump << TEXT("SELECT ") + GetDBFieldsString() + TEXT(" FROM Player") << "\n";
		CExtRecordset::SetSqlString(TEXT("SELECT ") + GetDBFieldsString() + TEXT(" FROM Player"), TEXT(""));
		CExtRecordset::DoExecute();
	}
}


CTLPlayerRecordset::CTLPlayerRecordset(void)
{
	DoInitialise();
}


void CTLPlayerRecordset::DoInitialise()
{
	m_strDBFieldNames.Add(TEXT("ID"));
	m_strDBFieldNames.Add(TEXT("ClubID"));
	m_strDBFieldNames.Add(TEXT("Forename"));
	m_strDBFieldNames.Add(TEXT("Surname"));
	m_strDBFieldNames.Add(TEXT("DateOfBirth"));
	m_strDBFieldNames.Add(TEXT("NationalityID"));
	m_strDBFieldNames.Add(TEXT("Role"));
	//m_strDBFieldNames.Add(TEXT("Wage"));
	m_strDBFieldNames.Add(TEXT("Temperament"));
	m_strDBFieldNames.Add(TEXT("OverallSkill"));
	m_strDBFieldNames.Add(TEXT("Handling"));
	m_strDBFieldNames.Add(TEXT("Tackling"));
	m_strDBFieldNames.Add(TEXT("Passing"));
	m_strDBFieldNames.Add(TEXT("Shooting"));
	m_strDBFieldNames.Add(TEXT("Pace"));
	m_strDBFieldNames.Add(TEXT("BestFoot"));
	m_strDBFieldNames.Add(TEXT("Heading"));
	m_strDBFieldNames.Add(TEXT("Strength"));
	m_strDBFieldNames.Add(TEXT("Flair"));
}


CTLPlayerRecordset::~CTLPlayerRecordset(void)
{
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		delete static_cast<CTLPlayer*>(m_RecordList.GetAt(nLoopCount));
	}
}


void CTLPlayerRecordset::DoWriteAll(FILE* pFile)
{
	uint16 nHowMany = m_RecordList.GetSize();
	fwrite(&nHowMany, sizeof nHowMany, 1, pFile);
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		static_cast<CTLPlayer*>(m_RecordList.GetAt(nLoopCount))->Write(pFile);
	}
	afxDump << " Players : " << nHowMany << "\n";
}


void CTLPlayerRecordset::DoReadClubPlayers(const uint16 nClubNumber, CNonDuplicateStringList& strSurnames, const uint16 nClubID)
{
	CString strClubID;
	strClubID.Format(TEXT("ClubID = %d"), nClubNumber);
	SetSqlString(TEXT(""), strClubID);
	CExtRecordset::DoExecute();

	// Loop through each player
	uint16 nPlayersFound = 0;
	while(!CRecordset::IsEOF())
	{
		AddRecord(new CTLPlayer);
		static_cast<CTLPlayer*>(GetCurrentRecord())->DoInitialiseFromDB(*this, nClubID, strSurnames);
		nPlayersFound++;
		CRecordset::MoveNext(); // goto next record
	}
	afxDump << nClubID << ": Players : " << nPlayersFound << "\n";
}
