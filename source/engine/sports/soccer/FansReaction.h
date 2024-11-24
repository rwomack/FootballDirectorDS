

/*

	Class - C++ Classes

	FansReaction.h: interface for the CFansReaction class.

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */

#if !defined(AFX_FANSREACTION_H__9417279B_A7CB_4373_A1B0_A8810EEDE3D1__INCLUDED_)
#define AFX_FANSREACTION_H__9417279B_A7CB_4373_A1B0_A8810EEDE3D1__INCLUDED_


class CFansReaction  
{
public:
	CFansReaction();
	virtual ~CFansReaction();

	static void					DoFansReactionToSale(CPlayer& _Player, const uint _Cost, int _RatingPositionInSquad, int _RatingPositionInPosition);
	static CString					DoFansReactionToSigning(CPlayer& _Player, int _RatingPositionInSquad, int _RatingPositionInPosition);
};

#endif // !defined(AFX_FANSREACTION_H__9417279B_A7CB_4373_A1B0_A8810EEDE3D1__INCLUDED_)
