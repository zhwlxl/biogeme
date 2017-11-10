//----------------------------------------------------------------
// File: patArc.cc
// Author: Michel Bierlaire
// Creation: Thu Oct 30 10:53:34 2008
//----------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "patArc.h"
#include "patNode.h"

#include "patDisplay.h"
#include "patConst.h"
#include "patErrNullPointer.h"

patArc::patArc(patULong theId, 
	       patNode* theUpNode, 
	       patNode* theDownNode, 
	       patString theName,
	       patError*& err) :
  userId(theId),
  internalId(patBadId),
  name(theName),
  length(0.0),
  generalizedCost(0.0) {
  if ((theUpNode == NULL) || (theDownNode == NULL)) {
    err = new patErrNullPointer("patNode") ;
    WARNING(err->describe()) ;
    return ;
  }
  upNodeId = theUpNode->userId ;
  downNodeId = theDownNode->userId ;
  

}

void patArc::setLength(patReal l) {
  length = l ;
  generalizedCost = l ;
}

patReal patArc::getLength() const {
  return length ;
}

ostream& operator<<(ostream& str, const patArc& x) {
  str << "Arc " 
      << x.userId 
      << " [" 
      << x.name 
      << "](" 
      << x.upNodeId 
      << "->" 
      << x.downNodeId 
      << ")" ;
  str << " length=" 
      << x.length << "m; " ;
  for (map<patString,patReal>::const_iterator i = x.attributes.begin() ;
       i != x.attributes.end() ;
       ++i) {
    if (i != x.attributes.begin()) {
      str << "; " ;
    }
    str << i->first << "=" << i->second ;
  }
  return str ;
}

patBoolean patArc::followedBy(patArc* follower, patError*& err) {
  if (follower == NULL) {
    err = new patErrNullPointer("patArc") ;
    WARNING(err->describe()) ;
    return patFALSE ;
  }
  return (downNodeId == follower->upNodeId) ;
}


patULong patArc::getUpNodeUserId() const {
  return upNodeId ;
}

patULong patArc::getDownNodeUserId() const {
  return downNodeId ;
}

patULong patArc::getInternalId() const {
  return internalId ;
}

patULong patArc::getUserId() const {
  return userId ;
}
