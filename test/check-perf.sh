#!/bin/bash

oldLog=old-perf.log
newLog=perf.log

mv -f $newLog $oldLog 2> /dev/null

# start the server
gpuserver &
sleep 0.5

# load our handler
gpuserver --load PrPixelCudaHandler
sleep 0.5

echo sending data
gpudriver --data ~/pr-data-100

# clean up
gpuserver --exit

if [ -f $oldLog ]; then
	./compare-perf.py old-perf.log perf.log
else
	echo $newLog generated
fi
