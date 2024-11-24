

/*

	C++ Classes

	Pocket Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/

#ifndef __CLUBMATCHSCORE_H__
#define __CLUBMATCHSCORE_H__

class CGameMatchScore
{
	// High 4 bits {scored} - Low 4 bits {conceded}

public:

	/*------------------------------------------------------------------------------
		Method:   	CGameMatchScore::GetScored
		Access:    	public 
		Parameter: 	void
		Returns:   	uint
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	uint					GetScored()
	{
		return m_Scored;
	}


	/*------------------------------------------------------------------------------
		Method:   	CGameMatchScore::GetConceded
		Access:    	public 
		Parameter: 	void
		Returns:   	uint
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	uint					GetConceded()
	{
		return m_Conceded;
	}


	/*------------------------------------------------------------------------------
		Method:   	CGameMatchScore::DoInitialise
		Access:    	public 
		Parameter: 	const uint _Scored
		Parameter: 	const uint _Conceded
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void					DoInitialise(const uint _Scored = 0, const uint _Conceded = 0)
	{
		m_Scored = _Scored;
		m_Conceded = _Conceded;
	}


protected:
	uint				m_Scored:7;
	uint				m_Conceded:7;
};
///////////////////////////////////////////////////////////////////////////////////////////////

template <class Ttype> class CClubMatchScore
{
public:

	//---------------------------------------------------------------------------------------
	void					DoAddToTallies(const Ttype Score, const Ttype Concede = 0)
    {
		m_Scored += Score;
		m_Conceded += Concede;
	}

	//---------------------------------------------------------------------------------------
	void					DoInitialise(const Ttype Score = 0, const Ttype Concede = 0)
    {
		m_Scored = Score;
		m_Conceded = Concede;
	}

	//---------------------------------------------------------------------------------------
	const Ttype				GetScored() const
	{
		return m_Scored;
	}

	//---------------------------------------------------------------------------------------
    const Ttype				GetConceded() const
	{
		return m_Conceded;
	}

	//---------------------------------------------------------------------------------------
	void					SetScored(const Ttype x)
	{
		m_Scored = x;
	}

	//---------------------------------------------------------------------------------------
    void					SetConceded(const Ttype x)
	{
		m_Conceded = x;
	}

	//---------------------------------------------------------------------------------------
	CString					ScoreStr()
	{
		CString str(TEXT("-"));
		if (m_Scored > 0 || m_Conceded > 0)
		{
			str.Format(TEXT("%d - %d"), GetScored(), GetConceded());
		}
		return str;
	}

protected:
	Ttype					m_Scored;
    Ttype					m_Conceded;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif