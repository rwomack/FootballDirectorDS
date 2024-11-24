/*                                                                   [Example.c]
** -----------------------------------------------------------------------------
** DS Sound Engine example v1.0
**
** (c)2007 Engine Software BV
** Developed by Jan-Lieuwe Koopmans
** -----------------------------------------------------------------------------
*/

#include <nitro.h>
#include "DirectSound.h"


// number of sound effect channels
#define MAX_SFX_CHANNELS 6

// include the generated arrays with original song filenames
#include "data/music.c"

// include sfx list
#include "data/sfx.h"   //enum list for your sfx calls

// globals

 int sfxnum = 0;
 
struct
{
   u8 songset;
   u8 song;
   u8 fade_song;
   u8 sfxset;

} globals;

// available songsets
typedef struct
{
   const char * const  pFile;
   const char * const *pSongs;
   const char * const  pCredits;

} songset_info;

static const songset_info songsets[] =
{
   { "music.bin", musicFiles, "Orchestralmedia.co.uk" },
   
};

// available sfxsets
static const char * const sfxsets[] =
{
   "sfx.bin"   
};

// forward declarations
void VBlankIntr();
void PrintInfo();

// the example code
void NitroMain()
{
   u32 key_state;

   // NitroSDK initialization
   OS_Init();
   GX_InitEx(1);
   FS_Init(2);

   // Create a heap
   OS_SetArenaLo(OS_ARENA_MAIN, OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1));
   OS_SetCurrentHeap(OS_ARENA_MAIN, OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi()));

   // VBlank interrupt settings
   OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
   (void)OS_EnableIrqMask(OS_IE_V_BLANK);
   (void)OS_EnableIrq();
   (void)GX_VBlankIntr(TRUE);

   globals.songset = 0;
   globals.song = 0;
   globals.sfxset = 0;
   
  
   // Sound Engine initialization

   snd_sys_init(0, NULL, NULL);
   snd_sng_init_s(songsets[globals.songset].pFile);
   snd_sfx_init_s(MAX_SFX_CHANNELS, sfxsets[globals.sfxset]);
  
   
   
   snd_sng_play(globals.song);

   PrintInfo();

   for(;;)
   {   	 
   
   
      // update key input
      const u32 pad = PAD_Read();
      const u32 key_trig = pad & (pad ^ key_state);
      key_state = pad;

      // handle pressed key
      switch (key_trig)
      {
      // play song
      case PAD_BUTTON_A:
         snd_sng_play(globals.song);
         break;

      // stop song
      case PAD_BUTTON_B:
         snd_sng_stop();
         break;

      // play sfx #1
      case PAD_BUTTON_X:
  		 if (sfxnum) 
  		 {
  		 //normally you will call by name using the enum list in sfx.h
  		 
  		 snd_sfx_play(sfxnum, 64, SND_PRI_NORMAL);  		   		                                
  		 sfxnum--;
  		 }
         break;

      // play sfx #2
      case PAD_BUTTON_Y:
      
      	 if (sfxnum<(NUM_SFX-1))
      	 {      	 	      	 
      	 //normally you will call by name using the enum list in sfx.h
      	 
         snd_sfx_play(sfxnum, 64, SND_PRI_NORMAL);
         sfxnum++;
         
      	 }
  	  	
         break;

      // next song
      case PAD_KEY_UP:
      case PAD_KEY_RIGHT:
         globals.song++;
         globals.song %= snd_sng_count();
         if (snd_sng_isPlaying())
         {
            snd_sng_play(globals.song);
         }
         PrintInfo();
         break;

      // prev song
      case PAD_KEY_DOWN:
      case PAD_KEY_LEFT:
         globals.song--;
         globals.song %= snd_sng_count();
         if (snd_sng_isPlaying())
         {
            snd_sng_play(globals.song);
         }
         PrintInfo();
         break;

      // next songset
      case PAD_BUTTON_L:
         {
            const u32 wasPlaying = snd_sng_isPlaying();

            globals.song = 0;
            globals.songset++;
            globals.songset %= sizeof(songsets) / sizeof(songset_info);

            snd_sng_init_s(songsets[globals.songset].pFile);
            if (wasPlaying)
            {
               snd_sng_play(globals.song);
            }
         }
         PrintInfo();
         break;

      // next sfxset
      case PAD_BUTTON_R:
         globals.sfxset++;
         globals.sfxset %= sizeof(sfxsets) / sizeof(char*);
         snd_sfx_init_s(MAX_SFX_CHANNELS, sfxsets[globals.sfxset]);
         PrintInfo();
         break;

      // fade in/out
      case PAD_BUTTON_START:
         if (!snd_sng_isFading())
         {
            if (snd_sng_isPlaying())
            {
               // fade out
               globals.fade_song = globals.song;
               snd_sng_fade(-8);
            }
            else
            {
               snd_sng_fade(+8);
               if (globals.song == globals.fade_song)
               {
                  snd_sng_resume();
               }
               else
               {
                  snd_sng_play(globals.song);
               }
            }
         }
         break;

      default:
         // pressed too many buttons at once
         break;
      }

      // wait for VBlank
      OS_WaitVBlankIntr();
   }
   // stop using the sound engine
   snd_sys_exit();
}

// VBlank handler that doesn't do anything interesting
void VBlankIntr()
{
   OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// Print manual & info to the debug console
void PrintInfo()
{
   OS_PutString("-----------------------------------=[DS Sound Engine example]=-----\n"
                "A button : play song\n"
                "B button : stop song\n"
                "X button : play sfx #1\n"
                "Y button : play sfx #2\n"
                "Up/Right : select next song\n"
                "Down/Left: select previous song\n"
                "L trigger: load next songset\n"
                "R trigger: load next sfxset\n"
                "Start    : fade song in or out\n"
                "-------------------------------------------------------------------\n"
                );
   OS_TPrintf(  "Sfxset : %s\n"
                "Songset: %s\n"
                "Credits: %s\n"
                "Song   : %s\n"
                "-------------------------------------------------------------------\n"
                , sfxsets[globals.sfxset]
                , songsets[globals.songset].pFile
                , songsets[globals.songset].pCredits
                , songsets[globals.songset].pSongs[globals.song]
                );
}
