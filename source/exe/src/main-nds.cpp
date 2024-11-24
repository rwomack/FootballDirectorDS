int main(int argc, const char **argv);

void PreSleepCallback(void* arg)
{
  FDSound::PauseAll();

  //Callback before DS enters sleep mode
  lLogFmt("NDS: Lid closed, sleeping...\n");
}

void PostSleepCallback(void* arg)
{
  //Callback after DS exits sleep mode
  lLogFmt("NDS: Lid opened, resuming\n");

  //hold for >15ms to allow sound system to recover
  uint32 firstTick = lPlatform_TickCount();
  while(1)
  {
    uint32 tick = lPlatform_TickCount();
    uint32 time = tick - firstTick;
    if( time > 200 ) break;
  }

  FDSound::ResumeAll();
}

static BOOL DSCartEjectCallback_CARD( void )
{
  return FALSE;
}

void NitroStartUp(void)
{
  lPlatform_Init();
}

void NitroMain()
{
  PMSleepCallbackInfo preSleepCallbackInfo;
  PMSleepCallbackInfo postSleepCallbackInfo;

  PM_SetSleepCallbackInfo(&preSleepCallbackInfo, PreSleepCallback, NULL);
  PM_SetSleepCallbackInfo(&postSleepCallbackInfo, PostSleepCallback, NULL);
  PM_AppendPreSleepCallback(&preSleepCallbackInfo);
  PM_AppendPostSleepCallback(&postSleepCallbackInfo);

  main(0,NULL);
}

void DSCardEjectHandle()
{
  lLogFmt("CART EJECT HANDLE\n");

  // TODO: Locale specific

  DSRender_StartFrame();
  FDResourceManager::GetDefaultFont()->PrintCenterX((192/2)-22,"The DS Card has been removed.\nPlease turn power OFF and reinsert\nthe DS Card before turning\npower back ON.");
  DSRender_SwapScreens();

  DSRender_StartFrame();
  FDResourceManager::GetDefaultFont()->PrintCenterX((192/2)-22,"The DS Card has been removed.\nPlease turn power OFF and reinsert\nthe DS Card before turning\npower back ON.");
  DSRender_SwapScreens();

  CARD_TerminateForPulledOut(); // termination
  // program halted. never reached here.
}

void PlatformSpecificInit()
{


}

void PlatformSpecificRenderInit()
{
  DSRender_SetCapturedRenderMode(true);  // Enable captured render mode
  DSRender_Init();
}

