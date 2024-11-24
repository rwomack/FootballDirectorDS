void PlatformSpecificInit()
{
}

void PlatformSpecificRenderInit()
{
  // Init rendering
 lRender::Init();

  lrSurfaceDesc desc;

  if( FDConfig::GetPlatform() == FDConfig::PLATFORM_NDS )
  {
    desc.xRes = 256;
    desc.yRes = 384;
  }
  else if( FDConfig::GetPlatform() == FDConfig::PLATFORM_PSP )
  {
    desc.xRes = 480;
    desc.yRes = 272;
  }
  else if( FDConfig::GetPlatform() == FDConfig::PLATFORM_WII )
  {
    // Widescreen: 720x496
    // Standard:   640x496

    desc.xRes = FDConfig::IsWideScreenEnabled() ? 720 : 640;
    desc.yRes = 480;
  }
  else
  {
    // PC / Mac
    desc.xRes = 640;
    desc.yRes = 480;
  }

  desc.colourBits = 24;
  desc.depthBits  = 16;
  desc.caps       = FDConfig::IsWindowedModeEnabled() ? LR_SURFACE_CAPS_PC_WINDOWED : 0;
  lRender::SurfaceCreate(desc);

  if( FDConfig::PlatformNDS() )
  {
    lSpriteManager::EmulateDS();
  }

  lUIWidget::SetStylusInput( true );

  lRender::BufferClear( L_ARGB(0,255,0,0) );
  lRender::BufferSwap();
  lRender::BufferClear( L_ARGB(0,255,0,0) );
  lRender::BufferSwap();

}

