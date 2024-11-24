// FansReaction.cpp: implementation of the CFansReaction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "FansReaction.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



/*------------------------------------------------------------------------------
	Method:   	CFansReaction::CFansReaction
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFansReaction::CFansReaction()
{

}


/*------------------------------------------------------------------------------
	Method:   	CFansReaction::~CFansReaction
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFansReaction::~CFansReaction()
{

}


 /*------------------------------------------------------------------------------
	Method:   	CFansReaction::DoFansReactionToSale
	Access:    	public 
	Parameter: 	CPlayer& _Player
	Parameter: 	const uint _Cost
	Parameter: 	int _RatingPositionInSquad
	Parameter: 	int _RatingPositionInPosition
	Returns:   	void
	Qualifier: 	
	Purpose:	Check if just sold a top player(within club) then send message 
				from fans expressing their feelings
------------------------------------------------------------------------------*/
void CFansReaction::DoFansReactionToSale(CPlayer& _Player, const uint _Cost, int _RatingPositionInSquad, int _RatingPositionInPosition)
{
	if (_RatingPositionInSquad < 4 || _RatingPositionInPosition < 2)
	{
		// One of the best in the squad
		CExtString str;
		if (_Cost > 0)
		{
			_Player.GetClub().DoLowerMorale(20);
			if (RandomNumber.IntLessThan(10) < 5)
			{
				str.Format(IDS_STARSALEMSG1, _Player.GetClub().GetManagerName(), _Player.GetName());
			}
			else
			{
				str.Format(IDS_STARSALEMSG2, _Player.GetClub().GetManagerName(), _Player.GetName());
			}
			_Player.GetClub().AddToClubNews(str);
		}
		else
		{
			_Player.GetClub().DoLowerMorale(30);
			if (RandomNumber.IntLessThan(10) < 5)
			{
				str.Format(IDS_STARLEAVEMSG1, _Player.GetClub().GetManagerName(), _Player.GetName());
			}
			else
			{
				str.Format(IDS_STARLEAVEMSG2, _Player.GetClub().GetManagerName(), _Player.GetName());
			}
		}
		_Player.GetClub().AddToClubNews(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CFansReaction::DoFansReactionToSigning
	Access:    	public 
	Parameter: 	CPlayer& _Player
	Parameter: 	int _RatingPositionInSquad
	Parameter: 	int _RatingPositionInPosition
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFansReaction::DoFansReactionToSigning(CPlayer& _Player, int _RatingPositionInSquad, int _RatingPositionInPosition)
{
	CString str;
	if (_RatingPositionInSquad < 4 || _RatingPositionInPosition < 2)
	{
		// One of the best in the squad
		_Player.GetClub().DoRaiseMorale(20);
		CExtString str;
		if (RandomNumber.IntLessThan(10) < 5)
		{
			str.Format(IDS_STARSIGNMSG1, _Player.GetClub().GetManagerName(), _Player.GetName());
		}
		else
		{
			str.Format(IDS_STARSIGNMSG2, _Player.GetClub().GetManagerName(), _Player.GetName());
		}
		_Player.GetClub().AddToClubNews(str);
	}
	return str;
}
