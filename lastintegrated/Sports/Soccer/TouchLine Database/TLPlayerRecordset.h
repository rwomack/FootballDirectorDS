#include "ExtRecordset.h"

#ifndef __TLPLAYERRECORDSET_H__
#define __TLPLAYERRECORDSET_H__

class CTLPlayerRecordset :
	public CExtRecordset
{
public:
	CTLPlayerRecordset(void);
	CTLPlayerRecordset(CDatabase* pDB, bool bReadAllRecords);
	~CTLPlayerRecordset(void);

	void					DoReadClubPlayers(const UInt16 nClubNumber, CNonDuplicateStringList& strSurnames, const UInt16 nClubID);
	void					DoInitialise(void);
	void					DoWriteAll(FILE* pFile);

protected:

};
#endif