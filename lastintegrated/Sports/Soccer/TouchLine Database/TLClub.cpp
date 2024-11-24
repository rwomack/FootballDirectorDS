#include "stdafx.h"
#include "TLClub.h"


CTLClub::CTLClub(void)
: m_nID(0)
, m_nGameID(0)
, m_nPopularityID(0)
, m_nStadiumCapacity(0)
, m_nYearFounded(0)
, m_nManagerID(0)
, m_nDivisionID(0)
, m_nRegionID(0)
{
}


CTLClub::~CTLClub(void)
{
}


//  Initialise from the current Touch-Line database club  record
void CTLClub::DoInitialiseFromDB(CExtRecordset& recsetTLClub, const uint16 nClubCount)
{
	m_nGameID = nClubCount;

	CDBVariant  rFieldValue;
	int iCurrentField = 0;

	// Copy each column into a variable
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nID = static_cast<uint16>(rFieldValue.m_lVal);
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_strName = *(rFieldValue.m_pstring);
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nDivisionID = static_cast<uint8>(rFieldValue.m_lVal) - 1;
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nPopularityID = static_cast<uint8>(rFieldValue.m_lVal);
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_strStadiumName = *(rFieldValue.m_pstring);
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nStadiumCapacity = static_cast<uint32>(rFieldValue.m_lVal);
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nYearFounded = (short)rFieldValue.m_lVal;
	recsetTLClub.GetFieldValue(iCurrentField++, rFieldValue);
	m_nManagerID = rFieldValue.m_iVal;
}	


void CTLClub::WriteStadiumData(FILE* pFile)
{
	m_strStadiumName.Write(pFile);
	fwrite(&m_nStadiumCapacity, sizeof m_nStadiumCapacity, 1, pFile);
}


void CTLClub::Write(FILE* pFile)
{
	m_strName.Write(pFile);
	fwrite(&m_nGameID, sizeof m_nGameID, 1, pFile);	// Serves as stadium ID
	fwrite(&m_nPopularityID, sizeof m_nPopularityID, 1, pFile);
	fwrite(&m_nRegionID, sizeof m_nRegionID, 1, pFile);
	fwrite(&m_nDivisionID, sizeof m_nDivisionID, 1, pFile);
	fwrite(&m_nYearFounded, sizeof m_nYearFounded, 1, pFile);
	fwrite(&m_nManagerID, sizeof m_nManagerID, 1, pFile);
}