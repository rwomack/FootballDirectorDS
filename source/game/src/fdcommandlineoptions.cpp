#include <fdmemory.h>
#include <fdcommandlineoptions.h>
#include <string.h>
#include <stdlib.h>

bool FDCommandLineOptions::ms_bAutoPlay = false;
int FDCommandLineOptions::ms_iTeamToPlay = -1;
int FDCommandLineOptions::ms_iSeasonsToPlay = -1;
bool FDCommandLineOptions::ms_bUseRandomSeed = false;
unsigned int FDCommandLineOptions::ms_uiRandomSeed = 42;
bool FDCommandLineOptions::ms_bLoadGame = false;
bool FDCommandLineOptions::ms_bQuitAtEnd = false;

void FDCommandLineOptions::ProcessCommandLine(const int argc, const char **argv)
{
#ifndef L_PLATFORM_NDS
	for (int i = 0; i < argc; i++)
	{
		if (::stricmp(argv[i], "-autoplay") == 0)
		{
			ms_bAutoPlay = true;
			continue;
		}

    if (::stricmp(argv[i], "-quitatend") == 0)
		{
      ms_bQuitAtEnd = true;
			continue;
		}

		if ((::stricmp(argv[i], "-team") == 0) || (::stricmp(argv[i], "-club") == 0))
		{
			i++;
			if (i >= argc)
				break;

			ms_iTeamToPlay = atoi(argv[i]);
			if ((ms_iTeamToPlay < 0) || (ms_iTeamToPlay > 157))
				ms_iTeamToPlay = -1;

			continue;
		}

		if (::stricmp(argv[i], "-randomseed") == 0)
		{
			i++;
			if (i >= argc)
				break;

			extern bool s_bUseSpecificRandomSeed;
			extern unsigned int s_uiSpecificRandomSeed;

			ms_bUseRandomSeed = s_bUseSpecificRandomSeed = true;
			ms_uiRandomSeed = s_uiSpecificRandomSeed = atoi(argv[i]);

			continue;
		}

    if (::stricmp(argv[i], "-seasons") == 0)
		{
			i++;
			if (i >= argc)
				break;

      ms_iSeasonsToPlay = atoi(argv[i]);

			continue;
		}

		if (::stricmp(argv[i], "-loadgame") == 0)
		{
			ms_bLoadGame = true;
			continue;
		}
	}
#endif
}

bool FDCommandLineOptions::IsAutoPlay()
{
	return ms_bAutoPlay;
}

int FDCommandLineOptions::SeasonsToPlay()
{
  return ms_iSeasonsToPlay;
}

int FDCommandLineOptions::GetTeamToPlay()
{
	return ms_iTeamToPlay;
}

bool FDCommandLineOptions::IsUsingRandomSeedFromCommandLine()
{
	return ms_bUseRandomSeed;
}

unsigned int FDCommandLineOptions::GetRandomSeedFromCommandLine()
{
	return ms_uiRandomSeed;
}

bool FDCommandLineOptions::ShouldLoadGame()
{
	return ms_bLoadGame;
}

void FDCommandLineOptions::ForceAutoPlay( bool b )
{
  ms_bAutoPlay = b;
}

bool FDCommandLineOptions::ShouldQuitAtEnd()
{
  return ms_bQuitAtEnd;
}
