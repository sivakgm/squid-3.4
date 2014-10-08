#
# The shell script helper requires wbinfo to be in the environment PATH.
# We can install anyway, but warn if the tool is missing
#
AC_PATH_PROG(WBINFO, wbinfo)
if test "x$WBINFO" = "x"; then
  AC_MSG_WARN([Samba wbinfo not found in default location. ext_wbinfo_group_acl may not work on this machine])
fi

# allow script install anyway when perl is present
if test "x$PERL" != "x" -a "x$POD2MAN" != "x"; then
  BUILD_HELPER="wbinfo_group"
fi
