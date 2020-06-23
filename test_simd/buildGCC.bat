@echo off
CD /D %~dp0
set file=main
gcc -m64 -std=gnu18 -Wall -Wextra -O3 -msse4.2 -Wl,-subsystem,console %file%.c -o %file%.exe 1>NUL
IF %ERRORLEVEL% EQU 1 (timeout /t 3) ELSE (goto OK)
:OK
%file%.exe
ffplay -loglevel quiet -hide_banner -autoexit -f lavfi "amovie=sound.wav,asplit[a][out1];[a]showspectrumpic=s=480x270:gain=2,format=yuv420p[out0]"
del %file%.exe %file%.obj 
REM del sound.wav
