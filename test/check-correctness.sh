#!/bin/bash

# start the server
gpuserver &
sleep 0.5

# load our handler
gpuserver --load PrPixelCudaHandler
sleep 0.5

# test against recorded sample data
file=correctness-report.txt
rm $file
gpudriver --data ~/pr-data-100 --verify &> $file

# clean up
gpuserver --exit

# show the results
less correctness-report.txt
