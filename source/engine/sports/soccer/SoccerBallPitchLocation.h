
/*
  ---------------------------------------------------------------------------------------

    File:- SoccerBallPitchLocation.h: interface for the CSoccerBallPitchLocation class.


    Tracks the location of the ball on the pitch, minute by minute


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#if !defined(AFX_SOCCERBALLPITCHLOCATION_H__3D908329_302F_4FAE_8ADB_85E2A84D69C7__INCLUDED_)
#define AFX_SOCCERBALLPITCHLOCATION_H__3D908329_302F_4FAE_8ADB_85E2A84D69C7__INCLUDED_

    const byte BallXPos[] =
	{
		0,
        MAXPITCHX, 0,               // HOMEGOAL, AWAYGOAL,
        0, 0,                       // HOMEBOOKING, AWAYBOOKING,
        0, 0,                       // HOMESENDINGOFF, AWAYSENDINGOFF,
        MAXPITCHX, 0,               // HOMEOWNGOAL, AWAYOWNGOAL,
        MAXPITCHX, 0,               // HOMEPENALTYGOAL, AWAYPENALTYGOAL,
        0, 0,                       // HOMEOFFSIDEATTEMPT, AWAYOFFSIDEATTEMPT,
        0, 0,                       // HOMEFOULPLAYERATTEMPT, AWAYFOULPLAYERATTEMPT,
        0, 0,                       // HOMEINJUREPLAYERATTEMPT, AWAYINJUREPLAYERATTEMPT,
        MAXPITCHX, 0,               // HOMESHOTATTEMPT, AWAYSHOTATTEMPT,
        MAXPITCHX, 0,               // HOMEMISSATTEMPT, AWAYMISSATTEMPT,
        0, 0,                       // HOMEFREEKICKATTEMPT, AWAYFREEKICKATTEMPT,
        MAXPITCHX, 0,               // HOMECORNERATTEMPT, AWAYCORNERATTEMPT,
	};
    const byte BallYPos[] =
	{
        0,
		PITCHGOALY, PITCHGOALY,		// HOMEGOAL, AWAYGOAL,
		0, 0,						// HOMEBOOKING, AWAYBOOKING,
		0, 0,						// HOMESENDINGOFF, AWAYSENDINGOFF,
        0, 0,                       // HOMEOWNGOAL, AWAYOWNGOAL,
        PITCHGOALY / 2, PITCHGOALY / 2,  // HOMEPENALTYGOAL, AWAYPENALTYGOAL,
        0, 0,                       // HOMEOFFSIDEATTEMPT, AWAYOFFSIDEATTEMPT,
        0, 0,                       // HOMEFOULPLAYERATTEMPT, AWAYFOULPLAYERATTEMPT,
        0, 0,                       // HOMEINJUREPLAYERATTEMPT, AWAYINJUREPLAYERATTEMPT,
        0, 0,                       // HOMESHOTATTEMPT, AWAYSHOTATTEMPT,
        0, 0,                       // HOMEMISSATTEMPT, AWAYMISSATTEMPT,
        0, 0,                       // HOMEFREEKICKATTEMPT, AWAYFREEKICKATTEMPT,
        0, 0,                       // HOMECORNERATTEMPT, AWAYCORNERATTEMPT,
	};

class CSoccerBallPitchLocation  
{
public:
	void					DoInitialise(const ushort nLength);
	void					DoLocateX(const ushort nTimePlayed, const byte xPosition);
	ushort					GetBallXLocation(ushort _TimePlayed);
	ushort					GetBallYLocation(ushort _TimePlayed);
	void					DoCalculateBallLocation(const ushort nTimePlayed, uint nEventID);
	void					SetBallLocation(const ushort _TimePlayed, const byte _XPosition, const byte _YPosition);
	void					SetBallLocationY(const ushort _TimePlayed, const byte _YPosition);

    // Variables
private:
	CNumericArray<byte>		m_MinuteEventBallX;
	CNumericArray<byte>		m_MinuteEventBallY;

};

#endif // !defined(AFX_SOCCERBALLPITCHLOCATION_H__3D908329_302F_4FAE_8ADB_85E2A84D69C7__INCLUDED_)
