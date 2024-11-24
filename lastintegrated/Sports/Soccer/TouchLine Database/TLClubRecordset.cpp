#include "stdafx.h"
#include "TLClub.h"


CTLClubRecordset::CTLClubRecordset(CDatabase* pDB, const bool bReadAllRecords /*= false*/)
: CExtRecordset(pDB)
{
	DoInitialise();
	if (bReadAllRecords)
	{
		CExtRecordset::SetSqlString(TEXT("SELECT ") + GetDBFieldsString() + TEXT(" FROM Club"), TEXT(""));
		CExtRecordset::DoExecute();
	}
}


CTLClubRecordset::CTLClubRecordset(void)
{
	DoInitialise();
}


void CTLClubRecordset::DoInitialise()
{
	m_strDBFieldNames.Add(TEXT("TeamID"));
	m_strDBFieldNames.Add(TEXT("Name"));
	m_strDBFieldNames.Add(TEXT("Division"));
	m_strDBFieldNames.Add(TEXT("PopularityID"));
	m_strDBFieldNames.Add(TEXT("StadiumName"));
	m_strDBFieldNames.Add(TEXT("StadiumCapacity"));
	m_strDBFieldNames.Add(TEXT("YearFounded"));
	m_strDBFieldNames.Add(TEXT("ManagerID"));
	m_strDBFieldNames.Add(TEXT("RegionID"));
}


CTLClubRecordset::~CTLClubRecordset(void)
{
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		delete static_cast<CTLClub*>(m_RecordList.GetAt(nLoopCount));
	}
}


void CTLClubRecordset::DoReadDivisionClubs(const uint16 nDivisionNumber)
{
	CString strDivisionID;

	// Build the SQL statement
	strDivisionID.Format(TEXT("Division = %d"), nDivisionNumber);
	CExtRecordset::SetSqlString(TEXT(""), strDivisionID);
	CExtRecordset::DoExecute();

	// Loop through each player
	while(!CRecordset::IsEOF())
	{
		CExtRecordset::AddRecord(new CTLClub);
		static_cast<CTLClub*>(GetCurrentRecord())->DoInitialiseFromDB(*this, CExtRecordset::GetSize() - 1);
		CRecordset::MoveNext(); // goto next record
	}
}


void CTLClubRecordset::DoReadAllPlayers(CTLPlayerRecordset& arsPlayers, CNonDuplicateStringList& astrSurnames)
{
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		arsPlayers.DoReadClubPlayers(static_cast<CTLClub*>(CExtRecordset::m_RecordList.GetAt(nLoopCount))->GetID(), astrSurnames, nLoopCount);
	}
}


void CTLClubRecordset::DoReadAllManagers(CTLManagerRecordset& arsManagers, CNonDuplicateStringList& astrSurnames)
{
	CTLClub* pTLClub;
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		pTLClub = static_cast<CTLClub*>(CExtRecordset::m_RecordList.GetAt(nLoopCount));
		pTLClub->SetManagerID(arsManagers.DoCreateManagerData(pTLClub->GetManagerID(), astrSurnames));
	}
}


void CTLClubRecordset::DoWriteAll(FILE* pFile)
{
	FILE* tlFile = fopen(TEXT("Clubs.bin"), "wb");
	uint16 nHowMany = m_RecordList.GetSize();
	fwrite(&nHowMany, sizeof nHowMany, 1, pFile);
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		static_cast<CTLClub*>(m_RecordList.GetAt(nLoopCount))->Write(tlFile);
	}
	fclose(tlFile);

	tlFile = fopen(TEXT("Stadiums.bin"), "wb");
	fwrite(&nHowMany, sizeof nHowMany, 1, pFile);
	for (uint16 nLoopCount = 0; nLoopCount < m_RecordList.GetSize(); nLoopCount++)
	{
		static_cast<CTLClub*>(m_RecordList.GetAt(nLoopCount))->WriteStadiumData(tlFile);
	}
	fclose(tlFile);
}
