# Makefile.am
# libraries/heuristic

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils -I$(top_srcdir)/libraries/linearAlgebra  -I$(top_srcdir)/libraries/trustRegion 

noinst_LTLIBRARIES = libheuristic.la

libstochasticGradient_la_CXXFLAGS = $(AM_CXXFLAGS)
 
if BUILD_STATIC
noinst_LIBRARIES = libheuristic.a
libstochasticGradient_a_SOURCES = $(libheuristic_la_SOURCES)
libstochasticGradient_a_CXXFLAGS = $(AM_CXXFLAGS)
endif

libheuristic_la_SOURCES = \

