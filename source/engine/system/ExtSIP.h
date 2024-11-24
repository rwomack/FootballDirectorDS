#if !defined(AFX_EXTSIP_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_)
#define AFX_EXTSIP_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_

// ExtSIP.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSIPState	
{			
public:
	CSIPState()
	{
		m_sipState = SIP_DOWN;
	}
	void					DoPreserve(CWnd* pWnd);
	void					DoRestore(CWnd* pWnd);
	tagSIPSTATE				GetState(void)
	{
		return m_sipState;
	}
	void					SetState(tagSIPSTATE sipState)
	{
		m_sipState = sipState;
	}
	
protected:
	tagSIPSTATE				m_sipState;
};

#endif