set file=main
CD /D %~dp0
for /f "delims=" %%i in ('where /f vcvars64.bat') do (
%%i
cl /W4 /O2 /TC %file%.c /link /RELEASE /MACHINE:X64 /SUBSYSTEM:CONSOLE && %file%.exe
ffplay -loglevel quiet -hide_banner -autoexit -f lavfi "amovie=sound.wav,asplit[a][out1];[a]showspectrumpic=s=480x270:gain=2,format=yuv420p[out0]"
REM - - - - - - - - clean up
del %file%.exe %file%.obj sound.wav
timeout /t 3
)