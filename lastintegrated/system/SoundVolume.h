
/*

	SoundVolume.h: interface for the CSoundVolume class.

	(c) Rw Software 2004 - 2008

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

	void					DoLowerVolume();
	void					DoRaiseVolume();
	uint					GetVolume() const;
	uint					GetVolumeLevel() const;
	bool					IsEnabled() const;
	void					SetVolume(const uint _nVolume);

protected:
	uint					m_nVolume;
};

#endif // !defined(AFX_SOUNDVOLUME_H__82ABB6AE_9B34_4449_B302_521DB9BEE4CA__INCLUDED_)
