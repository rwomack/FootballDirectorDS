#if !defined(AFX_STORAGELISTBOX_H__798A5F4B_A4C5_4A5A_B1B2_3829626BF1D7__INCLUDED_)
#define AFX_STORAGELISTBOX_H__798A5F4B_A4C5_4A5A_B1B2_3829626BF1D7__INCLUDED_

// storagelistbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStorageListBox window

class CStorageListBox : public CComboBox
{
// Construction
public:
	CStorageListBox();
	virtual ~CStorageListBox();

	BOOL 					OnInitDialog(UINT nID, CWnd* pParent);
	CString					GetCurrentCardPath();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_STORAGELISTBOX_H__798A5F4B_A4C5_4A5A_B1B2_3829626BF1D7__INCLUDED_)
