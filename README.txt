FastTrack

The project consists of four packages: FastTrackAlg, FastTrackSvc, FastTrackD, and IpcLib. To use the project, copy its contents to a folder named  FastTrack in your DaVinci folder. A sample folder structure might look like this:

cmtuser
  DaVinci_v33r1p1
    FastTrack
      FastTrackAlg
      FastTrackD
      FastTrackSvc
      README.txt

All of the projects are built using CMT.

To run the server daemon, execute:
cd FastTrackD/cmt
cmt config
source setup.sh
fasttrackd

To run the test algorithm, execute:
gaudirun.py FastTrackAlg/options.test.py


FastTrackAlg

This is a test Gaudi algorithm making use of FastTrackSvc.


FastTrackSvc

This is a Gaudi service wrapping a FastTrackD client.


FastTrackD

The central server daemon interfacing with the GPU.


IpcLib

A shared inter-process communication library for FastTrackSvc and FastTrackD.


