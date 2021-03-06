AC_PREREQ(2.52)
AC_INIT(keyring_tool, 1.3)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(keyring_tool, 1.3)

PACKAGE_MAJOR=1
PACKAGE_MINOR=2
PACKAGE_VERSION=$PACKAGE_MAJOR.$PACKAGE_MINOR

AC_SUBST(PACKAGE_MAJOR)
AC_SUBST(PACKAGE_MINOR)
AC_SUBST(PACKAGE_VERSION)

AC_DEFINE_UNQUOTED(PACKAGE_MAJOR, "$PACKAGE_MAJOR",
                   [Major version of package])
AC_DEFINE_UNQUOTED(PACKAGE_MINOR, "$PACKAGE_MINOR",
                   [Minor version of package])
AC_DEFINE_UNQUOTED(PACKAGE_VERSION, "$PACKAGE_VERSION",
                   [Version of package])
AC_DEFINE_UNQUOTED(PACKAGE_STRING, "$PACKAGE $PACKAGE_VERSION",
                  [Full name and version of package])
AC_DEFINE_UNQUOTED(PACKAGE_URL, "://sourceforge.net/projects/gnomekeyringtool",
                  [URL of package])


dnl AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h stdio.h getopt.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([getpass strdup ])

dnl Checking for enable-maintainer_mode.
dnl AC_ARG_ENABLE(maintainer-mode,
dnl AC_HELP_STRING([--enable-maintainer-mode],
dnl               [Turn on debugging and very strict compile-time warnings]),
dnl               [
dnl                 if test "$enableval" = "yes" ; then
dnl                    enable_maintainer=yes
dnl                 fi
dnl               ])

dnl if test "$enable_maintainer_mode" = "yes" ; then
dnl    enable_debugging="yes"
dnl 
dnl    if test "$GCC" = "yes"; then
dnl      dnl  CFLAGS="$CFLAGS -Wpointer-arith -Wwrite-strings -Wshadow -Wall -Wmissing-prototypes -Wmissing-declarations"
dnl      CFLAGS="$CFLAGS -Wall -g"
dnl    fi
dnl fi

PKG_PROG_PKG_CONFIG

dnl Checking for Gnome-keyring
PKG_CHECK_MODULES(GNOMEKEYRING, gnome-keyring-1)
GNOMEKEYRING_PREFIX="`$PKG_CONFIG --variable=prefix gnome-keyring-1`"
old_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags glib-2.0`"
AC_SUBST(GNOMEKEYRING_CFLAGS)
AC_SUBST(GNOMEKEYRING_LIBS)
AC_SUBST(GNOMEKEYRING_PREFIX)

AC_MSG_RESULT(for gnome-keyring-1 installed in $GNOMEKEYRING_PREFIX)

AC_MSG_CHECKING(for gnome_keyring_change_password in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_change_password,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_CHANGE_PASSWORD, [1],
                   "Define to 1 if you have the 'gnome_keyring_change_password' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_unlock in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_unlock,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_UNLOCK, [1],
                   "Define to 1 if you have the 'gnome_keyring_unlock' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_create in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_create,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_CREATE, [1],
                   "Define to 1 if you have the 'gnome_keyring_create' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_delete in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_delete,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_DELETE, [1],
                   "Define to 1 if you have the 'gnome_keyring_delete' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_get_info in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_get_info,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_GET_INFO, [1],
                   "Define to 1 if you have the 'gnome_keyring_get_info' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_lock in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_lock,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
                [AC_DEFINE(HAVE_GNOME_KEYRING_LOCK, [1],
                   "Define to 1 if you have the 'gnome_keyring_lock' function.")
                 AC_MSG_RESULT(yes)
                ], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_set_default_keyring in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_set_default_keyring,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
          [AC_DEFINE(HAVE_GNOME_KEYRING_SET_DEFAULT_KEYRING, [1],
             "Define to 1 if you have the 'gnome_keyring_set_default_keyring' function.")
           AC_MSG_RESULT(yes)], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_get_default_keyring in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_get_default_keyring,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
          [AC_DEFINE(HAVE_GNOME_KEYRING_GET_DEFAULT_KEYRING, [1],
             "Define to 1 if you have the 'gnome_keyring_get_default_keyring' function.")
           AC_MSG_RESULT(yes)], AC_MSG_RESULT(no))

AC_MSG_CHECKING(for gnome_keyring_list_keyring_names in gnome-keyring.h)
AC_EGREP_HEADER(gnome_keyring_list_keyring_names,
                $GNOMEKEYRING_PREFIX/include/gnome-keyring-1/gnome-keyring.h,
          [AC_DEFINE(HAVE_GNOME_KEYRING_LIST_KEYRING_NAMES, [1],
             "Define to 1 if you have the 'GNOME_KEYRING_LIST_KEYRING_NAMES' function.")
           AC_MSG_RESULT(yes)], AC_MSG_RESULT(no))

dnl Checking for GLib
CPPFLAGS=$old_CPPFLAG
PKG_CHECK_MODULES(GLIB_2, glib-2.0)
AC_SUBST(GLIB_2_CFLAGS)
AC_SUBST(GLIB_2_LIBS)

AC_OUTPUT(Makefile)
