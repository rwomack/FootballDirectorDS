
/*

	GridRow.h: interface for the GridRow class.


	(c) Rw Software 2007 - 2008

  ---------------------------------------------------------------
*/


#pragma once


class CGridRow
{
public:
	CGridRow(); 
	~CGridRow() {}

	void					DoAddToValue(const int _Value);
	void					DoCreate(CString _Text = _T(""), void*_Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	COLORREF				GetBackgroundColour();
	CPNGDrawInfo*			GetBitmap();
	int						GetBitmapImageNumber();
	COLORREF				GetDisabledTextColour();
	void*					GetPointer();
	ePointerType			GetPointerType();
	CString					GetText();
	COLORREF				GetTextColour();
	int						GetValue();
	bool					IsDisabled();
	bool					IsNumeric();
	bool					SetBackgroundColour(COLORREF _Colour);
	void					SetBitmap(CPNGDrawInfo* _Bitmap, int _ImageNumber = 0);
	void					SetDisabled(bool _Value);
	bool					SetItemDate(CDate& _Date);
	bool					SetItemMoney(const CString& _Str, const int _Amount);
	bool					SetItemNum(int _NumText = 0, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetItemPercentage(const uint _CurValue);
	void 					SetPointer(void* _Pointer);
	void 					SetPointerType(ePointerType _PointerType);
	void 					SetPointerDetails(void* _Pointer, ePointerType _PointerType);
	bool					SetStrId(uint _StrID, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetText(const CString& _Text, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetTextColour(COLORREF _Colour);

protected:
	CString      			m_Text;
	int          			m_Count;
	COLORREF	 			m_BackgroundColour;
	COLORREF	 			m_TextColour;
	COLORREF	 			m_DisabledTextColour;
	bool					m_IsDisabled;

	int          			m_ImageNumber;
	CPNGDrawInfo*			m_Bitmap;

	void*					m_Pointer;
	ePointerType			m_PointerType;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
