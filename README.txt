Coprocessor Manager
===================

The purpose of this project is in integration of accelerator hardware for
massively parallel computation with Gaudi. The Coprocessor Manager is a
process that runs outside of Gaudi and manages execution of dedicated
algorithms on the hardware. It is capable of receiving  multiple concurrent
data streams from Gaudi and other sources.


What's inside
-------------

Brunel v45r0 is shipped with this release, alongside a modified version of the
Fast VELO Pixel tracking algorithm running on the Coprocessor Manager. There
is  a number of supporting packages and several sample projects demonstrating
how to extend Coprocessor Manager yourself.


Build instructions
------------------

* To compile everything:

	. init.sh
	cd CpManager
	make

* To run the server daemon:

	. run_server.sh

* To run Brunel with the config file:

	. run_brunel_client.sh


Package Descriptions
--------------------

	Brunel_v46r0
	├── CUDA - cuda_library document type for using CUDA within CMT
	├── CpManager
	│   ├── CpAlgorithm     - Gaudi interface to SampleCpHandler
	│   ├── CpDriver        - plays recorded data back to the Coprocessor Manager
	│   ├── CpHandler       - base class for Coprocessor Manager handlers
	│   ├── CpIpc           - inter-process communication library
	│   ├── CpServer        - main Coprocessor Manager executable
	│   ├── CpService       - Gaudi service communicating with Coprocessor Manager
	│   └── SampleCpHandler - very simple Coprocessor Manager handler
	└── Pr
	    ├── PrPixel              - Gaudi algorithm for PrPixel
	    ├── PrPixelCuda          - Coprocessor Manager handler for PrPixel
	    ├── PrPixelSerialization - shared serialization code
	    └── PrPixelTest          - unit tests
