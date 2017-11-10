//-*-c++-*------------------------------------------------------------
//
// File name : bioMhRandomWalkTransition.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 17:17:34 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "bioMhRandomWalkTransition.h"
#include "bioMhDetourTransition.h"
#include "bioMhPathGenState.h"
#include "patNetwork.h"

bioMhRandomWalkTransition::bioMhRandomWalkTransition(patNetwork* n,
						     patReal a, 
						     patReal b, 
						     patReal scale,
						     patUniform* rng) :
  currentState(NULL),
  theNetwork(n),
  theScale(scale),
  theRng(rng),
  theRandomWalkAlgo(a,b) {

  theDetourProcess = new bioMhDetourTransition(theNetwork,
					       theScale,
					       theRng) ;
}

bioMarkovTransition bioMhRandomWalkTransition::getNextState(bioMarkovState* cs,
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

  bioMhPathGenState* theOriginalState = currentState ;


  // First select a new "gate" (node C)

  bioMarkovTransition intermediate = theDetourProcess->getNextState(currentState,
								    err) ;

  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  currentState = dynamic_cast<bioMhPathGenState*>(intermediate.theNextState) ;
  // Generate a path with randomwalk between the upstream node and the
  // selected node.

  patULong userU = currentState->thePath.listOfNodes[currentState->u] ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }
  patULong userV = currentState->thePath.listOfNodes[currentState->v] ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  patULong userC = currentState->nodeC->getUserId() ;
  patOd theFirstPathOd(userU,userC) ;


  pair<patPath,patReal> firstPath = theRandomWalkAlgo.generateNextPath(theNetwork, theFirstPathOd,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  // Generate a path with randomwalk between the
  // selected node and the downstream node

  patOd theSecondPathOd(userC,userV) ;


  pair<patPath,patReal> secondPath = theRandomWalkAlgo.generateNextPath(theNetwork, theSecondPathOd,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }


  patPath newPath = firstPath.first ;
  newPath.pushBack(secondPath.first,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  // There may be a memory leak issue here, as this is not released anywhere.
  bioMhPathGenState* newState = currentState->insertPath(newPath,err) ;

  bioMarkovTransition result ;
  result.name = "RandWalk" ;
  result.theCurrentState = currentState ;
  result.theNextState = newState ;

  result.theProbaForward = intermediate.theProbaForward * firstPath.second * secondPath.second ;

  patULong u = currentState->u ;
  patULong v = currentState->v ;
  patULong cIndex = theOriginalState->getIndexOfNodeC(err) ;
  if (cIndex == patBadId) {
    stringstream str ;
    str << "Node " << *(theOriginalState->nodeC) << " not found on path " << theOriginalState->thePath << " between " << userU << "and " << userV ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  patPath subPathUC = theOriginalState->thePath.extractSubPath(u,cIndex,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  patReal probaUC =  theRandomWalkAlgo.probability(subPathUC,theNetwork,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }
  patPath subPathCV = theOriginalState->thePath.extractSubPath(cIndex,v,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  patReal probaCV =  theRandomWalkAlgo.probability(subPathCV,theNetwork,err) ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  result.theProbaBackward = intermediate.theProbaBackward * probaUC * probaCV ;
  if (err != NULL) {
    WARNING(err->describe());
    return bioMarkovTransition() ;
  }

  currentState = newState ;

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
