#include <kernel.h>
#include <mediaman.h>
#include <powersvc.h>
#include <fatms.h>

#define PSP_USEHOST

SCE_MODULE_INFO( Football, 0, 1, 1 );

int sce_newlib_heap_kb_size = 17408;    // 20000k heap
int sce_user_main_thread_attribute = SCE_KERNEL_TH_USE_VFPU;

static bool g_PSPDiscEject = false;
static bool g_PSPDiscChanged = false;
static void HandleDiscEject();

static int MemoryStickCallback(int count, int arg, void *param);

typedef void  (*PlatformFileIOCallback)( bool bError );
LPLATFORM_API void lPlatformPSP_Temp_SetIOCallback( PlatformFileIOCallback callback );
void  diskErrorCallback( bool bError );

void PSPFooterTick( float delta );

#include <umderror.h>
#include <mediaman.h>
#include <kernel.h>
#include <utility/utility_common.h>
#include <utility/utility_sysparam.h>
#include <utility/utility_module.h>

#ifdef PSP_USEHOST
#define DEVKIT_TOP        "host0:"
#else
#define DEVKIT_TOP        "disc0:/PSP_GAME/USRDIR/"
#endif

static SceInt32 umd_func( SceInt32 count, SceInt32 arg1, SceVoid *arg2 );
static int ExitFunction(int count, int arg_impose, void *arg_self);
static int PowerCallback(int count, int arg, void *common);
static int MemoryStickCallback(int count, int arg, void *param);

// in main
void PlatformSpecificInit()
{
  // set up callbacks
  SceUID uid = sceKernelCreateCallback("exit", ExitFunction, NULL);

  if (uid < 0)
  {
    printf ("ERROR: sceKernelCreateCallback(%x) EXIT\n", uid);
    return;//
  }

	int ret = sceKernelRegisterExitCallback(uid);
	if( ret != 0 )
	{
  	printf("ERROR: unable to register EXIT callback %d\n",ret);
	}

  SceUID umd_cb = sceKernelCreateCallback("INSERT_EJECT_UMD", umd_func, NULL);

  if (umd_cb < 0)
  {
    printf ("ERROR: sceKernelCreateCallback(%x) UMD\n", umd_cb);
    return;//
  }

  ret = sceUmdRegisterUMDCallBack(umd_cb);
  if( ret != 0 )
	{
  	printf("ERROR: unable to register EXIT callback %d\n",ret);
	}

	SceUID pwr = sceKernelCreateCallback("power", PowerCallback, NULL);

	if( pwr < 0 )
	{
  	printf("ERROR: sceKernelCreateCallback(%x) PWR\n",pwr);
    return;//
	}

	ret = scePowerRegisterCallback( SCE_POWER_CALLBACKSLOT_AUTO, pwr );
	if( ret < 0 )
	{
  	printf("ERROR: unable to create power callback\n");
	}

	SceUID mem = sceKernelCreateCallback("ms", MemoryStickCallback, NULL);

	if( mem < 0 )
	{
    lLogFmt("ERROR: sceKernelCreateCallback(%x) MS\n",mem);
    return;//
	}

	ret = sceFatmsRegisterNotifyCallback( mem );
	if( ret < 0 )
	{
    lLogFmt("ERROR: unable to create ms callback\n");
	}

  // set default device
  #ifdef PSP_USEHOST
  lPlatform_PSP_FileSetDefaultDevice("host0:");
  #else
  lPlatform_PSP_FileSetDefaultDevice("disc0:/PSP_GAME/USRDIR");

  sceUmdActivate( SCE_UMD_MODE_POWERON, SCE_UMD_ALIAS_NAME );
  if (ret < 0)
  {
    lLogFmt ("fatal error : sceUmdActivate() 0x%x\n", ret);
    return;//
  }
  else
  {
    lLogFmt("UMD ACTIVE\n");
  }
  #endif

  //hangs on black screen if disc is ejected
  //sceUmdWaitDriveStat(SCE_UMD_READABLE);
}

void PlatformSpecificRenderInit()
{
  lRender::Init();

  lrSurfaceDesc surf;
  surf.xRes = 480;
  surf.yRes = 272;
  surf.colourBits = 32;
  surf.depthBits = 16;
  lRender::SurfaceCreate( surf );
}



static void HandleDiscEject()
{
  /*
  static int s_antiFlicker = 0;
  static int s_lastStatus = -1;

  float oldT = lPlatform_GetSeconds();
  //lEvent::ShowPSPDiscEjectMessage();
  PSPDisableVideoRender( true );

  lRender::BufferClear( L_ARGB(255,0,0,0) );
  lRenderPSP::BufferSwapEndFrame();
  lEvent::CheckPostRender();
  lRenderPSP::BufferSwapStartNextFrame();

  lLogFmt("HandleDiscEject - last %d anti %d\n", s_lastStatus, s_antiFlicker);

  while( lPlatform_Process() )
  {
    float newTime   = lPlatform_GetSeconds();
    float deltaTime = (newTime - oldT);
    oldT = newTime;

    lRender::BufferClear( L_ARGB(255,0,0,0) );

    lInput->Update();
    //check
    int status = sceUmdGetDriveStat();

    //if( !g_PSPDiscChanged )
    {
      int newStatus;

      // no disk change
      if( g_PSPDiscEject )
      {
        //lLogFmt("DiscEjected - show disk eject message\n");
        newStatus = 0;
      } else
      {
        if( (status & SCE_UMD_READY) )
        {
          //lLogFmt("Disc ready - hide all reading\n");
          newStatus = 1;
        } else
        {
          //lLogFmt("Disc not ready - show reading\n");
          newStatus = 2;
        }
      }

      s_antiFlicker--;

      if( newStatus != s_lastStatus )
      {
        if( s_antiFlicker < 0 )
        {
          // 20 frames before another change
          s_antiFlicker = 20;
          s_lastStatus = newStatus;

          lLogFmt("last: %d new: %d\n", s_lastStatus, newStatus);

          switch( newStatus )
          {
            case 0:
            {
              //lLogFmt("DiscEjected - show disk eject message\n");
              lEvent::HidePSPDiscReadingMessage();
              lEvent::ShowPSPDiscEjectMessage();
              break;
            }

            case 1:
            {
              lEvent::HidePSPDiscEjectMessage();
              lEvent::HidePSPDiscReadingMessage();
              break;
            }

            case 2:
            {
              lEvent::HidePSPDiscEjectMessage();
              lEvent::ShowPSPDiscReadingMessage();
              break;
            }
          }
        }
      }
    }

    FDDialog::Tick(deltaTime);
    if( FDDialog::IsOpen() )
    {
      if( !lStorageManager::IsDeleteListOpen() )
      {
        FDDialog::Render();
      }

      if( lStorageManager::IsDeleteListOpen() )
      {
        lRender::BufferClear( L_ARGB(255,0,0,0) );
        lRenderPSP::BufferSwapEndFrame();
        lEvent::CheckPostRender();
        lRenderPSP::BufferSwapStartNextFrame();
      }
      continue;
    }
    break;
  }

  lLogFmt("HandleDiscEject done\n");

  lRender::BufferClear( L_ARGB(255,0,0,0) );
  lRenderPSP::BufferSwapEndFrame();
  lEvent::CheckPostRender();
  lRenderPSP::BufferSwapStartNextFrame();

  lRender::BufferClear( L_ARGB(255,0,0,0) );
  lRenderPSP::BufferSwapEndFrame();
  lEvent::CheckPostRender();
  lRenderPSP::BufferSwapStartNextFrame();

  PSPDisableVideoRender( false );
  g_PSPDiscEject = false;

  s_antiFlicker = 0;*/
}

// callback from IO function
void  diskErrorCallback( bool bError )
{
  int status = sceUmdGetDriveStat();
  if(!(status & SCE_UMD_MEDIA_IN))
  {
    g_PSPDiscEject = true;
    //TODOPSPDisableVideoRender( true );
  }

  HandleDiscEject();
}

int MemoryStickCallback(int count, int arg, void* param)
{
  if( arg & SCE_FATMS_NOTIFY_ASSIGNED )
  {
    //inserted
    lLogFmt("MEMORY CARD INSERTED\n");
    //TODOlStorageManager::CheckSaveID();
  }
  else if( arg & SCE_FATMS_NOTIFY_UNASSIGNED )
  {
    //ejected
    lLogFmt("MEMORY CARD EJECTED\n");
  }

  return 0;
}

int PowerCallback(int count, int arg, void* common)
{
  lLogFmt("\n\n-----\nPower callback\n-----\n");

  bool bSuspend = false;

  if( arg & SCE_POWER_CALLBACKARG_SUSPENDING )
  {
    lLogFmt("SUSPENDING\n");
    bSuspend = true;
  } else
  if( arg & SCE_POWER_CALLBACKARG_STANDINGBY )
  {
    lLogFmt("STANDING BY\n");
    bSuspend = true;
  }
  else if( arg & SCE_POWER_CALLBACKARG_RESUMING )
  {
    lLogFmt("RESUMING\n");
    g_PSPDiscEject = false;
  }
  else if( arg & SCE_POWER_CALLBACKARG_RESUME_COMP )
  {
    lLogFmt("RESUME COMPLETE\n");
    g_PSPDiscEject = false;
  }

  if( bSuspend )
  {
    //TODOPSPDisableVideoRender( true );
  }

  return 0;
}

void CancelPSMFOpen();

static int ExitFunction(int count, int arg_impose, void *arg_self)
{
  lLogFmt("EXITGAME\n");
  //while( lEvent::GetPSPUtilityMode() == LEVENT_PSP_UTILITY_TYPE_SAVELOAD )
  //{
  //  lLogFmt("WAITING FOR SAVE\n");
  //  lEvent::Check();
  //}

  //CancelPSMFOpen();

  sceKernelExitGame();
  return 0;
}

#define MEDIA_STAT_MASK   (SCE_UMD_MEDIA_OUT | SCE_UMD_MEDIA_IN | SCE_UMD_MEDIA_CHG | SCE_UMD_NOT_READY | SCE_UMD_READY | SCE_UMD_READABLE)

static int umd_func(SceInt32 count, SceInt32 arg, SceVoid *param)
{
  lLogFmt("UMD CB\n");

	SceInt32 tmparg;

	(void)count;

	tmparg = arg & MEDIA_STAT_MASK;
  if (arg ^ tmparg)
  {
		printf("Included unknown media stat(%x) ??????????????\n\n", arg);

  } else
  {
    while (tmparg != 0x00)
    {
      if (tmparg & SCE_UMD_MEDIA_OUT)
      {
				printf("== Ejected (%x) !!\n", tmparg);
				tmparg ^= SCE_UMD_MEDIA_OUT;

//        CancelPSMFOpen();

				g_PSPDiscEject = true;
				g_PSPDiscChanged = false;
			}
      if (tmparg & SCE_UMD_MEDIA_IN)
      {
				printf("== Inserted (%x) !!\n", tmparg);
				tmparg ^= SCE_UMD_MEDIA_IN;
        g_PSPDiscEject = false;
			}
      if (tmparg & SCE_UMD_NOT_READY)
      {
				printf("== Device NOT ready(%x) !!!!!!!!!!!!\n", tmparg);

				int status = sceUmdGetDriveStat();
				if( status & SCE_UMD_MEDIA_CHG )
				{
  				lLogFmt("MEDIA CHANGED!!\n");
  				g_PSPDiscChanged = true;
				}

				tmparg ^= SCE_UMD_NOT_READY;
			}
      if (tmparg & SCE_UMD_READY)
      {
				printf("== Device ready (%x) !!!!!!!!!!!!\n", tmparg);
				tmparg ^= SCE_UMD_READY;
        g_PSPDiscEject = false;
			}
      if (tmparg & SCE_UMD_MEDIA_CHG)
      {
				printf("== Changed (another media is inserted) (%x) !!\n", tmparg);
				tmparg ^= SCE_UMD_MEDIA_CHG;
				g_PSPDiscChanged = true;
        g_PSPDiscEject = false;
			}
      if (tmparg & SCE_UMD_READABLE)
      {
				printf("== Readable (%x) !!\n", tmparg);
				tmparg ^= SCE_UMD_READABLE;
        g_PSPDiscEject = false;
			}
		}
	}

	return 0;
}

/*

void DoSplashScreens(void)
{
  #ifdef ENABLE_SPLASHSCREENS

  if(!CKConfig::IsDebugMenuEnabled())
  {
    lSharedPtr<lSpriteBank> sb = CKResourceManager::GetSpriteBank(CKConfig::IsWideScreenEnabled() ? "splash_wide" : "splash");

    if( CKConfig::PlatformNDS() )
    {
      #if defined(L_PLATFORM_NDS) || defined(L_PLATFORM_WINDOWS)
      lSplashScreen::AddImage( sb, "midas", "gamerholix", 4.0f, 1.0f, 1.0f, false );
      #endif
    }
    else
    {
      #if !defined L_PLATFORM_NDS
      lSplashScreen::AddImage( sb, "midas", 3.0f, 0.5f, 0.5f, false );
      lSplashScreen::AddImage( sb, "gamerholix", 3.0f, 0.5f, 0.5f, true );
      #endif
    }

    lSplashScreen::Start();
  }


  float oldTime = lPlatform_GetSeconds();
  bool bLastHighlighted = false;

  #ifdef L_PLATFORM_WII
  float fCheckCount = 0;
  #endif

  int selected = -1;

  bool bDone = lPlatform_Process();
  while( bDone )
  {
    bDone = lPlatform_Process();
    s_bSystemQuit = !bDone;

    float newTime   = lPlatform_GetSeconds();
    float deltaTime = (newTime - oldTime);
    oldTime = newTime;

    #ifdef L_PLATFORM_WII
    if( g_bResetFlag )
    {
      lLogFmt("ALTERNATIVE RESET\n");
      CKDialog::Reset();
    }
    #endif

    #ifdef L_PLATFORM_WII
    WPADDisconnect(1);
    WPADDisconnect(2);
    WPADDisconnect(3);
    fCheckCount+=deltaTime;
    if( fCheckCount >= 2.0f )
    {
      lEvent::WiiDiscCheck("pause.lsb");//check with any existing file
      fCheckCount = 0;
    }
    lEvent::Check();
    CKDialog::Tick(deltaTime);
    if( CKDialog::IsOpen() || CKDialog::IsRenderBlack() )
    {
      CKDialog::Tick(deltaTime);
      if( !CKDialog::IsRenderBlack() ) CKDialog::Render();
      //CKResourceManager::GetDefaultFont()->Print(0,0,lStorageManager::GetLog());
      continue;
    }
    CKDialog::RenderHomeButtonIcon();
    #endif


    #if defined(L_PLATFORM_NDS)
    ProcessNDSLidFold();
    #endif

    if( ! lRender::IsReady() )
    {
      continue;
    }



    lInput->Update();
    lAudio::Update();
    lAudioStream::Tick();

    CKSound::TickSounds(deltaTime);
    CKMusic::Tick(deltaTime);
    CKResourceManager::Tick( deltaTime );

    if( !lSplashScreen::IsFinished() )
    {
      lSplashScreen::Tick(deltaTime);
    }
    else
    {
      return;
    }

    #if !defined(L_PLATFORM_NDS)
    lRender::BufferClear( L_ARGB(255,0,0,0) );
    #else
    DSRender_StartFrame();
    #endif

    lSplashScreen::Render();


    #ifdef L_PLATFORM_WII
    CKDialog::RenderHomeButtonIcon();
    lEvent::DisableResetSignal();
    #endif

    lSpriteManager::BufferSwap();

    #ifdef L_PLATFORM_WII
    lEvent::EnableResetSignal();
    #endif

    HandleCardPullOut();
  }

  lSplashScreen::Clear();
  #endif
}




  #ifdef L_PLATFORM_PSP
  lEvent::Init();
  lDialogManager::Init(NULL,NULL,NULL);
  FDDialog::Init();

  #ifdef L_PLATFORM_PSP
  float lastStickMove = 0.0f;
  #endif

  // clear cross if already pressed
  lInput->Update();
  lInput->SetInputState(LI_PSP_PAD1_BUTTON_CROSS,0);


  // show warning
  lStorageManager::ShowAutosaveWarning();

  float oldT = lPlatform_GetSeconds();
  while( lPlatform_Process() )
  {
    float newTime   = lPlatform_GetSeconds();
    float deltaTime = (newTime - oldT);
    oldT = newTime;
    if( g_PSPDiscEject || g_PSPDiscChanged )
		{
      HandleDiscEject();
	  }

    lInput->Update();
    if( !lInput->GetInputState( LI_PSP_PAD1_BUTTON_INTERCEPTED ) )
    {
      if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) < -0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: L\n"); lInput->SetInputState( LI_PAD1_DPAD_LEFT, 1 );  lInput->SetInputState( LI_PAD1_LEFT, 1 );  lastStickMove = 0.0f;  }
      if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) >  0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: R\n"); lInput->SetInputState( LI_PAD1_DPAD_RIGHT, 1 ); lInput->SetInputState( LI_PAD1_RIGHT, 1 ); lastStickMove = 0.0f; }
      if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) < -0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: U\n"); lInput->SetInputState( LI_PAD1_DPAD_UP, 1 );    lInput->SetInputState( LI_PAD1_UP, 1 );    lastStickMove = 0.0f;  }
      if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) >  0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: D\n"); lInput->SetInputState( LI_PAD1_DPAD_DOWN, 1 );  lInput->SetInputState( LI_PAD1_DOWN, 1 );  lastStickMove = 0.0f;  }
    }

    lRender::BufferClear( L_ARGB(255,0,0,0) );
    lEvent::Check();
    if( lEvent::GetPSPUtilityMode() != LEVENT_PSP_UTILITY_TYPE_SAVELOAD && !FDDialog::IsOpen() )
    {
      break;
    }

    FDDialog::Tick(deltaTime);
    if( FDDialog::IsOpen() )
    {
      FDDialog::Render();
      continue;
    }
    lRender::BufferClear( L_ARGB(255,0,0,0) );
    #ifdef L_PLATFORM_PSP
    lRenderPSP::BufferSwapEndFrame();
    #endif
    lEvent::CheckPostRender();
    #ifdef L_PLATFORM_PSP
    lRenderPSP::BufferSwapStartNextFrame();
    #else
    lRender::BufferSwap();
    #endif
  }
  lRender::BufferClear( L_ARGB(255,0,0,0) );
  lRender::BufferSwap();

  // do a load now
  FDProfileManager::Init();
  oldT = lPlatform_GetSeconds();
  while( lPlatform_Process() )
  {
    float newTime   = lPlatform_GetSeconds();
    float deltaTime = (newTime - oldT);
    oldT = newTime;

    if( g_PSPDiscEject || g_PSPDiscChanged )
		{
      HandleDiscEject();
	  }

    lInput->Update();
    if( !lInput->GetInputState( LI_PSP_PAD1_BUTTON_INTERCEPTED ) )
    {
      if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) < -0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: L\n"); lInput->SetInputState( LI_PAD1_DPAD_LEFT, 1 );  lInput->SetInputState( LI_PAD1_LEFT, 1 );  lastStickMove = 0.0f;  }
      if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) >  0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: R\n"); lInput->SetInputState( LI_PAD1_DPAD_RIGHT, 1 ); lInput->SetInputState( LI_PAD1_RIGHT, 1 ); lastStickMove = 0.0f; }
      if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) < -0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: U\n"); lInput->SetInputState( LI_PAD1_DPAD_UP, 1 );    lInput->SetInputState( LI_PAD1_UP, 1 );    lastStickMove = 0.0f;  }
      if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) >  0.4f && lastStickMove > 0.2f ) { lLogFmt("Analog: D\n"); lInput->SetInputState( LI_PAD1_DPAD_DOWN, 1 );  lInput->SetInputState( LI_PAD1_DOWN, 1 );  lastStickMove = 0.0f;  }
    }

    lRender::BufferClear( L_ARGB(255,0,0,0) );
    lEvent::Check();
    if( lEvent::GetPSPUtilityMode() != LEVENT_PSP_UTILITY_TYPE_SAVELOAD && !FDDialog::IsOpen() )
    {
      break;
    }

    FDDialog::Tick(deltaTime);
    if( FDDialog::IsOpen() )
    {
      FDDialog::Render();
      continue;
    }
    lRender::BufferClear( L_ARGB(255,0,0,0) );
    #ifdef L_PLATFORM_PSP
    lRenderPSP::BufferSwapEndFrame();
    #endif
    lEvent::CheckPostRender();
    #ifdef L_PLATFORM_PSP
    lRenderPSP::BufferSwapStartNextFrame();
    #else
    lRender::BufferSwap();
    #endif
  }
  lRender::BufferClear( L_ARGB(255,0,0,0) );
  lRender::BufferSwap();
  #endif


    #ifdef L_PLATFORM_PSP

    PSPFooterTick( deltaTime );

    //lock into a loop here and display a message
    //asking for disc to be re-instered
		//detect disc insert and exit the loop
		if( g_PSPDiscEject || g_PSPDiscChanged )
		{
      HandleDiscEject();
	  }

	  lEvent::Check();
    FDDialog::Tick(deltaTime);
    if( FDDialog::IsOpen() )
    {
      FDDialog::Render();
      continue;
    }

    #endif

*/


