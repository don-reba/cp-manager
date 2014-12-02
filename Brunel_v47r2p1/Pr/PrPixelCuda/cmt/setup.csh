# echo "Setting PrPixelCuda v1r1 in /home/dcampora/projects/gaudi/gaudi-gpu-manager-dev/Brunel_v47r2p1/Pr"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=PrPixelCuda -version=v1r1 -path=/home/dcampora/projects/gaudi/gaudi-gpu-manager-dev/Brunel_v47r2p1/Pr  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

