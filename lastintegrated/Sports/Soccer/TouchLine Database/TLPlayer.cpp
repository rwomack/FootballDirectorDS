#include "stdafx.h"
#include ".\tlplayer.h"

CTLPlayer::CTLPlayer(void)
: m_nID(0)
, m_nTeamID(0)
, m_nSurnameID(0)
, m_dateOfBirth(0)
, m_nNationalityID(0)
, m_nPosition(0)
, m_nTemperament(0)
, m_nOverallSkill(0)
, m_nHandling(0)
, m_nTackling(0)
, m_nPassing(0)
, m_nShooting(0)
, m_nPace(0)
, m_nBestFoot(0)
, m_nStrength(0)
, m_nFlair(0)
{
}


CTLPlayer::~CTLPlayer(void)
{
}


//  Initialise from the current Touch-Line database manager record
void CTLPlayer::DoInitialiseFromDB(CTLPlayerRecordset& recsetTLPlayer, const short nClubID, CNonDuplicateStringList& strSurnames)
{
	CDBVariant  rFieldValue;
	int iCurrentField = 0;

	// Copy each column into a variable
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nID = rFieldValue.m_iVal;
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nTeamID = nClubID;
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_strForename = rFieldValue.m_pstring->Left(1);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nSurnameID = strSurnames.FindStringID(*(rFieldValue.m_pstring));
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	tagTIMESTAMP_STRUCT stDateOfBirth = *(rFieldValue.m_pdate);
	m_dateOfBirth = CDate(stDateOfBirth.year, stDateOfBirth.month, stDateOfBirth.day);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nNationalityID = static_cast<uint16>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nPosition = static_cast<uint8>(--rFieldValue.m_iVal);
	m_nPosition = m_nPosition | (m_nBestFoot * 128);

	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
    m_nTemperament = static_cast<uint8>(rFieldValue.m_iVal) / 10;	// *** In db 0-100, in game 0-10 ***
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nOverallSkill = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nHandling = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nTackling = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nPassing = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nShooting = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nPace = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nBestFoot = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nHeading = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nStrength = static_cast<uint8>(rFieldValue.m_iVal);
	recsetTLPlayer.GetFieldValue(iCurrentField++, rFieldValue);
	m_nFlair = static_cast<uint8>(rFieldValue.m_iVal);

	if (m_nOverallSkill == 0)
	{
	}
}


void CTLPlayer::Write(FILE* pFile)
{
	fwrite(&m_nTeamID, sizeof m_nTeamID, 1, pFile);

	// CPerson data,
	m_strForename.Write(pFile, false);
	fwrite(&m_nSurnameID, sizeof m_nSurnameID, 1, pFile);
	m_dateOfBirth.Write(pFile);
	fwrite(&m_nNationalityID, sizeof m_nNationalityID, 1, pFile);
	fwrite(&m_nTemperament, sizeof m_nTemperament, 1, pFile);

	// CSoccerPlayer data,
	fwrite(&m_nOverallSkill, sizeof m_nOverallSkill, 1, pFile);
	fwrite(&m_nHandling, sizeof m_nHandling, 1, pFile);
	fwrite(&m_nTackling, sizeof m_nTackling, 1, pFile);
	fwrite(&m_nPassing, sizeof m_nPassing, 1, pFile);
	fwrite(&m_nShooting, sizeof m_nShooting, 1, pFile);
	fwrite(&m_nPace, sizeof m_nPace, 1, pFile);
	fwrite(&m_nPosition, sizeof m_nPosition, 1, pFile);
	fwrite(&m_nHeading, sizeof m_nHeading, 1, pFile);
	fwrite(&m_nStrength, sizeof m_nStrength, 1, pFile);
	fwrite(&m_nFlair, sizeof m_nFlair, 1, pFile);
}