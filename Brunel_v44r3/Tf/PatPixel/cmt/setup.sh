# echo "Setting PatPixel v1r0 in /home/dcampora/git_projects/gaudi-gpu/Brunel_FastTrack_Branch/Brunel_v44r3/Tf"

if test "${CMTROOT}" = ""; then
  CMTROOT=/sw/lib/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=PatPixel -version=v1r0 -path=/home/dcampora/git_projects/gaudi-gpu/Brunel_FastTrack_Branch/Brunel_v44r3/Tf  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

