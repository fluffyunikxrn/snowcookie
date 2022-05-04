@ECHO OFF
SET app=".\vendor\bin\premake\premake5.exe"
SET args="vs2022"
CALL %app% %args%
PAUSE
EXIT