#ifndef __ALPHATAB_H__
#define __ALPHATAB_H__


typedef BOOL (__stdcall *UnregisterFunc1Proc)( UINT, UINT );

class CJoyPad
{
public:
	CJoyPad()
	{
		m_hCoreDll = LoadLibrary(_T("coredll.dll"));
		if (m_hCoreDll)
		{
			m_procUndergisterFunc = (UnregisterFunc1Proc)GetProcAddress(m_hCoreDll, _T("UnregisterFunc1"));
		}
	}

	~CJoyPad() {}

	BOOL 					RegisterHotKey(HWND hWnd, int id, UINT vk)
	{
    FDASSERT(m_procUndergisterFunc);
		BOOL bResult1 = m_procUndergisterFunc(MOD_WIN, vk);
		BOOL bResult2 = ::RegisterHotKey(hWnd, id, MOD_WIN, vk);
		return bResult2 && bResult1;
	}

protected:
	HINSTANCE 				m_hCoreDll;
	UnregisterFunc1Proc 	m_procUndergisterFunc;
};
#endif
