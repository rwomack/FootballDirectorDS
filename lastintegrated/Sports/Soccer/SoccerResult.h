
/*

	File:- SoccerResult.h

	Result class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#ifndef __RESULT_H__
#define __RESULT_H__


class CSoccerResultDetails
{
public:
	// Constructors
    CSoccerResultDetails();

    // Methods
	ushort					GetClubID() const;
    void					SetClubDetails(ushort nClubID);
	CString					GetName();
    bool					IsUserControlledClub();

    // Variables
 private:
	ushort					m_ClubID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSoccerResult
{
 public:
	// Constructors
	 CSoccerResult();
	 CSoccerResult(const ushort _HomeClubID, const ushort _AwayClubID);

	void					DoAddClubIDs(const ushort _HomeClubID, const ushort _AwayClubID);
	uint					GetAttendance() const;
	CFixture*				GetFixture();
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
