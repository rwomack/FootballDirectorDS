
/*

	TransparentStatic.h: interface for the CDate class.

	(c) RW Software 2003

  ---------------------------------------------------------------
*/

#ifndef __TRANSPARENTSTATIC_H__
#define __TRANSPARENTSTATIC_H__



// CTransparentStatic

class CTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic)

public:
	CTransparentStatic();
	virtual ~CTransparentStatic();

 	void					OnCreate(const UINT nID, CWnd* pParent, const int iFontSize, const CString szString, const COLORREF ForeColor = BLACK, const int nWeight = FW_MEDIUM);
	void					OnCreate(const UINT nID, CWnd* pParent, const int iFontSize, const int nStringID, const COLORREF ForeColor = BLACK, const int nWeight = FW_MEDIUM);
	void					OnInitialise(const int iFontSize, const CString szString, const COLORREF ForeColor = BLACK, const int nWeight = FW_MEDIUM);
	void					SetFontSize(const int iPoints, const int nWeight);
	BOOL					OnEraseBkgnd(CDC* pDC);


protected:
	CFont					m_Font;
	bool					m_bFontCreated;
	COLORREF				m_ForeColor;

	DECLARE_MESSAGE_MAP()
//public:
	afx_msg void OnPaint();
};

#endif
