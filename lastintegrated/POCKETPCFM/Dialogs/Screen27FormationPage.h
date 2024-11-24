#if !defined(AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
#define AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_

// Screen27FormationPage.h : header file
//

class CExtRect;

/////////////////////////////////////////////////////////////////////////////
// Screen27Tactics dialog

class CScreen27FormationPage : public CBitmapPropertyPage
{
    DECLARE_DYNCREATE(CScreen27FormationPage)

// Construction
public:
    CScreen27FormationPage();
    ~CScreen27FormationPage();

// Dialog Data
    enum 
	{ 
		iPitchXGap = 19,
		iPitchYGap = 14,

		IDD = IDD_SCREEN2703 
	};

	
// Implementation
protected:
    CVOImage				m_Pitch;
	ushort					m_iPlayerSelected;

    BOOL					OnInitDialog();
    void					OnPaint();
	void					OnSelchangeFormationList();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

};
#endif // !defined(AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
