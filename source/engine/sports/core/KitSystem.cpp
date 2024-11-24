
/*
	File:- KitSystem.cpp


	Football Director

	(c) Sports Director Ltd 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CKit::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CKit::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_ID = _Resource.ReadByte();
	m_PrimaryColour = _Resource.ReadUInt32();
	m_SecondaryColour = _Resource.ReadUInt32();
}


/*------------------------------------------------------------------------------
	Method:   	CKit::GetID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CKit::GetID()
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CKit::GetPrimaryColour
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CKit::GetPrimaryColour()
{
	return m_PrimaryColour;
}


/*------------------------------------------------------------------------------
	Method:   	CKit::GetSecondaryColour
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CKit::GetSecondaryColour()
{
	return m_SecondaryColour;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CKitSystem::~CKitSystem
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKitSystem::~CKitSystem()
{
	delete []m_KitList;
}


/*------------------------------------------------------------------------------
	Method:   	CKitSystem::operator[]
	Access:    	public 
	Parameter: 	ushort _Value
	Returns:   	CKit&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKit& CKitSystem::operator [] (ushort _Value)
{
	EXT_ASSERT(_Value < GetSize());
	return m_KitList[_Value];
}


/*------------------------------------------------------------------------------
	Method:   	CKitSystem::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CKitSystem::DoInitialise()
{
	CBinaryReader theResource(0, IDR_KITSYSTEM, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_KitList = FD_ENGINE_NEW CKit[CExtArray::GetSize()];
	for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_KitList[ListPos].DoInitialiseFromResource(theResource);
	}
}
