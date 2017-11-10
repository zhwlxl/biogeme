//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovProcess.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 14:42:48 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patDisplay.h"
#include "bioMarkovProcess.h"
#include "bioMarkovState.h"
#include "patErrMiscError.h"

bioMarkovProcess::bioMarkovProcess() : stationary(patFALSE), theWarmup(5000) {

}


bioMarkovState* bioMarkovProcess::runToStationarity(bioMarkovState* start,
						    patError*& err) {
  DEBUG_MESSAGE("Run until stationarity") ;
  WARNING("NAIVE IMPLEMENTATION. MUST BE IMPROVED") ;
  bioMarkovState* currentState = start ;
  for (patULong i = 0 ; i < theWarmup ; ++i) {
    if ((i % 1000) == 0) {
      DEBUG_MESSAGE("Iteration " << i) ;
    }
    bioMarkovTransition t = getNextState(currentState,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    currentState = t.theNextState ;
  }
  DEBUG_MESSAGE("Stationary process...") ;
  stationary = patTRUE ;
  return currentState ;
}

patBoolean bioMarkovProcess::isStationary() const {
  return stationary ;
}

void bioMarkovProcess::collectStatistics(bioMarkovTransition* theTransition,
					 patError*& err){
  patULong i = theTransition->theCurrentState->getIndicator(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }
  patULong j = theTransition->theNextState->getIndicator(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return ;
  }

  pair<patULong,patULong> ij(i,j) ;
  map<pair<patULong,patULong>,pair<patULong,patReal> >::iterator found =
    theStatistics.find(ij) ;
  if (found == theStatistics.end()) {
    theStatistics[ij] = pair<patULong,patReal>(1,theTransition->theProbaForward) ;
  }
  else {
    if (found->second.second != theTransition->theProbaForward) {
      stringstream str ;
      str << "Transition " << i << "-->" << j << ": different probabilities: " << found->second.second << " and " << theTransition->theProbaForward ;
      err = new patErrMiscError(str.str()) ;
      WARNING(err->describe()) ;
      return ;
    }
    ++found->second.first ;
  }
}




void bioMarkovProcess::printEmpiricalTransitionProba(patString fileName) {
  ofstream f(fileName.c_str()) ;
  map<patULong,patReal> total ;
  for (map<pair<patULong,patULong>,pair<patULong,patReal> >::const_iterator 
	 iter = theStatistics.begin() ;
       iter != theStatistics.end() ;
       ++iter) {
    total[iter->first.first] += patReal(iter->second.first) ;
  }
  f << "i\tj\tEmpir.\tTheor." << endl ;
  for (map<pair<patULong,patULong>,pair<patULong,patReal> >::const_iterator 
	 iter = theStatistics.begin() ;
       iter != theStatistics.end() ;
       ++iter) {
    f << iter->first.first << '\t'
      << iter->first.second << '\t'
      << iter->second.first / total[iter->first.first] << '\t'
      << iter->second.second << endl ;
  }
  
  f.close() ;


}

void bioMarkovProcess::setWarmup(patULong w) {
  theWarmup = w ;
}
