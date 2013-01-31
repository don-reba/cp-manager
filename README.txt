FastTrack

The project consists of four packages: FastTrackAlg, FastTrackSvc, FastTrackD, and IpcLib. To use the project, place it in your 'cmtuser' directory. A sample folder structure might look like this:

cmtuser
  gaudi-gpu-manager
    cmt
    FastTrackAlg
    FastTrackD
    FastTrackSvc
    IncLib
    README.txt

All of the projects are built using CMT.

To run the server daemon, do:
cd FastTrackD/cmt
cmt config
source setup.sh
fasttrackd

To run the test algorithm, do:
gaudirun.py FastTrackAlg/options.test.py


FastTrackAlg

This is a test Gaudi algorithm making use of FastTrackSvc.


FastTrackSvc

This is a Gaudi service wrapping a FastTrackD client.


FastTrackD

The central server daemon interfacing with the GPU.


IpcLib

A shared inter-process communication library for FastTrackSvc and FastTrackD.


