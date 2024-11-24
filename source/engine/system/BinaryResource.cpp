
/*

	File:- BinaryResource.cpp

	Implementation of the CBinaryResource class.

	(c) Rw Software 2004 - 2007

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include <stdafx.h>
#include "footballdirector.h"

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#include <nitro/itcm_begin.h>
#endif

CDataFile::CDataFile() : m_bInBitMode(false), m_uiBitBuffer(0), m_uiBitCounter(0)
{
}

CDataFile::~CDataFile()
{
}

bool         CDataFile::IsValid()
{
  return false;
}

bool         CDataFile::Reading(void)
{
  return false;
}

bool         CDataFile::Writing(void)
{
  return false;
}

unsigned int CDataFile::Access(void *ptr, unsigned int size)
{
  if( Reading() )
  {
    return Read( ptr, size );
  } else
  {
    return Write( ptr, size );
  }
}

void CDataFile::EnterBitMode(const bool bWriting)
{
	EXT_ASSERT(m_bInBitMode == false);
	m_bInBitMode = true;
	m_uiBitBuffer = 0;
	m_uiBitCounter = 0;
}

void CDataFile::ExitBitMode()
{
	EXT_ASSERT(m_bInBitMode == true);
	m_bInBitMode = false;

	if (Writing() && (m_uiBitCounter > 0))
	{
		// Write out the remainder, if there is any.
		m_uiBitBuffer <<= (32 - m_uiBitCounter);
		Write(&m_uiBitBuffer, sizeof(m_uiBitBuffer));
	}
}

unsigned int CDataFile::ReadBits(const unsigned int uiNumBits)
{
	EXT_ASSERT(uiNumBits > 0);
	EXT_ASSERT(m_bInBitMode == true);
	EXT_ASSERT(Reading());

	unsigned int uiResult = 0;
	if (m_uiBitCounter < uiNumBits)
	{
		uiResult = m_uiBitBuffer & ((1 << m_uiBitCounter) - 1);
		Read(&m_uiBitBuffer, sizeof(m_uiBitBuffer));

		const unsigned int uiBitsRemaining = uiNumBits - m_uiBitCounter;
		m_uiBitCounter = 32;
		uiResult <<= uiBitsRemaining;
		uiResult |= m_uiBitBuffer >> (m_uiBitCounter - uiBitsRemaining);
		m_uiBitCounter -= uiBitsRemaining;
	}
	else
	{
		uiResult = (m_uiBitBuffer >> (m_uiBitCounter - uiNumBits)) & ((1 << uiNumBits) - 1);
		m_uiBitCounter -= uiNumBits;
	}

	return uiResult;
}

unsigned int CDataFile::WriteBits(const unsigned int uiBitsToWrite, const unsigned int uiNumBits)
{
	EXT_ASSERT(uiNumBits > 0);
	EXT_ASSERT(m_bInBitMode == true);
	EXT_ASSERT(Writing());
	EXT_ASSERT((uiBitsToWrite & ((1 << uiNumBits) - 1)) == uiBitsToWrite);

	if ((m_uiBitCounter + uiNumBits) <= 32)
	{
		m_uiBitBuffer <<= uiNumBits;
		m_uiBitBuffer |= uiBitsToWrite;
		m_uiBitCounter += uiNumBits;

		return 0;
	}
	else
	{
		// Write the first part, and then the remainder.
		const unsigned int uiFirstPartBits = 32 - m_uiBitCounter;
		const unsigned int uiSecondPartBits = (m_uiBitCounter + uiNumBits) - 32;

		m_uiBitBuffer <<= uiFirstPartBits;
		m_uiBitBuffer |= (uiBitsToWrite >> uiSecondPartBits);

		Write(&m_uiBitBuffer, sizeof(m_uiBitBuffer));

		m_uiBitBuffer = (uiBitsToWrite & ((1 << uiSecondPartBits) - 1));
		m_uiBitCounter = uiSecondPartBits;

		return 1;
	}
}

class CDataFileLocal : public CDataFile
{
  lFILE_HANDLE m_hnd;

public:
  CDataFileLocal( const char *file )
  {
    lLogFmt("CDataFileLocal: %s\n", file);
    m_hnd = lPlatform_FileOpen( file, L_IO_ACCESS_READ );

    if( !IsValid() )
    {
      lLogFmt("   FAILED!\n");
    }
  }

  virtual ~CDataFileLocal()
  {
    lPlatform_FileClose(m_hnd);
  }

  virtual bool         IsValid()
  {
    return ( m_hnd != lINVALID_FILE_HANDLE );
  }

  virtual bool         Reading(void)
  {
    return true;
  }

  virtual unsigned int Read(void *data, unsigned int size)
  {
    return lPlatform_FileRead( m_hnd, data, size );
  }

  virtual unsigned int Write(const void *data, unsigned int size)
  {
    return 0;
  }

};


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
CBinaryReader::CBinaryReader(HMODULE _hModule, const uint _ResourceID, LPCTSTR _pcszClass) : m_File(NULL), m_hGlobal(NULL)
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
	UnloadData();

#ifdef RESOURCESFROMFILE
  m_LoadFromFile = true;

	switch (_ResourceID)
	{
		case IDR_CAREERPATH:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("CareerPath.bin"));
			break;
		case IDR_CHARITYSHIELD:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("CharityShield.bin"));
			break;
		case IDR_CLUB:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Clubs.bin"));
			break;
		case IDR_COUNTRY:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Country.bin"));
			break;
		case IDR_CUP:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Cup.bin"));
			break;
		case IDR_DIVISION:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Division.bin"));
			break;
    case IDR_DIVISIONSTRENGTH:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("DivisionStrength.bin"));
			break;
		case IDR_FEDERATION:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Federation.bin"));
			break;
		case IDR_FEDERATIONCUP:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("FederationCup.bin"));
			break;
		case IDR_FIXT20:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Fixt20.bin"));
			break;
		case IDR_FIXT24:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Fixt24.bin"));
			break;
		case IDR_FORMATION:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Formation.bin"));
			break;
		case IDR_INJURYEVENTTYPE:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("InjuryEventType.bin"));
			break;
		case IDR_INJURYTYPE:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("InjuryType.bin"));
			break;
		case IDR_MANAGER:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Managers.bin"));
			break;
		case IDR_PLAYER:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Players.bin"));
			break;
		case IDR_PLAYOFF:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Playoff.bin"));
			break;
		case IDR_POPULARITY:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Popularity.bin"));
			break;
    case IDR_PLAYERLOANS:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("PlayersOnLoan.bin"));
			break;
    case IDR_TRANSFERDEADLINESYSTEM:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("TransferDeadlineSystem.bin"));
			break;
		case IDR_SKILLMONEYVALUE:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("SkillMoneyValue.bin"));
			break;
		case IDR_STADIUM:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Stadiums.bin"));
			break;
    case IDR_TACTIC:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Tactic.bin"));
			break;
		case IDR_SURNAME:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("Surnames.bin"));
			break;
    case IDR_KITSYSTEM:
      m_File = FD_ENGINE_NEW CDataFileLocal(theApp.szAddPath("kits.bin"));
			break;
		default:
			m_LoadFromFile = false;
      lLogFmt("LoadFromFile: Resource %d not found\n", _ResourceID);
			break;
	};

	if (m_LoadFromFile == false)
	{

#endif
#ifndef RESOURCESFROMFILE
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
    FDASSERT(false);
		return;
	}
	m_dwSize = SizeofResource(_hModule, m_hrsrc);
	m_hGlobal = LoadResource(_hModule, m_hrsrc);
	m_pData = (PBYTE) LockResource(m_hGlobal);
	m_pCurrentPosition = m_pData;
#endif
#ifdef RESOURCESFROMFILE
	}
	else
	{
    FDASSERT(m_File);
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
    delete m_File;
	}
//  else
#else
	if(m_hGlobal)
	{
		DeleteObject(m_hGlobal);
	}
#endif
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
    m_File->Read(_Dest, _HowMany);
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
    m_File->Read(&temp, 1);
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
    m_File->Read(_Val, _Count);
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
    m_File->Read(_Val, 2* _Count);
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
    m_File->Read(&temp, 2);
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
    m_File->Read(&temp, 4);
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

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif
