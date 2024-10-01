::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

::----------------------------------------------------------------
:: VARIABLES
::----------------------------------------------------------------

@set path_bin_r_win32=      bin\debug-x64
@set path_include_r_common= r-common\include
@set path_include_vcpkg=    vcpkg_installed\x64-windows\include
@set path_libpath_vcpkg=    vcpkg_installed\x64-windows\lib

::----------------------------------------------------------------
:: DEPENDENCIES
::----------------------------------------------------------------

if not exist %path_bin_r_win32% (
    mkdir %path_bin_r_win32%
)

if not exist %path_include_vcpkg% (
    call vcpkg install
)

::----------------------------------------------------------------
:: ARGUMENTS
::----------------------------------------------------------------

@set cl_flags=        /c  ^
                      /Zi

@set cl_output=       /Fo:%path_bin_r_win32%\RWin32.obj ^
                      /Fd:%path_bin_r_win32%\RWin32.pdb


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
:: LINK
::----------------------------------------------------------------

call lib.exe /out:bin\debug-x64\RWin32.lib bin\debug-x64\RWin32.obj

::----------------------------------------------------------------
:: END
::----------------------------------------------------------------

popd