
/*

	ExtPoint.cpp: implementation of the CExtPoint class.


	(c) Rw Software 1994 - 2007

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"

#include "ExtPoint.h"


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::CExtPoint
	Access:    	public 
	Parameter: 	const ushort _x
	Parameter: 	const ushort _y
	Returns: 
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtPoint::CExtPoint(const ushort _x, const ushort _y)
{
	m_X = _x;
	m_Y = _y;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::operator=
	Access:    	public 
	Parameter: 	const CExtPoint & _Point
	Returns:   	CExtPoint&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtPoint& CExtPoint::operator =(const CExtPoint& _Point)
{
	m_X = _Point.m_X;
	m_Y = _Point.m_Y;
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::GetX
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CExtPoint::GetX() const
{
	return m_X;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::GetY
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CExtPoint::GetY() const
{
	return m_Y;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::SetX
	Access:    	public 
	Parameter: 	const ushort _x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPoint::SetX(const ushort _x)
{
	m_X = _x;
}


/*------------------------------------------------------------------------------
	Method:   	CExtPoint::SetY
	Access:    	public 
	Parameter: 	const ushort _x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtPoint::SetY(const ushort _x)
{
	m_Y = _x;
}
