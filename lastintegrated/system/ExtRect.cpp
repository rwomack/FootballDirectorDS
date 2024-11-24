/*

	ExtRect.cpp : implementation file

	(c) RW Software 2003

---------------------------------------------------------------
*/

#include "stdafx.h"
#include "ExtRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CExtRect::PtInRect
	Access:    	public 
	Parameter: 	const CPoint _ThePoint
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CExtRect::PtInRect(const CPoint _ThePoint)
{
	bool bRet = false;
	if (_ThePoint.x >= CRect::left && _ThePoint.x < CRect::right &&
		_ThePoint.y >= CRect::top && _ThePoint.y < CRect::bottom)
	{
		bRet = true;
	}
	return bRet;
}
