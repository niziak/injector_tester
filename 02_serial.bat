@echo off
IF "nizinski_w" == "%USER%" (
	START tools_win32\putty.exe -load "COM25 115"
) else (
	START tools_win32\putty.exe -serial com25 -sercfg 115200,8,n,1,N
)


