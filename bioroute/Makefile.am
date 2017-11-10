ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/libraries/utils  -I$(top_srcdir)/libraries/metropolisHastings -I$(top_srcdir)/libraries/randomNumbers `$(XMLWRAPPCONFIG) --cxxflags`

LIBS = `$(XMLWRAPPCONFIG) --libs`
bioroute_LDADD = $(shell pwd)/../libraries/utils/libutils.la $(shell pwd)/../libraries/metropolisHastings/libmetropolisHastings.la $(shell pwd)/../libraries/randomNumbers/librandomNumbers.la

bioroute_LDFLAGS = -static

if BUILD_BIOROUTE
bin_PROGRAMS = bioroute
endif
if INSTALL_VERSIONED
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	for p in $(bin_PROGRAMS); do \
	mv -f $${p}$(EXEEXT) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) && \
	$(LN_S) $${p}$(INSTALL_VERSION_SUFFIX)$(EXEEXT) $${p}$(EXEEXT); \
	done
endif

bioroute_SOURCES = \
bioMhCombinedTransition.cc \
bioMhDetourTransition.cc \
bioMhPathGenState.cc \
bioMhRandomWalkTransition.cc \
bioMhShuffleTransition.cc \
bioPathMetropolisHastings.cc \
bioStateRepository.cc \
patArc.cc \
patArcIterator.cc \
patComputeAngle.cc \
patLinkIterator.cc \
patLinkSubpath.cc \
patListSubpath.cc \
patMonteCarlo.cc \
patNetwork.cc \
patNode.cc \
patNodeIterator.cc \
patOd.cc \
patPath.cc \
patPathGenerationAlgo.cc \
patPathIterator.cc \
patPathListIterator.cc \
patRandomWalk.cc \
patReadNetworkFromXml.cc \
patReadUserInfoFromXml.cc \
patShortestPathAlgoDest.cc \
patShortestPathTreeDest.cc \
patSubpath.cc \
bioroute.cc

EXTRA_DIST = \
bioMhCombinedTransition.h \
bioMhDetourTransition.h \
bioMhPathGenState.h \
bioMhRandomWalkTransition.h \
bioMhShuffleTransition.h \
bioPathMarkovProcess.h \
bioPathMetropolisHastings.h \
bioStateRepository.h \
patArc.h \
patArcIterator.h \
patComputeAngle.h \
patLinkIterator.h \
patLinkSubpath.h \
patListSubpath.h \
patMonteCarlo.h \
patNetwork.h \
patNode.h \
patNodeIterator.h \
patOd.h \
patPath.h \
patPathGenerationAlgo.h \
patPathIterator.h \
patPathListIterator.h \
patRandomWalk.h \
patReadNetworkFromXml.h \
patReadUserInfoFromXml.h \
patShortestPathAlgoDest.h \
patShortestPathTreeDest.h \
patSubpath.h
