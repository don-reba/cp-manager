@echo off
setlocal

set root=%CD%
cd ..\Brunel_v44r3\GpuManager\

:: back up the generated files with use content
for %%f in (GpuServer\src\api\*_impl.cpp) do copy "%%f" "%root%"

rmdir/s/q GpuIpc\GpuIpc\api
rmdir/s/q GpuIpc\src\api
rmdir/s/q GpuService\src\api
rmdir/s/q GpuServer\src\api

xcopy/s "%root%\api" .
