# echo "Setting GaudiKernel v30r0 in /home/abadalov/cmtuser/GPU/Brunel_v45r0"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=GaudiKernel -version=v30r0 -path=/home/abadalov/cmtuser/GPU/Brunel_v45r0  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

