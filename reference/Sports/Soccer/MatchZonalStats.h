
/*

	MatchVars.h: interface for the CMatchVars class.


	Football Director

	(c) Rw Software 1994 - 2008


  ---------------------------------------------------------------
*/


#if !defined MATCHZONALSTATS
#define MATCHZONALSTATS


class CMatchZonalStats
{
public:
	void					DoInitialise()
	{	
         m_Pace = 0;
         m_Energy = 0;
         m_Heading = 0;
         m_Passing = 0;
         m_Strength = 0;
	}
	void AddVars(CPlayer* pPlayer)
	{
		AddPace(pPlayer->GetSkills().GetPace());
		AddEnergy(pPlayer->GetSkills().GetEnergy());
		AddHeading(pPlayer->DoCalculateEffectiveSkill(SKILL_HEADING));
		AddStrength(pPlayer->GetSkills().GetStrength());
		AddPassing(pPlayer->DoCalculateEffectiveSkill(SKILL_PASSING));
	}
	int				GetPace()						
	{ 
		return m_Pace; 
	}
    int				GetEnergy()							
	{ 
		return m_Energy; 
	}
	void				SetPace(const int value)			
	{ 
		m_Pace = value; 
	}
    void				SetEnergy(const int value)					
	{ 
		m_Energy = value; 
	}
	void AddPace(const int _Value)					
	{ 
		m_Pace += _Value; 
	}
	void AddEnergy(const int _Value)					
	{ 
		m_Energy += _Value; 
	}
	void AddHeading(const int _Value)				
	{ 
		m_Heading += _Value;  
	}
	void AddPassing(const int _Value)				
	{ 
		m_Passing += _Value;  
	}
	void AddStrength(const int _Value)				
	{ 
		m_Strength += _Value;  
	}
    int				GetHeading() const
	{ 
		return m_Heading;  
	}
    int				GetPassing() const					
	{ 
		return m_Passing;  
	}
    int				GetStrength() const					
	{ 
		return  m_Strength;  
	}

protected:
    uint                    m_Pace;
    uint                    m_Energy;
    uint                    m_Heading;
    uint                    m_Passing;
    uint                    m_Strength;
};

#endif