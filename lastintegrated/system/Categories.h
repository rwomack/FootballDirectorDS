/********************************************************************
	created:	24:6:2003   14:35
	filename: 	D:\PROJECTS\system\Categories.h
	file path:	D:\PROJECTS\system
	file base:	Categories
	file ext:	h
	author:		Roger Womack

	(c) RW Software 2003
		
	purpose:	
*********************************************************************/
// CCategories

#if !defined(CATEGORIES_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
#define CATEGORIES_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_

typedef struct 
{
	char					m_szCategoryName1[dmCategoryLength];
	char					m_szCategoryName2[dmCategoryLength];
	char					m_szCategoryName3[dmCategoryLength];
	char					m_szCategoryName4[dmCategoryLength];
	char					m_szCategoryName5[dmCategoryLength];
	char					m_szCategoryName6[dmCategoryLength];
	char					m_szCategoryName7[dmCategoryLength];
	char					m_szCategoryName8[dmCategoryLength];
	char					m_szCategoryName9[dmCategoryLength];
	char					m_szCategoryName10[dmCategoryLength];
	char					m_szCategoryName11[dmCategoryLength];
	char					m_szCategoryName12[dmCategoryLength];
	char					m_szCategoryName13[dmCategoryLength];
	char					m_szCategoryName14[dmCategoryLength];
	char					m_szCategoryName15[dmCategoryLength];
	char					m_szCategoryName16[dmCategoryLength];
} structCategories;

typedef struct 
{
	UInt32					m_nlastUniqDataRecID; 
	structCategories		m_stCategories;
} structCeDBDatabaseCategories;

class CCategories
{
// Construction
public:
	CCategories() {};
	~CCategories() {};

	void					Read(CFile& file);
	void					Write(CFile& file);

	structCategories*		GetStruct(void)
	{
		return &m_structCategories;
	}
	UInt16					DoCountCategoriesUsed(void);
	void					DoCreate(void);
	UInt16					DoFindCategoryStr(CString strCategory);
	bool					DoAddCategoryStr(CString strCategory);
	CString					GetCategoryStr(UInt16 nCategoryId);
	void					SetCategoryStr(UInt16 nCategoryId, CString strCategory);
	char*					pCategory(UInt16 nCategoryId);

protected:
	structCategories		m_structCategories;
};
/////////////////////////////////////////////////////////////////////////////

class CCeDBDatabaseCategories : public CCategories
{
public:
	CCeDBDatabaseCategories() {}; 
	~CCeDBDatabaseCategories() {};

	void					Write(CCeDBDatabase& dbCellar);

	void					DoInitialise(structCeDBDatabaseCategories* pCEDBDatabaseCategories);
	void					DoCreate(CCeDBDatabase& dbCellar);
	UInt32					DoCreateUniqueDataRecordID(CCeDBDatabase& dbCellar);
	bool					DoAddCategoryStr(CCeDBDatabase& dbCellar, CString strCategory);
	void					SetCategoryStr(CCeDBDatabase& dbCellar, UInt16 nCategoryId, CString strCategory);

protected:
	structCeDBDatabaseCategories m_structCeDBDatabaseCategories;
};
#endif