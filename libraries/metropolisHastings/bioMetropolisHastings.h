//-*-c++-*------------------------------------------------------------
//
// File name : bioMetropolisHastings.h
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 11:13:35 2010
//
//--------------------------------------------------------------------

#ifndef bioMetropolisHastings_h
#define bioMetropolisHastings_h

#include "patType.h"
#include "patError.h"

class bioMarkovProcess ;
class bioMarkovState ;
class patUniform ;

/**
   Object implementing the Metropolis-Hastings algorithm
 */

class bioMetropolisHastings {


 public:
  /**
     Once the process is stationary, the simulated states will be
     selected every "simulationIntervals" steps of the process
   */
  bioMetropolisHastings(bioMarkovProcess* aProcess, 
			bioMarkovState* aState,
			patULong simulationInterval = 100) ;
  void setRandomNumber(patUniform* aRndNumberGen) ;
  bioMarkovState* simulateNextState(patError*& err) ;
 private:
  patUniform* theRandomNumberGenerator ;
  bioMarkovProcess* theMarkovProcess ;
  bioMarkovState* theCurrentState ;
  patULong theSimulationInterval ;
  patULong numberOfReject;
  patULong numberOfAccept;
  
};
#endif
