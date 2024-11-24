#include "stdafx.h"
#include ".\extrecordset.h"

CExtRecordset::CExtRecordset(void)
{
}

CExtRecordset::~CExtRecordset(void)
{
}


BOOL CExtRecordset::DoExecute(const CRecordset::OpenType eOpenType, const CRecordset::OpenOptions eOpenOptions)
{
	ASSERT(m_strSql.IsEmpty() == false);
	if (CRecordset::IsOpen() == TRUE)
	{
		CRecordset::FlushResultSet();
		return CRecordset::Requery();
	}
	else
	{
		return CRecordset::Open(eOpenType, m_strSql, eOpenOptions);
	}
}


void CExtRecordset::SetSqlString(CString SqlString, CString strSQLFilter)
{
	if (CRecordset::IsOpen() == TRUE)
	{
		CRecordset::m_strFilter = strSQLFilter;
	}
	else
	{
		m_strSql = SqlString;
	}
}


CString CExtRecordset::GetDBFieldString(const UInt16 nFieldNumber)
{
	ASSERT(nFieldNumber < m_strDBFieldNames.GetSize());
	return m_strDBFieldNames.GetAt(nFieldNumber);
}


CString CExtRecordset::GetDBFieldsString()
{
	CString strRet;
	UInt16 nLoopSize = m_strDBFieldNames.GetSize() - 1;
	UInt16 nLoopCount;
	for (nLoopCount = 0; nLoopCount < nLoopSize; nLoopCount++)
	{
		strRet += m_strDBFieldNames[nLoopCount] + TEXT(", ");
	}
	return 	strRet + m_strDBFieldNames.GetAt(nLoopCount);
}


void CExtRecordset::GetFieldValue(const UInt16 nFieldNumber, CDBVariant & rFieldValue)
{
	CRecordset::GetFieldValue(GetDBFieldString(nFieldNumber), rFieldValue);
}
