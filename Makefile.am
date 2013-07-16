# To support AC_CHECK_DEFINED
ACLOCAL_AMFLAGS = -I m4

# Since everything we compile requires the same headers,
# we define the global AM_CPPFLAGS. We also require the
# C++11 standard to be able to use std::shared_ptr.
AM_CPPFLAGS = -I$(srcdir)/include

AUTOMAKE_OPTIONS = foreign

# Always include gtest in distributions.
DIST_SUBDIRS = gtest

if HAVE_DOXYGEN

doc:
	$(DOXYGEN) $(top_srcdir)/doc/Doxyfile

endif

# The following local target definition is copied from the Protobuf project:
#   Build gtest before we build our tests. We don't add gtest to SUBDIRS
#   because then "make check" would also build and run all of gtest's own tests,
#   which takes a lot of time and is generally not useful to us. Also, we don't
#   want "make install" to recurse into gtest since we don't want to overwrite
#   the installed version of gtest if there is one.
check-local:
	@echo "Making lib/libgtest.a lib/libgtest_main.a in gtest"
	@cd gtest && $(MAKE) $(AM_MAKEFLAGS) lib/libgtest.la lib/libgtest_main.la

# We need to create a new custom target to run the tests because Autoconf does
# not reliably add check-local before the check target.
test: check-local check

.PHONY: doc

# The following local target definition is copied from the Protobuf project:
#   We would like to clean gtest when "make clean" is invoked. But we have to
#   be careful because clean-local is also invoked during "make distclean", but
#   "make distclean" already recurses into gtest because it's listed among the
#   DIST_SUBDIRS. distclean will delete gtest/Makefile, so if we then try to
#   cd to the directory again and "make clean" it will fail. So, check that the
#   Makefile exists before recursing.
clean-local:
	@if test -e gtest/Makefile; then \
	  echo "Making clean in gtest"; \
	  cd gtest && $(MAKE) $(AM_MAKEFLAGS) clean; \
	fi

dist_doc_DATA = README.md

# Build rules for libraries.
lib_LTLIBRARIES = lib/libsup.la

lib_libsup_la_SOURCES = \
  src/sup_plugin.cpp

pkginclude_HEADERS = \
  include/sup_plugin.h \
  include/log_sup_plugin.h \
  include/linear_sup_plugin.h

TESTS = test/sup
check_PROGRAMS = test/sup

test_sup_SOURCES = \
  test/log_sup_plugin_test.cpp \
  test/linear_sup_plugin_test.cpp

test_sup_CPPFLAGS = -I$(srcdir)/include -I$(top_srcdir)/gtest/include
test_sup_LDADD = $(top_builddir)/gtest/lib/libgtest.la \
  $(top_builddir)/gtest/lib/libgtest_main.la \
  lib/libsup.la
