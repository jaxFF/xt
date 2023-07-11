@echo off 
setlocal 
set path=%path%;Z:\xt\tools

pushd ..\

IF NOT EXIST tools\btime.exe ( pushd tools\btime && call build.bat && popd )
btime --begin xt_build.bld

set BuildDir=%~dp0..\build
set BuildType=%1
set Compiler=%2
IF NOT "%1"=="" ( 
    IF "%BuildType%"=="msvc" ( set "Compiler=%BuildType%" && set "BuildType=%2" )
    IF "%BuildType%"=="clang" ( set "Compiler=%BuildType%" && set "BuildType=%2" )
)

IF "%BuildType%"=="" (set BuildType=debug)
IF "%Compiler%"=="" (set Compiler=msvc)
IF "%BuildType%"=="clean" (goto clean)

set CommonWarningsMSVC=-wd4302 -wd4311 -wd4201 -wd4100 -wd4189 -wd4505 -wd4164 -wd4127 -wd4018 -wd4244 -wd4146
set CommonWarningsClang= -Wno-microsoft-include -Wno-writable-strings -Wno-unused-variable -Wno-pointer-to-int-cast -Wno-pointer-bool-conversion -Wno-unused-command-line-argument -Wno-constant-logical-operand -Wno-unknown-pragmas -Wno-missing-braces -Wno-varargs -Wno-unused-function -Wno-ignored-attributes %CommonWarningsMSVC%
set CommonCompilerFlags=-DBUILD_WIN32=1 -fp:fast -fp:except- -nologo -Gm- -GR- -EHsc- -EHa- -MP -Od -Oi -FC -Zi -GS- -Gs9999999 -WX -W3 -D_CRT_SECURE_NO_WARNINGS
set CommonLinkerFlags= -nologo -incremental:no -noexp -noimplib -opt:ref user32.lib opengl32.lib

IF "%BuildType%"=="release" ( set CommonCompilerFlags=%CommonCompilerFlags% -O2 )

IF "%Compiler%"=="msvc" ( 
    echo Building with MSVC in %BuildType% 
    set CompileCommand=cl && set CommonCompilerFlags=-DCOMPILER_MSVC=1 %CommonCompilerFlags% %CommonWarningsMSVC%
) ELSE IF "%Compiler%"=="clang" (
    echo Building with Clang in %BuildType% 
    set CompileCommand=clang-cl && set CommonCompilerFlags=-DCOMPILER_CLANG=1 %CommonCompilerFlags% %CommonWarningsClang%
)

echo Compilation flags:%CommonCompilerFlags% /link%CommonLinkerFlags%
IF NOT EXIST %BuildDir% mkdir %BuildDir%
pushd %BuildDir%

del *.pdb > NUL 2> NUL && del *.map > NUL 2> NUL

%CompileCommand% %CommonCompilerFlags% -I..\imgui -I..\imgui\examples ..\src\win32_main.cpp -Fowin32_main.obj -Fmwin32_main.map /link %CommonLinkerFlags%
btime --end xt_build.bld

popd
popd

exit /b 0

:clean
echo Cleaning build directory: %BuildDir% && echo Are you sure you want to continue [Y/n]? *ALL FILES WILL BE DELETED!!!*
set /P c=
if /I "%c%"=="N" ( exit /b 0 ) else ( echo Removing files in %BuildDir%... && del /s /q %BuildDir%\*.* )
