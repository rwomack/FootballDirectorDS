
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

  	float          			GetRatingNegativeMovement() const;
  	float          			GetRatingPositiveMovement() const;
  	float          			GetRating() const;
  	void          			SetRatingPositiveMovement(const float _RatingPositiveMovement);
  	void          			SetRatingNegativeMovement(const float _RatingNegativeMovement);
  	void         	 		SetRating(const float _Rating);
	void					DoResetVariables();
  	void          			DoRaiseRating(const float _HowMany);
  	void          			DoLowerRating(const float _HowMany);
	bool					IsNull() const;

protected:
  	float          			m_Rating;
	float          			m_RatingPositiveMovement;
  	float          			m_RatingNegativeMovement;
};

#endif // !defined(AFX_RATING_H__F2A039B5_6470_4F15_8016_D2FE26B32B31__INCLUDED_)
