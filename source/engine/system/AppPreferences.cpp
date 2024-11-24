
/*

	AppPreferences.cpp : implementation file

	(c) RW Software 2003

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
{};


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoInitialise
	Access:    	public
	Parameter: 	CSystemPreferences& _sysPrefs
	Parameter: 	CString _strKey
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CApplicationPreferences::DoInitialise(CSystemPreferences& _sysPrefs, CString _strKey)
{
	DoReadRegistry(_strKey);
	DoUpdatePath();
	if (_sysPrefs.LoadPrefs(&m_nHandangoRegCode, sizeof(m_nHandangoRegCode)) == false)
	{
		m_nHandangoRegCode = 0;
		_sysPrefs.SavePrefs(&m_nHandangoRegCode, sizeof(m_nHandangoRegCode));
	}
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
CString& CApplicationPreferences::ManagerName()
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
//TODO  CString wstrFileName;
//TODO  GetModuleFileName(null, wstrFileName.GetBufferSetLength(256), 256);
//TODO  wstrFileName.ReleaseBuffer();
//TODO
//TODO  int nLastOccurrence = 1;
//TODO  int nCurrentOccurence = 0;
//TODO  while(nCurrentOccurence >= 0 && nCurrentOccurence < wstrFileName.GetLength())
//TODO  {
//TODO    nCurrentOccurence  = wstrFileName.Find(TEXT("\\"), nCurrentOccurence + 1);
//TODO    if (nCurrentOccurence > 0)
//TODO    {
//TODO      nLastOccurrence = nCurrentOccurence;
//TODO    }
//TODO  }
//TODO
//TODO  CString strDefaultAppPath = wstrFileName.Left(nLastOccurrence);
//TODO  CString strDefaultAppEXE = wstrFileName.Right(wstrFileName.GetLength() - (nLastOccurrence +  1));
//TODO
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  m_strAppPath = reg.ReadString(TEXT("AppPath"), strDefaultAppPath);
//TODO  CString strAppEXE = reg.ReadString(TEXT("AppEXE"), strDefaultAppEXE);
//TODO
//TODO  if (m_strAppPath != strDefaultAppPath)
//TODO  {
//TODO    m_strAppPath = strDefaultAppPath;
//TODO    reg.WriteString(TEXT("AppPath"), m_strAppPath);
//TODO  }
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoReadRegistry
	Access:    	public
	Parameter: 	CString strKey
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoReadRegistry(CString strKey)
{
	m_strKey = strKey;
//TODO  CVORegistry reg(HKEY_CURRENT_USER, strKey);
//TODO  m_SoundFX.SetVolume(reg.ReadDWORD(TEXT("SoundFX"), 4));
//TODO  m_Music.SetVolume(reg.ReadDWORD(TEXT("Music"), 4));
//TODO  m_MusicInGame.SetVolume(reg.ReadDWORD(TEXT("MusicInGame"), 4));
//TODO  m_nMatchSpeed = reg.ReadDWORD(TEXT("MatchSpeed"), 3);
  m_nMatchSpeed = 3;
//TODO  CString strDefaultManagerName = TEXT("The Gaffer");
//TODO  m_strManagerName = reg.ReadString(TEXT("ManagerName"), strDefaultManagerName);
  m_strManagerName = TEXT("The Gaffer"); //tmp
//TODO  m_bAutosave = static_cast<bool>(reg.ReadDWORD(TEXT("Autosave"), 0));
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("RegistrationCode"), (uint)nInputRegCode);
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  return (ushort)reg.ReadDWORD(TEXT("RegistrationCode"), 0);
  return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoLowerSoundFXVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoLowerSoundFXVolume()
{
	GetSoundFX().DoLowerVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("SoundFX"), GetSoundFX().GetVolume());
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoRaiseSoundFXVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoRaiseSoundFXVolume()
{
	GetSoundFX().DoRaiseVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("SoundFX"), GetSoundFX().GetVolume());
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("SoundFX"), _nSoundFX);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoLowerMusicVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoLowerMusicVolume()
{
	GetMusic().DoLowerVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("Music"), GetMusic().GetVolume());
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoRaiseMusicVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoRaiseMusicVolume()
{
	GetMusic().DoRaiseVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("Music"), GetMusic().GetVolume());
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("MatchSpeed"), m_nMatchSpeed);
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoLowerMusicInGameVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoLowerMusicInGameVolume()
{
	GetMusicInGame().DoLowerVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("MusicInGame"), GetMusicInGame().GetVolume());
}


/*------------------------------------------------------------------------------
	Method:   	CApplicationPreferences::DoRaiseMusicInGameVolume
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CApplicationPreferences::DoRaiseMusicInGameVolume()
{
	GetMusicInGame().DoRaiseVolume();
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("MusicInGame"), GetMusicInGame().GetVolume());
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteDWORD(TEXT("Autosave"), static_cast<DWORD>(m_bAutosave));
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
//TODO  CVORegistry reg(HKEY_CURRENT_USER, m_strKey);
//TODO  reg.WriteString(TEXT("ManagerName"), m_strManagerName);
}
