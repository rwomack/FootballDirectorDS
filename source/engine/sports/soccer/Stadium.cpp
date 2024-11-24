
 /*

        Class:- Stadium


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "Stadium.h"


/*------------------------------------------------------------------------------
	Method:   	CStadium::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource & _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_Name.ReadFromResource(_Resource);
#ifdef	ACTIVESTADIUMPLANS
	m_StandData.Create(File);
#else
    m_Capacity = _Resource.ReadUInt32();
	EXT_ASSERT(m_Capacity > 0 && m_Capacity < 250000);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::Read(CDataFile* _pDataFile)
{
	m_Name.Read(_pDataFile);
#ifdef	ACTIVESTADIUMPLANS
	m_StandData.Read(_pDataFile);
#else
    _pDataFile->Read(&m_Capacity, sizeof(m_Capacity));
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::Write(CDataFile* _pDataFile)
{
	m_Name.Write(_pDataFile);
#ifdef	ACTIVESTADIUMPLANS
	m_StandData.Write(_pDataFile);
#else
    _pDataFile->Write(&m_Capacity, sizeof(m_Capacity));
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::GetName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CStadium::GetName()
{
	return m_Name;
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::GetCapacity
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CStadium::GetCapacity()
{
#ifdef	ACTIVESTADIUMPLANS
	return m_StandData.GetTotalCapacity();
#else
	return m_Capacity;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::GetCapacityStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CExtString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CExtString CStadium::GetCapacityStr()
{
	CExtString str;
	str.FormatLargeNumber(GetCapacity());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::SetCapacity
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::SetCapacity(const uint _NewCapacity)
{
	EXT_ASSERT(_NewCapacity > 0);
#ifdef	ACTIVESTADIUMPLANS
	m_StandData.SetTotalCapacity(_NewCapacity);
#else
    m_Capacity = _NewCapacity;
#endif
}


#ifdef	ACTIVESTADIUMPLANS
/*------------------------------------------------------------------------------
	Method:   	CStadium::LowerCapacity
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::LowerCapacity(const uint x)
{
	m_StandData.LowerTotalCapacity(x);
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::RaiseCapacity
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadium::RaiseCapacity(const uint x)
{
	m_StandData.RaiseTotalCapacity(x);
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::StandCapacity
	Access:    	public 
	Parameter: 	const ushort WhichStand
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CStadium::StandCapacity(const ushort WhichStand)
{
	return m_StandData.GetCapacity(WhichStand);
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::GetStandPtr
	Access:    	public 
	Parameter: 	void
	Returns:   	CStandList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStandList& CStadium::GetStandPtr()
{
	return m_StandData;
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::StandsUnderRefurbishment
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int	CStadium::StandsUnderRefurbishment()
{
	int nNumberOfStands = 0;
	for (int LoopCount = 0; LoopCount < NUMBEROFSTANDS; LoopCount++)
	{
		if (m_StandData.BuildingStatus(LoopCount) == REFURBISH)
		{
			nNumberOfStands++;
		}
	}
	return nNumberOfStands;
}


/*------------------------------------------------------------------------------
	Method:   	CStadium::DoEndOfWeek
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CStadium::DoEndOfWeek()
{
	return m_StandData.DoEndOfWeek();
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CStadiumList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadiumList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_STADIUM, TEXT("FDDATA"));
	m_HowMany = theResource.ReadUInt16();
	m_pList = FD_ENGINE_NEW CStadium[m_HowMany];
    for (int LoopCount = 0; LoopCount < m_HowMany; LoopCount++)
	{
		m_pList[LoopCount].DoInitialiseFromResource(theResource);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CStadiumList::~CStadiumList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CStadiumList::~CStadiumList()
{
    delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CStadiumList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadiumList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	for (int LoopCount = 0; LoopCount < m_HowMany; LoopCount++)
	{
        m_pList[LoopCount].Read(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CStadiumList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStadiumList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    for (int LoopCount  = 0; LoopCount < m_HowMany; LoopCount++)
	{
        m_pList[LoopCount].Write(_pDataFile);
	}
}

