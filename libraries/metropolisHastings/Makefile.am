ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils -I$(top_srcdir)/libraries/randomNumbers


noinst_LTLIBRARIES =

if BUILD_BISONBIOGEME
noinst_LTLIBRARIES += libmetropolisHastings.la
endif

if BUILD_GUI
noinst_LIBRARIES = libmetropolisHastingsgtk.a
endif

if BUILD_PYTHON
noinst_LTLIBRARIES += libmetropolisHastingspython.la
endif

if BUILD_STATIC
libmetropolisHastings_la_LDFLAGS = -static
libmetropolisHastingspython_la_LDFLAGS = -static
endif

libmetropolisHastingsgtk_a_SOURCES = $(libmetropolisHastings_la_SOURCES)
libmetropolisHastingsgtk_a_CXXFLAGS = $(AM_CXXFLAGS) $(GTK_CXXFLAGS)

libmetropolisHastingspython_la_SOURCES = $(libmetropolisHastings_la_SOURCES)
libmetropolisHastingspython_la_CXXFLAGS = $(AM_CXXFLAGS) $(PYTHON_CXXFLAGS)


libmetropolisHastings_la_SOURCES = \
bioMarkovProcess.cc \
bioMarkovState.cc \
bioMarkovTransition.cc \
bioMetropolisHastings.cc \
bioMarkovProcess.h \
bioMarkovState.h \
bioMarkovTransition.h \
bioMetropolisHastings.h 