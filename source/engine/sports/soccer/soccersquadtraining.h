
/*

	SoccerSquadTraining.h: interface for the CSoccerSquadTraining class.

  
	Football Director

	(c) Rw Software 1994 - 2008
  
	----------------------------------------------------------------------------
*/



#if !defined(AFX_SOCCERSQUADTRAINING_H__26F42DC0_8D28_11D1_B220_0080C883C4EB__INCLUDED_)
#define AFX_SOCCERSQUADTRAINING_H__26F42DC0_8D28_11D1_B220_0080C883C4EB__INCLUDED_


class CTrainingItem
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ***********NOTE************
	// If this class ever gains any virtual methods, you must change CSquadTraining's
	// Read()/Write() to account for this. (The vtable pointer will mess up the
	// I/O as written.
	//////////////////////////////////////////////////////////////////////////

    // Methods
	void				DoInitialise(const byte _Percentage = 0, const byte _Selection = 0);
	byte 				GetSelection();
	byte 				GetPercentage();
	void 				SetSelection(const byte _Value);
	void 				SetPercentage(const byte _Value);

protected:
	byte				m_Percentage;
	byte				m_Selection;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSquadTraining 
{
    // Variables
private:
    byte					m_TeamTrainingHours;
    byte					m_5aSidePercent;
    CTrainingItem			m_Tactics;
    CTrainingItem			m_SetPiece;

public:
	CSquadTraining();
	~CSquadTraining();

    // IO Methods
	void    Write(CDataFile* _pDataFile);
	void    Read(CDataFile* _pDataFile);

    // Methods
	CString					GetSetPieceToTrainStr(const ushort _SetPieceID);
	byte					GetTeamTrainingHours();
	byte					Get5aSidePercent();
  float          Get5aSideHours();
	byte 					GetRestingPercent();
	byte 					GetTacticsToTrainHours();
	void 					SetTeamTrainingHours(byte _Percent);
	void 					Set5aSidePercent(byte _Percent);
	CTrainingItem& 			GetSetPiece();
	CTrainingItem& 			GetTactics();
};

#endif // !defined(AFX_SOCCERSQUADTRAINING_H__26F42DC0_8D28_11D1_B220_0080C883C4EB__INCLUDED_)
