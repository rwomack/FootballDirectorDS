
 /*

		Program:- MatchCommentary.cpp

		The match commentary class routines

		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


// macro for getting the number of elements in an array
#define countof( array ) ( sizeof( array )/sizeof( array[0] ) )

/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::GetText
	Access:    	public
  Returns:    CString
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
void CMatchCommentary::DoFullTime(bool bExtraTime)
{
  if( bExtraTime )
  {
    m_strText.LoadString(IDS_FULLTIME_EXTRA);
  } else
  {
    m_strText.LoadString(IDS_FULLTIME);
  }
}


/*------------------------------------------------------------------------------
	Method:   	CMatchCommentary::DoHalfTime
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchCommentary::DoHalfTime(bool bExtraTime)
{
  if( bExtraTime )
  {
    m_strText.LoadString(IDS_HALFTIME_EXTRA);
  } else
  {
    m_strText.LoadString(IDS_HALFTIME);
  }
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
void CMatchCommentary::DoTakeInjuredPlayerOff(CString _PlayerName, CString _ClubName)
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
void CMatchCommentary::DoBringSubOnAfterInjury(CString _PlayerOnName, CString _ClubName, CString _PlayerOffName)
{
  if( RandomNumber.IntLessThan(10) > 5 )
    m_strText.Format(IDS_COMMENTARY47, _PlayerOffName, _ClubName, _PlayerOnName);
  else
    m_strText.Format(IDS_COMMENTARY47_VARIANT1, _PlayerOffName, _ClubName, _PlayerOnName);
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
void CMatchCommentary::DoNoSubOnAfterInjury(CString _PlayerOnName, CString _ClubName, const ushort _PlayersInTeamOnPitch)
{
  if( RandomNumber.IntLessThan(10) > 5 )
    m_strText.Format(IDS_COMMENTARY48, _PlayerOnName, _ClubName, _PlayersInTeamOnPitch);
  else
    m_strText.Format(IDS_COMMENTARY48_VARIANT1, _PlayerOnName, _ClubName, _PlayersInTeamOnPitch);
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
void CMatchCommentary::DoKickOff(CString _ClubName)
{
  if( RandomNumber.IntLessThan(10) > 5 )
    m_strText.Format(IDS_COMMENTARYKICKOFF, _ClubName);
  else
    m_strText.Format(IDS_COMMENTARYKICKOFF_VARIANT1, _ClubName);
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
void CMatchCommentary::DoBookPlayer(CString _PlayerName, CString _ClubName)
{
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY4, IDS_COMMENTARY5,
    IDS_COMMENTARY6, IDS_COMMENTARY7,
    IDS_COMMENTARY68, IDS_COMMENTARY69,

    // variants
    IDS_COMMENTARY4_VARIANT1, IDS_COMMENTARY5_VARIANT1,
    IDS_COMMENTARY6_VARIANT1, IDS_COMMENTARY7_VARIANT1,
    IDS_COMMENTARY68_VARIANT1, IDS_COMMENTARY69_VARIANT1
	};
    m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoSendPlayerOff(CString _PlayerName, CString _ClubName, const bool _TwoYellows)
{
	if (_TwoYellows == false)
	{
		static const UINT nCommentaryIDs[] =
		{
      IDS_COMMENTARY10, IDS_COMMENTARY11,
      IDS_COMMENTARY12, IDS_COMMENTARY13,
      IDS_COMMENTARY14, IDS_COMMENTARY15,

      // variants
      IDS_COMMENTARY10_VARIANT1, IDS_COMMENTARY11_VARIANT1,
      IDS_COMMENTARY12_VARIANT1, IDS_COMMENTARY13_VARIANT1,
      IDS_COMMENTARY14_VARIANT1, IDS_COMMENTARY15_VARIANT1,
		};
    m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _PlayerName, _ClubName);
	}
	else
	{
		static const UINT nCommentaryIDs[] =
		{
      IDS_COMMENTARY70, IDS_COMMENTARY71,
      //variants
      IDS_COMMENTARY70_VARIANT1, IDS_COMMENTARY71_VARIANT1,
		};
    m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoSubstituteUserInjuredPlayer(CString _PlayerName, CString _ClubName)
{
	// Player is coming off, User controlled team
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY49, IDS_COMMENTARY50, IDS_COMMENTARY51,
    IDS_COMMENTARY52, IDS_COMMENTARY53, IDS_COMMENTARY54,
    // variant
    IDS_COMMENTARY49_VARIANT1, IDS_COMMENTARY50_VARIANT1, IDS_COMMENTARY51_VARIANT1,
    IDS_COMMENTARY52_VARIANT1, IDS_COMMENTARY53_VARIANT1, IDS_COMMENTARY54_VARIANT1
	};
  m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoInjuredPlayerOK(CString _PlayerName, CString _ClubName)
{
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY55, IDS_COMMENTARY56, IDS_COMMENTARY57,
    IDS_COMMENTARY58, IDS_COMMENTARY59, IDS_COMMENTARY60,
    //variants
    IDS_COMMENTARY55_VARIANT1, IDS_COMMENTARY56_VARIANT1, IDS_COMMENTARY57_VARIANT1,
    IDS_COMMENTARY58_VARIANT1, IDS_COMMENTARY59_VARIANT1, IDS_COMMENTARY60_VARIANT1,
	};
  m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoNormalGoalScored(CString _PlayerName, CString _ClubName)
{
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY22, IDS_COMMENTARY23, IDS_COMMENTARY24,
    IDS_COMMENTARY25, IDS_COMMENTARY26, IDS_COMMENTARY27,
    IDS_COMMENTARY73, IDS_COMMENTARY74, IDS_COMMENTARY75,
    IDS_COMMENTARY76, IDS_COMMENTARY77, IDS_COMMENTARY78,
    IDS_COMMENTARY79, IDS_COMMENTARY80, IDS_COMMENTARY81,
    IDS_COMMENTARY82, IDS_COMMENTARY83, IDS_COMMENTARY84,

    IDS_COMMENTARY22_VARIANT1, IDS_COMMENTARY23_VARIANT1, IDS_COMMENTARY24_VARIANT1,
    IDS_COMMENTARY25_VARIANT1, IDS_COMMENTARY26_VARIANT1, IDS_COMMENTARY27_VARIANT1,
    IDS_COMMENTARY73_VARIANT1, IDS_COMMENTARY74_VARIANT1, IDS_COMMENTARY75_VARIANT1,
    IDS_COMMENTARY76_VARIANT1, IDS_COMMENTARY77_VARIANT1, IDS_COMMENTARY78_VARIANT1,
    IDS_COMMENTARY79_VARIANT1, IDS_COMMENTARY80_VARIANT1, IDS_COMMENTARY81_VARIANT1,
    IDS_COMMENTARY82_VARIANT1, IDS_COMMENTARY83_VARIANT1, IDS_COMMENTARY84_VARIANT1,
	};
  m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _ClubName, _PlayerName);
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
void CMatchCommentary::DoFreekickGoal(CString _PlayerName, CString _ClubName)
{
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY29, IDS_COMMENTARY30, IDS_COMMENTARY31,
    IDS_COMMENTARY32, IDS_COMMENTARY33, IDS_COMMENTARY34,
    IDS_COMMENTARY85,

    IDS_COMMENTARY29_VARIANT1, IDS_COMMENTARY30_VARIANT1, IDS_COMMENTARY31_VARIANT1,
    IDS_COMMENTARY32_VARIANT1, IDS_COMMENTARY33_VARIANT1, IDS_COMMENTARY34_VARIANT1,
    IDS_COMMENTARY85_VARIANT1
	};
	CString strText;
  strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _ClubName, _PlayerName);
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
void CMatchCommentary::DoSubstitutePlayer(CString _PlayerOffName, CString _PlayerOnName, CString _ClubName)
{
	static const UINT nCommentaryIDs[] =
	{
    IDS_COMMENTARY61, IDS_COMMENTARY62,  IDS_COMMENTARY63,
    IDS_COMMENTARY64, IDS_COMMENTARY65,  IDS_COMMENTARY66,
    IDS_COMMENTARY67,

    IDS_COMMENTARY61_VARIANT1, IDS_COMMENTARY62_VARIANT1,  IDS_COMMENTARY63_VARIANT1,
    IDS_COMMENTARY64_VARIANT1, IDS_COMMENTARY65_VARIANT1,  IDS_COMMENTARY66_VARIANT1,
    IDS_COMMENTARY67_VARIANT1
	};
  m_strText.Format(nCommentaryIDs[RandomNumber.IntLessThan(countof(nCommentaryIDs))], _ClubName, _PlayerOnName, _PlayerOffName);
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
void CMatchCommentary::DoOwnGoal(CString _PlayerName, CString _ClubName)
{
	static const UINT nGoalTextID[] =
	{
    IDS_COMMENTARY16, IDS_COMMENTARY17, IDS_COMMENTARY18, IDS_COMMENTARY19,
    IDS_COMMENTARY20, IDS_COMMENTARY21, IDS_COMMENTARY72,

    IDS_COMMENTARY16_VARIANT1, IDS_COMMENTARY17_VARIANT1, IDS_COMMENTARY18_VARIANT1, IDS_COMMENTARY19_VARIANT1,
    IDS_COMMENTARY20_VARIANT1, IDS_COMMENTARY21_VARIANT1, IDS_COMMENTARY72_VARIANT1,
	};
  m_strText.Format(nGoalTextID[RandomNumber.IntLessThan(countof(nGoalTextID))], _ClubName, _PlayerName);
	lLogFmt("***** OWN GOAL ***** the plonka\n%s - %s\n",_ClubName, _PlayerName);

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
void CMatchCommentary::DoOffside(const CString &_PlayerAndClubName)
{
	static const UINT strID[] =
	{
    IDS_PLAYEROFFSIDE, IDS_PLAYEROFFSIDE2,
    IDS_PLAYEROFFSIDE3, IDS_PLAYEROFFSIDE4,
    IDS_PLAYEROFFSIDE5, IDS_PLAYEROFFSIDE6,
	};
  m_strText.Format(strID[RandomNumber.IntLessThan(countof(strID))], _PlayerAndClubName);
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
void CMatchCommentary::DoCorner(const CString &_ClubName)
{
	static const UINT strID[] =
	{
    IDS_CORNERTO, IDS_CORNERTO2,
    IDS_CORNERTO3, IDS_CORNERTO4,
    IDS_CORNERTO5, IDS_CORNERTO6,
	};
  m_strText.Format(strID[RandomNumber.IntLessThan(countof(strID))], _ClubName);
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
void CMatchCommentary::DoShotSaved(CString _PlayerName, CString _ClubName)
{
	static const UINT nTextID[] =
	{
    IDS_COMMENTARY41, IDS_COMMENTARY42, IDS_COMMENTARY43,
    IDS_COMMENTARY44, IDS_COMMENTARY45, IDS_COMMENTARY46,
    IDS_COMMENTARY88, IDS_COMMENTARY89, IDS_COMMENTARY90,
    IDS_COMMENTARY91,

    IDS_COMMENTARY41_VARIANT1, IDS_COMMENTARY42_VARIANT1, IDS_COMMENTARY43_VARIANT1,
    IDS_COMMENTARY44_VARIANT1, IDS_COMMENTARY45_VARIANT1, IDS_COMMENTARY46_VARIANT1,
    IDS_COMMENTARY88_VARIANT1, IDS_COMMENTARY89_VARIANT1, IDS_COMMENTARY90_VARIANT1,
    IDS_COMMENTARY91_VARIANT1
	};
	CString strText;
  strText.Format(nTextID[RandomNumber.IntLessThan(countof(nTextID))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoScoredGoalFromCorner(CString _PlayerName, CString _ClubName)
{
  if( RandomNumber.IntLessThan(2) > 0 )
    m_strText.Format(IDS_COMMENTARY28, _PlayerName, _ClubName);
  else
    m_strText.Format(IDS_COMMENTARY28_VARIANT1, _PlayerName, _ClubName);
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
void CMatchCommentary::DoShotOffTarget(CString _PlayerName, CString _ClubName)
{
	static const UINT nTextID[] =
	{
    IDS_COMMENTARYOFFTARGET1, IDS_COMMENTARYOFFTARGET2, IDS_COMMENTARYOFFTARGET3,
    IDS_COMMENTARYOFFTARGET4, IDS_COMMENTARYOFFTARGET5, IDS_COMMENTARYOFFTARGET6,
    IDS_COMMENTARYOFFTARGET7, IDS_COMMENTARYOFFTARGET8, IDS_COMMENTARYOFFTARGET9,
    IDS_COMMENTARYOFFTARGET10, IDS_COMMENTARYOFFTARGET11,

    IDS_COMMENTARYOFFTARGET1_VARIANT1, IDS_COMMENTARYOFFTARGET2_VARIANT1, IDS_COMMENTARYOFFTARGET3_VARIANT1,
    IDS_COMMENTARYOFFTARGET4_VARIANT1, IDS_COMMENTARYOFFTARGET5_VARIANT1, IDS_COMMENTARYOFFTARGET6_VARIANT1,
    IDS_COMMENTARYOFFTARGET7_VARIANT1, IDS_COMMENTARYOFFTARGET8_VARIANT1, IDS_COMMENTARYOFFTARGET9_VARIANT1,
    IDS_COMMENTARYOFFTARGET10_VARIANT1, IDS_COMMENTARYOFFTARGET11_VARIANT1
	};
	CString strText;
  strText.Format(nTextID[RandomNumber.IntLessThan(countof(nTextID))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoHeaderOffTarget(CString _PlayerName, CString _ClubName)
{
	static const UINT nTextID[] =
	{
		// These dont mention shot or header!
    IDS_COMMENTARYOFFTARGET1, IDS_COMMENTARYOFFTARGET2, IDS_COMMENTARYOFFTARGET3,
    IDS_COMMENTARYOFFTARGET4, IDS_COMMENTARYOFFTARGET5, IDS_COMMENTARYOFFTARGET6,

    IDS_COMMENTARYOFFTARGET1_VARIANT1, IDS_COMMENTARYOFFTARGET2_VARIANT1, IDS_COMMENTARYOFFTARGET3_VARIANT1,
    IDS_COMMENTARYOFFTARGET4_VARIANT1, IDS_COMMENTARYOFFTARGET5_VARIANT1, IDS_COMMENTARYOFFTARGET6_VARIANT1
	};
	CString strText;
  strText.Format(nTextID[RandomNumber.IntLessThan(countof(nTextID))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoDisAllowedShot(CString _PlayerName, CString _ClubName)
{
	static const UINT nTextID[] =
	{
    IDS_COMMENTARY35, IDS_COMMENTARY36, IDS_COMMENTARY37,
    IDS_COMMENTARY38, IDS_COMMENTARY39, IDS_COMMENTARY40,
    IDS_COMMENTARY86, IDS_COMMENTARY87,

    IDS_COMMENTARY35_VARIANT1, IDS_COMMENTARY36_VARIANT1, IDS_COMMENTARY37_VARIANT1,
    IDS_COMMENTARY38_VARIANT1, IDS_COMMENTARY39_VARIANT1, IDS_COMMENTARY40_VARIANT1,
    IDS_COMMENTARY86_VARIANT1, IDS_COMMENTARY87_VARIANT1
	};
  m_strText.Format(nTextID[RandomNumber.IntLessThan(countof(nTextID))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoDisAllowedHeader(CString _PlayerName, CString _ClubName)
{
	static const UINT nTextID[] =
	{
    IDS_COMMENTARY35, IDS_COMMENTARY36, IDS_COMMENTARY37,
    IDS_COMMENTARY38, IDS_COMMENTARY39, IDS_COMMENTARY40,

    IDS_COMMENTARY35_VARIANT1, IDS_COMMENTARY36_VARIANT1, IDS_COMMENTARY37_VARIANT1,
    IDS_COMMENTARY38_VARIANT1, IDS_COMMENTARY39_VARIANT1, IDS_COMMENTARY40_VARIANT1
	};
  m_strText.Format(nTextID[RandomNumber.IntLessThan(countof(nTextID))], _PlayerName, _ClubName);
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
void CMatchCommentary::DoScoredPenaltyGoal(CString _PlayerName, CString _ClubName)
{
  if( RandomNumber.IntLessThan(2) > 0 )
    m_strText.Format(IDS_COMMENTARY1, _PlayerName, _ClubName);
  else
    m_strText.Format(IDS_COMMENTARY1_VARIANT1, _PlayerName, _ClubName);
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
void CMatchCommentary::DoPenaltySaved(CString _PlayerName, CString _ClubName)
{
  if( RandomNumber.IntLessThan(2) > 0 )
    m_strText.Format(IDS_COMMENTARY2, _PlayerName, _ClubName);
  else
    m_strText.Format(IDS_COMMENTARY2_VARIANT1, _PlayerName, _ClubName);
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
void CMatchCommentary::DoPenaltyMissed(CString _PlayerName, CString _ClubName)
{
  if( RandomNumber.IntLessThan(2) > 0 )
    m_strText.Format(IDS_COMMENTARY3, _PlayerName, _ClubName);
  else
    m_strText.Format(IDS_COMMENTARY3_VARIANT1, _PlayerName, _ClubName);
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
void CMatchCommentary::DoFreeKick(CString _ClubName)
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
void CMatchCommentary::DoUpdateText(CString _strText, const bool _NewLine /*= false*/)
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
      m_strText += CString("\n") + _strText;
		}
		else
		{
      if (m_strText.Right(1) == CString("."))
			{
				m_strText = m_strText.Left(m_strText.GetLength() - 1);
			}
      m_strText += CString(", ") + _strText;
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
