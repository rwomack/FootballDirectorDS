
/*

	File:- SoccerResult.h

	Result class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#ifndef __RESULT_H__
#define __RESULT_H__

#include <SoccerDefines.h>

class CSoccerResultDetails
{
public:
	// Constructors
    CSoccerResultDetails();

    // Methods
	ClubID					GetClubID() const;
    void					SetClubDetails(ClubID nClubID);
	CString					GetName();
    bool					IsUserControlledClub();

    // Variables
 private:
	ClubID					m_ClubID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSoccerResult
{
 public:
	// Constructors
	 CSoccerResult();
	 CSoccerResult(const ClubID _HomeClubID, const ClubID _AwayClubID);

	void					DoAddClubIDs(const ClubID _HomeClubID, const ClubID _AwayClubID);
	uint					GetAttendance() const;
	CFixture*				GetFixture();
	CClub&					GetHomeClub();
	CClub&					GetAwayClub();
	CSoccerResultDetails&	GetHomeClubDetails();
	CSoccerResultDetails&	GetAwayClubDetails();
	CString					GoalsStr();
	void					SetResult(CFixture* _Fixture);

	// Variables
 private:
	CSoccerResultDetails	m_HomeClubDetails;
	CSoccerResultDetails	m_AwayClubDetails;
    CFixture*				m_pFixture;

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSoccerResultList : public CPointerArray<CSoccerResult>
{
public:
    CSoccerResultList();				// Constructor
    ~CSoccerResultList();				// Destructor

	// Methods
    void					SortByHomeClub();
};
#endif
