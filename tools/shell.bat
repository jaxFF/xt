@echo off
set path=z:\qdbg\tools;%path%
call faster-vcvars.bat 64
if not exist z:\ (call mount_z.bat)
pushd ..\
wsl.exe -l | find /i "Windows"
if errorlevel 1 ( %comspec% /k "call wsl" )
popd