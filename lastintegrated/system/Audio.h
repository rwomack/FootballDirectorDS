//	C++ Class for Audio Support - Caffeine Software Ltd

//	Darren Garside

//  20/06/96
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Audio Support Class

#ifndef __AUDIO_H__
#define __AUDIO_H__

//#include <digitalv.h>
#include <mmsystem.h>
#include <vfw.h>
#include <afxcmn.h>

class CAudio : public CObject
{
	DECLARE_DYNAMIC (CAudio);
	public:
		// Constructors
			CAudio();

		// Methods
			DWORD	InitCD (void);
			void	InitMIDI (void);
			void	InitWAVE (void);
			void	InitAVI (CString FName, HWND hWnd);
			DWORD	PlayCD (int TrackNo, BOOL Loop);
			void	PlayMIDI (CString FName, BOOL Loop);
			void	PlayWAVE (CString FName, BOOL Loop, BOOL Wait);
			void	PlayScreenWAV (CString FName);
			void    PlayWAV (CString FName, BOOL Loop, BOOL Wait, BOOL When);
			void	PlayAVI(CString FName, HWND hWnd);
			DWORD	CloseCD (MCI_GENERIC_PARMS lpStop);
			DWORD	CloseMIDI (MCI_GENERIC_PARMS lpStop);
			DWORD	CloseWAVE (void);
			DWORD	CloseAVI (void);

			DWORD	CheckAVIPlaying(void);
			DWORD	CheckWAVPlaying(void);

		// Variables
			MCIDEVICEID			cdDeviceID;
			MCIDEVICEID			midiDeviceID;
			MCIDEVICEID			waveDeviceID;

			MCI_OPEN_PARMS		cdOpenParms;
			MCI_OPEN_PARMS		midiOpenParms;
			MCI_OPEN_PARMS		waveOpenParms;

			MCI_PLAY_PARMS		cdPlayParms;
			MCI_PLAY_PARMS		midiPlayParms;
			MCI_PLAY_PARMS		wavePlayParms;

			HWND				aviHWnd;
};

#endif
