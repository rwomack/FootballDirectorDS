#include "afxdb.h"
#include "ExtRecordset.h"
#include "Date.h"
#include "ExtString.h"

class CTLManager
{
public:
	CTLManager(void);
	virtual ~CTLManager(void);

	//  Initialise from the current Touch-Line database club  record
	void					DoInitialiseFromDB(CExtRecordset& recsetTLManager, CNonDuplicateStringList& strSurnames);
	void					Write(FILE* pFile);

protected:
	short					m_nID;
	CExtString				m_strForename;
	UInt16					m_nSurnameID;
	CDate					m_dateOfBirth;
	UInt16					m_nNationality;
};
