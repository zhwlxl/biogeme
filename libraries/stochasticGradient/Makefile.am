# Makefile.am
# libraries/stochasticGradient

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils -I$(top_srcdir)/libraries/linearAlgebra  -I$(top_srcdir)/libraries/trustRegion 

noinst_LTLIBRARIES = libstochasticGradient.la

libstochasticGradient_la_CXXFLAGS = $(AM_CXXFLAGS)
 
if BUILD_STATIC
noinst_LIBRARIES = libstochasticGradient.a
libstochasticGradient_a_SOURCES = $(libstochasticGradient_la_SOURCES)
libstochasticGradient_a_CXXFLAGS = $(AM_CXXFLAGS)
endif

libstochasticGradient_la_SOURCES = \
bioStochasticGradient.cc \
bioStochasticGradient.h \
bioIterativeFunction.h
