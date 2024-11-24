// PalmPDBHeader.h

#if !defined(PALMPDBHEADER_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
#define PALMPDBHEADER_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_

#define FLIPNIBBLE(x) (((x & 0xf0 >> 4) | (x & 0x0f << 4)))
#define FLIPWORD(x) ((FLIPNIBBLE((x & 0xFF00) >> 8 )) | ((FLIPNIBBLE(x & 0xFF)) << 8 ))
#define FLIPLONG(x) (((FLIPWORD((x & 0xFFFF0000) >> 16 ))) | ((FLIPWORD(x & 0xFFFF) << 16 )))

class CEndianFile : public CFile
{
public:

	UInt16			IntRead(void)
	{
		UInt16 nInt16;
		CFile::Read(&nInt16, 16);
		nInt16 = (UInt16)FLIPWORD(nInt16);
		return nInt16;
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CPalmPDBHeader


class CPalmPDBHeader
{
// Construction
public:
	CPalmPDBHeader() {};
	~CPalmPDBHeader() {};

	void					DoInitialiseFromPDB(CEndianFile& file);
	inline const UInt16		GetNumberOfRecords(void)
	{
		return m_nCount;
	}

protected:
	BYTE					m_szName[32];
	UInt16					m_nFlags;
	UInt16					m_nVersion;
	UInt32					m_nCreationTime;
	UInt32					m_nModificationTime;
	UInt32					m_nBackupTime;
	UInt32					m_nModificationNumber;
	UInt32					m_nAppInfo;
	UInt32					m_nSortInfo;
	UInt32					m_nType;
	UInt32					m_nCreator;
	UInt32					m_nUniqueID;
	UInt32					m_nNextRecordList;
	UInt16					m_nCount;
};


/////////////////////////////////////////////////////////////////////////////
// CPalmPDBRecordList


class CPalmPDBRecordList
{
// Construction
public:
	CPalmPDBRecordList() {};
	~CPalmPDBRecordList() {};

	void					DoInitialiseFromPDB(CEndianFile& file);
	UInt8					GetCategory()
	{
		return(m_nAttributes & dmRecAttrCategoryMask);
	}
	UInt32					GetOffset()
	{
		return m_nOffset;
	}

protected:
	UInt32					m_nOffset;
	UInt8					m_nAttributes;
	UInt24					m_nId;
};


/////////////////////////////////////////////////////////////////////////////
// CPalmPDBappInfoRecord

class CCategories;

class CPalmPDBappInfoRecord
{
// Construction
public:
	CPalmPDBappInfoRecord()
	{
		m_nFileOffset = 0;
	};
	~CPalmPDBappInfoRecord() {};

	void					DoInitialiseFromPDB(CEndianFile& file);
	void					DoSaveToPDB(CEndianFile& file);
	CCategories*			GetCategories()
	{
		return &m_Categories;
	}

protected:
	UInt32					m_nFileOffset;
	UInt16					m_nBitField;
	CCategories				m_Categories;
	UInt8					m_nUnique[dmRecNumCategories];
	UInt8					m_nLastCategoryUsed;
};

/////////////////////////////////////////////////////////////////////////////


class CWineGuidePalmPDBappInfoRecord : public CPalmPDBappInfoRecord
{
// Construction
public:
	CWineGuidePalmPDBappInfoRecord()
	{
		m_nFileOffset = 0;
	};
	~CWineGuidePalmPDBappInfoRecord() {};

	void					DoInitialiseFromPDB(CEndianFile& file);
	void					DoSaveToPDB(CEndianFile& file);

	UInt16					GetVersionNumber(void)
	{
		return m_stEditionType.wVersion;
	}

	CString					GetEditionStr(void)
	{
		CString strRet(m_stEditionType.EditionStr);
		return strRet;
	}
protected:
	UInt8					m_nReserved1;
	UInt16					m_nReserved2;
	UInt16					m_nVarietalIndexStart;
	UInt16					m_nNumWineRecords;
	WineGuideEditionType	m_stEditionType;
};

/////////////////////////////////////////////////////////////////////////////
// CPalmPDB


class CPalmPDB
{
public:
	CPalmPDB ()
	{
		m_pRecordList = NULL;
		m_bExists = false;
	};
	~CPalmPDB ()
	{
		if (m_bExists == true)
		{
			delete []m_pRecordList;
			m_cEndianFile.Close();
		}
	};

	void					DoDestroyData(void);
	bool					DoInitialise(CString strFileAndPathName, UINT nOpenFlags = CFile::modeRead);
	void					DoSaveToPDB(void)
	{
		m_cAppInfoRecord.DoSaveToPDB(m_cEndianFile);
	}
	void					DmNewRecord(UInt16 *atP, UInt32 size);
	Err						DmRemoveRecord(UInt16 index);
	const UInt16			DmNumRecordsInCategory(const UInt16 category);
	void					DmGetRecord(UInt16 index, void* pBuffer);
	void					DmGetRecordFieldNoSeek(UInt16 index, void* pBuffer, UInt16 nBytes);
	void					DmGetRecordField(UInt16 index, void* pBuffer, UInt16 nBytes, UInt16 offset = 0);
	UInt16					DmGetRecordLength(UInt16 index);
	UInt16					DmPositionInCategory(UInt16 index, UInt16 category);
	void					DmQueryRecord(UInt16 index, void* pBuffer)
	{
		DmGetRecord(index, pBuffer);
	}
	Err						DmReleaseRecord(UInt16 index, Boolean dirty)
	{
		return 0;
	}
	Err						DmSeekRecordInCategory(UInt16 *indexP, UInt16 offset, Int16 direction, UInt16 category);
	void					DumpRecordDetails(void);
	const UInt8				GetCategory(const UInt16 RecordIndexNum) const
	{
		return m_pRecordList[RecordIndexNum].GetCategory();
	}
	
	const UInt16			GetSize(void)
	{
		return m_cHeader.GetNumberOfRecords();
	}
	
	CPalmPDBappInfoRecord&	GetPDBappInfoRecord(void)
	{
		return m_cAppInfoRecord;
	}
	
	CEndianFile*			pFile(void)
	{
		return &m_cEndianFile;
	}

	bool					GetExists(void) const
	{
		return m_bExists;
	}

	void					SetExists(const bool bExists)
	{
		m_bExists = bExists;
	}

	UInt16					GetVersionNumber(void)
	{
		return m_cAppInfoRecord.GetVersionNumber();
	}

	CString					GetEditionStr(void)
	{
		return m_cAppInfoRecord.GetEditionStr();
	}

protected:
	CPalmPDBHeader			m_cHeader;
	CPalmPDBRecordList*		m_pRecordList;
	CWineGuidePalmPDBappInfoRecord	m_cAppInfoRecord;
    CEndianFile				m_cEndianFile;
	bool					m_bExists;
};
#endif
