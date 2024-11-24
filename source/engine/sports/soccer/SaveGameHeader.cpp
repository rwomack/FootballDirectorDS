
 /*

	File:- SaveGameHeader.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "SaveGameHeader.h"

CSaveGameFile::CSaveGameFile()
	: CSaveGameHeader()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameFile::SetFileName
	Access:    	public
	Parameter: 	CString & _FileName
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSaveGameFile::SetFileNameFromID(const int _NameID)
{
	m_FileName.LoadString(_NameID);
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameFile::GetFileName
	Access:    	public
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CSaveGameFile::GetFileName()
{
	return m_FileName;
}
/////////////////////////////////////////////////////////////////////////////

/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::CSaveGameHeader
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSaveGameHeader::CSaveGameHeader()
{
	m_KitID = 0xff;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSaveGameHeader::Read(CDataFile* _pDataFile)
{
  _pDataFile->Read(&m_KitID, sizeof(m_KitID));
  _pDataFile->Read(&m_Calendar, sizeof(m_Calendar));

	byte nUserClubNameSize;
  _pDataFile->Read(&nUserClubNameSize, sizeof(nUserClubNameSize));
	m_UserClubName.Empty();

  //LPTSTR junk = m_UserClubName.GetBufferSetLength(nUserClubNameSize);  // pointer to the CString
  //_pDataFile->Read(junk, nUserClubNameSize);
  //m_UserClubName.ReleaseBuffer();
}

/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSaveGameHeader::Write(CDataFile* _pDataFile)
{
  _pDataFile->Write(&m_KitID, sizeof(m_KitID));
  _pDataFile->Write(&m_Calendar, sizeof(m_Calendar));

	byte nUserClubNameSize = m_UserClubName.GetLength();
  _pDataFile->Write(&nUserClubNameSize, sizeof(nUserClubNameSize));
  //char* pAnsiString = FD_ENGINE_NEW char[nUserClubNameSize + 1];
  //_pDataFile->Write(pAnsiString, nUserClubNameSize);
  //delete pAnsiString;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::GetClubName
	Access:    	public
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CSaveGameHeader::GetClubName()
{
	return m_UserClubName;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::GetCalendar
	Access:    	public
	Returns:   	CCalendar&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CSaveGameHeader::GetCalendar()
{
	return m_Calendar;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::GetKitID
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSaveGameHeader::GetKitID()
{
	return m_KitID;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::IsEmpty
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CSaveGameHeader::IsEmpty()
{
	if (m_KitID == 0xff)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CSaveGameHeader::SetDetails
	Access:    	public
	Parameter: 	const byte _KitID
	Parameter: 	CString _ClubName
	Parameter: 	CCalendar & _Calendar
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSaveGameHeader::SetDetails(const byte _KitID, CString _ClubName, CCalendar& _Calendar)
{
	m_KitID = _KitID;
	m_UserClubName = _ClubName;
	m_Calendar = _Calendar;
}
