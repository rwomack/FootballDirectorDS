

 /*

    Class:- PlayerCareerPath


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPath::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerCareerPath::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_Frequency = _Resource.ReadByte();
	for (uint n = 0; n < PLAYERAGERANGE; n++)
	{
		m_Positive[n] = _Resource.ReadByte();
		m_Negative[n] = _Resource.ReadByte();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPath::DoCalculateWhichElementForAge
	Access:    	public const 
	Parameter: 	const byte aAge
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerCareerPath::DoCalculateWhichElementForAge(const byte _Age) const
{
	return max(min(_Age, 40), 16) - 16;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPath::GetFrequency
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPath::GetFrequency() const
{ 
	return m_Frequency; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPath::PositiveMultiplier
	Access:    	public const 
	Parameter: 	const byte aAge
	Returns:   	double
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
double CPlayerCareerPath::PositiveMultiplier(const byte _Age) const
{
	return (double)m_Positive[DoCalculateWhichElementForAge(_Age)] / 100;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPath::NegativeMultiplier
	Access:    	public const 
	Parameter: 	const byte aAge
	Returns:   	const double
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
double CPlayerCareerPath::NegativeMultiplier(const byte _Age) const
{
	return m_Negative[DoCalculateWhichElementForAge(_Age)];
}
//////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::CPlayerCareerPathList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerCareerPathList::CPlayerCareerPathList()
{
	CBinaryReader theResource(0, IDR_CAREERPATH, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);
	m_pList = new CPlayerCareerPath[CExtArray::GetSize()];

	m_FirstOutfieldId = theResource.ReadByte();
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
        m_pList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::~CPlayerCareerPathList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerCareerPathList::~CPlayerCareerPathList()
{
	delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::GetFirstOutfieldID
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPathList::GetFirstOutfieldID() const					
{ 
	return m_FirstOutfieldId; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::RandomGoalkeeperCareerPath
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPathList::RandomGoalkeeperCareerPath()
{
	return DoFindPath();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::DoFindPath
	Access:    	public 
	Parameter: 	byte _FirstPathID
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPathList::DoFindPath(byte _FirstPathID /*= 0*/)
{
	int Value = RandomNumber.IntLessThan(99);
	do
	{
		if (Value < GetAt(_FirstPathID)->GetFrequency())
		{
			break;
		}
		_FirstPathID++;
	}
	while(1);
	return _FirstPathID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::RandomOutfieldCareerPath
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPathList::RandomOutfieldCareerPath()
{
	return DoFindPath(GetFirstOutfieldID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::RandomCareerPath
	Access:    	public 
	Parameter: 	ePlayerPosition _Position
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerCareerPathList::RandomCareerPath(ePlayerPosition _Position)
{
	if (_Position == GOALKEEPER)
	{
		return RandomGoalkeeperCareerPath();
	}
	else
	{
		return RandomOutfieldCareerPath();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerCareerPathList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CPlayerCareerPath*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerCareerPath* CPlayerCareerPathList::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber <= GetSize());
	return static_cast<CPlayerCareerPath*> (&m_pList[_ElementNumber]);
}
