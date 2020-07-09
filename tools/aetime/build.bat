@echo off
REM -Zi for debugging
cl -nologo -FC -O2 -TC -EHsc aetime.c /link
@if ERRORLEVEL 1 exit /B 1
robocopy . ../ aetime.exe > nul