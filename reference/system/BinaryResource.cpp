
/*

	File:- BinaryResource.cpp
  
	Implementation of the CBinaryResource class.
	
	(c) Rw Software 2004 - 2008
	 
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
*/


#include "Stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::CBinaryResource
	Access:    	public 
	Parameter: 	HMODULE _hModule
	Parameter: 	const uint _ResourceID
	Parameter: 	LPCTSTR _pcszClass
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBinaryReader::CBinaryReader(HMODULE _hModule, const uint _ResourceID, LPCTSTR _pcszClass)
{
	LoadData(_hModule, _ResourceID, _pcszClass);
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::~CBinaryResource
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBinaryReader::~CBinaryReader()
{
	UnloadData();
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::LoadData
	Access:    	public 
	Parameter: 	HMODULE _hModule
	Parameter: 	uint _ResourceID
	Parameter: 	LPCTSTR _pcszClass
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::LoadData(HMODULE _hModule, uint _ResourceID, LPCTSTR _pcszClass)
{
#ifdef RESOURCESFROMFILE
		m_LoadFromFile = true;
	switch (_ResourceID)
	{
		case IDR_CAREERPATH:
			m_File = fopen(theApp.szAddPath("CareerPath.bin"), "rb");
			break;	
		case IDR_CHARITYSHIELD:
			m_File = fopen(theApp.szAddPath("CharityShield.bin"), "rb");
			break;
		case IDR_CLUB:
			m_File = fopen(theApp.szAddPath("Clubs.bin"), "rb");
			break;
		case IDR_COUNTRY:
			m_File = fopen(theApp.szAddPath("Country.bin"), "rb");
			break;
		case IDR_CUP:
			m_File = fopen(theApp.szAddPath("Cup.bin"), "rb");
			break;
		case IDR_DIVISION:
			m_File = fopen(theApp.szAddPath("Division.bin"), "rb");
			break;
		case IDR_DIVISIONSTRENGTH:
			m_File = fopen(theApp.szAddPath("DivisionStrength.bin"), "rb");
			break;
		case IDR_FEDERATION:
			m_File = fopen(theApp.szAddPath("Federation.bin"), "rb");
			break;
		case IDR_FEDERATIONCUP:
			m_File = fopen(theApp.szAddPath("FederationCup.bin"), "rb");
			break;
		case IDR_FIXT20:
			m_File = fopen(theApp.szAddPath("Fixt20.bin"), "rb");
			break;
		case IDR_FIXT24:
			m_File = fopen(theApp.szAddPath("Fixt24.bin"), "rb");
			break;
		case IDR_FORMATION:
			m_File = fopen(theApp.szAddPath("Formation.bin"), "rb");
			break;
		case IDR_INJURYEVENTTYPE:
			m_File = fopen(theApp.szAddPath("InjuryEventType.bin"), "rb");
			break;
		case IDR_INJURYTYPE:
			m_File = fopen(theApp.szAddPath("InjuryType.bin"), "rb");
			break;
		case IDR_MANAGER:
			m_File = fopen(theApp.szAddPath("Managers.bin"), "rb");
			break;
		case IDR_PLAYER:
			m_File = fopen(theApp.szAddPath("Players.bin"), "rb");
			break;
		case IDR_PLAYOFF:
			m_File = fopen(theApp.szAddPath("Playoff.bin"), "rb");
			break;
		case IDR_POPULARITY:
			m_File = fopen(theApp.szAddPath("Popularity.bin"), "rb");
			break;
		case IDR_SKILLMONEYVALUE:
			m_File = fopen(theApp.szAddPath("SkillMoneyValue.bin"), "rb");
			break;
		case IDR_STADIUM:
			m_File = fopen(theApp.szAddPath("Stadiums.bin"), "rb");
			break;
		case IDR_SURNAME:
			m_File = fopen(theApp.szAddPath("Surnames.bin"), "rb");
			break;
		default:
			m_LoadFromFile = false;
			break;
	};

	if (m_LoadFromFile == false)
	{
	
#endif
	m_dwSize = 0;
	m_hGlobal = 0;
	m_pData = 0;
	
	if(_hModule == 0)
	{
		_hModule = GetModuleHandle(null);
	}
	m_hrsrc = FindResource(_hModule, (LPCTSTR)_ResourceID, _pcszClass);
	if(m_hrsrc == 0)
	{
		EXT_ASSERT(false);
		return;
	}
	m_dwSize = SizeofResource(_hModule, m_hrsrc);
	m_hGlobal = LoadResource(_hModule, m_hrsrc);
	m_pData = (PBYTE) LockResource(m_hGlobal);
	m_pCurrentPosition = m_pData;
#ifdef RESOURCESFROMFILE
	}
	else
	{
		EXT_ASSERT(m_File);
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::UnloadData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::UnloadData()
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		fclose(m_File);
	}
	else

#endif
	if(m_hGlobal)
	{
		DeleteObject(m_hGlobal);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::IsLoaded
	Access:    	public const 
	Returns:   	const bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CBinaryReader::IsLoaded() const
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		return (m_File != null);
	}
#endif
	return (m_pData != null);
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::GetData
	Access:    	public 
	Parameter: 	void
	Returns:   	PBYTE
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
PBYTE CBinaryReader::GetData()
{
	return m_pData;
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::GetSize
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
uint CBinaryReader::GetSize() const
{
	return m_dwSize;
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::SetUserData
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::SetUserData(const uint _Value)
{
	m_dwUser = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::GetUserData
	Access:    	public const 
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CBinaryReader::GetUserData() const
{
	return m_dwUser;
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadBytes
	Access:    	public 
	Parameter: 	byte* _Dest
	Parameter: 	const ushort _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::ReadBytes(byte* _Dest, const ushort _HowMany)
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		_pDataFile->Read(_Dest, 1, _HowMany, m_File);
		return;
	}
#endif
	memcpy(_Dest, m_pCurrentPosition, _HowMany);
	m_pCurrentPosition += _HowMany;
}
	

/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadUInt8
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CBinaryReader::ReadByte()
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		byte temp;
		_pDataFile->Read(&temp, 1, 1, m_File);
		return temp;
	}
#endif
	return (byte) *(m_pCurrentPosition++);
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadUInt8Array
	Access:    	public 
	Parameter: 	byte* _Val
	Parameter: 	uint _Count
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::ReadUInt8Array(byte* _Val, uint _Count)
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		_pDataFile->Read(_Val, 1, _Count, m_File);
		return;
	}
#endif
	for (uint LoopCount = 0; LoopCount < _Count; LoopCount++)
	{
		*_Val++ = (byte) *(m_pCurrentPosition++);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadUInt16Array
	Access:    	public 
	Parameter: 	ushort* _Val
	Parameter: 	uint _Count
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBinaryReader::ReadUInt16Array(ushort* _Val, uint _Count)
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		_pDataFile->Read(_Val, 2, _Count, m_File);
		return;
	}
#endif
	byte teb;
	for (uint LoopCount = 0; LoopCount < _Count; LoopCount++)
	{
		teb = *m_pCurrentPosition++;
		*_Val++ = teb + (byte) *(m_pCurrentPosition++);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadUInt16
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CBinaryReader::ReadUInt16()
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		ushort temp;
		_pDataFile->Read(&temp, 2, 1, m_File);
		return temp;
	}
#endif
	byte teb = *m_pCurrentPosition++;
	ushort val = teb + (*m_pCurrentPosition++ << 8);
	return val;
}


/*------------------------------------------------------------------------------
	Method:   	CBinaryResource::ReadUInt32
	Access:    	public 
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CBinaryReader::ReadUInt32()
{
#ifdef RESOURCESFROMFILE
	if (m_LoadFromFile == true)
	{
		uint temp;
		_pDataFile->Read(&temp, 4, 1, m_File);
		return temp;
	}
#endif
	byte teb = *m_pCurrentPosition++;
	uint val = teb;
	teb = *m_pCurrentPosition++;
	val += teb << 8;
	teb = *m_pCurrentPosition++;
	val += teb << 16;
	teb = *m_pCurrentPosition++;
	val += teb << 24;
	return val;
}
