// SoundManager.cpp: implementation of the CSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "UI.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifdef FMOD


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::CSoundManager
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundManager::CSoundManager()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::~CSoundManager
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundManager::~CSoundManager()
{
	FSOUND_Sample_Free(m_pSound[SOUND_CHEER]);
	FSOUND_Sample_Free(m_pSound[SOUND_CROWDLRG]);
	FSOUND_Sample_Free(m_pSound[SOUND_CROWDMED]);
	FSOUND_Sample_Free(m_pSound[SOUND_CROWDSML]);
	FSOUND_Sample_Free(m_pSound[SOUND_FULLWHST]);
	FSOUND_Sample_Free(m_pSound[SOUND_GOALCHEE]);
	FSOUND_Sample_Free(m_pSound[SOUND_MISS]);
	FSOUND_Sample_Free(m_pSound[SOUND_ROAR]);
	FSOUND_Sample_Free(m_pSound[SOUND_WHISTLE]);
	FSOUND_Sample_Free(m_pSound[SOUND_BEEP]);
	FSOUND_Sample_Free(m_pSound[SOUND_BOO]);

	FMUSIC_StopSong(m_pMusic);
	FMUSIC_FreeSong(m_pMusic);

	FSOUND_Close();
}


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundManager::DoInitialise()
{
	FSOUND_Init(22050, 32, 0);
	
	m_pMusic = FMUSIC_LoadSong(theApp.szAddPath("FD_theme.xm"));
	EXT_ASSERT(m_pMusic != null);
	FMUSIC_SetMasterVolume(m_pMusic, theApp.GetApplicationPreferences().GetMusic().GetVolumeLevel());
	FMUSIC_PlaySong(m_pMusic);

	m_pSound[SOUND_CHEER] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Cheer.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_CHEER] != null);
	m_pSound[SOUND_CROWDLRG] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("CrowdLrg.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_CROWDLRG] != null);
	m_pSound[SOUND_CROWDMED] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("CrowdMed.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_CROWDMED] != null);
	m_pSound[SOUND_CROWDSML] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("CrowdSml.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_CROWDSML] != null);
	m_pSound[SOUND_FULLWHST] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("FullWhst.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_FULLWHST] != null);
	m_pSound[SOUND_GOALCHEE] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("GoalChee.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_GOALCHEE] != null);
	m_pSound[SOUND_MISS] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Miss.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_MISS] != null);
	m_pSound[SOUND_ROAR] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Roar.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_ROAR] != null);
	m_pSound[SOUND_WHISTLE] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Whistle.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_WHISTLE] != null);
	m_pSound[SOUND_BOO] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Boo.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_BOO] != null);
	m_pSound[SOUND_BEEP] = FSOUND_Sample_Load(FSOUND_FREE, theApp.szAddPath("Button2.wav"), 0, 0, 0);
	EXT_ASSERT(m_pSound[SOUND_BEEP] != null);
}


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::DoPlaySoundFX
	Access:    	public 
	Parameter: 	const eSOUNDFX theFX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundManager::DoPlaySoundFX(const eSOUNDFX theFX)
{
	EXT_ASSERT(m_pSound[theFX] != null);
	FSOUND_PlaySound(FSOUND_FREE, m_pSound[theFX]);
}


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::SetMasterVolume
	Access:    	public 
	Parameter: 	const int nVolume
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundManager::SetMasterVolume(const int nVolume)
{
	FMUSIC_SetMasterVolume(pMusic(), nVolume);
}


/*------------------------------------------------------------------------------
	Method:   	CSoundManager::SetSFXMasterVolume
	Access:    	public 
	Parameter: 	const int nVolume
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundManager::SetSFXMasterVolume(const int nVolume)
{
	FSOUND_SetSFXMasterVolume(nVolume);
}
#endif
