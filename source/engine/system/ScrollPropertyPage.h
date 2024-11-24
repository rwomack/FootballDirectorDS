#if !defined(AFX_SCROLLPROPERTYPAGE_H__0FA3165D_9AC9_4F15_BA36_BA1CAB3BF18A__INCLUDED_)
#define AFX_SCROLLPROPERTYPAGE_H__0FA3165D_9AC9_4F15_BA36_BA1CAB3BF18A__INCLUDED_

// ScrollPropertyPage.h : header file
//

class CScrollPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CScrollPropertyPage)
public:
	CScrollPropertyPage()
	{
		DoResetVariables();
	}
	CScrollPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0) : 
		CPropertyPage(nIDTemplate, nIDCaption) 
	{
		DoResetVariables();
	}
	~CScrollPropertyPage() {};

	// Implementation
	void					DoResetVariables(void);

protected:
    int						m_nVertInc;
	int						m_nVscrollMax;
	int						m_nVscrollPos;
	int						m_nSavedVscrollMax;
	int						m_nSavedVscrollPos;
	
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) ;
	BOOL					OnSetActive(void);
	BOOL					OnKillActive(void);
		
	DECLARE_MESSAGE_MAP()
};

#endif