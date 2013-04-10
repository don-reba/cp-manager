FastTrack

The FastTrack project provides GPU-accelerated routines for Gaudi algorithms. The routines run on a central daemon process, called FastTrackD. This process communicates with Gaudi clients via the FastTrackSvc Gaudi service. There is a sample algorithm called FastTrackAlg demonstrating the use of this service.

To build the project, go to FastTrackRelease/cmt and run 'cmt broadcast cmt make'.

To run the server daemon:
  cd FastTrackD/cmt
  cmt config
  source setup.sh
  fasttrackd

To run the test algorithm:
  gaudirun.py FastTrackAlg/options.test.py


Package Descriptions

FastTrackAlg - A test Gaudi algorithm making use of FastTrackSvc.
FastTrackD - The central server daemon handling the GPU.
FastTrackIpc - A shared interprocess communication library for FastTrackSvc and FastTrackD.
FastTrackRelease - A release package for building the whole project.
FastTrackSvc - A Gaudi service wrapping a FastTrackD client.
