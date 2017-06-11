ACLOCAL_AMFLAGS = -I m4

# if HAVE_DOXYGEN
# DOXYDIR=doc/doxygen
# endif

serialproxyconfdir = $(sysconfdir)
serialproxyconf_DATA = serialproxy.rc
EXTRA_DIST = $(serialproxyconf_DATA)

SUBDIRS=src $(DOXYDIR)
DIST_SUBDIRS=src $(DOXYDIR)
