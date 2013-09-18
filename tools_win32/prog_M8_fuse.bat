@echo off
REM ************************************************
REM * default fuses for M8
REM * lfuse = 0xE1	FUSE_SUT0 & FUSE_CKSEL3 & FUSE_CKSEL2 & FUSE_CKSEL1
REM * hfuse = 0xD9	FUSE_SPIEN & FUSE_BOOTSZ1 & FUSE_BOOTSZ0
REM ************************************************
REM
REM 2F D1 - eeprom preserve + ext 8Mhz crystal + brown detection + 4ms delay startup time
REM
REM avrdude.exe -v -c usbasp  -y -p m8 -U lfuse:w:0x2F:m -U hfuse:w:0xD1:m
REM

pause