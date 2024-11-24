
/*

	File:- Rating.cpp

	implementation of the CRating class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "Rating.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



/*------------------------------------------------------------------------------
	Method:   	CRating::CRating
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CRating::CRating()
	: m_Rating(0)
	, m_RatingPositiveMovement(0)
	, m_RatingNegativeMovement(0)
{

}


/*------------------------------------------------------------------------------
	Method:   	CRating::~CRating
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRating::~CRating()
{

}


/*------------------------------------------------------------------------------
	Method:   	CRating::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRating::DoResetVariables()
{
	m_Rating = 0;
	m_RatingPositiveMovement = 0;
	m_RatingNegativeMovement = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::IsNull
	Access:    	public
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CRating::IsNull() const
{
	if (m_Rating == 0 && m_RatingPositiveMovement == 0 && m_RatingNegativeMovement == 0)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::GetRating
	Access:    	public
  	Returns:    float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CRating::GetRating() const
{
	return m_Rating;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::GetRatingPositiveMovement
	Access:    	public
  	Returns:    float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CRating::GetRatingPositiveMovement() const
{
	return m_RatingPositiveMovement;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::GetRatingNegativeMovement
	Access:    	public 
	Returns:   	float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CRating::GetRatingNegativeMovement() const
{
	return m_RatingNegativeMovement;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::SetRatingPositiveMovement
	Access:    	public 
	Parameter: 	const float _RatingPositiveMovement
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRating::SetRatingPositiveMovement(const float _RatingPositiveMovement)
{
	m_RatingPositiveMovement = _RatingPositiveMovement;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::SetRatingNegativeMovement
	Access:    	public 
	Parameter: 	const float _RatingNegativeMovement
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRating::SetRatingNegativeMovement(const float _RatingNegativeMovement)
{
	m_RatingNegativeMovement = _RatingNegativeMovement;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::SetRating
	Access:    	public
  	Parameter:  const float _Rating
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRating::SetRating(const float _Rating)
{
	EXT_ASSERT(_Rating >= 0);
	m_Rating = _Rating;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::DoRaiseRating
	Access:    	public
  	Parameter:  const float _HowMany
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRating::DoRaiseRating(const float _HowMany)
{
	m_Rating = min(99, static_cast<int>(GetRating() + _HowMany));
	m_RatingPositiveMovement += _HowMany * 2;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::DoLowerRating
	Access:    	public
  	Parameter:  const float _HowMany
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CRating::DoLowerRating(const float _HowMany)
{
	m_Rating = max(5, static_cast<int>(GetRating() - _HowMany));
	m_RatingNegativeMovement += _HowMany * 2;
}


/*------------------------------------------------------------------------------
	Method:   	CRating::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRating::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
    _pDataFile->Read(&m_Rating, sizeof(m_Rating));
    _pDataFile->Read(&m_RatingPositiveMovement, sizeof(m_RatingPositiveMovement));
    _pDataFile->Read(&m_RatingNegativeMovement, sizeof(m_RatingNegativeMovement));
}


/*------------------------------------------------------------------------------
	Method:   	CRating::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRating::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
    _pDataFile->Write(&m_Rating, sizeof(m_Rating));
    _pDataFile->Write(&m_RatingPositiveMovement, sizeof(m_RatingPositiveMovement));
    _pDataFile->Write(&m_RatingNegativeMovement, sizeof(m_RatingNegativeMovement));
}
