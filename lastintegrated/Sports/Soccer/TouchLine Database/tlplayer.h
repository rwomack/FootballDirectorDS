#include "afxdb.h"
#include "Date.h"
#include "TLPlayerRecordset.h"
#include "ExtString.h"

class CTLPlayer
{
public:
	CTLPlayer(void);
	~CTLPlayer(void);

	//  Initialise from the current Touch-Line database club  record
	void					DoInitialiseFromDB(CTLPlayerRecordset& recsetTLPlayer, const short nClubID, CNonDuplicateStringList& strSurnames);
	void					Write(FILE* pFile);

protected:
	short					m_nID;
	short					m_nTeamID;
	CExtString				m_strForename;
	UInt16					m_nSurnameID;
	CDate					m_dateOfBirth;
	UInt16					m_nNationalityID;
	uint8					m_nPosition;
	//UInt32					m_nWage;
	uint8					m_nTemperament;
	uint8					m_nOverallSkill;
	uint8					m_nHandling;
	uint8					m_nTackling;
	uint8					m_nPassing;
	uint8					m_nShooting;
	uint8					m_nPace;
	uint8					m_nBestFoot;
	uint8					m_nHeading;
	uint8					m_nStrength;
	uint8					m_nFlair;
};
