#include "stdafx.h"
#include ".\tlmanager.h"

CTLManager::CTLManager(void)
: m_nID(0)
, m_nSurnameID(0)
, m_dateOfBirth(0)
, m_nNationality(0)
{
}

CTLManager::~CTLManager(void)
{
}


//  Initialise from the current Touch-Line database manager record
void CTLManager::DoInitialiseFromDB(CExtRecordset& recsetTLManager, CNonDuplicateStringList& strSurnames)
{
	CDBVariant  rFieldValue;
	int iCurrentField = 0;

	// Copy each column into a variable
	recsetTLManager.GetFieldValue(iCurrentField++, rFieldValue);
	m_nID = rFieldValue.m_iVal;
	recsetTLManager.GetFieldValue(iCurrentField++, rFieldValue);
	m_strForename = rFieldValue.m_pstring->Left(1);
	recsetTLManager.GetFieldValue(iCurrentField++, rFieldValue);
	m_nSurnameID = strSurnames.FindStringID(*(rFieldValue.m_pstring));
	recsetTLManager.GetFieldValue(iCurrentField++, rFieldValue);
	tagTIMESTAMP_STRUCT stDateOfBirth = *(rFieldValue.m_pdate);
	m_dateOfBirth = CDate(stDateOfBirth.year, stDateOfBirth.month, stDateOfBirth.day);
	recsetTLManager.GetFieldValue(iCurrentField++, rFieldValue);
	m_nNationality = theApp.TranslateNationality(static_cast<uint16>(rFieldValue.m_iVal));
}


void CTLManager::Write(FILE* pFile)
{
	// CPerson data,
	m_strForename.Write(pFile, false);
	fwrite(&m_nSurnameID, sizeof m_nSurnameID, 1, pFile);
	m_dateOfBirth.Write(pFile);
	fwrite(&m_nNationality, sizeof m_nNationality, 1, pFile);
	afxDump << m_strForename << " " << m_nSurnameID << " " << m_nNationality << "\n";
}