

/*
	SaveGameHeader


	Football Director

	(c) Sports Director Ltd 2008- 

  ---------------------------------------------------------------------------------------
*/


#ifndef __SAVEGAMEHEADER_H__
#define __SAVEGAMEHEADER_H__


class CSaveGameHeader
{
public:
	CSaveGameHeader();
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);
	CString&				GetClubName();
	CCalendar&				GetCalendar();
	byte					GetKitID();
	bool					IsEmpty();
	void					SetDetails(const byte _KitID, CString _ClubName, CCalendar& _Calendar);

protected:
	byte					m_KitID;
	CString					m_UserClubName;
	CCalendar				m_Calendar;
};
/////////////////////////////////////////////////////////////////////////////


class CSaveGameFile : public CSaveGameHeader
{
public:
	CSaveGameFile();

	CString&				GetFileName();
	void					SetFileNameFromID(const int _NameID);

protected:
	CString					m_FileName;
};

#endif