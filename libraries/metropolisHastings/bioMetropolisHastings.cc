//-*-c++-*------------------------------------------------------------
//
// File name : bioMetropolisHastings.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 11:24:31 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patMath.h"
#include "patDisplay.h"
#include "bioMetropolisHastings.h"
#include "patErrNullPointer.h"
#include "bioMarkovProcess.h"
#include "bioMarkovTransition.h"
#include "bioMarkovState.h"
#include "patUniform.h"

bioMetropolisHastings::bioMetropolisHastings(bioMarkovProcess* aProcess, 
					     bioMarkovState* aState,
					     patULong simulationInterval)
  : theMarkovProcess(aProcess),
    theCurrentState(aState),
    theSimulationInterval(simulationInterval),
    theRandomNumberGenerator(NULL),
    numberOfAccept(0),
    numberOfReject(0) {
  
  ofstream deb("debugMH.lis",ios::out) ;
  deb.close() ;
}


bioMarkovState* bioMetropolisHastings::simulateNextState(patError*& err) {
  if (theMarkovProcess == NULL) {
    err = new patErrNullPointer("bioMarkovProcess") ;
    WARNING(err->describe()) ;
    return NULL ;
  }
  if (theCurrentState == NULL) {
    err = new patErrNullPointer("bioMarkovState") ;
    WARNING(err->describe()) ;
    return(NULL) ;
  }
  if (theRandomNumberGenerator == NULL) {
    err = new patErrNullPointer("patUniform") ;
    WARNING(err->describe()) ;
    return(NULL) ;
  }
  if (!theMarkovProcess->isStationary()) {
    theCurrentState = theMarkovProcess->runToStationarity(theCurrentState,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
  }
  patULong steps = 0 ;

  ofstream deb("debugMH.lis",ios::app) ;

  while (steps <= theSimulationInterval) {


    deb << theCurrentState->print() << '\t' ;

    bioMarkovTransition trans = theMarkovProcess->getNextState(theCurrentState,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    

    patReal Qij = trans.theProbaForward ;
    patReal Qji = trans.theProbaBackward ;
    patReal bi = trans.theCurrentState->getWeight(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }
    patReal bj = trans.theNextState->getWeight(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }

    patReal alpha = bj * Qji / (bi * Qij) ;

    patReal r = theRandomNumberGenerator->getUniform(err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return NULL ;
    }

    if (r < alpha) {
      deb << "A" << endl ;
      ++numberOfAccept ;
      theCurrentState = trans.theNextState ;
    }
    else {
      deb << "R" << endl ;
      ++numberOfReject ;
    }
    ++steps ;

    
  }
  deb.close() ;
  patReal den = numberOfAccept + numberOfReject ;
  patReal percAcc = 100.0 * numberOfAccept / den ;
  patReal percRej = 100.0 * numberOfReject / den ;
  //  DEBUG_MESSAGE("Accept: " << percAcc << " Reject: " << percRej) ;
  return theCurrentState ;

}

void bioMetropolisHastings::setRandomNumber(patUniform* aRndNumberGen) {
  theRandomNumberGenerator = aRndNumberGen ;
}
