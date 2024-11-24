
/*

	Class:- Current Form

	Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------
 */


 #ifndef __CURRENTFORM_H__
 #define __CURRENTFORM_H__


class CMatchResult
{
private:
	byte					m_Scored;
	byte					m_Conceded;

public:
    // Methods
	byte					GetScored()	const;
	byte					GetConceded	() const;
	bool					HasBeenPlayed() const;
	bool					HasDrawnMatch();
	bool					HasWonMatch();
	void					DoResetVariables();
	void					SetResult(byte _Scored, byte _Conceded);
	CString					WinDrawLoseStr();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CCurrentForm
{
public:
	static const uint		NUMFORMMATCHES = 6;					// How many matches to include in current form calculations for a club

	// Variables
private:
    byte					m_CurrentFormPts;
    byte					m_CurrentFormPosn;
    byte					m_CurrentFormTotalConceded;
    byte					m_CurrentFormScored;
    CMatchResult			m_CurrentForm[NUMFORMMATCHES];

public:
    // Constructor
    CCurrentForm();

    // Methods
    void					DoResetVariables();
	void					DoUpdateCurrentForm(byte _Scored, const byte _Conceded, const byte _PointsForWin, const byte _PointsForDraw);
    byte					GetCurrentFormPlayed() const;
	byte					GetCurrentFormPts() const;
    byte					GetCurrentFormPosn() const;
    byte					GetCurrentFormConceded() const;
    byte					GetCurrentFormScored() const;
    byte					GetCurrentFormDrawn();
    byte					GetCurrentFormWon();
	byte					GetCurrentFormLost();
	void					SetCurrentFormPts(const byte x);
	void					SetCurrentFormPosn(const byte _Value);
    void					SetCurrentFormConceded(const byte x);
    void					SetCurrentFormScored(const byte x);
    CString		            WinDrawLoseStr();
};

#endif
