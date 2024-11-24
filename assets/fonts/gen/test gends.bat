@echo off
lfontgen.exe ..\ds\font1.fnt /windows 8 "Century Gothic"
copy fontimage.tga ..\ds\font8.tga

lfontgen.exe ..\ds\font1.fnt /windows 12 "Calibri" /bold
copy fontimage.tga ..\ds\font10.tga

lfontgen.exe ..\ds\font1.fnt /windows 12 "Gautami"
copy fontimage.tga ..\ds\font12.tga

lfontgen.exe ..\ds\font1.fnt /windows 14 "Gautami" /bold 
copy fontimage.tga ..\ds\font14.tga

lfontgen.exe ..\ds\font1.fnt /windows 16 "Gautami" /bold 
copy fontimage.tga ..\ds\font16.tga

lfontgen.exe ..\ds\font1.fnt /windows 26 "Gautami" /bold 
copy fontimage.tga ..\ds\font26.tga

rem lfontgen.exe ..\ds\font1.fnt  /windows 22 "Impact" 
rem copy fontimage.tga ..\ds\fontim.tga



