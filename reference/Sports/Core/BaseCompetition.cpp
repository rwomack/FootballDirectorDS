
 /*

	Program:- BaseCompetition.cpp

    The base competition class routines

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"





/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::~CBaseCompetition
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBaseCompetition::~CBaseCompetition() 
{
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::DoDailyTasks
	Access:    	public 
	Parameter: 	CActiveCountry & _ActiveCountry
	Parameter: 	bool _UserClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::DoDailyTasks(CActiveCountry& _ActiveCountry, bool _UserClub)
{
	if (_UserClub == true)
	{
		WorldData().GetTransferDeadlineSystem().GetAt(m_TransferDeadlineSystemID).DoAnnouncements(_ActiveCountry);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::CanBuyPlayer
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CBaseCompetition::CanBuyPlayer()
{
	return WorldData().GetTransferDeadlineSystem().GetAt(m_TransferDeadlineSystemID).CanBuyPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::CanLoanPlayer
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CBaseCompetition::CanLoanPlayer()
{
	return WorldData().GetTransferDeadlineSystem().GetAt(m_TransferDeadlineSystemID).CanLoanPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::DoInitialiseFromResource(CBinaryReader& _Resource)
{
#ifdef TROPHYID
    m_TrophyID = _Resource.ReadByte();
#endif
	CAvailableSubstitutes::DoInitialiseFromResource(_Resource);
	OnInitialiseName(_Resource);
	m_TransferDeadlineSystemID = _Resource.ReadUInt16();
	m_PercentageFAGateReceipts = _Resource.ReadByte();
	m_PercentageAwayGateReceipts = _Resource.ReadByte();
	//DEBUG_OUTPUT(("%s : PercentageAwayGateReceipts %d : PercentageFAGateReceipts %d"), (GetName(), m_PercentageAwayGateReceipts, m_PercentageFAGateReceipts));
	m_WinningClubID = _Resource.ReadUInt16();
	m_RunnersUpClubID = _Resource.ReadUInt16();
#ifdef NEWSPAPERON
    m_NewsEvents.DoResetVariables();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetWinningClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CBaseCompetition::GetWinningClubID() 
{ 
	return m_WinningClubID; 
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetRunnersUpClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CBaseCompetition::GetRunnersUpClubID() 
{ 
	return m_RunnersUpClubID; 
}


#ifdef TROPHYID
/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetTrophyID
	Access:    	public const 
	Parameter: 	void
	Returns:   	const byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CBaseCompetition::GetTrophyID() const
{ 
	return m_TrophyID; 
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::OnInitialiseName
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::OnInitialiseName(CBinaryReader& _Resource)
{
    CExtString Title;
	Title.ReadFromResource(_Resource);
	m_TitleID = WorldData().GetText().DoFindStringID(Title);
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CBaseCompetition::GetName()										
{ 
	return WorldData().GetText().GetString(m_TitleID); 
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetPercentageAwayGateReceipts
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CBaseCompetition::GetPercentageAwayGateReceipts()
{
	return m_PercentageAwayGateReceipts;
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::GetPercentageFAGateReceipts
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CBaseCompetition::GetPercentageFAGateReceipts()
{
	return m_PercentageFAGateReceipts;
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
#ifdef TROPHYID
    _pDataFile->Read(&m_TrophyID, sizeof m_TrophyID);
#endif
	CAvailableSubstitutes::Read(_pDataFile);
	_pDataFile->Read(&m_WinningClubID, sizeof m_WinningClubID);
	_pDataFile->Read(&m_RunnersUpClubID, sizeof m_RunnersUpClubID);
#ifdef NEWSPAPERON
	m_NewsEvents.Read(File);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
#ifdef TROPHYID
    _pDataFile->Write(&m_TrophyID, sizeof m_TrophyID);
#endif
	CAvailableSubstitutes::Write(_pDataFile);
	_pDataFile->Write(&m_WinningClubID, sizeof m_WinningClubID);
	_pDataFile->Write(&m_RunnersUpClubID, sizeof m_RunnersUpClubID);
#ifdef NEWSPAPERON
	m_NewsEvents.Write(File);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::DoAddChampions
	Access:    	public 
	Parameter: 	const ushort ClubNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::DoAddChampions(const ushort _ClubID)
{
	m_WinningClubID = _ClubID;
	//@@NewsPaperEventPtr()->SetChampions(ClubNum);
}


/*------------------------------------------------------------------------------
	Method:   	CBaseCompetition::DoAddRunnersUp
	Access:    	public 
	Parameter: 	const ushort ClubNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBaseCompetition::DoAddRunnersUp(const ushort _ClubID)
{
	m_RunnersUpClubID = _ClubID;
//@@	NewsPaperEventPtr()->SetRunnersUp(ClubNum);
}
