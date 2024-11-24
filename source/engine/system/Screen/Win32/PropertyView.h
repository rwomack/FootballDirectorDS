/*=============================================================================
 * This is an MFC based extension class.
 * Copyright (C) 2000 YEAsoft Int'l.
 * All rights reserved.
 *=============================================================================
 * Copyright (c) 2000 YEAsoft Int'l (Leo Moll, Andrea Pennelli).
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *=============================================================================
 * FILENAME		:	PropertyView.h
 * PURPOSE		:	Declaration of a "property sheet"-like view
 * SCOPE		:	Property View Classes
 * HISTORY		: =============================================================
 */ 

#if !defined(AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_)
#define AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_


class CPropertyView;
class CPropertyViewPage;

#ifndef baseCPropertyView
#define baseCPropertyView		CFormView
#endif
#ifndef baseCPropertyViewPage
#define baseCPropertyViewPage	CDialog
#endif


class CPropertyViewPage : public baseCPropertyViewPage
{
	DECLARE_DYNAMIC				(CPropertyViewPage)

// Construction
public:
	CPropertyViewPage			();
	CPropertyViewPage			(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	CPropertyViewPage			(UINT nIDTemplate, UINT nIDCaption = 0, CWnd* pWnd = null);

// Dialog Data

// Attributes
public:
	CPropertyView *				GetView					() const;

// Operations
public:
	void						Construct				(UINT nIDTemplate, UINT nIDCaption = 0);
	void						Construct				(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	virtual void				SetModified				(BOOL bModified = TRUE);
	virtual BOOL				GetModified				() const { return m_bModified; }

// Overrides
public:
	virtual BOOL				OnSetActive				();
	virtual BOOL				OnKillActive			();
	virtual void				OnInitialUpdate			() { UpdateData (FALSE); OnUpdate (null, 0, null); }
	virtual void				OnUpdate				(CView* pSender, LPARAM lHint, CObject* pHint) { }
	virtual BOOL				OnApply					();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyViewPage)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void					CommonConstruct			(LPCTSTR lpszTemplateName, UINT nIDCaption);

	// Generated message map functions
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void			OnOK();
	virtual void			OnCancel();
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	CPropertyView *			m_pView;
	BOOL					m_bModified;
	BOOL					m_bFirstSetActive;
	CSize					m_szMin;
	UINT					m_nIDCaption;
	friend class			CPropertyView;
};
/////////////////////////////////////////////////////////////////////////////


class CPropertyView : public baseCPropertyView
{
// Construction
protected:
	CPropertyView();
	DECLARE_DYNCREATE		(CPropertyView)

// Attributes
public:
	int						GetActiveIndex			() const;
	int						GetPageIndex			(CPropertyViewPage *pPage) const;
	int						GetPageCount			() const;
	CPropertyViewPage *		GetPage					(int nPage) const;
	CPropertyViewPage *		GetActivePage			() const { return GetPage (GetActiveIndex ()); }
	BOOL					SetActivePage			(int nPage);
	BOOL					SetActivePage			(CPropertyViewPage *pPage);
	CTabCtrl *				GetTabControl			() { return &m_TabCtrl; }
	CImageList *			GetImageList			() const;
	CImageList *			SetImageList			(CImageList *pImageList);
	void					EnableStackedTabs		(BOOL bStacked);
	void					EnableScrollView		(BOOL bScrollView);

	BOOL					SetPageTitle			(int nPage, UINT nID);
	BOOL					SetPageTitle			(int nPage, LPCTSTR pszTitle);
	BOOL					GetPageTitle			(int nPage, LPTSTR lpszStringBuf, int nMaxCount) const;

// Operations
public:
	BOOL					AddPage					(CPropertyViewPage *pPage, int nImage = -1) { return AddPage (pPage, (LPCTSTR) null, nImage); }
	BOOL					AddPage					(CPropertyViewPage *pPage, UINT nID, int nImage);
	BOOL					AddPage					(CPropertyViewPage *pPage, LPCTSTR pszTitle, int nImage = -1);
	BOOL					RemovePage				(CPropertyViewPage *pPage);
	BOOL					RemovePage				(int nPage);
	BOOL					PressButton				(int nButton) { if ( nButton == PSBTN_APPLYNOW ) return Apply (); return FALSE; }
	BOOL					Apply					();
	virtual void			SetModified				(BOOL bChanged = TRUE);

// Overrides
public:
	virtual void			PreCreateTabControl		(CRect &rect, DWORD &dwStyle) { }

// Overrides
public:
	virtual void			OnInitialUpdate();
protected:
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual void			OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void			OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	virtual ~CPropertyView();

protected:
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);
	afx_msg void			OnSize(UINT nType, int cx, int cy);
	afx_msg void			OnSetFocus(CWnd* pOldWnd);
	afx_msg void			OnTabChanging(NMHDR * pNotifyStruct, LRESULT* result);
	afx_msg void			OnTabChanged(NMHDR * pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	void					EnablePage				(CPropertyViewPage *pPage, BOOL bEnable = TRUE);
	BOOL					RemovePage				(CPropertyViewPage *pPage, int iIndex);
	void					AdjustPagePosition		(CPropertyViewPage *pPage);

// Implementation
protected:
	BOOL					m_bAdjusted;
    CTabCtrl				m_TabCtrl;
	int						m_nActivePage;
	BOOL					m_bStacked;
	BOOL					m_bScrollView;
};
#endif // !defined(AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_)
