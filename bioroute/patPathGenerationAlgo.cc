//----------------------------------------------------------------
// File: patPathGenerationAlgo.cc
// Author: Michel Bierlaire
// Creation: Tue May 19 17:02:38 2009
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patDisplay.h"
#include "patLoopTime.h"
#include "patPathGenerationAlgo.h"
#include "patErrMiscError.h"

patPathGenerationAlgo::patPathGenerationAlgo(patString aName) : 
  algoName(aName),
  maximumNumberOfTrials(0) {

}

void patPathGenerationAlgo::setMaximumNumberOfTrials(patULong mnt) {
  maximumNumberOfTrials = mnt ;
}

void patPathGenerationAlgo::generateAllPaths(patNetwork* theNetwork, patOd aOd, patError*& err) {
  if (maximumNumberOfTrials == 0) {
    err = new patErrMiscError("The maximum number of trials must be specificed") ;
    WARNING(err->describe()) ;
    return ;
  }
  patLoopTime lt(maximumNumberOfTrials) ;
  for (patULong trial = 0 ; trial < maximumNumberOfTrials ; ++trial) {
    if (trial == 0) {
      lt.setIteration(trial) ;
    }
    if (trial % 100 == 0) {
      lt.setIteration(trial) ;
      DEBUG_MESSAGE(lt) ;
    }
    pair<patPath,patReal> thePath = generateNextPath(theNetwork,aOd,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return ;
    }
    map<patPath,pair<patReal,patULong> >::iterator found ;
    found = listOfPaths.find(thePath.first) ;
    if (found == listOfPaths.end()) {
      listOfPaths[thePath.first] = pair<patReal,patULong>(thePath.second,1) ;
    } 
    else {
      if (found->second.first != thePath.second) {
	stringstream str ;
	str << "Path generated with proba " << thePath.second << " and previously generated with proba " << found->second.first ;
	err = new patErrMiscError(str.str()) ;
	return ;
      }
      ++found->second.second ;
    }
    
  }
  return ;
}

patString patPathGenerationAlgo::getName() {
  return algoName ;
}
