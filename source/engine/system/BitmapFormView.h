
/*

    VScrollForm.h: interface for the Form based classes.


	(c) Rw Software 2003-2007

  ---------------------------------------------------------------
 */



#ifndef __VSCROLLFRM_H__
#define __VSCROLLFRM_H__

#ifdef ORIGINAL_CODE

class CBasicDisplay;
class COffScreenBuffer;
class CPointerArray;


class CBitmapFormView : public CFormView
{
public:
	CBitmapFormView(UINT nIDTemplate);
	virtual ~CBitmapFormView() {};

	CPointerArray&			ButtonList();
	void					DoInitialiseControlList();
	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	bool					Isinitialised();
	void					OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID, bool _ColourBackGround = false, COLORREF _Colour = 0x00000000);
	BOOL					OnEraseBkgnd(CDC* pDC);
	void					DoLoadBitmap(const UINT nIDResource);
	void					OnInitRedrawArea();
	CBasicDisplay&			GetBasicDisplay();
	void					ReDraw();
	void					ResetDataRect();
	int						GetCentreX();
	CRect&					GetDataRect();
	COffScreenBuffer*		GetMemDC();
	uint					OnGetDlgCode();
	virtual void			OnSize(UINT nType, int cx, int cy);

protected:
	CBasicDisplay*			m_BasicDisplay;
	CPointerArray			m_listButtons;
	CString					m_strHelpTopic;

	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnSetFocus(CWnd* _OldWnd);

	DECLARE_MESSAGE_MAP()
};
#endif
#endif
