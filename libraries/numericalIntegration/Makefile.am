ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils 

noinst_LTLIBRARIES =

if BUILD_PYTHON
noinst_LTLIBRARIES += libnumericalIntegration.la
endif

if BUILD_STATIC
libnumericalIntegration_la_LDFLAGS = -static
endif

libnumericalIntegration_la_CXXFLAGS = $(AM_CXXFLAGS) $(PYTHON_CXXFLAGS)

libnumericalIntegration_la_SOURCES = \
DEIntegrationConstants.h \
DEIntegrator.h \
bioComputeIntegral.cc \
bioComputeIntegral.h \
bioMori.h \
bioMoriFunction.h
