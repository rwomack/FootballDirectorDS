#if !defined(AFX_PLAYERFORM_H__451609C2_D9E7_11D1_A86A_00001C30771E__INCLUDED_)
#define AFX_PLAYERFORM_H__451609C2_D9E7_11D1_A86A_00001C30771E__INCLUDED_

/*

    PlayerForm Class

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


	static const uint		NUMPLAYERFORMMATCHES = 4;			// How many matches to include in current form calculations for a player

	
class CPlayerRating
{
    // Variables
private:
    byte					m_Rating[NUMPLAYERFORMMATCHES];

public:
    CPlayerRating();
    CPlayerRating(const byte x);
    ~CPlayerRating();
    CPlayerRating&  operator = (const CPlayerRating &p);

    // Other methods
	void DoAddPerformance(const byte _Value);
    byte                   CurrentRating();
	byte					GetLastPerformance();
	sbyte					GetPerformance();
	void					OnInitialise(const byte _InitialRating);
    void                    DoResetVariables();
	void					DoMoveDown();
};

#endif // !defined(AFX_PLAYERFORM_H__451609C2_D9E7_11D1_A86A_00001C30771E__INCLUDED_)
