
/*
 
	AppPreferences.cpp : implementation file

	(c) Rw Software 2003 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"
#include "VORegistry.h"


// CApplicationPreferences

/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::CApplicationPreferences
	Access:    	public 
	Returns:   	
	Purpose:
------------------------------------------------------------------------------*/
CApplicationPreferences::CApplicationPreferences()
	: m_nMatchSpeed(0)
	, m_bAutosave(false)
{

}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoInitialise
	Access:    	public 
	Parameter: 	CSystemPreferences& _sysPrefs
	Parameter: 	int _strKey
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CApplicationPreferences::DoInitialise(CSystemPreferences& _sysPrefs, const int _KeyID)
{
	DoReadRegistry(_KeyID);
	DoUpdatePath();
#ifdef HANDANGO
	if (_sysPrefs.LoadPrefs(&m_nHandangoRegCode, sizeof(m_nHandangoRegCode)) == false)
	{
		m_nHandangoRegCode = 0;
		_sysPrefs.SavePrefs(&m_nHandangoRegCode, sizeof(m_nHandangoRegCode));
	}
#endif
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetSoundFX
	Access:    	public 
	Returns:   	CSoundVolume&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundVolume& CApplicationPreferences::GetSoundFX()
{
	return m_SoundFX;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetMusic
	Access:    	public 
	Returns:   	CSoundVolume&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundVolume& CApplicationPreferences::GetMusic()
{
	return m_Music;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetMusicInGame
	Access:    	public 
	Returns:   	CSoundVolume&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundVolume& CApplicationPreferences::GetMusicInGame()
{
	return m_MusicInGame;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetMatchSpeed
	Access:    	public const 
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CApplicationPreferences::GetMatchSpeed() const
{
	return m_nMatchSpeed;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoLowerMatchSpeed
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoLowerMatchSpeed()
{
	if (m_nMatchSpeed > 0)
	{
		m_nMatchSpeed--;
		DoUpdateMatchSpeedRegistry();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoRaiseMatchSpeed
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoRaiseMatchSpeed()
{
	if (m_nMatchSpeed < 5)
	{
		m_nMatchSpeed++;
		DoUpdateMatchSpeedRegistry();
	}
}

/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetAppPath
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CApplicationPreferences::GetAppPath()
{
	return m_strAppPath;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::ManagerName
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CApplicationPreferences::GetManagerName()
{
	return m_strManagerName;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoUpdatePath
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Change the registry key under which our settings are stored.
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoUpdatePath()
{
	CString wstrFileName;
	GetModuleFileName(null, wstrFileName.GetBufferSetLength(256), 256);
	wstrFileName.ReleaseBuffer();

	int nLastOccurrence = 1;
	int nCurrentOccurence = 0;
	while(nCurrentOccurence >= 0 && nCurrentOccurence < wstrFileName.GetLength())
	{
		nCurrentOccurence  = wstrFileName.Find(TEXT("\\"), nCurrentOccurence + 1);
		if (nCurrentOccurence > 0)
		{
			nLastOccurrence = nCurrentOccurence;
		}
	}

	CString strDefaultAppPath = wstrFileName.Left(nLastOccurrence);
	CString strDefaultAppEXE = wstrFileName.Right(wstrFileName.GetLength() - (nLastOccurrence +  1));

	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	m_strAppPath = reg.ReadString(TEXT("AppPath"), strDefaultAppPath);
	CString strAppEXE = reg.ReadString(TEXT("AppEXE"), strDefaultAppEXE);

	if (m_strAppPath != strDefaultAppPath)
	{
		m_strAppPath = strDefaultAppPath;
		reg.WriteString(TEXT("AppPath"), m_strAppPath);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoReadRegistry
	Access:    	public 
	Parameter: 	CString strKey
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoReadRegistry(const int _KeyID)
{
	m_strKey.LoadString(_KeyID);
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	m_SoundFX.SetVolume(reg.ReadDWORD(TEXT("SoundFX"), 4));
	m_Music.SetVolume(reg.ReadDWORD(TEXT("Music"), 4));
	m_MusicInGame.SetVolume(reg.ReadDWORD(TEXT("MusicInGame"), 4));
	m_nMatchSpeed = reg.ReadDWORD(TEXT("MatchSpeed"), 3);
	CString strDefaultManagerName = TEXT("The Gaffer");
	m_strManagerName = reg.ReadString(TEXT("ManagerName"), strDefaultManagerName);
	m_bAutosave = static_cast<bool>(reg.ReadDWORD(TEXT("Autosave"), 0));
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::WriteRegistrationCode
	Access:    	public const 
	Parameter: 	const ushort nInputRegCode
	Returns:   	void
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::WriteRegistrationCode(const ushort nInputRegCode) const
{
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("RegistrationCode"), (uint)nInputRegCode);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetRegistrationCode
	Access:    	public 
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CApplicationPreferences::GetRegistrationCode()
{
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	return (ushort)reg.ReadDWORD(TEXT("RegistrationCode"), 0);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetSoundEffectsVolume
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetSoundEffectsVolume(const int _Value)
{
	GetSoundFX().SetVolume(_Value);
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("SoundFX"), _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetSoundFX
	Access:    	public 
	Parameter: 	const uint _nSoundFX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetSoundFX(const uint _nSoundFX)
{
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("SoundFX"), _nSoundFX);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetMenuMusicVolume
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetMenuMusicVolume(const int _Value)
{
	GetMusic().SetVolume(_Value);
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("Music"), _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetMatchSpeed
	Access:    	public 
	Parameter: 	const uint _nMatchSpeed
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetMatchSpeed(const uint _nMatchSpeed)
{
	m_nMatchSpeed = _nMatchSpeed;
	DoUpdateMatchSpeedRegistry();
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoUpdateMatchSpeedRegistry
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoUpdateMatchSpeedRegistry()
{
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("MatchSpeed"), m_nMatchSpeed);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetMusicInGameVolume
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetMusicInGameVolume(const int _Value)
{
	GetMusicInGame().SetVolume(_Value);
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("MusicInGame"), _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetAutosave
	Access:    	public 
	Parameter: 	const bool _bAutosave
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetAutosave(const bool _bAutosave)
{
	m_bAutosave = _bAutosave;
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteDWORD(TEXT("Autosave"), static_cast<DWORD>(m_bAutosave));
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetManagerName
	Access:    	public 
	Parameter: 	CString& _strManagerName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetManagerName(CString& _strManagerName)
{
	m_strManagerName = _strManagerName;
	CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
	reg.WriteString(TEXT("ManagerName"), m_strManagerName);
}


#ifdef HANDANGO
/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::GetHandangoRegCode
	Access:    	public const 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CApplicationPreferences::GetHandangoRegCode() const
{
	return m_nHandangoRegCode;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::SetHandangoRegCode
	Access:    	public 
	Parameter: 	const ushort nHandangoRegCode
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::SetHandangoRegCode(const ushort nHandangoRegCode)
{
	m_nHandangoRegCode = nHandangoRegCode;
}
#endif
