@echo off
CD /D %~dp0
set file=main
gcc -Wall -Wextra -pedantic-errors -O2 %file%.c -o %file%.exe 1>NUL
REM if source with errors print to terminal else play file
IF %ERRORLEVEL% EQU 1 (timeout /t 3) ELSE (goto OK)
:OK
echo building sucessful & echo.
%file%.exe
ffplay -hide_banner -autoexit -f lavfi "amovie=sound.wav,asplit[a][out1];[a]showspectrumpic=s=480x270:gain=2,format=yuv420p[out0]"
del %file%.exe %file%.obj
REM del sound.wav
