// SystemPreferences.h: interface for the CSystemPreferences class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMPREFERENCES_H__C63FE942_3436_4CBE_BBE4_6EFE7C3B1D6C__INCLUDED_)
#define AFX_SYSTEMPREFERENCES_H__C63FE942_3436_4CBE_BBE4_6EFE7C3B1D6C__INCLUDED_


class CSystemPreferences  
{
public:
	CSystemPreferences();
	virtual ~CSystemPreferences();

	CString					GetSaveDirectory();
	char*					GetUserName();
	bool					LoadPrefs(void* Prefs, int Size);
	bool					Read(wchar_t* wszKey, void* Prefs, int Size);
	bool					SavePrefs(void* Prefs, int Size);

protected:
    wchar_t					m_wszName[512];
	CString					m_DeviceName;
	bool					m_UserNamePresent;
	CString					m_UserName;
	char					m_szUserName[256];
};

#endif // !defined(AFX_SYSTEMPREFERENCES_H__C63FE942_3436_4CBE_BBE4_6EFE7C3B1D6C__INCLUDED_)
