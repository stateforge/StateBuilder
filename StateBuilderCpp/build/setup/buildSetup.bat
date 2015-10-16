
set QTDIR=C:\Qt\4.7.3
set PATH=%QTDIR%\bin;%PATH%

del ..\..\bin\statebuilder-cpp.jar
del ..\..\bin\statebuildercpp.exe

call cmake.exe  ../../
call cmake.exe --build . --target Launch4jSetupWin
call cmake.exe --build . --target IzPackUnixPost