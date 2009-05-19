echo off

REM batch file for distribution of "simanity"
REM $Header: /comm/simanity/dist.bat 1     09/05/14 4:45 tsupo $

chmod -R +w dist

rm -Rf dist\src

mkdir dist\src
mkdir dist\src\include
mkdir dist\src\lib

copy *.c dist\src
copy *.h dist\src

copy Release\simanity.exe dist
copy Release\xmlRPC.dll dist

copy ..\xmlRPC\xmlRPC.h dist\src\include
copy ..\xmlRPC\Release\xmlRPC.lib dist\src\lib
