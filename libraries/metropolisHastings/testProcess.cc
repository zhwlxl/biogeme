#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patDisplay.h"
#include "testProcess.h"
#include "patDiscreteDistribution.h"

testProcess::testProcess(patUniform* rng, patULong firstState) : 
  bioMarkovProcess(),
  currentState(firstState),
  theRng(rng),
  theStates(4),
  equiProba(4,1.0) {

  vector<patReal> weights(4) ;
  weights[0] = 20.0 ;
  weights[1] = 8.0 ;
  weights[2] = 3.0 ;
  weights[3] = 1.0 ;
  
  for (patULong i = 0 ; i < 4; ++i) {
    testState s(i,weights) ;
    theStates[i] = s ;
  }

}

bioMarkovTransition testProcess::getNextState(bioMarkovState* cs,
					      patError*& err) {
  testState* theState = dynamic_cast<testState*>(cs) ;
  currentState = theState->theId ;
  bioMarkovTransition t ;
  t.theCurrentState = &(theStates[currentState]) ;
  patULong next = patDiscreteDistribution(&equiProba,theRng)() ;
  t.theNextState = &(theStates[next]) ;
  t.theProbaForward = 0.25 ;
  t.theProbaBackward = 0.25 ;
  currentState = next ;
  return t ;
}

testState* testProcess::getCurrentState() {
  return &(theStates[currentState]) ;
}
