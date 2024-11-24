#if !defined(AFX_Screen13_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen13_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen13.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CScreen13Dlg dialog


class CScreen13Dlg : public CPlayerContractDlg
{
// Construction
public:
    CScreen13Dlg(CYouth* pYouth, CString strPrompt, CWnd* pParent = null);   // standard constructor
	
// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};


protected:
	CYouth*					m_pCurrentYouth;
	CYouthNameTextRegion	m_NameText;
	
	void					OnCancelButton();
	BOOL					OnInitDialog();
	void					OnOfferContract();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen13_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
