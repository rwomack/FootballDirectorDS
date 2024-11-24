
/*

	TextRegion.h: interface for the TextRegion class.


	(c) Rw Software 2007

  ---------------------------------------------------------------
*/


#pragma once


class CTextRegion : public CGridRow
{
public:
	CTextRegion(void);
	~CTextRegion(void);

	void					DoInitialise(CBasicDisplay* _BasicDisplay, CString _Text, CRect _Rect, void* _Pointer = NULL, int _Style = COL_NORMAL);
	void					MoveWindow(CRect _Rect);
	void					OnPaint();
	int						GetAlignment();

protected:
	CBasicDisplay*			m_BasicDisplay;
	int			 			m_Style;		
	CRect		 			m_WindowRect;
	CFont*					m_Font;
};
