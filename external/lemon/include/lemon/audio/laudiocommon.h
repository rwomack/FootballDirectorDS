//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : laudio.h
// Description : Common audio interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./laudioapi.h"
#include "../containers/larray.h"

#ifndef LAUDIOCOMMON_H_INCLUDED
#define LAUDIOCOMMON_H_INCLUDED

BEGIN_L_NAMESPACE

// Common interface
//
// These define a common interface, but no default implementation is supplied.
// The platform should implement the base members, as well as deriving and extending
// if required.

class lAudioDriverDescCommon; // Common audio driver description interface
class lAudioOutputDescCommon; // Common audio output description interface
class lAudioDeviceCommon;     // Common audio device interface

// Pre-declaration for platform specific derived interfaces

class lAudioDriverDesc;      // Platform audio driver description interface
class lAudioOutputDesc;      // Platform audio output description interface
class lAudioDevice;          // Platform audio device interface

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Audio typedefs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// audio array trait
typedef TArrayTrait<3,8,16,L_ARENA_AUDIO> TAudioArrayTrait;  // 3/8 + 16

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Audio defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_AUDIO_CHANNEL_INDEX_BITS 12     // bits used to audio channel
#define L_AUDIO_CHANNEL_REF_BITS   20     // bits used for channel reference

#define L_AUDIO_CHANNEL_INVALID    (0xffffffff)

// driver ids

#define L_AUDIO_DEFAULT_DRIVER       0
#define L_AUDIO_DEFAULT_OUTPUT       0
#define L_AUDIO_DEFAULT_VOICE_OUTPUT -1

#define L_SAMPLE_PCM_FORMAT( _dataType, _bitsPerContainer, _bitsPerSample, _numChannels ) \
  ( ((uint8)_dataType) + (((uint8)_bitsPerContainer)<<8) + (((uint8)_bitsPerSample)<<16) + (((uint8)_numChannels)<<24) )

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSAMPLE_DATA
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//##
// Sound sample data types
enum lSAMPLE_DATA
{
  L_SAMPLE_DATA_UNKNOWN     = 0x00, // unknown data type
  L_SAMPLE_DATA_PCM_INTEGER = 0x01, // n-bit linear integer two's-complement pulse code modulation
  L_SAMPLE_DATA_PCM_FLOAT   = 0x02, // n-bit linear IEEE float pulse code modulation
  L_SAMPLE_DATA_G711_ALAW   = 0x03, // CCITT ADPCM G711 A-law (can be 8, 10, 12, or 16 bit?)
  L_SAMPLE_DATA_G711_ULAW   = 0x04, // CCITT ADPCM G711 u-law (can be 8, 10, 12, or 16 bit?)
  L_SAMPLE_DATA_G723_24     = 0x05, // 3-bit CCITT ADPCM G723 24kbps
  L_SAMPLE_DATA_G721_32     = 0x06, // 4-bit CCITT ADPCM G721 32kbps
  L_SAMPLE_DATA_G723_40     = 0x07, // 5-bit CCITT ADPCM G723 40kbps
  L_SAMPLE_DATA_VAG_ADPCM   = 0x08, // Sony VAG ADPCM format (16 byte blocks, 16 bit header, 28x4 bit samples)
  L_SAMPLE_DATA_IMA_ADPCM   = 0x09, // Nintendo DS IMA ADPCM format
  L_SAMPLE_DATA_DSP_ADPCM   = 0x10, // Nintendo Wii DSP ADPCM format
  L_SAMPLE_DATA_MAX         = 0x10,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSAMPLE_FORMAT
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//##
// Sound sample formats (a combination of data types, bits per container, bits per sample, channels)
enum lSAMPLE_FORMAT
{
  L_SAMPLE_UNKNOWN     = 0x00, // unknown type

  L_SAMPLE_PCM_I8_1CH    = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_PCM_INTEGER,8,8,1),
  L_SAMPLE_PCM_I8_2CH    = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_PCM_INTEGER,8,8,2),
  L_SAMPLE_PCM_I16_1CH   = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_PCM_INTEGER,16,16,1),
  L_SAMPLE_PCM_I16_2CH   = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_PCM_INTEGER,16,16,2),

  L_SAMPLE_VAG_ADPCM_1CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_VAG_ADPCM,4,4,1),
  L_SAMPLE_VAG_ADPCM_2CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_VAG_ADPCM,4,4,2),

  L_SAMPLE_IMA_ADPCM_1CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_IMA_ADPCM,4,4,1),
  L_SAMPLE_IMA_ADPCM_2CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_IMA_ADPCM,4,4,2),

  L_SAMPLE_DSP_ADPCM_1CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_DSP_ADPCM,4,4,1),
  L_SAMPLE_DSP_ADPCM_2CH = L_SAMPLE_PCM_FORMAT(L_SAMPLE_DATA_DSP_ADPCM,4,4,2),

  lSAMPLE_FORCE_DWORD  = 0xffffffff
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSAMPLE_FLAG
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//##
// Sound sample flags
enum lSAMPLE_FLAG
{
  L_SAMPLE_FLAG_COMPRESSED   = 0x01,
  L_SAMPLE_FLAG_VBR          = 0x02,
  L_SAMPLE_FLAG_USER_POINTER = 0x04,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSound
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef L_AUDIO_SUPPORT_IFF
struct lIFFContext;
#endif

#ifdef L_AUDIO_SUPPORT_IMPORT_IOSTREAM
class lIOStreamSeekable; // for lSound::Import
#endif

//##
// Sound data container, allows import / export and querying of sound data
class LAUDIO_API lSound
{
  // sample format,      8-bits (an lSAMPLE_DATA)
  // bits per container, 8-bits
  // bits per sample,    8-bits
  // number of channels, 8-bits
  uint32 m_type;
  uint32 m_frequency;
  uint32 m_dataSize;
  uint32 m_flags;
  void * m_data;

public:

  lSound();
  lSound( const lSound & );
  lSound( lSAMPLE_FORMAT format, unsigned int sampleFrequency, unsigned int dataByteSize, unsigned int flags );

  ~lSound();

  lSound &operator=(const lSound &);

  void           SetHeader( lSAMPLE_FORMAT format, unsigned int sampleFrequency, void * data, unsigned int dataByteSize, unsigned int flags );
  void           Reset();

  // sound information

  lSAMPLE_FORMAT Format() const;         // sample format
  unsigned int   Frequency() const;      // sample frequency in Hz
  float          Length() const;         // sample length in seconds

  // type information

  lSAMPLE_DATA   DataType() const;
  unsigned int   BitsPerSample() const;
  unsigned int   BitsPerContainer() const;
  unsigned int   NumChannels() const;
  unsigned int   Flags() const;
  unsigned int   BlockSize() const;
  unsigned int   NumSamples() const;

  // flag access

  unsigned int   GetFlags() const;

  // data access interface

  void         * GetData() const;
  unsigned int   GetDataSize() const;

  // sound manipulation operations

  lSound       * Copy() const;

  // import/export

  static lSound * Import( const char * filename );
  #ifdef L_AUDIO_SUPPORT_IMPORT_IOSTREAM
  static lSound * Import( lIOStreamSeekable *, const char * filename );
  #endif
  bool            Export( const char * filename ) const;

  // load / save to IFF

  #ifdef L_AUDIO_SUPPORT_IFF
  static lSound * LoadIFF( lIFFContext *ctxt );
  void            SaveIFF( lIFFContext *ctxt );
  #endif

  // static sound information/manipulation interface

  // type information

  static const char *   DataName(lSAMPLE_DATA);
  static lSAMPLE_DATA   DataId(const char *str);

  static lSAMPLE_DATA   DataType(lSAMPLE_FORMAT);
  static unsigned int   BitsPerSample(lSAMPLE_FORMAT);
  static unsigned int   BitsPerContainer(lSAMPLE_FORMAT);
  static unsigned int   NumChannels(lSAMPLE_FORMAT);
  static unsigned int   Flags(lSAMPLE_FORMAT);

  static unsigned int   BlockSize(lSAMPLE_FORMAT);
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lSoundStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Virtual base class for stream classes to derive from, used by the lAudio::PlayStream function
// for play back of streaming sounds
class LAUDIO_API lSoundStream
{
public:
  virtual ~lSoundStream() {}

  // sound information of output

  virtual lSAMPLE_FORMAT Format() const = 0;         // sample format
  virtual unsigned int   Frequency() const = 0;      // sample frequency in Hz
  virtual unsigned int   GetFlags() const = 0;       // sample flags

  // recommendation on buffers from stream? i.e. number of buffers, amount to buffer

  //##
  // Read data from stream
  // The function should return all of the data, any not returned will be filled with silence
  virtual uint32       Read( void *data, uint32 size ) = 0;
  //virtual uint32     Read(void *, uint32, void *, uint32) = 0;  // this one is the advanced version which will be written in base to use the above

  //##
  // Rewind the stream to the start
  virtual void         Rewind( bool bFreshPlay ) = 0;

  //##
  // Is this EOF ?
  virtual bool         IsEOF() = 0;

  // Perhaps derive from lSound to define types etc (or make a base type)
  // classes can give options to set the output or should we expose all possible outputs and let the audio stream choose? (seems a bit too automated)
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioDriverDescCommon - Common audio driver description interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Common audio driver description interface
class LAUDIO_API lAudioDriverDescCommon
{
public:
  const char * name;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioOutputDescCommon - Common audio output description interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Common audio output description interface
class LAUDIO_API lAudioOutputDescCommon
{
public:
  const char * name;

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioDeviceCommon - Common audio device interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Common audio device interface
class LAUDIO_API lAudioDeviceCommon
{
public:

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPLAY_FLAG
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Playback flags used with lAudio::Play
enum lPLAY_FLAG
{
  L_PLAY_FLAG_NONE               = 0x00,
  L_PLAY_FLAG_LOOP               = 0x01,  //## Play this sound back looped
  L_PLAY_FLAG_CACHE_PERSISTENT   = 0x02,  //## PS2 only: Force this sound to be cached permanently in the SPU cache
                                          //   (note this will fill the SPU memory and you will have to flush using lAudioPS2::FlushSPUCache()
  L_PLAY_FLAG_PREBUFFER_STREAM   = 0x04,  //## PC only: Prebuffer the full size of the stream buffer
  L_PLAY_FLAG_START_PAUSED       = 0x08,  //## Start the playback of this sound paused
  L_PLAY_FLAG_DONT_STEAL         = 0x10,  //## Don't steal a channel when attempting the Play

  // NOTE: We should merge these two flags

  L_PLAY_FLAG_PS2_SET_LOOP_DATA  = 0x20,  //## PS2 ONLY: Set the loop data flags in the sound data (this increases the execution time of ::Play)
                                          //   as it has to touch all the data setting the LOOP flags in the ADPCM headers and setting the LOOP_START
                                          //   and LOOP_END flags in the ADPCM headers at the start + end of the data

  L_PLAY_FLAG_PSP_SET_LOOP_DATA  = 0x20,  //## PSP ONLY: Set the loop data flags in the sound data (this increases the execution time of ::Play)
                                          //   as it has to touch all the data setting the LOOP flags in the ADPCM headers and setting the LOOP_START
                                          //   and LOOP_END flags in the ADPCM headers at the start + end of the data

  L_PLAY_FLAG_BACKGROUND_UPLOAD  = 0x40,  //## PS2 ONLY: Perform the upload of the sound in the background, the sound will not play immediately on
                                          //   calling ::Play but will be added to a queue of sounds being loaded in the background which are
                                          //   processed during lAudio::Update. Currently this functionality is PS2 only but the flag will become
                                          //   useable on other platforms.

  L_PLAY_FLAG_STREAM_UPLOAD      = 0x80,  //## PS2 ONLY: This streams the sound data as it is needed rather than uploading the whole sample into SPU ram
                                          //   this has extra overhead during processing inside lAudio::Update but enables multiple very large samples
                                          //   to be played. Note: Currently this is PS2 only for MONO ADPCM sounds.
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPRIORITY_FLAG
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  L_PRIORITY_MASK                    = 0x00ffffff,
  L_PRIORITY_FLAG_NEVER_STEAL        = 0x80000000, //## Set this sound playback so it wont be stolen while it is playing
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudioChannel - Audio channel interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Audio channel interface
class LAUDIO_API lAudioChannel
{
  uint32 m_id;
public:
  lAudioChannel();
  lAudioChannel( int index );

  lAudioChannel & operator = ( const lAudioChannel &s );
  //bool            operator == ( const lAudioChannel &s ) const;
  //bool            operator != ( const lAudioChannel &s ) const;

  //operator uint32 & ();
  operator const uint32 & () const;

  int GetIndex()    { return m_id & ((1<<L_AUDIO_CHANNEL_INDEX_BITS)-1); }
  int GetRefCount() { return (m_id>>L_AUDIO_CHANNEL_INDEX_BITS) & ((1<<L_AUDIO_CHANNEL_REF_BITS)-1); }

  //##
  // Play this sound on this channel
  bool Play( lSound *, uint32 flag = L_PLAY_FLAG_NONE );

  //##
  // Play this sound stream on this channel
  bool PlayStream( lSoundStream *, uint32 flag = L_PLAY_FLAG_NONE );

  //##
  // Stop playback on this channel
  void Stop();

  //##
  // Pause playback on this channel
  void Pause();

  //##
  // Resume playback on this channel
  void Resume();

  //##
  // Set volume on this channel
  void SetVolume( float );

  //##
  // Get volume on this channel
  float GetVolume();

  //##
  // Set pan on this channel
  void SetPan( float );

  //##
  // Get pan on this channel
  float GetPan();

  //##
  // Return whether this channel is still playing
  bool IsPlaying();

  //##
  // Set the sample frequency on this channel
  void SetFrequency(uint32);

  //##
  // Get the sample frequency on this channel
  uint32 GetFrequency();
};

// lAudioChannel inline methods

inline lAudioChannel::lAudioChannel() : m_id(L_AUDIO_CHANNEL_INVALID)                { }
inline lAudioChannel::lAudioChannel( int index ) : m_id(index)                       { }

inline lAudioChannel & lAudioChannel::operator = ( const lAudioChannel &s )          { m_id = s.m_id; return *this; }
//inline bool            lAudioChannel::operator == ( const lAudioChannel &s ) const   { return m_id == s.m_id; }
//inline bool            lAudioChannel::operator != ( const lAudioChannel &s ) const   { return m_id != s.m_id; }

//inline lAudioChannel::operator uint32 & ()                                           { return m_id; }
inline lAudioChannel::operator const uint32 & () const                               { return m_id; }


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lAudio - Common static audio interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Common static audio interface
class LAUDIO_API lAudio
{
public:

  // initialisation

  static void                   Init();
  static void                   Deinit();

  // device interface

  static const lAudioDriverDesc * DriverGetAvail(unsigned int *);
  static const lAudioOutputDesc * OutputGetAvail(int, unsigned int *);

  static lAudioDevice           * DeviceCreate(int driverId, int outputId, void *platformSpecific );
  static void                     DeviceDestroy(lAudioDevice *);
  static bool                     DeviceSelect(lAudioDevice *);
  static void                     DeviceEnable(lAudioDevice *, bool );
  static lAudioDevice           * DeviceGetDefault();
  static lAudioDevice           * DeviceGetCurrent();

  static void                     Update();

  // channel

  static lAudioChannel            GetChannel( int index );

  //static lAudioChannel            Play( lSound *, lAudioChannel channelToReuse, uint32 flag = L_PLAY_FLAG_NONE, uint32 priority = 0 );
  static lAudioChannel            Play( lSound *, lAudioChannel channelToReuse, uint32 flag = L_PLAY_FLAG_NONE );
  static lAudioChannel            PlayStream( lSoundStream *, lAudioChannel channelToReuse, uint32 flag = L_PLAY_FLAG_NONE );

  static void                     ChannelStop( lAudioChannel );
  static void                     ChannelPause( lAudioChannel );
  static void                     ChannelResume( lAudioChannel );
  static bool                     ChannelIsPlaying( lAudioChannel );

//  static void                     ChannelSetPriority( lAudioChannel, uint32 );
//  static uint32                   ChannelGetPriority( lAudioChannel );

  static void                     ChannelSetVolume( lAudioChannel, float );
  static float                    ChannelGetVolume( lAudioChannel );

  static void                     ChannelSetPan( lAudioChannel, float );
  static float                    ChannelGetPan( lAudioChannel );

  static void                     ChannelSetFrequency( lAudioChannel, uint32 );
  static uint32                   ChannelGetFrequency( lAudioChannel );
};



END_L_NAMESPACE
#endif
