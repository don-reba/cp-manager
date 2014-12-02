# echo "Setting GpuServer v1r1 in /home/dcampora/projects/gaudi/new-gaudi-gpu-manager-dev/Brunel_v47r2p1/GpuManager"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=GpuServer -version=v1r1 -path=/home/dcampora/projects/gaudi/new-gaudi-gpu-manager-dev/Brunel_v47r2p1/GpuManager  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

