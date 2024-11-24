#include "afxdb.h"

#ifndef __EXTRECORDSET_H__
#define __EXTRECORDSET_H__


class CExtRecordset :
	public CRecordset
{
public:
	CExtRecordset(void);
	CExtRecordset(CDatabase* pDB)
		: CRecordset(pDB) 
	, m_iCurrentRecord(-1) {};
	~CExtRecordset(void);

	void					AddRecord(void* pRecord)
	{
		m_iCurrentRecord = m_RecordList.Add(pRecord);
	}
	int						GetCurrentRecordID(void) const
	{
		ASSERT(m_iCurrentRecord > -1);
		return m_iCurrentRecord;
	}
	void*					GetCurrentRecord(void) const
	{
		ASSERT(m_iCurrentRecord > -1);
		return m_RecordList.GetAt(m_iCurrentRecord);
	}
	void					SetSqlString(CString SqlString, CString strSQLFilter);
	CString					GetDBFieldString(const UInt16 nFieldNumber);
	CString					GetDBFieldsString(void);
	void					GetFieldValue(const UInt16 nFieldNumber, CDBVariant & rFieldValue);
	BOOL					DoExecute(const CRecordset::OpenType eOpenType = CRecordset::forwardOnly, 
									  const CRecordset::OpenOptions eOpenOptions = CRecordset::readOnly);
	const UInt16			GetSize(void)
	{
		return m_RecordList.GetSize();
	}
protected:
	CString					m_strSql;
	CPtrArray				m_RecordList;
	int						m_iCurrentRecord;
	CStringArray			m_strDBFieldNames;
};
#endif