@echo off
set COMPORT=COM25


avrdude.exe avrdude.conf -p atmega328p -P %COMPORT% -c arduino -U flash:w:main.hex
putty.exe -serial %COMPORT% -sercfg 115200,8,n,1,N
pause
