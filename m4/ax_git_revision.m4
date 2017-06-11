# version information based on git annotated tags
# based on
# http://rem.eifzilla.de/archives/2012/08/17/beta-release-numbering-with-git
# many thanks

# This is to help aclocal find these macros, as it can't see m4_define.
AC_DEFUN([AX_GIT_REVISION], [m4_if([1])])

m4_define([git_revision], m4_esyscmd([git branch -v 2>/dev/null | awk '/^\* / {printf "%s",$3}']))
m4_define([git_revision_dec],
          m4_esyscmd_s([echo $((0x$(echo ]git_revision[|head -c 4)))]))

m4_define([git_recent_tag_version],
           m4_esyscmd_s([git describe --match '[0-9].*[0-9]' --abbrev=0]))
m4_define([git_betastring],
           m4_esyscmd_s([git describe --match '[0-9].*[0-9]' --long|\
                        awk -F- '$3!=0{print"-"$3}']))
m4_define([git_full_version],[git_recent_tag_version[]git_betastring])