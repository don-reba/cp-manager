@echo off
setlocal

set root=%CD%
cd ..

:: back up the generated files with use content
for %%f in (FastTrackD\src\Api\*_impl.cpp) do copy "%%f" "%root%"

rmdir/s/q FastTrackIpc\FastTrackIpc\api
rmdir/s/q FastTrackIpc\src\api
rmdir/s/q FastTrackSvc\src\api
rmdir/s/q FastTrackD\src\api

xcopy/s IpcGen\api .
