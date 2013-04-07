if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /sw/lib/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=PatPixel -version=v1r0 -path=/home/dcampora/git_projects/gaudi-gpu/Brunel_FastTrack_Branch/Brunel_v44r3/Tf $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

