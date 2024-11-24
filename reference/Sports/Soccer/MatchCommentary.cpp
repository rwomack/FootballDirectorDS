
 /*

		Program:- MatchCommentary.cpp

		The match commentary class routines

		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::GetText
	Access:    	public 
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchCommentary::GetText()
{
	return m_strText;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoFullTime
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoFullTime()
{
	m_strText.LoadString(IDS_FULLTIME);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoHalfTime
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoHalfTime()
{
	m_strText.LoadString(IDS_HALFTIME);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoTakeInjuredPlayerOff
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoTakeInjuredPlayerOff(CString& _PlayerName, CString& _ClubName)
{
	m_strText.Format(IDS_INJUREDINMATCHOFF, _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoTakeInjuredPlayerOff ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoBringSubOnAfterInjury
	Access:    	public 
	Parameter: 	CString & _PlayerOnName
	Parameter: 	CString & _ClubName
	Parameter: 	CString & _PlayerOffName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoBringSubOnAfterInjury(CString& _PlayerOnName, CString& _ClubName, CString& _PlayerOffName)
{
	m_strText.Format(IDS_COMMENTARY47, _PlayerOffName, _ClubName, _PlayerOnName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoBringSubOnAfterInjury ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoNoSubOnAfterInjury
	Access:    	public 
	Parameter: 	CString & _PlayerOnName
	Parameter: 	CString & _ClubName
	Parameter: 	const ushort _PlayersInTeamOnPitch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoNoSubOnAfterInjury(CString& _PlayerOnName, CString& _ClubName, const ushort _PlayersInTeamOnPitch)
{
	m_strText.Format(IDS_COMMENTARY48, _PlayerOnName, _ClubName, _PlayersInTeamOnPitch);
#ifdef DUMPCOMMENTARY
	afxDump << "DoNoSubOnAfterInjury ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoKickOff
	Access:    	public 
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoKickOff(CString& _ClubName)
{
    m_strText.Format(IDS_COMMENTARYKICKOFF, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoKickOff ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoBookPlayer
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoBookPlayer(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY4, IDS_COMMENTARY5,
		IDS_COMMENTARY6, IDS_COMMENTARY7, 
		IDS_COMMENTARY68, IDS_COMMENTARY69
	};
    m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(6)], _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoBookPlayer ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoSendPlayerOff
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Parameter: 	const bool _TwoYellows
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoSendPlayerOff(CString& _PlayerName, CString& _ClubName, const bool _TwoYellows)
{
	if (_TwoYellows == false)
	{
		static const UINT nCommentaryIDs[] = 
		{
			IDS_COMMENTARY10, IDS_COMMENTARY11,
			IDS_COMMENTARY12, IDS_COMMENTARY13,
			IDS_COMMENTARY14, IDS_COMMENTARY15,
		};
		m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(5)], _PlayerName, _ClubName);
	}
	else
	{
		static const UINT nCommentaryIDs[] = 
		{
			IDS_COMMENTARY70, IDS_COMMENTARY71,
		};
		m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(1)], _PlayerName, _ClubName);
	}
#ifdef DUMPCOMMENTARY
	afxDump << "DoSendPlayerOff ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoSubstituteUserInjuredPlayer
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoSubstituteUserInjuredPlayer(CString& _PlayerName, CString& _ClubName)
{
	// Player is coming off, User controlled team
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY49, IDS_COMMENTARY50, IDS_COMMENTARY51,
		IDS_COMMENTARY52, IDS_COMMENTARY53, IDS_COMMENTARY54
	};
	m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(5)], _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoSubstituteUserInjuredPlayer ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoInjuredPlayerOK
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoInjuredPlayerOK(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY55, IDS_COMMENTARY56, IDS_COMMENTARY57,
		IDS_COMMENTARY58, IDS_COMMENTARY59, IDS_COMMENTARY60
	};
	m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(5)], _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoInjuredPlayerOK ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoNormalGoalScored
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoNormalGoalScored(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY22, IDS_COMMENTARY23, IDS_COMMENTARY24,
		IDS_COMMENTARY25, IDS_COMMENTARY26, IDS_COMMENTARY27,
		IDS_COMMENTARY73, IDS_COMMENTARY74, IDS_COMMENTARY75,
		IDS_COMMENTARY76, IDS_COMMENTARY77, IDS_COMMENTARY78,
		IDS_COMMENTARY79, IDS_COMMENTARY80, IDS_COMMENTARY81,
		IDS_COMMENTARY82, IDS_COMMENTARY83, IDS_COMMENTARY84,
	};
	m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(18)], _ClubName, _PlayerName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoNormalGoalScored ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoFreekickGoal
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoFreekickGoal(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY29, IDS_COMMENTARY30, IDS_COMMENTARY31,
		IDS_COMMENTARY32, IDS_COMMENTARY33, IDS_COMMENTARY34,
		IDS_COMMENTARY85
	};
	CString strText;
	strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(7)], _ClubName, _PlayerName);
	DoUpdateText(strText, true);
#ifdef DUMPCOMMENTARY
	afxDump << "DoFreekickGoal ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoSubstitutePlayer
	Access:    	public 
	Parameter: 	CString & _PlayerOffName
	Parameter: 	CString & _PlayerOnName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoSubstitutePlayer(CString& _PlayerOffName, CString& _PlayerOnName, CString& _ClubName)
{
	static const UINT nCommentaryIDs[] = 
	{
		IDS_COMMENTARY61, IDS_COMMENTARY62,  IDS_COMMENTARY63, 
		IDS_COMMENTARY64, IDS_COMMENTARY65,  IDS_COMMENTARY66, 
		IDS_COMMENTARY67
	};
	m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(7)], _ClubName, _PlayerOnName, _PlayerOffName);
#ifdef DUMPCOMMENTARY
	m_strText.OutputDebug();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoOwnGoal
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoOwnGoal(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nGoalTextID[] = 
	{
		IDS_COMMENTARY16, IDS_COMMENTARY17, IDS_COMMENTARY18, IDS_COMMENTARY19,
		IDS_COMMENTARY20, IDS_COMMENTARY21, IDS_COMMENTARY72,
	};
	m_strText.Format(nGoalTextID[RandomNumber.IntLessThan(7)], _ClubName, _PlayerName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoOwnGoal ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoOffside
	Access:    	public 
	Parameter: 	CString _PlayerAndClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoOffside(CString _PlayerAndClubName)
{
	static const UINT strID[] = 
	{
		IDS_PLAYEROFFSIDE, IDS_PLAYEROFFSIDE2,
		IDS_PLAYEROFFSIDE3, IDS_PLAYEROFFSIDE4,
		IDS_PLAYEROFFSIDE5, IDS_PLAYEROFFSIDE6
	};
	m_strText.Format(strID[RandomNumber.IntLessThan(9)], _PlayerAndClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoOffside ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoCorner
	Access:    	public 
	Parameter: 	CString _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoCorner(CString _ClubName)
{
	static const UINT strID[] = 
	{
		IDS_CORNERTO, IDS_CORNERTO2,
		IDS_CORNERTO3, IDS_CORNERTO4,
		IDS_CORNERTO5, IDS_CORNERTO6
	};
	m_strText.Format(strID[RandomNumber.IntLessThan(6)], _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoCorner ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoShotSaved
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoShotSaved(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nTextID[] = 
	{
		IDS_COMMENTARY41, IDS_COMMENTARY42, IDS_COMMENTARY43, 
		IDS_COMMENTARY44, IDS_COMMENTARY45, IDS_COMMENTARY46, 
		IDS_COMMENTARY88, IDS_COMMENTARY89, IDS_COMMENTARY90, 
		IDS_COMMENTARY91 
	};
	CString strText;
	strText.Format(nTextID[RandomNumber.IntLessThan(10)], _PlayerName, _ClubName);
	DoUpdateText(strText, true);
#ifdef DUMPCOMMENTARY
	afxDump << "DoShotSaved ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoScoredGoalFromCorner
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoScoredGoalFromCorner(CString& _PlayerName, CString& _ClubName)
{
	m_strText.Format(IDS_COMMENTARY28, _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoScoredGoalFromCorner ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoShotOffTarget
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoShotOffTarget(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nTextID[] =
	{
		IDS_COMMENTARYOFFTARGET1, IDS_COMMENTARYOFFTARGET2, IDS_COMMENTARYOFFTARGET3,
		IDS_COMMENTARYOFFTARGET4, IDS_COMMENTARYOFFTARGET5, IDS_COMMENTARYOFFTARGET6,
		IDS_COMMENTARYOFFTARGET7, IDS_COMMENTARYOFFTARGET8, IDS_COMMENTARYOFFTARGET9,
		IDS_COMMENTARYOFFTARGET10, IDS_COMMENTARYOFFTARGET11
	};
	CString strText;
	strText.Format(nTextID[RandomNumber.IntLessThan(11)], _PlayerName, _ClubName);
	DoUpdateText(strText, true);
#ifdef DUMPCOMMENTARY
	afxDump << "DoShotOffTarget ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoHeaderOffTarget
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoHeaderOffTarget(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nTextID[] =
	{
		// These dont mention shot or header!
		IDS_COMMENTARYOFFTARGET1, IDS_COMMENTARYOFFTARGET2, IDS_COMMENTARYOFFTARGET3,
		IDS_COMMENTARYOFFTARGET4, IDS_COMMENTARYOFFTARGET5, IDS_COMMENTARYOFFTARGET6
	};
	CString strText;
	strText.Format(nTextID[RandomNumber.IntLessThan(6)], _PlayerName, _ClubName);
	DoUpdateText(strText, true);
#ifdef DUMPCOMMENTARY
	afxDump << "DoHeaderOffTarget ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoDisAllowedShot
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoDisAllowedShot(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nTextID[] =
	{
		IDS_COMMENTARY35, IDS_COMMENTARY36, IDS_COMMENTARY37,
		IDS_COMMENTARY38, IDS_COMMENTARY39, IDS_COMMENTARY40,
		IDS_COMMENTARY86, IDS_COMMENTARY87
	};
	m_strText.Format(nTextID[RandomNumber.IntLessThan(8)], _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoDisAllowedShot ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoDisAllowedHeader
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoDisAllowedHeader(CString& _PlayerName, CString& _ClubName)
{
	static const UINT nTextID[] =
	{
		IDS_COMMENTARY35, IDS_COMMENTARY36, IDS_COMMENTARY37,
		IDS_COMMENTARY38, IDS_COMMENTARY39, IDS_COMMENTARY40
	};
	m_strText.Format(nTextID[RandomNumber.IntLessThan(6)], _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoDisAllowedHeader ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoScoredPenaltyGoal
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoScoredPenaltyGoal(CString& _PlayerName, CString& _ClubName)
{
	m_strText.Format(IDS_COMMENTARY1, _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoScoredPenaltyGoal ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoPenaltySaved
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoPenaltySaved(CString& _PlayerName, CString& _ClubName)
{
	m_strText.Format(IDS_COMMENTARY2, _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoPenaltySaved ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoPenaltyMissed
	Access:    	public 
	Parameter: 	CString & _PlayerName
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoPenaltyMissed(CString& _PlayerName, CString& _ClubName)
{
	m_strText.Format(IDS_COMMENTARY3, _PlayerName, _ClubName);
#ifdef DUMPCOMMENTARY
	afxDump << "DoPenaltyMissed ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoFreeKick
	Access:    	public 
	Parameter: 	CString & _ClubName
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoFreeKick(CString& _ClubName)
{
	CString strText;
	strText.Format(IDS_FREEKICKTO, _ClubName);
	DoUpdateText(strText);
#ifdef DUMPCOMMENTARY
	afxDump << "DoFreeKick ** " << m_strText << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoUpdateText
	Access:    	public 
	Parameter: 	CString & _strText
	Parameter: 	const bool _NewLine
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoUpdateText(CString& _strText, const bool _NewLine /*= false*/)
{
	if (m_strText.IsEmpty() == TRUE)
	{
		m_strText = _strText;
	}
	else
	{
		// Start new line for additional text?
		if (_NewLine)
		{
			m_strText += TEXT("\n") + _strText;
		}
		else
		{
			if (m_strText.Right(1) == TEXT("."))
			{
				m_strText = m_strText.Left(m_strText.GetLength() - 1);
			}
			m_strText += TEXT(", ") + _strText;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::Empty
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::Empty()
{
	m_strText.Empty();
}
