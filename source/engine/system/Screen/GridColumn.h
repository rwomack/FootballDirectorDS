
/*

	GridColumn.h: interface for the GridColumn class.


	(c) Rw Software 2003-2007

  ---------------------------------------------------------------
*/


#pragma once


class CGridColumn
{
public:
	CGridColumn();
	~CGridColumn();

	int						GetStyle();
	int						GetWidth();
	void					SetStyle(int _Style = COL_NORMAL);
	void					SetWidth(int _Width);
	void					SetTextColour(COLORREF _TextColour);
	COLORREF				GetTextColour();

	CPointerArray			m_Rows;
protected:
	int						m_Style;
	int						m_Width;
	COLORREF				m_TextColour;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
