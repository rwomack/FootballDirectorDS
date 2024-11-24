#ifndef FDCOMMANDLINEOPTIONS_H_INCLUDED
#define FDCOMMANDLINEOPTIONS_H_INCLUDED

class FDCommandLineOptions
{
public:
	static void ProcessCommandLine(const int argc, const char **argv);

	static bool IsAutoPlay();
	static int GetTeamToPlay();
  static int SeasonsToPlay();
	static bool IsUsingRandomSeedFromCommandLine();
	static unsigned int GetRandomSeedFromCommandLine();

	static bool ShouldLoadGame();
  static bool ShouldQuitAtEnd();

  static void ForceAutoPlay( bool b = true );
private:
	static bool ms_bAutoPlay;
  static bool ms_bQuitAtEnd;
	static int ms_iTeamToPlay;
  static int ms_iSeasonsToPlay;
	static bool ms_bUseRandomSeed;
	static unsigned int ms_uiRandomSeed;
	static bool ms_bLoadGame;
};

#endif // #ifndef FDCOMMANDLINEOPTIONS_H_INCLUDED
