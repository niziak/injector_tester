@echo off
set COMPORT=COM25

REM replace space with leading zero
set MYTIME=%TIME: =0%
set SET_TIME_COMMAND=T%MYTIME:~0,2%%MYTIME:~3,2%%MYTIME:~6,2%E

echo "Setting COM port %COMPORT% speed"
mode %COMPORT%: BAUD=115200 PARITY=n DATA=8 STOP=1 rts=off dtr=off
sleep -m 1000
echo "Command "%SET_TIME_COMMAND% will be send to COM port %COMPORT%
set SET_TIME_COMMAND="T090919E"
echo %SET_TIME_COMMAND%> \\.\%COMPORT%
pause
