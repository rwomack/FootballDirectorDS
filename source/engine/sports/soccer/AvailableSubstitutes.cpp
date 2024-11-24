
 /*

	Program:- AvailableSubstitutes.cpp

    The Available Substitutes class routines

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::CAvailableSubstitutes
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CAvailableSubstitutes::CAvailableSubstitutes()
{
	m_SubsSelect = MAXSUBS;
	m_SubsUse = MAXSUBS;
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAvailableSubstitutes::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_SubsSelect = _Resource.ReadByte();
    m_SubsUse = _Resource.ReadByte();
	EXT_ASSERT(m_SubsSelect > 0 && m_SubsUse > 0 && m_SubsUse <= m_SubsSelect);
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAvailableSubstitutes::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Read(&m_SubsSelect, sizeof(m_SubsSelect));
	EXT_ASSERT(m_SubsSelect < PLAYERSINTEAM);
    _pDataFile->Read(&m_SubsUse, sizeof(m_SubsUse));
	EXT_ASSERT(m_SubsUse < PLAYERSINTEAM);
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAvailableSubstitutes::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Write(&m_SubsSelect, sizeof(m_SubsSelect));
    _pDataFile->Write(&m_SubsUse, sizeof(m_SubsUse));
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::GetSubsSelect
	Access:    	public const 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CAvailableSubstitutes::GetSubsSelect() const						
{ 
	return m_SubsSelect; 
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::GetSubsUse
	Access:    	public const 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CAvailableSubstitutes::GetSubsUse() const							
{ 
	return m_SubsUse; 
}


/*------------------------------------------------------------------------------
	Method:   	CAvailableSubstitutes::SetSubstitutes
	Access:    	public 
	Parameter: 	const byte _Select
	Parameter: 	const byte _Use
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAvailableSubstitutes::SetSubstitutes(const byte _Select, const byte _Use)
{
	m_SubsSelect = _Select;
	m_SubsUse = _Use;
}