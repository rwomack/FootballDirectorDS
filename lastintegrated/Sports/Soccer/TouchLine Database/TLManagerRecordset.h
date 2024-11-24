#include "ExtRecordset.h"


#ifndef __TLMANAGERRECORDSET_H__
#define __TLMANAGERRECORDSET_H__

class CTLManagerRecordset :
	public CExtRecordset
{
public:
	CTLManagerRecordset(void);
	CTLManagerRecordset(CDatabase* pDB, bool bReadAllRecords);
	~CTLManagerRecordset(void);

	void					DoInitialise(void);
	short					DoCreateManagerData(const short nManagerID, CNonDuplicateStringList& strSurnames);
	void					DoWriteAll(FILE* pFile);

protected:

};
#endif