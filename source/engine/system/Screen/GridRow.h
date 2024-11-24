
/*

	GridRow.h: interface for the GridRow class.


	(c) Rw Software 2007

  ---------------------------------------------------------------
*/


#pragma once


class CGridRow
{
public:
	CGridRow(); 
	~CGridRow() {}

	void					DoCreate(CString _Text, void*_Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	COLORREF				GetBackgroundColour();
	CPNGDrawInfo*			GetBitmap();
	void*					GetPointer();
	ePointerType			GetPointerType();
	CString					GetText();
	COLORREF				GetTextColour();
	bool					SetBackgroundColour(COLORREF _Colour);
	void					SetBitmap(CPNGDrawInfo* _Bitmap, int _ImageNumber = 0);
	bool					SetText(CString _Text, void *pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetTextColour(COLORREF _Colour);
	void 					SetPointer(void* _Pointer);
	void 					SetPointerType(ePointerType _PointerType);
	void 					SetPointerDetails(void* _Pointer, ePointerType _PointerType);

protected:
	CString      			m_Text;
	int          			m_Count;
	COLORREF	 			m_BackgroundColour;
	COLORREF	 			m_TextColour;

	int          			m_ImageNumber;
	CPNGDrawInfo*			m_Bitmap;

	void*					m_Pointer;
	ePointerType			m_PointerType;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
