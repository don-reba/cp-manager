#!/bin/bash

. init.sh

gpuserver --load PrPixelCudaHandler
sleep 0.1

gaudirun.py Brunel-Default.py

gpuserver --exit
