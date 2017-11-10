//----------------------------------------------------------------
// File: patArc.h
// Author: Michel Bierlaire
// Creation: Thu Oct 30 09:40:25 2008
//----------------------------------------------------------------

#ifndef patArc_h
#define patArc_h

#include <iostream>
#include <map>
#include <list>
#include "patGeoCoordinates.h"
#include "patError.h"
#include "patType.h"
#include "patString.h"
#include "patNode.h"

class patArc {

  friend class patNetwork ;
  friend class patNode ;
  friend class patPath ;
  friend class patLinkSubpath ;
  friend ostream& operator<<(ostream& str, const patArc& x) ;

 public:

  patArc(patULong theId, 
	 patNode* theUpNode, 
	 patNode* theDownNode, 
	 patString theName,
	 patError*& err) ;
  
  /**
     @param l length of the arc in meters
   */
  void setLength(patReal l) ;
  /**
     @return length of the arc in meters
   */
  patReal getLength() const ;

  /**
     @return patTRUE is the arc "follower" is actually consecutive to
     the current arc
   */
  patBoolean followedBy(patArc* follower, patError*& err) ;


  /**
   */
  patULong getUpNodeUserId() const ;
  /**
   */
  patULong getDownNodeUserId() const ;

  /**
   */
  patULong getInternalId() const ;

  /**
   */
  patULong getUserId() const ;

 public:
  patReal generalizedCost ;
  
 protected:
  patReal frozenGeneralizedCost ;
  patULong userId ;
  patULong internalId ;
  patULong upNodeId ;
  patULong downNodeId ;
  patString name ;
  map<patString,patReal> attributes ;
  patReal length ;
  list<patGeoCoordinates> polyline ;
} ;

#endif
