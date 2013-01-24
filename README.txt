FastTrack

The project consists of three packages: FastTrackAlg, FastTrackSvc, and FastTrackD.


FastTrackAlg

This is a test Gaudi algorithm making use of FastTrackSvc.

To build, go to FastTrackAlg/cmt and execute 'cmt make'.

To run, execute 'gaudirun.py FastTrackAlg/options/test.py'.


FastTrackSvc

This is a Gaudi service wrapping a FastTrackD client.

To build, go to FastTrackSvc/cmt and execute 'cmt make'


FastTrackD

The central server daemon interfacing with the GPU.

To build, go to FastTrackSvc and execute:
  mkdir tmp
  cd tmp
  cmake ../src
  make

To run, execute 'FastTrackD/bin/fasttrackd'.
