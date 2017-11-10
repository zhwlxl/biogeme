//-*-c++-*------------------------------------------------------------
//
// File name : bioMhCombinedTransition.h
// Author :    Michel Bierlaire
// Date :      Fri Aug 13 09:40:25 2010
//
//--------------------------------------------------------------------

#ifndef bioMhCombinedTransition_h
#define bioMhCombinedTransition_h

#include "bioMarkovProcess.h"

class patUniform ;

/**
   This transition combines two other transitions with a given
   probability.
 */

class bioMhCombinedTransition : public bioMarkovProcess {

public:
  bioMhCombinedTransition(bioMarkovProcess* first, 
			  bioMarkovProcess* second, 
			  patReal probaFirst,
			  patUniform* rng) ;

  virtual bioMarkovTransition getNextState(bioMarkovState* currentState, 
					   patError*& err) ;

 private:
  bioMarkovProcess* theFirstProcess ;
  bioMarkovProcess* theSecondProcess ;
  patReal probaFirstTransition ;
  patUniform* theRng ;
};

#endif
