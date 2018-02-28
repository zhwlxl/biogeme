# Makefile.am
# biogemedylib

include $(top_srcdir)/libraries.mk

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils  -I$(top_srcdir)/libraries/randomNumbers -I$(top_srcdir)/libraries/cfsqp -I$(top_srcdir)/libraries/ipopt -I$(top_srcdir)/libraries/solvopt -I$(top_srcdir)/libraries/linearAlgebra -I$(top_srcdir)/libraries/trustRegion -I$(top_srcdir)/libraries/pythonbiogeme $(PYTHON_INCLUDES)


lib_LTLIBRARIES = libbiogemedylib.la
libbiogemedylib_la_SOURCES = biogemedylib.cc
libbiogemedylib_la_LIBADD = $(PYTHON_SHARED_LIBS) $(COMMON_SHARED_LIBS) $(PYTHON_LIBS)
libbiogemedylib_la_LDFLAGS = $(AM_LDFLAGS) $(PYTHON_LDFLAGS)

if BUILD_IPOPT
libbiogemedylib_la_LDFLAGS += $(IPOPT_LDFLAGS)
endif




