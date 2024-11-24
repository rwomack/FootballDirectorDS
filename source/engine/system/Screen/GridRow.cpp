
/*

	GridRow.cpp: implementation for the GridRow class.


	(c) Rw Software 2007

	---------------------------------------------------------------
*/

#include "stdafx.h"

#include "KeyScrollBar.h"
#include "GridColumn.h"
#include "Grid.h"
#include "GridRow.h"


/*------------------------------------------------------------------------------
	Method:   	CGridRow::CGridRow
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGridRow::CGridRow()
{
	m_Pointer = NULL;
	m_BackgroundColour = TRANSCOLOUR;
	m_TextColour = TRANSCOLOUR;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetText
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CGridRow::GetText()
{
	return m_Text;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetText
	Access:    	public
	Parameter: 	CString _Text
	Parameter: 	void * pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetText(CString _Text, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	m_Text = _Text;
	if (_Pointer != NULL)
	{
		m_Pointer = _Pointer;
		m_PointerType = _PointerType;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetBackgroundColour
	Access:    	public
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetBackgroundColour(COLORREF _Colour)
{
	m_BackgroundColour = _Colour;
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetBackgroundColour
	Access:    	public
	Returns:   	COLORREF
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGridRow::GetBackgroundColour()
{
	return m_BackgroundColour;
}

/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetTextColour
	Access:    	public
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetTextColour(COLORREF _Colour)
{
	m_TextColour = _Colour;
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetTextColour
	Access:    	public
	Returns:   	COLORREF
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGridRow::GetTextColour()
{
	return m_TextColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::DoCreate
	Access:    	public
	Parameter: 	CString _Text
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::DoCreate(CString _Text, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	m_Text = _Text;
	m_Pointer = _Pointer;
	m_PointerType = _PointerType;
	m_Bitmap = NULL;
	m_ImageNumber = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetBitmap
	Access:    	public
	Returns:   	CPNGDrawInfo*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPNGDrawInfo* CGridRow::GetBitmap()
{
	return m_Bitmap;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetBitmap
	Access:    	public
	Parameter: 	CPNGDrawInfo * _Bitmap
	Parameter: 	int _ImageNumber
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::SetBitmap(CPNGDrawInfo* _Bitmap, int _ImageNumber /*= 0*/)
{
  FDASSERT(_Bitmap != NULL);
	m_Bitmap = _Bitmap;
	m_ImageNumber = _ImageNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetPointer
	Access:    	public
	Returns:   	void*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void* CGridRow::GetPointer()
{
	return m_Pointer;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetPointerType
	Access:    	public
	Returns:   	ePointerType
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePointerType CGridRow::GetPointerType()
{
	return m_PointerType;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetPointer
	Access:    	public
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::SetPointer(void* _Pointer)
{
	m_Pointer = _Pointer;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetPointerType
	Access:    	public
	Parameter: 	ePointerType _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::SetPointerType(ePointerType _PointerType)
{
	m_PointerType = _PointerType;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetPointerDetails
	Access:    	public
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::SetPointerDetails(void* _Pointer, ePointerType _PointerType)
{
	m_Pointer = _Pointer;
	m_PointerType = _PointerType;
}
