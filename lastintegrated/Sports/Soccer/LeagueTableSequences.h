
/*

	File:- LeagueTableSequences.h


	Football Director

	(c) RW Software 1994 - 2004

 ---------------------------------------------------------------------------------------
*/


#ifndef __LEAGUETABLESEQUENCES_H__
#define __LEAGUETABLESEQUENCES_H__

class CLeagueTableSequences
{
	// Variables
private:
	uint8					m_NumberSuccessiveWins;
    uint8					m_NumberSuccessiveDefeats;
    uint8					m_NumberGamesSinceAWin;
    uint8					m_NumberGamesSinceADefeat;

public:
	// Constructors
	CLeagueTableSequences(void);

	// IO Methods
	void					Write(FILE* File);
	void					Read(FILE* File);
	
	const uint8		GetNumberSuccessiveWins(void)						
	{ 
		return m_NumberSuccessiveWins; 
	}
    const uint8		GetNumberSuccessiveDefeats(void)					
	{ 
		return m_NumberSuccessiveDefeats; 
	}
    const uint8		GetNumberGamesSinceAWin(void)						
	{ 
		return m_NumberGamesSinceAWin; 
	}
    const uint8		GetNumberGamesSinceADefeat(void)					
	{ 
		return m_NumberGamesSinceADefeat; 
	}
	void				SetNumberHomeSuccessiveWins(const uint8 c_Value)	
	{ 
		m_NumberSuccessiveWins = c_Value; 
	}
    void				SetNumberHomeSuccessiveDefeats(const uint8 c_Value)	
	{ 
		m_NumberSuccessiveDefeats = c_Value; 
	}
    void				SetNumberHomeGamesSinceAWin(const uint8 c_Value)	
	{ 
		m_NumberGamesSinceAWin = c_Value; 
	}
    void				SetNumberHomeGamesSinceADefeat(const uint8 c_Value)	
	{ 
		m_NumberGamesSinceADefeat = c_Value; 
	}

	void					DoResetVariables(void);
	void					DrawnGame(void);
	void					WonGame(void);
	void					LostGame(void);
};
#endif