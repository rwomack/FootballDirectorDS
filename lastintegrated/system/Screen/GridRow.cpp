
/*

	GridRow.cpp: implementation for the GridRow class.


	(c) Rw Software 2007

	---------------------------------------------------------------
*/

#include "stdafx.h"

#include "UI.h"


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
	m_TextColour = BLACK;
	m_DisabledTextColour = GREY;
	m_IsDisabled = false;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::DoAddToValue
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::DoAddToValue(const int _Value)
{
	m_Count += _Value;
	m_Text.Format(_T("%d"), m_Count);
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
void CGridRow::DoCreate(CString _Text /*= _T("")*/, void*_Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	m_Text = _Text;
	m_Count = -1;
	m_Pointer = _Pointer;
	m_PointerType = _PointerType;
	m_Bitmap = NULL;
	m_ImageNumber = 0;
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
	Method:   	CGridRow::GetBitmapImageNumber
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGridRow::GetBitmapImageNumber()
{
	return m_ImageNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::GetDisabledTextColour
	Access:    	public 
	Returns:   	COLORREF
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGridRow::GetDisabledTextColour()
{
	return m_DisabledTextColour;
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
//	EXT_ASSERT(m_Pointer != NULL);
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
	Method:   	CGridRow::GetValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGridRow::GetValue()
{
	return m_Count;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::IsDisabled
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::IsDisabled()
{
	return m_IsDisabled;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::IsNumeric
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::IsNumeric()
{
	if (m_Count != -1)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetDisabled
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridRow::SetDisabled(bool _Value)
{
	m_IsDisabled = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetItemDate
	Access:    	public 
	Parameter: 	CDate & _Date
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetItemDate(CDate& _Date)
{
	m_Count = _Date.GetDate();
	return SetText(_Date.ShortDateStr());
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetItemMoney
	Access:    	public 
	Parameter: 	CString & _Str
	Parameter: 	int _Amount
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetItemMoney(const CString& _Str, const int _Amount)
{
	m_Count = _Amount;
	return SetText(_Str);
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetItemNum
	Access:    	public 
	Parameter: 	int _NumText
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetItemNum(int _NumText /*= 0*/, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAXPOINTER*/)
{
	m_Count = _NumText;
	CString str;
	str.Format(_T("%d"),_NumText);
	return SetText(str, _Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGridRow::SetItemPercentage
	Access:    	public 
	Parameter: 	const uint _CurValue
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetItemPercentage(const uint _CurValue)
{
	CString szNumBuff;
	szNumBuff.Format(TEXT("%d%%"), min(100, max(0, _CurValue)));
	return SetText(szNumBuff);
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
bool CGridRow::SetText(const CString& _Text, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
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
	Method:   	CGridRow::SetStrId
	Access:    	public 
	Parameter: 	uint _StrID
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridRow::SetStrId(uint _StrID, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	CString str;
	str.LoadString (_StrID);
	return SetText(str, _Pointer, _PointerType);
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
	EXT_ASSERT(_Bitmap != NULL);
	m_Bitmap = _Bitmap;
	m_ImageNumber = _ImageNumber;
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
