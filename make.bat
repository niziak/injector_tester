@echo off
SET PATH=n:/tools/WinAVR/utils/bin;%PATH%
n:/tools/WinAVR/utils/bin/make -j%NUMBER_OF_PROCESSORS% %1 %2 %3 %4 %5 %6
REM 

