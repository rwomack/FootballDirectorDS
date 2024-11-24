enum
{
  ERROR_NONE,
  ERROR_ACCESS_CORRUPT,
  ERROR_ACCESS_FAIL,
};

#include <lemon/object/lsharedptr.h>
#include <framework/lsprite.h>
#include <framework/lspritebank.h>
#include <framework/lfont.h>
#include <lemon/input/linput.h>
#include <fdresourcemanager.h>
#include <framework/dsrender.h>

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PZError
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static lSharedPtr<lSprite> s_mainSprite;
static bool  s_init = false;
static int   s_type = ERROR_NONE;
static float s_ignoreInput = 1.0f;

static bool s_bDebounce = false;
static bool s_bFirstTick = false;

static void ErrorInit( int errorType )
{
  lLogFmt("Error::Init %d (currently %d)\n", errorType, s_type);

  if( errorType != ERROR_NONE )
  {
    if( s_type != ERROR_NONE )
    {
      lLogFmt("Error: Type already %d - not overwriting\n", s_type);
      return;
    }
  }

  s_init = true;
  s_type = errorType;
  s_ignoreInput = 0.5f;
  s_bFirstTick = true;

  if( errorType == ERROR_NONE )
  {
    return;
  }

  //load sprites
  lSharedPtr<lSpriteBank> sb = FDResourceManager::GetSpriteBank("error" );

  if( errorType == ERROR_ACCESS_CORRUPT )
  {
    s_mainSprite = lSharedPtr<lSprite>(lSprite::Create( sb, "Nintendo_Corrupt_Eng" ));
  } else
  {
    s_mainSprite = lSharedPtr<lSprite>(lSprite::Create( sb, "Nintendo_AccessFail_Eng" ));
  }

  s_mainSprite->SetNormalCoordinates(true);
  s_mainSprite->SetTransparent(false);
  s_mainSprite->SetWidth(1.0f);
  s_mainSprite->SetHeight(1.0f);
}

static bool ErrorTick(float delta)
{
  if( !s_init )
  {
    lLogFmt("ErrorTick not initialised\n");
    return true;
  }

  if( s_type == ERROR_ACCESS_CORRUPT )
  {
    bool bPressed = false;

    if( lInput->GetMouseButton( 0 ) )
    {
      bPressed = true;
    }

    if( s_bDebounce )
    {
      if( bPressed )
      {
        //lLogFmt("Cancel press as we haven't let go yet\n");
        bPressed = false;
      } else
      {
        //lLogFmt("User has let go, next press will be valid\n");
        s_bDebounce = false;
      }
    }

    if( s_ignoreInput > 0.0f )
    {
      // ignore input
      if( bPressed )
      {
        bPressed = false;
        s_bDebounce = true;
      }
    }

    s_ignoreInput -= delta;
    if( s_ignoreInput <= 0.0f )
    {
      if( bPressed )
      {
        //lLogFmt(" Mouse pressed - continue\n");
        return true;
      }
    }

    // signal that we dont want to continue yet
    return false;
  }

  // signal that processing is done and we can halt
  return false;
}

static void ErrorRender()
{
  if( s_mainSprite != NULL )
  {
    s_mainSprite->Render();
  } else
  {
    lLogFmt("No sprite\n");
  }
}

static void ErrorHandle( int errorType )
{
  ErrorInit( errorType );

  lInput->Update();

  while( !ErrorTick( 1.0f / 30.0f ) )
  {
#ifdef L_PLATFORM_NDS
  if ( CARD_IsPulledOut() ) // check if card is pulled out
  {
    lLogFmt("CART EJECT HANDLE\n");

    DSRender_StartFrame();
    FDResourceManager::GetDefaultFont()->PrintCenterX((192/2)-22,"The DS Card has been removed.\nPlease turn power OFF and reinsert\nthe DS Card before turning\npower back ON.");
    DSRender_SwapScreens();

    DSRender_StartFrame();
    FDResourceManager::GetDefaultFont()->PrintCenterX((192/2)-22,"The DS Card has been removed.\nPlease turn power OFF and reinsert\nthe DS Card before turning\npower back ON.");
    DSRender_SwapScreens();

    CARD_TerminateForPulledOut(); // termination
    // program halted. never reached here.
  }
#endif

#ifdef L_PLATFORM_NDS
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
#endif

    lInput->Update();

    DSRender_StartFrame();
    ErrorRender();
    DSRender_SwapScreens();

    DSRender_StartFrame();
    ErrorRender();
    DSRender_SwapScreens();
  }

  ErrorInit( ERROR_NONE );
}
