// PalmPDBHeader.cpp

#include "stdafx.h"
#include "PalmDataMgr.h"
#include "Categories.h"
#include "PalmPDBHeader.h"

/////////////////////////////////////////////////////////////////////////////
// CPalmPDBHeader


 // ---------------------------------------------------------------------------
void CPalmPDBHeader::DoInitialiseFromPDB(CEndianFile& file)
{
	file.Read(&m_szName[0], 32);
	file.Read(&m_nFlags, sizeof(m_nFlags));
	m_nFlags = (UInt16)FLIPWORD(m_nFlags);
	file.Read(&m_nVersion, sizeof(m_nVersion));
	m_nVersion = (UInt16)FLIPWORD(m_nVersion);
	file.Read(&m_nCreationTime, sizeof(m_nCreationTime));
	m_nCreationTime = FLIPLONG(m_nCreationTime);
	file.Read(&m_nModificationTime, sizeof(m_nModificationTime));
	m_nModificationTime = FLIPLONG(m_nModificationTime);
	file.Read(&m_nBackupTime, sizeof(m_nBackupTime));
	m_nBackupTime = FLIPLONG(m_nBackupTime);
	file.Read(&m_nModificationNumber, sizeof(m_nModificationNumber));
	m_nModificationNumber = FLIPLONG(m_nModificationNumber);
	file.Read(&m_nAppInfo, sizeof(m_nAppInfo));
	m_nAppInfo = FLIPLONG(m_nAppInfo);
	file.Read(&m_nSortInfo, sizeof(m_nSortInfo));
	m_nSortInfo = FLIPLONG(m_nSortInfo);
	file.Read(&m_nType, sizeof(m_nType));
	m_nType = FLIPLONG(m_nType);
	file.Read(&m_nCreator, sizeof(m_nCreator));
	m_nCreator = FLIPLONG(m_nCreator);
	file.Read(&m_nUniqueID, sizeof(m_nUniqueID));
	m_nUniqueID = FLIPLONG(m_nUniqueID);
	file.Read(&m_nNextRecordList, sizeof(m_nNextRecordList));
	m_nNextRecordList = FLIPLONG(m_nNextRecordList);
	file.Read(&m_nCount, sizeof(m_nCount));
	m_nCount = (UInt16)FLIPWORD(m_nCount);
}

/////////////////////////////////////////////////////////////////////////////
// CPalmPDBRecordList


 // ---------------------------------------------------------------------------
void CPalmPDBRecordList::DoInitialiseFromPDB(CEndianFile& file)
{
	file.Read(&m_nOffset, sizeof(m_nOffset));
	m_nOffset = FLIPLONG(m_nOffset);
	file.Read(&m_nAttributes, sizeof(m_nAttributes));
	file.Read(&m_nId, sizeof(m_nId));
}

/////////////////////////////////////////////////////////////////////////////
// CWineGuidePalmPDBappInfoRecord


 // ---------------------------------------------------------------------------
void CWineGuidePalmPDBappInfoRecord::DoInitialiseFromPDB(CEndianFile& file)
{
	CPalmPDBappInfoRecord::DoInitialiseFromPDB(file);
	file.Read(&m_nReserved1, sizeof(m_nReserved1));
	file.Read(&m_nReserved2, sizeof(m_nReserved2));
	file.Read(&m_nVarietalIndexStart, sizeof(m_nVarietalIndexStart));
	m_nVarietalIndexStart = FLIPWORD(m_nVarietalIndexStart);
	file.Read(&m_nNumWineRecords, sizeof(m_nNumWineRecords));
	m_nNumWineRecords = FLIPWORD(m_nNumWineRecords);
	file.Read(&m_stEditionType, sizeof(m_stEditionType));
	m_stEditionType.BuildNumber = FLIPWORD(m_stEditionType.BuildNumber);
	m_stEditionType.wVersion = FLIPWORD(m_stEditionType.wVersion);
}

/////////////////////////////////////////////////////////////////////////////
// CPalmPDBappInfoRecord


 // ---------------------------------------------------------------------------
void CPalmPDBappInfoRecord::DoInitialiseFromPDB(CEndianFile& file)
{
	UInt16 nTemp;
	m_nFileOffset = file.GetPosition();
	file.Read(&m_nBitField, sizeof(m_nBitField));
	file.Read(&nTemp, sizeof(nTemp));					// alignment
	m_Categories.Read(file);
	file.Read(&m_nUnique, 16);
	file.Read(&m_nLastCategoryUsed, sizeof(m_nLastCategoryUsed));
}


 // ---------------------------------------------------------------------------
void CPalmPDBappInfoRecord::DoSaveToPDB(CEndianFile& file)
{
	UInt16 nTemp;
	ASSERT(m_nFileOffset != 0);		// Ensure read in
	file.Seek(m_nFileOffset, CFile::begin);

	file.Write(&m_nBitField, sizeof(m_nBitField));
	file.Write(&nTemp, sizeof(nTemp));					// alignment
	m_Categories.Write(file);
	file.Write(&m_nUnique, 16);
	file.Write(&m_nLastCategoryUsed, sizeof(m_nLastCategoryUsed));
}

/////////////////////////////////////////////////////////////////////////////
// CPalmPDB


 // ---------------------------------------------------------------------------
void CPalmPDB::DoDestroyData()
{
	delete [] m_pRecordList;
	m_pRecordList = NULL;
}


 // ---------------------------------------------------------------------------
bool CPalmPDB::DoInitialise(CString strFileAndPathName, UINT nOpenFlags)
{
    m_bExists = m_cEndianFile.Open(strFileAndPathName, nOpenFlags);
	if (m_bExists == true)
	{
		m_cHeader.DoInitialiseFromPDB(m_cEndianFile);

		UInt16 nNumberOfRecords = m_cHeader.GetNumberOfRecords();
		ASSERT(m_pRecordList == NULL);
		m_pRecordList = new(CPalmPDBRecordList[nNumberOfRecords]);

		for (int nLoopCount = 0; nLoopCount < nNumberOfRecords; nLoopCount++)
		{
			m_pRecordList[nLoopCount].DoInitialiseFromPDB(m_cEndianFile);
		}
		m_cAppInfoRecord.DoInitialiseFromPDB(m_cEndianFile);
	}
	return m_bExists;
}


 // ---------------------------------------------------------------------------
const UInt16 CPalmPDB::DmNumRecordsInCategory(const UInt16 category)
{
	UInt16 nNumRecordsInCategory = 0;
	UInt16 nNumberOfRecords = m_cHeader.GetNumberOfRecords();
	for (int nLoopCount = 0; nLoopCount < nNumberOfRecords; nLoopCount++)
	{
		if (m_pRecordList[nLoopCount].GetCategory() == category)
		{
			nNumRecordsInCategory++;
		}
	}
	return nNumRecordsInCategory;
}


 // ---------------------------------------------------------------------------
UInt16 CPalmPDB::DmGetRecordLength(UInt16 index)
{
	ASSERT(index <= m_cHeader.GetNumberOfRecords());
	if (index < m_cHeader.GetNumberOfRecords() - 1)
	{
		return m_pRecordList[index + 1].GetOffset() - m_pRecordList[index].GetOffset();
	}
	else
	{
		return static_cast<UInt16>(m_cEndianFile.GetLength() - m_pRecordList[index].GetOffset());
	}
}


 // ---------------------------------------------------------------------------
void CPalmPDB::DmNewRecord(UInt16 *atP, UInt32 size)
{
}


 // ---------------------------------------------------------------------------
Err CPalmPDB::DmRemoveRecord(UInt16 index)
{
	Err eError = 0;
//dmErrReadOnly
//dmErrIndexOutOfRange
//dmErrNotRecordDB
//memErrChunkLocked
//memErrInvalidParam

	return eError;
}


 // ---------------------------------------------------------------------------
void CPalmPDB::DmGetRecordField(UInt16 index, void* pBuffer, UInt16 nBytes, UInt16 offsetInRecord)
{
	ASSERT(index <= m_cHeader.GetNumberOfRecords());
	UINT nOffset = m_cEndianFile.Seek(m_pRecordList[index].GetOffset() + offsetInRecord, CFile::begin);
	ASSERT(m_pRecordList[index].GetOffset() + offsetInRecord == nOffset);
	m_cEndianFile.Read(pBuffer, nBytes);
}

 // ---------------------------------------------------------------------------
void CPalmPDB::DmGetRecordFieldNoSeek(UInt16 index, void* pBuffer, UInt16 nBytes)
{
	m_cEndianFile.Read(pBuffer, nBytes);
}


 // ---------------------------------------------------------------------------
void CPalmPDB::DmGetRecord(UInt16 index, void* pBuffer)
{
	ASSERT(index <= m_cHeader.GetNumberOfRecords());
	m_cEndianFile.Seek(m_pRecordList[index].GetOffset(), CFile::begin);
	m_cEndianFile.Read(pBuffer, DmGetRecordLength(index));
}


 // ---------------------------------------------------------------------------
Err	CPalmPDB::DmSeekRecordInCategory(UInt16 *indexP, UInt16 offset, Int16 direction, UInt16 category)
{
	Err eError = 0;
	if (offset >= m_cHeader.GetNumberOfRecords())
	{
		eError = dmErrIndexOutOfRange;
	}
	else
	{
		UInt16 nCurrentRecord = offset;
		while(m_pRecordList[nCurrentRecord].GetCategory() != category)
		{
			nCurrentRecord += direction;
			if (nCurrentRecord == m_cHeader.GetNumberOfRecords())
			{
				eError = dmErrSeekFailed;
				break;
			}
		}
		*indexP = nCurrentRecord;
	}
	return eError;
}


 // ---------------------------------------------------------------------------
UInt16 CPalmPDB::DmPositionInCategory(UInt16 index, UInt16 category)
{
	UInt16 nReturn = 0;
	ASSERT(0);
	return nReturn;
}


 // ---------------------------------------------------------------------------
void CPalmPDB::DumpRecordDetails()
{
#ifdef _DEBUG
	UInt16 nNumberOfRecords = m_cHeader.GetNumberOfRecords();
	UInt32 nRecordCount[16];
	for (int nLoopCount = 0; nLoopCount < 16; nLoopCount++)
	{
		nRecordCount[nLoopCount] = 0;
	}
	for (nLoopCount = 0; nLoopCount < nNumberOfRecords; nLoopCount++)
	{
		nRecordCount[m_pRecordList[nLoopCount].GetCategory()]++;
		afxDump << "Record " << nLoopCount << " Category " << m_pRecordList[nLoopCount].GetCategory() << " Length " << DmGetRecordLength(nLoopCount) << "\n";
	}

	afxDump << "\n";
	for (nLoopCount = 0; nLoopCount < 16; nLoopCount++)
	{
		afxDump << "Category " << nLoopCount << " " << m_cAppInfoRecord.GetCategories()->GetCategoryStr(nLoopCount) << " Size " << nRecordCount[nLoopCount] << "\n";
	}
#endif
}


