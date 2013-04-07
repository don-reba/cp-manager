#!/bin/bash

export CMTCONFIG=x86_64-slc5-gcc46-opt
export User_release_area=`pwd`
source /sw/lib/LbLogin.sh
SetupProject Brunel v44r3 --build-env

# getpack Tf/PatPixel  # we provide our own.
# getpack Sft/FstTools # needed?

# run with
# gaudirun.py Brunel-SftPixel.py
