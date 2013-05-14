
GpuManager - brunel-integration branch
=====================================

The GpuManager project provides GPU/TBB-accelerated routines for Gaudi algorithms.
The routines run on a central daemon process, called GpuServer. This process
communicates with Gaudi clients via the GpuService Gaudi service.
There is a sample algorithm called GpuAlgorithm demonstrating the use of this service.


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

	. run_server.sh

* In other tab, execute Brunel with the config file

	. run_brunel_client.sh


Package Descriptions
--------------------

* CodeGenerator - Code Generator for new algorithm signatures to be
* implemented on the GPU. Creates code to be inserted in GpuService.

* Brunel_v44r3 - Brunel framework
	* GpuManager
		* GpuAlgorithm - A test Gaudi algorithm making use of GpuService.
		* GpuServer - The central server daemon handling the GPU.
		* GpuIpc - A shared interprocess communication library for GpuService and GpuServer.
		* GpuService - A Gaudi service wrapping a GpuServer client.
	* Tf/PatPixel - Modified implementation of pixel VELO tracking.

* tbbpixel_library - Brunel-independent implementation of pixel library with TBB.
