# echo "Setting GpuServer v1r1 in /home/dcampora/git_projects/gaudi-gpu/gpu-manager/brunel-integration/Brunel_v44r3/GpuManager"

if test "${CMTROOT}" = ""; then
  CMTROOT=/sw/lib/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=GpuServer -version=v1r1 -path=/home/dcampora/git_projects/gaudi-gpu/gpu-manager/brunel-integration/Brunel_v44r3/GpuManager  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

