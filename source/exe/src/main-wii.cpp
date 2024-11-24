//TODO
// Add Dialog::RenderHomeButtonIcon();

//Do:
//    lEvent::DisableResetSignal();
//       BufferSwap
//    lEvent::EnableResetSignal();


#include <revolution/vi.h>
#include <revolution/sc.h>
#include <revolution/tpl.h>
#include <revolution/mem/allocator.h>
#include <revolution/gx.h>
#include <lemon/render/lrdrawhelp.h>
#include <revolution/mem.h>

extern MEMAllocator  MyDemoAllocator2;

//#define SYSALLOC_USEMEM2

extern "C"
{
  void * __sys_alloc(__std(size_t) blocksize )
  {
    lLogFmt("Alloc: %d\n", blocksize);
    #ifdef SYSALLOC_USEMEM2
    return MEMAllocFromAllocator( &MyDemoAllocator2, blocksize );
    #else
    return lAlloc( blocksize );
    #endif
  }


  void __sys_free(void * block)
  {
    #ifdef SYSALLOC_USEMEM2
    MEMFreeToAllocator( &MyDemoAllocator2, block );
    #else
    lFree( block );
    #endif
  }
};


uint8 s_wiiAspectRatio;
uint8 s_wiiLanguage;
extern bool g_enableCKLemonChanges;
extern bool g_bEnableControllerCheck;


// for main
/*  g_bEnableControllerCheck = false;
  lEvent::Init();
  lDialogManager::Init(NULL,NULL,NULL);
  CKDialog::Init();

  // It is valid to disable the button without the home button prohibition icon during strap screens
  //lEvent::DisableHomeButton( true );
  CKDialog::SetHomeButtonEnabled(false);


  DoStrapScreen();
  CKDialog::SetHomeButtonEnabled(true);
  //lEvent::DisableHomeButton( false );
  g_bEnableControllerCheck = true;

  DoEventLoop();

  HomeButtonPreload();*/


void PlatformSpecificInit()
{
}

void PlatformSpecificRenderInit()
{
  int tvfmt = VIGetTvFormat();
  switch( tvfmt )
  {
    case VI_NTSC:    lLogFmt("NTSC\n"); break;
    case VI_PAL:     lLogFmt("PAL\n"); break;
    case VI_EURGB60: lLogFmt("EU60\n"); break;
    case VI_MPAL:    lLogFmt("MPAL\n"); break;
  }

  g_enableCKLemonChanges = true;

  s_wiiAspectRatio = SCGetAspectRatio();
  if( s_wiiAspectRatio == SC_ASPECT_RATIO_16x9 )
  {
    FDConfig::EnableWideScreen(true);
  }

  lrSurfaceDesc surf;
  surf.xRes = FDConfig::IsWideScreenEnabled() ? 720 : 640;
  surf.yRes = 480;
  surf.colourBits = 24;
  surf.depthBits = 16;
  lRender::SurfaceCreate( surf );

  if( SCGetEuRgb60Mode() == SC_EURGB60_MODE_ON )
  {
    lSpriteManager::SetHackScaleNTSC(true);
  }

/*  g_bEnableControllerCheck = false;
  lEvent::Init();
  lDialogManager::Init(NULL,NULL,NULL);
  CKDialog::Init();

  // It is valid to disable the button without the home button prohibition icon during strap screens
  //lEvent::DisableHomeButton( true );
  CKDialog::SetHomeButtonEnabled(false);


  DoStrapScreen();
  CKDialog::SetHomeButtonEnabled(true);
  //lEvent::DisableHomeButton( false );
  g_bEnableControllerCheck = true;

  DoEventLoop();

  HomeButtonPreload();*/
}


/*
//Strap screen

extern bool g_enableCKLemonChanges;
extern bool g_bEnableControllerCheck;
uint8 s_wiiAspectRatio;
uint8 s_wiiLanguage;
extern bool g_bResetFlag;
void HomeButtonPreload();


void DoStrapScreen()
{
  //Look up correct strap file based on resolution + locale
  char sprite[64];
  char *prefixsize;

  //NOTE: two resolutions to choose fro, pick right one
  if( s_wiiAspectRatio == SC_ASPECT_RATIO_16x9 )
    prefixsize = "strapA_16_9_853x480_";
  else
    prefixsize = "strapA_640x480_";

  s_wiiLanguage = SCGetLanguage();

  lString bank;

  switch( s_wiiLanguage )
  {
    case SC_LANG_GERMAN       : sprintf( sprite, "%sGerman", prefixsize); break;
    case SC_LANG_FRENCH       : sprintf( sprite, "%sFrench", prefixsize); break;
    case SC_LANG_SPANISH      : sprintf( sprite, "%sSpanish_EU", prefixsize); break;
    case SC_LANG_ITALIAN      : sprintf( sprite, "%sItalian", prefixsize); break;
    case SC_LANG_DUTCH        : sprintf( sprite, "%sDutch", prefixsize); break;
    default:
    case SC_LANG_ENGLISH      : sprintf( sprite, "%sEnglish", prefixsize); break;
  }
  bank = lString("strap");

  lLogFmt("WII LANG %d %s\n",s_wiiLanguage,sprite);

  lSharedPtr<lSprite> strapSprite;
  bool bSpriteLoaded = false;

  float oldTime = lPlatform_GetSeconds();
  float fade = 0.0f;
  float fadeLength = 0.5f;
  float timePassed = 0.0f;
  bool  bFadingOut = false;

  float fCheckCount = 0;
  float fStrapDelay = 0;
  float fStrapDelayTime = 1.5f;

  float fCheckDelay = 0;
  float fCheckDelayTime = 1.0f;

  float fHomeButtonDelay = 1.5f;

  while(lPlatform_Process())
  {
    if( g_bResetFlag )
    {
      lLogFmt("ALTERNATIVE RESET\n");
      CKDialog::Reset();
    }


    float timeNow = lPlatform_GetSeconds();
    float delta = timeNow - oldTime;
    oldTime = timeNow;

    if( delta > (1.0f / 30.0f) ) delta = (1.0f / 30.0f);

    fHomeButtonDelay -= delta;

    if( fHomeButtonDelay > 0.0f )
    {
      // don't display home button icon for first 1.5 seconds
      CKDialog::ClearHomeButton();
    }

    lRender::BufferClear( L_ARGB(255,0,0,0) );

    lInput->Update();
    lAudio::Update();

    WPADDisconnect(1);
    WPADDisconnect(2);
    WPADDisconnect(3);

    if( fCheckDelay < fCheckDelayTime ) fCheckDelay+=delta;
    else
    {
      fCheckCount+=delta;
      if( fCheckCount >= 2.0f )
      {
        lEvent::WiiDiscCheck("pause.lsb");//check with any existing file
        fCheckCount = 0;
      }

      lEvent::Check();
    }

    CKDialog::Tick(delta);
    if( CKDialog::IsOpen() || CKDialog::IsRenderBlack() )
    {
      if( !CKDialog::IsRenderBlack() ) CKDialog::Render();
      else lLogFmt("BLACK\n");
      continue;
    }

    //lrDrawHelp::DrawBitmap( 0, strapScreen, 0, 0, 1, 1 );
    if( fStrapDelay < fStrapDelayTime ) fStrapDelay += delta;
    if( !bSpriteLoaded && fStrapDelay > fStrapDelayTime )
    {
      lLogFmt("Load strap sprite...\n");
      strapSprite = CKResourceManager::GetSprite(bank.CStr(),sprite);
      strapSprite->SetPosition(0,0);
      strapSprite->SetWidth(lSpriteManager::GetDisplayWidth());
      strapSprite->SetHeight(lSpriteManager::GetDisplayHeight());
      bSpriteLoaded = true;
    }
    else if( bSpriteLoaded )
    {
      strapSprite->Render();
      timePassed += delta;

      float a = 0.0f;

      if( fade < fadeLength )
      {
        fade += delta;
        if( fade > fadeLength )
        {
          fade = fadeLength;
        }

        a = (1.0f/fadeLength) * (fadeLength-fade);
      }

      if( fade > fadeLength )
      {
        fade += delta;

        a = (1.0f/fadeLength) * (fade-fadeLength);
      }

      if( a > 1.0f ) a = 1.0f;
      if( a < 0.0f ) a = 0.0f;

      lrDrawHelp::DrawRect( 0, 0.0f, 0.0f, 1.0f, 1.0f, L_ARGB( a*255.0f, 0,0,0 ) );
    }

    CKDialog::RenderHomeButtonIcon();

    lRender::BufferSwap();

    if( timePassed > 1.5f )
    {
      if( timePassed > 20.5f )
      {
        if( !bFadingOut )
        {
          bFadingOut = true;
          fade += delta; // kick the fade above 1.0 to start it fading out.
        }
      } else
      {
        if( lInput->GetInputState(LI_WII_PAD1_BUTTON_A) )
        {
          if( !bFadingOut )
          {
            bFadingOut = true;
            fade += delta; // kick the fade above 1.0 to start it fading out.
          }
        }
      }
    }

    if( fade >= (fadeLength*2.0f) )
    {
      if( !CKDialog::IsHomeButtonDisabledShown() )
      {
        break;
      }
    }

  }

  CKDialog::ClearHomeButton();
}


void DoEventLoop()
{
  float oldTime = lPlatform_GetSeconds();
  float fCheckCount = 2.0f;

  while(lPlatform_Process())
  {
    float timeNow = lPlatform_GetSeconds();
    float delta = timeNow - oldTime;
    oldTime = timeNow;

    lRender::BufferClear( L_ARGB(255,0,0,0) );

    lInput->Update();
    lAudio::Update();

    WPADDisconnect(1);
    WPADDisconnect(2);
    WPADDisconnect(3);

    lEvent::Check();

    fCheckCount+=delta;
    if( fCheckCount >= 2.0f )
    {
      lEvent::WiiDiscCheck("wii/uk_english.txz");//check with any existing file
      fCheckCount = 0;
    }

    if( FDDialog::IsOpen() )
    {
      //todo: prevent controller disconnect message
      //from flashing up before strap sprite is rendered
      //for first time
      FDDialog::Tick(delta);
      FDDialog::Render();
      continue;
    } else
    {
      lLogFmt("No dialog - quitting event loop\n");
      lRender::BufferClear( L_ARGB(255,0,0,0) );
      lRender::BufferSwap();
      break;
    }
  }

}

    #ifdef L_PLATFORM_WII
    if( g_bResetFlag )
    {
      lLogFmt("ALTERNATIVE RESET\n");
      CKDialog::Reset();
    }
    #endif

 */

