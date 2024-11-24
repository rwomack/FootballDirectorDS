
/*

	EXtPropertyPage.h: interface for the Property Page based classes.


	(c) Rw Software 2003-2007

---------------------------------------------------------------
*/



#ifndef __EXTPROPERTYPAGE_H__
#define __EXTPROPERTYPAGE_H__

class COffScreenBuffer;
class CBasicDisplay;


class CBitmapPropertyPage : public CPropertyPage
{
public:
	CBitmapPropertyPage(UINT nIDTemplate) :
	  CPropertyPage(nIDTemplate) {};

	  BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	  virtual void			OnInitRedrawArea();
	  BOOL					OnEraseBkgnd(CDC* pDC);
	  void					OnPaint(bool _ShouldBlit = false);
	  BOOL					OnInitDialog(const CString _strHelpTopic);
	  virtual void			ReDraw();
	  COffScreenBuffer*		GetMemDC();
	  void					ResetDataRect();
	  CRect&				GetDataRect();
	  CBasicDisplay&		GetBasicDisplay();
	  uint					OnGetDlgCode();
	  virtual void			OnSize(UINT nType, int cx, int cy) {};

protected:

	DECLARE_MESSAGE_MAP()
};
#endif