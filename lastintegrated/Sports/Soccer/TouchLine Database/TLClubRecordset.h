
#ifndef __TLCLUBRECORDSET_H__
#define __TLCLUBRECORDSET_H__

class CTLClubRecordset :
	public CExtRecordset
{
public:
	CTLClubRecordset(void);
	CTLClubRecordset(CDatabase* pDB, const bool bReadAllRecords = false);
	~CTLClubRecordset(void);

	void					DoInitialise(void);
	void					DoReadDivisionClubs(const uint16 nDivisionNumber);
	void					DoReadAllPlayers(CTLPlayerRecordset& arsPlayers, CNonDuplicateStringList& astrSurnames);
	void					DoReadAllManagers(CTLManagerRecordset& arsManagers, CNonDuplicateStringList& astrSurnames);
	void					DoWriteAll(FILE* pFile);

protected:

};
#endif