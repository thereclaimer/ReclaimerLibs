::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

::----------------------------------------------------------------
:: VARIABLES
::----------------------------------------------------------------

::source file
@set path_source_file=               src\r-libs.cpp

::build path
@set path_build=                     build\debug
@set path_build_bin=                 %path_build%\bin
@set path_build_lib=                 %path_build%\lib
@set path_build_obj=                 %path_build%\obj
@set path_build_include=             %path_build%\include

::include directories
@set path_include=                   include
@set path_include_r_algorithms=      %path_include%\algorithms
@set path_include_r_common=          %path_include%\common
@set path_include_r_datastructures=  %path_include%\data-structures
@set path_include_r_math=            %path_include%\math
@set path_include_r_memory=          %path_include%\memory
@set path_include_r_string=          %path_include%\string
@set path_include_r_win32=           %path_include%\win32

::internal directories
@set path_internal=                  internal
@set path_internal_r_algorithms=     %path_internal%\algorithms
@set path_internal_r_common=         %path_internal%\common
@set path_internal_r_datastructures= %path_internal%\data-structures
@set path_internal_r_math=           %path_internal%\math
@set path_internal_r_memory=         %path_internal%\memory
@set path_internal_r_string=         %path_internal%\string
@set path_internal_r_win32=          %path_internal%\win32

::source directories
@set path_src=                       src
@set path_src_r_algorithms=          %path_src%\algorithms
@set path_src_r_common=              %path_src%\common
@set path_src_r_datastructures=      %path_src%\data-structures
@set path_src_r_math=                %path_src%\math
@set path_src_r_memory=              %path_src%\memory
@set path_src_r_string=              %path_src%\string
@set path_src_r_win32=               %path_src%\win32

::vcpkg install directories
@set path_vcpkg=                     vcpkg_installed
@set path_vcpkg_include=             %path_vcpkg%\x64-windows\include
@set path_vcpkg_lib=                 %path_vcpkg%\x64-windows\lib

::----------------------------------------------------------------
:: DEPENDENCIES
::----------------------------------------------------------------

::delete and recreate the build directories
if exist build rmdir /s /q build
mkdir %path_build_bin%
mkdir %path_build_lib%
mkdir %path_build_obj%
mkdir %path_build_include%

::make sure vcpkg dependencies are installed
if not exist %path_vcpkg% call vcpkg install

::copy all header files to the build directory
for /R %path_include%  %%f in (*.*) do xcopy "%%f" %path_build_include% /Y /Q
for /R %path_internal% %%f in (*.*) do xcopy "%%f" %path_build_include% /Y /Q
xcopy %path_vcpkg_include%\*.* %path_build_include% /S /I

::----------------------------------------------------------------
:: ARGUMENTS
::----------------------------------------------------------------

@set cl_flags=        /Zi ^
                      /LD ^
                      /MD ^
                      /O1

@set cl_output=       /Fe:%path_build_bin%\RLibs.dll ^
                      /Fd:%path_build_bin%\RLibs.pdb ^
                      /Fo:%path_build_obj%\RLibs.obj

@set cl_includes=     /I %path_build_include%        ^
                      /I %path_src_r_algorithms%     ^
                      /I %path_src_r_common%         ^
                      /I %path_src_r_datastructures% ^
                      /I %path_src_r_math%           ^
                      /I %path_src_r_memory%         ^
                      /I %path_src_r_string%         ^
                      /I %path_src_r_win32%

@set cl_source=       %path_source_file%

@set cl_link=         /link                                    ^
                      /LIBPATH:vcpkg_installed\x64-windows\lib ^
                      /IMPLIB:build\debug\lib\RLibs.lib

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

popd