@echo off
echo Has the pack file been rebuilt!
pause
call ..\..\tools\pspumdgenc\umdgenc /c foot.umi
mkdir built
call ..\..\tools\pspumdgenc\umdgenc /g foot.umi built
