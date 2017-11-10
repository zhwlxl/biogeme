
/**
There are 4 states.
Each transition probability is 1/4.
 */

#include "testState.h"
#include "bioMarkovProcess.h"
class patUniform ;

class testProcess: public bioMarkovProcess {

 public:
  testProcess(patUniform* rng, patULong firstState = 0 ) ;
  virtual bioMarkovTransition getNextState(bioMarkovState* cs,
					   patError*& err)  ;
  testState* getCurrentState()  ;
 private:
  vector<testState> theStates ;
  patULong currentState ;
  patUniform* theRng ;
  vector<patReal> equiProba ;
};
