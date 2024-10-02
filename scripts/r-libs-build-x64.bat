::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

::----------------------------------------------------------------
:: VARIABLES
::----------------------------------------------------------------

@set path_build=            build\debug
@set path_include_r_common= r-common\include
@set path_include_vcpkg=    vcpkg_installed\x64-windows\include
@set path_libpath_vcpkg=    vcpkg_installed\x64-windows\lib

::----------------------------------------------------------------
:: DEPENDENCIES
::----------------------------------------------------------------

if not exist %path_build% (
    mkdir %path_build%
    mkdir %path_build%\bin
    mkdir %path_build%\include
    mkdir %path_build%\lib
    mkdir %path_build%\obj
)

if not exist %path_include_vcpkg% (
    call vcpkg install
)

xcopy %path_include_r_common%\* %path_build%\include /E /I /H /Y
xcopy r-win32\include\*         %path_build%\include /E /I /H /Y
xcopy include\*                 %path_build%\include /E /I /H /Y


::----------------------------------------------------------------
:: ARGUMENTS
::----------------------------------------------------------------

@set cl_flags=        /LD  ^
                      /Zi  ^
                      /O1

@set cl_output=       /Fe:%path_build%\bin\RLibs.dll ^
                      /Fo:%path_build%\obj\RLibs.obj ^
                      /Fd:%path_build%\bin\RLibs.pdb

@set cl_includes=     /I r-win32\src             ^
                      /I r-win32\include         ^
                      /I r-win32\internal        ^
                      /I %path_include_r_common% ^
                      /I %path_include_vcpkg%

@set cl_source=       r-win32\src\r-win32.cpp

@set cl_link=         /link        ^
                      /LIBPATH:vcpkg_installed\x64-windows\lib

@set cl_libs=         user32.lib   ^
                      advapi32.lib ^
                      gdi32.lib    ^
                      opengl32.lib ^
                      glew32.lib   ^
                      GlU32.Lib

::----------------------------------------------------------------
:: COMPILE
::----------------------------------------------------------------

call cl.exe       ^
    %cl_flags%    ^
    %cl_output%   ^
    %cl_includes% ^
    %cl_source%   ^
    %cl_link%     ^
    %cl_libs%

::----------------------------------------------------------------
:: END
::----------------------------------------------------------------

copy %path_build%\bin\RLibs.lib %path_build%\lib\

popd