
set QTDIR=C:\Qt\4.7.3
set PATH=%QTDIR%\bin;%PATH%

del ..\..\bin\statebuilder-cpp.jar
del ..\..\bin\statebuildercpp.exe

call cmake.exe -G "Visual Studio 9 2008" ../../
call cmake.exe --build . --target Launch4jSetupWin
call cmake.exe --build . --target IzPackUnixPost