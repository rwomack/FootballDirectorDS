
/*

    BitmapFormView.h: interface for the Form based classes.


	(c) Rw Software 2003 - 2008

  ---------------------------------------------------------------
*/



#ifndef __VSCROLLFRM_H__
#define __VSCROLLFRM_H__

class CBasicDisplay;
class COffScreenBuffer;


class CBitmapFormView : public CFormView
{
public:
	CBitmapFormView(UINT nIDTemplate);
	virtual ~CBitmapFormView() {};


	void					DoInitFullScreen();
	void					DoLoadBitmap(const UINT nIDResource);
	void					DoRemoveAllData();
	CBasicDisplay*			GetBasicDisplay();
	int						GetCentreX();
	int						GetCentreY();
	CGrid&					GetGrid();
	int						GetHeight();
	COffScreenBuffer*		GetMemDC();
	int						GetWidth();
	int						GetWindowHeight();
	bool					IsInitialised();
	void					OnActionMenu(int _Menu, int _yPos, int _CommandToRemove = 0);
	BOOL					OnEraseBkgnd(CDC* pDC);
	uint					OnGetDlgCode();
	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	void OnInitialiseView(const int _HelpTopic, int _StrID = 0);
	void OnInitialUpdate(const int _HelpTopic, CBasicDisplay* _BasicDisplay, int _StrID = 0);
	void					OnInitRedrawArea();
	virtual void			OnSize(UINT nType, int cx, int cy);
	void					ReDraw();
	void					SetIsInitialised(bool _Value);

protected:
	CBasicDisplay*			m_BasicDisplay;
	CString					m_strHelpTopic;
	bool					m_IsInitialised;

	virtual	void			OnActionClicked();
	void					OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
#endif
