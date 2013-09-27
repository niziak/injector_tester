@echo off
rem C:\WinAVR-20081205\bin\

avrdude.exe -v -c usbasp  -y -p m8 -U flash:w:..\timer.hex
REM -U eeprom:w:..\sterownikIO.eep

REM ************************************************
REM * default fuses for M162
REM * lfuse = 0xEF	External Crystal 8Mhz Startup 16CK + 4,1ms (CKSEL=1111 SUT=10)
REM * hfuse = 0xD9	SPI en, BOOTSZ=00(default) (1024 @1C00)
REM * efuse = 0xFD	brown out detection at Vcc=1.8V
REM *
REM ************************************************
REM * ESAVE BIT + BOD at 2,7V
REM * -U lfuse:w:0xef:m -U hfuse:w:0xd1:m -U efuse:w:0xfb:m 
REM ************************************************
REM
REM avrdude.exe -v -c usbasp  -y -p m162 -U lfuse:w:0xef:m -U hfuse:w:0xd1:m -U efuse:w:0xfb:m
REM

IF ERRORLEVEL 0 GOTO EXIT1
pause
:EXIT1