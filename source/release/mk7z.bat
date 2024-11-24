@echo off

del footballbuild.7z
set COMPRESSION=-mx8 -mmt=on -mhe=on
rem set ARCHIVEPASSWORD=-pLeeds
set ARCHIVEPASSWORD=
cd pc
..\7z\7z.exe a %COMPRESSION% %ARCHIVEPASSWORD% -r -x!.svn* -x!.svn -x!.svn-* -x!7z -xr-!mk7z.bat -xr-!mkpc.bat ../footballbuild.7z *.*
cd..

