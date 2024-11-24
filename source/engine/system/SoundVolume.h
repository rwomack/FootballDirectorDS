
/*

	SoundVolume.h: interface for the CSoundVolume class.

	(c) Rw Software 2004-2007

  ---------------------------------------------------------------
*/

#if !defined(AFX_SOUNDVOLUME_H__82ABB6AE_9B34_4449_B302_521DB9BEE4CA__INCLUDED_)
#define AFX_SOUNDVOLUME_H__82ABB6AE_9B34_4449_B302_521DB9BEE4CA__INCLUDED_


	const uint MAXMUSICVOLUME = 7;

	
class CSoundVolume  
{
public:
	CSoundVolume();
	virtual ~CSoundVolume();

	bool					IsEnabled() const;
	void					SetVolume(const uint _nVolume);
	void					DoLowerVolume();
	void					DoRaiseVolume();
	const uint			GetVolume() const;
	const uint			GetVolumeLevel() const;

protected:
	uint					m_nVolume;
};

#endif // !defined(AFX_SOUNDVOLUME_H__82ABB6AE_9B34_4449_B302_521DB9BEE4CA__INCLUDED_)
