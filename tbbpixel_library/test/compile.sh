#!/bin/bash

ADD_RULES="-L/usr/share/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21 -ltbb"
g++ $ADD_RULES -L. -lpixeltbb test.cpp 

