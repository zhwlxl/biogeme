ACLOCAL_AMFLAGS = -I m4 
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils -I$(top_srcdir)/libraries/randomNumbers -I$(top_srcdir)/libraries/parameters -I$(top_srcdir)/libraries/cfsqp -I$(top_srcdir)/libraries/solvopt -I$(top_srcdir)/libraries/linearAlgebra -I$(top_srcdir)/libraries/trustRegion -I$(top_srcdir)/libraries/bisonbiogeme

#qtguibiogeme_CXXFLAGS = $(QT4_CFLAGS)
qtguibiogeme_CXXFLAGS = $(QT_CXXFLAGS)

#qtguibiogeme_LDADD = $(QT4_LIBS) $(shell pwd)/../libraries/libbiogeme.la
qtguibiogeme_LDADD = $(QT_LIBS) $(shell pwd)/../libraries/libbiogeme.la


qtguibiogeme_static_CXXFLAGS = $(qtguibiogeme_CXXFLAGS)
qtguibiogeme_static_LDFLAGS = -static -static-libgcc -static-libstdc++
qtguibiogeme_static_SOURCES = $(qtguibiogeme_SOURCES)
qtguibiogeme_static_QTSOURCES = $(qtguibiogeme_QTSOURCES)
#qtguibiogeme_static_LDADD = $(QT4_LIBS) $(shell pwd)/../libraries/bisonbiogeme/libbisonbiogemeqt.a  $(shell pwd)/../libraries/randomNumbers/librandomNumbersqt.a $(shell pwd)/../libraries/cfsqp/libcfsqpqt.a $(shell pwd)/../libraries/solvopt/libsolvoptqt.a $(shell pwd)/../libraries/trustRegion/libtrustRegionqt.a $(shell pwd)/../libraries/linearAlgebra/liblinearAlgebraqt.a   $(shell pwd)/../libraries/parameters/libparametersqt.a  $(shell pwd)/../libraries/utils/libutilsqt.a
qtguibiogeme_static_LDADD = $(QT_LIBS) $(shell pwd)/../libraries/bisonbiogeme/libbisonbiogemeqt.a  $(shell pwd)/../libraries/randomNumbers/librandomNumbersqt.a $(shell pwd)/../libraries/cfsqp/libcfsqpqt.a $(shell pwd)/../libraries/solvopt/libsolvoptqt.a $(shell pwd)/../libraries/trustRegion/libtrustRegionqt.a $(shell pwd)/../libraries/linearAlgebra/liblinearAlgebraqt.a   $(shell pwd)/../libraries/parameters/libparametersqt.a  $(shell pwd)/../libraries/utils/libutilsqt.a

if USE_QT
if BUILD_BISONBIOGEME
bin_PROGRAMS = qtguibiogeme
if BUILD_STATIC
bin_PROGRAMS += qtguibiogeme_static
endif
if INSTALL_VERSIONED
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	for p in $(bin_PROGRAMS); do \
	mv -f $${p}$(EXEEXT) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) && \
	$(LN_S) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) $${p}$(EXEEXT); \
	done
endif
endif
endif

moc-%.cc: %.h
	@MOC@ -o$@ $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(MOC_CPPFLAGS) $<

ui-%.h: %.ui
	@UIC@ -o $@ $<

qrc-%.cc: %.qrc
	@RCC@ -o $@ $<

qtguibiogeme_SOURCES = \
bioQtGui.cc \
bioQtMainWindow.cc \
bioQtMainWindow.h \
moc-bioQtMainWindow.cc 

