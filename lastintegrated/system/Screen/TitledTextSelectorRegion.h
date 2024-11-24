#if !defined(TITLEDTEXTSELECTORREGION_INCLUDED_)
#define TITLEDTEXTSELECTORREGION_INCLUDED_

/*

	TitledTextSelectorRegion.h: interface for the TitledTextSelectorRegion class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/

class CTitledTextSelector : public CTextSelector
{
public:
	CTitledTextSelector(void);
	virtual ~CTitledTextSelector(void);

	void					DoInitialise(int _YPos, int _TitleStrID, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownGfxID, int _ButtonUpGfxID);
	void					MoveWindow();
	void					SetStyle(const uint _Style);

protected:
	CTextRegion*			m_TitleTextRegion;
	int						m_Ypos;
};
#endif