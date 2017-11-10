//----------------------------------------------------------------
// File: bioPathMetropolisHastings.cc
// Author: Michel Bierlaire
// Creation: Wed Aug 11 17:19:20 2010
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "bioPathMetropolisHastings.h"
#include "patNetwork.h"
#include "bioMetropolisHastings.h"

bioPathMetropolisHastings::bioPathMetropolisHastings(patString name, 
						     patULong warmup,
						     patULong simulationInterval,
						     patReal a,
						     patReal b,
						     patReal scaleDetour,
						     patReal scaleImportanceSampling,
						     patULong seed) :
  patPathGenerationAlgo(name),
  kumaA(a),
  kumaB(b),
  rng(seed),
  theDetourProcess(NULL), 
  theShuffleProcess(NULL),
  theCombinedProcess(NULL),
  initialized(patFALSE),
  currentState(NULL),
  theScaleDetour(scaleDetour) ,
  theScaleImportanceSampling(scaleImportanceSampling),
  theWarmup(warmup) {
  
}

pair<patPath,patReal> bioPathMetropolisHastings::generateNextPath(patNetwork* aNetwork, 
							      patOd aOd, 
							      patError*& err) {



  if (aNetwork == NULL) {
    err = new patErrNullPointer("patNetwork") ;
    WARNING(err->describe()) ;
    return pair<patPath,patReal>() ;
  }
  if (!initialized) {

    // Take as first state the shortest path

    patBoolean success ;
    patPath shortestPath = aNetwork->getShortestPath(aOd,&success,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return pair<patPath,patReal>() ;
    }
    if (!success) {
      stringstream str ;
      str << "Problem with the computation of the shortest path for OD pair " << aOd ;
      err = new patErrMiscError(str.str()) ;
      WARNING(err->describe()) ;
      return pair<patPath,patReal>() ;
    }

    // Select the node abut in the middle of the path

    DEBUG_MESSAGE("Node in the middle of path: " << shortestPath) ;
    patULong node = shortestPath.listOfNodes.size() / 2 ;
    DEBUG_MESSAGE("Position: " << node) ;
    patULong userNode = shortestPath.listOfNodes[node] ;
    DEBUG_MESSAGE("USer Id: " <<userNode) ;
    patNode* theNode = aNetwork->getNodeFromUserId(userNode) ;
    DEBUG_MESSAGE("The node: " << *theNode) ;
    currentState = new bioMhPathGenState(aNetwork,
					 shortestPath,
					 0,
					 shortestPath.listOfNodes.size()-1,
					 theNode,
					 theScaleDetour,
					 theScaleImportanceSampling,
					 err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return pair<patPath,patReal>() ;
    }
    DEBUG_MESSAGE("First state: " << *currentState << " ID= " << currentState->getIndicator(err)) ;

    theShuffleProcess = new bioMhShuffleTransition(aNetwork,
						   theScaleDetour,
						   &rng) ;

    
    theRandomWalkProcess = new bioMhRandomWalkTransition(aNetwork,
							 kumaA,
							 kumaB,
							 theScaleDetour,
							 &rng) ;


    // For debugging purposes

//     theRandomWalkProcess->runToStationarity(currentState,err) ;
    
//     theRandomWalkProcess->printEmpiricalTransitionProba(patString("empirical.log")) ;
//     exit(0) ;



    theFullProcess = new bioMhCombinedTransition(theShuffleProcess,
						 theRandomWalkProcess,
						 0.5,
						 &rng) ;

    theFullProcess->setWarmup(theWarmup) ;

    theCoreAlgorithm = new bioMetropolisHastings(theFullProcess,
						 currentState) ;
    theCoreAlgorithm->setRandomNumber(&rng) ;
    initialized = patTRUE ;
  }



  bioMarkovState* newState = theCoreAlgorithm->simulateNextState(err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return pair<patPath,patReal>() ;
  }

  bioMhPathGenState* n = dynamic_cast<bioMhPathGenState*>(newState) ;

  
//   bioMhPathGenState* c = dynamic_cast<bioMhPathGenState*>(theTransition.theCurrentState) ;

//   // THIS IS FOR DEBUGGING PURPOSES. WE SIMPLY APPLY THE PROCESS. NO MH YET.

//   bioMarkovTransition theTransition = theFullProcess->getNextState(currentState,err) ;
//   if (err != NULL) {
//     WARNING(err->describe()) ;
//     return pair<patPath,patReal>() ;
//   }
  
//   bioMhPathGenState* c = dynamic_cast<bioMhPathGenState*>(theTransition.theCurrentState) ;
//   bioMhPathGenState* n = dynamic_cast<bioMhPathGenState*>(theTransition.theNextState) ;

// //   DEBUG_MESSAGE("**********************") ;
// //   DEBUG_MESSAGE("Current: " << *c) ;
// //   DEBUG_MESSAGE("Next: " << *n) ;
// //   DEBUG_MESSAGE("Forward proba: " << theTransition.theProbaForward) ;
// //   DEBUG_MESSAGE("Backward proba: " << theTransition.theProbaBackward) ;
// //   DEBUG_MESSAGE("**********************") ;

//   currentState = n ;
//   DELETE_PTR(c) ;
  return pair<patPath,patReal>(n->thePath,0.0) ;
}

patString bioPathMetropolisHastings::getReport() {
  
}


