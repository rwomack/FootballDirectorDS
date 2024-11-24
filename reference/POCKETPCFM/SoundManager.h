// SoundManager.h: interface for the CSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDMANAGER_H__0E2C9F96_5464_44EF_B3EC_74CAA92B6967__INCLUDED_)
#define AFX_SOUNDMANAGER_H__0E2C9F96_5464_44EF_B3EC_74CAA92B6967__INCLUDED_

#ifdef FMOD

class CSoundManager  
{
public:
	CSoundManager();
	virtual ~CSoundManager();

	void					DoInitialise();
	void					DoPlaySoundFX(const eSOUNDFX theFX);
	void					SetMasterVolume(const int nVolume);
	void					SetSFXMasterVolume(const int nVolume);
	FMUSIC_MODULE*			pMusic()
	{
		return m_pMusic;
	}

protected:
	FMUSIC_MODULE*			m_pMusic;
	FSOUND_SAMPLE*			m_pSound[SOUND_MAX];
};
#endif // FMOD
#endif // !defined(AFX_SOUNDMANAGER_H__0E2C9F96_5464_44EF_B3EC_74CAA92B6967__INCLUDED_)
