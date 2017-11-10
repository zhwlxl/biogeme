//-*-c++-*------------------------------------------------------------
//
// File name : bioMarkovProcess.h
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 11:06:35 2010
//
//--------------------------------------------------------------------

#ifndef bioMarkovProcess_h
#define bioMarkovProcess_h

#include <map>
#include "bioMarkovTransition.h"
#include "patError.h"
#include "patConst.h"

/**
This virtual object represents a Markov process
*/



class bioMarkovProcess {

public:
  bioMarkovProcess() ;
  virtual bioMarkovTransition getNextState(bioMarkovState* cs,
					   patError*& err) = PURE_VIRTUAL ;
  virtual bioMarkovState* runToStationarity(bioMarkovState* start,
					    patError*& err) ;
  virtual patBoolean isStationary() const ;

  virtual void collectStatistics(bioMarkovTransition* theTransition,
				 patError*& err);

  virtual void printEmpiricalTransitionProba(patString fileName) ;

  virtual void setWarmup(patULong w) ;
protected:
  patBoolean stationary ;

  // First: pair of states
  // Second: number of transtions + theoretical proba
  map<pair<patULong,patULong>,pair<patULong,patReal> > theStatistics ;

  patULong theWarmup ;
};

#endif
