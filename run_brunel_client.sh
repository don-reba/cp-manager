#!/bin/bash

. init.sh

cpserver --load PrPixelCudaHandler
sleep 0.1

gaudirun.py Brunel-Default.py

cpserver --exit
