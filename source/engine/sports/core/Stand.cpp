
 /*

        Class:- Stand


	Football Director

	(c) RW Software 1994 - 2001

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "Stand.h"



 //--------------------------------------------------------------------------
CStand::CStand(CDataFile* File)
{
	ASSERT (File != NULL);
    //m_PriceBand.Read(File);
    m_Size.Read(File);
    m_Roof.Read(File);
    m_Seat.Read(File);
    m_Box.Read(File);

	m_Capacity = 0;
	m_WeeksToConstruct = 0;
	m_CostToConstruct = 0;
	m_BuildingStatus = NOTHING;
	m_NewSize = 0;
	m_NewRoof = 0;
	m_NewSeat = 0;
	m_NewBox = 0;
}


 //--------------------------------------------------------------------------
CStand::~CStand()
{
}


 //--------------------------------------------------------------------------
CStand::CStand()
{
}


 //---------------------------------------------------------------------------------------
bool CStand::DoEndOfWeek(void)
{
	bool bRet = false;
	if (m_BuildingStatus == REFURBISH || m_BuildingStatus == NEWBUILDING)
	{
		if (--m_WeeksToConstruct == 0)
		{
			m_Size = m_NewSize;
			m_Roof = m_NewRoof;
			m_Seat = m_NewSeat;
			m_Box = m_NewBox;
			m_BuildingStatus = NOTHING;
			bRet = true;
		}
	}
	return bRet;
}

 //---------------------------------------------------------------------------------------
void CStand::Write(CDataFile* File)
{
	ASSERT (File != NULL);
    m_Capacity.Write(File);
    //File->Write(&m_PriceBand, sizeof(m_PriceBand));
    m_Size.Write(File);
    m_Roof.Write(File);
    m_Seat.Write(File);
    m_Box.Write(File);
    m_WeeksToConstruct.Write(File);
    m_CostToConstruct.Write(File);
    m_BuildingStatus.Write(File);
    m_NewSize.Write(File);
    m_NewRoof.Write(File);
    m_NewSeat.Write(File);
    m_NewBox.Write(File);
}


 //---------------------------------------------------------------------------------------
void CStand::Read(CDataFile* File)
{
	ASSERT (File != NULL);
    m_Capacity.Read(File);
    //m_PriceBand.Read(File);
    m_Size.Read(File);
    m_Roof.Read(File);
    m_Seat.Read(File);
    m_Box.Read(File);
    m_WeeksToConstruct.Read(File);
    m_CostToConstruct.Read(File);
    m_BuildingStatus.Read(File);
    m_NewSize.Read(File);
    m_NewRoof.Read(File);
    m_NewSeat.Read(File);
    m_NewBox.Read(File);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //--------------------------------------------------------------------------
CStandList::CStandList()
{
}


 //--------------------------------------------------------------------------
CStandList::~CStandList()
{
	RemoveList();
}


 //--------------------------------------------------------------------------
void CStandList::Create(CDataFile* File)
{
	SetSize(NUMBEROFSTANDS);
    m_Capacity.Read(File);
    for (uint32 ToFind = 0; ToFind < NUMBEROFSTANDS; ToFind ++)
	{
		SetAt(ToFind, FD_ENGINE_NEW CStand(File));
	}
 }


 //---------------------------------------------------------------------------------------
void CStandList::Write(CDataFile* File)
{
	ASSERT (File != NULL);
    m_Capacity.Write(File);
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        static_cast<CStand*> (GetAt(ListPos))->Write(File);
	}
 }


 //---------------------------------------------------------------------------------------
void CStandList::Read(CDataFile* File)
{
	ASSERT (File != NULL);
    m_Capacity.Read(File);
	for (uint32 n = 0; n < GetSize(); n ++)
	{
		GetAt(n)->Read(File);
    }
 }


 //---------------------------------------------------------------------------------------
void CStandList::DeleteAll(void)
{
	CStand* ListPtr;
 	int HowMany = GetSize();
	if (HowMany == 0)
	{
		return;		// None to delete
	}
    for (uint32 ListPos = 0; ListPos < HowMany; ListPos ++)
	{
	    ListPtr = GetAt(ListPos);
		if (ListPtr != NULL)
		{
			delete ListPtr;
			SetAt(ListPos, NULL);
		}
	}
 }


 //---------------------------------------------------------------------------------------
void CStandList::RemoveList(void)
{
	if (GetSize() > 0 )
	{
		DeleteAll();
		RemoveAll();
	}
 }


 //---------------------------------------------------------------------------------------
int CStandList::GetTotalCapacity(void)
{
	return m_Capacity;
}


 //---------------------------------------------------------------------------------------
void CStandList::SetTotalCapacity(int x)
{
	ASSERT (x > 0);
	m_Capacity = x;
}


 //---------------------------------------------------------------------------------------
void CStandList::LowerTotalCapacity(int x)
{
	ASSERT (x > 0);
	m_Capacity -= x;
}


 //---------------------------------------------------------------------------------------
void CStandList::RaiseTotalCapacity(int x)
{
	ASSERT (x > 0);
	m_Capacity += x;
}


 //---------------------------------------------------------------------------------------
int	CStandList::GetCapacity(int WhichStand)
{
	return GetAt(WhichStand)->GetCapacity();
}


 //---------------------------------------------------------------------------------------
/*int	CStandList::GetPriceBandCapacity(int x)
{
	int iRet = 0;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->GetPriceBand() == x)
		{
			iRet+= GetAt(ListPos)->GetCapacity();
		}
	}
	return iRet;
}


 //---------------------------------------------------------------------------------------
uint8 CStandList::GetPriceBand(int x)
{
	return GetAt(x)->GetPriceBand();
}


 //---------------------------------------------------------------------------------------
void CStandList::SetPriceBand(int x, uint8 y)
{
	GetAt(x)->SetPriceBand(y);
}*/


 //---------------------------------------------------------------------------------------
uint8 CStandList::BuildingStatus(int x)
{
	 return GetAt(x)->GetBuildingStatus();
}


 //---------------------------------------------------------------------------------------
bool CStandList::DoEndOfWeek(void)
{
	bool bRet = false;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->DoEndOfWeek() == true)
		{
			m_Capacity += GetAt(ListPos)->GetCapacity();
			GetAt(ListPos)->SetCapacity(0);
			bRet = true;
		}
	}
	return bRet;
}
