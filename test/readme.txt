These tests expect to find previously recorded data at ~/pr-data-100. To record it, run:

  gpuserver --datadir ~/pr-data-100 &
  gaudirun.py Brunel-Default.py
  gpuserver --exit
