@echo off

setlocal EnableDelayedExpansion

REM Modified by: Jaxon "jaxne" Laing
REM See: https://github.com/desvandev/faster-vcvars

set _target_build=
IF [%1] == [32] set _target_build=x86
IF [%1] == [64] set _target_build=x64
IF [!_target_build!] == [] (
		ECHO Error: Target build must be either 32 or 64
		EXIT
	)

set "_path_vs=C:\Program Files (x86)\Microsoft Visual Studio\"
set "_path_winkit=C:\Program Files (x86)\Windows Kits\10\"

set _latest_vs=
set _latest_msvc=
set _latest_winkit=

set _lib_msvc_lib=
set _lib_ucrt=
set _lib_um=

set _libpath_metadata=
set _libpath_references=

set _include_path=
set _include_msvc_path=

set _new_paths=
set _new_lib=
set _new_libpath=
set _new_include=

FOR /f "tokens=*" %%G IN ('DIR /b /a:d /o:n "!_path_vs!" 2^>nul ^| findstr /R /C:"^20[1-2][0-9]$"') DO (
		set "_latest_vs=!_path_vs!%%G\"
	)

FOR /f "tokens=*" %%I IN ('DIR /b /a:d /o:n "!_latest_vs!Community\VC\Tools\MSVC\"') DO (
		set "_latest_msvc=!_latest_vs!Community\VC\Tools\MSVC\%%I\bin\HostX64\!_target_build!"
		set "_lib_msvc_lib=!_latest_vs!Community\VC\Tools\MSVC\%%I\lib\!_target_build!"
		set "_include_msvc_path=!_latest_vs!Community\VC\Tools\MSVC\%%I\include"
	)

FOR /f "tokens=*" %%H IN ('DIR /b /a:d /o:n "!_path_winkit!bin\" 2^>nul ^| findstr /R /C:"^[0-9][0-9][.].*"') DO (
		set "_latest_winkit=!_path_winkit!bin\%%H\x64"
		set "_lib_ucrt=!_path_winkit!lib\%%H\ucrt\!_target_build!"
		set "_lib_um=!_path_winkit!lib\%%H\um\!_target_build!"
		set "_libpath_metadata=!_path_winkit!UnionMetadata\%%H"
		set "_libpath_references=!_path_winkit!References\%%H"
		set "_include_path=!_path_winkit!include\%%H"
	)


set "DevEnvDir=!_latest_vs!Community\Common7\IDE\"

:: merging variable Path
set "_new_paths=!_latest_msvc!;"
set "_new_paths=!_new_paths!!_latest_winkit!;"
set "_new_paths=!_new_paths!!_path_winkit!bin\x64;"
set "_new_paths=!_new_paths!!_latest_vs!Community\\MSBuild\Current\Bin;"
set "_new_paths=!_new_paths!!DevEnvDir!;"
set "Path=!_new_paths!%Path%"

:: merging variable LIB
set "_new_lib=!_lib_msvc_lib!;"s
set "_new_lib=!_new_lib!!_lib_ucrt!;"
set "_new_lib=!_new_lib!!_lib_um!;"
set "LIB=!_new_lib!%LIB%"

:: merging variable LIBPATH
set "_new_libpath=!_lib_msvc_lib!;"
set "_new_libpath=!_new_libpath!!_lib_msvc_lib!\store\references;"
set "_new_libpath=!_new_libpath!!_libpath_metadata!;"
set "_new_libpath=!_new_libpath!!_libpath_references!;"
set "LIBPATH=!_new_libpath!%LIBPATH%"

:: merging variable INCLUDE
set "_new_include=!_include_msvc_path!;"
set "_new_include=!_new_include!!_include_path!\ucrt;"
set "_new_include=!_new_include!!_include_path!\shared;"
set "_new_include=!_new_include!!_include_path!\um;"
set "_new_include=!_new_include!!_include_path!\winrt;"
set "_new_include=!_new_include!!_include_path!\cppwinrt;"
set "INCLUDE=!_new_include!%INCLUDE%"

endlocal & set Path=%Path% & set LIB=%LIB% & set LIBPATH=%LIBPATH% & set INCLUDE=%INCLUDE%

exit /B