
/*

	AppPreferences.h: interface for the CApplicationPreferences class.

	(c) Rw Software 2004-2007

  ---------------------------------------------------------------
*/

#ifndef __APPLICATIONPREFERENCES_H__
#define __APPLICATIONPREFERENCES_H__


class CSoundVolume;

class CApplicationPreferences
{

public:
	CApplicationPreferences();

	bool					DoInitialise(CSystemPreferences& _sysPrefs, CString _strKey);
	void					DoUpdatePath();
	void					DoReadRegistry(CString strKey);
	void					DoWriteRegistry(CString strKey);
	void					DoUpdateMatchSpeedRegistry();
	ushort					GetHandangoRegCode() const;
	void					DoRaiseSoundFXVolume();
	void					DoLowerSoundFXVolume();
	void					DoLowerMusicVolume();
	void					DoRaiseMusicVolume();
	void					DoRaiseMusicInGameVolume();
	void					DoLowerMusicInGameVolume();
	void					SetHandangoRegCode(const ushort nHandangoRegCode);
	CString&				GetAppPath();
	uint					GetMatchSpeed() const;
	void					SetMatchSpeed(const uint _nMatchSpeed);
	void					WriteRegistrationCode(const ushort nInputRegCode) const;
	ushort					GetRegistrationCode();
	CString&				ManagerName();
	void					SetAutosave(const bool _bAutosave);
	void					SetManagerName(CString& _strManagerName);
	void					DoLowerMatchSpeed();
	void					DoRaiseMatchSpeed();
	CSoundVolume&			GetSoundFX();
	CSoundVolume&			GetMusic();
	CSoundVolume&			GetMusicInGame();
	void					SetSoundFX(const uint _nSoundFX);
	void					setMusic(const uint nMusic);
	void					setMusicInGame(const uint nMusic);

protected:
	CSoundVolume			m_SoundFX;
	CSoundVolume			m_Music;
	CSoundVolume			m_MusicInGame;
	uint					m_nMatchSpeed;
	CString					m_strKey;
	ushort					m_nHandangoRegCode;
	CString					m_strAppPath;
	CString					m_strManagerName;
	bool					m_bAutosave;
};

#endif
