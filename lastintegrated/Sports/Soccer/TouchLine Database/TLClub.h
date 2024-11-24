
#ifndef __TLCLUB_H__
#define __TLCLUB_H__

class CTLClub
{
public:
	CTLClub(void);
	virtual ~CTLClub(void);

	void					WriteStadiumData(FILE* pFile);
	void					Write(FILE* pFile);

	//  Initialise from the current Touch-Line database club  record
	void					DoInitialiseFromDB(CExtRecordset& recsetTLClub, const uint16 nClubCount);
	short					GetID(void) const
	{
		return m_nID;
	}
	short					GetManagerID(void) const
	{
		return m_nManagerID;
	}
	void					SetManagerID(const short newID)
	{
		m_nManagerID = newID;
	}

protected:
	uint16					m_nID;
	short					m_nGameID;
	CExtString				m_strName;
	uint8					m_nPopularityID;
	CExtString				m_strStadiumName;
	uint32					m_nStadiumCapacity;
	short					m_nYearFounded;
	short					m_nManagerID;
	uint8					m_nDivisionID;
	uint8					m_nRegionID;
};
#endif