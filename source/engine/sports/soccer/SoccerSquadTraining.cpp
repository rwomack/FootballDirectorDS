
 /*

    Program:- SoccerSquadTraining.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "SoccerSquadTraining.h"


/*------------------------------------------------------------------------------
	Method:   	CTrainingItem::GetPercentage
	Access:    	protected
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CTrainingItem::GetPercentage()
{
	return m_Percentage;
}


/*------------------------------------------------------------------------------
	Method:   	CTrainingItem::GetSelection
	Access:    	protected
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CTrainingItem::GetSelection()
{
	return m_Selection;
}


/*------------------------------------------------------------------------------
	Method:   	CTrainingItem::SetPercentage
	Access:    	protected
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTrainingItem::SetPercentage(const byte _Value)
{
	m_Percentage = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CTrainingItem::SetSelection
	Access:    	protected
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTrainingItem::SetSelection(const byte _Value)
{
	m_Selection = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CTrainingItem::DoInitialise
	Access:    	protected
	Parameter: 	const byte Percentage
	Parameter: 	const byte Selection
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTrainingItem::DoInitialise(const byte _Percentage /*= 0*/, const byte _Selection /*= 0*/)
{
	m_Percentage = _Percentage;
	m_Selection = _Selection;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::CSquadTraining
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSquadTraining::CSquadTraining()
{
    m_TeamTrainingHours = 20;
    m_5aSidePercent = 20;
    m_Tactics.DoInitialise(40);
    m_SetPiece.DoInitialise(20);
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::~CSquadTraining
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSquadTraining::~CSquadTraining()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::GetTeamTrainingHours
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSquadTraining::GetTeamTrainingHours()
{
	return m_TeamTrainingHours;
}


 //---------------------------------------------------------------------------------------
CString CSquadTraining::GetSetPieceToTrainStr(const ushort _SetPieceID)
{
	uint SetPieceToTrainStrId[] =
	{
    IDS_OFFSIDETRAP,
    IDS_COUNTERATTACK,
    IDS_TAKINGCORNERS,
    IDS_DEFENDINGCORNERS,
    IDS_FREEKICKS,
    IDS_PENALTIES,
    IDS_LONGBALL,
    IDS_PASSINGFOOTBALL,
    IDS_DRIBBLING
	};
	CString strMessage;
	strMessage.LoadString(SetPieceToTrainStrId[_SetPieceID]);
	return strMessage;
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::Get5aSidePercent
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSquadTraining::Get5aSidePercent()
{
	return m_5aSidePercent;
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::Get5aSideHours
	Access:    	public
  Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CSquadTraining::Get5aSideHours()
{
  return static_cast<float>(Get5aSidePercent()) * (static_cast<float>(GetTeamTrainingHours()) / 100);
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::GetRestingPercent
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSquadTraining::GetRestingPercent()
{
	return 100 -(m_SetPiece.GetPercentage() + m_Tactics.GetPercentage() + m_5aSidePercent);
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::GetTacticsToTrainHours
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSquadTraining::GetTacticsToTrainHours()
{
  float Hours = static_cast<float>(GetTeamTrainingHours()) / 100;
	return static_cast<byte>(GetTactics().GetPercentage() * Hours);
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::SetTeamTrainingHours
	Access:    	public
	Parameter: 	byte _Percent
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSquadTraining::SetTeamTrainingHours(byte _Percent)
{
	m_TeamTrainingHours = _Percent;
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::Set5aSidePercent
	Access:    	public
	Parameter: 	byte _Percent
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSquadTraining::Set5aSidePercent(byte _Percent)
{
	m_5aSidePercent = _Percent;
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::GetSetPiece
	Access:    	public
	Returns:   	CTrainingItem&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTrainingItem& CSquadTraining::GetSetPiece()
{
	return m_SetPiece;
}


/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::GetTactics
	Access:    	public
	Returns:   	CTrainingItem&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTrainingItem& CSquadTraining::GetTactics()
{
	return m_Tactics;
}

/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::Write
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Saves the state of the object.
------------------------------------------------------------------------------*/
void CSquadTraining::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_TeamTrainingHours, sizeof(m_TeamTrainingHours));
	_pDataFile->Write(&m_5aSidePercent, sizeof(m_5aSidePercent));
	_pDataFile->Write(&m_Tactics, sizeof(m_Tactics));
	_pDataFile->Write(&m_SetPiece, sizeof(m_SetPiece));
}

/*------------------------------------------------------------------------------
	Method:   	CSquadTraining::Read
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Loads the state of the object.
------------------------------------------------------------------------------*/
void CSquadTraining::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_TeamTrainingHours, sizeof(m_TeamTrainingHours));
	_pDataFile->Read(&m_5aSidePercent, sizeof(m_5aSidePercent));
	_pDataFile->Read(&m_Tactics, sizeof(m_Tactics));
	_pDataFile->Read(&m_SetPiece, sizeof(m_SetPiece));
}

