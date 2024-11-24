set SPRITEPACK=..\..\..\external\framework\bin\spritepack
rem set SPRITEPACK=..\..\..\..\..\framework\tools\spritepack\spritepack

%SPRITEPACK% ..\..\..\source\system\nds\ui.lsb -dsno *.png *.bmp layout\next_match_overlay.png
%SPRITEPACK% ..\..\..\source\system\nds\uiicon.lsb -dsno icons\generic_box.png icons\grad*.png icons\*.png icons\*.bmp

%SPRITEPACK% ..\..\..\source\system\nds\uilargekit.lsb -dsno -split 30 30 kits\largekits\shirts_30x30_FX3.png
%SPRITEPACK% ..\..\..\source\system\nds\uismallkit.lsb -dsno -split 16 16 kits\smallkits\shirts_16x16_FX3.png

%SPRITEPACK% ..\..\..\source\system\nds\uimenu.lsb -dsno menu\admin\*.png menu\club\*.png menu\comp\*.png menu\game\*.png menu\match\*.png

REM title bars
%SPRITEPACK% ..\..\..\source\system\nds\blue_bar.lsb -dsno layout\blue_bar.png
%SPRITEPACK% ..\..\..\source\system\nds\green_bar.lsb -dsno layout\green_bar.png
%SPRITEPACK% ..\..\..\source\system\nds\purple_bar.lsb -dsno layout\purple_bar.png
%SPRITEPACK% ..\..\..\source\system\nds\red_bar.lsb -dsno layout\red_bar.png
%SPRITEPACK% ..\..\..\source\system\nds\yellow_bar.lsb -dsno layout\yellow_bar.png

REM pitch
%SPRITEPACK% ..\..\..\source\system\nds\pitch_top.lsb -dsno layout\pitch_top.png

REM menubar
%SPRITEPACK% ..\..\..\source\system\nds\menu_bar.lsb -dsno layout\menu_bar.png
%SPRITEPACK% ..\..\..\source\system\nds\menu_bar_4.lsb -dsno layout\menu_bar_4.png
%SPRITEPACK% ..\..\..\source\system\nds\menu_bar_2.lsb -dsno layout\menu_bar_2.png
%SPRITEPACK% ..\..\..\source\system\nds\splitter.lsb -dsno layout\splitter.png
%SPRITEPACK% ..\..\..\source\system\nds\message_box_small.lsb -dsno layout\message_box_small.png
%SPRITEPACK% ..\..\..\source\system\nds\message_box_large.lsb -dsno layout\message_box_large.png

REM badges
%SPRITEPACK% ..\..\..\source\system\nds\badge.lsb -dsno badge\*.png

REM processing
%SPRITEPACK% ..\..\..\source\system\nds\processing.lsb -dsno processing\processing.png processing\*.png

REM processing
%SPRITEPACK% ..\..\..\source\system\nds\winlose.lsb -dsno winlose\*.png

REM Formations
%SPRITEPACK% ..\..\..\source\system\nds\formationblobs.lsb -dsno -split 12 12 formation12.png -dsno -split 16 16 formation16.png

%SPRITEPACK% ..\..\..\source\system\nds\anims.lsb -dsno -split 69 20 animations\corner.png -dsno -split 44 40 animations\freekick2.png -dsno -split 36 33 animations\penalty.png

%SPRITEPACK%  ..\..\..\source\system\nds\playerinfo.lsb -dsno playerinfo\generic_badge.png -dsno playerinfo\generic_player.png
%SPRITEPACK%  ..\..\..\source\system\nds\flags.lsb -dsno -split 24 16 playerinfo\flags_FX.png

call buildbg %SPRITEPACK%  activity_background
call buildbg %SPRITEPACK%  cup1
call buildbg %SPRITEPACK%  wl_back
call buildbg %SPRITEPACK%  background1
call buildbg %SPRITEPACK%  background2
call buildbg %SPRITEPACK%  background3
call buildbg %SPRITEPACK%  background4
call buildbg %SPRITEPACK%  background5

REM title text
%SPRITEPACK% ..\..\..\source\system\nds\en_titles.lsb -dsno title\en_*.png title\en_*.bmp

REM new cal2
%SPRITEPACK%  ..\..\..\source\system\nds\uical.lsb -dsno cal2\calendar.png -dsno -split 20 13 cal2\day.png -dsno -split 20 12 cal2\month.png -dsno -split 20 6 cal2\year.png


REM fitness/emotes
%SPRITEPACK%  ..\..\..\source\system\nds\uifaces.lsb -dsno -split 12 12 default.png

pause
