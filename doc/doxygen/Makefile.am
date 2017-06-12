docpkg = $(PACKAGE_TARNAME)-doxy-$(PACKAGE_VERSION).tar.gz

doc_DATA = $(docpkg)

# install only man pages for most important functions and
# data structures
man3_MANS = man/man3/serialproxy.h.3

$(docpkg): doxygen.stamp
	tar chof - html | gzip -9 -c >$@

doxygen.stamp: doxyfile
	$(DOXYGEN) $(DOXYFLAGS) $<
	echo Timestamp > $@

$(man3_MANS): doxygen.stamp

install-data-hook:
	cd $(DESTDIR)$(docdir) && tar xf $(docpkg)

uninstall-hook:
	cd $(DESTDIR)$(docdir) && rm -rf html

CLEANFILES = doxywarn.txt doxygen.stamp doxyfile doxygen.dox $(docpkg)

clean-local:
	rm -rf html latex man
