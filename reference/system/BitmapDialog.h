
/*

    BitmapDialog.h: interface for the Dialog based classes.


	(c) Rw Software 2003 - 2008

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

	void					DoBlit();
	void					DoDisableOKButton();
	void					DoEnableOKButton();
	void					DoInitFullScreen();
	void					DoLoadBitmap(const UINT nIDResource);
	void					DoTurnOffFullScreen();
	CBasicDisplay*			GetBasicDisplay();
	int						GetCentreX();
	virtual uint			OnGetDlgCode();
	CGrid&					GetGrid();
	COffScreenBuffer*		GetMemDC();
	CTextRegion*			GetTextRegion(const int _TextRegionID);
	bool					IsInitialised();
	void					LoadMenu(int _MenuBarID);
	virtual BOOL			OnEraseBkgnd(CDC* pDC);
	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	BOOL					OnInitDialog(const int _HelpTopic, const int _StrID, const bool _FullScreen = false);
	BOOL					OnInitDialog(const int _HelpTopic, const int _StrID, UINT _BitmapID);
	virtual void			OnInitRedrawArea(const bool _bShowDate = true);
	virtual void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void			OnLButtonDown(UINT _nFlags, CPoint _Point); 
	virtual void			OnLButtonUp(UINT _nFlags, CPoint _Point); 
	virtual void			OnPaint(const bool bShowDate = true, bool bShouldBlit = false);
	virtual void			OnSize(UINT nType, int cx, int cy);
	virtual void			OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					ReDraw();
	void					ReplaceMenu(int _MenuBarID);
	void					SetIsInitialised(bool _Value);

protected:
	CBasicDisplay*			m_BasicDisplay;
	CCommandBar				m_wndCommandBar;
	bool					m_IsInitialised;

	DECLARE_MESSAGE_MAP()
};
#endif
