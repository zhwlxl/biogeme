//-*-c++-*------------------------------------------------------------
//
// File name : bioMhPathGenState.cc
// Author :    Michel Bierlaire
// Date :      Mon Aug  9 16:25:39 2010
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patErrNullPointer.h"
#include "patErrMiscError.h"
#include "patErrOutOfRange.h"
#include "patNetwork.h"
#include "bioMhPathGenState.h"
#include "patArc.h"
#include "bioStateRepository.h"

bioMhPathGenState::bioMhPathGenState(patNetwork* n, 
				     patPath p, 
				     patULong _u, 
				     patULong _v, 
				     patNode* aNode,
				     patReal scaleDetour,
				     patReal scaleImportanceSampling,
				     patError*& err) :
  theNetwork(n),
  thePath(p),
  u(_u),
  v(_v),
  nodeC(aNode),
  theScaleDetour(scaleDetour),
  theScaleImportanceSampling(scaleImportanceSampling),
  weight(0.0),
  bioMarkovState() {

  weight = thePath.computeDeviationFromSpPath(theNetwork,
					      theScaleImportanceSampling,
					      err) ;


}

patReal bioMhPathGenState::getWeight(patError*& err) const {
  // Numbers of states for a given path
  // If c = 0 --->  pathLength - 1  possibilities for v
  // Otherwise -->  c  possibilities for u
  //                pathLength - c possibilities for v
  // Let pathLength = L 
  // Total: (L-1) + sum_{c=1}^{L-1} c(L-c) = (L-1)(L^2+L+6)/6 
  
  patReal L = thePath.listOfNodes.size() ;

  return 6.0 * weight / ((L-1)*(L*L+L+6)) ;
}

patPath bioMhPathGenState::getSubpath(patError*& err) {
  patPath sp = thePath.extractSubPath(u,v,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patPath() ;
  }
  return sp ;
}


bioMhPathGenState* bioMhPathGenState::insertPath(patPath newSubpath,
						 patError*& err) {

  patPath newPath(thePath) ;
  newPath.overwrite(newSubpath,u,v,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }
  if (!theNetwork->isPathValid(newPath)) {
    stringstream str ;
    str << "Invalid path: " << newPath ;
    err = new patErrMiscError(str.str()) ;
    WARNING(err->describe()) ;
    return NULL ;
  }

  patULong newV = u + newSubpath.listOfNodes.size() - 1;
  bioMhPathGenState* newState = new bioMhPathGenState(theNetwork,
						      newPath,
						      u,
						      newV,
						      nodeC,
						      theScaleDetour,
						      theScaleImportanceSampling,
						      err) ;

  if (err != NULL) {
    WARNING(err->describe()) ;
    return NULL ;
  }

  return newState ;


}

patULong bioMhPathGenState::getIndicator(patError*& err) const {
  patULong theId = bioStateRepository::the()->addState(*this,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patBadId ;
  }
  return theId ;


}


ostream& operator<<(ostream &str, const bioMhPathGenState& x) {
  str << "[" << x.thePath.listOfNodes[x.u] << "("<<x.u<<")," << x.thePath.listOfNodes[x.v] << " ("<<x.v<<")]["<<*x.nodeC<<"]:" << x.thePath ;
  return str ;
}



patULong bioMhPathGenState::getIndexOfNodeC(patError*& err) {
  thePath.getNodeIndex(nodeC,u,v+1,err) ;
  if (err != NULL) {
    WARNING(err->describe()) ;
    return patBadId ;
  }
}

patULong bioMhPathGenState::getUserU() const {
  if (u >= thePath.listOfNodes.size()) {
    return patBadId ;
  }
  return thePath.listOfNodes[u] ;
}

patULong bioMhPathGenState::getUserV() const {
  if (v >= thePath.listOfNodes.size()) {
    return patBadId ;
  }
  return thePath.listOfNodes[v] ;
}

patBoolean operator<(const bioMhPathGenState& s1, const bioMhPathGenState& s2) {
  if (s1.thePath < s2.thePath) {
    return patTRUE ;
  }
  
  if (s2.thePath < s1.thePath) {
    return patFALSE ;
  }

  if (s1.u < s2.u) {
    return patTRUE ;
  }

  if (s1.u > s2.u) {
    return patFALSE ;
  }

  if (s1.v < s2.v) {
    return patTRUE ;
  }

  if (s1.v > s2.v) {
    return patFALSE ;
  }
  return (*s1.nodeC < *s2.nodeC) ;
}

pair<patBoolean,patString> bioMhPathGenState::isValidState() const {
  patBoolean ok = patTRUE ;
  stringstream str ;
  if (getUserU() != thePath.od.orig) {
    str << "Error in origin: " << getUserU() << "<>" << thePath.od.orig << endl ;
    ok = patFALSE ;
  }
  if (getUserV() != thePath.od.dest) {
    str << "Error in dest: " << getUserV() << "<>" << thePath.od.dest << endl ;
    ok = patFALSE ;
  }
  if (!thePath.contains(nodeC->getUserId())) {
    str << "Error: node " << nodeC->getUserId() << " not in path " << thePath << endl ;
    ok =  patFALSE ;
  }
  return pair<patBoolean,patString>(ok,patString(str.str())) ;
}

patString bioMhPathGenState::print() const {
  stringstream str ;
  str << thePath ;
  return patString(str.str()) ;
}
