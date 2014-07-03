# echo "Setting GaudiKernel v30r0 in /home/abadalov/cmtuser/GPU/Brunel_v45r0"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=GaudiKernel -version=v30r0 -path=/home/abadalov/cmtuser/GPU/Brunel_v45r0  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

