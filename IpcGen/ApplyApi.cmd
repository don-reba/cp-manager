@echo off
setlocal

set root=%CD%

cd ..

:: lib
rmdir/s/q FastTrackIpc\FastTrackIpc\Api
mkdir FastTrackIpc\FastTrackIpc\api
copy IpcGen\api\lib\* FastTrackIpc\FastTrackIpc\api

:: client
rmdir/s/q FastTrackSvc\src\Api
mkdir FastTrackSvc\src\api
copy IpcGen\api\client\* FastTrackSvc\src\api

:: server
for %%f in (FastTrackD\src\Api\*_impl.cpp) do copy "%%f" "%root%"
rmdir/s/q FastTrackD\src\Api
mkdir FastTrackD\src\api
copy IpcGen\api\server\* FastTrackD\src\api
