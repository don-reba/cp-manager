@echo off
setlocal

:: generate the updated API
rmdir/s/q Api
bin\IpcGen Api.thrift -out Api

:: the Admin service is special, because GpuServer is its own client
del Api\GpuService\src\Api\AdminID.h
move Api\GpuService\src\Api\Admin.* Api\GpuServer\src\Api

:: remove old and busted
set root=%CD%
cd ..\Brunel_v44r3\GpuManager\

rmdir/s/q GpuIpc\GpuIpc\Api
rmdir/s/q GpuIpc\src\Api
rmdir/s/q GpuService\src\Api
rmdir/s/q GpuServer\src\Api

:: substitute in th new hotness
xcopy/s "%root%\Api" .
