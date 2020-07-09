@echo off 
setlocal

IF NOT EXIST tools\aetime.exe (
    pushd tools\aetime
    call build.bat
    popd
)

aetime --begin xt_build.aet

set ProjectDir=%~dp0
set BuildDir=%~dp0build

set BuildType=%1
set Compiler=%2

IF NOT "%1"=="" ( 
    IF "%BuildType%"=="msvc" (
        set Compiler=%BuildType%
        set BuildType=
    )

    IF "%BuildType%"=="clang" (
        set Compiler=%BuildType%
        set BuildType=
    )
)

IF "%BuildType%"=="" (set BuildType=debug)
IF "%Compiler%"=="" (set Compiler=clang)
IF "%BuildType%"=="clean" (goto clean)

set CommonWarningsMSVC=-wd4302 -wd4311 -wd4201 -wd4100 -wd4189 -wd4505 -wd4164 -wd4127 -wd4018 -wd4244 -wd4551
set CommonWarningsClang=-Wno-null-dereference -Wno-writable-strings -Wno-unused-variable -Wno-pointer-to-int-cast -Wno-pointer-bool-conversion -Wno-unused-command-line-argument -Wno-constant-logical-operand -Wno-unknown-pragmas -Wno-missing-braces -ferror-limit=2000 %CommonWarningsMSVC%
set CommonCompilerFlags=-DBUILD_WIN32=1 -I..\..\src -D_CRT_SECURE_NO_WARNINGS -fp:fast -fp:except- -nologo -Gm- -GR- -EHsc- -EHa- -Od -Oi -WX -W3 -FC -Zi -GS- -Gs9999999
set CommonLinkerFlags= -nologo -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib kernel32.lib shlwapi.lib Imm32.lib Xinput.lib Shell32.lib opengl32.lib Dbghelp.lib

IF "%BuildType%"=="release" (
    set CommonCompilerFlags=%CommonCompilerFlags% -O2
)

IF "%Compiler%"=="msvc" (
    echo Building with MSVC in %BuildType%
    set CompileCommand=cl
    set CommonCompilerFlags=-DCOMPILER_MSVC=1 %CommonCompilerFlags% %CommonWarningsMSVC%
) ELSE IF "%Compiler%"=="clang" (
    echo Building with Clang in %BuildType%
    set CompileCommand=clang-cl
    set CommonCompilerFlags=-DCOMPILER_CLANG=1 %CommonCompilerFlags% %CommonWarningsClang%
)

IF NOT EXIST %BuildDir% mkdir %BuildDir%
pushd %BuildDir%
del *.pdb > NUL 2> NUL
del *.map > NUL 2> NUL

echo Compilation flags:%CommonCompilerFlags% /link%CommonLinkerFlags%

REM -arch:IA32 required for 32-bit builds??
REM 32-bit launcher build
REM %CompileCommand%  %CommonCompilerFlags% ..\..\src\launcher\win32_launch.cpp ..\..\src\engine\msvc.c -Fmwin32_launch.map /link %CommonLinkerFlags% -subsystem:windows,5.1 platform.lib renderapi_gl.lib opengl32.lib mathlib.lib libfstd.lib
REM 64-bit launcher build
%CompileCommand% %CommonCompilerFlags% -I..\imgui -I..\imgui\examples ..\src\win32_main.cpp -MT -Fowin32_main.obj -Fmwin32_main.map /link %CommonLinkerFlags%

popd

aetime --end xt_build.aet

exit /b 0

:clean
echo Cleaning build directory: %BuildDir% 
echo Are you sure you want to continue [Y/n]? *ALL FILES WILL BE DELETED!!!*
set /P c=
if /I "%c%"=="N" (exit /b 0) else (
    echo Removing files in %BuildDir%...
    del /s /q %BuildDir%\*.*
)

exit /b 0