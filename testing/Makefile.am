include $(top_srcdir)/libraries.mk
pkgdatdir = ${pkgdatadir}$(INSTALL_VERSION_SUFFIX)

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils  -I$(top_srcdir)/libraries/randomNumbers -I$(top_srcdir)/libraries/cfsqp -I$(top_srcdir)/libraries/solvopt -I$(top_srcdir)/libraries/linearAlgebra -I$(top_srcdir)/libraries/trustRegion -I$(top_srcdir)/libraries/pythonbiogeme -D__PYTHONBIOGEME=\"$(pkgdatdir)\" $(PYTHON_INCLUDES) 


testing_CXXFLAGS = $(AM_CXXFLAGS) 
testing_LDFLAGS = $(AM_LDFLAGS)
testing_LDADD = $(COMMON_SHARED_LIBS)
if BUILD_PYTHON
testing_CXXFLAGS += $(PYTHON_CXXFLAGS)
testing_LDFLAGS += $(PYTHON_LDFLAGS)
testing_LDADD += $(PYTHON_SHARED_LIBS) $(PYTHON_LIBS)
if APPLE
testing_LDFLAGS += -Wl,-stack_size -Wl,1000000
endif
endif

bin_PROGRAMS = testing

if BUILD_STATIC
testing_static_CXXFLAGS = $(testing_CXXFLAGS)
testing_static_SOURCES = $(testing_SOURCES)
testing_static_LDFLAGS = $(testing_LDFLAGS)
testing_static_LDADD = $(COMMON_STATIC_LIBS)
if BUILD_PYTHON
testing_static_LDADD += $(PYTHON_STATIC_LIBS) $(PYTHON_LIBS)
endif
if APPLE
testing_static_LDFLAGS += -Wl,-stack_size -Wl,1000000
endif
bin_PROGRAMS += testing_static
endif

if INSTALL_VERSIONED
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	for p in $(bin_PROGRAMS); do \
	mv -f $${p}$(EXEEXT) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) && \
	$(LN_S) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) $${p}$(EXEEXT); \
	done
endif


testing_SOURCES = \
testing.cc

