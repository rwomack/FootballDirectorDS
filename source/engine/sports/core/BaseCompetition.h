
/*

	File:- BaseCompetition.h

	C++ Classes

	BaseCompetition class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __BASECOMPETITION_H__
#define __BASECOMPETITION_H__

static const uint		TOTALMATCHES = 106;
class CActiveCountry;

class CBaseCompetition : public CAvailableSubstitutes
{
public:
    ~CBaseCompetition(); 

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	bool					CanBuyPlayer();
	bool					CanLoanPlayer();
	void					DoDailyTasks(CActiveCountry& _ActiveCountry, bool _UserClub);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	CString&				GetName();										
	byte					GetPercentageAwayGateReceipts();
	byte					GetPercentageFAGateReceipts();
	ClubID					GetWinningClubID() const;
	ClubID					GetRunnersUpClubID() const;
	void					OnInitialiseName(CBinaryReader& _Resource);
	void					SetRunnersUpClubID(const ClubID _ClubID);
	void					SetWinningClubID(const ClubID _ClubID);
#ifdef TROPHYID
	byte					GetTrophyID() const;
#endif

    // Variables
protected:
    ushort					m_TitleID;
	ushort					m_TransferDeadlineSystemID;
	byte					m_PercentageAwayGateReceipts;
	byte					m_PercentageFAGateReceipts;
	ClubID					m_WinningClubID;
	ClubID					m_RunnersUpClubID;
#ifdef TROPHYID
	byte					m_TrophyID;
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif