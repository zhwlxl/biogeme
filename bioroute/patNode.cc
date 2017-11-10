//----------------------------------------------------------------
// File: patNode.cc
// Author: Michel Bierlaire
// Creation: Thu Oct 30 10:51:19 2008
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patDisplay.h"
#include "patConst.h"
#include "patNode.h"
#include "patGeoCoordinates.h" 
#include "patStlSetIterator.h"

patNode::patNode(patULong theId, 
		 patString theName, 
		 patReal lat, 
		 patReal lon) :
  userId(theId),
  internalId(patBadId),
  name(theName),
  geoCoord(lat,lon),
  isCentroid(patFALSE) { 

}

void patNode::addSuccessor(patULong aSucc) {
  userSuccessors.insert(aSucc) ;
}

void patNode::addPredecessor(patULong aPred) {
  userPredecessors.insert(aPred) ;
}

ostream& operator<<(ostream& str, const patNode& x) {

  str << "Node " << x.userId << " [" << x.name << "] " 
      << x.geoCoord ;
  if (!x.userPredecessors.empty()) {
    str << " P(" ;
    for (set<patULong>::iterator i = x.userPredecessors.begin() ;
	 i != x.userPredecessors.end() ;
	 ++i) {
      if (i != x.userPredecessors.begin()) {
	str << "," ;
      }
      str << *i ;
    }
    str << ")" ;
  }
  if (!x.userSuccessors.empty()) {
    str << " P(" ;
    for (set<patULong>::iterator i = x.userSuccessors.begin() ;
	 i != x.userSuccessors.end() ;
	 ++i) {
      if (i != x.userSuccessors.begin()) {
	str << "," ;
      }
      str << *i ;
    }
    str << ")" ;
  }
  
  return str ;
}

patBoolean  patNode::disconnected() const {
  if (userSuccessors.size() > 0) {
    return patFALSE ;
  }
  if (userPredecessors.size() > 0) {
    return patFALSE ;
  }
  return patTRUE ;
}


patString patNode::getName() const {
  return name ;
}

patIterator<patULong>* patNode::getSuccessors() {
  patIterator<patULong>* ptr = new patStlSetIterator<patULong>(userSuccessors) ;
  return ptr ;
}

patULong patNode::getUserId() const {
  return userId ;
}

patULong patNode::getInternalId() const {
  return internalId ;
}

patBoolean operator<(const patNode& n1, const patNode& n2) {
  return (n1.internalId < n2.internalId) ;
}
