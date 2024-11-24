
 /*

	Rating.h: interface for the CRating class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATING_H__F2A039B5_6470_4F15_8016_D2FE26B32B31__INCLUDED_)
#define AFX_RATING_H__F2A039B5_6470_4F15_8016_D2FE26B32B31__INCLUDED_


class CRating  
{
public:
	CRating();
	~CRating();

    // IO Methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

	double					GetRatingNegativeMovement() const;
	double					GetRatingPositiveMovement() const;
	double					GetRating() const;
	void					SetRatingPositiveMovement(const double _RatingPositiveMovement);
	void					SetRatingNegativeMovement(const double _RatingNegativeMovement);
	void					SetRating(const double _Rating);
	void					DoResetVariables();
	void					DoRaiseRating(const double _HowMany);
	void					DoLowerRating(const double _HowMany);
	bool					IsNull() const;

protected:
	double					m_Rating;
	double					m_RatingPositiveMovement;
	double					m_RatingNegativeMovement;
};

#endif // !defined(AFX_RATING_H__F2A039B5_6470_4F15_8016_D2FE26B32B31__INCLUDED_)
