
FastTrack - brunel-integration branch
=====================================

The FastTrack project provides GPU/TBB-accelerated routines for Gaudi algorithms.
The routines run on a central daemon process, called FastTrackD. This process
communicates with Gaudi clients via the FastTrackSvc Gaudi service.
There is a sample algorithm called FastTrackAlg demonstrating the use of this service.


What is there
-------------

Brunel v44r3 is shipped with this release, alongside with a modified version of the
Tf/PatPixel project (pixel VELO tracking).

A separate implementation of the pixel with TBB is included in the
tbbpixel_library folder.


Build instructions
------------------

* Compile Brunel

	. init.sh
	cd Brunel_v44r3
	make

* Open a new tab and execute the server daemon

	. init.sh
	cd Brunel_v44r3/FastTrackD/cmt
	cmt config
	. setup.sh
	fasttrackd

* In other tab, execute Brunel with the config file

	. init.sh
	gaudirun.py Brunel-SftPixel.py


Package Descriptions
--------------------

* Brunel_v44r3 - Brunel framework
	* FastTrackAlg - A test Gaudi algorithm making use of FastTrackSvc.
	* FastTrackD - The central server daemon handling the GPU.
	* FastTrackIpc - A shared interprocess communication library for FastTrackSvc and FastTrackD.
	* FastTrackRelease - A release package for building the whole project.
	* FastTrackSvc - A Gaudi service wrapping a FastTrackD client.
	* Tf/PatPixel - Modified implementation of pixel VELO tracking

* tbbpixel_library - Brunel-independent implementation of pixel library with TBB
