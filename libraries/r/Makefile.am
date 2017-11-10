# Makefile.am
# R interface

# Use the R compiling environment
CXX=$(RCXX)

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils  -I$(top_srcdir)/libraries/randomNumbers -I$(top_srcdir)/libraries/gaussHermite -I$(top_srcdir)/libraries/cfsqp -I$(top_srcdir)/libraries/ipopt -I$(top_srcdir)/libraries/solvopt -I$(top_srcdir)/libraries/linearAlgebra -I$(top_srcdir)/libraries/trustRegion $(PYTHON_INCLUDES) 

if BUILD_R
AM_CPPFLAGS += $(R_CXXFLAGS)
endif

#noinst_LTLIBRARIES = librinterface.la

librinterface_la_CXXFLAGS = $(AM_CXXFLAGS)
 
if BUILD_STATIC
noinst_LIBRARIES = librinterface.a
librinterface_a_SOURCES = $(librinterface_la_SOURCES)
librinterface_a_CXXFLAGS = $(AM_CXXFLAGS)
endif


librinterface_la_SOURCES = \
bioRInterface.cc \
bioRInterface.h

