@echo off
setlocal

set root=%CD%
cd ..\Brunel_v44r3\GpuManager\

rmdir/s/q GpuIpc\GpuIpc\api
rmdir/s/q GpuIpc\src\api
rmdir/s/q GpuService\src\api
rmdir/s/q GpuServer\src\api

xcopy/s "%root%\api" .
