// SystemPreferences.cpp: implementation of the CSystemPreferences class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemPreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------
CSystemPreferences::CSystemPreferences()
{
#ifdef POCKETPC	
	wchar_t OEMInfo[512];
	SystemParametersInfo(SPI_GETOEMINFO, sizeof(OEMInfo), &OEMInfo ,0);
    m_DeviceName = OEMInfo;

	HKEY hKey;
	DWORD dwLen, dwType;
	wchar_t wszOwnerInfo[512];
	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("ControlPanel\\Owner"), 0, 0, &hKey) == ERROR_SUCCESS)
	{
        if (RegQueryValueEx(hKey, TEXT("Owner"), 0, &dwType, null, &dwLen) == ERROR_SUCCESS)
		{
			RegQueryValueEx(hKey, TEXT("Owner"), 0, &dwType, (BYTE*)&wszOwnerInfo, &dwLen);
			wcstombs(m_szUserName, wszOwnerInfo, sizeof(wszOwnerInfo));
			m_UserName = wszOwnerInfo;
			if (m_UserName.GetLength() > 0)
			{
				m_UserNamePresent = true;
			}
         }
        RegCloseKey(hKey);
  }
#endif
}


//---------------------------------------------------------------------------------------
CSystemPreferences::~CSystemPreferences()
{

}


//---------------------------------------------------------------------------------------
bool CSystemPreferences::Read(wchar_t* wszKey, VOID *Prefs, int Size)
{
#ifdef POCKETPC	
	HKEY		MyKey;
    DWORD		Len = Size;
           
    // Try to open and read the data
#ifdef TARGET_SIMULATOR
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName, 0, KEY_ALL_ACCESS, &MyKey) == ERROR_SUCCESS)
#else
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName, 0, 0, &MyKey) == ERROR_SUCCESS)
#endif
	{
		return false;
	}
	RegCloseKey(MyKey);
#endif
	return true;
}


//---------------------------------------------------------------------------------------
bool CSystemPreferences::LoadPrefs(VOID *Prefs, int Size)
{
#ifdef POCKETPC	
	HKEY		MyKey;
    DWORD		Len = Size;
           
    // Try to open and read the data
#ifdef TARGET_SIMULATOR
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName, 0, KEY_ALL_ACCESS, &MyKey) == ERROR_SUCCESS)
#else
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName, 0, 0, &MyKey) == ERROR_SUCCESS)
#endif
	{
		return false;
	}
	RegCloseKey(MyKey);
#endif
	return true;
}


//---------------------------------------------------------------------------------------
bool CSystemPreferences::SavePrefs(VOID *Prefs, int Size)
{
#ifdef POCKETPC	
	HKEY  MyKey;
	uint  rammel = 0;

#ifdef TARGET_SIMULATOR
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName, 0, KEY_ALL_ACCESS, &MyKey) != ERROR_SUCCESS)
#else
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_wszName,0, 0, &MyKey) != ERROR_SUCCESS)
#endif             
    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, m_wszName,0,TEXT("DATA"),REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,null,&MyKey,(LPDWORD)&rammel)!=ERROR_SUCCESS)
	{
		return false;
	}
 
	rammel = RegSetValueEx(MyKey,TEXT("PREFS"),0,REG_BINARY,(CONST BYTE *)Prefs, Size);
	RegCloseKey(MyKey);
 
#ifdef TARGET_BE300
	CGDFlushRegistry();
#endif
#endif 
	// Success
	return true;
}
