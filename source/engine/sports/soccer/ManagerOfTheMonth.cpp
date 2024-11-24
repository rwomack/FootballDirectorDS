
/*

		File:- ManagerOfTheMonth.cpp


		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::CManagerOfTheMonth
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerOfTheMonth::CManagerOfTheMonth()
{
	DoInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::CManagerOfTheMonth
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	CString _Month
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerOfTheMonth::CManagerOfTheMonth(CClub* _Club, CString _Month)
{
	DoCreate(_Club, _Month);
}


/*------------------------------------------------------------------------------
Method:   	CManagerOfTheMonth::ClubStr
Access:    	public 
Returns:   	CString
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CString CManagerOfTheMonth::ClubStr()			
{ 
	return WorldData().GetClubByID(m_ClubID).GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::DoCreate
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	CString _Month
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerOfTheMonth::DoCreate(CClub* _Club, CString _Month)
{
	EXT_ASSERT(_Month.IsEmpty() == FALSE);
	EXT_ASSERT_POINTER(_Club, CClub);
    m_Month = _Month;
    m_ManagerNameID = (CPersonName&)_Club->GetManager();
    m_ClubID = _Club->DoFindID();
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerOfTheMonth::Read(CDataFile* _pDataFile)
{
    m_Month.Read(_pDataFile);
    _pDataFile->Read(&m_ManagerNameID, sizeof(CPersonName));
    _pDataFile->Read(&m_ClubID, sizeof(m_ClubID));
 }


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerOfTheMonth::Write(CDataFile* _pDataFile)
{
    m_Month.Write(_pDataFile);
    _pDataFile->Write(&m_ManagerNameID, sizeof(CPersonName));
    _pDataFile->Write(&m_ClubID, sizeof(m_ClubID));
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerOfTheMonth::DoInitialise()
{
    m_Month.Empty();
    m_ClubID.id = NOCLUB;
}


/*------------------------------------------------------------------------------
Method:   	CManagerOfTheMonth::GetClubID
Access:    	public 
Returns:   	ushort
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
ClubID CManagerOfTheMonth::GetClubID()			
{ 
	return m_ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::IsEmpty
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManagerOfTheMonth::IsEmpty()
{
	return m_ClubID.id == NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::MonthStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManagerOfTheMonth::MonthStr()			
{ 
	return m_Month; 
}


/*------------------------------------------------------------------------------
	Method:   	CManagerOfTheMonth::ManagerStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CManagerOfTheMonth::ManagerStr()		
{ 
	return m_ManagerNameID.GetName(); 
}
