
/*

	ExtPNGStatic.h:

	(c) RW Software 2003

  ---------------------------------------------------------------
*/

#ifndef __EXTPNGSTATIC_H__
#define __EXTPNGSTATIC_H__



// CExtPNGStatic

class CExtPNGStatic : public CStatic
{
	DECLARE_DYNAMIC(CExtPNGStatic)

public:
	CExtPNGStatic();
	virtual ~CExtPNGStatic();

	void					OnCreate(const UINT nID, CWnd* pParent, DWORD dwResourceID, LPCTSTR pcszClass, HMODULE hModule = 0);

protected:
	CVOImage				m_picPicture;
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

#endif
