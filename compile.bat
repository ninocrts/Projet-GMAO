<<<<<<< HEAD
=======
%comspec% /c "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
>>>>>>> master
cl.exe /Zi /EHsc /Ivcpkg_installed/x64-windows/include /Fe:server.exe server.c /link vcpkg_installed/x64-windows/lib/websockets.lib 