@echo off
REM Faster-vcvars rid of the nasty wait that I kept encountering specific to WSL
REM See: https://github.com/desvandev/faster-vcvars
call faster-vcvars.bat 64
wsl
exit