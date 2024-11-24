mkdir auto_%1_%2_%3
xcopy pc\*.* auto_%1_%2_%3 /s /y
cd auto_%1_%2_%3
call main -autoplay -quitatend -team %1 -randomseed %2 -seasons %3 > log-stdout.txt
call ..\7z\7z.exe a log_%1_%2_%3.7z log.txt log-stdout.txt
del log.txt
del log-stdout.txt
cd..


