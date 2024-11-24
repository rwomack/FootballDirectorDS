//����������������������������������������������������������������������������
// File        : main.cpp
// Description : Game entry point
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#ifdef USE_DLMALLOC
#  include <dlmalloc.h>
#endif

#include <lemon/lapi.h>
#ifdef L_PLATFORM_WINDOWS
#include <shellapi.h>
#endif

//#define ENABLE_SPLASHSCREENS
#define ENABLE_TITLESCREEN
//#define PSP_MOUSE

#ifdef L_PLATFORM_PSP
  //#define DISABLE_LOG
#endif

#define ENABLE_NEW_LAYOUT

// #define ENABLE_MEMCHECK_KEY
// #define ENABLE_SKIP_KEY
// #define DS_AUTOPLAY
#ifdef L_PLATFORM_WINDOWS
 //#define ENABLE_MEMCHECK_KEY
 #define ENABLE_SKIP_KEY
#endif

#include <lemon/render/lrender.h>
#include <lemon/input/linput.h>

#include <fdgame.h>
#include <fdtitle.h>
#include <fdresourcemanager.h>
#include <lemon/audio/laudio.h>
#include <framework/levent.h>
#include <game/lgameworld.h>
#include <game/lgameclient.h>
#include <framework/lsplashscreen.h>
#include <framework/lspritebank.h>
#include <framework/lsprite.h>
#include <framework/lspritemanager.h>
#include <framework/lfont.h>
#include <framework/ltext.h>
#include <framework/ldialog.h>
#include <ui/luimouse.h>
#include <ui/luiwidget.h>
#include <fdtext.h>
#include <fdsound.h>
#include <fdcommandlineoptions.h>
#include <fdinput.h>
#include <fdconfig.h>

using namespace L_NAMESPACE_NAME;

#if defined(L_PLATFORM_NDS)
#include <framework/dsrender.h>
#endif

#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif // #ifdef _DEBUG
#endif // #ifdef _WIN32

#if defined(L_PLATFORM_NDS)
#  include "main-nds.cpp"
#endif

#if defined(L_PLATFORM_WINDOWS)
#  include "main-win.cpp"
#endif

#if defined(L_PLATFORM_PSP)
#  include "main-psp.cpp"
#endif

#if defined(L_PLATFORM_WII)
#  include "main-wii.cpp"
#endif


extern bool s_bPerformingCARDAccess;

//����������������������������������������������������������������������������
// MainClient
//����������������������������������������������������������������������������

class MainClient : public lGameClient
{
public:
  bool debounce;
  lSharedPtr<lSpriteBank> s_mouseBank;
  const lSpriteBankSprite * s_mouseSprite;

  MainClient()
  {
    s_mouseBank = FDResourceManager::GetSpriteBank("ui", false);
    s_mouseSprite = s_mouseBank->FindSprite("mouse");

    if (FDCommandLineOptions::IsAutoPlay() == false)
    {
      #ifdef ENABLE_SPLASHSCREENS
      lSharedPtr<lSpriteBank> sb = FDResourceManager::GetSpriteBank("splash");

#if defined(L_PLATFORM_NDS) || defined(L_PLATFORM_WINDOWS)
      if( FDConfig::PlatformNDS() )
      {
        lSplashScreen::AddImage( sb, "fl_splash",       "pinnacle",           2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "sportsdirector", "fdl",                 2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "DISCLAIMER-S1B", "DISCLAIMER-S2B",      5.0f, 0.5f, 0.5f, false );
      }
#endif
#if !defined(L_PLATFORM_NDS)
      if( !FDConfig::PlatformNDS() )
      {
        lSplashScreen::AddImage( sb, "fl_splash",       2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "sportsdirector",  2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "DISCLAIMER-S1B",  5.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "pinnacle",        2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "fdl",             2.0f, 0.5f, 0.5f, false );
        lSplashScreen::AddImage( sb, "DISCLAIMER-S2B",  5.0f, 0.5f, 0.5f, false );
      }
#endif

      lSplashScreen::Start();
      #endif
    }

    debounce = true;

    FDTitle::Init();
    FDSound::PlayMusic( FD_MENU_MUSIC );
  }

  ~MainClient()
  {
  }

  virtual void ClientInit()
  {
    lLogSys("MainClient","ClientInit\n");
  }

  virtual void ClientDeinit()
  {
    lLogSys("MainClient","ClientDeinit\n");
  }

  virtual void Tick(float delta)
  {
    if( !lSplashScreen::IsFinished() )
    {
      lSplashScreen::Tick(delta);
    }
    #ifdef ENABLE_TITLESCREEN
    else if( FDTitle::IsOpen() )
    {
      // splashscreen leaves brightness down
      #if defined(L_PLATFORM_NDS)
      GX_SetMasterBrightness( 0 );
      GXS_SetMasterBrightness( 0 );
      #endif

      FDTitle::Tick(delta);
      if( !FDTitle::IsOpen())
      {
        lLogFmt("Init title closing\n");
        GetGame()->GetWorld()->BeginPlay();
      }
    }
    #endif
    else
    {
      lSplashScreen::Clear();

      FDTitle::Deinit();
    }
  }

  virtual void Render()
  {
    #if !defined(L_PLATFORM_NDS)
    lRender::BufferClear( L_ARGB(0,0,0,0) );
    #else

    if (PAD_DetectFold() == TRUE)
    {
      if( !s_bPerformingCARDAccess )
      {
        PM_GoSleepMode(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
      } else
      {
        lLogFmt("Fold detected - waiting on card access\n");
      }
    }

    DSRender_StartFrame();
    #endif

    if( !lSplashScreen::IsFinished() )
    {
      lSplashScreen::Render();
    }
    #ifdef ENABLE_TITLESCREEN
    else if( FDTitle::IsOpen() )
    {
      FDTitle::Render();
    }
    #endif
    else
    {
      GetGame()->GetWorld()->RenderClientView( GetClientView(), 0, NULL );
    }

#ifdef PSP_MOUSE
    // Render mouse
    SpriteState state;
    state.x = lUIMouse::GetPixelX();
    state.y = lUIMouse::GetPixelY();
    state.w = s_mouseSprite->width;
    state.h = s_mouseSprite->height;
    state.clipX0 = 0;
    state.clipY0 = 0;
    state.clipX1 = 1;
    state.clipY1 = 1;
    state.rotation = 0;
    state.flipFlags = 0;
    state.frameIndex = 0;
    state.colour = L_RGBA(255,255,255,255);
    state.transparent = 1;

    lSpriteManager::Render(s_mouseBank.Get(), s_mouseSprite, &state );
#endif
    #ifdef L_PLATFORM_PSP
    lRenderPSP::BufferSwapEndFrame();
    lEvent::CheckPostRender();
    lRenderPSP::BufferSwapStartNextFrame();
    #else
    lSpriteManager::BufferSwap();
    #endif
  }

  virtual void RenderLeafs( lGameRenderContext *ctx, lGameLeaf ** leafList, int numLeafs )
  {
  }
};


//����������������������������������������������������������������������������
// Main entry
//����������������������������������������������������������������������������

void LogNull( const char *str )
{
  // do nothing
}

static FDGame     * s_game   = NULL;
static MainClient * s_client = NULL;
static float s_oldTime = 0.0f;

extern bool s_skipButtonPressed;
extern bool s_linksButtonPressed;

// Testing
extern bool s_nextScreen;
extern bool s_prevScreen;

extern bool s_resetMenuItems;
extern bool s_bUpdate;

// PSP UserMessageBox controls
extern bool s_userMessageBoxActive;
extern bool s_leftPressed;
extern bool s_rightPressed;
extern bool s_activatePressed;

extern bool s_backButton;

bool DoMainLoop()
{
  if( !lPlatform_Process() )
  {
    lLogFmt("Process returned false\n");
    return false;
  }

 #if !defined(L_PLATFORM_NDS)
  if( !lRender::IsReady() )
  {
    return true;
  }
 #endif

#ifdef L_PLATFORM_NDS
  if ( CARD_IsPulledOut() ) // check if card is pulled out
  {
    DSCardEjectHandle();
  }
#endif

  float newTime   = lPlatform_GetSeconds();
  float deltaTime = (newTime - s_oldTime);
  s_oldTime = newTime;

  lInput->Update();

    #ifdef ENABLE_MEMCHECK_KEY
#ifdef L_PLATFORM_NDS
    if( lInput->GetInputState( LI_NDS_PAD1_BUTTON_SELECT ) )
    {
      lInput->SetInputState( LI_NDS_PAD1_BUTTON_SELECT, 0 );
#endif
#ifdef L_PLATFORM_WINDOWS
    if( lInput->GetInputState( LI_KEY_M ) )
    {
      lInput->SetInputState( LI_KEY_M, 0 );
#endif

      lLogFmt("\n\n");
      lLogBreak();
      lPlatform_MemoryLeakDump();

      lLogBreak();

#ifdef USE_DLMALLOC
      DumpDlmallocStats();
      char filename_buffer[128];
      static int memory_dump_filename_counter = 0;
      ::sprintf(filename_buffer, "memory%03d.xml", memory_dump_filename_counter);
      DumpDlmallocStatsToFile(filename_buffer);
      memory_dump_filename_counter++;
#endif

      lLogBreak();
      lLogFmt("\n\n");
    }
    #endif

  s_skipButtonPressed = false;

#ifdef ENABLE_SKIP_KEY
#ifdef L_PLATFORM_WIN32
  if( lInput->GetInputState( LI_KEY_S ) )
  {
#elif L_PLATFORM_NDS
  if( lInput->GetInputState( LI_NDS_PAD1_BUTTON_R ) )
  {
#else
  if( 0 )                               // never occur on other platforms
  {
#endif
    s_skipButtonPressed = true;
  }
#endif

  if (FDCommandLineOptions::IsAutoPlay())
	  s_skipButtonPressed = true;

  // Links button pressed
  SetLinkEnabledOverride(false);
#ifdef L_PLATFORM_WIN32
  if( lInput->GetInputState( LI_KEY_L ) )
  {
#elif L_PLATFORM_NDS
  if( lInput->GetInputState( LI_NDS_PAD1_BUTTON_L ) )
  {
#else
  if( 0 )                               // never occur on other platforms
  {
#endif
    SetLinkEnabledOverride(true);
  }

  s_nextScreen = s_prevScreen = false;

#ifdef L_PLATFORM_WINDOWS
  // Simulate changing screens without needing data - hopefully
  // Changed to make changing L and R for PSP emulation
  s_resetMenuItems = false;
  s_bUpdate = true;

  // Page up
  /*if( lInput->GetInputState( LI_KEY_PRIOR ) )
  {
    lInput->SetInputState( LI_KEY_PRIOR, 0 );
    s_nextScreen = true;
    s_bUpdate = false;
  }

  //Page down
  if( lInput->GetInputState( LI_KEY_NEXT ) )
  {
    lInput->SetInputState( LI_KEY_NEXT, 0 );
    s_prevScreen = true;
    s_bUpdate = false;
  }

  // Reset menu items
  if( lInput->GetInputState( LI_KEY_DELETE ) )
  {
    lInput->SetInputState( LI_KEY_DELETE, 0 );
    s_resetMenuItems = true;
    s_bUpdate = false;
  }*/
#endif


#if !defined(L_PLATFORM_NDS)
  lAudio::Update();
#else
  FDSound::Tick( deltaTime );
#endif

  // process text system
  lUIPoint mouse;
  bool bClick = false;

#ifdef PSP_MOUSE
#ifdef L_PLATFORM_PSP
  lUIWidget::MousePSP(true);
  lUIMouse::Tick( deltaTime );
#endif
#endif

  mouse.x = float(lUIMouse::GetPixelX());
  mouse.y = float(lUIMouse::GetPixelY());

  if( lInput->GetInputState( LI_MOUSE_BUTTON1 ) )
  {
    bClick = true;
  }
#ifdef L_PLATFORM_WII
  if( lInput->GetInputState( LI_WII_PAD1_BUTTON_A  ) )
  {
    bClick = true;
  }
#endif

#ifdef PSP_MOUSE
  #ifdef L_PLATFORM_PSP
  if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_CROSS  ) )
  {
    bClick = true;
  }
  #endif
#endif

  /*if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_CIRCLE ) || lInput->GetInputState( LI_KEY_BACK ) )
  {
    lInput->SetInputState( LI_PSP_PAD1_BUTTON_CIRCLE, 0 );
    lInput->SetInputState( LI_KEY_BACK, 0 );
    s_backButton = true;
  }
  
  if( s_userMessageBoxActive )
  {
    if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_CROSS ) )
    {
      lInput->SetInputState( LI_PSP_PAD1_BUTTON_CROSS, 0 );
      s_activatePressed = true;
    }

    if( lInput->GetInputState( LI_PAD1_LEFT ) )
    {
      lInput->SetInputState( LI_PAD1_LEFT, 0 );
      s_leftPressed = true;
    }

    if( lInput->GetInputState( LI_PAD1_RIGHT ) )
    {
      lInput->SetInputState( LI_PAD1_RIGHT, 0 );
      s_rightPressed = true;
    }
  }

  if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_L ) )
  {
    lInput->SetInputState( LI_PSP_PAD1_BUTTON_L, 0 );
    s_prevScreen = true;
  }

  if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_R ) )
  {
    lInput->SetInputState( LI_PSP_PAD1_BUTTON_R, 0 );
    s_nextScreen = true;
  }*/

  FDTextUpdate( mouse.x, mouse.y, bClick );

  // tick game
  s_game->Tick( deltaTime );

  // render
  s_client->Render();


  return true;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

extern void LoadScreenUpdate();
extern void ProgressScreenUpdate();

void ProcessingCallback( const char *txt = NULL )
{
  static float s_lastRenderTime = 0.0f;
  float newTime   = lPlatform_GetSeconds();
  float sinceLastRenderTime = (newTime - s_lastRenderTime);

//#ifdef L_PLATFORM_NDS
//  if( txt )
//    OS_Printf("Processing callback: %s : %.3fs\n", txt, sinceLastRenderTime);
//  else
//    OS_Printf("Processing callback: %.3fs\n", sinceLastRenderTime);
//#endif

  if (( sinceLastRenderTime >= 0.2f ) && (FDCommandLineOptions::IsAutoPlay() == false))
  {
#ifdef L_PLATFORM_NDS
    //OS_Printf("Doing main loop\n");
#endif

    ProgressScreenUpdate();
    LoadScreenUpdate();

    DoMainLoop();
    DoMainLoop();
    s_lastRenderTime = newTime;
  }
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif

#if defined(L_PLATFORM_WIN32)
static void LogToFile(const char *str)
{
  static bool bClearFile = true;
  lFILE_HANDLE hnd = lPlatform_FileOpen("log.txt", L_IO_ACCESS_WRITE, bClearFile ? L_IO_OPEN_CREATE_ALWAYS : L_IO_OPEN_EXISTING);
  if( hnd != lINVALID_FILE_FIND_HANDLE )
  {
    bClearFile = false;
    lPlatform_FileSeek( hnd, 0, L_IO_SEEK_END );
    lPlatform_FileWrite( hnd, str, strlen(str) );
    lPlatform_FileClose(hnd);
  }
}
#endif

int main(int argc, const char **argv)
{
//  s_halfFrameRate = true;

#ifdef _WIN32
#ifdef _DEBUG
  //int memoryDebugSettings = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  //memoryDebugSettings |= _CRTDBG_ALLOC_MEM_DF | /*_CRTDBG_CHECK_EVERY_16_DF | */ /* _CRTDBG_DELAY_FREE_MEM_DF | */ _CRTDBG_LEAK_CHECK_DF;
  //_CrtSetDbgFlag(memoryDebugSettings);
#endif // #ifdef _WIN32
#endif // #ifdef _DEBUG

  FDCommandLineOptions::ProcessCommandLine(argc, argv);

#ifdef DS_AUTOPLAY
  FDCommandLineOptions::ForceAutoPlay();
#endif

#ifndef L_PLATFORM_NDS
  lPlatform_Init();
#endif

#ifdef L_PLATFORM_WINDOWS
  // Set interactive assert handler
  lAssertSetHandler( &lPlatform_FailAssertInteractive );
#endif

#ifdef DISABLE_LOG
  lLogCHook( LogNull );
#else
#  if defined(L_PLATFORM_WIN32)
  //lLogCHook( LogToFile );
#  endif
#endif

  // do platform specific init
  PlatformSpecificInit();

  // do platform specific render init
  PlatformSpecificRenderInit();
  
  lClassRegistrar::RegisterClasses();

  // Init input
  lInputSystem::Select( new lInputSystem );

#if defined(L_PLATFORM_WINDOWS)
  lInput->Init( lRenderPC::GetHWnd() );
#else
  lInput->Init( NULL );
#endif

  lInput->Update();
  lInput->SetInputState( LI_NDS_PAD1_BUTTON_A, 0 );
  lInput->SetInputState( LI_MOUSE_BUTTON1, 0 );
  
  lEvent::Init();
  lDialogManager::Init(NULL,NULL,NULL);

  // Init resource manager
  FDResourceManager::Init();

#ifdef L_PLATFORM_NDS
  // set card eject callback
  CARD_SetPulledOutCallback( DSCartEjectCallback_CARD );
#endif

  // init sound
  FDSound::Init();

  // load locale

  lLogFmt("Loading locale\n");
  lText::Reset();

  lArray<int> languageIdList;
  languageIdList.Push( LTEXT_LANGUAGE_UK_ENGLISH );
  languageIdList.Push( LTEXT_LANGUAGE_SPANISH );
  languageIdList.Push( LTEXT_LANGUAGE_FRENCH );
  languageIdList.Push( LTEXT_LANGUAGE_ITALIAN );
  languageIdList.Push( LTEXT_LANGUAGE_GERMAN );
  lText::SetSupportedLanguages( languageIdList );
  lText::SetLanguage( LTEXT_LANGUAGE_UK_ENGLISH );

  lIOStreamSeekable * file = FDResourceManager::GetFile( "uk_english.txz" );

  if( file != NULL )
  {
    lText::Load( file );
    delete file;
  } else
  {
    lLogFmt("Couldn't load language info\n");
  }

  FDInput::Init();
  
  #ifdef ENABLE_NEW_LAYOUT
  FDConfig::EnableConvertedScreens(true);
  #ifdef L_PLATFORM_WINDOWS
  if(!FDConfig::PlatformNDS())
  {
    lSpriteManager::EmulateDS(false);
  }
  #endif
  #endif

  // Create game and client

  FDGame     * game   = FD_GAME_NEW FDGame();
  MainClient * client = FD_GAME_NEW MainClient();

  game->AddClient( client );
  game->GetWorld()->Init();
  //game->GetWorld()->BeginPlay();

  s_game   = game;
  s_client = client;

  // Main loop

  #if defined(L_PLATFORM_NDS)
  DSRender_StartDisplay();
  #endif

  #ifdef L_PLATFORM_PSP
  float lastStickMove = 0.0f;
  #endif

  //float oldTime = lPlatform_GetSeconds();
  s_oldTime = lPlatform_GetSeconds();

  if (FDCommandLineOptions::IsAutoPlay() == true)
  {
    FDTitle::ForceClose();
    FDSound::StopMusic();
	  game->GetWorld()->BeginPlay();

	  // Set up the player to use the specified team. This is an extremely compressed version of the
	  // standard game setup.
	  extern void StartAutoplayGame();
	  StartAutoplayGame();
  }

  while( 1 )
  {
    FDInput::Update();
    
    if( !DoMainLoop() )
    {
      // process returned false
      break;
    }
  }

  // cleanup

  game->GetWorld()->EndPlay();
  game->RemoveClient( client );
  delete client;
  delete game;

  // deinit

  lInput->Deinit();
  lInputSystemBase::Select( NULL );

  #ifndef L_PLATFORM_NDS
  lRender::Deinit();
  #endif

  // ELN - 6-Apr-2008: Currently disabled as there are things that are not getting released which are
  // referenced in global arrays. Destroying the mspaces causes problems because the memory is gone
  // when the destructors are called.
  // Destroy mspaces.
  //for (int i = 0; i < FDMPT_MAX; i++)
  //  destroy_mspace(g_FDMspaces[i]);

  return 0;
}


