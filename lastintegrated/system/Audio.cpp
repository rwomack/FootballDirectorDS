/*
	Program :-	Audio & Video Support
	14/11/96

	Darren Garside
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#include "stdafx.h"
#include "Audio.h"

IMPLEMENT_DYNAMIC (CAudio, CObject);

 //----------------------------------------------------------------------------
CAudio::CAudio()
{
}


DWORD CAudio::InitCD ()
{
	cdOpenParms.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_CD_AUDIO;

	DWORD dwResult = mciSendCommand(NULL,MCI_OPEN,
		MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_SHAREABLE | MCI_WAIT,
		(DWORD)(LPVOID)&cdOpenParms);

	if (!dwResult)
	{
		cdDeviceID = cdOpenParms.wDeviceID;

		MCI_SET_PARMS mciSetInfo;
		mciSetInfo.dwTimeFormat = MCI_FORMAT_TMSF;

		dwResult = mciSendCommand(cdDeviceID, MCI_SET,
			MCI_SET_TIME_FORMAT | MCI_WAIT,
			(DWORD)(LPVOID)&mciSetInfo);
	}

	return(dwResult);
}


void CAudio::InitMIDI ()
{
}


void CAudio::InitWAVE ()
{
}


void CAudio::InitAVI (CString FName, HWND hWnd)
{
	HWND h = MCIWndCreate(hWnd,AfxGetInstanceHandle(),
						  MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR,FName);

	MCIWndPlay(h);
	return;
}


DWORD CAudio::PlayCD (int TrackNo, BOOL Loop)
{
	cdPlayParms.dwFrom = MCI_MAKE_TMSF(TrackNo,0,0,0);

	DWORD dwResult = mciSendCommand(cdDeviceID, MCI_PLAY,
		MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&cdPlayParms);

	// Check error number and return message if appropriate

	return(dwResult);
}


void CAudio::PlayMIDI (CString FName, BOOL Loop)
{
	midiOpenParms.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_SEQUENCER;
	midiOpenParms.lpstrElementName = (LPCSTR)FName;

	int NumMidi = midiOutGetNumDevs();

	if(NumMidi > 0)
	{
		DWORD RetCode = mciSendCommand(NULL,MCI_OPEN,
			MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
			(DWORD)(LPVOID)&midiOpenParms);

		if(RetCode == 0)
		{
			midiDeviceID = midiOpenParms.wDeviceID;
			midiPlayParms.dwCallback = NULL;
			RetCode = mciSendCommand(midiDeviceID,MCI_PLAY, MCI_NOTIFY,
				(DWORD)(LPVOID)&midiPlayParms);
		}
		else
		{
			char tmpBuffer[128];
			mciGetErrorString(RetCode,tmpBuffer,sizeof(tmpBuffer));
			AfxMessageBox(tmpBuffer, MB_OK + MB_ICONEXCLAMATION);
		}
	}
	else
	{
		AfxMessageBox("No Midi Devices Found",MB_OK | MB_ICONEXCLAMATION);
	}
	return;
}


void CAudio::PlayWAVE (CString FName, BOOL Loop, BOOL Wait)
{
	PlayWAV (FName, Loop, Wait, FALSE);
	return;
}


void CAudio::PlayScreenWAV (CString FName)
{
	PlayWAV (FName, FALSE, TRUE, TRUE);
	return;
}


void CAudio::PlayWAV (CString FName, BOOL Loop, BOOL Wait, BOOL When)
{
	if (Wait == TRUE) {
		while(!sndPlaySound(NULL,SND_NOSTOP));		// Wait for previous sound to stop
	}

	if (Loop == TRUE) {								// Loop continuously if neccesary
		if (When == TRUE) {
			sndPlaySound(FName, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		}
		else {
			sndPlaySound(FName, SND_SYNC | SND_NODEFAULT | SND_LOOP);
		}
	}
	else {
		if (When == TRUE) {
			sndPlaySound(FName, SND_ASYNC | SND_NODEFAULT);
		}
		else {
			sndPlaySound(FName, SND_SYNC | SND_NODEFAULT);
		}
	}
	return;
}



void CAudio::PlayAVI(CString FName, HWND hWnd)
{
	aviHWnd = MCIWndCreate(hWnd,AfxGetInstanceHandle(),
						  MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR,FName);

	MCIWndPlay(aviHWnd);
	return;
}



DWORD CAudio::CloseCD (MCI_GENERIC_PARMS lpStop)
{
	DWORD dwResult = mciSendCommand(cdDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD)(LPVOID)&lpStop);
	return(dwResult);
}


DWORD CAudio::CloseMIDI (MCI_GENERIC_PARMS lpStop)
{
	DWORD dwResult = mciSendCommand(midiDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD)(LPVOID)&lpStop);
	return(dwResult);
}


DWORD CAudio::CloseWAVE ()
{
	DWORD dwResult = sndPlaySound(NULL,SND_NOSTOP);
	if (dwResult == FALSE) {
		dwResult = sndPlaySound(NULL,NULL);
	}
	return(dwResult);
}


DWORD CAudio::CloseAVI ()
{
	return MCIWndClose(aviHWnd);
}


DWORD CAudio::CheckAVIPlaying()
{
	CString lp;

	if (MCIWndGetMode(aviHWnd,(LPCTSTR)lp,sizeof(lp)) == MCI_MODE_PLAY) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


DWORD CAudio::CheckWAVPlaying()
{
	return sndPlaySound(NULL,SND_NOSTOP);
}
