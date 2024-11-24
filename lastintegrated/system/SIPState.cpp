#include "stdafx.h"
#include "SIPState.h"

stSIPState::stSIPState(HWND hWnd, bool bNewState)
{
	m_hWnd=hWnd;
	memset(&m_info,0,sizeof(SIPINFO));

	m_info.cbSize=sizeof(SIPINFO);

	SHSipInfo(SPI_GETSIPINFO,0,&m_info,0);

	SHSipPreference(hWnd,bNewState?SIP_UP:SIP_FORCEDOWN);
}

stSIPState::~stSIPState()
{

	if(m_info.fdwFlags & SIPF_ON)
		SHSipPreference(m_hWnd,SIP_UP);
	else
		SHSipPreference(m_hWnd,SIP_FORCEDOWN);

	//SHSipInfo(SPI_SETSIPINFO,0,&m_info,0);
}
