# echo "Setting PrPixelCuda v1r1 in /home/dcampora/projects/gaudi/gaudi-gpu-manager-dev/Brunel_v47r2p1/Pr"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=PrPixelCuda -version=v1r1 -path=/home/dcampora/projects/gaudi/gaudi-gpu-manager-dev/Brunel_v47r2p1/Pr  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

