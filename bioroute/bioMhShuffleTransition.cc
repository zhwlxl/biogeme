//-*-c++-*------------------------------------------------------------
//
// File name : bioMhShuffleTransition.cc
// Author :    Michel Bierlaire
// Date :      Wed Aug 11 15:01:03 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patMath.h"
#include "bioMhShuffleTransition.h"
#include "patDiscreteDistribution.h"
#include "patErrOutOfRange.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "patDisplay.h"
#include "bioMhPathGenState.h"
#include "patNetwork.h"
#include "bioMhDetourTransition.h"

bioMhShuffleTransition::bioMhShuffleTransition(patNetwork* n, 
					       patReal scale,
					       patUniform* rng) :
  currentState(NULL),
  theNetwork(n),
  theScale(scale),
  theRng(rng)  {

    theDetourProcess = new bioMhDetourTransition(theNetwork,
						 theScale,
						 theRng) ;
  
}
  
bioMarkovTransition bioMhShuffleTransition::getNextState(bioMarkovState* cs,
							 patError*& err) {
  if (theNetwork == NULL) {
    err = new patErrNullPointer("patNetwork") ;
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  if (cs == NULL) {
    err = new patErrNullPointer("bioMhPathGenState");
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  
  currentState = dynamic_cast<bioMhPathGenState*>(cs) ;

  patULong cIndex = currentState->getIndexOfNodeC(err) ;
  if (cIndex == patBadId) {
    stringstream str ;
    str << "Node " << *(currentState->nodeC) << " not found on path " << currentState->thePath << " between " << currentState->getUserU() << "and " << currentState->getUserV() ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }

  // Draw randomly the upstream and the downstream node. 

  patULong pathLength = currentState->thePath.listOfNodes.size() ;

  patULong newU ;
  patULong newV ;


  // Numbers of states for a given path
  // If c = 0 --->  pathLength - 1  possibilities for v
  // Otherwise -->  c  possibilities for u
  //                pathLength - c possibilities for v
  // Total  2 * pathLength - 1

  patReal prob ;
  if (cIndex == 0) {
    newU = 0 ;
    newV = drawRandomInteger.drawNumber(1,pathLength) ;
    
    // Probability

    prob = 1.0 / (patReal(pathLength)-1.0) ;
  }
  else {

    newU = drawRandomInteger.drawNumber(0,cIndex) ;
    newV = drawRandomInteger.drawNumber(cIndex,pathLength) ;
    
    prob = 1.0 / (patReal(cIndex) * patReal(pathLength-cIndex));
  }


  bioMhPathGenState* theIntermediaryState = 
    new bioMhPathGenState(theNetwork,
			  currentState->thePath,
			  newU,
			  newV,
			  currentState->nodeC,
			  currentState->theScaleDetour,
			  currentState->theScaleImportanceSampling,
			  err) ;


  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }

  
  bioMarkovTransition result ;

  result.name = "Shuffle" ;
  result.theCurrentState = currentState ;
  result.theNextState = theIntermediaryState ;
  result.theProbaForward = prob ; 
  result.theProbaBackward = prob ;

  collectStatistics(&result,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }

  ofstream f("states.log",ios::app) ;
  f << result.print() << endl ;
  f.close() ;

  return result ;
}

