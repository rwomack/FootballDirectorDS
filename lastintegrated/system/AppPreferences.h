
/*

	AppPreferences.h: interface for the CApplicationPreferences class.

	(c) Rw Software 2004- 2008

  ---------------------------------------------------------------
*/

#ifndef __APPLICATIONPREFERENCES_H__
#define __APPLICATIONPREFERENCES_H__


class CSoundVolume;

class CApplicationPreferences
{

public:
	CApplicationPreferences();

	bool					DoInitialise(CSystemPreferences& _sysPrefs, const int _KeyID);
	void					DoLowerMatchSpeed();
	void					DoRaiseMatchSpeed();
	void					DoUpdatePath();
	void					DoReadRegistry(const int _KeyID);
	void					DoWriteRegistry(CString strKey);
	void					DoUpdateMatchSpeedRegistry();
	CString&				GetAppPath();
	uint					GetMatchSpeed() const;
	ushort					GetRegistrationCode();
	CString&				GetManagerName();
	CSoundVolume&			GetSoundFX();
	CSoundVolume&			GetMusic();
	CSoundVolume&			GetMusicInGame();
	void					SetAutosave(const bool _bAutosave);
	void					SetMatchSpeed(const uint _nMatchSpeed);
	void					SetManagerName(CString& _strManagerName);
	void					SetSoundFX(const uint _nSoundFX);
	void					SetMusic(const uint _Music);
	void					SetMusicInGame(const uint _Music);
	void 					SetSoundEffectsVolume(const int _Value);
	void 					SetMenuMusicVolume(const int _Value);
	void 					SetMusicInGameVolume(const int _Value);
	void					WriteRegistrationCode(const ushort nInputRegCode) const;

#ifdef HANDANGO
	ushort					GetHandangoRegCode() const;
	void					SetHandangoRegCode(const ushort nHandangoRegCode);
#endif

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
