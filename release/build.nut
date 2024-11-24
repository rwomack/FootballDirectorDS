//
// Football Director Release Script for lBuild.
// -------------------------------------------
//

build.name   = "Football Director";
build.target = "Dev";

include("../../../lemon/build/scripts/svn.nut");
include("../../../lemon/build/scripts/utils.nut");
include("../../../lemon/build/scripts/fdlmake.nut");
include("../../../lemon/build/scripts/package.nut");
include("../../../lemon/build/scripts/versioninfo.nut");
include("../../../lemon/build/scripts/buildinfo.nut");

// --------------------------------------

// user: build.svndir
// user: build.releaseId -- auto set if not specified.

DefaultBuildProperty( "inPlace",         true );
DefaultBuildProperty( "versionInf",      "/source/version.inf" );
DefaultBuildProperty( "versionHeader",   "/source/game/include/version.h" );
DefaultBuildProperty( "releaseInfoFile", "/source/system/release.txt" );
DefaultBuildProperty( "releaseDir",      @"\\server1.fdl.local\store\builds\@external\sportsdirector\footballdirector" );
DefaultBuildProperty( "prepdir",         "package_prep" );

DefaultBuildProperty( "path7Zip",        path.Qualify(@"..\..\..\lemon\build\release\7z\") );
DefaultBuildProperty( "pathFDLMake",     path.Qualify(@"..\..\..\lemon\bin\win32\") );
DefaultBuildProperty( "pathSVN",         "" );

// --------------------------------------

function BuildTools( sourceTree )
{
  print("Building tools\n");

  local sourceDir = sourceTree + "/source/tools";

  local config = {}

  config.win32 <- {};
  config.win32.debug <- true;

  FDLMake( sourceDir, config );
}

// --------------------------------------

function BuildAssets( sourceTree )
{
  print("Building assets\n");
  local assetsDir = sourceTree + "/assets";
  exec( "buildall.bat", { shell=true, workingdir=assetsDir } );
}

// --------------------------------------

function BuildSource( sourceTree )
{
  print("Building source\n");

  local sourceDir = sourceTree + "/source";

  local config = {}

  config.win32 <- {};
  config.win32.release <- true;

  config.nds <- {};
  config.nds.release <- true;

  FDLMake( sourceDir, config );
}

// --------------------------------------

function ReleaseFile( info, releaseDir, fileName )
{
  local releasePath = releaseDir + "\\" + info.id;
  //don't include a date tag + "\\" + info.date_tag;

  mkdir( releasePath );

  copy( fileName, releasePath + "\\" + path.FileName(fileName) );
}

// --------------------------------------

function Build( bRelease )
{
  local versionInfo = ReadVersionInfo( path.Join( build.svndir, build.versionInf ) );
  local buildInfo   = CreateBuildInfo( build.svndir, GetVersionId(versionInfo), build.releaseId );

  // Log the build info

  LogBuildInfo( buildInfo );

  // Delete prep dir

  if( direxists( build.prepdir ) )
  {
    rmdir( build.prepdir );
  }

  mkdir( build.prepdir );

  // Store build info for later build steps

  WriteBuildInfo( buildInfo );

  // Get clean copy of source tree

  local sourceTree;

  if( build.inPlace )
  {
    print("Building in-place\n");
    sourceTree = build.svndir;
  }
  else
  {
    print("Building in exported copy\n");
    sourceTree = build.prepdir + "\\source_prep";
    SVNExport( build.svndir, sourceTree );
  }

  // Patch version number into the source tree
  // and write release file

  PatchBuildInfo( sourceTree, buildInfo )

  // Build the tools

  // No tools.
  //BuildTools( sourceTree );

  // Build the assets

  // Asset commited into SVN by devs.
  //BuildAssets( sourceTree );

  // Build the code

  BuildSource( sourceTree );

  // Package the build

  local packageName =  "football_director";

  // DS

  local srlName = format( "%s_v%03d_%04d.srl", packageName, versionInfo.external, versionInfo.internal );

  local ndsFileList = [];

  ndsFileList.push( "release.txt" );
  ndsFileList.push( { [srlName] = "main.srl" } );

  local ndsArchive = Package( build.prepdir, path.Join(sourceTree,"/source/system"), ndsFileList, packageName,
                              buildInfo.version, buildInfo.id, "ds", buildInfo.compressionFlags, buildInfo.compressionPassword );

  // PC

  local exeName = format( "%s_v%03d_%04d.exe", packageName, versionInfo.external, versionInfo.internal );

  local pcFileList = [];

  pcFileList.push( "release.txt" );
  pcFileList.push( { [exeName] = "main.exe" } );
  pcFileList.push( "nds" );

  local pcArchive = Package( build.prepdir, path.Join(sourceTree,"/source/system"), pcFileList, packageName,
                             buildInfo.version, buildInfo.id, "pc", buildInfo.compressionFlags, buildInfo.compressionPassword );


  // Revert the patched files info

  if( build.inPlace )
  {
    SVNRevert( sourceTree + build.versionHeader );
  }

  if( bRelease )
  {
    ReleaseFile(buildInfo,build.releaseDir,ndsArchive);
    ReleaseFile(buildInfo,build.releaseDir,pcArchive);

    // Increase the build number and commit it.

    versionInfo.internal += 1;

    WriteVersionInfo( versionInfo, path.Join( build.svndir, build.versionInf ) );

    // Commit

    SVNCommit( path.Join( build.svndir, build.versionInf ),
               format("* Increasing internal version number to %03d.%04d", versionInfo.external, versionInfo.internal ) );
  }
}

// --------------------------------------

function TargetDev()
{
  if( ! ("releaseId" in build) )
  {
    build.releaseId <- "dev";
  }

  Build( false );
}

function TargetDaily()
{
  if( ! ("releaseId" in build) )
  {
    build.releaseId <- "daily";
  }

  Build( true );
}

// --------------------------------------
// Register targets
// --------------------------------------

build.RegisterTarget( "Daily",   TargetDaily,   [] );
build.RegisterTarget( "Dev",     TargetDev,     [] );
