::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

call link.exe                                ^
    /DLL                                     ^
    /OUT:RLibs.dll                           ^
    /LIBPATH:build\debug-x64\lib             ^
    /LIBPATH:vcpkg_installed\x64-windows\lib ^
    user32.lib                               ^
    opengl32.lib                             ^
    glew32.lib                               ^
    gdi32.lib                                ^
    imgui.lib                                ^
    Xinput.lib

::----------------------------------------------------------------
:: END
::----------------------------------------------------------------

popd