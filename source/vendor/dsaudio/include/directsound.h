// ---[ DS SoundEngine v1.08 header ]-------------------------------------------
//
// (c)2001-2007 by Engine Software BV
// Developed by Jan-Lieuwe Koopmans
// -----------------------------------------------------------------------------

#ifndef DIRECTSOUND_H_INCLUDED
#define DIRECTSOUND_H_INCLUDED

// --- header files ------------------------------------------------------------
#include <nitro/types.h>
#include <nitro/spi.h>

// --- definitions -------------------------------------------------------------
// (changing these values requires recompilation of the sound engine)

// --- conditional compilation
// (these include extra code)

#ifndef SDK_FINALROM
#   define SND_SAFE_MODE // --> description:
#endif
// checks parameters and variables when they can be invalid.
// (disabling it will preserve some space and a tiny bit of CPU power)

#define SND_INFO_SUPPORT // --> description:
// implement the snd_sng_getInfo/snd_sng_setInfo functions
// (if you have the sourcecode, you don't really need these functions.)

#define SND_DONT_STRIP_FREQUENCIES // --> description:
// also store frequencies in the table which are hardly used by composers.
// (it adds around 8kB of data.)

#define SND_ALLOW_EFFECT_HANDLER // --> description:
// adds effect handler functionality.

#define SND_STREAMING_SUPPORT // --> description:
// adds support for sample streaming (requires nitro's file system)

#define SND_STREAMING_ASYNC // --> description:
// enable Async streaming (only valid when SND_STREAMING_SUPPORT is enabled)

//#define SND_MICROPHONE_SUPPORT // --> description:
// adds support for the built in microphone

// --- compilation constants

// which channels can be used by the engine? (0 = don't touch, 1 = usable)
//      Channel                F    Capabilities
//      ---------              --   ----------------------                       
#define SND_CHN_0              1 // PCM, ADPCM, CAPTURE #0
#define SND_CHN_1              1 // PCM, ADPCM, CAPTURE #0
#define SND_CHN_2              1 // PCM, ADPCM, CAPTURE #1
#define SND_CHN_3              1 // PCM, ADPCM, CAPTURE #1
#define SND_CHN_4              1 // PCM, ADPCM
#define SND_CHN_5              1 // PCM, ADPCM
#define SND_CHN_6              1 // PCM, ADPCM
#define SND_CHN_7              1 // PCM, ADPCM
#define SND_CHN_8              1 // PCM, ADPCM, SQUARE
#define SND_CHN_9              1 // PCM, ADPCM, SQUARE
#define SND_CHN_A              1 // PCM, ADPCM, SQUARE
#define SND_CHN_B              1 // PCM, ADPCM, SQUARE
#define SND_CHN_C              1 // PCM, ADPCM, SQUARE
#define SND_CHN_D              1 // PCM, ADPCM, SQUARE
#define SND_CHN_E              1 // PCM, ADPCM, NOISE
#define SND_CHN_F              1 // PCM, ADPCM, NOISE

// maximum number of mixer channels (count the '1's up here when using default)
//  (lower value decreases memory usage)
#define SND_MAX_CHANNELS       16

// maximum number of channels used for music (must be <= SND_MAX_CHANNELS)
//  (lower value decreases memory usage, other channels can still be used for sfx)
#define SND_MAX_MUS_CHANNELS   16

// strength of vibration: 0 = normal, 1 = strong, 2 = very strong
#define SND_VIBRATO_STRENGTH   0

// sample rate of the capture used for post-processing effects
#define SND_EFFECT_SAMPLE_RATE 32000

// processing interval of the capture used for some post-processing effects
//  (currently unused)
#define SND_EFFECT_INTERVAL    4

// dma channel used by sound engine
//  (only used to clean buffers)
#define SND_DMA_CHANNEL        3

// can ITCM be used and how aggressively?
//  (0 means: don't use ITCM, 1 means: use a bit of ITCM .. 8 means: use lots)
#ifndef SND_USE_ITCM
#   ifdef SDK_DEBUG
#      define SND_USE_ITCM     0
#   else
#      define SND_USE_ITCM     1
#   endif
#endif

// --- version info

// engine version string
#define SND_ENGINE_VERSION    "1.08"
// data version (1.01)
#define SND_VERSION           0x101

// --- enumerated definitions --------------------------------------------------

// --- enumeration containing all mixer post-processing effects
// (use this in calls to 'snd_mix_setEffect')
enum _snd_mix_effects
{
   SND_MIX_NORMAL    = 0,
   SND_MIX_REVERB
};

// --- enumeration you can use for sound effect priorities 
// (use this in calls to 'snd_sfx_play[Ex]')
enum _snd_sfx_priority
{
   SND_PRI_VERY_LOW  = 0,
   SND_PRI_LOW       = 8,
   SND_PRI_NORMAL    = 16,
   SND_PRI_HIGH      = 32,
   SND_PRI_VERY_HIGH = 64
   // etc.
};

// --- enumeration used to get (r) or set (w) song info 
// (use this in calls to snd_sng_getInfo/snd_sng_setInfo)
enum _snd_sng_info_type
{
   SND_INFO_ROW     = 0,   // [rw] current row
   SND_INFO_POS,           // [rw] current position (write: activates when current pattern ends)
   SND_INFO_POS_ROW,       // [ w] current position (hi 16 bits) and row (lo 16 bits)
   SND_INFO_PAT,           // [r ] current pattern
   SND_INFO_BPM,           // [rw] current tempo in beats per minute
   SND_INFO_LOOPS,         // [r ] amount of loops performed
   SND_INFO_MUTE,          // [ w] mute channels   (bit 0 = chn 1, bit 1 = chn 2, etc.)
   SND_INFO_UNMUTE         // [ w] unmute channels (bit 0 = chn 1, bit 1 = chn 2, etc.)
   // etc. (more can be added on request)
};

// --- enumeration with some extra songNr bit masks 
// (use this in calls to snd_sng_play)
enum _snd_sng_extra_bitmasks
{
   SND_SNG_JINGLE     = 0x100  // bitwise OR with songNr when you want previous song 
                               // to continue playing after this song finishes playing
                               // (currently only works in non-streaming mode;
                               //  make sure active song is stopped when using this in
                               //  streaming mode)
};

// --- extended sound effects structure
typedef struct
{
   u16 status;       // + 0 status (read only, updated every time you call extended sfx functions)
   u16 fx;           // + 2 effect number
   u16 pri;          // + 4 priority
   u8  vol;          // + 6 volume (1..64)
   s8  pan;          // + 7 panning (-64..+63)
   u32 freq;         // + 8 playback frequency (0 = use default)
   s32 loopStart;    // +12 start of loop (offset in samples, negative means no loop)
                     //(+16)
} SND_SFX_EX; // 16 bytes

// masks for status flags
enum _snd_sfx_status
{
   SND_SFX_MUTE    = 0x001,  // if set: sound effect is muted (not playing)
   SND_SFX_PAUSE   = 0x002   // if set: sound effect is paused
};

// --- enumeration containing microphone related constants
enum _snd_mic_constants
{
   SND_MIC_SAMPLING_TYPE_U8             = MIC_SAMPLING_TYPE_8BIT,
   SND_MIC_SAMPLING_TYPE_U12            = MIC_SAMPLING_TYPE_12BIT,
   SND_MIC_SAMPLING_TYPE_S8             = MIC_SAMPLING_TYPE_SIGNED_8BIT,
   SND_MIC_SAMPLING_TYPE_S12            = MIC_SAMPLING_TYPE_SIGNED_12BIT,
   SND_MIC_SAMPLING_TYPE_U12_UNFILTERED = MIC_SAMPLING_TYPE_12BIT_FILTER_OFF,
   SND_MIC_SAMPLING_TYPE_S12_UNFILTERED = MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF,

   SND_MIC_AMPGAIN_20X                  = PM_AMPGAIN_20,
   SND_MIC_AMPGAIN_40X                  = PM_AMPGAIN_40,
   SND_MIC_AMPGAIN_80X                  = PM_AMPGAIN_80,
   SND_MIC_AMPGAIN_160X                 = PM_AMPGAIN_160,
   SND_MIC_AMPGAIN_DEFAULT              = PM_AMPGAIN_DEFAULT
};

// --- interface functions -----------------------------------------------------
// (See Manual.pdf for a detailed description of each function.)

typedef void *(*allocFuncPtr)(u32 size, u32 align);
typedef void  (*freeFuncPtr) (void *ptr);

#ifdef __cplusplus
extern "C" {
#endif

// --- system functions
void snd_sys_init           (u32 chnMask,               // which channels the engine can use (0 = default mask)
                             allocFuncPtr,              // pointer to allocation function (NULL = use OS_Alloc)
                             freeFuncPtr);              // pointer to deallocation function (NULL = use OS_Free)
void snd_sys_exit           (void);
void snd_sys_panic          (void);

// --- mixer functions
void snd_mix_setMasterVol   (u32 volume);               // mixer master volume (0..127)
void snd_mix_stopAllSound   (void);
void snd_mix_enableEffects  (void *buffer,              // ptr to effect buffer
                             u32 size);                 // size of the effect buffer
void snd_mix_disableEffects (void);
void snd_mix_setEffect      (u32 effect,                // post-processing effect
                             u32 volume);               // post-processing effect volume (0..127)
void snd_mix_setEffectVol   (u32 volume);               // post-processing effect volume (0..127)

// --- song related functions
void snd_sng_init           (const void *pSngSet);      // ptr to songset (4 byte aligned)
void snd_sng_init_s         (const char *pSngSetFile);  // ptr to songset filename
u32  snd_sng_count          (void);
void snd_sng_play           (u32 sngNr);                // song number (with optional OR for extra functionality)
u32  snd_sng_isPlaying      (void);
u32  snd_sng_isJingle       (void);
void snd_sng_stop           (void);
void snd_sng_pause          (void);
u32  snd_sng_isPaused       (void);
void snd_sng_resume         (void);
void snd_sng_fade           (s32 fadeSpd);              // fade speed (fadeout < 0 > fadein)
u32  snd_sng_isFading       (void);

// --- advanced song related functions
void snd_sng_setEffHandler  (void(*pHandler)(u32,u32)); // ptr to handler: void func(u32 eff,u32 dat)
void snd_sng_setChannels    (u32 channels);             // music channels (0 = off)
void snd_sng_setMasterVol   (u32 volume);               // music master volume (0..256)
void snd_sng_setInfo        (u32 type,                  // info type
                             u32 data);                 // info parameter
u32  snd_sng_getInfo        (u32 type);                 // info type

// --- sound effect related functions
void snd_sfx_init           (u32 channels,              // sfx channels (0 = off)
                             const void *pSfxSet);      // ptr to sfxset (4 byte aligned)
void snd_sfx_init_s         (u32 channels,              // sfx channels (0 = off)
                             const char *pSfxSetFile);  // ptr to sfxset filename
u32  snd_sfx_count          (void);
void snd_sfx_play           (u32 sfxNr,                 // sfx number
                             u32 volume,                // sfx volume (0..64[..128])
                             u32 priority);             // sfx priority
void snd_sfx_pauseAll       (void);
void snd_sfx_resumeAll      (void);
void snd_sfx_stopAll        (void);
u32  snd_sfx_isPlaying      (u32 sfxNr);                // sfx number

// --- advanced sound effect related functions
void snd_sfx_playEx         (SND_SFX_EX *pSfxEx);       // 'static' ptr to SND_SFX_EX struct
void snd_sfx_updateEx       (SND_SFX_EX *pSfxEx);       //             .
void snd_sfx_pauseEx        (SND_SFX_EX *pSfxEx);       //             .
void snd_sfx_resumeEx       (SND_SFX_EX *pSfxEx);       //             .
void snd_sfx_stopEx         (SND_SFX_EX *pSfxEx);       //             .
void snd_sfx_setChannels    (u32 channels);             // sfx channels
void snd_sfx_setMasterVol   (u32 volume);               // sfx master volume (0..256)

// --- microphone functions
void snd_mic_init           (u32 samplingType,          // sampling type
                             u32 samplingRate,          // sampling rate in Hz
                             u32 gain);                 // amp gain aka input volume (0 = default)
void snd_mic_exit           (void);
s32  snd_mic_poll           (void);
/* TODO
void snd_mic_pollAsync      (void);
void snd_mic_start          (void *pBuffer,
                             u32 size,
                             u32 loop);
void snd_mic_stop           (void);
void snd_mic_play           (void);
*/

// --- compressed sound stream related functions
// (N/A)

#ifdef __cplusplus
}
#endif

#ifndef SND_STREAMING_SUPPORT
#   undef SND_STREAMING_ASYNC // make sure this is not defined when streaming support is disabled
#endif

#endif
