 #ifndef __EXTPROPERTYSHEET_H__
#define __EXTPROPERTYSHEET_H__

class CExtPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CExtPropertyPage)

// Construction
public:
	CExtPropertyPage() : CPropertyPage() {};
	CExtPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0) : CPropertyPage(nIDTemplate, nIDCaption) {};
	CExtPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0) : CPropertyPage(lpszTemplateName, nIDCaption) {};

	virtual BOOL			PreTranslateMessage(MSG* _pMsg);
	virtual BOOL			OnSetActive();

protected:
	void					OnOK();
	void					OnCancel();
	void  OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void  OnKeyUp(UINT _Char, UINT _RepCnt, UINT _Flags);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////////////////////////////


class CExtPropertySheet : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CExtPropertySheet)

	CExtPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage = 0)
		: CPropertySheet(pszCaption, pParentWnd, iSelectPage) {};
	CExtPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage = 0)
		: CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {};

	virtual BOOL			PreTranslateMessage(MSG* _pMsg);

protected:
	BOOL					ContinueModal();
	void					SetTabText(const int _TabID, const int _StringID);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
