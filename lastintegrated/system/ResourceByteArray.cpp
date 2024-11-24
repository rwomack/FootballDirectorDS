// ResourceByteArray.cpp: implementation of the CResourceByteArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


 //----------------------------------------------------------------------------------------------------------------------------
CResourceByteArray::CResourceByteArray(UInt32 dwResourceID, LPCTSTR pcszClass)
{
	HRSRC hrsrc = FindResource(GetModuleHandle(NULL), (LPCTSTR)dwResourceID, pcszClass);
	m_dwSize = SizeofResource(GetModuleHandle(NULL), hrsrc);
	HGLOBAL hGlobal = LoadResource(GetModuleHandle(NULL), hrsrc);
	m_pData = (PBYTE) LockResource(hGlobal);
}


 //----------------------------------------------------------------------------------------------------------------------------
CResourceByteArray::~CResourceByteArray()
{

}


 //----------------------------------------------------------------------------------------------------------------------------
const PBYTE CResourceByteArray::GetData(void)
{
	return m_pData;
}


 //----------------------------------------------------------------------------------------------------------------------------
const BYTE CResourceByteArray::GetAt(const Int16 aIdx)
{
	return *(m_pData + aIdx);
}


 //----------------------------------------------------------------------------------------------------------------------------
const UInt16 CResourceByteArray::GetUpperBound(void)
{
	return m_dwSize - 1;
}