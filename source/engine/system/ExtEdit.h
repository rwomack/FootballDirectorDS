#ifndef __EXTEDIT_H__
#define __EXTEDIT_H__


/*

	File:- ExtEdit.h

	Extension to CEdit class

	(c) Rw Software 1994 - 2007

  ---------------------------------------------------------------------------------------
 */

class CExtEdit : public CEdit
{
protected:



public:



	int						GetValue();
	void					SetValue(const int _Value);


};
/////////////////////////////////////////////////////////////////////////////

class CSIPEdit : public CEdit
{
protected:
	bool					m_bSurrenderScrollKeys;		//DSR - set to true if this control should toss up/down/select back at parent

public:
							CSIPEdit();

	void					SetSurrenderScrollKeys(const bool bSurrenderScrollKeys);
	void					OnLButtonUp(UINT _Flags, CPoint _point);
	void					OnSetFocus(CWnd* _pOldWnd);
	void					SetNewWindowText(CString _strLookup);
	void					SetAllowSIPup(const bool _bAllowSIPup);
	void					SetFocus();
	uint					OnGetDlgCode(); 
	virtual					BOOL PreTranslateMessage(MSG* _pMsg);


protected:
	bool					m_bAllowSIPup;

    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CSIPUnderLinedEdit : public CSIPEdit
{
public:
	CSIPUnderLinedEdit()
	{
		m_colorUnderLine = RGB(192, 192, 192);
		m_LineY = 15;
	}

protected:
	COLORREF				m_colorUnderLine;
	ushort					m_LineY;

	void					OnPaint();
	void					DrawUnderLine(CDC* _pDC);
	void					OnLButtonDown(UINT _Flags, CPoint _point);
	void					OnRecognizeGesture(NMHDR* _pNMHDR, LRESULT* _pResult);
	BOOL					OnEraseBkgnd(CDC* _pDC);
	void					OnSetFocus(CWnd* _pOldWnd);
	void					OnKillFocus(CWnd* _pNewWnd);

    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CSIPLookupEdit : public CSIPUnderLinedEdit
{
public:
	CSIPLookupEdit();

protected:
	DWORD					m_dwSIPFlags;

	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void					OnKeyUp(UINT _Char, UINT _RepCnt, UINT _Flags);
	uint					OnGetDlgCode();
	void					OnSetFocus(CWnd* _pOldWnd);
	void					OnKillFocus(CWnd* _pNewWnd);

    DECLARE_MESSAGE_MAP()
};
#endif
