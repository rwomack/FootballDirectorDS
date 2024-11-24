
/********************************************************************
	created:	2003/06/24
	created:	24:6:2003   14:23
	filename: 	D:\PROJECTS\system\Categories.cpp
	file path:	D:\PROJECTS\system
	file base:	Categories
	file ext:	cpp
	author:		Roger Womack
	
	purpose:	
*********************************************************************/


#include "stdafx.h"
#include "Categories.h"


/////////////////////////////////////////////////////////////////////////////
// CCategories


 // ---------------------------------------------------------------------------
void CCategories::Write(CFile& file)
{
	file.Write(&m_structCategories, sizeof(structCategories));
}


 // ---------------------------------------------------------------------------
void CCategories::Read(CFile& file)
{
	file.Read(&m_structCategories, sizeof(structCategories));
}


 // ---------------------------------------------------------------------------
void CCategories::SetCategoryStr(UInt16 nCategoryId, CString strCategory)
{
	ASSERT(nCategoryId < dmRecNumCategories);
	wcstombs(pCategory(nCategoryId), strCategory, dmCategoryLength - 1);
}


 // ---------------------------------------------------------------------------
UInt16 CCategories::DoCountCategoriesUsed()
{
	UInt16 nCountUsedCategories = 0;
	for (int n = 0; n < dmRecNumCategories; n++)
	{
		if (strlen(pCategory(n)) != 0)
		{
			nCountUsedCategories++;
		}
	}
	return nCountUsedCategories;
}


 // ---------------------------------------------------------------------------
UInt16 CCategories::DoFindCategoryStr(CString strCategory)
{
	UInt16 nFirstCategory = 0;
	char szToFind[dmCategoryLength];
	wcstombs(szToFind, strCategory, dmCategoryLength - 1);
	for (int n = 0; n < dmRecNumCategories; n++)
	{
//		CString strCat;
//		strCat.Format(TEXT("Seeking %s, Found %s %d"), pCategory(n), szToFind, strcmp(pCategory(n), szToFind));
//		AfxMessageBox(strCat);
		if (strcmp(pCategory(n), szToFind) == 0)
		{
			break;
		}
		nFirstCategory++;
	}
	return nFirstCategory;
}


 // ---------------------------------------------------------------------------
char*	CCategories::pCategory(UInt16 nCategoryId)
{
	switch(nCategoryId)
	{
		case 0:
			return m_structCategories.m_szCategoryName1;
		case 1:
			return m_structCategories.m_szCategoryName2;
		case 2:
			return m_structCategories.m_szCategoryName3;
		case 3:
			return m_structCategories.m_szCategoryName4;
		case 4:
			return m_structCategories.m_szCategoryName5;
		case 5:
			return m_structCategories.m_szCategoryName6;
		case 6:
			return m_structCategories.m_szCategoryName7;
		case 7:
			return m_structCategories.m_szCategoryName8;
		case 8:
			return m_structCategories.m_szCategoryName9;
		case 9:
			return m_structCategories.m_szCategoryName10;
		case 10:
			return m_structCategories.m_szCategoryName11;
		case 11:
			return m_structCategories.m_szCategoryName12;
		case 12:
			return m_structCategories.m_szCategoryName13;
		case 13:
			return m_structCategories.m_szCategoryName14;
		case 14:
			return m_structCategories.m_szCategoryName15;
		case 15:
			return m_structCategories.m_szCategoryName16;
		default:
			ASSERT(0);
	}
	return NULL;
}


 // ---------------------------------------------------------------------------
bool CCategories::DoAddCategoryStr(CString strCategory)
{
	bool bRet = false;
	char szCategory[dmCategoryLength];
	wcstombs(szCategory, strCategory, dmCategoryLength - 1);

	UInt16 nFirstFreeCategory = dmRecNumCategories;
	for (int nLoopCount = 0; nLoopCount < dmRecNumCategories; nLoopCount++)
	{
		if (strlen(pCategory(nLoopCount)) == 0 && nFirstFreeCategory == dmRecNumCategories)
		{
			// We can use this one
			nFirstFreeCategory = nLoopCount;
			// Continue, to ensure it is not a duplicate
		}
		else
		{
			// Check for duplicate being added
			if (strcmp(szCategory, pCategory(nLoopCount)) == 0)
			{
				nFirstFreeCategory = dmRecNumCategories;
				break;
			}
		}
	}
	if (nFirstFreeCategory != dmRecNumCategories)
	{
		SetCategoryStr(nFirstFreeCategory, strCategory);
		bRet = true;
	}
	return bRet;
}


 // ---------------------------------------------------------------------------
void CCategories::DoCreate()
{
	// Empty the category strings
	for (int nCount = 1; nCount < dmRecNumCategories; nCount++)
	{
		SetCategoryStr(nCount, TEXT(""));
	}
}


 // ---------------------------------------------------------------------------
CString	CCategories::GetCategoryStr(UInt16 nCategoryId)
{
	ASSERT(nCategoryId < dmRecNumCategories);
	CString szCategory(pCategory(nCategoryId));
	return szCategory;
}
/////////////////////////////////////////////////////////////////////////////


 // ---------------------------------------------------------------------------
void CCeDBDatabaseCategories::Write(CCeDBDatabase& dbCellar)
{
	CCeDBProp propRec[2];

	propRec[0].SetLong(0xffff);
	propRec[0].SetIdent(0);
	propRec[0].SetSortFlags(CCeDBProp::Sort_Ascending);
	dbCellar.SeekFirstEqual(propRec[0]);
	
	CEBLOB	blobCategories;
	blobCategories.dwCount = sizeof(m_structCeDBDatabaseCategories);
	m_structCeDBDatabaseCategories.m_stCategories = CCategories::m_structCategories;
	blobCategories.lpb = (BYTE*)&m_structCeDBDatabaseCategories;

	propRec[0] = CCeDBProp((LONG)CATINFO, PROP_CATEGORY);			// Unique category ID
	propRec[1] = CCeDBProp(blobCategories, PROP_BLOB);		// Text

	CCeDBRecord	recCategory;
	recCategory.AddProps(propRec, 2);
	dbCellar.WriteCurrRecord(&recCategory);
}


 // ---------------------------------------------------------------------------
void CCeDBDatabaseCategories::DoInitialise(structCeDBDatabaseCategories* pCEDBDatabaseCategories)
{
	m_structCeDBDatabaseCategories = *pCEDBDatabaseCategories;
	CCategories::m_structCategories = m_structCeDBDatabaseCategories.m_stCategories;
}


 // ---------------------------------------------------------------------------
UInt32 CCeDBDatabaseCategories::DoCreateUniqueDataRecordID(CCeDBDatabase& dbCellar)
{
	UInt32 nRet = m_structCeDBDatabaseCategories.m_nlastUniqDataRecID++;
	Write(dbCellar);
	return nRet;
}


 // ---------------------------------------------------------------------------
bool CCeDBDatabaseCategories::DoAddCategoryStr(CCeDBDatabase& dbCellar, CString strCategory)
{
	bool bRet = CCategories::DoAddCategoryStr(strCategory);
	Write(dbCellar);
	return bRet;
}


 // ---------------------------------------------------------------------------
void CCeDBDatabaseCategories::SetCategoryStr(CCeDBDatabase& dbCellar, UInt16 nCategoryId, CString strCategory)
{
	CCategories::SetCategoryStr(nCategoryId, strCategory);
	Write(dbCellar);
}


 // ---------------------------------------------------------------------------
void CCeDBDatabaseCategories::DoCreate(CCeDBDatabase& dbCellar)
{
	m_structCeDBDatabaseCategories.m_nlastUniqDataRecID = 0;
	CCategories::DoCreate();
	// Create the default categories
	int nCategoryID[] =
	{
		IDS_UNFILEDWINES, IDS_HOMECELLAR,
		IDS_OFFICECELLAR, IDS_TASTINGHISTORY,
		IDS_WINESILIKE, IDS_WINESTOBUY
	};
	
	CString strCategory;
	strCategory.LoadString(IDS_kEdChoiceChoiceStr);
	for (int nCount = 0; nCount < 6; nCount++)
	{
		strCategory.LoadString(nCategoryID[nCount]);
		CCategories::DoAddCategoryStr(strCategory);
	}

//	strCategory = TEXT("Test Category");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 1");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 2");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 3");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 4");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 5");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 6");
//	CCategories::DoAddCategoryStr(strCategory);
//	strCategory = TEXT("Test Category 7");
//	CCategories::DoAddCategoryStr(strCategory);

	CEBLOB	blobCategories;
	blobCategories.dwCount = sizeof(m_structCeDBDatabaseCategories);
	m_structCeDBDatabaseCategories.m_stCategories = CCategories::m_structCategories;
	blobCategories.lpb = (BYTE*)&m_structCeDBDatabaseCategories;

	CCeDBProp propRec[2];
	propRec[0] = CCeDBProp((LONG)CATINFO, PROP_CATEGORY);	// Category ID
	propRec[1] = CCeDBProp(blobCategories, PROP_BLOB);		

	CCeDBRecord	recCategory;
	recCategory.AddProps(propRec, 2);
	dbCellar.AddRecord(&recCategory);
}
