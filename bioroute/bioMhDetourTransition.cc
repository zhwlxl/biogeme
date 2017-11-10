//-*-c++-*------------------------------------------------------------
//
// File name : bioMhDetourTransition.cc
// Author :    Michel Bierlaire
// Date :      Sun Aug 22 17:44:04 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patMath.h"
#include "bioMhDetourTransition.h"
#include "patDiscreteDistribution.h"
#include "patErrOutOfRange.h"
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "patDisplay.h"
#include "bioMhPathGenState.h"
#include "patNetwork.h"

bioMhDetourTransition::bioMhDetourTransition(patNetwork* n, 
					       patReal scale,
					       patUniform* rng) :
  currentState(NULL),
  theNetwork(n),
  theScale(scale),
  theRng(rng)  {
  
}
  
bioMarkovTransition bioMhDetourTransition::getNextState(bioMarkovState* cs,
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

  patULong pathLength = currentState->thePath.listOfNodes.size() ;

  patULong u = currentState->u ;
  patULong v = currentState->v ;

  patPath subpath = currentState->thePath.extractSubPath(u,v,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  

  // The probability to be drawn is given by a logit where the "utility" is 
  // abs(Length(u,v)  - (SP(u,c) + SP(c,v)))
  // that is the difference between 
  //  (i) the current length of the subpath between u and v, and
  //  (ii) the length of the new subpath after inserting c

  patULong userU = currentState->thePath.listOfNodes[u] ;
  patULong userV = currentState->thePath.listOfNodes[v] ;

  patReal lengthUV = subpath.getGeneralizedCost(theNetwork,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }
  
  patULong nNodes = theNetwork->nbrOfNodes() ;


  vector<patReal> util(nNodes) ;
  patReal maxUtil ;
  for (patULong internalId = 0 ;
       internalId < nNodes ;
       ++internalId) {
    
    patNode* theNode = theNetwork->getNodeFromInternalId(internalId) ;
    patULong userC = theNode->getUserId() ;

    patOd UC(userU,userC) ;
    patReal costUC = theNetwork->getShortestPathCost(UC,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return bioMarkovTransition() ;
    }
    patOd CV(userC,userV) ;
    patReal costCV = theNetwork->getShortestPathCost(CV,err) ;
    if (err != NULL) {
      WARNING(err->describe()) ;
      return bioMarkovTransition() ;
    }
    if (costUC == patMaxReal || costCV == patMaxReal) {
      util[internalId] = patMaxReal ;
    }
    else {
      util[internalId] = patAbs(lengthUV - costUC - costCV) ;
    }
    if (internalId == 0) {
      maxUtil = util[internalId] ;
    }
    else {
      if (maxUtil < util[internalId]) {
	maxUtil = util[internalId] ;
      }
    }
  }
  
  patReal sum = 0.0 ;
  for (patULong j = 0 ; j < util.size() ; ++j) {
    //    DEBUG_MESSAGE("Before [" << j << "]=" << util[j]) ;

    // If the path is not connected, the shortest pathcost is patMaxReal
    if (util[j] == patMaxReal) {
      util[j] = 0.0 ;
    }
    else {
      util[j] = 
	exp(-theScale * (util[j]-maxUtil)) ;
    }
    //    DEBUG_MESSAGE("Unorm. " << j << "=" << util[j]) ;
    sum += util[j] ;
  }
  for (patULong j = 0 ; j < util.size() ; ++j) {
    util[j] /= sum ;
  }
  pair<patULong,patReal> draw = patDiscreteDistribution(&util,theRng,patTRUE).getDrawWithProba() ;
  patULong selectedNode = draw.first ;
  patReal probaForSelectedNode = draw.second ;
  patNode* theSelectedNode = theNetwork->getNodeFromInternalId(selectedNode) ;
  if (theSelectedNode == NULL) {
    stringstream str ;
    str << "Node with internal Id " << selectedNode << " is unknown" ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()); 
    return bioMarkovTransition() ;
  }



  bioMhPathGenState* theNewState = new bioMhPathGenState(theNetwork,
							 currentState->thePath,
							 u,
							 v,
							 theSelectedNode,
							 currentState->theScaleDetour,
							 currentState->theScaleImportanceSampling,
							 err) ;


  if (err != NULL) {
    WARNING(err->describe()) ;
    return bioMarkovTransition() ;
  }

  bioMarkovTransition result ;
  result.name = "Detour" ;
  result.theCurrentState = currentState ;
  result.theNextState = theNewState ;
  result.theProbaForward = probaForSelectedNode ;
  result.theProbaBackward = util[selectedNode] ;

  ofstream f("states.log",ios::app) ;
  f << result.print() << endl ;
  f.close() ;


  return result ;
}



