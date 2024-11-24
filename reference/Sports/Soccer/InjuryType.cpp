

/*
    Class - CInjuryType


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryType::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_MaxRnd = _Resource.ReadByte();
    m_Severity = _Resource.ReadByte();
    m_BasePeriod = _Resource.ReadByte();
    m_RndPeriod = _Resource.ReadByte();
    m_Plural = _Resource.ReadByte();
    m_MatchPerformanceEffect = _Resource.ReadByte();
    m_RndMatchPerformanceEffect = _Resource.ReadByte();
	CExtString strName;
	strName.ReadFromResource(_Resource);
	m_Description.DoFindID(strName);
	m_CarryOff = _Resource.ReadByte();
    m_PhysioActive = _Resource.ReadByte();
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetBasePeriod
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetBasePeriod() const
{
	return 	m_BasePeriod;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetCarryOff
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CInjuryType::GetCarryOff() const
{
	return 	m_CarryOff;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetDescription
	Access:    	public 
	Returns:   	CStringID&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStringID& CInjuryType::GetDescription()
{
	return 	m_Description;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetMatchPerformanceEffect
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetMatchPerformanceEffect() const
{
	return 	m_MatchPerformanceEffect;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetMaxRnd
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetMaxRnd() const
{
	return 	m_MaxRnd;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetPlural
	Access:    	public 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CInjuryType::GetPlural() const
{
	return m_Plural;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetRandomPeriod
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetRandomPeriod() const
{
	return 	m_RndPeriod;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetRandomMatchPerformanceEffect
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetRandomMatchPerformanceEffect() const
{
	return 	m_RndMatchPerformanceEffect;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryType::GetSeverity
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjuryType::GetSeverity() const
{
	return 	m_Severity;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CInjuryTypeList::CInjuryTypeList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryTypeList::CInjuryTypeList()
{
	m_pList = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryTypeList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryTypeList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_INJURYTYPE, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pList = new CInjuryType[CExtArray::GetSize()];
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
        m_pList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryTypeList::~CInjuryTypeList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryTypeList::~CInjuryTypeList()
{
	if (m_pList != NULL)
	{
		delete []m_pList;
	}
};


/*------------------------------------------------------------------------------
	Method:   	CInjuryTypeList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CInjuryType&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryType& CInjuryTypeList::GetAt(const ushort _ElementNumber)
{
    EXT_ASSERT(_ElementNumber < GetSize());
	return static_cast<CInjuryType&> (m_pList[_ElementNumber]);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CInjuryEventType::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryEventType::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	CInjuryType::DoInitialiseFromResource(_Resource);
	CExtString strName;
	strName.ReadFromResource(_Resource);
	m_PluralDescription.DoFindID(strName);
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryEventType::GetPluralDescription
	Access:    	public 
	Returns:   	CStringID&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStringID& CInjuryEventType::GetPluralDescription()
{
	return 	m_PluralDescription;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CInjuryEventTypeList::CInjuryEventTypeList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryEventTypeList::CInjuryEventTypeList()
{
	m_pList = NULL;
}

/*------------------------------------------------------------------------------
	Method:   	CInjuryEventTypeList::~CInjuryEventTypeList
	Access:    	public 
	Returns:
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryEventTypeList::~CInjuryEventTypeList()
{
	if (m_pList != NULL)
	{
		delete []m_pList;
	}
};


/*------------------------------------------------------------------------------
	Method:   	CInjuryEventTypeList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryEventTypeList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_INJURYEVENTTYPE, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pList = new CInjuryEventType[CExtArray::GetSize()];
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
        m_pList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryEventTypeList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CInjuryEventType&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjuryEventType& CInjuryEventTypeList::GetAt(const ushort _ElementNumber)
{
    EXT_ASSERT(_ElementNumber < GetSize());
	return static_cast<CInjuryEventType&> (m_pList[_ElementNumber]);
}
