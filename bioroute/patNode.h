//----------------------------------------------------------------
// File: patNode.h
// Author: Michel Bierlaire
// Creation: Thu Oct 30 09:33:09 2008
//----------------------------------------------------------------

#ifndef patNode_h
#define patNode_h

#include <set>
#include "patGeoCoordinates.h"
#include "patType.h"
#include "patString.h"
#include "patIterator.h"


class patNode {

  friend class patDisconnectedNode ;
  friend class patNetwork ;
  friend class patArc ;
  friend ostream& operator<<(ostream& str, const patNode& x) ;
  friend patBoolean operator<(const patNode& n1, const patNode& n2) ;

 public:
  /**
     @param theId User ID of the node
     @param theName Name of the node
     @param lat Latitude of the node
     @param lon Longitude of the node
   */
  patNode(patULong theId, patString theName, patReal lat, patReal lon) ;

  patString getName() const ;


  patIterator<patULong>* getSuccessors() ;

  patULong getUserId() const ;

  patULong getInternalId() const ;
 protected:
  /**
     @param aSucc user ID of the successor to add
   */
  void addSuccessor(patULong aSucc) ;
  /**
     @param aPred user ID of the predecessor to add
   */
  void addPredecessor(patULong aPred) ;

  /**
   */
  patBoolean disconnected() const ;

  /**
   */
  patBoolean isCentroid ;
 protected:
  patULong userId ;
  patULong internalId ;
  patString name ;
  patGeoCoordinates geoCoord ;
  set<patULong> userPredecessors ;
  set<patULong> userSuccessors ;
  
} ;


#endif 
