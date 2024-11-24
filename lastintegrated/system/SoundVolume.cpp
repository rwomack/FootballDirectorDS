// SoundVolume.cpp: implementation of the CSoundVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::CSoundVolume
	Access:    	public 
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundVolume::CSoundVolume()
{

}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::~CSoundVolume
	Access:    	virtual public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoundVolume::~CSoundVolume()
{

}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::IsEnabled
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CSoundVolume::IsEnabled() const
{
	return (bool)(m_nVolume > 0);
}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::GetVolume
	Access:    	public const 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CSoundVolume::GetVolume() const
{
	return m_nVolume;
}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::GetVolumeLevel
	Access:    	public const 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CSoundVolume::GetVolumeLevel() const
{
	return m_nVolume * (255 / MAXMUSICVOLUME);
}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::DoRaiseVolume
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundVolume::DoRaiseVolume()
{
	if (m_nVolume < MAXMUSICVOLUME)
	{
		m_nVolume++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::DoLowerVolume
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundVolume::DoLowerVolume()
{
	if (m_nVolume > 0)
	{
		m_nVolume--;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoundVolume::SetVolume
	Access:    	public 
	Parameter: 	const uint _nVolume
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoundVolume::SetVolume(const uint _nVolume)
{
	m_nVolume = min(_nVolume, MAXMUSICVOLUME);
}
