
/*

	ExtStatic.h : main header file for the ExtendedStatic gadget


	(c) Rw Software 2007

  ---------------------------------------------------------------------------------------
 */


#ifndef __EXTSTATIC_H__
#define __EXTSTATIC_H__


/////////////////////////////////////////////////////////////////////////////


class CExtStatic : public CStatic
{
public:
	CExtStatic() 
		: m_hBrush(null)
		, m_bFontCreated(false)
		, CStatic()	
	  {}
	~CExtStatic() ;

	void					SetValue(const int _Value);
	HBRUSH					CtlColor(CDC* _pDC, UINT _nCtlColor);
	void					OnCreate(const UINT nID, CWnd* pParentWnd, CFont* pFont, const CString szString, 
									 const COLORREF ForeColor = BLACK, const COLORREF BackColor = WHITE);
	void					OnCreate(const int iFontSize, const COLORREF ForeColor, const COLORREF BackColor, const CString szString, const RECT& rect, 
									 CWnd* pParentWnd, const uint iResourceId = 0, const int iAlign = SS_CENTER, const int nWeight = FW_MEDIUM);
	void					OnCreate(const int iFontSize, const COLORREF ForeColor, const COLORREF BackColor, const int iStringId, const RECT& rect, 
									 CWnd* pParentWnd, const uint iResourceId, const int iAlign = SS_CENTER, const int nWeight = FW_MEDIUM);
	void					OnCreate(const UINT nID, CWnd* pParentWnd, const int iFontSize, const COLORREF ForeColor, 
									 const COLORREF BackColor, const int iStringId = 0, const int nWeight = FW_MEDIUM);
	void					OnCreate(const UINT nID, CWnd* pParentWnd, const int iFontSize, const COLORREF ForeColor, 
									 const COLORREF BackColor, const CString szString, const int nWeight = FW_MEDIUM);
	void					OnCreate(CFont* pFont, const COLORREF ForeColor, const COLORREF BackColor, const int iStringId, const RECT& rect,
									 CWnd* pParentWnd, const uint iResourceId, const int iAlign = SS_LEFT);
	void					OnInitialise(const int iFontSize, const COLORREF ForeColor, const COLORREF BackColor, 
										 const int iStringId = 0, const int nWeight = FW_MEDIUM);
	void					OnInitialise(const int iFontSize, const COLORREF ForeColor, const COLORREF BackColor, 
										 const CString szString, const int nWeight = FW_MEDIUM);
	void					OnInitialise(CFont* pFont, const COLORREF ForeColor, const COLORREF BackColor, const int iStringId);
	void					OnInitialise(CFont* pFont, const COLORREF ForeColor, const COLORREF BackColor, const CString szString);
	void					OnInitialiseColors(const COLORREF _ForeColor = BLACK, const COLORREF _BackColor = WHITE);
	void					SetFontSize(const int _iPoints, const int _nWeight = FW_MEDIUM);
	void					SetColors(const COLORREF _BackColour, const COLORREF _ForeColour);
	void					SetForeColor(const COLORREF _ForeColour);
	void					SetBackColor(const COLORREF _BackColour);
	void					SetWindowTextNull();
	void					SetWindowText(CString _String);
	void					Update(CString _Str);
	void					Update(const int iStringId);
	void					OnLButtonDown(uint _Flags, CPoint _Point);

protected:
	COLORREF				m_ForeColor;
	COLORREF				m_BackColor;
	CBrush*					m_hBrush;
	CFont					m_Font;
	bool					m_bFontCreated;

    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class CExtIntStatic : public CExtStatic
{
public:	
	void					SetValue(const int _Vakue, bool _bUpdateStatic = true);
	int						GetValue() const;
	void					IncValue(const bool _bUpdateStatic = true);
	void					DecValue(const bool _bUpdateStatic = true);
	void					AddValue(const int _Value, const bool _bUpdateStatic = true);
	void					SubValue(const int _Value, const bool _bUpdateStatic = true);
	void					DisplayPercentage(const int _Value);
	
protected:
	int						m_nValue;
};
/////////////////////////////////////////////////////////////////////////////


class CPercentageStatic : public CExtIntStatic
{
public:	
	void					DoUpdateDisplay();
	void					SetValue(const int _Value);
};
/////////////////////////////////////////////////////////////////////////////
#endif





















