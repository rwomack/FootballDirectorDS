
/*

    BitmapDialog.h: interface for the Dialog based classes.


	(c) Rw Software 2003-2007

  ---------------------------------------------------------------
*/



#ifndef __VSCROLLDLG_H__
#define __VSCROLLDLG_H__

class COffScreenBuffer;
class CBasicDisplay;

class CBitmapDialog : public CDialog
{
public:
	CBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = null);
	CBitmapDialog() {};
	virtual ~CBitmapDialog();

	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	void					DoRefreshDate();
	void					ResetDataRect();
	CRect&					GetDataRect();
	COffScreenBuffer*		GetMemDC();
	CBasicDisplay&			GetBasicDisplay();
	int						GetCentreX();
	void					DoInitFullScreen();
	void					DoTurnOffFullScreen();
	void					DoEnableOKButton();
	void					DoDisableOKButton();
	virtual void			OnInitRedrawArea();
	virtual void			OnPaint(const bool bShowDate = true, bool bShouldBlit = false);
	BOOL					OnInitDialog(const CString _strHelpTopic, int _StrID);
	BOOL					OnInitDialog(const CString _strHelpTopic, int _StrID, bool _bColourBackGround, COLORREF _Colour);
	BOOL					OnInitDialog(const CString _strHelpTopic, int _StrID, UINT _BitmapID);
	virtual BOOL			OnEraseBkgnd(CDC* pDC);
	void					DoLoadBitmap(const UINT nIDResource);
	void					ReDraw();
	virtual void			OnSize(UINT nType, int cx, int cy);
	virtual void			OnLButtonDown(UINT _nFlags, CPoint _Point); 
	virtual void			OnLButtonUp(UINT _nFlags, CPoint _Point); 
	virtual uint			OnGetDlgCode();

protected:
	CBasicDisplay*			m_BasicDisplay;

	DECLARE_MESSAGE_MAP()
};
#endif
